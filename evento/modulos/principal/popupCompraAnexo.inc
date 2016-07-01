<?php 
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$somenteLeitura = 'N';
$desabilitado = 'disabled="disabled"';
$campos	= array("copid"				=> $_SESSION['copid'],
				"coxdatainclusao" 	=> "now()" ,
				"usucpf"    		=> $_SESSION['usucpf'],
				"tpaid"     		=> $_POST['tpaid'],
				"coxstatus" 		=> "'A'"
				);	
if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'evento.php?modulo=principal/cadCompraAnexo&acao=A';</script>";
    exit;
}

# Array de perfis que veem todas as unidades
$arPerfisVerTodas = array(EVENTO_PERFIL_CGCC, 
						 	  EVENTO_PERFIL_CONSULTA,
						 	  EVENTO_PERFIL_PERFIL_EMPRESA,
						 	  EVENTO_PERFIL_SUPER_USUARIO						 	  
							  );
$arPerfis = arrayPerfil();
foreach($arPerfis as $perfil){
	if(in_array($perfil,$arPerfisVerTodas)){
		$somenteLeitura = 'S';
		$desabilitado = "";
		break;
	}
}

$file = new FilesSimec("coanexo", $campos ,"evento");

if($_FILES["Arquivo"]){	
	 
	$arquivoSalvo = $file->setUpload( $_POST['arqdescricao']);
	if($arquivoSalvo){
		echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
		echo"<script>window.location.href = 'evento.php?modulo=principal/cadCompraAnexo&acao=A';</script>";	
	}
}
  
if( $_SESSION['copid'] != '')
{ 
	$sql = "SELECT 
				co.copdsc,  
				co.copnumprocesso,
				to_char(co.copdataabertura::date,'DD/MM/YYYY') AS copdataabertura,		 
				co.cooid, 		 
				co.cocid, 		 
				co.conid, 
				to_char(co.copdatalimite::date,'DD/MM/YYYY') AS copdatalimite,	
				co.usucpf ,
				us.usunome
			FROM 
				evento.coprocesso AS co 
			LEFT JOIN seguranca.usuario AS us ON us.usucpf = co.usucpf 
			WHERE
				co.copid = '".$_SESSION['copid']."'
				";
	$rsDadosProcesso = $db->carregar( $sql ); 
} 
?>
<?php 
monta_titulo( $titulo_modulo, "");
montaCabecalhoProcesso($_SESSION['copid'], false);
?>
<script src="../includes/prototype.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<form name=formulario id=formulario method=post enctype="multipart/form-data">
<input type="hidden" name="salvar" value="0"> 
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
        <td width='50%'>
            <input type="file" name="Arquivo" id="Arquivo" <?php echo $desabilitado;?> />
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
        </td>      
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
        <td><?php 
          $sql = "
            SELECT tpaid AS codigo, tpadescricao AS descricao
                FROM evento.tipoanexo
        "; 
        $db->monta_combo('tpaid', $sql, $somenteLeitura, "Selecione...", '', '', '', '100', 'S', 'tpaid'); 
        ?></td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
        <td><?= campo_textarea( 'arqdescricao', 'S', $somenteLeitura, '', 60, 2, 250 ); ?></td>
    </tr>
    <?php if($somenteLeitura == 'S') { ?>
	    <tr style="background-color: #cccccc">
	        <td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
	        <td height="30"><input type="button" name="botao" value="Salvar" onclick="validaForm();"></td>
	    </tr> 
    <?php } ?>
</table>
<table border="0" cellspacing="0" cellpadding="3" align="center" class="Tabela">
    <?
    if($somenteLeitura == 'S'){
		$linhaExcluir = "'<center><a style=\" cursor: pointer;\"onclick=\"javascript:excluirAnexo(' || arq.arqid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>'";    	
    } else {
		$linhaExcluir = "'<center><img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\"></center>'";
    }    	
        $sql = "SELECT
                        $linhaExcluir as acao,                       
                        to_char(arq.arqdata,'DD/MM/YYYY'),
                        tarq.tpadescricao,
                        arq.arqdescricao,
                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadCompraAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
                        arq.arqtamanho || ' kbs' as tamanho ,
                        arq.arqdescricao,                               
                        usu.usunome
                    FROM
                        ((public.arquivo arq INNER JOIN evento.coanexo aqb
                        ON arq.arqid = aqb.arqid) INNER JOIN evento.tipoanexo tarq
                        ON tarq.tpaid = aqb.tpaid) INNER JOIN seguranca.usuario usu
                        ON usu.usucpf = arq.usucpf
                    WHERE
                        arq.arqstatus = 'A' AND  aqb.copid = " .$_SESSION['copid'];
		 
        $cabecalho = array( "Ação",
                            "Data Inclusão",
                            "Tipo Arquivo",
                            "Nome Arquivo",
                            "Tamanho (Mb)",
                            "Descrição Arquivo",
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
		alerta	= alerta+" - Voce deve escolher um arquivo\r\n";
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