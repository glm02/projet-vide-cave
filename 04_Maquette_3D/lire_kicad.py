"""Extraction en lecture seule de l'implantation KiCad fournie."""
import json
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parents[1]
SOURCE = ROOT / '03_Electronique' / 'G206A_CAMEL_TP_CAO' / 'G206_CAMEL_TP_CAO.kicad_pcb'

def parse(text):
    stack = []
    root = None
    for token in re.findall(r'"(?:\\.|[^"\\])*"|[()]|[^\s()]+', text):
        if token == '(':
            node = []
            if stack:
                stack[-1].append(node)
            else:
                root = node
            stack.append(node)
        elif token == ')':
            stack.pop()
        else:
            stack[-1].append(json.loads(token) if token.startswith('"') else token)
    return root

def field(node, key):
    return next((item[1:] for item in node if isinstance(item, list) and item and item[0] == key), [])

def run():
    root = parse(SOURCE.read_text(encoding='utf-8'))
    data = {'source': str(SOURCE.relative_to(ROOT)), 'footprints': [], 'segments': [], 'outline': [100,70,190,150]}
    for node in root:
        if not isinstance(node, list) or not node:
            continue
        if node[0] == 'gr_rect' and field(node, 'layer') == ['Edge.Cuts']:
            data['outline'] = [*map(float, field(node,'start')), *map(float,field(node,'end'))]
        if node[0] == 'segment':
            data['segments'].append({'start': list(map(float,field(node,'start'))), 'end': list(map(float,field(node,'end'))), 'width': float(field(node,'width')[0]), 'layer': field(node,'layer')[0]})
        if node[0] == 'footprint':
            properties = {item[1]:item[2] for item in node if isinstance(item,list) and item[0] == 'property'}
            pads = []
            for item in node:
                if isinstance(item,list) and item[0] == 'pad':
                    pads.append({'number': item[1], 'type': item[2], 'shape': item[3], 'at': list(map(float,field(item,'at'))), 'size': list(map(float,field(item,'size')))})
            data['footprints'].append({'name': node[1], 'ref': properties.get('Reference',''), 'value': properties.get('Value',''), 'at': list(map(float,field(node,'at'))), 'pads':pads})
    data['outside_outline'] = [f['ref'] for f in data['footprints'] if not (data['outline'][0] <= f['at'][0] <= data['outline'][2] and data['outline'][1] <= f['at'][1] <= data['outline'][3])]
    (pathlib.Path(__file__).resolve().parent/'implantation_kicad.json').write_text(json.dumps(data,ensure_ascii=False,indent=2),encoding='utf-8')
    print(f"Contour {data['outline']}; {len(data['footprints'])} composants; {len(data['segments'])} pistes; hors contour: {data['outside_outline']}")

if __name__ == '__main__':
    run()
