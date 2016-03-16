<?php
if($_POST['tipoMunicipio'] && !$_REQUEST["formulario"]){
	header( 'Content-type: text/html; charset=iso-8859-1' );
	tipoMunicipio();
}

$tipoDiagnostico = "M";

# Pegamos o tipo do diagnóstico, se é estadual ou municipal
if($_REQUEST['indid']){
	$tipoDiagnostico = (cte_verifica_tipo_por_indicador($_REQUEST['indid']) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL) ? "E" : "M";
}

# carrega municipio de acordo com o check marcado
function tipoMunicipio(){
	global $db;
	if($_POST['tipoMunicipio'] == 'null'){
		echo "";
		die;
	} else {
		$arTipoMunicipio = explode(",",$_POST['tipoMunicipio']);
		$arTipoMunicipio2 = array();
		foreach ($arTipoMunicipio as $dados){
			$divisao = $cont%2;
			if($divisao == 0){
				$dados_anterior = $dados;
			} else {
				$arTipoMunicipio2[$dados_anterior]= $dados;
				$dados_anterior = "";
			}
			$cont++;
		}
		
		$join = "";
		$arWhere = array();
		
		if($arTipoMunicipio2['grandescidades'] == 'true' || $arTipoMunicipio2['indigena'] == 'true' || $arTipoMunicipio2['quilombola'] == 'true'){
			$join .= " left join territorios.muntipomunicipio mtm on mtm.muncod = m.muncod and mtm.estuf = m.estuf
			";
			if($arTipoMunicipio2['grandescidades'] == 'true'){
				$arWhere[] = 1;
			}
			if($arTipoMunicipio2['indigena'] == 'true'){
				$arWhere[] = 16;
			}
			if($arTipoMunicipio2['quilombola'] == 'true'){
				$arWhere[] = 17;
			}
		}
		
		if($arTipoMunicipio2['grandescidades'] == 'true' || 
		   $arTipoMunicipio2['indigena'] == 'true'       || 
		   $arTipoMunicipio2['quilombola'] == 'true'){
		
			$sql = "select distinct m.muncod as codigo, m.estuf as estado, m.mundescricao as nome from territorios.municipio as m 
					";
			$sql .= $join;
			
			if(count($arWhere)){
				$sql .= "where mtm.tpmid in (".implode(",", $arWhere).") order by nome"; 
			}
			$arDados = $db->carregar($sql);
			
			$itens = array();			
			foreach($arDados as $dados){
				$itens[] = 
				  	  '{ \'codigo\':\'' . $dados['codigo'] . '\','
	                 . ' \'estado\':\'' . $dados['estado'] . '\','
	                 . ' \'nome\':  "' . $dados['nome']   . '"}';
			}
			header('content-type: application/json;charset=iso-8859-1');
	    	echo 'var arDados=[' , implode(',', $itens) , '];';
		} else {
			echo "";
		}
		die;
	}
}

$abas = array(0 => array("id"=>"1","descricao"=>"Dados","link"=>"/cte/cte.php?modulo=principal/configuracao/guia_subacao&acao=A&ppsid=".$_REQUEST['ppsid']),
			  1 => array("id"=>"2","descricao"=>"Motivos","link"=>"/cte/cte.php?modulo=principal/configuracao/guia_subacao_motivo&acao=A&ppsid=".$_REQUEST['ppsid']));
echo montarAbasArray($abas, $_SERVER['REQUEST_URI']);

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

