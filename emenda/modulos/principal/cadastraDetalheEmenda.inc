<?php
header('content-type: text/html; charset=ISO-8859-1');

if( $_REQUEST['requisicao'] == 'vincularAcoesPar' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	if( $_POST['chk_disabled'] && is_array($_POST['chk_disabled']) ){
		$arPpsid_D = array();
		foreach ($_POST['chk_disabled'] as $iniid => $arPpsid) {				
			foreach ($arPpsid as $ppsid) {
				array_push($arPpsid_D, $ppsid);
			}
		}
		$filtro = implode(', ', $arPpsid_D);
		$filtro = " and ppsid not in ($filtro)";
	}
	$db->executar("DELETE FROM emenda.emendapariniciativa WHERE edeid = {$_POST['edeid']} $filtro");
	if( $_POST['chk'] && is_array($_POST['chk']) ){
		foreach ($_POST['chk'] as $iniid => $arPpsid) {				
			foreach ($arPpsid as $ppsid) {
				$sql = "INSERT INTO emenda.emendapariniciativa(ppsid, iniid, edeid) 
						VALUES($ppsid, $iniid, {$_POST['edeid']})";
				$db->executar($sql);
			}
		}
	}
	echo $db->commit();
	exit();
}

if( $_REQUEST['requisicao'] == 'reduziValorRecurso' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$_POST['edevalorreducao'] = str_replace(".", "", $_POST['edevalorreducao']);
	$_POST['edevalorreducao'] = str_replace(",", ".", $_POST['edevalorreducao']);
	
	$edevalorreducao = $_POST['edevalorreducao'];
	$edeid = $_POST['edeid'];
	$sql = "update emenda.emendadetalheentidade set edevalorreducao = $edevalorreducao where edeid = $edeid";
	
	$db->executar($sql);
	echo $db->commit();
	exit();
}

if( $_REQUEST['requisicao'] == 'mostraAcoesPar' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	if($_POST['mapcod'] == '40'){
		$itrid = '2';
		$filtro = " and dim.itrid in ($itrid) ";
	} elseif($_POST['mapcod'] == '30'){
		$itrid = '1,3';
		$filtro = " and dim.itrid in ($itrid) ";
	}
	 
	$sql = "SELECT distinct
				case when (select count(epiid) from emenda.emendapariniciativa where edeid = ide.edeid and ppsid = pse.ppsid) > 0 then
					(case when sub.subacao > 0 then
                    	'<center><input type=\"checkbox\" checked disabled name=\"chk['||ide.iniid||'][]\" id=\"chk\" value=\"'||ps.ppsid||'\">
                    		<input type=\"hidden\" name=\"chk_disabled['||ide.iniid||'][]\" id=\"chk_disabled\" value=\"'||ps.ppsid||'\"/></center>'
                    else
						'<center><input type=\"checkbox\" checked  name=\"chk['||ide.iniid||'][]\" id=\"chk\" value=\"'||ps.ppsid||'\"></center>'
					end)
				else
					'<center><input type=\"checkbox\" name=\"chk['||ide.iniid||'][]\" id=\"chk\" value=\"'||ps.ppsid||'\"></center>'
				end as codigo,
				ps.ppsdsc as descricao
			FROM par.propostasubacao ps
			    inner join par.propostasubacaoiniciativaemenda pse on pse.ppsid = ps.ppsid
			    inner join emenda.iniciativadetalheentidade ide on ide.iniid = pse.iniid and ide.idestatus = 'A'
                inner join par.indicador i on i.indid = ps.indid and i.indstatus = 'A'
                inner join par.area a on a.areid = i.areid and a.arestatus = 'A'
                inner join par.dimensao dim on dim.dimid = a.dimid
                INNER JOIN par.criterio c on c.indid = i.indid 
                INNER JOIN par.propostaacao pa ON pa.crtid = c.crtid
                left join (SELECT distinct
                                count(s.sbaid) as subacao,
                                vede.edeid,
                                s.ppsid
                            FROM
                                par.subacao s
                                inner join par.subacaodetalhe sd on sd.sbaid = s.sbaid
                                inner join par.subacaoemendapta sp on sp.sbdid = sd.sbdid and sp.sepstatus = 'A'
                                inner join emenda.v_emendadetalheentidade vede on vede.emdid = sp.emdid
                                inner join emenda.entidadebeneficiada enb on enb.enbid = vede.entid
                                inner join emenda.ptemendadetalheentidade pte on vede.edeid = pte.edeid
                                inner join emenda.planotrabalho ptr on ptr.ptrid = pte.ptrid
                                inner join par.subacaoemendapta sep on sep.ptrid = ptr.ptrid and vede.emdid = sep.emdid and sep.sepstatus = 'A'
                            WHERE
                                vede.edestatus = 'A'
                                and s.sbastatus = 'A'
                                and vede.ededisponivelpta = 'S'
                                and ptr.ptrstatus = 'A'
                                and ptr.ptrexercicio = '{$_SESSION['exercicio']}'
                                and ptr.sisid = 23 group by vede.edeid, s.ppsid) as sub on sub.edeid = ide.edeid and sub.ppsid = ps.ppsid
			WHERE ps.frmid in (14, 15)
				and ide.edeid = {$_POST['edeid']}
				$filtro
 			order by ps.ppsdsc ";
	
	$cabecalho = array('Selecione', 'Descrição');
	$db->monta_lista($sql, $cabecalho, 10000000, 30, 'N', 'center', '', 'formAcoesPar');
	exit();
}

