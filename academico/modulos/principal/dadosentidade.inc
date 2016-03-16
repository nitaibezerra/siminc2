<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--

jQuery.noConflict();

jQuery(document).ready(function(){

	// Desativar enter no componente entidade. Feito dessa forma pois o IE não deixa alterar o tipo de um input
	var value = jQuery('#btngravar').attr('value'); // grab value of original
	var html = '<input type="button" id="btngravar" name="btngravar" value="'+value+'" />';
	jQuery('#btngravar').after(html).remove(); // add new, then remove original input
		
	jQuery('#btngravar').click(function(){
		jQuery('#frmEntidade').submit();
	});

});

//-->
</script>
<?php
require_once APPRAIZ . "includes/classes/entidades.class.inc";

$_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
$entid = $_SESSION['academico']['entid'] ? $_SESSION['academico']['entid'] : $_REQUEST['entidunidade'];
$orgid = $_SESSION['academico']['orgid'];

$existecampus = academico_existeentidade( $entid );

if( !$existecampus ){
	echo "<script>
			alert('A Unidade informada não existe!');
			history.back(-1);
		  </script>";
	die;
}

// instancia o objeto do módulo 
$academico = new academico();

if( $_REQUEST['atualizaReitorias'] ){
	$entidID = $_REQUEST['entidID'];
	$sql = "UPDATE entidade.entidade SET entstatus= 'I' WHERE entid ={$entidID}"; 
	$db->executar($sql);
	$db->commit();
}

if( $_REQUEST['atualizaCampus'] ){
	$entidNovo = $_REQUEST['entidNovo'];
	$sql = "UPDATE entidade.entidade SET entstatus= 'I' WHERE entid ={$entidNovo}"; 
	$db->executar($sql);
	$db->commit();
}

//if ($_REQUEST['opt'] && ($_REQUEST['entnumcpfcnpj'] || $_REQUEST['entcodent'] || $_REQUEST['entunicod'])) {
if ($_REQUEST['opt'] && $_REQUEST['entid'] ) {
	if ($_REQUEST['opt'] == 'salvarRegistro') {
		$entidade = new Entidades();
		$entidade->carregarEntidade($_REQUEST);
		$entidade->salvar();
		//salvarRegistroDetalhes($_REQUEST);
        echo "<script type=\"text/javascript\">
        		alert('Dados gravados com sucesso');
        		window.location='?modulo=principal/dadosentidade&acao=A&entidunidade=".$_REQUEST['entid']."';
        	  </script>";
        exit;
    }
}
	
?>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/entidades.js"></script>
<script language="JavaScript" src="./geral/js/academico.js"></script>
<link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
<?php