if ( $_REQUEST["formulario"] ) {
	try {
		/////////////////////// Validações /////////////////////
		$peso = $_REQUEST["ppspeso"];	
		$formaExecucao = $_REQUEST["frmid"];
		if($peso == NULL){
			$peso = 1;
		}
		if($peso < 1){
			echo '<script>	alert("Valor mínimo para o peso da subação é igual a 1."); 
			window.location = "cte.php?modulo=principal/configuracao/guia_subacao&acao=A&ppsid='.$_REQUEST["ppsid"].'";</script>';
		}
		if($formaExecucao == NULL){
			echo '<script>	alert("Escolha uma forma de execução"); 
			window.location = "cte.php?modulo=principal/configuracao/guia_subacao&acao=A&ppsid='.$_REQUEST["frmid"].'";</script>';
		}
		
		if( ($_REQUEST["crtid"] && $_REQUEST["ppaid"]) || ($tipoDiagnostico == 'E' && !$_REQUEST["ppsid"]) ){
			$ppaid = $_REQUEST["ppaid"] ? $_REQUEST["ppaid"] : 'null';
			$crtid = $_REQUEST["crtid"] ? $_REQUEST["crtid"] : 'null';
			$indid = $_REQUEST["indid"] ? $_REQUEST["indid"] : 'null';
			$ppsdsc = $_REQUEST["ppsdsc"] ? $_REQUEST["ppsdsc"] : '';
			$ppsobjetivo = $_REQUEST["ppsobjetivo"] ? $_REQUEST["ppsobjetivo"] : '';
			$undid = $_REQUEST["undid"] ? $_REQUEST["undid"] : '';
			$ppsobra = $_REQUEST["ppsobra"] ? $_REQUEST["ppsobra"] : 'null';
			$prgid = $_REQUEST["prgid"] ? $_REQUEST["prgid"] : '';
			$ppstexto = $_REQUEST["ppstexto"] ? $_REQUEST["ppstexto"] : '';
			$ppsmetodologia = $_REQUEST["ppsmetodologia"] ? $_REQUEST["ppsmetodologia"] : '';
			$frmid = $_REQUEST["frmid"] ? $_REQUEST["frmid"] : '';
			$ppspeso = $_REQUEST["ppspeso"] ? $_REQUEST["ppspeso"] : 'null';
			$Monitorado = $_REQUEST["Monitorado"] ? $_REQUEST["Monitorado"] : '';
			$Universal = $_REQUEST["Universal"] ? $_REQUEST["Universal"] : '';
			$indqtdporescola = $_REQUEST["indqtdporescola"] ? $_REQUEST["indqtdporescola"] : '';
			$ppsparecerpadrao = $_REQUEST["ppsparecerpadrao"] ? $_REQUEST["ppsparecerpadrao"] : '';
			$ppsvaluni = number_format( $_REQUEST["ppsparecerpadrao"], 2, ".", "" );
			
			if( $_REQUEST['indid'] ){
				$ordem = "select count(*) + 1 as count from cte.proposicaosubacao where indid = {$_REQUEST['indid']}";
			} else {
				$sql = 'select count(*) as count from cte.proposicaoacao where ppaid = ' . (integer) $_REQUEST['ppaid'];
				$ordem = "select count(*) + 1 as count from cte.proposicaosubacao where ppaid = {$_REQUEST["ppaid"]}"; 
				if ($db->pegaUm( $sql ) != 1)
					throw new Exception('A ação não existe.');
			}
			$ppsordem = $db->pegaUm( $ordem );
			$sqlInsercao = ( "	insert into cte.proposicaosubacao ( ppaid, crtid, indid, ppsdsc, ppsobjetivo, undid, ppsobra, prgid,
																	ppstexto, ppsmetodologia, frmid, ppspeso, ppsmonitoramento, ppsvaluni,
																	ppsindcobuni, indqtdporescola, ppsparecerpadrao, ppsordem )
														   values ( $ppaid, $crtid, $indid, '$ppsdsc', '$ppsobjetivo', $undid, $ppsobra, $prgid,
																	'$ppstexto', '$ppsmetodologia', $frmid, $ppspeso, $Monitorado, $ppsvaluni,
																	$Universal, $indqtdporescola, '$ppsparecerpadrao', $ppsordem )
								 returning ppsid" );

			$ppsid = $db->pegaUm( $sqlInsercao );								 
								 
			if ( !$ppsid ) {
				throw new Exception('Erro ao inserir subação.');
			}
		} else {
			$atribuicao = array();
			if ( !empty( $_REQUEST["undid"] ) ) {
				array_push( $atribuicao, sprintf( "undid = %d", $_REQUEST["undid"] ) );
			} else {
				array_push( $atribuicao, sprintf( "undid = null" ) );
			}
			if ( !empty( $_REQUEST["prgid"] ) ) {
				array_push( $atribuicao, sprintf( "prgid = %d", $_REQUEST["prgid"] ) );
			} else {
				array_push( $atribuicao, sprintf( "prgid = null" ) );
			}
			if ( !empty( $_REQUEST["frmid"] ) ) {
				array_push( $atribuicao, sprintf( "frmid = %d", $_REQUEST["frmid"] ) );
			} else {
				array_push( $atribuicao, sprintf( "frmid = null" ) );
			}
			if ( !empty( $_REQUEST["ppsobra"] ) ) {
				array_push( $atribuicao, sprintf( "ppsobra = %d", $_REQUEST["ppsobra"] ) );
			} else {
				array_push( $atribuicao, sprintf( "ppsobra = null" ) );
			}
			if ( !empty( $_REQUEST["ppspeso"] ) ) {
				array_push( $atribuicao, sprintf( "ppspeso = %d", $_REQUEST["ppspeso"] ) );
			} else {
				array_push( $atribuicao, sprintf( "ppspeso = 1" ) );
			}
			array_push( $atribuicao, sprintf( "ppsdsc = '%s'", $_REQUEST["ppsdsc"] ) );
			array_push( $atribuicao, sprintf( "ppstexto = '%s'", $_REQUEST["ppstexto"] ) );
			array_push( $atribuicao, sprintf( "ppsobjetivo = '%s'", $_REQUEST["ppsobjetivo"] ) );
			array_push( $atribuicao, sprintf( "ppsmetodologia = '%s'", $_REQUEST["ppsmetodologia"] ) );
			array_push( $atribuicao, sprintf( "ppsmonitoramento = '%s'", $_REQUEST["Monitorado"] ) );
			array_push( $atribuicao, sprintf( "ppsindcobuni = '%s'", $_REQUEST["Universal"] ) );
			array_push( $atribuicao, sprintf( "ppsparecerpadrao = '%s'", $_REQUEST["ppsparecerpadrao"] ) );
			
			if($_REQUEST["ppsvaluni"]){
				$valorUnitario = str_replace(".","",$_REQUEST["ppsvaluni"]);
				$valorUnitario = str_replace(",",".",$valorUnitario);
				array_push( $atribuicao, sprintf( "ppsvaluni = '%s'", $valorUnitario ) );
			}
	
			$sql = sprintf(
				"update cte.proposicaosubacao set %s where ppsid = %d",
				implode( ",", $atribuicao ),
				$_REQUEST["ppsid"]
			);
	
			if ( !$db->executar( $sql ) ) {
				throw new Exception( "Erro ao atualizar os dados do guia." );
			}
			$atribuicao = array();
			if ( !empty( $_REQUEST["undid"] ) ) {
				array_push( $atribuicao, sprintf( "undid = %d", $_REQUEST["undid"] ) );
			} else {
				array_push( $atribuicao, sprintf( "undid = null" ) );
			}
			if ( !empty( $_REQUEST["prgid"] ) ) {
				array_push( $atribuicao, sprintf( "prgid = %d", $_REQUEST["prgid"] ) );
			} else {
				array_push( $atribuicao, sprintf( "prgid = null" ) );
			}
			if ( !empty( $_REQUEST["frmid"] ) ) {
				array_push( $atribuicao, sprintf( "frmid = %d", $_REQUEST["frmid"] ) );
			} else {
				array_push( $atribuicao, sprintf( "frmid = null" ) );
			}
			array_push( $atribuicao, sprintf( "sbadsc = '%s'", $_REQUEST['ppsdsc'] ) );
			array_push( $atribuicao, sprintf( "sbatexto = '%s'", $_REQUEST['ppstexto'] ) );
			array_push( $atribuicao, sprintf( "sbaobjetivo = '%s'", $_REQUEST['ppsobjetivo'] ) );
			array_push( $atribuicao, sprintf( "sbastgmpl = '%s'", $_REQUEST['ppsmetodologia'] ) );
			//array_push( $atribuicao, sprintf( "indqtdporescola = %s", $_REQUEST['indqtdporescola'] ) );
	
	        $sql = sprintf('update cte.proposicaosubacao set indqtdporescola = %s where ppsid = %d',
	                       $_REQUEST['indqtdporescola'],
	                       $_REQUEST['ppsid']);
	
			$db->executar($sql);
	
			$sql = sprintf("update cte.subacaoindicador set %s where ppsid = %d",
	                       implode( ",", $atribuicao ),
	                       $_REQUEST["ppsid"]);
	
			$db->executar($sql);
		}

		# Insere municipio somente se for diagnóstico Municipal
		if($tipoDiagnostico == "M"){
			$ppsid = $_REQUEST["ppsid"] ? $_REQUEST["ppsid"] : $ppsid;
			///////////// insere a tabela subacaomunicipio via combo municipios ///////////////////		
			$sql ="delete from cte.subacaomunicipio where ppsid = ".$ppsid."";
	
			$db->executar($sql);
			if ( !$_REQUEST['municipios'][0] == NULL ){
				/// Lista combo Municipios///////
				$arTipoMunicipio = $_REQUEST['municipios'];
				foreach($arTipoMunicipio as $todosMunicipios){
					if(substr($todosMunicipios,0,1) == '_'){
					 	$todosMunicipios = substr_replace($todosMunicipios,'',0,1);
					}
					$sqlMunicipio ="insert into cte.subacaomunicipio (ppsid, muncod) VALUES (".$ppsid.",".$todosMunicipios.")";
					$db->executar($sqlMunicipio);	
				}
			}
		}
		
		////////////////////Insere na subacaomunicipio via combo Calssificação IDEB//////////////////////////////////
		if( !$_REQUEST['ideb'][0] == NULL ){
			$ClassificacaoIDEB = $_REQUEST['ideb'];
			//$arraydeMunIDEB = array();
			foreach($ClassificacaoIDEB as $ClassificacaoIDEB){
				$SQLRecuperaMunicipioIDEB = "
					select tm.tpmid, m.muncod  
					from territorios.tipomunicipio tm, 
     				territorios.municipio m, 
     				territorios.muntipomunicipio mtm
					where tm.tpmid = ".$ClassificacaoIDEB." 
					and tm.tpmid = mtm.tpmid 
					and m.muncod = mtm.muncod";			
			
				$RecuperaIDEB = $db->carregar( $SQLRecuperaMunicipioIDEB); 
				foreach($RecuperaIDEB as $RecuperaIDEB ){
					$sqlMunicipioIDEB ="insert into cte.subacaomunicipio (ppsid, muncod, tpmid) VALUES (".$ppsid.",".$RecuperaIDEB['muncod'].",".$RecuperaIDEB['tpmid'].")";
					
					$db->executar($sqlMunicipioIDEB);			
				}
			}
		}		

		$db->commit();
		?>
		<script type="text/javascript">
			window.opener.location.reload();
			window.close();
		</script>
		<?
		exit();
		
	} catch ( Exception $erro ) {
		$db->rollback();
?>
		<html>
			<head>
				<script type="text/javascript">
				alert( '<?= $erro->getMessage(); ?>' );
				location.href = "?modulo=<?= $_REQUEST["modulo"] ?>&acao=<?= $_REQUEST["acao"] ?>";
				</script>
			</head>
			<body>
			<body/>
		</html>
<?
		exit();
	}
}

