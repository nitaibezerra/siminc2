<?
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';

if($_REQUEST['scsid']) {
	$_SESSION['fabrica_var']['scsid'] = $_REQUEST['scsid'];
}

// resgatando o estado da solicitação
$estado_wf = wf_pegarEstadoAtual($db->pegaUm("SELECT docid FROM fabrica.solicitacaoservico WHERE scsid='".$_SESSION['fabrica_var']['scsid']."'"));

// se o estado não for Pré-análise redirecionar para lista inicial
if($estado_wf['esdid'] != WF_ESTADO_PRE_ANALISE && $estado_wf['esdid'] != WF_ESTADO_ANALISE) {
    ?>
    <script type="text/javascript">
        location.href ='fabrica.php?modulo=principal/listarSolicitacoes&acao=A';
    </script>
    <?php
    //header("Location: http://simec-branches-ss752/fabrica/fabrica.php?modulo=principal/listarSolicitacoes&acao=A/");
    exit;
}

//permissao para edição da tela
 $habil = 'S';
if(!verificaPermissaoEdicao()) $habil = 'N';



/**** somente super usuário, fiscal, gestor podem inserir/alterar uma analise ****/
$pfls = arrayPerfil();

if(in_array(PERFIL_SUPER_USUARIO, $pfls) ||
   in_array(PERFIL_GERENTE_PROJETO,  $pfls) ||
   in_array(PERFIL_ADMINISTRADOR,  $pfls) ||
   in_array(PERFIL_GESTOR_CONTRATO,  $pfls) ) {

     $habil = 'S';

} else {

	 $habil = 'N';
}


if(in_array(PERFIL_SUPER_USUARIO, $pfls) ||
   in_array(PERFIL_GERENTE_PROJETO,  $pfls) ||
   in_array(PERFIL_ADMINISTRADOR,  $pfls) ||
   in_array(PERFIL_GESTOR_CONTRATO,  $pfls)) {

   	$permitirDataMaior = true;

} else {

	$permitirDataMaior = false;
}



$sql = "SELECT 
            s.docid, a.mtiid, a.tpsid, a.ctrid, a.vgcid, a.ansid, a.ansgarantia, a.ansdsc, a.ansprevinicio, a.ansprevtermino, a.ansqtdpf
            , to_char(a.ansdtrecebimento, 'dd/mm/YYYY') as ansdtrecebimento, a.mensuravel
		FROM fabrica.analisesolicitacao a
		LEFT JOIN fabrica.solicitacaoservico s ON s.scsid=a.scsid
		WHERE a.scsid='".$_SESSION['fabrica_var']['scsid']."'";
$analisesolicitacao = $db->pegaLinha($sql);

if($analisesolicitacao) {

	$mtiid              = $analisesolicitacao['mtiid'];
	$tpsid              = $analisesolicitacao['tpsid'];
	$ansgarantia        = $analisesolicitacao['ansgarantia'];
	$ansdsc             = $analisesolicitacao['ansdsc'];
	$ansprevinicio      = $analisesolicitacao['ansprevinicio'];
	$ansprevtermino     = $analisesolicitacao['ansprevtermino'];
	$ansqtdpf           = $analisesolicitacao['ansqtdpf'];
	$ansdtrecebimento   = $analisesolicitacao['ansdtrecebimento'];
	$ctrid              = $analisesolicitacao['ctrid'];
	$vgcid              = $analisesolicitacao['vgcid'];
	$ansid              = $analisesolicitacao['ansid'];
    $acaoContrato       = "A";
    $ansmensuravel      = $analisesolicitacao['mensuravel'];
	$requisicao         = "atualizarPreAnaliseSolicitacaoServico";

} else {

	//insere Pré-análise
    $idScsid    = $_SESSION['fabrica_var']['scsid'];
	$sql        = "INSERT INTO fabrica.analisesolicitacao(scsid) VALUES ({$idScsid}) RETURNING ansid;";
    $retorno    = $db->pegaUm($sql);
    $db->commit();
    
    $ansid          = $retorno;

    $acaoContrato   = "I";
	$requisicao     = "atualizarPreAnaliseSolicitacaoServico";
}