$perfilNotBloq = array(//PERFIL_IFESCADBOLSAS, 
    PERFIL_CADASTROGERAL,
    PERFIL_MECCADBOLSAS,
    PERFIL_MECCADCURSOS,
    PERFIL_MECCADASTRO,
    PERFIL_ADMINISTRADOR,
    PERFIL_REITOR,
    PERFIL_ALTA_GESTAO,
    PERFIL_ASSESSORIA_ALTA_GESTAO,
    PERFIL_INTERLOCUTOR_INSTITUTO,
    PERFIL_PROREITOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
$permissoes['gravar'] = academico_possui_perfil(array(PERFIL_ADMINISTRADOR,PERFIL_INTERLOCUTOR_INSTITUTO,PERFIL_REITOR,PERFIL_PROREITOR)) ? true : false;

//validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança
//print_r($permissoes);
//die;
$bloqueado = $permissoes['gravar'] ? false :  true;
$bloq = '';
$hide = '';
if($bloqueado) {
	$bloq = 'disabled="disabled"';
	$hide = 'hide';
}
// Busca a unidade pai do Campus
$autoriazacaoconcursos = new autoriazacaoconcursos();
$unidade = ($_REQUEST['entidunidade'] || $_SESSION['academico']['entid']) ? $entid : $autoriazacaoconcursos->buscaentidade($entid);

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

if($unidade) {
	$_SESSION['academico']['entid'] 		= $unidade;
	$_SESSION['academico']['entidadenivel'] = 'unidade';
	$_SESSION['sig_var']['entid'] 			= $_SESSION['academico']['entid'];
	$_SESSION['sig_var']['iscampus'] 		= 'nao';
	
	$edtdsc = $db->pegaUm("SELECT entd.edtdsc FROM academico.entidadedetalhe entd WHERE entd.entid='".$_SESSION['sig_var']['entid']."'");
	$edtdsc = simec_htmlspecialchars(str_replace(array("\r\n","\n\r","\r","\n"), '\n', $edtdsc), ENT_QUOTES);
}
//die( $_SESSION['academico']['orgid'] );
// Monta as abas
//if ($_SESSION['baselogin'] == "simec_desenvolvimento"){
//	$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57276' : ABA_DADOS_ENTIDADES_CURSOS; //Dados Desenv
//}else{
//	$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57267' : ABA_DADOS_ENTIDADES_CURSOS;
//}
#$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( "Dados da Instituição", "");

// Testando se a varival $unidade esta correta (feito pelo Alexandre para mapear erros ocorridos)
if(!$unidade) {
	echo "<script>alert('Unidade não selecionada.');window.location='?modulo=inicio&acao=C';</script>";
	exit;
}
?>
<div class=''>
    <div class='col-md-2' style='position:static;'>
        <?=cria_aba_2( $abacod_tela, $url, $parametros);?>
    </div>
    <div class='col-md-9' style='position:static;'>
<?php
echo $autoriazacaoconcursos->cabecalho_entidade($entid);
$entidade = new Entidades();
$entidade->carregarPorEntid($unidade);
echo $entidade->formEntidade("academico.php?modulo=principal/dadosentidade&acao=A&opt=salvarRegistro",
							 array("funid" => FUNCAO_UNIVERSIDADE, "entidassociado" => null),
							 array("enderecos"=>array(1),"fotos"=>true,"gravar"=>$permissoes['gravar']),null,$bloq,$hide
							 );

/*$arPerfil = academico_array_perfil();
if( in_array( PERFIL_SUPERUSUARIO, $arPerfil ) || in_array( PERFIL_ADMINISTRADOR, $arPerfil ) ){
}*/
							 
							 
						 
if ($permissoes['insereEntidade']):
?>
	<center>
	<fieldset style="background: #fff; width: 95%;">
	<legend>Lista de Campus</legend>
	<?
	$academico->academico_listacampus( $unidade, $orgid );
	?>
	<input type="button" value="Novo Campus" style="margin-top: 3px" onclick="location.href='academico.php?modulo=principal/inserirInstituicaoCampus&acao=C'">
	</fieldset>
	</center>
	<center>
	<fieldset style="background: #fff; width: 95%;">
	<legend>Reitoria</legend>
	<?
	$academico->academico_listareitoria( $unidade, $orgid ); 
	?>
	<input type="button" value="Nova Reitoria" style="margin-top: 3px" onclick="location.href='academico.php?modulo=principal/inserirInstituicaoCampus&acao=C&tipoEntidade=reitoria'">
	</fieldset>
	</center>
<?
endif;
?>
    </div>
</div>
<?php
if($bloqueado) {
	echo "<script>document.getElementById('btngravar').disabled=true</script>";
}
 
?>
<script type="text/javascript">
document.getElementById('tr_entcodent').style.display 			= 'none';
document.getElementById('tr_entnuninsest').style.display 		= 'none';
document.getElementById('tr_entungcod').style.display 			= 'none';
document.getElementById('tr_tpctgid').style.display 			= 'none';
document.getElementById('tr_njuid').style.display 				= 'none';
document.getElementById('tr_tpcid').style.display 				= 'none';
document.getElementById('tr_tplid').style.display 				= 'none';
document.getElementById('tr_tpsid').style.display 				= 'none';
document.getElementById('tr_funcoescadastradas').style.display  = 'none';

frmEntidade = new GerenciaTabEntidade();

<? 
if ( !($db->testa_superuser() || academico_possui_perfil(PERFIL_ADMINISTRADOR)) ):
?>

/*
 * DESABILITANDO O CAMPO DE CNPJ
 * DESABILITANDO A SIGLA DA ENTIDADE
 * DESABILITANDO O NOME DA ENTIDADE
 */
frmEntidade.blockCampo(['entnumcpfcnpj', 'entsig', 'entnome']);

<? 
elseif ( !$db->testa_superuser() ):
?>

frmEntidade.blockCampo('entnumcpfcnpj', ['onfocus', 'onmouseout', 'onkeyup', 'readOnly', 'class']);
frmEntidade.blockCampo('entnome', ['readOnly', 'class']);

<?
endif;
?>


<? 
if (academico_possui_perfil(PERFIL_ADMINISTRADOR)):
?>
var entunicod = document.getElementById('entunicod');
entunicod.setAttribute('onblur','MouseBlur(this);');

/*
 * Habilitando o campo NOME
 */
var entnome = document.getElementById('entnome'); 
entnome.readOnly = false;
entnome.className = 'normal';
entnome.setAttribute('onfocus',    'MouseClick(this);this.select();');
entnome.setAttribute('onmouseout', 'MouseOut(this);');
entnome.setAttribute('onblur',	   'MouseBlur(this);');

/*
 * Habilitando o campo Razão Social 
 */
var entnome = document.getElementById('entrazaosocial'); 
entnome.readOnly = false;
entnome.className = 'normal';
entnome.setAttribute('onfocus',    'MouseClick(this);this.select();');
entnome.setAttribute('onmouseout', 'MouseOut(this);');
entnome.setAttribute('onblur',	   'MouseBlur(this);');
//document.getElementById('entnumcpfcnpj').setAttribute('onkeyup', 	'');
<?
else:
?>

/*
 * DESABILITANDO O CÓDIGO DA UNIDADE
 */
frmEntidade.blockCampo('entunicod'); 
<?
endif;
?>

frmEntidade.addTR('tr_funcoescadastradas', 
				  'trcaracterizacao', 
				  'Caracterização da Instituição', 
				  '', 
				  true);
				  
frmEntidade.addTR('trcaracterizacao', 
				  'trdadosespecificos', 
				  'Caracterização da Instituição<br/>(texto sucinto destacando informações<br/> importantes sobre a unidade.<br/> Incluir informações sobre sua data de criação <br/>e sua importância para a região, dentre outras) :', 
				  '<textarea <?=$bloq ?>onkeyup="textCounter( this.form.edtdsc, this.form.no_edtdsc, 1000);" onkeydown="textCounter( this.form.edtdsc, this.form.no_edtdsc, 1000 );" style="width: 70ex;" onblur="MouseBlur( this ); textCounter( this.form.edtdsc, this.form.no_edtdsc, 1000);" onmouseout="MouseOut( this );" onfocus="MouseClick( this );" onmouseover="MouseOver( this );" rows="10" cols="70" name="edtdsc" id="edtdsc" class="txareanormal"><?=$edtdsc; ?></textarea>' + 
				  '<br/>'+
				  '<input class="CampoEstilo" type="text" value="<?= (1000 - strlen($edtdsc)) ?>" maxlength="6" size="6" name="no_edtdsc" style="border-left: 3px solid rgb(136, 136, 136); text-align: right; color: rgb(128, 128, 128);" readonly=""/>' +
				  '<font face="Verdana" color="red" size="1"> máximo de caracteres</font>');


</script>
<script>
	function abredadoscampus( entid ){
		return window.location = '?modulo=principal/alterarCampus&acao=A&entid=' + entid;
	}
	function abredadosreitoria( entid ){
		return window.location = '?modulo=principal/alterarReitoria&acao=A&entid=' + entid;
	}
	function inativarreitoria( entidID ){
		if(confirm("Deseja realmente excluir a Reitoria"))
		  {
			var myAjax = new Ajax.Request('academico.php?modulo=principal/dadosentidade&acao=A', {
		        method:     'post',
		        parameters:  '&atualizaReitorias=true&entidID=' + entidID,
		        onComplete: function (res){	
	       			alert("Reitoria excluida com sucesso!");
	       			window.location.href = window.location.href;
		        }
		  	}); 
		  }
	}
	function inativarcampus( entidade ){
		if(confirm("Deseja realmente excluir o Campus"))
		  {
			var myAjax = new Ajax.Request('academico.php?modulo=principal/dadosentidade&acao=A', {
		        method:     'post',
		        parameters:  '&atualizaCampus=true&entidNovo=' + entidade,
		        onComplete: function (res){	
	       			alert("Campus excluido com sucesso!");
	       			window.location.href = window.location.href;
		        }
		  	}); 
		  }
	}
</script>
<?php //if(academico_possui_perfil( PERFIL_ASSISTENCIA_ESTUDANTIL )):
if( !academico_possui_perfil(array(PERFIL_SUPERUSUARIO,PERFIL_ADMINISTRADOR,PERFIL_REITOR,PERFIL_INTERLOCUTOR_INSTITUTO,PERFIL_PROREITOR)) ):
?>
	<script>
		jQuery('input[type=button]').remove();
	</script>
<?php endif; ?>