$_REQUEST["ppsid"] = $_REQUEST["ppsid"] ? $_REQUEST["ppsid"] : 0;

$sql = '
select
    *
    --ps.ppsid,
    --ps.ppaid,
    --ps.ppsdsc,
    --ps.ppsordem,
    --d.itrid,
    --i.indid,
    --ps.indid
    ,ps.indid as indicadorsubacao
    ,ps.indqtdporescola as qtdporescola
from
    cte.proposicaosubacao ps
left join
    cte.proposicaoacao pa on pa.ppaid = ps.ppaid
left join
    cte.criterio c on c.crtid = pa.crtid
inner join
    cte.indicador i on (i.indid = ps.indid or i.indid = c.indid) and i.indstatus = \'A\'
inner join
    cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = \'A\'
inner join
    cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = \'A\'
where
	ps.ppsid = '. $_REQUEST['ppsid'];

$subacao = $db->pegaLinha($sql);

if($subacao['indicadorsubacao']){
	$tipoDiagnostico = "E"; 
}

$sql = sprintf(
	"select ps.ppsid
	from cte.proposicaosubacao ps
	inner join cte.proposicaoacao pa on pa.ppaid = ps.ppaid
	inner join cte.criterio c on c.crtid = pa.crtid
	inner join cte.indicador i on i.indid = c.indid and i.indstatus = 'A'
	inner join cte.areadimensao ad on ad.ardid = i.ardid and ad.ardstatus = 'A'
	inner join cte.dimensao d on d.dimid = ad.dimid and d.dimstatus = 'A'
	inner join cte.instrumento it on it.itrid = d.itrid
	order by it.itrdsc, d.dimcod, ad.ardcod, i.indcod, c.ctrord, ps.ppsordem"
);

