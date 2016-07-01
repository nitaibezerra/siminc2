<?php 
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";


//verifica sessão da pagina $_SESSION['ctrid']
verificaSessaoPagina();


$campos	= array("ctrid"				=> $_SESSION['ctrid'],
				"ancdatainclusao" 	=> "now()" ,
				"usucpf"    		=> $_SESSION['usucpf'],
				"tpaid"     		=> $_POST['tpaid'],
				"ancstatus" 		=> "'A'"
				);	
				
if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'evento.php?modulo=principal/cadContratoAnexo&acao=A';</script>";
    exit;
} 


$file = new FilesSimec("ctanexo", $campos ,"evento");



if($_FILES["Arquivo"]){	
		 
	$arquivoSalvo = $file->setUpload( $_POST['arqdescricao']);
	
	if($arquivoSalvo){
		echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
		echo"<script>window.location.href = 'evento.php?modulo=principal/cadContratoAnexo&acao=A';</script>";	
	}
}
 
if($_REQUEST['arqidDel'] != ''){
   
    $anexos = array();
    $sql = "UPDATE evento.ctanexo SET ancstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);

    $db->commit();
    echo"<script>window.location.href = 'evento.php?modulo=principal/cadContratoAnexo&acao=A';</script>";

}
 /*
if( $_SESSION['ctrid'] != '')
{ 
	$sql = "SELECT  
					ctr.ctrnum, 
		    		ctr.ctrano, 
		    		ctr.tpcid, 
		    		ctr.ctrproccontr, 
		    		ctr.ctrprocexecfin, 
		    		ctr.ctrprocexecctr, 
            		ctr.modid, 
            		ctr.ctrnummod, 
            		ctr.ctrobj, 
            		ctr.usgid, 
            		ctr.ctrnumcron, 
            		ctr.ctaid, 
            		coalesce( ctr.ctrvlrinicial, 0 ) / 1 as ctrvlrinicial,					            		 
       				ctr.ctrobsvlrini, 
       				coalesce( ctr.ctrvlrglobal, 0 ) / 1 as ctrvlrglobal,
			       	coalesce( ctr.ctrvlrtotal, 0 ) / 1 as ctrvlrtotal,
       				coalesce( ctr.ctrvlrmensal, 0 ) / 1 as ctrvlrmensal, 
       				ctr.ctrobsvlr, 
       				to_char(ctr.ctrdtiniciovig::date,'YYYY-MM-dd') AS ctrdtiniciovig,
       				to_char(ctr.ctrdtfimvig::date,'YYYY-MM-dd') AS ctrdtfimvig,       		
       				ctr.obvid 
  			FROM evento.ctcontrato ctr 
			WHERE
				ctr.ctrid = '".$_SESSION['ctrid']."'
				";
	
	$rsDadosProcesso = $db->carregar( $sql ); 
} 
*/
?>
<?php
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
cabecalhoContrato( $_SESSION['ctrid'] );
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );
montaCabecalhoContrato($_SESSION['ctrid']);
?>
<script src="../includes/prototype.js"></script>
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
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
        <td><?php 
          $sql = "
            SELECT tpaid AS codigo, tpadescricao AS descricao
                FROM evento.tipoanexo order by 2
        "; 
        $db->monta_combo('tpaid', $sql, 'S', "Selecione...", '', '', '', '100', 'S', 'tpaid'); 
        ?></td>
    </tr>
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
        $sql = "SELECT
                        '<center><a style=\" cursor: pointer;\"onclick=\"javascript:excluirAnexo(' || arq.arqid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>' as acao,                       
                        to_char(arq.arqdata,'DD/MM/YYYY'),
                        tarq.tpadescricao,
                        arq.arqdescricao,
                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadContratoAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
                        arq.arqtamanho || ' kbs' as tamanho ,
                        arq.arqdescricao,                               
                        usu.usunome
                    FROM
                        ((public.arquivo arq INNER JOIN evento.ctanexo anc
                        ON arq.arqid = anc.arqid) INNER JOIN evento.tipoanexo tarq
                        ON tarq.tpaid = anc.tpaid) INNER JOIN seguranca.usuario usu
                        ON usu.usucpf = arq.usucpf
                    WHERE
                        arq.arqstatus = 'A' AND  anc.ctrid = " .$_SESSION['ctrid'];
		 
        $cabecalho = array( "Ação",
                            "Data Inclusão",
                            "Tipo Arquivo",
                            "Descrição Arquivo",
                            "Nome Arquivo",
                            "Tamanho (Mb)",    
                            "Responsável");
        $db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '' );
    ?>
</table>
<script language="javascript" type="text/javascript">

 
var query;
var objForm = document.forms["formulario"];

function validaForm(){
	var saida	= true;
	var alerta	= "Ocorreram os seguintes erros:\r\n\r\n";
	var tipo = document.getElementById('tpaid');
	
	if(objForm.Arquivo.value==''){
		alerta	= "Voce deve escolher um arquivo\r\n";
		saida	= false;
	}
	if( tipo.value == '' ){
		alert("É necessário informar um tipo de documento");
		return false;
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