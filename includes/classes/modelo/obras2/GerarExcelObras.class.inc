<?php

class GerarExcelObras
{

    // define parametros(init)
    public function __construct()
    {
        global $nomeDoArquivoXls;
        $this->armazena_dados = ""; // Armazena dados para imprimir(temporario)
        $this->nomeDoArquivoXls = $nomeDoArquivoXls; // Nome do arquivo excel
        $this->ExcelStart();
    }

// fim constructor
    // Monta cabecario do arquivo(tipo xls)
    public function ExcelStart()
    {

        //inicio do cabecario do arquivo
        $this->armazena_dados = pack("vvvvvv", 0x809, 0x08, 0x00, 0x10, 0x0, 0x0);
    }

    // Fim do arquivo excel
    public function FechaArquivo()
    {
        $this->armazena_dados .= pack("vv", 0x0A, 0x00);
    }

    // monta conteudo
    //function MontaConteudoString( $excel_linha, $excel_coluna, $value){
    public function MontaConteudoString($excel_linha, $excel_coluna, $value)
    {
        $tamanho = strlen($value);
        $this->armazena_dados .= pack("v*", 0x0204, 8 + $tamanho, $excel_linha, $excel_coluna, 0x00, $tamanho);
        $this->armazena_dados .= $value;
    }

//Fim, monta Col/Lin

    public function MontaConteudoNumero($row, $col, $num)
    {
        $record = 0x0203;
        $length = 0x000E;
        $xf = 0x0F;

        $header = pack("vv", $record, $length);
        $data = pack("vvv", $row, $col, $xf);
        $xl_double = pack("d", $num);
        $this->armazena_dados .= ($header . $data . $xl_double);
    }

    // Gera arquivo(xls)
    public function GeraArquivo($type) 
    {

        $this->FechaArquivo();

        switch ($type) {
            case 'D':
            default:
                header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
                header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
                header("Pragma: no-cache");
                header("Content-type: application/excel; name=$this->nomeDoArquivoXls" . ".xls");
                header("Content-Disposition: attachment; filename=$this->nomeDoArquivoXls" . ".xls");
                header("Content-Description: MID Gera excel");
                print ( $this->armazena_dados);
                break;
            case 'F':
                return $this->armazena_dados;
                break;
        }
    }
}