$lista    = $db->carregarColuna( $sql, "ppsid" );
$anterior = @$lista[ array_search( $_REQUEST["ppsid"], $lista ) - 1 ];
$proximo  = @$lista[ array_search( $_REQUEST["ppsid"], $lista ) + 1 ];

if($subacao["frmid"] != 8){ // Se for Assistência técnica do MEC mostra campo Valor Unitario
		$mostraounao = 'style=" display:none;"';
}

if($subacao["ppsmonitoramento"] == "f" || !$_REQUEST["ppsid"] ){
	$checadoMonitoraN = 'checked="checked"'; 
}else{
	$checadoMonitoraS = 'checked="checked"'; 
}

if($subacao["ppsindcobuni"] == "f" || !$_REQUEST["ppsid"] ){
	$checadoUniversalN = 'checked="checked"'; 
}else{
	$checadoUniversalS = 'checked="checked"'; 
}

if ($subacao['qtdporescola'] == 't') {
    $indqtdporescola = 'checked="checked"';
    $indqtdglobal    = '';
} else {
    $indqtdporescola = '';
    $indqtdglobal    = 'checked="checked"';
}

?>
<html style="overflow: scroll;">
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		<!--  script language="JavaScript" src="../../includes/funcoes.js"></script-->
		<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/prototype.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body onLoad="habilitaValorUnitario(<?=$subacao["frmid"]?>);">
    <center>
		<div id="aguarde" style="display: none;background-color:#ffffff;position:absolute;color:#000033;top:50%;left:30%;border:2px solid #cccccc; width:300;font-size:12px;z-index:0;">
			<br><img src="../imagens/wait.gif" border="0" align="middle"> Aguarde! Carregando Dados...<br><br>
		</div>
	</center>
<form method="post" name="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="indid" value="<?php echo $subacao['indid']; ?>" />
	<input type="hidden" name="ppsid" value="<?= $_REQUEST["ppsid"] ?>"/>
	<!-- div id='divTipoMunicipio' style="display: block;"></div -->
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<colgroup>
			<col style="width: 125px;"/>
			<col/>
		</colgroup>
		<tbody>
<!--
        <tr>
				<td align='right' class="SubTituloDireita">Instrumento:</td>
				<td><?= $lista['itrdsc'] ?></td>
			</tr>
