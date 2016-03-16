<?
include APPRAIZ ."includes/workflow.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

require_once APPRAIZ . "fabrica/classes/HistoricoPausaServico.class.inc";
$oHistoricoPausaServico = new HistoricoPausaServico();

echo '<br>';

if($_REQUEST['scsid']) {
	$_SESSION['fabrica_var']['scsid'] = $_REQUEST['scsid'];
}

// resgatando o estado da solicitação
$estado_wf = wf_pegarEstadoAtual($db->pegaUm("SELECT docid FROM fabrica.solicitacaoservico WHERE scsid='".$_SESSION['fabrica_var']['scsid']."'"));

// se o estado não for em analise redirecionar para lista inicial
if($estado_wf['esdid'] != WF_ESTADO_ANALISE) {
    ob_clean();
    header("location:fabrica.php?modulo=principal/listarSolicitacoes&acao=A");
    exit;
}



//permissao para edição da tela
 $habil = 'S';
if(!verificaPermissaoEdicao()) $habil = 'N';



/**** somente super usuário, fiscal, gestor podem inserir/alterar uma analise ****/
$pfls = arrayPerfil();

if(in_array(PERFIL_SUPER_USUARIO, $pfls) ||
   in_array(PERFIL_FISCAL_CONTRATO,  $pfls) ||
   in_array(PERFIL_GESTOR_CONTRATO,  $pfls) ||
   in_array(PERFIL_ADMINISTRADOR, $pfls)) {

     $habil = 'S';

} else {

	 $habil = 'N';
}


if(in_array(PERFIL_SUPER_USUARIO, $pfls) ||
   in_array(PERFIL_GESTOR_CONTRATO,  $pfls) ||
   in_array(PERFIL_ADMINISTRADOR, $pfls)) {

   	$permitirDataMaior = true;

} else {

	$permitirDataMaior = false;
}



$sql = "SELECT s.docid, a.tpsid, a.ctrid, a.ansid, a.ansgarantia, a.ansdsc, a.ansprevinicio, a.ansprevtermino, a.ansqtdpf, a.ansambienteweb, to_char(a.ansdtrecebimento, 'dd/mm/YYYY') as ansdtrecebimento
		FROM fabrica.analisesolicitacao a
		LEFT JOIN fabrica.solicitacaoservico s ON s.scsid=a.scsid
		WHERE a.scsid='".$_SESSION['fabrica_var']['scsid']."'";

$analisesolicitacao = $db->pegaLinha($sql);

if($analisesolicitacao) {
	$tpsid = $analisesolicitacao['tpsid'];
	$ansgarantia = $analisesolicitacao['ansgarantia'];
	$ansdsc = $analisesolicitacao['ansdsc'];
	$ansprevinicio = $analisesolicitacao['ansprevinicio'];
	$ansprevtermino = $analisesolicitacao['ansprevtermino'];
	$ansqtdpf = $analisesolicitacao['ansqtdpf'];
	$ansdtrecebimento = $analisesolicitacao['ansdtrecebimento'];
	$ctrid = $analisesolicitacao['ctrid'];
	$ansambienteweb = $analisesolicitacao['ansambienteweb'];
	$ansid = $analisesolicitacao['ansid'];

	$requisicao = "atualizarAnaliseSolicitacaoServico";

} else {



	//insere analise
	$sql = "INSERT INTO fabrica.analisesolicitacao(scsid) VALUES ({$_SESSION['fabrica_var']['scsid']}) RETURNING ansid;";
	$ansid = $db->pegaUm($sql);

	$requisicao = "atualizarAnaliseSolicitacaoServico";
	//$requisicao = "inserirAnaliseSolicitacaoServico";
}


//recupera campo misto
$existe1 = $db->pegaUm("select count(fdpid) from fabrica.servicofaseproduto where ansid = $ansid and tpeid in (1)");
$existe2 = $db->pegaUm("select count(fdpid) from fabrica.servicofaseproduto where ansid = $ansid and tpeid in (2)");
if($existe1 > 0 && $existe2 > 0) $mistoAux = 't';
$misto = $_REQUEST['misto'] ? 't' : $mistoAux;



