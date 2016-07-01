<?php
include_once APPRAIZ."www/brasilpro/_funcoes_subacao_obras.php";
$sbaid = $_REQUEST['sbaid'] ? $_REQUEST['sbaid'] : $_SESSION['brasilpro']['obras']['sbaid'];
if( $_REQUEST['preid'] )
{
	$sbaid = $db->pegaUm("SELECT sbaid FROM cte.subacaoobra WHERE preid = ".$_REQUEST['preid']);
	if( $sbaid ){
		$_SESSION['brasilpro']['obras']['sbaid'] = $sbaid;
	}
	
	$muncod = $db->pegaUm("SELECT muncod FROM obras.preobra WHERE preid = ".$_REQUEST['preid']);
	if( $sbaid ){
		$_SESSION['brasilpro']['muncod'] = $muncod;
	}
}

$_SESSION['brasilpro']['obras']['sbaid'] = $sbaid;

if(!$sbaid){
	echo "<script>alert('Subação não encontrada!');window.close();</script>";
	die;
}

$ano = $_REQUEST['ano'] ? $_REQUEST['ano'] : $_SESSION['brasilpro']['obras']['ano'];
$ano = $ano ? $ano : $db->pegaUm("SELECT sobano FROM par.subacaoobra WHERE preid = ".$_REQUEST['preid']);
$_SESSION['brasilpro']['obras']['ano'] = $ano;

$_SESSION['brasilpro']['preid'] = $preid = $_REQUEST['preid'];

$habilitado = 'S';
$travaCorrecao = false;
$boAtivo = 'S';

if($_REQUEST['preid']){
	if( !testaOrigem( $preid ) ){
		echo "<script>
				alert('Obra inválida.');
				window.close();
			  </script>";
	}
	$_SESSION['brasilpro']['esfera'] = pegaEsfera( $preid );
	$obSubacaoControle = new SubacaoControle();
	$obPreObra = new PreObra();
	$arDados = $obSubacaoControle->recuperarPreObra($preid);
	$docid = prePegarDocid($preid);
	$esdid = prePegarEstadoAtual($docid);
	
	if( $esdid ){
		$arrPerfil = array(CTE_PERFIL_ADMINISTRADOR,
						   CTE_PERFIL_SUPER_USUARIO,
						   CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
						   CTE_PERFIL_EQUIPE_LOCAL);
		$arrSituacao = array(WF_EM_CADASTRAMENTO, WF_TIPO_EM_CORRECAO);
		
		if( !in_array($esdid, $arrSituacao) || !possuiPerfil($arrPerfil) ){
			$bloq = true;
			$boAtivo = 'N';
			$stAtivo = 'disabled="disabled"';
			$habilitado = 'N';
			$travaCorrecao = true;
		}
	}
}


require_once APPRAIZ ."includes/classes/fileSimec.class.inc";

if($_POST['ajaxObras']){
	$_POST['ajaxObras']($_POST);
}
if($_POST['requisicao']){
	$_POST['requisicao']();
}
if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	die;
}

/** Carrega informações de localização e dados da subação  **/
$sql = "SELECT
		    dim.dimid,
		    dim.dimcod,
		    dim.dimdsc,
		    are.ardid,
		    are.ardcod,
		    are.arddsc,
		    ind.indid,
		    ind.indcod,
		    ind.inddsc,
		    aci.acidsc,
		    aci.acilocalizador,
		    pto.ptoid
		FROM
		    cte.acaoindicador aci
		INNER JOIN cte.pontuacao pto on pto.ptoid = aci.ptoid and pto.ptostatus = 'A'
		INNER JOIN cte.indicador ind on ind.indid = pto.indid
		INNER JOIN cte.subacaoindicador sbi ON sbi.aciid = aci.aciid
		INNER JOIN cte.areadimensao are on are.ardid = ind.ardid
		INNER JOIN cte.dimensao dim on dim.dimid = are.dimid
		WHERE
		    sbi.sbaid = ".$sbaid;

$subacao = $db->pegaLinha($sql);

?>
<html>
	<head>
	    <meta http-equiv="Cache-Control" content="no-cache">
	    <meta http-equiv="Pragma" content="no-cache">
	    <meta http-equiv="Connection" content="Keep-Alive">
	    <meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
	    <title>PAR 2010 - Plano de Metas - Subação</title>
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	</head>
	<body>
		<script type="text/javascript">
			function salvarObras()
			{
				var erro = 0;
				$("[class~=obrigatorio]").each(function() { 
					if(!this.value || this.value == "Selecione..."){
						erro = 1;
						alert('Favor preencher todos os campos obrigatórios!');
						this.focus();
						return false;
					}
				});
				if(erro == 0){
					$("#form_subacao").submit();
				}
			}
		</script>
			<table class="tabela" style="margin-bottom:15px" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td colspan="2" style="color: blue; font-size: 22px">
						<a href="#">
							<?php $entidadePar = ($_SESSION['brasilpro']['itrid'] == 3) ? $_SESSION['brasilpro']['estuf'] : $_SESSION['brasilpro']['muncod']; ?>
							<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['brasilpro']['itrid']); ?>
						</a>
					</td>
				</tr>
				<tr>
						<td class="SubTituloDireita">Dimensão:</td>
						<td><?php echo $subacao['dimcod'] , '. ' , $subacao['dimdsc'] ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Área:</td>
					<td><?php echo $subacao['dimcod'] , '.' , $subacao['ardcod'] , '. ' , $subacao['arddsc']; ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Indicador:</td>
					<td>
						<?php echo $subacao['dimcod'] , '.' , $subacao['ardcod'] , '.' , $subacao['indcod']   , '. ' , $subacao['inddsc']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Ação:</td>
					<td><?php echo $subacao['acidsc']; ?></td>
				</tr>
			</table>
			<?php $abaParObras = $_GET['aba'] ? "subacaoObras".$_GET['aba'] : null ?>
			<?php $stPaginaAtual = "brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&sbaid=$sbaid&ano=$ano".($preid ? "&preid=$preid" : "").($abaParObras ? "&aba=".$_GET['aba'] : "" ) ?>
			<?=carregaAbasSubacaoObras($stPaginaAtual, Array('preid' => $preid, 'sbaid' => $sbaid, 'ano' => $ano ) )?>
			<?php include_once(APPRAIZ."brasilpro/modulos/principal/obras/".($abaParObras ? $abaParObras : "subacaoObrasDados").".inc" ) ?>
	</body>
</html>