-->
			<?php if( $_REQUEST["ppsid"] ){ ?>
				<tr>
					<td align='right' class="SubTituloDireita">Dimensão:</td>
					<td><b><?= $subacao['dimcod'] ?></b> <?= $subacao['dimdsc'] ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Área:</td>
					<td><b><?= $subacao['dimcod'] . '.' . $subacao['ardcod'] ?></b> <?= $subacao['arddsc'] ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita">Indicador:</td>
					<td><b><?= $subacao['dimcod'] . '.' . $subacao['ardcod'] . '.' . $subacao['indcod'] ?></b> <?= $subacao['inddsc'] ?></td>
				</tr>
				<?php if($tipoDiagnostico == "M"){ ?>
					<tr>
						<td align='right' class="SubTituloDireita">Critério:</td>
						<td><b><?= abs( $subacao['ctrord'] - 5 ) ?></b> <?= $subacao['crtdsc'] ?></td>
					</tr>
					<tr>
						<td align='right' class="SubTituloDireita">Ação:</td>
						<td><b><?= $subacao['ppadsc'] ?></b></td>
					</tr>
				<?php } ?>
			<?php }
			else{ ?>
				<tr>
					<td>
						<input type="hidden" name="ppaid" value="<?php echo $_REQUEST["ppaid"] ?>" />
						<input type="hidden" name="crtid" value="<?php echo $_REQUEST["crtid"] ?>" />
						<input type="hidden" name="indid" value="<?php echo $_REQUEST["indid"] ?>" />
					</td>
				</tr>
			<?php } ?>
			<tr>
				<td align='right' class="SubTituloDireita">Subação:</td>
				<td>
					<?php
					$ppsdsc = $_REQUEST["ppsdsc"] ? $_REQUEST["ppsdsc"] : $subacao["ppsdsc"];
					echo campo_texto( 'ppsdsc', 'S', 'S', '', 70, 2000, '', '' );
					?>
				</td>
			</tr>
			<tr>													
				<td align='right' class="SubTituloDireita">Objetivo:</td>
				<td>
					<?
					$ppsobjetivo = $subacao['ppsobjetivo'];
					echo campo_texto( 'ppsobjetivo', 'N', 'S', '', 70, 150, '', '' );
					?>
				</td>
			</tr>
			<tr>				
				<td align='right' class="SubTituloDireita">Unidade Medida:</td>
				<td>
			<?php
				$undid = $subacao["undid"];
				if($tipoDiagnostico == "M"){
					$db->monta_combo(
						"undid",
						"select undid as codigo , unddsc as descricao from cte.unidademedida where undtipo = 'M' order by unddsc",
						"S", "- Selecione -", "", "", "", 350, "S"
					);
				} else {
					$db->monta_combo(
						"undid",
						"select undid as codigo , unddsc as descricao from cte.unidademedida where undtipo = 'E' order by unddsc",
						"S", "- Selecione -", "", "", "", 350, "S"
					);
				}
			?>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Tipo de Obra:</td>
				<td>
					<select name="ppsobra" class="CampoEstilo">
						<option value="">&nbsp;</option>
						<option value="<?= CTE_OBRA_AMPLIACAO ?>" <?= $subacao["ppsobra"] == CTE_OBRA_AMPLIACAO ? 'selected="selected"' : '' ?>>Ampliação</option>
						<option value="<?= CTE_OBRA_CONSTRUCAO ?>" <?= $subacao["ppsobra"] == CTE_OBRA_CONSTRUCAO ? 'selected="selected"' : '' ?>>Construção</option>
						<option value="<?= CTE_OBRA_REFORMA ?>" <?= $subacao["ppsobra"] == CTE_OBRA_REFORMA ? 'selected="selected"' : '' ?>>Reforma</option>
					</select>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Programa:</td>
				<td>
					<?php
					$prgid = $subacao["prgid"];
					$sql = sprintf( "select prgid as codigo, prgdsc as descricao from cte.programa order by prgdsc" );
					$db->monta_combo( "prgid", $sql, "S", "- Selecione -", "alterarPrograma", "" , "" , 350, "S" );
					?>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Texto para Geração do Termo de Cooperação:</td>
				<td>
					<textarea id="ppstexto" name="ppstexto" class="CampoEstilo" style="width: 100%; height: 150px;"><?=$subacao['ppstexto']; ?></textarea>
					<br/>
					<input type="button" class="botao" name="" value="Objetivo" onclick="incluirMarcadorObjetivo();"/>
					<input type="button" class="botao" name="" value="Quantidade" onclick="incluirMarcadorQuantidade();"/>
					<input type="button" class="botao" name="" value="Unidade Medida" onclick="incluirMarcadorBeneficiario();"/>
					<input type="button" class="botao" name="" value="Programa" onclick="incluirMarcadorPrograma();"/>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Estratégia de Implementação:</td>
				<td>
				<?php
				$ppsmetodologia = $subacao["ppsmetodologia"];
				echo campo_textarea( "ppsmetodologia", "N", "S", '', 70, 3, 0 );
				?>
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Forma de Execução:</td>
				<td>				
				<?php
				$frmid = $subacao['frmid'];
				$sql = '
                SELECT
                    frmid as codigo,
                    frmdsc as descricao
                FROM
                    cte.formaexecucao
                WHERE
                    frmtipo = \'' . $tipoDiagnostico . '\'
                    AND
                    frmbrasilpro = FALSE
                ORDER BY
                    frmdsc';

				$db->monta_combo('frmid', $sql, 'S', '- Selecione -', 'habilitaValorUnitario', '', '', '', 'N');
				?>
				</td>
			</tr>
			
			<tr id="ValorUnitario">
				<td align='right' class="SubTituloDireita">Valor Unitário (R$):</td>
				<td>
				<?
				$ppsvaluni = $subacao["ppsvaluni"];
				$ppsvaluni = number_format( (float) $ppsvaluni, 2, ",", "." );	
				echo campo_texto( 'ppsvaluni', 'N', 'S', '', 70, 150, '###.###.###,##', '' );
				?>
				</td>
			</tr>
