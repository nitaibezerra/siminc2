<?php
$perfil = pegaPerfil($_SESSION['usucpf']);

if($_REQUEST['download'] == 'manual'){
	
	$caminho = APPRAIZ. "www/proinfantil/documentos/Manual_Unidades_do_Proinfancia.pdf";
	if ( !is_file( $caminho ) ) {
        echo"<script>
            	alert('Arquivo não encontrado na pasta.');
            	window.location.href = 'proinfantil.php?modulo=principal/listaObrasMunicipio&acao=A';
            </script>";
    	exit();
	} 
    $filename = "Manual_Unidades_do_Proinfancia.pdf";
    header( 'Content-type: pdf' );
    header( 'Content-Disposition: attachment; filename='.$filename);
    readfile( $caminho );
    echo"<script>window.location.href = 'proinfantil.php?modulo=principal/listaObrasMunicipio&acao=A';</script>";	
} 

$ocultar = false;
if( in_array(EQUIPE_MUNICIPAL,$perfil)){
	$ocultar = true;
}

if ($_REQUEST['filtraMunicipio'] && $_REQUEST['estuf']) {
	header('content-type: text/html; charset=ISO-8859-1');
	filtraMunicipio($_REQUEST['estuf']);
	exit;
}

unset( $_SESSION['proinfantil']['popup'] );
unset( $_SESSION['proinfantil']['muncod'] );
unset( $_SESSION['proinfantil']['obrid'] );
unset( $_SESSION['proinfantil']['pinid'] );
unset( $_SESSION['proinfantil']['estuf'] );

include APPRAIZ . "includes/cabecalho.inc";
include_once "_funcoes_proinfancia.php";
echo "<br/>";
?>

<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
	<tr>
		<td class="subtituloEsquerda">
			<input type="button" value="Voltar" id="btnVoltar"/>
		</td>
		<td class="subtitulodireita" valign="top">
			<a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/listaObrasMunicipio&acao=A&download=manual'"><b><img src="../imagens/pdf.gif" width="18px" border="0"> Manual Unidades do Proinfância</b></a>
		</td>
	</tr>
</table>
<br/>

<?php 
$db->cria_aba($abacod_tela,$url,$parametros); 

if($_POST){
	extract($_POST);
}
?>

<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.msg_erro{color:#990000}
	.link{cursor: pointer}
	.mini{width:12px;height:12px}
	.sucess_msg{color: blue;}
	.img_middle{vertical-align:middle;border:0px}
	.hidden{display:none}
	.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
	.fechar{position:relative;right:-5px;top:-26px;}
	.img{background-color:#FFFFFF}
	.red{color:#990000}
</style>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
$(function() {
	 $('[name=btn_pesquisar]').click(function() {
	 	$("[name=btn_pesquisar]").val("Carregando...");
	 	$("[name=btn_pesquisar]").attr("disabled","disabled");
	 	$("#formulario_pesquisa").submit();
	 });
	 $('[name=btn_listar_todas]').click(function() {
	 	window.location.href = window.location;
	 });
	 $('#btnVoltar').click(function(){
		document.location.href = 'proinfantil.php?modulo=inicio&acao=C';
	 });
});

function editarProInfantil(obrid){
	var url = "?modulo=principal/questionarioMunicipio&acao=A&obrid=" + obrid;
	window.location.href = url;
}

function filtraMunicipio(estuf) {
	if(estuf!=''){
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "filtraMunicipio=true&" + "estuf=" + estuf,
		   success: function(resp){
				document.getElementById("td_municipio").innerHTML = resp;
			}
		});
	}
}
</script>

<form id="formulario_pesquisa" name="formulario_pesquisa" method="post" action="">
	<input type="hidden" value="pesquisarProInfantil" name="requisicao" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
	        <td class="SubTituloDireita">Nome da Obra:</td>
	        <td>
	        	<?php echo campo_texto( 'obrnome', 'N', "S", '', 65, 60, '', '', 'left', '', 0,'','',$obrnome); ?>
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Tipologia:</td>
	        <td>
	        	<?php $sql = "select distinct tpoid as codigo, tpodsc as descricao from obras.tipologiaobra where tpoid in (".OBRA_TIPO_A.",".OBRA_TIPO_B.",".OBRA_TIPO_C.",".MI_OBRA_TIPO_B.",".MI_OBRA_TIPO_C.") order by tpodsc"; ?>
	        	<?php $db->monta_combo("tpoid",$sql,"S","Selecione...",'',"","","","N","","",$tpoid); ?>
	        </td>
	    </tr>
	    <?php if( !$ocultar ){ ?>
	    <tr>
	        <td class="SubTituloDireita">Situação do Plano:</td>
	        <td>
	        	<?php $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid = ".WF_PROINFANTIL." AND esdstatus = 'A' ORDER BY esdordem"; ?>
	        	<?php $db->monta_combo("esdid",$sql,"S","Selecione...",'',"","","","N","","",$stoid); ?>
	        </td>
	    </tr>
		<?php } ?>	    
	    <tr>
	        <td class="SubTituloDireita">Situação da Obra:</td>
	        <td>
	        	<?php $sql = "select
									e.esdid as codigo,
								    e.esddsc as descricao
								from workflow.estadodocumento e 
								where 
									e.tpdid = 105
								    and e.esdid in (690, 693, 768)
								    and e.esdstatus = 'A' 
								order by e.esddsc"; ?>
	        	<?php $db->monta_combo("stoid",$sql,"S","Selecione...",'',"","","","N","","",$stoid); ?>
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">UF:</td>
	        <td>
	        	<?php $sql = "SELECT estuf AS codigo, estdescricao AS descricao FROM territorios.estado ORDER BY estdescricao"; ?>
				<?php $db->monta_combo( "estuf", $sql, "S", "Selecione...", 'filtraMunicipio', "","","","N","","",$estuf); ?>
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Município:</td>
	        <td id="td_municipio">
	        	<?php $sql = "SELECT ter.muncod AS codigo, ter.mundescricao AS descricao FROM territorios.municipio ter WHERE ter.estuf = '$estuf' ORDER BY ter.estuf, ter.mundescricao"; ?>
	        	<?php $db->monta_combo("muncod",$sql,"S","Selecione...",'',"","","","N","","",$muncod); ?>
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Ano Início de Atendimento:</td>
	        <td>
	        	<?php $sql = "SELECT distinct	
	        						SUBSTRING(trim(r.resdsc), 7, 4) as codigo,
	        						SUBSTRING(trim(r.resdsc), 7, 4) as descricao 
								FROM proinfantil.questionario q
									INNER JOIN 	questionario.resposta r on r.qrpid = q.qrpid
								WHERE
									r.perid = 1587
									and cast(SUBSTRING(trim(r.resdsc), 7, 4) as integer) > 0
								order by
									codigo"; ?>
				<?php $db->monta_combo( "ano_atend", $sql, "S", "Selecione...", '', "","","","N","","",$ano_atend); ?>
	        </td>
	    </tr>
	    <tr>
			<td colspan="2" class="SubtituloTabela center" >
				<input type="button" value="Pesquisar" name="btn_pesquisar" />
				<input type="button" value="Listar Todas" name="btn_listar_todas" />
			</td>
		</tr>
	</table>
</form>
<?php listarObrasProInfantil(); ?>