<?php

include_once APPRAIZ . "pdeescola/classes/MeParceiro.class.inc";
include_once APPRAIZ . "pdeescola/classes/MeMaisEducacao.class.inc";
include_once APPRAIZ . 'includes/workflow.php';
require_once APPRAIZ . 'includes/classes/entidades.class.inc';
	
me_verificaSessao();
 

$boExisteEntidade = boExisteEntidade( $_SESSION['meentid'] );
if( !$boExisteEntidade ){
	echo "<script>
			alert('A Entidade informada não existe!');
			history.back(-1);
		  </script>";
	die;
}

if( $_SESSION['memid'] != '' &&  $_REQUEST['rel_estimativa'] != '' ){
	$sql_combo = "SELECT
							ent.entid as codigo 
						FROM 
							entidade.entidade ent
						INNER JOIN
							pdeescola.memaiseducacao mme ON mme.entid = ent.entid AND mme.memanoreferencia = " . $_SESSION["exercicio"] . "
						WHERE mme.memid = ".$_SESSION['memid']."
							ORDER BY
							ent.entnome";
	$entid = $db->pegaUm( $sql_combo );
	echo $entid;
	//	echo "
//		<script>
//			window.open( 'pdeescola.php?modulo=merelatorio/meresultado_estimativa&acao=A&entid=$entid&tipo=html', 
//			'relatorio', 'width=800,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
//		</script>
//		";
	die();
}
// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;

if(!$_SESSION["memid"]){
	echo "<script>
			alert('Não existe escola atribuida para o seu Perfil.');
			window.location.href = '/pdeescola/pdeescola.php?modulo=inicio&acao=C'; 
		  </script>";
	die;
}

$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid'];
$existeDocid = $db->pegaUm($sql);

if($existeDocid) {
	// Recupera ou cria o docid.
	$docid = meCriarDocumento( $_SESSION['meentid'] , $_SESSION['memid'] );
	if($docid == false) {
		echo "<script>
				alert('Esta entidade não consta na tabela principal do Mais Educação');
				history.back(-1);
			  </script>";
		die;
	}
}

$sql = "SELECT count(*) FROM seguranca.perfilusuario WHERE usucpf = '".$_SESSION['usucpf']."' AND pflcod in (".PDEESC_PERFIL_CAD_MAIS_EDUCACAO.", ".PDEESC_PERFIL_SUPER_USUARIO.", ".PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO.")";
$vPerfilCadMaisEscola = $db->pegaUm($sql);
 
if((integer)$vPerfilCadMaisEscola > 0) {
	if(!$existeDocid) {
		if((integer)$_SESSION["exercicio"] == meMaxProgramacaoExercicio()) {
			// Recupera ou cria o docid.
			$docid = meCriarDocumento( $_SESSION['meentid'] , $_SESSION['memid'] );
			if($docid == false) {
				echo "<script>
						alert('Esta entidade não consta na tabela principal do Mais Educação');
						history.back(-1);
					  </script>";
				die;
			}
		} else {
			$docid = false;
		}
	}

}


$esdid = mePegarEstadoAtual( $docid );
if((integer)$esdid == CADASTRAMENTO_ME || (integer)$esdid == CORRECAO_CADASTRAMENTO_ME) { 
	$somenteConsulta = false;
}

	
$perfis = arrayPerfil();
if(in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $perfis)||
	in_array(PDEESC_PERFIL_CAD_MAIS_EDUCACAO, $perfis)||
	in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis)){
	$somenteConsulta = false;
}

$boSalvar = (!$somenteConsulta) ? true : false;

$campo_tipo_entidade = ($_GET['campo_tipo_entidade']) ? $_GET['campo_tipo_entidade'] : 1;

if($_GET['entid']){
	$sql = "SELECT entnumcpfcnpj FROM entidade.entidade WHERE entid = ". $_GET['entid'];
	$numcpfcnpj = $db->pegaUm($sql);
	if(strlen($numcpfcnpj) > 11){
		$campo_tipo_entidade = 2;
	}
}