if( $_REQUEST['requisicao'] == 'mostraRecurso' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$sql = "SELECT DISTINCT
			    ed.edeid as id,
			    ee.enbcnpj as cnpj,
			    (select ug.ungcod from public.unidadegestora ug where ug.ungcnpj = ee.enbcnpj limit 1) as gestora,
			    ee.enbnome as nome,
			    ee.estuf as uf,
			    ed.edevalor as valor,
			    ed.edevalorreducao
			FROM
			    emenda.emendadetalheentidade ed
				inner join emenda.entidadebeneficiada ee ON ee.enbid = ed.enbid
			WHERE
			    ed.edeid = {$_POST['edeid']} 
			ORDER BY
			    nome asc";
	$dados = $db->pegaLinha( $sql );
	$dados = ($dados ? $dados : array() );
	$edevalorreducao = number_format( $dados['edevalorreducao'], 2, ',', '.');
	?>
	<form action="" method="post" id="formReducao" name="formReducao">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=6 align="center" style="width: 100%">
		<tr>
			<th colspan="4" style="text-align: center;">Dados da Entidades Beneficiadas</th>
		</tr>
		<tr>
			<td class="subtitulodireita" width="20%">CNPJ:</td>
			<td width="30%"><?=$dados['cnpj'] ?></td>
			<td class="subtitulodireita" width="20%">Entidade Beneficiada:</td>
			<td width="30%"><?=$dados['nome'] ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">UG:</td>
			<td><?=$dados['gestora'] ?></td>
			<td class="subtitulodireita">UF:</td>
			<td><?=$dados['uf'] ?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Valor (R$):</td>
			<td><?php echo 'R$ ' . number_format( $dados["valor"], 2, ',', '.');?></td>
			<td class="subtitulodireita" style="color: red;">Valor a Reduzir(R$):</td>
			<td><?php echo campo_texto( 'edevalorreducao', 'N', 'S', '', 16, 30, '###.###.###,##', '', 'left', '', 0, 'id="edevalorreducao"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value)");  ?></td>
		</tr>
	</table>
	</form>
	<?
	exit();
}

if($_REQUEST['verificavinculo']){
	header('content-type: text/html; charset=ISO-8859-1');
	validaEmendaVinculoPTA( $_POST );
	exit;
}
if($_REQUEST['verificavinculoentidade']){
	header('content-type: text/html; charset=ISO-8859-1');
	validaEmendaEntidadeVinculoPTA( $_POST );
	exit;
}
if($_REQUEST['verificavinculoentidadeemenda']){
	header('content-type: text/html; charset=ISO-8859-1');
	validaEmendaEntidadeVinculoPTA( $_POST );
	exit;
}

if ( $_REQUEST["ajax"] == 'alterarentidade' ){
	header('content-type: text/html; charset=ISO-8859-1');
	echo buscaDadosEntidadeDetalhe( $_REQUEST["edeid"] );
	die;
}

switch ($_REQUEST["requisicao"]) {
	case 'buscarentidade':
		$enbentidade = buscarEntidadeBeneficiadaPorCNPJ($_REQUEST["enbcnpj"]);
		
		ob_clean();
		
		if($enbentidade) echo simec_json_encode($enbentidade);
		else echo "naoexiste";
			
		die;
	break;
	case 'verificaentidade':
		$enbids = buscaEntidadePorCNPJ( $_REQUEST["enbcnpj"] );
		echo count($enbids);
		die;
	break;
	case 'populaentidadebase':
		buscaEntidadePorCNPJ( $_REQUEST["enbcnpj"], 'populabase' );
		die;
	break;
	case 'verificaentidadebase':
		buscaEntidadePorCNPJBase( $_REQUEST );
		die;
	break;
	case 'selecioaEntidade':
		echo dadosEntidadeSelecionada( $_REQUEST["enbid"] );
		die;
	break;
	case 'cadastraIniciativa':
		insereIniciativaEmendaDetalhe( $_REQUEST );
	break;
	
	case 'cadastra':
		inserirEntidadeDetalhe( $_REQUEST );		
	break;
	
	case 'excluir':
		verificaEmenda( $_REQUEST["edeid"], 'detalheEntidade' );
		excluirEntidadeDetalhe( $_REQUEST["edeid"] );
	break;
	
	default:
	break;
	
}

if ( $_REQUEST["emdid"] || $_SESSION["emenda"]["emdid"] ){

	$_SESSION["emenda"]["emdid"] = $_REQUEST["emdid"] ? $_REQUEST["emdid"] : $_SESSION["emenda"]["emdid"];
	verificaEmenda( $_SESSION["emenda"]["emdid"], 'detalheEmenda' );
	pegaEmendaPorDetalhe( $_SESSION["emenda"]["emdid"] );
	
	$dadosDetalheEmenda = pegaDadosDetalheEmenda( $_SESSION["emenda"]["emdid"] );
	
}else{
		
	print "<script>"
		. "		alert('Favor selecionar um detalhe da emenda!');"
		. "		history.back(-1);"
		. "</script>";
		
}

$respAutor = verificaAutor( "cadastraDetalheEmenda" );

validaSessionPTA( $_SESSION["emenda"]["emdid"], false );

$emdid = $_SESSION["emenda"]["emdid"];

$sql = "SELECT 
		  count(iedid) as total
		FROM 
		  emenda.iniciativaemendadetalhe
		WHERE
			emdid = ".$emdid."
			and iedstatus = 'A'";

$total = $db->pegaUm( $sql );

$etoid = $dadosDetalheEmenda['etoid'];

$habilita = ( $total == 0 ? 'N' : 'S');

if( $etoid == 4 ) $habilita = 'S';

$boEntidade = 'false';
/*if( $dadosDetalheEmenda['acacod'] != '0048' ){
	if($dadosDetalheEmenda['mapcod']  == '30' || $dadosDetalheEmenda['mapcod']  == '40'){
		$boEntidade = 'true';
	}
}*/

require_once APPRAIZ. "www/includes/webservice/pj.php";
require_once APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
// Monta as abas e o título
$db->cria_aba($abacod_tela,$url,$parametros);
//monta_titulo( 'Dados do Detalhe da Emenda', '');

$perfilEmenda = pegaPerfilArray( $_SESSION["usucpf"] );

$boImpositivo = $db->pegaUm("select count(ed.emdid) from emenda.emenda v
								inner join emenda.emendadetalhe ed on ed.emeid = v.emeid 
							where emdimpositiva = '6'
								and ed.emdid = $emdid");

$boLiberarEmenda = true;
/*if( $boImpositivo > 0 && $_SESSION['exercicio'] == date('Y') ){
	$dataFimLibera = $_SESSION['exercicio']."-02-18";
	$boLiberarEmenda = verificaDataImpositivo($dataFimLibera);
}*/

//if( $dadosDetalheEmenda["autid"] == 2448 ) $filAutor = " and e.emecod = '{$dadosDetalheEmenda['numero']}' ";

$sql = "select
		    sum(ed.emdvalor) as valor
		from
			emenda.emenda e
		    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
		where
			e.autid = {$dadosDetalheEmenda["autid"]}
			and e.emelibera = 'S'
			and e.emeano = '{$_SESSION['exercicio']}'";
$vlEmendaAutor = $db->pegaUm($sql);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<style type="">
	.ui-dialog-titlebar{
    	text-align: center;
    }
</style>

<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />

<script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/webservice/cpf.js"></script>
<script type="text/javascript" src="/includes/webservice/pj.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>

<head>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
</head>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form action="" method="post" id="formEntidadeDetalhe" name="formEntidadeDetalhe">
	<input type="hidden" value="cadastraIniciativa" name="requisicao"/>
	<input type="hidden" value="" id="boentidade" name="boentidade"/>
	<input type="hidden" value="<?=$boEntidade; ?>" id="entidadePar" name="entidadePar"/>
	<input type="hidden" value="<?=$emdid; ?>" name="emdid"/>
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<th colspan="4" style="text-align: center;">Dados da Emenda/Autor</th>
		</tr>
		<tr>
			<td class="subtitulodireita" width="50%" colspan="2">
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" style="width: 100%">
					<tr>
						<th colspan="2" style="text-align: center;">Dados da Emenda</th>
					</tr>
					<tr>
						<td class="subtitulodireita" width="50%"><b>Número:</b></td>
						<td width="50%"><?php echo $dadosDetalheEmenda["numero"];?></td>
					</tr>
					<tr>
						<td class="subtitulodireita"><b>Valor Emenda:</b></td>
						<td colspan="2"><?php echo 'R$ ' . number_format($dadosDetalheEmenda["valor"] , 2, ',', '.');?></td>
					</tr>
<?php 			if( $_SESSION['exercicio'] < '2015' ){?>
					 <tr>
						<td class="subtitulodireita"><b>Ajuste com base na RCL(Valor Final):</b></td>
						<td colspan="2"><?php 
							/*$percent = (25.363 / 100);
							$vlrReducao = ($percent * $dadosDetalheEmenda["valor"]);*/								
							echo 'R$ ' . number_format($dadosDetalheEmenda['emdliberacaosri'] , 2, ',', '.');?></td>
					</tr>
<?php 			}?>
					<tr>
						<td class="subtitulodireita"><b>Valor Distribuido:</b></td>
						<td colspan="2"><?php 
							$vlrTotalEntidade = $db->pegaUm("select sum(edevalor) from emenda.emendadetalheentidade where edestatus = 'A' and emdid = ".$emdid);
							$vlrTotalReducao = $db->pegaUm("select sum(edevalorreducao) from emenda.emendadetalheentidade where edestatus = 'A' and emdid = ".$emdid);
							$saldoEmenda = ($vlrTotalEntidade - $vlrTotalReducao);
							if( $saldoEmenda <= $dadosDetalheEmenda["valor"] ){
								echo '<span style="color: blue;">R$ ' . number_format( ($vlrTotalEntidade - $vlrTotalReducao) , 2, ',', '.')."</span>";
							} else {
								echo '<span style="color: red;">R$ ' . number_format( ($vlrTotalEntidade - $vlrTotalReducao) , 2, ',', '.')."</span>";
							}
							?></td>
					</tr>
				</table>
			</td>
			<td class="subtitulodireita" width="50%" colspan="2">
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" style="width: 100%">
					<tr>
						<th colspan="2" style="text-align: center;">Dados do Autor</th>
					</tr>
					<tr>
						<td class="subtitulodireita" width="50%"><b>Nome:</b></td>
						<td width="50%"><?php echo $dadosDetalheEmenda["autor"];?></td>
					</tr>
					<tr>
						<td class="subtitulodireita"><b>Soma dos valores de todas as emendas<br> do(a) <?php echo $dadosDetalheEmenda["autnome"];?>:</b></td>
						<td><?php echo 'R$ '.number_format($vlEmendaAutor, '2', ',', '.'); ?></td>
					</tr>
					<!--  <tr>
						<td class="subtitulodireita"><b>Redução com base em 1,2% da RCL de 2013:</b></td>
						<td><?php 
							/*$percent = (25.363 / 100);
							$vlrReducao = ($percent * $vlEmendaAutor);	*/							
							echo 'R$ ' . number_format($vlrReducao , 2, ',', '.');
						?></td>
					</tr>
					<tr>
						<td class="subtitulodireita"><b>Valor Final:</b></td>
						<td colspan="2"><?php 
						$percent = (25.363 / 100);
						$vlrReducao = ($percent * $vlEmendaAutor);	
							
						echo 'R$ ' . number_format( ($vlEmendaAutor - $vlrReducao) , 2, ',', '.');?></td>
					</tr>-->
				</table>
			</td>
		</tr>
		<tr>
			<th colspan="4" style="text-align: center;">Dados do detalhe da Emenda</th>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Unidade Orçamentária:</b></td>
			<td><?php echo $dadosDetalheEmenda["unidade"];?></td>
			<td class="subtitulodireita"><b>Responsável:</b></td>
			<td><?php echo $dadosDetalheEmenda["responsavel"];?></td>
						
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Modalidade de Aplicação:</b></td>
			<td><?php echo $dadosDetalheEmenda["mod"];?></td>
			<td class="subtitulodireita"><b>Fonte:</b></td>
			<td><?php echo $dadosDetalheEmenda["fonte"];?></td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>GND:</b></td>
			<td><?php echo $dadosDetalheEmenda["gnd1"];?></td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Funcional Programática:</b></td>
			<td colspan="3">
				<b><?php echo $dadosDetalheEmenda["funcional"].'</b> - '.$dadosDetalheEmenda["subtitulo"];?>
			</td>
		</tr>
		<?
		if( !in_array(  AUTOR_EMENDA, $perfilEmenda ) ){
		 ?>
		<tr>
			<td class="subtitulodireita"><b>Iniciativas da Emenda:</b></td>
			<td colspan="2">
				<?php
				
					if ( !$dadosDetalheEmenda["resid"] ){
						print "<span style='color:red;'>Não é possível atribuir uma Iniciativa pois a Emenda não possui um responsável atribuído.</span>";
					}else{
						
						$iniid = pegaIniciativasEmenda( $emdid );
						$arEdeobjeto = $iniid;
						
						
						$sql = "SELECT
									i.iniid as codigo,
									i.ininome as descricao
								FROM
									emenda.iniciativa i
									inner join emenda.iniciativaresponsavel ir
										on ir.iniid = i.iniid
										and ir.irestatus = 'A'
										and i.inistatus = 'A'
								WHERE
									i.gndcod = {$dadosDetalheEmenda["gnd"]} AND
									ir.resid  = {$dadosDetalheEmenda["resid"]}
								ORDER BY
									i.ininome";
									
						combo_popup( 'iniid', $sql, '', '400x400', 0, array(), '', $respAutor["habilitadoEmendas"], false, false, 05, 400 );
					}
					if( in_array( SUPER_USUARIO, $perfilEmenda ) || in_array( GESTOR_EMENDAS, $perfilEmenda ) || in_array( LIBERAR_SENHA, $perfilEmenda ) || in_array( ASSESSORIA_PARLAMENTAR, $perfilEmenda ) ){
						$disabled = 'onclick="salvaDetalheEmenda();"  style="cursor: pointer;" ';
					} else {
						$disabled =  ( !possuiPermissao('geral') ? 'disabled="disabled"' :  $respAutor["disabledEmendas"] );
					}
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="4" align="center">
				<input type="button" value="Salvar" <?=$disabled; if ( !$dadosDetalheEmenda["resid"] ){ echo "disabled"; }?> />
				<input type="button" value="Voltar" onclick="window.location='emenda.php?modulo=principal/cadastroEmendas&acao=A';" style="cursor: pointer;"/>
			</td>
		</tr>
		<?} ?>
	</table>
</form>
<br>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<th colspan="2"> Entidades Beneficiadas pelo Detalhe da Emenda </th>
	</tr>
	<tr>
		<td colspan="2" style="text-align: center;"><b><span style="color: red;">ATENÇÃO<br>			
				Somente é permitido o cadastramento de CNPJ's referentes a entes federados ou a entidades privadas
				sem fins lucrativos, nesse último caso, exclusivamente, Associação de Pais e Amigos dos Excepcionais (APAES).
			</span></b></td>
	</tr>
	<?
	if( $habilita == 'N' ){
		echo '<tr>
				<td colspan="2" style="color: red">Para distribuir o recurso para entidades é necessário vincular iniciativa a emenda!</td>
			</tr>';
	}
	?>
	<tr>
		<td colspan="2">
			<form id="formulario" name="formulario" method="post" action="">
				<input type="hidden" value="cadastra" name="requisicao"/>
				<input type="hidden" value="<?=$emdid; ?>" name="emdid" id="emdid" />
				<input type="hidden" value="" name="edeid" id="edeid"/>
				<input type="hidden" value="<?php echo $_SESSION['exercicio']; ?>" name="exercicio" id="exercicio"/>
				<input type="hidden" value="<?php echo date('Y'); ?>" name="anoatual" id="anoatual"/>
				<input type="hidden" value="<?=$dadosDetalheEmenda["emetipo"]; ?>" name="emetipo" id="emetipo"/>
				<input type="hidden" value="<?=$dadosDetalheEmenda["numero"]; ?>" name="emecod" id="emecod"/>
				<input type="hidden" value="<?=$dadosDetalheEmenda['mapcod']; ?>" name="mapcod" id="mapcod"/>
				<input type="hidden" value="<?=$dadosDetalheEmenda["mod"]; ?>" name="mod" id="mod"/>
				<input type="hidden" value="<?=$dadosDetalheEmenda["acacod"]; ?>" name="acacod" id="acacod"/>
				<input type="hidden" value="<?=$dadosDetalheEmenda['emeid']; ?>" name="emeid"/>
				<input type="hidden" value="<?=$dadosDetalheEmenda['etoid']; ?>" name="etoid"/>
				<input type="hidden" value="<?=$dadosDetalheEmenda["autor"]; ?>" name="autor"/>
				
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" style="width: 100%">
					<tr>
						<th width="13%">CNPJ</th>
						<th width="24%">Entidade Beneficiada</th>
						<th width="05%">UF</th>
						<th width="10%">Valor (R$)</th>
						<th width="22%">Iniciativa <img src="../imagens/ico_ajuda.gif" title="As iniciativas escolhidas neste campo serão as alternativas disponíveis na elaboração do plano de trabalho para a instituição"></th>
						<th width="12%">Nome Usuário</th>
						<th width="10%">Data Alteração</th>
						<th width="05%">Ação</th>
					</tr>
					<!-- <tr id="trReceita" style="display: none;">
						<td colspan='6' style='background: rgb(252, 253, 219) none repeat scroll 0% 0%; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;'>
							<img src="../imagens/restricao.png"> <b>Esta não é a entidade que procura?</b> <a href="#" onclick="procurarNome($('entnumcpfcnpj').value, true);">Clique aqui</a> para consultar a base da Receita Federal.
						</td>
					</tr>-->
					<tr align="center">
						<td>
							<span id="lupaCnpj" style="text-align: center; vertical-align: middle;"></span>
							<input type="hidden" name="enbid" id="enbid" value=""/>
							<?php echo campo_texto( 'enbcnpj', 'N', $habilita, '', 14, 18, '##.###.###/####-##', '', 'left', '', 0, 'id="enbcnpj"', '', '', 'validarCNPJ(this);verificarEntidadeBeneficiada();');  ?>
						</td>
						<td>
							<?php echo campo_texto( 'enbnome', 'N', 'N', '', 40, 150, '', '', 'left', '', 0, 'id="enbnome"');  ?>
						</td>
						<td>
							<?php echo campo_texto( 'estuf', 'N', 'N', '', 3, 2, '', '', 'left', '', 0, 'id="estuf"');  ?>
							<input type="hidden" name="muncod" id="muncod">
						</td>
						<td>
							<?php
							$habValor = 'N';
							if( $dadosDetalheEmenda["emetipo"] == 'X' ) $habValor = 'S';
							echo campo_texto( 'edevalor', 'N', $habValor, '', 16, 15, '###.###.###,##', '', 'left', '', 0, 'id="edevalor"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value)");  ?>
						</td>
						<td id="combo_objeto">
							<?php 
							$sql = "SELECT
										ei.iniid as codigo,
										ei.ininome as descricao
									FROM 
										emenda.iniciativa ei
									INNER JOIN 
										emenda.iniciativaresponsavel ir on ir.iniid = ei.iniid 
										--and ir.irestatus = 'A'
									INNER JOIN
										emenda.iniciativaemendadetalhe id ON ei.iniid = id.iniid
									WHERE
										id.emdid = {$emdid} AND iedstatus = 'A'
									ORDER BY
										ininome";
							
							combo_popup( 'edeobjeto', $sql, '', '400x400', 0, array(), '', ($habilita ? $habilita : $respAutor["habilitadoEmendas"]), false, false, 05, 250 );
							?>
						</td>
						<td>
							<div id="usucpfalteracao"></div>
						</td>
						<td>
							<div id="ededataalteracao"></div>
						</td>
						<td>
							<?
							if( possuiPermissao() && (in_array( SUPER_USUARIO, $perfilEmenda ) || in_array( GESTOR_EMENDAS, $perfilEmenda )) ){ ?>
								<img src="/imagens/gif_inclui.gif" title="Incluir" style="cursor: pointer;" onclick="insereDadosEntidade();">
								<img src="/imagens/excluir.gif" title="Cancelar" style="cursor: pointer;" onclick="limpaDadosEntidade();">
							<?	
							} else {
								if( possuiPermissao() && (in_array( LIBERAR_SENHA, $perfilEmenda ) || in_array( AUTOR_EMENDA, $perfilEmenda ) || in_array( ASSESSORIA_PARLAMENTAR, $perfilEmenda ) ) && $habilita == 'S' ) { ?>
									<img src="/imagens/gif_inclui.gif" title="Incluir" style="cursor: pointer;" onclick="insereDadosEntidade();">
									<img src="/imagens/excluir.gif" title="Cancelar" style="cursor: pointer;" onclick="limpaDadosEntidade();">
							<? } else { ?>
									<img src="/imagens/gif_inclui_d.gif" style="cursor: pointer;" onclick="">
									<img src="/imagens/excluir_01.gif" style="cursor: pointer;" onclick="">
							<? } 
							}?>
							
						</td>
					</tr>
					<tr bgcolor="#F5F5F5">
						<td colspan="10">
							<table width="95%" border="0">
								<tr>
									<td></td>
									<th colspan="5">Dados do responsável pela elaboração do PTA</th>
								</tr>
								<tr>
									<td width="05%" style="text-align: right;"><img border="0" src="../imagens/seta_filho.gif"></td>
									<th width="15%">CPF</th>
									<th width="30%">Nome</th>
									<th width="10%">(DDD) Telefone</th>
									<th width="15%">Função/Cargo</th>
									<th width="30%">E-mail</th>
								</tr>
								<tr>
									<td></td>
									<td style="text-align: center;"><?=campo_texto( 'edecpfresp', 'N', $habilita, '', 14, 14, '###.###.###-##', '','','','','id="edecpfresp"','','',"this.value=mascaraglobal('###.###.###-##',this.value); buscaCPFReceita(this.value);"); ?></td>
									<td style="text-align: center;"><?=campo_texto( 'edenomerep', 'N', 'N', '', 40, 200, '', '','','','','id="edenomerep"'); ?></td>
									<td style="text-align: center;" nowrap="nowrap">(<?=campo_texto( 'ededddresp', 'N', $habilita, '', 2, 2, '##', '','','','','id="ededddresp"'); ?>)&nbsp;<?=campo_texto( 'edetelresp', 'N', $habilita, '', 10, 9, '####-####', '','','','','id="edetelresp"'); ?></td>
									<td style="text-align: center;" nowrap="nowrap"><?=campo_texto( 'edefuncaoresp', 'N', $habilita, '', 20, 100, '', '','','','','id="edefuncaoresp"'); ?></td>
									<td style="text-align: center;" nowrap="nowrap"><?=campo_texto( 'edemailresp', 'N', $habilita, '', 40, 100, '', '','','','','id="edemailresp"', '', '', 'validaEmail(this.value);'); ?></td>
								</tr>
							</table>
						</td>
					</tr>
				</table>
				<br>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" style="width: 100%">
					<tr>
						<th colspan="12">Lista de Entidades Beneficiadas</th>
					</tr>
					<tr>
						<td colspan="12">
							<table cellSpacing="0" cellPadding="0" align="center" style="width: 100%;">
							<tr>
								<td class="subtitulodireita" width="20%" style="font-weight: bold" valign="top">Legenda:</td>
								<td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='/imagens/alteracao.gif' title='Alterar' style='cursor: pointer;'>&nbsp;-&nbsp;Vincular Ações do PAR a Entidade</td> 
								<td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='/imagens/alterar.gif' title='Alterar' style='cursor: pointer;'>&nbsp;-&nbsp;Alterar Detalhe entidade</td>
								<td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='/imagens/excluir.gif' title='Excluir' style='cursor: pointer;'>&nbsp;-&nbsp;Excluir Detalhe entidade</td>
								<!--  <td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='/imagens/editar_nome.gif' title='Reduzir' style='cursor: pointer;'>&nbsp;-&nbsp;Redução com base em 1,2% da RCL de 2013</td> -->
							</tr>
						</table>
						</td>
					</tr>
					<tr>
						<th width="13%">CNPJ</th>
						<?php if( $_SESSION['usucpf'] == '05646593638' ) echo '<th>edeid</th>'; ?>
						<th width="24%">Entidade Beneficiada</th>
						<th width="05%">UG</th>
						<th width="05%">UF</th>
						<th width="10%">Valor (R$)</th>
						<?if( $_SESSION['exercicio'] >= '2014' ){ ?>
						<th width="05%">Redução(R$)</th>
						<!--  <th width="05%">Reduzido(R$)</th>-->
						<?} ?>
						<th width="22%">Iniciativa</th>
						<th width="12%">Nome Usuário</th>
						<th width="10%">Data Alteração</th>
						<th width="05%">Ação</th>
					</tr>
				<?php listaEntidadesDetalhe( $emdid, $habilita ); ?>
				</table>
				<div id="acoespar"></div>
			</form>
		</td>
	</tr>
</table>
<div id='divTeste'></div>
<div id="dialog_acoes" title="Vincular Ações do PAR" style="display: none" >
	<div style="padding:5px;text-align:justify;" id="mostraRetorno"></div>
</div>

<div id="dialog_recurso" title="Redução com base em 1,2% da RCL de 2013" style="display: none" >
	<div style="padding:5px;text-align:justify;" id="mostraRetornoRecurso"></div>
</div>
<script type="text/javascript">
	jQuery.noConflict();
	<?if( !$boLiberarEmenda ){ ?>
		if(!confirm('Esta Emenda é Impositivo. Seu Prazo de indicação expirou, deseja indicar mesmo assim?')){
			window.location.href = 'emenda.php?modulo=principal/cadastroEmendas&acao=A';
		}
	<?} ?>
	
	jQuery(document).ready(function(){
		if(jQuery('#entidadePar').val() == 'true'){
			jQuery('#tr_par').show();
		} else {
			jQuery('#tr_par').hide();
		}
	});

	/*function romualdo(){
		selectAllOptions( document.getElementById( 'edeobjeto' ) );
		
		jQuery.ajax({
				type: "POST",
				url: 'emenda.php?modulo=principal/cadastraDetalheEmenda&acao=A',
				async: false,
				success: function(retorno) {
					console.log(retorno);
				}
			});
	}*/
	
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url){
		
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(500,300);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function displayStaticMessage(messageContent,cssClass){
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(500,300);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
		
		
	}
	
	function closeMessage(){
		messageObj.close();	
	}
	
	function validarCNPJ( obj ){

		if( obj.value ){
			if( !validarCnpj( obj.value  ) ){
				alert( "CNPJ inválido!\nFavor informar um CNPJ válido!" );
				obj.value = "";	
				document.getElementById( "enbnome" ).value = "";
			}
		}	
		
	}
	
	function salvaDetalheEmenda(){ 
		selectAllOptions( document.getElementById( 'iniid' ) );
		
		var retornoEntidade = verificaEmendaDetalheExisteVinculoEntidade();
		if( retornoEntidade == 'false' ){
			return false;
		}
		var retornoPTA = validaEmendaEntidadeVinculoPTA();
		if( retornoPTA == 'false' ){
			return false;
		}
		document.getElementById( 'formEntidadeDetalhe' ).submit();
	}
	/**
	verifica se a emenda tem vinculo com o plano de trabalho
	*/
	
	function validaEmendaEntidadeVinculoPTA(){
		var retorno = '';
		var myajax = new Ajax.Request('emenda.php?modulo=principal/cadastraDetalheEmenda&acao=A', {
				        method:     'post',
				        parameters: '&verificavinculoentidade=true&'+$('formEntidadeDetalhe').serialize(),
				        asynchronous: false,
				        onComplete: function (res){
				        	//$('divTeste').update(res.responseText);
				        	if( res.responseText != '' ){
				        		alert( 'A(s) iniciativa(s) abaixo não pode(m) ser excluída(s), porque est(a)(ão) vinculada(s) ao PTA:\n' +  res.responseText );
								retorno = 'false';
				        	} else {
				        		retorno = 'true';
				        	}
				        }
				  });
		return retorno;
	}
	/**
	Verifica se existe vinculo entre a emenda e a entidade
	*/
	function verificaEmendaDetalheExisteVinculoEntidade(){
		var retorno = '';
		
		$('boentidade').value = 'true';
		var myajax = new Ajax.Request('emenda.php?modulo=principal/cadastraDetalheEmenda&acao=A', {
				        method:     'post',
				        parameters: '&verificavinculoentidadeemenda=true&'+$('formEntidadeDetalhe').serialize(),
				        asynchronous: false,
				        onComplete: function (res){
				        	//$('divTeste').update(res.responseText);
				        	if( res.responseText != '' ){
				        		alert( 'A(s) iniciativa(s) abaixo não pode(m) ser excluída(s), porque est(a)(ão) vinculada(s) a Entidade:\n' +  res.responseText );
								retorno = 'false';
				        	} else {
				        		retorno = 'true';
				        	}
				        }
				  });
		return retorno;
	}
	
	function in_array(needle,haystack) {
		return new RegExp('(^|\,)'+needle+'(\,|$)','gi').test(haystack);	
	}
	
	function carregaDetalheEmendaEntidade(idImg, edeid){
		var img 	 = $( idImg );
		var tr_nome = 'listaDetalheEntidade_'+ edeid;
		var td_nome  = 'trV_'+ edeid;
		
		if($(tr_nome).style.display == 'none' && $(td_nome).innerHTML == ""){
			$(td_nome).innerHTML = 'Carregando...';
			img.src = '../imagens/menos.gif';
			//carregaDetalheEntidade(edeid, td_nome);
		}if($(tr_nome).style.display == 'none' && $(td_nome).innerHTML != ""){
			$(tr_nome).style.display = '';
			img.src = '../imagens/menos.gif';
		} else {
			$(tr_nome).style.display = 'none';
			img.src = '/imagens/mais.gif';
		}
	}
	
	function vinculaSubAcoes( edeid ){
		
		jQuery.ajax({
			type: "POST",
			url: "emenda.php?modulo=principal/cadastraDetalheEmenda&acao=A",
			data: "requisicao=mostraAcoesPar&edeid="+edeid+"&mapcod="+jQuery('#mapcod').val(),
			async: false,
			success: function(msg){
				jQuery( "#dialog_acoes" ).show();
				jQuery( "#mostraRetorno" ).html(msg);
				jQuery( '#dialog_acoes' ).dialog({
						resizable: false,
						width: 800,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							'Salvar': function() {
								vinculaAcaoPar( edeid );
								//jQuery( this ).dialog( 'close' );
							},
							'Cancelar': function() {
								jQuery( this ).dialog( 'close' );
							}
						}
				});
			}
		});
	}
	
	function mostraRecurso( edeid ){
		
		jQuery.ajax({
			type: "POST",
			url: "emenda.php?modulo=principal/cadastraDetalheEmenda&acao=A",
			data: "requisicao=mostraRecurso&edeid="+edeid,
			async: false,
			success: function(msg){
				jQuery( "#dialog_recurso" ).show();
				jQuery( "#mostraRetornoRecurso" ).html(msg);
				jQuery( '#dialog_recurso' ).dialog({
						resizable: false,
						width: 800,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							'Salvar': function() {
								if( jQuery('[name="edevalorreducao"]').val() == '' ){
									alert('O Campo "Valor a Reduzir(R$)" não pode ser vazio.');
									jQuery('[name="edevalorreducao"]').focus();
									return false;
								}
								reduziValorRecurso( edeid );
								jQuery( this ).dialog( 'close' );
							},
							'Cancelar': function() {
								jQuery( this ).dialog( 'close' );
							}
						}
				});
			}
		});
	}
	
	function vinculaAcaoPar( edeid ){
		var dados = jQuery('#formAcoesPar').serialize();
		//console.log(dados);
		
		jQuery.ajax({
			type: "POST",
			url: "emenda.php?modulo=principal/cadastraDetalheEmenda&acao=A",
			data: "requisicao=vincularAcoesPar&edeid="+edeid+"&"+dados,
			async: false,
			success: function(msg){
				
				if(msg == 1){
					alert('Operação Realizada com Sucesso!');
				} else {
					alert('Falha na Operação!');
				}
				window.location.href = window.location;
			}
		});
	}
	
	function reduziValorRecurso( edeid ){
		var dados = jQuery('#formReducao').serialize();
		//console.log(dados);
		
		jQuery.ajax({
			type: "POST",
			url: "emenda.php?modulo=principal/cadastraDetalheEmenda&acao=A",
			data: "requisicao=reduziValorRecurso&edeid="+edeid+"&"+dados,
			async: false,
			success: function(msg){
				if(msg == 1){
					alert('Operação Realizada com Sucesso!');
				} else {
					alert('Falha na Operação!');
				}
				window.location.href = window.location;
			}
		});
	}
		
</script>
<div id="debug"></div>