$menu = array(0 => array("id" => 1, "descricao" => "Analisar Solicitação", 	"link" => "/fabrica/fabrica.php?modulo=principal/analiseDemandaCon&acao=A&scsid=".$_SESSION['fabrica_var']['scsid']),
				  1 => array("id" => 2, "descricao" => "Observações", 		 	"link" => "/fabrica/fabrica.php?modulo=principal/cadSSObservacaoCon&acao=A&tipoobs=analiseDemanda"),
				  2 => array("id" => 3, "descricao" => "Anexos da Solicitação",	"link" => "/fabrica/fabrica.php?modulo=principal/analiseDemandaAnexosCon&acao=".$_REQUEST['acao']),
				  3 => array("id" => 4, "descricao" => "Providências", 		 	"link" => "/fabrica/fabrica.php?modulo=principal/providenciasCon&acao=A&tipoaba=analiseDemanda")
			  	  );

echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
$titulo_modulo = "Análise preliminar da Solicitação de Serviço";
monta_titulo($titulo_modulo, 'Fiscal Técnico');

?>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/fabrica.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script>
function checarDatas(){
	if(document.getElementById('ansprevinicio').value && document.getElementById('ansprevtermino').value) {

		var data_atual = '<? echo (($ansdtrecebimento)?$ansdtrecebimento:date("d/m/Y")); ?>';
		var data_1 = document.getElementById('ansprevinicio').value;
		var data_2 = document.getElementById('ansprevtermino').value;

		var comparaatual = parseInt(data_atual.split("/")[2].toString() + data_atual.split("/")[1].toString() + data_atual.split("/")[0].toString());
		var compara01 = parseInt(data_1.split("/")[2].toString() + data_1.split("/")[1].toString() + data_1.split("/")[0].toString());
		var compara02 = parseInt(data_2.split("/")[2].toString() + data_2.split("/")[1].toString() + data_2.split("/")[0].toString());

		if (compara01 > compara02) {
			alert('Data de início não pode ser maior que a data de termino');
			return false;
		}

		if (comparaatual > compara01) {
			alert('As datas de previsão não podem ser retroativas');
			return false;
		}

		if (comparaatual > compara02) {
			alert('As datas de previsão não podem ser retroativas');
			return false;
		}

	}

	return true;

}

function limpar() {
	var form = document.formulario;

	for(i=0;i<form.elements.length;i++) {
		if(form.elements[i].type) {
			if(form.elements[i].type == "radio" && form.elements[i].name.substr(0,5)=="dspid") {
				form.elements[i].checked = false;
			}
		}
	}

	atualizarConsultaProdutos();

}

function IsGarantia(valor) {

	if(valor == "TRUE") {
		if(document.getElementById('sidid').value == "") {
			alert('Selecione um sistema');
			document.getElementsByName('ansgarantia')[1].checked='checked';
			return false;
		}
		document.getElementById('tr_osfinalizadas').style.display = "";
		document.getElementById('td_osfinalizadas').innerHTML = "Carregando...";

		$.ajax({
	   		type: "POST",
	   		url: "fabrica.php?modulo=principal/analiseDemanda&acao=A",
	   		data: "requisicao=pegarOsProjetoFinalizada&sidid="+document.getElementById('sidid').value,
	   		success: function(msg){
	   			document.getElementById('td_osfinalizadas').innerHTML = msg;
	   		}
	 		});

	 } else {

		document.getElementById('td_osfinalizadas').innerHTML = "";
		document.getElementById('tr_osfinalizadas').style.display = "none";

	 }
}

function filtraContrato(sidid)
{
 	if(sidid){
 		$.ajax({
   		type: "POST",
   		url: "fabrica.php?modulo=principal/analiseDemanda&acao=A",
   		data: "requisicao=filtraContratoPorSistema&sidid="+sidid,
   		success: function(msg){
   			document.getElementById('td_contrato').innerHTML = msg;
   		}
 		});
 	}
}