# Excluir Parceiros
if($_REQUEST['idParceiroExcluir']){
	$sql = "DELETE FROM entidade.funentassoc WHERE feaid IN (SELECT fea.feaid FROM pdeescola.meparceiro mpa
														     LEFT JOIN entidade.funcaoentidade fen ON fen.entid = mpa.entid 
														     LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid   
														     WHERE mpa.mepid='".$_REQUEST['idParceiroExcluir']."' AND fen.funid IN('".FUN_PARCEIRO_ME_PF."','".FUN_PARCEIRO_ME_PJ."'))";
	$db->executar($sql);
	$sql = "delete from pdeescola.meparceiro where mepid = ". $_REQUEST['idParceiroExcluir'];
	$db->executar($sql);
	$db->commit();
	echo "<script>
				  alert('Parceiro excluido com sucesso.'); 
		          window.location.href = '/pdeescola/pdeescola.php?modulo=meprincipal/parceiros&acao=A';
		  </script>";
	exit;
}

if ($_REQUEST['opt'] == 'salvarRegistro') {

	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	
	$cnpjAnterior = str_replace(array(".", "/", "-"), "", $_REQUEST['entnumcpfcnpj']);
		
	# Verifica se o campo quantidade alunos é numerico
	$mepquantidadealuno = ($_POST['mepquantidadealuno'] ? $_POST['mepquantidadealuno'] : 0);
	if(!is_numeric($mepquantidadealuno)){
		echo "<script>
				alert('Será aceito somente números no campo Quant. Alunos.');
				history.back(-1);
			  </script>";
		exit;
	}
        
   	/*
	 * Mais Educação 
	 */	
    $obMaisEducacao = new MeMaisEducacao();
	$memid = $obMaisEducacao->pegaUm( "SELECT memid FROM pdeescola.memaiseducacao WHERE entid = ". $_SESSION['meentid'] ." and memanoreferencia = " . $_SESSION["exercicio"] );
	if(!$memid && empty($memid)){
		$obMaisEducacao->entid = $_SESSION['meentid'];
		$obMaisEducacao->docid = null;
		$obMaisEducacao->memdataatualizacao = date("Y-m-d");
		$obMaisEducacao->memanoreferencia = $_SESSION["exercicio"];
		$memid = $obMaisEducacao->salvar();
	}
    /*
	 * Me-Parceiro 
	 */
	if(!$mepquantidadealuno || $mepquantidadealuno==0) $mepquantidadealuno = '0';
	$obMeParceiro = new MeParceiro($_POST['entid_parceiro']);
	$obMeParceiro->memid = $memid;
	$obMeParceiro->entid = $entidade->getEntid();
	$obMeParceiro->mepquantidadealuno = $mepquantidadealuno;
	$obMeParceiro->salvar();
	$db->commit();
	
	echo "<script>
			alert('Dados Gravados com sucesso');
			window.location='pdeescola.php?modulo=meprincipal/parceiros&acao=A';
		  </script>";
	exit;
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
echo montarAbasArray(carregaAbasMaisEducacao(), "/pdeescola/pdeescola.php?modulo=meprincipal/parceiros&acao=A");
$titulo = "Mais Educação";
$subtitulo = "Cadastro - Parceiros";
echo monta_titulo($titulo, $subtitulo);
echo cabecalhoMaisEducacao();
$opcoes = array	("CPF" => array	("valor" => "1",
								 "id"    => "cpf",
								 "callback" => "tipoEntidade(this.value);"
					),
				 "CNPJ" => array ("valor"    => "2",
								  "id"    	 => "cnpj",
								  "callback" => "tipoEntidade(this.value);"	
					),
				);
?>				
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tbody id="tableEntidade">
<tr>
<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Tipo de busca:</label></td>
<td><?php echo campo_radio( 'cpfcnpj', $opcoes, 'h', true ); ?></td>
</tr>
</tbody>
</table>

<table width="95%" align="center" cellSpacing="0" cellPadding="0">
<tr>
<td>
<?
$entidade = new Entidades();
if($_REQUEST['entid']) {
	$entidade->carregarPorEntid($_REQUEST['entid']);
}
switch($campo_tipo_entidade) {
	case 1:
		echo $entidade->formEntidade("pdeescola.php?modulo=meprincipal/parceiros&acao=A&opt=salvarRegistro",
									 array("funid" => FUN_PARCEIRO_ME_PF, "entidassociado" => $_SESSION['meentid']),
									 array("enderecos"=>array(1))
									 );
		break;
	case 2:
		echo $entidade->formEntidade("pdeescola.php?modulo=meprincipal/parceiros&acao=A&acao=A&opt=salvarRegistro",
									 array("funid" => FUN_PARCEIRO_ME_PJ, "entidassociado" => $_SESSION['meentid']),
									 array("enderecos"=>array(1))
									 );
		break;
	default:
		die("Problemas com a função da entidade");
}


 
if( (count( $perfis ) == 1 && in_array(PDEESC_PERFIL_CONSULTA_MAIS_EDUCACAO, $perfis)) || !$boSalvar ){
	echo "<script>
			document.getElementById('btngravar').disabled = 1;
		  </script> 
	";				
	$imgExcluir		= "/imagens/excluir_01.gif";			 
	$onclickExcluir = "return false";			 
}else{
	$imgExcluir		= "/imagens/excluir.gif";			 
	$onclickExcluir = "excluirParceiro(' || mepar.mepid || ');";	
}
?>
</td>
<td width="3%" valign="top">

<?php 
if($docid) {
	// Barra de estado atual e ações e Historico
	wf_desenhaBarraNavegacao( $docid , array( 'memid' => $_SESSION['memid'] ) );
}
if(($esdid) && ((integer)$esdid == FINALIZADO_ME)) {
	// Recupera o perfil do usuário no Mais Educação.
	$usuPerfil = arrayPerfil();
	
	if(((integer)$vPerfilCadMaisEscola > 0) || in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {
		?>	 
			<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
			<tr style="background-color: #c9c9c9; text-align:center;">
			<td style="font-size:7pt; text-align:center;">
			<span title="relatorio_plano_escola"><b>relatório</b></span>
			</td>
			</tr>
			<tr style="text-align:center;">
				<td style="font-size:7pt; text-align:center;">
					<span title="relatorio_plano_escola"><a href="#" alt="Ver o relatório Plano de Atendimento da Escola" title="Ver o relatório Plano de Atendimento da Escola" onclick="abreRelatorio();">Plano de Atendimento da Escola</a></span>
				</td>
			</tr>
			
			<script type="text/javascript">
			function abreRelatorio() {
				var janela = window.open('pdeescola.php?modulo=merelatorio/plano_atendimento_escola&acao=A', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
				janela.focus();
			}
			</script>
		<? 
	}
} 

if($_SESSION['exercicio'] < 2013){
	?>
		<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
		<tr  style="text-align:center;">
			<td>
				<a style="cursor: pointer"  onclick="abrirRelEstimativa();" >
					<img src="/imagens/print.png" border="0" alt="Versão para impressão" >  
					<br> Relatório de Estimativa
				</a>
			</td>
		</tr> 
		</table>
	<?
}
?>

</td> 
</tr>
</table>

<?

$sql = "SELECT 
		'<a style=\"margin: 0 -5px 0 5px;\" href=\"pdeescola.php?modulo=meprincipal/parceiros&acao=A&entid='|| e.entid ||'&mepquantidadealuno=' || mepar.mepquantidadealuno ||'\" \"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
 		 &nbsp;<a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"$onclickExcluir\" style=\"cursor:hand\"><img src=\"$imgExcluir\" border=0 title=\"Excluir\"></a>' as acao,
		'<a style=\"margin: 0 -5px 0 5px;\" href=\"pdeescola.php?modulo=meprincipal/parceiros&acao=A&entid='|| e.entid ||'&mepquantidadealuno=' || mepar.mepquantidadealuno ||'\">' || e.entnome ||'</a>' as entnome,
		'<center>' || mepar.mepquantidadealuno || '</center>',
		mepar.memid
		FROM pdeescola.meparceiro mepar
		inner join entidade.entidade e on mepar.entid = e.entid
		inner join entidade.funcaoentidade fe on fe.entid = e.entid
		WHERE mepar.memid = ". $_SESSION["memid"] ." and fe.funid IN ('".FUN_PARCEIRO_ME_PF."','".FUN_PARCEIRO_ME_PJ."') order by mepar.mepid";

$arMeParceiro = $db->carregar($sql);
$cabecalho = array("Ação","Nome","Quantidade de Aluno");
$db->monta_lista_array($arMeParceiro, $cabecalho, 50, 20, '', 'center','');
echo "<br />";
?>
<script type="text/javascript">
var tipo_entidade = <?php echo $campo_tipo_entidade; ?>;
if(tipo_entidade == 2){
	$('cnpj').checked = true;
} else {
	$('cpf').checked = true;
}
document.getElementById('tblentidade').style.width = '100%';
var linhanovo = document.getElementById('tblentidade').insertRow((document.getElementById('tblentidade').rows.length-1));
coluna0 = linhanovo.insertCell(0);
coluna0.className = "SubTituloDireita";
coluna0.innerHTML = "Quantidade :";
coluna1 = linhanovo.insertCell(1);
coluna1.innerHTML = "<input type='text' name='mepquantidadealuno' id='mepquantidadealuno' class='CampoEstilo' onkeypress='return somenteNumeros(event);' value='<? echo $_REQUEST['mepquantidadealuno']; ?>'>";


<?
/*
 * Quando for pessoa juridica filtrar alguns campos do formulario
 */
switch($campo_tipo_entidade) {
	case 1:
?>
	/*
	 * DESABILITANDO O NOME DA ENTIDADE
	 */
	document.getElementById('entnome').readOnly = true;
	document.getElementById('entnome').className = 'disabled';
	document.getElementById('entnome').onfocus = "";
	document.getElementById('entnome').onmouseout = "";
	document.getElementById('entnome').onblur = "";
	document.getElementById('entnome').onkeyup = "";

	$('frmEntidade').onsubmit  = function(e) {
		if (trim($F('entnumcpfcnpj')) == '') {
			alert('CPF é obrigatório.');
	    	return false;
		}
		if (trim($F('entnome')) == '') {
			alert('O nome da entidade é obrigatório.');
			return false;
		}
		if (trim($F('entdatanasc')) != '') {
			if(!validaData(document.getElementById('entdatanasc'))) {
				alert("Data de nascimento é inválida.");
				return false;
			}
		}
		return true;
	}
<?
	break;
	case 2:
?>
	document.getElementById('tr_entnuninsest').style.display = 'none';
	document.getElementById('tr_entungcod').style.display = 'none';
	document.getElementById('tr_tpctgid').style.display = 'none';
	document.getElementById('tr_entunicod').style.display = 'none';
	document.getElementById('tr_entcodent').style.display = 'none';
	/*
	 * DESABILITANDO O NOME DA ENTIDADE
	 */
	document.getElementById('entnome').readOnly = true;
	document.getElementById('entnome').className = 'disabled';
	document.getElementById('entnome').onfocus = "";
	document.getElementById('entnome').onmouseout = "";
	document.getElementById('entnome').onblur = "";
	document.getElementById('entnome').onkeyup = "";

	$('frmEntidade').onsubmit  = function(e) {
		if (trim($F('entnumcpfcnpj')) == '') {
			alert('CPF é obrigatório.');
	    	return false;
		}
		if (trim($F('entnome')) == '') {
			alert('O nome da entidade é obrigatório.');
			return false;
		}
		return true;
	}
<?
	break;
}
?>

function tipoEntidade(campo_tipo_entidade){
	window.location.href = "/pdeescola/pdeescola.php?modulo=meprincipal/parceiros&acao=A&campo_tipo_entidade="+campo_tipo_entidade;
}
 
function abrirRelEstimativa(){

			var req = new Ajax.Request('pdeescola.php?modulo=meprincipal/parceiros&acao=A', {
							        method:     'post',
							        parameters: 'rel_estimativa=t',							         
							        onComplete: function (res) { 
										return window.open('pdeescola.php?modulo=merelatorio/meresultado_estimativa&acao=A&entid='+res.responseText+'&tipo=html',
							                  'estima',
							                  'height=420,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
									}
				});
		} 
function excluirParceiro(idParceiro){
	if(confirm('Deseja Realmente excluir este Parceiro')){
		window.location.href = "/pdeescola/pdeescola.php?modulo=meprincipal/parceiros&acao=A&idParceiroExcluir="+idParceiro;
		return true;
	} else {
		return false;
	}
}

function popupMapa(entid){
	window.open('pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
}

</script>