<?php
	# Se diagnóstico for municipal, então mostra combo de Município
	if($tipoDiagnostico == "M"){
?>
		<tr>
        	<td class="SubTituloDireita" valign="top">Município</td>
        	<td>
        	<!-- label><input type="checkbox" onclick="buscaPorTipoMunicipio('capitais');" name="capitais" id="capitais" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['capitais'] == 1 ? 'checked="checked"' : '' ?>/> Capitais</label -->
			<label><input type="checkbox" onclick="buscaPorTipoMunicipio('grandescidades');" name="grandescidades" id="grandescidades" value="1" class="normal" style="margin-top: -1px;" <?= $_REQUEST['grandescidades'] == 1 ? 'checked="checked"' : '' ?>/> Grandes Cidades e Capitais</label>
			<label><input type="checkbox" onclick="buscaPorTipoMunicipio('indigena');" name="indigena" id="indigena"value="16" class="normal" style="margin-top: -1px;" <?= $_REQUEST['indigena'] == 1 ? 'checked="checked"' : '' ?>/> Comunidade Indígena</label>
			<label><input type="checkbox" onclick="buscaPorTipoMunicipio('quilombola');" name="quilombola" id="quilombola" value="17" class="normal" style="margin-top: -1px;" <?= $_REQUEST['quilombola'] == 1 ? 'checked="checked"' : '' ?>/> Comunidade Quilombola</label>
			<br /><br />
        <?php
			$sqlComboMunicipio = "select m.estuf, m.mundescricao, sm.muncod  
					        	from cte.proposicaosubacao ps, 
					        	     cte.subacaomunicipio sm, 
					        	     territorios.municipio m 
					          where m.muncod = sm.muncod 
					        	and sm.ppsid = ps.ppsid 
					        	and sm.tpmid IS NULL 
					        	and ps.ppsid = ".$_REQUEST["ppsid"]; 

			$listaMunicipios = $db->carregar( $sqlComboMunicipio); 
        ?>
        	<select multiple="multiple" size="5" name="municipios[]" id="municipios" ondblclick="abrepopupMunicipio();" class="CampoEstilo" style="width:400px;" >
        <?
        	if($listaMunicipios != ""){
				foreach($listaMunicipios as $listaMunicipios){
         			$CodigoMunicipio = $listaMunicipios["muncod"];
         			$EstadodoMunicipio = $listaMunicipios["estuf"];
         			$Omunicipio = $listaMunicipios["mundescricao"];
        ?>
        		<option value="<?=$CodigoMunicipio ?>"><?=$Omunicipio." - ".$EstadodoMunicipio?></option>
        <? 
				} 
			} else if($listaMunicipios == ""){
		?>
				<option value="">Duplo clique para selecionar da lista</option>
		<?
			} 
		?>
        	</select>
        	<br />
        	<input type="button" name="excluirTodos" id="excluirTodos" value="Limpar Todos" onclick="limpaComboMunicipio();" />
        </td>
    </tr>
    <tr id="ClassificacaoIDEB">
		<td align='right' class="SubTituloDireita">Classificação IDEB:</td>
		<td>
				<?php
				$sqlsubacaomunicipioexistente ="select distinct tm.tpmdsc 
												from cte.subacaomunicipio sam, 
													 territorios.tipomunicipio tm 
												where sam.tpmid is not null and tm.tpmid = sam.tpmid
												and ppsid = ".$_REQUEST["ppsid"];
				$RecuperaIDEBsubacaomunic = $db->carregar( $sqlsubacaomunicipioexistente); 
				
				$sqlComboIDEB = "
					select distinct
						tmu.tpmid as codigo,
						tmu.tpmdsc as descricao
					from territorios.tipomunicipio tmu
					left join
						cte.subacaomunicipio sam on sam.tpmid = tmu.tpmid
												and sam.ppsid = " . $_REQUEST["ppsid"] . "
					where
						tmu.gtmid = ( select gtmid from territorios.grupotipomunicipio where gtmdsc = 'Classificação IDEB' ) and
						tmu.tpmstatus = 'A'
				";

				$sql = "
					select distinct
						tmu.tpmid as codigo,
						tmu.tpmdsc as descricao
					from territorios.tipomunicipio tmu
					inner join
						cte.subacaomunicipio sam on sam.tpmid = tmu.tpmid
												and sam.ppsid = " . $_REQUEST["ppsid"] . "
					where
						tmu.gtmid = ( select gtmid from territorios.grupotipomunicipio where gtmdsc = 'Classificação IDEB' ) and
						tmu.tpmstatus = 'A'";

				$nome  = 'ideb';
				$$nome = $db->carregar($sql);
				combo_popup( "ideb", $sqlComboIDEB, "Classificação IDEB", "215x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
		</td>
	</tr>
<?php
} # Fim do ( if($tipoDiagnostico == "M") )
?>
	<tr>
		<td align='right' class="SubTituloDireita">Peso da subação:</td>
		<td>
		<?
					$ppspeso = $subacao['ppspeso'];
					echo campo_texto( 'ppspeso', 'N', 'S', '', 70, 150, '', '' );
					?>
		</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita">Passível de monitoramento:</td>
	<td>
		<input type="radio" name="Monitorado" id="Monitorado" <?=$checadoMonitoraS?> value="true"/>
        <label for="fisico">Sim</label>
        &nbsp;&nbsp;&nbsp;
        <input type="radio" name="Monitorado" <?=$checadoMonitoraN?> id="Monitorado" value="false"/>
        <label for="fisico">Não</label>
        &nbsp;&nbsp;&nbsp;
	
	</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita">Cobertura Universal MEC:</td>
	<td>
		<input type="radio" name="Universal" id="Universal" <?=$checadoUniversalS?> value="true"/>
        <label for="fisico">Sim</label>
        &nbsp;&nbsp;&nbsp;
        <input type="radio" name="Universal" id="Universal" <?=$checadoUniversalN?> value="false"/>
        <label for="fisico">Não</label>
        &nbsp;&nbsp;&nbsp;
	
	</td>
	</tr>

	<tr>
	<td align='right' class="SubTituloDireita">Cronograma:</td>
	<td>
        <input type="radio" name="indqtdporescola" id="indqtdglobal"    <?php echo $indqtdglobal ?> value="false" />
        <label for="indqtdglobal">Global</label>

        &nbsp;&nbsp;&nbsp;

		<input type="radio" name="indqtdporescola" id="indqtdporescola" <?php echo $indqtdporescola ?> value="true"  />
        <label for="indqtdporescola">Por escola</label>
	</td>
	</tr>
	<tr>
	<td align='right' class="SubTituloDireita">Parecer padrão:</td>
	<td>
       <textarea id="ppsparecerpadrao" name="ppsparecerpadrao" class="CampoEstilo" style="width: 100%; height: 150px;"><?=$subacao['ppsparecerpadrao']; ?></textarea>
	</td>
	</tr>
			<!--
			<tr>
				<td align='right' class="SubTituloDireita">Status:</td>
				<td>
					<?php
					//$ssuid = $subacao["ssuid"];
					//$sql = sprintf( "select ssuid as codigo, ssudescricao as descricao from cte.statussubacao" );
					//$db->monta_combo( "ssuid", $sql, 'S', '- Selecione -', '', '' );
					?>
				</td>
			</tr>
			-->
			<tr bgcolor="#C0C0C0">
				<td>&nbsp;</td>
				<td>
					<input type="button" class="botao" name="btalterar" value="Salvar" onclick="enviarFormulario( this );"/>
					<?php if( $anterior ): ?>
					<input type="button" class="botao" name="" value="Anterior" onclick="anterior();"/>
					<?php endif; ?>
					<?php if( $proximo ): ?>
					<input type="button" class="botao" name="" value="Próximo" onclick="proximo();"/>
					<?php endif; ?>
					<input type="button" class="botao" name="btvoltar" value="Voltar" onclick="voltar();" style="margin: 0 0 0 20px;"/>
				</td>
			</tr>
		</tbody>
	</table>
</form>

<script type="text/javascript">
	function insertAtCursor(myField, myValue) {
		// IE support
		if (document.selection) {
			myField.focus();
			sel = document.selection.createRange();
			sel.text = myValue;
		}
		// MOZILLA/NETSCAPE support
		else if (myField.selectionStart || myField.selectionStart == '0' ) {
			var startPos = myField.selectionStart;
			var endPos = myField.selectionEnd;
			myField.value = myField.value.substring(0, startPos) + myValue + myField.value.substring(endPos, myField.value.length);
		} else {
			myField.value += myValue;
		}
	}
	
</script>

<script type="text/javascript">
	
function habilitaValorUnitario(selecionado){
	var tr  = document.getElementById( 'ValorUnitario' );
	if(selecionado == 8){
		if (document.selection){
		tr.style.display = 'block';
		}else{
		tr.style.display = 'table-row';
		}
	}else{
		tr.style.display = 'none';
	}
	
}


function abrepopupMunicipio(){
	window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/combo_municipios_bandalarga.php','Municipios','width=400,height=400,scrollbars=1');
}
function enviarFormulario( botao )
{

	selectAllOptions( document.getElementById( 'ideb' ) );
	selectAllOptions( document.getElementById( 'municipios' ));
	selectAllOptions( document.getElementById( 'frmid' ));
	selectAllOptions( document.getElementById( 'tipoMunicipio' ));
	var validacao = true;
	var mensagem = '';
	if(trim( document.formulario.frmid.value ) == '' ){
		mensagem += '\n Forma de execução';
		validacao = false;
	}
	if( trim( document.formulario.ppsdsc.value ) == '' ){
		mensagem += '\n Subação';
		validacao = false;
	}
	/*
	if( trim( document.formulario.ppsobjetivo.value ) == '' ){
		mensagem += '\n Objetivo';
		validacao = false;
	}
	*/
	if( trim( document.formulario.undid.value ) == '' ){
		mensagem += '\n Unidade de Medida';
		validacao = false;
	}
	if( trim( document.formulario.prgid.value ) == '' ){
		mensagem += '\n Programa';
		validacao = false;
	}
	if( validacao == false ){
		alert( 'Os seguintes campos precisam ser informados.' + mensagem );
		return false;
	}

	botao.disabled = true;
	document.formulario.submit();
}

/* NAVEGAÇÃO */

function voltar(){
	window.close();
}

function anterior(){
	window.location = '?modulo=<?= $_REQUEST["modulo"] ?>&acao=<?= $_REQUEST["acao"] ?>&ppsid=<?= $anterior ?>';
}

function proximo(){
	window.location = '?modulo=<?= $_REQUEST["modulo"] ?>&acao=<?= $_REQUEST["acao"] ?>&ppsid=<?= $proximo ?>';
}

/* EVENTOS DO FORMULÁRIO */

function alterarObjetivo(){}

function alterarBeneficiario( undid ){}

function alterarPrograma( prgid ){}

function incluirMarcadorObjetivo(){
	var marcador = '#OBJETIVO#';
	insertAtCursor( document.getElementById( 'ppstexto' ), marcador );
}

function incluirMarcadorBeneficiario(){
	var marcador = '#UNIDADE_MEDIDA#';
	insertAtCursor( document.getElementById( 'ppstexto' ), marcador );
}

function incluirMarcadorQuantidade(){
	var marcador = '#QUANTIDADE#';
	insertAtCursor( document.getElementById( 'ppstexto' ), marcador );
}

function incluirMarcadorPrograma(){
	var marcador = '#PROGRAMA#';
	insertAtCursor( document.getElementById( 'ppstexto' ), marcador );
}	

function buscaPorTipoMunicipio(tipoMunicipio){
	var grandescidades = document.getElementById('grandescidades').checked;
	var indigena = document.getElementById('indigena').checked;
	var quilombola = document.getElementById('quilombola').checked;
	
	var arGrandescidades = new Array(2);
	arGrandescidades[0] = 'grandescidades'; 
	arGrandescidades[1] = grandescidades;

	var arIndigena = new Array(2);
	arIndigena[0] = 'indigena'; 
	arIndigena[1] = indigena;

	var arQuilombola = new Array(2);
	arQuilombola[0] = 'quilombola'; 
	arQuilombola[1] = quilombola;

	var arTipoMunicipio = new Array(3);
	arTipoMunicipio[0] = arGrandescidades;
	arTipoMunicipio[1] = arIndigena;
	arTipoMunicipio[2] = arQuilombola;

	if(arTipoMunicipio == ""){
		arTipoMunicipio = null;
	}
	
	var textocombogeral = "Duplo clique para selecionar da lista";
	var comboMunicipio = document.getElementById('municipios');
	var boNaoApaga = 0;

	for (var i = comboMunicipio.length-1; i >= 0; i--) {
		var indice = comboMunicipio[i].index;
		var valor  = comboMunicipio[i].value;
		// Se tem '_' limpa 
		if(valor.indexOf('_') == 0){
			comboMunicipio.remove(i);
		}
	}
	
	new Ajax.Request(window.location.href, {
        method: 'post',
        asynchronous: false,
        parameters: 'tipoMunicipio='+ arTipoMunicipio,
        onLoading: $('aguarde').show(),
        onComplete: function(res)
        {
			// Se teve resposta do ajax
			if(res.responseText != ""){
				eval(res.responseText);
				// se o array retornado do banco
				if(arDados.length > 1){
		        	for (var j = 0; j < arDados.length; j++) {
			        	var valor = arDados[j].codigo;
		        		var nome = arDados[j].nome;
		        		var estado = arDados[j].estado;
						// Verifica se exite esse valor na combo e retorna o sua posição na combo
				        //var indiceRetorno = arComboMunicipio.in_array(valor); # não funciona
				        var indiceRetorno = retornaIndice(valor);
				        
			        	// Não estiver no array insere como novo.
			        	if( indiceRetorno == '-1'){
							var d = comboMunicipio.length++;
							comboMunicipio.options[d].value = "_"+valor;
							comboMunicipio.options[d].text = nome +' - '+ estado;
			        	} else { // Atualiza o valor existente colocando '_'
		        			comboMunicipio.options[indiceRetorno].value = "_"+valor;
		        			comboMunicipio.options[indiceRetorno].text = nome +' - '+ estado;
				        }
					}
				} 
	        } else { // else do res.responseText != "";
	        	if(comboMunicipio.length == 0 ){
					var oOption = document.createElement("option");
					oOption.text = textocombogeral;
					oOption.value = "";
					document.getElementById("municipios").appendChild(oOption);
					boNaoApaga = 1;
				}
	        }
		} // fim do onComplete
	});
	if(!boNaoApaga)
		deletaIndice(textocombogeral);
		
	$('aguarde').hide();
}

function deletaIndice(textocombogeral){
	var comboMunicipio = document.getElementById('municipios');
	for (var j = 0; j < comboMunicipio.length; j++) {
		if(comboMunicipio.options[j].text == textocombogeral ){
			comboMunicipio.remove(j);
		}					
	}
}

function retornaIndice(valor){
	var comboMunicipio = document.getElementById('municipios');
	for (var i = 0; i < comboMunicipio.length; i++) {
		var indiceCombo = comboMunicipio.options[i].index;
		var textoCombo = comboMunicipio.options[i].text;
		var valorCombo = comboMunicipio.options[i].value;
		
		if(valorCombo.indexOf('_') == 0){
			var tam = valorCombo.length;
			var valorCombo = valorCombo.substring(1,tam);
		}
		
		if(valorCombo == valor){
			return indiceCombo;
		}
	}
	return '-1';
}

function limpaComboMunicipio(){
	var textocombogeral = "Duplo clique para selecionar da lista";
	var comboMunicipio = document.getElementById('municipios');
	for (var i = comboMunicipio.length-1; i >= 0; i--) {
		comboMunicipio.remove(i);
	}
	if(comboMunicipio.length == 0 ){
		var oOption = document.createElement("option");
		oOption.text = textocombogeral;
		oOption.value = "";
		comboMunicipio.appendChild(oOption);
	}
	
	var grandescidades = document.getElementById('grandescidades').checked;
	var indigena = document.getElementById('indigena').checked;
	var quilombola = document.getElementById('quilombola').checked;
	
	if(grandescidades == true){ document.getElementById('grandescidades').checked = false; }
	
	if(indigena == true){ document.getElementById('indigena').checked = false; }
	
	if(quilombola == true){ document.getElementById('quilombola').checked = false; }
		
}
</script>