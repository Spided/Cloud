import docx
from reportlab.pdfgen import canvas

def convert_word_to_pdf(input_file, output_file):
    doc = docx.Document(input_file)
    c = canvas.Canvas(output_file)
    y = 750
    for para in doc.paragraphs:
        c.drawString(100, y, para.text)
        y -= 20
    c.save()

if __name__ == "__main__":
    import sys
    if len(sys.argv) == 3:
        convert_word_to_pdf(sys.argv[1], sys.argv[2])
    else:
        print("Usage: python convert.py input.docx output.pdf")