function mostraDisciplina()
{
	if(confirm('Na alteração do valor deste campo, apagará todo quadro de artefatos desta SS. \n\nDeseja realmente alterar este campo? '))
	{
		/*
		var fiscal = document.getElementById( 'fiscal' );
		selectAllOptions( fiscal );
		*/

		var tpsid = document.getElementById('tpsid').value;

		if(tpsid == '6'){
			document.getElementById('tr_disciplinas').style.display = "none";
			document.getElementById('divMisto').style.display = "none";
		}
		else{

			document.getElementById('tr_disciplinas').style.display = "";
			document.getElementById('divMisto').style.display = "";

			var ansid = "<?=$ansid?>";
			var misto = document.formulario.misto.checked;

			if(misto) misto = '1';
			else misto = '';

			$.ajax({
		   		type: "POST",
		   		url: "fabrica.php?modulo=principal/analiseDemanda&acao=A",
		   		data: "requisicao=inserirArtefatos&ansid="+ansid+"&misto="+misto+"&tpsid="+tpsid,
		   		success: function(msg){
			   		//alert(msg);
			   		//document.getElementById('divMisto').innerHTML = msg;
					if(msg == 'OK'){
						document.formulario.requisicao.value='';
		   				document.formulario.submit();
		   			}
		   			//document.getElementById('td_osfinalizadas').innerHTML = msg;
		   		}
		 		});

		}
	}
	else{
		document.getElementById('tpsid').value = "<?=$tpsid?>";
		var mistoAux = "<?=$misto?>";

		if(mistoAux == 't') document.formulario.misto.checked = true;
		else document.formulario.misto.checked = false;

	}
}




<? if($ansgarantia == "t") : ?>
$(document).ready(function() {
	IsGarantia('TRUE');
});
<? endif; ?>