//recupera campo misto
$existe1 = $db->pegaUm("select count(fdpid) from fabrica.servicofaseproduto where ansid = $ansid and tpeid in (1)");
$existe2 = $db->pegaUm("select count(fdpid) from fabrica.servicofaseproduto where ansid = $ansid and tpeid in (2)");
if($existe1 > 0 && $existe2 > 0) $mistoAux = 't';
$misto = $_REQUEST['misto'] ? 't' : $mistoAux;



$menu = carregarMenuAnaliseSolicitacao();

// somente mostrar a analise (apagar a aba de gerencia de arquivos e termos)
if($habil=='N') {
	unset($menu[2],$menu[3],$menu[4]);
}

echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
$titulo_modulo = "Pré-análise da Solicitação de Serviço";
monta_titulo($titulo_modulo, 'Gerente de Projetos');

$mostraDisciplina = "";
if($tpsid > 5) $mostraDisciplina = "none";
?>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<!--<script type="text/javascript" src="./js/fabrica.js"></script>-->
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
	/*
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
 	*/
}


function mostraDisciplina()
{
	if(confirm('Na alteração do valor deste campo, apagará todo quadro de artefatos desta SS. \n\nDeseja realmente alterar este campo? '))
	{
		/*
		var fiscal = document.getElementById( 'fiscal' );
		selectAllOptions( fiscal );
		*/

		var vgcid = document.getElementById('vgcid').value;
		var mtiid = document.getElementById('mtiid').value;
		var tpsid = document.getElementById('tpsid').value;

		if( parseInt(tpsid) > 5 ){
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
		   		data: "requisicao=inserirArtefatos&ansid="+ansid+"&misto="+misto+"&tpsid="+tpsid+"&vgcid="+vgcid+"&mtiid="+mtiid,
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
		document.getElementById('vgcid').value = "<?=$vgcid?>";
		document.getElementById('mtiid').value = "<?=$mtiid?>";
		document.getElementById('tpsid').value = "<?=$tpsid?>";
		var mistoAux = "<?=$misto?>";

		if(mistoAux == 't') document.formulario.misto.checked = true;
		else document.formulario.misto.checked = false;

	}
}

function IsMensuravel(valor) {

	if(valor == "TRUE") {
		if(document.getElementById('sidid').value == "") {
			alert('Selecione um sistema');
			document.getElementsByName('ansmensuravel')[1].checked='checked';
			return false;
		}
        }
}

<? if($ansgarantia == "t") : ?>
$(document).ready(function() {
	IsGarantia('TRUE');
});
<? endif; ?>
    
<? if($ansmensuravel == "t") : ?>
$(document).ready(function() {
	IsMensuravel('TRUE');
});
<? endif; ?>    

function abreListaAnalise(nome, titulo)
{
	var arr = nome.split("_");
	
	var ansid = arr[0];
	var dspid = arr[1];
	var tpeid = arr[2];
	
	if(!ansid){
		alert('Sessão expirou! \nFavor entre novamente.');
		return false;
	}

	//var popUp = window.open('fabrica.php?modulo=principal/popListaAnalise&acao=A&nome='+nome+'&titulo='+titulo+'', 'popListaAnalise', 'height=500,width=400,scrollbars=yes,top=50,left=200');
	var popUp = window.open('fabrica.php?modulo=principal/popFaseProdutos&acao=A&ansid='+ansid+'&dspid='+dspid+'&tpeid='+tpeid, 'popListaAnalise', 'height=500,width=800,scrollbars=yes,top=50,left=200');
	popUp.focus();
}

function filtraItem(vgcid){
	if(vgcid){
 		$.ajax({
   		type: "POST",
   		url: "fabrica.php?modulo=principal/analiseDemanda&acao=A",
   		data: "requisicao=filtraItem&vgcid="+vgcid,
   		success: function(msg){
	   			document.getElementById('td_item').innerHTML = msg;
   			}
 		});
 	}
}

function filtraTipoServico(mtiid){
	if(mtiid){
 		$.ajax({
   		type: "POST",
   		url: "fabrica.php?modulo=principal/analiseDemanda&acao=A",
   		data: "requisicao=filtraTipoServico&mtiid="+mtiid,
   		success: function(msg){
	   			document.getElementById('divTipoServico').innerHTML = msg;
   			}
 		});
 	}
}

</script>

<form method="post" name="formulario" id="formulario" enctype="multipart/form-data">
<input type="hidden" name="requisicao" value="<? echo $requisicao; ?>">
<input type="hidden" name="acaoContrato" value="<? echo $acaoContrato; ?>">
<input type="hidden" name="ansid" value="<? echo $ansid; ?>">
<input type="hidden" name="ctrid" value="<?=$ctrid?>">
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
	<?if($estado_wf['esdid'] == WF_ESTADO_ANALISE) { ?>
	<tr>
		<td class="SubtituloDireita">Fiscal Responsável:</td>
		<td>
		<?php
			//$ctrid = $db->pegaUm("select ctrid from fabrica.analisesolicitacao where ansid = $ansid");
			if($ctrid && !$vgcid) $andFiscal = " and v.ctrid = $ctrid ";
			if($vgcid) $andFiscal .= " and v.vgcid = $vgcid ";

			$sql_combo = "select distinct
							u.usucpf as codigo,
							u.usunome as descricao
						from
							fabrica.fiscalvigencia f
						inner join
							fabrica.vigenciacontrato v on v.vgcid = f.vgcid
						inner join
							seguranca.usuario u ON u.usucpf = f.usucpf
						where 
							v.vgcstatus = 'A'
						$andFiscal
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
			$db->monta_combo("fiscal",$sql_combo,$habil,"-- Selecione --","","","","","S","fiscal","",$fiscal);

		?>
		</td>
	</tr>
	<?}?>
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
							sidabrev ||' - '|| siddescricao as descricao
						from
							demandas.sistemadetalhe
						where
							sidstatus = 'A'
						order by
							2";
				$db->monta_combo("sidid",$sql,$habil,"-- Selecione --","","","","","S","sidid","",$sidid);

				?>
				
		</td>
	</tr>
    <tr>
    	<td class="SubTituloDireita">Tipo:</td>
        <td><input type="radio" name="ansmensuravel" value="TRUE" <? echo (($ansmensuravel=="t"||!$ansmensuravel)?"checked ":""); echo $disabled_; ?> <? echo (($habil=='S')?"":"disabled"); ?> onclick="if(this.checked){IsMensuravel(this.value);}" > Mensurável  <input type="radio" name="ansmensuravel" value="FALSE" <? echo (($ansmensuravel=="f")?"checked ":""); echo $disabled_; ?> <? echo (($habil=='S')?"":"disabled"); ?> onclick="if(this.checked){IsMensuravel(this.value);}" > Não Mensurável</td>        
    </tr>
	<tr>
    	<td class="SubtituloDireita" id="contrato" name="contrato" >Contrato:</td>
        <td>
              <?php
	              //recupera contratos antigos
	              if($ctrid && !$vgcid){
	              	 $sql = "select 
								max(vc.vgcid)
							from fabrica.vigenciacontrato vc
							where vc.vgcstatus = 'A' and vc.tsvid in (1,5)";
					$vgcid = $db->pegaUm($sql);
	              }
					
				  $sql = "select 
								vc.vgcid as codigo, 
								(ent.entnome || ' - ' || to_char(vc.vgcdtinicio, 'dd/MM/YYYY') || ' - ' || to_char(vc.vgcdtfim , 'dd/MM/YYYY') ) as descricao
							from fabrica.vigenciacontrato vc
							inner join fabrica.contrato ct on ct.ctrid = vc.ctrid
							inner join entidade.entidade ent on ent.entid = ct.entidcontratado and ct.ctrstatus = 'A' and ct.ctrtipoempresaitem = 1
							where vc.vgcstatus = 'A' and vc.tsvid in (1,5)
							order by vc.vgcdtinicio";
	                    
	              $db->monta_combo("vgcid",$sql,$habil,"-- Selecione --","filtraItem","","","","S","vgcid","",$vgcid); 
              ?>
        </td>
    </tr>
    <tr>
    	<td class="SubtituloDireita" id="item">Item:</td>
        <td id="td_item">
              <?php
              		if($vgcid){
              			
	              		$sql = "select distinct
							                mi.mtiid as codigo,
							                mt.mtcsigla ||' - '|| mi.mtinome as descricao
							          from 
							                fabrica.metricaitem mi
							          inner join fabrica.metrica mt on mt.mtcid = mi.mtcid
							          inner join fabrica.metricaitemcontrato mc on mc.mtiid = mi.mtiid 
							          inner join fabrica.vigenciacontratometricaitem vc on vc.mtiid = mc.mtiid
							          where 
							          	mi.mtistatus = 'A'
							          	AND vc.vgcid = ".$vgcid."
							          ";
	              		
	              		$db->monta_combo("mtiid",$sql, $habil,"-- Selecione --",'filtraTipoServico','','', '', "S", 'mtiid','',$mtiid);
              		}
              		else{
              			?>
              			<select name="mtiid" id="mtiid" disabled="disabled" onclick="alert('Favor selecionar o Contrato!')" >
							<option>Selecione...</option>
						</select>
              			<?	
              		}
              		
              		
               ?>
        </td>
    </tr>
	<tr>
		<td class="SubTituloDireita">Tipo de serviço:</td>
		<td id="td_tpsid">

			<div id="divTipoServico" style="float:left">
				<?  
				if($mtiid){
					$tpsid = $_REQUEST['tpsid'] ? $_REQUEST['tpsid'] : $tpsid;
					
					$sql = "SELECT ts.tpsid as codigo, ts.tpsdsc as descricao 
						    FROM fabrica.tiposervico ts
						    inner join fabrica.vigenciacontratometricaitem vc on vc.mtiid = ts.mtiid
						    WHERE ts.tpsstatus='A' 
						    	AND vc.mtiid = ".$mtiid."
						    group by ts.tpsid, ts.tpsdsc
						    ORDER BY tpsdsc";
					$db->monta_combo('tpsid', $sql, $habil, '-- Selecione --', 'mostraDisciplina', '', '', '', 'S', 'tpsid');
				}else{ ?>
					<select name="tpsid" id="tpsid" disabled="disabled" onclick="alert('Favor selecionar o Item!')" >
						<option>-- Selecione --</option>
					</select>
				<?}?>

				&nbsp;&nbsp;
			</div>

			<div id="divMisto" style="display: <?=$mostraDisciplina?>">
				<input type="checkbox" name="misto" <?= (($misto == 't') ? 'checked="checked"' : '') ?> <? echo (($habil=='S')?"":"disabled"); ?> title="Misto." onclick="mostraDisciplina();"> Misto
			</div>
		</td>
	</tr>
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

                            window.open('?modulo=principal/popup/alterarPrevisaoTermino&acao=A&scsid=<?php echo $_SESSION['fabrica_var']['scsid']; ?>&alteracao=SS', 'alterarPrevisaoTermino', 'top=350, left=100, align=center,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=850,height=320');

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


	</td>
</tr>
</table>
</form>


<script type="text/javascript">
function submeterAnaliseSolicitacaoServico() {
	
	if(document.getElementById('sidid').value == '') {
		alert('Selecione um sistema');
		return false;
	}

	if(document.getElementById('vgcid').value == '') {
		alert('Selecione um Contrato');
		return false;
	}
	
	if(document.getElementById('mtiid').value == '') {
		alert('Selecione um Item');
		return false;
	}
		
	if(document.getElementById('tpsid').value.length == 0) {
		alert('Selecione um tipo de serviço');
		return false;
	}
	
	var form = document.getElementById('formulario');
	
	if(document.getElementsByName('ansgarantia')[0].checked == true) {
		if(document.getElementById('odsidorigem').value.length == 0) {
			alert('Preencha a O.S. garantia');
			return false;
		}
	}
	
	if(document.getElementById('ansdsc').value.length == 0) {
		alert('Preencha a descrição detalhada');
		return false;
	}
	
	if(document.getElementById('ansprevinicio').value.length == 0) {
		alert('Preencha a previsão de início');
		return false;
	}

	if(document.getElementById('ansprevtermino').value.length == 0) {
		alert('Preencha a previsão de termino');
		return false;
	}
	
	var dataInicio = $("#ansprevinicio").val();
	var dataInicioConvertida = dataInicio.substring(6,10) + dataInicio.substring(3,5) + dataInicio.substring(0,2);
	
	var dataFim = $("#ansprevtermino").val();
	var dataFimConvertida = dataFim.substring(6,10) + dataFim.substring(3,5) + dataFim.substring(0,2);
	
	if (dataInicioConvertida > dataFimConvertida){
		alert('Período informado inválido');
		return false;
	}
	
	<?if($estado_wf['esdid'] == WF_ESTADO_ANALISE) { ?>
		if(document.getElementById('fiscal').value.length == 0) {
			alert( 'Favor selecionar o fiscal!' );
			return false;
		}
	<?}?>
	
      
	divCarregando();
	document.getElementById('formulario').submit();
}

</script>
