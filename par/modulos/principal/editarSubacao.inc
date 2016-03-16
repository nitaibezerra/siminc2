<?php
function salvarSubacao()
{
	$arCamposNulo = array();
	$oAcao = new Subacao();
	$oAcao->sbaid = $_POST['sbaid'];
	if( empty($_POST['foaid'])  ) $arCamposNulo = array( 'foaid' ); else $oAcao->foaid = $_POST['foaid'];
	$oAcao->sbadsc = $_POST['sbadsc'];
	$oAcao->frmid = $_POST['frmid'];
	$oAcao->undid = $_POST['undid'];
	$oAcao->prgid = $_POST['prgid'];
	$oAcao->sbacronograma = $_POST['sbacronograma'];
	$oAcao->sbaestrategiaimplementacao = $_POST['sbaestrategiaimplementacao'];
	$oAcao->salvar(true, true, $arCamposNulo);
	$oAcao->commit();
	
	$oAcao->carregarPorId($_POST['sbaid']);
	if($oAcao->ppsid){
		$oPropostaSubacao = new PropostaSubacao();
		$oPropostaSubacao->carregarPorId($oAcao->ppsid);
		$oPropostaSubacao->ppscronograma = $_POST['sbacronograma'];
		$oPropostaSubacao->salvar();
		$oPropostaSubacao->commit();
	}
	
	echo "<script>
				alert('Operação realizada com sucesso!');
				parent.window.opener.location.href = parent.window.opener.location;
				window.close;
			</script>";
	exit;
}

if($_POST['requisicao']){
	$_POST['requisicao']();
}

$sbaid = $_REQUEST['sbaid'];
if(!$sbaid){
	echo "<script>alert('Subação não encontrada!');window.close();</script>";
}
/** Carrega informações de localização e dados da subação  **/
$sql = "SELECT  s.sbaid,
				s.sbadsc,
				s.ptsid,
				s.foaid,
				d.dimid,
				d.dimcod,
				d.dimdsc,
				ar.areid,
				ar.arecod,
				ar.aredsc,
				i.indcod,
				i.indid,
				i.inddsc,
				a.acidsc,
				p.ptoid,
				s.sbaestrategiaimplementacao,
				s.undid,
				s.prgid,
				s.frmid,
				s.sbacronograma
		FROM par.subacao 	 	 s
		INNER JOIN par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus = 'A'
		INNER JOIN par.pontuacao p  ON p.ptoid  = a.ptoid AND p.ptostatus = 'A'
		INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus = 'A' 
		INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
		INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
		INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
		LEFT JOIN par.propostasubacao pps ON pps.ppsid = s.ppsid
		WHERE s.sbastatus = 'A' AND s.sbaid = $sbaid";

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
	<body onunload="window.opener.location.reload();">
		<script type="text/javascript">
			function salvarSubacao()
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
		<form name="form_subacao" id="form_subacao" method="post" action="" >
			<input type="hidden" name="sbaid" value="<?php echo $sbaid ?>" />
			<input type="hidden" name="requisicao" value="salvarSubacao" />
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td colspan="2" style="color: blue; font-size: 22px">
						<a href="#">
							<?php $entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod']; ?>
							<?php echo EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar, $_SESSION['par']['itrid']); ?>
						</a>
					</td>
				</tr>
				<tr>
						<td class="SubTituloDireita">Dimensão:</td>
						<td><?php echo $subacao['dimcod'] , '. ' , $subacao['dimdsc'] ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Área:</td>
					<td><?php echo $subacao['dimcod'] , '.' , $subacao['arecod'] , '. ' , $subacao['aredsc']; ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Indicador:</td>
					<td>
						<?php echo $subacao['dimcod'] , '.' , $subacao['arecod'] , '.' , $subacao['indcod']   , '. ' , $subacao['inddsc']; ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Ação:</td>
					<td><?php echo $subacao['acidsc']; ?></td>
				</tr>
				<tr style="background-color: #cccccc;">
					<td align="left" colspan="2"><strong>Dados da Subação</strong></td>
				</tr>
				<!-- 
				<tr>
					<td class="SubTituloDireita">Forma de atendimento:</td>
					<td>
						<?php // $sql = "select foaid as codigo, foadescricao as descricao from par.formaatendimento where foastatus = 'A' order by descricao"; ?>
						<?php // $foaid = $subacao['foaid']; $db->monta_combo("foaid",$sql,"S","Selecione...","","","","","N") ?>
					</td>
				</tr>
				 -->
				<tr>
					<td width="25%" class="SubtituloDireita" >Descrição da Subação:</td>
					<td><?php $sbadsc = $subacao['sbadsc']; echo campo_textarea('sbadsc',"S","S","",90,5,"","") ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Estratégia de Implementação:</td>
					<td><?php $sbaestrategiaimplementacao = $subacao['sbaestrategiaimplementacao']; echo campo_textarea('sbaestrategiaimplementacao',"S","S","",90,5,"","") ?></td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Programa:</td>
					<td>
						<?php $sql = "select prgid as codigo, prgdsc as descricao from par.programa where prgstatus = 'A' order by descricao"; ?>
						<?php $prgid = $subacao['prgid']; $db->monta_combo("prgid",$sql,"S","Selecione...","","","","200px","S") ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Unidade de Medida:</td>
					<td>
						<?php $sql = "select undid as codigo, unddsc as descricao from par.unidademedida where undstatus = 'A' order by descricao"; ?>
						<?php $undid = $subacao['undid']; $db->monta_combo("undid",$sql,"S","Selecione...","","","","","S") ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Forma de Execução:</td>
					<td>
						<?php $sql = "select frmid as codigo, frmdsc as descricao from par.formaexecucao order by descricao"; ?>
						<?php $frmid = $subacao['frmid']; $db->monta_combo("frmid",$sql,"S","Selecione...","","","","","S") ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Cronograma:</td>
					<td>
						<?php $arrCronograma = array( 0 => array("codigo" => 1, "descricao" => "Global"), 1 => array("codigo" => 2, "descricao" => "Escola")  ) ?>
						<?php $sbacronograma = $subacao['sbacronograma']; $db->monta_combo("sbacronograma",$arrCronograma,"S","Selecione...","","","","","S") ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita"></td>
					<td class="SubTituloEsquerda" >
						<input type="button" value="Salvar" name="btn_salvar" onclick="salvarSubacao()" />
						<input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>