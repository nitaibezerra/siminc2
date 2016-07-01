<?php

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";


if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec("arquivospde", $campos  );
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'pdeescola.php?modulo=principal/documentosAnexos&acao=A';</script>";
    exit;
} 

include APPRAIZ . 'includes/cabecalho.inc';
#include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';
echo montarAbasArray(carregaAbas(), "/pdeescola/pdeescola.php?modulo=principal/documentosAnexos&acao=A");

/*
 * Correção por Alexandre Dourado 20/11/09
 */
if(!$_SESSION['pdeid']) {
	die("<script>
			alert('Plano do pdeescola não encontrado. Refaça o procedimento.');
			window.location='pdeescola.php?modulo=inicio&acao=C';
		 </script>");
}
 
$campos	= array("pdeid"		=> $_SESSION['pdeid'],
				"arpdata"	=>"'".date('Y-m-d')."'",
				"arpstatus" => "'A'"
				);	
$file = new FilesSimec("arquivospde", $campos ,"pdeescola");

if($_FILES["Arquivo"]){	
	//pre( $_POST ,1);
	$arquivoSalvo = $file->setUpload( "".$_POST['arqdescricao']."");
	if($arquivoSalvo){
		echo '<script type="text/javascript"> alert(" Sucesso.");</script>';
		echo"<script>window.location.href = 'pdeescola.php?modulo=principal/documentosAnexos&acao=A';</script>";	
	}
}
 
if($_REQUEST['arqidDel'] != ''){
   
    $anexos = array();
    $sql = "UPDATE pdeescola.arquivospde SET arpstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);

    $db->commit();
    echo"<script>window.location.href = 'pdeescola.php?modulo=principal/documentosAnexos&acao=A';</script>";

}

/*function DownloadArquivo($param){
        global $db;
       
        $sql ="SELECT * FROM public.arquivo WHERE arqid = ".$param['arqid'];
        $arquivo = $db->carregar($sql);
        $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo[0]['arqid']/1000) .'/'.$arquivo[0]['arqid'];
        if ( !is_file( $caminho ) ) {
            $_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
        }
        $filename = str_replace(" ", "_", $arquivo[0]['arqnome'].'.'.$arquivo[0]['arqextensao']);
        header( 'Content-type: '. $arquivo[0]['arqtipo'] );
        header( 'Content-Disposition: attachment; filename='.$filename);
        readfile( $caminho );
        exit();
}*/
?>




<form name=formulario id=formulario method=post enctype="multipart/form-data">
<input type="hidden" name="salvar" value="0"> 
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
        <td width='50%'>
            <input type="file" name="Arquivo" id="Arquivo"/>
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
        </td>      
    </tr>
    <!--<tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
        <td><?php 
          $sql = "
            SELECT tpaid AS codigo, tpadescricao AS descricao
                FROM evento.tipoanexo
        "; 
        $db->monta_combo('tpaid', $sql, 'S', "Selecione...", '', '', '', '100', 'S', 'tpaid'); 
        ?></td>
    </tr>
    -->
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
        <td><?= campo_textarea( 'arqdescricao', 'S', 'S', '', 60, 2, 250 ); ?></td>
    </tr>
    <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
        <td height="30"><input type="button" name="botao" value="Salvar" onclick="validaForm();"></td>
    </tr> 
</table>
<table border="0" cellspacing="0" cellpadding="3" align="center" class="Tabela">
    <?
    	if( $_SESSION['pdeid'] != '' ){
	        $sql = "SELECT
	                        '<center><a href=\"#\" onclick=\"javascript:excluirAnexo(' || arq.arqid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,                       
	                        to_char(arq.arqdata,'DD/MM/YYYY'),
	                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/documentosAnexos&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
	                        arq.arqtamanho || ' kbs' as tamanho ,
	                        arq.arqdescricao
	                    FROM public.arquivo arq 
	                         INNER JOIN pdeescola.arquivospde aqb ON arq.arqid = aqb.arqid  
	                    WHERE
	                        aqb.arpstatus = 'A' AND  aqb.pdeid = ".$_SESSION['pdeid'];
	
	        $cabecalho = array( "Ação",
	                            "Data Inclusão",
	                            "Nome Arquivo",
	                            "Tamanho (Mb)",
	                            "Descrição Arquivo");
	         
	        $db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
    	}
    ?>
</table>


 
<script>
var query;
var objForm = document.forms["formulario"];

function validaForm(){
	var saida	= true;
	var alerta	= "Ocorreram os seguintes erros:\r\n\r\n";
	if(objForm.Arquivo.value==''){
		alerta	= alerta+" - Voce deve escolher um arquivo\r\n";
		saida	= false;
	}

	if(saida==false){
		alert(alerta);
	}
	else{
		objForm.submit();
	}
}
	function excluirAnexo( arqid ){  
		if ( confirm( 'Deseja excluir o Documento?' ) ) {
			location.href= window.location+'&arqidDel='+arqid;
		}
	}
</script>