</script>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="<? echo $requisicao; ?>">
<input type="hidden" name="ansid" value="<? echo $ansid; ?>">
<?
telaCabecalhoSolicitacaoServico(array("scsid" => $_SESSION['fabrica_var']['scsid']));
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td width="95%">
	<table class="listagem" width="100%">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">Detalhamento da solicitação de serviço</td>
	</tr>
	<tr>
		<td width="25%" class="SubtituloDireita">Sistema:</td>
		<td>
			<?php

				if(!$sidid){
					if($_SESSION['fabrica_var']['scsid']){
						$sql = "SELECT sidid FROM fabrica.solicitacaoservico WHERE scsid=".$_SESSION['fabrica_var']['scsid'];
						$sidid = $db->pegaUm($sql);
					}
				}

				$sql = "select
							sidid as codigo,
							siddescricao as descricao
						from
							demandas.sistemadetalhe
						where
							sidstatus = 'A'
						order by
							siddescricao";
				$db->monta_combo("sidid",$sql,$habil,"Selecione...","","","","","S","sidid","",$sidid);

				?>
				<input type="hidden" name="ctrid" id="ctrid" value="<?php echo $ctrid ?>">
		</td>
	</tr>
	<tr>
		<td class="SubtituloDireita">Fiscal Responsável:</td>
		<td>
		<?php
			$ctrid = $db->pegaUm("select ctrid from fabrica.analisesolicitacao where ansid = $ansid");
			if($ctrid) $where = "where ctrid = $ctrid";

			$sql_combo = "select distinct
							u.usucpf as codigo,
							u.usunome as descricao
						from
							fabrica.fiscalcontrato f
						inner join
							seguranca.usuario u ON u.usucpf = f.usucpf
						$where
						order by 2";

			if( $_SESSION['fabrica_var']['scsid'] )
			{
				/*** Se existem fiscais cadastrados ***/
				if( $db->pegaUm("SELECT count(1) FROM fabrica.fiscalsolicitacao WHERE scsid = ".$_SESSION['fabrica_var']['scsid']) > 0 )
				{
					$sql_carregados = "SELECT
									 		u.usucpf as codigo --,u.usunome as descricao
									   FROM
									   		fabrica.fiscalsolicitacao fs
									   INNER JOIN
									   		seguranca.usuario u ON u.usucpf = fs.usucpf
									   WHERE
									   		fs.scsid = ".$_SESSION['fabrica_var']['scsid'];
					$fiscal = $db->pegaUm( $sql_carregados );


				}


				if($_REQUEST['fiscal']){

					$sql_carregados = "SELECT
									 		u.usucpf as codigo--,	u.usunome as descricao
									   FROM
									   		seguranca.usuario u
									   WHERE
									   		u.usucpf in ('".$_REQUEST['fiscal']."')";
					$fiscal = $db->pegaUm( $sql_carregados );
				}

			}

			//combo_popup( 'fiscal', $sql_combo, 'Selecione o(s) Fiscal(is)', '400x400', 1, array(), '', 'S', true, true, 5, 400, null, null, null, null, null, true, true, null, true );
			$db->monta_combo("fiscal",$sql_combo,$habil,"Selecione...","","","","","S","fiscal","",$fiscal);

		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo de serviço:</td>
		<td id="td_tpsid">

			<div id="divTipoServico" style="float:left">
				<?php // if($tpsid){
					$tpsid = $_REQUEST['tpsid'] ? $_REQUEST['tpsid'] : $tpsid;
					$sql = "SELECT tpsid as codigo, tpsdsc as descricao FROM fabrica.tiposervico WHERE tpsstatus='A' ORDER BY tpsdsc";
					$db->monta_combo('tpsid', $sql, $habil, 'Selecione', 'mostraDisciplina', '', '', '', 'S', 'tpsid');
				//}else{ ?>
				<!--
					<select name="tpsid" id="tpsid" disabled="disabled" onclick="alert('Favor selecionar o sistema!')" >
						<option>Selecione...</option>
					</select>
				 -->
				<?php //} ?>

				&nbsp;&nbsp;
			</div>

			<div id="divMisto">
				<input type="checkbox" name="misto" <?= (($misto == 't') ? 'checked="checked"' : '') ?> <? echo (($habil=='S')?"":"disabled"); ?> title="Misto." onclick="mostraDisciplina();"> Misto
			</div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
			<img src="../imagens/ajuda.gif" title="Criação e manutenção de sítios e portais, layout's, imagens, multimídia, infográficos e ilustrações." />
			Ambiente WEB:
		</td>
		<td>
			<input type="checkbox" name="ansambienteweb" <?= (($ansambienteweb == 't') ? 'checked="checked"' : '') ?> <? echo (($habil=='S')?"":"disabled"); ?> title="Criação e manutenção de sítios e portais, layout's, imagens, multimídia, infográficos e ilustrações." /> Marque se a solicitação se refere a criação de sítios e portais
		</td>
	</tr>
	<?php
	$mostraDisciplina = "";
	if($tpsid == '6') $mostraDisciplina = "none";
	?>
	<tr id="tr_disciplinas" style="display: <?=$mostraDisciplina?>">
		<td class="SubTituloDireita">Disciplinas contratadas:</td>
		<td>
		<?
		$sql = "SELECT dspid, dspdsc FROM fabrica.disciplina WHERE dspstatus='A'";
		$disciplinas = $db->carregar($sql);

		// sera utilizado para criar as colunas com os tipo de execução por disciplina
		$sql = "SELECT tpeid, tpedsc FROM fabrica.tipoexecucao WHERE tpestatus='A'";
		$tipoexecucao = $db->carregar($sql);

		?>
		<table class="listagem" width="100%">
		<tr>
			<td class="SubTituloCentro">Disciplina</td>
			<?
			if($tipoexecucao[0]) {
				foreach($tipoexecucao as $tpe) {
					echo "<td class=SubTituloCentro>".$tpe['tpedsc']."</td>";
				}
			}
			?>
		</tr>
		<?
		if($disciplinas[0])
		{
			foreach($disciplinas as $disciplina)
			{
				echo "<tr>
					  	<td class=SubTituloDireita height='30px' nowrap>
					  		".$disciplina['dspdsc']."
					  	</td>";

				if($tipoexecucao[0])
				{
					foreach($tipoexecucao as $tpe)
					{
						$params 				= array();
						$params['nome']			= $ansid.'_'.$disciplina['dspid'].'_'.$tpe['tpeid'];
						$params['valueButton']	= '+ Associar mais artefatos';
						$params['titulo']		= 'Produtos Esperados';

						echo '<td style="text-align:left;" width="50%">';
						//popLista($params);

						/*** Se algum parâmetro foi criado... ***/
						if( !empty($params) )
						{
							/*** ***/
							if( $params['nome'] )
							{
								/*** Cria a variável com o value do botão. Se não houver sido informada, usa-se o padrão ***/
								$valueButton = ( $params['valueButton'] ) ? $params['valueButton'] : 'Abrir';

								/*** Imprime o botão que abrir a pop-up. (Arquivo: www/geral/popLista.php) ***/
								if($habil=='S') {
									$divBotaoSim = '<br> <a style=cursor:pointer; onclick="abreListaAnalise(\''.urlencode($params['nome']).'\', \''.urlencode($titulo).'\');"><font title="Clique para associar mais artefatos">'.$valueButton.'</font></a> <br><br>';
									$divBotaoNao = '<a style=cursor:pointer; onclick="abreListaAnalise(\''.urlencode($params['nome']).'\', \''.urlencode($titulo).'\');"><font color="red" title="Clique para associar artefatos">Nenhum artefato foi associado.</font></a>';
								} else {
									$divBotaoSim = '';
									$divBotaoNao = '<font color="red">Nenhum artefato foi associado.</font>';
								}



								if( $ansid )
								{

									$sql = "SELECT distinct f.fasid, f.fasdsc
											FROM fabrica.servicofaseproduto sp
											INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
											INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
											INNER JOIN fabrica.fase f ON f.fasid = fd.fasid
											WHERE sp.ansid = {$ansid}
											order by 1";
									$fase = $db->carregar($sql);

									$listaProdutos = '';

									if($fase){

										for($i=0;$i<=count($fase)-1;$i++){

											$sql = "SELECT p.prddsc
													FROM fabrica.servicofaseproduto sp
													INNER JOIN fabrica.fasedisciplinaproduto fdp ON fdp.fdpid = sp.fdpid
													INNER JOIN fabrica.fasedisciplina fd ON fd.fsdid = fdp.fsdid
													INNER JOIN fabrica.produto p ON p.prdid = fdp.prdid
													WHERE sp.ansid = {$ansid}
													and fd.fasid = {$fase[$i]['fasid']}
													and fd.dspid = {$disciplina['dspid']}
													and sp.tpeid = {$tpe['tpeid']}
													order by 1";
											//dbg($sql,1);
											$produto = $db->carregarColuna($sql);

											if($produto){
												$listaProdutos .= "<b>{$fase[$i]['fasdsc']}</b><br/>";
												foreach($produto as $p) {
													$listaProdutos .= "<span style='padding-left:20px'><img src=../imagens/seta_filho.gif align=absmiddle> ".$p."</span><br/>";
												}
											}

										}

									}


								}




								if($listaProdutos) {
									$mostraBotaoSim = '';
									$mostraBotaoNao = 'none';
								}
								else{
									$mostraBotaoSim = 'none';
									$mostraBotaoNao = '';
								}

								//imprime as divs
								echo '<div id="'.$disciplina['dspid'].'_'.$tpe['tpeid'].'">'.$listaProdutos.'</div>';
								echo '<div style="text-align:left;display:'.$mostraBotaoSim.';" id="botaosim_'.$disciplina['dspid'].'_'.$tpe['tpeid'].'">'.$divBotaoSim.'</div>';
								echo '<div style="text-align:center;display:'.$mostraBotaoNao.';" id="botaonao_'.$disciplina['dspid'].'_'.$tpe['tpeid'].'">'.$divBotaoNao.'</div>';

							}
						}

						echo '</td>';
					}
				}

				echo '</tr>
					  <tr id="tr_disciplina_'.$disciplina['dspid'].'" style="display:none;">
						<td class="SubTituloDireita" valign="middle"><b>Produtos</b></td>
						<td id="td_disciplina_'.$disciplina['dspid'].'_1" style="text-align:center;">dfsdsfdsf<br />dfdsfsdfdsfdsf<br />dfdsfsdf</td>
						<td id="td_disciplina_'.$disciplina['dspid'].'_2" style="text-align:center;">sdfdsfdsf</td>
					  </tr>';
			}
		}
		else
		{
			echo "<tr>
					<td class=SubTituloEsquerda colspan=".(count($tipoexecucao)+1).">Não existem disciplinas cadastradas</td>
				  </tr>";
		}
		?>
		</table>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Serviço em garantia?</td>
		<td><input type="radio" name="ansgarantia" value="TRUE" <? echo (($ansgarantia=="t")?"checked ":""); echo $disabled_; ?> <? echo (($habil=='S')?"":"disabled"); ?> onclick="if(this.checked){IsGarantia(this.value);}" > Sim <input type="radio" name="ansgarantia" value="FALSE" <? echo (($ansgarantia=="f"||!$ansgarantia)?"checked ":""); echo $disabled_; ?> <? echo (($habil=='S')?"":"disabled"); ?> onclick="if(this.checked){IsGarantia(this.value);}" > Não</td>
	</tr>
	<tr id="tr_osfinalizadas" style="display:none;">
		<td class="SubTituloDireita">O.S. garantia:</td>
		<td id="td_osfinalizadas"></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição detalhada:</td>
		<td><? echo campo_textarea( 'ansdsc', 'S', $habil, '', '70', '4', '5000'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Previsão de início:</td>
		<td><? echo campo_data2('ansprevinicio','S', empty($ansprevtermino) ? $habil : 'N', 'Previsão de início', 'S' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Previsão de Término:</td>
		<td>
                    <? echo campo_data2('ansprevtermino','S', empty($ansprevtermino) ? $habil : 'N', 'Previsão de Término', 'S' ); ?>
                    <?php if(!empty($ansprevtermino) && $habil == 'S'): ?>
                    <script type="text/javascript">
                        function alterarPrevisao(){

                            window.open('?modulo=principal/popup/alterarPrevisaoTermino&acao=A&scsid=<?php echo $_GET['scsid']; ?>', 'alterarPrevisaoTermino', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');

                        }
                    </script>
                    <a href="javascript: alterarPrevisao()"
                       style="text-decoration: underline;">
                        Redefinir Previs&atilde;o de T&eacute;rmino
                    </a>
                    <?php endif; ?>
                </td>
	</tr>

	<!--
	<tr>
		<td class="SubTituloDireita">Qtd. de P.F. Estimado:</td>
		<td><? echo campo_texto('ansqtdpf', 'N', $habil, 'Qtd. de P.F. Estimado', 10, 11, '##########', '', '', '', 0, 'id="ansqtdpf"' ); ?></td>
	</tr>
	 -->
	<tr>
		<td class="SubTituloDireita" colspan="2">
		<? if($habil=='S') : ?>
		<input type="button" name="inseriranalise" value="Salvar" onclick="<? if(!$permitirDataMaior): ?>if(checarDatas()) <? endif; ?> submeterAnaliseSolicitacaoServico();">
		<? endif; ?>
		<input type="button" name="voltar" value="Voltar" onclick="history.back(-1);">
		</td>
	</tr>
	</table>

	</td>
	<td valign="top" width="5%">
	<?
	$dados_wf = array('scsid' => $_SESSION['fabrica_var']['scsid']);
	wf_desenhaBarraNavegacao(pegarDocidSolicitacaoServico($dados_wf), $dados_wf);
	?>

            <?php if(!empty($ansprevtermino)): ?>
            <script type="text/javascript">

                function popPausarServico(){

                    window.open('?modulo=principal/popup/pausarServico&acao=A&scsid=<?php echo $_GET['scsid']; ?>', 'pausarServico', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');

                }

            </script>
            <table border="0" cellpadding="3" cellspacing="0"
                style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
                <tr style="background-color: #c9c9c9; text-align: center;">
                    <td style="font-size: 7pt; text-align: center;">
                        <span title="estado atual">
                            <b>outras ações</b>
                        </span>
                    </td>
                </tr>

                <tr>
                    <td style="font-size: 7pt; text-align: center; border-bottom: 2px solid #c9c9c9;"
			onmouseover="this.style.backgroundColor='#ffffdd';"
			onmouseout="this.style.backgroundColor='';">
                        <a href="javascript: popPausarServico();">
                        <?php if($oHistoricoPausaServico->estadoPausa('',$_GET['scsid']) == 'I'): ?>
                             Retomar SS
                        <?php else: ?>
                             Pausar SS
                        <?php endif; ?>
                        </a>
                    </td>
                </tr>

            </table>
            <?php endif; ?>

	</td>
</tr>
</table>
</form>