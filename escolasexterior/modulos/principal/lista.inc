<?php

// Inativar escola
if ($_POST['inativar_escola'] || $_POST['ativar_escola']) {
	$status = isset($_POST['ativar_escola']) ? 'A' : 'I';
	$sql = "UPDATE escolasexterior.escolasexterior SET eexstatus = '{$status}' WHERE eexid = ".$_POST['eexid'];
	$db->executar($sql);
	if($db->commit()){
		die('true');
	}
	die('false');	
}


unset($_SESSION['eexid']);

if ($_REQUEST['eexid']){
	$_SESSION['eexid'] = $_REQUEST['eexid'];
	header( "Location: escolasexterior.php?modulo=principal/dadosEscola&acao=A" );
	exit();
}

/*
if ( selecionarEntidade($_GET['entid']) ){
	header( "Location: escolasexterior.php?modulo=painel&acao=A" );
	exit();
}
*/

include APPRAIZ . 'includes/cabecalho.inc';

echo '<br/>';

$abacod_tela = 57522;
$url = 'escolasexterior.php?modulo=lista&acao=A';
$parametros = null;
$arMnuid = array();

$db->cria_aba($abacod_tela,$url,$parametros, $arMnuid);

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="post" name="formulario" id="formulario">
					<input type="hidden" name="pesquisa" value="1" />
					<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
					<div style="float: left;">
						<table width="100%" border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom">
									Escola:
									<br/>
									<?php $eexnomeestabelecimento = simec_htmlentities( $_REQUEST['eexnomeestabelecimento'] ); ?>
									<?= campo_texto( 'eexnomeestabelecimento', 'N', 'S', '', 50, 200, '', '' ); ?>
								</td>
								<td valign="bottom">
									Código Escola:
									<br/>
									<?php $eexid_pesq = simec_htmlentities( $_REQUEST['eexid_pesq'] ); ?>
									<?= campo_texto( 'eexid_pesq', 'N', 'S', '', 30, 200, '', '' ); ?>
								</td>								
							</tr>
							<tr>
								<td>
									Situação:
									<br>
									<?php
									$esdid = $_REQUEST['esdid'];
									$sql = "SELECT
											 esdid AS codigo,
											 esddsc AS descricao
											FROM
											 workflow.estadodocumento et
											 INNER JOIN workflow.tipodocumento tp ON tp.tpdid = et.tpdid AND
											 	sisid = ".$_SESSION['sisid']."
											 WHERE et.tpdid = ".WF_TPDID_EE."
											ORDER BY
											 esdordem;";
									
									$db->monta_combo( "esdid", $sql, 'S', 'Selecione...', '', '' );
									?>
								</td>
								<td>
									Etapa de Ensino:
									<br/>
									<?php
									$eteid = $_REQUEST['eteid'];
									$sql = "SELECT
											 	ete.eteid AS codigo,
											 	ete.etedsc AS descricao
											FROM
											 	escolasexterior.etapaensino ete
											ORDER BY
												ete.etedsc;";
									
									$db->monta_combo( "eteid", $sql, 'S', 'Selecione...', '', '' );
									?>
								</td>							
							</tr>
							<?php 
							if( checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR)) ) {
							?>
							<tr>
								<td valign="middle" id="anoanterior">
									Mostrar escolas inativas?
									<input type="checkbox" name="status" value="1" <?php echo ($_REQUEST['status'] == 1) ? 'checked' : '' ?>/>Sim
								</td>
							</tr>
							<?php } ?>
							<tr>
								<td><br/></td>
							</tr>
						</table>
						<div style="float: left;">
							<input type="button" name="" value="Pesquisar" id="btnPesquisar"/>
							&nbsp;&nbsp;
							<input type="button" name="" value="Cadastrar Nova Escola" id="btnCadEscola" onclick="location.href = 'escolasexterior.php?modulo=principal/cadastroEscola&acao=A';"/>
						</div>
					</div>	
				</form>
			</td>
		</tr>
	</tbody>
</table>
<?php
	
$anoAnterior = $_SESSION["exercicio"]-1;	
	
extract($_POST);

$_GET['esdid'] = $_GET['esdid'] == '0' ? 'naoiniciado' : $_GET['esdid'];
$esdid = $esdid ? $esdid : $_GET['esdid'];

$perfil = arrayPerfil();


if( 
	(!in_array(PERFIL_CADASTRADOR, $perfil)
	&& !in_array(PERFIL_INTERESSADO, $perfil)) // nao ter nenhum dos dois
	// || 
	// (count($perfil) > 1 && (!in_array(PERFIL_CADASTRADOR, $perfil)
	// || !in_array(PERFIL_INTERESSADO, $perfil))) // ter mais de 1 e 1 deles não ser um dos dois (ter ao menos um perfil que possa ver - que são todos os outros)
){
	// não faz nada nessa condicional
}else if( in_array(PERFIL_CADASTRADOR, $perfil) ) // para somente perfil PERFIL_CADASTRADOR
	$where[] = " 
		e.paiid IN (
			SELECT paiid 
			FROM escolasexterior.usuarioresponsabilidade 
			WHERE 
				usucpf = '".$_SESSION['usucpf']."' 
				AND pflcod = ".PERFIL_CADASTRADOR."
		) ";
else if( in_array(PERFIL_INTERESSADO, $perfil) ) // para somente perfil PERFIL_INTERESSADO
	$where[] = " 
		e.eexid IN (
			SELECT eexid 
			FROM escolasexterior.usuarioresponsabilidade 
			WHERE 
				usucpf = '".$_SESSION['usucpf']."' 
				AND pflcod = ".PERFIL_INTERESSADO."
		) ";


if($eexnomeestabelecimento)
	$where[] = " e.eexnomeestabelecimento iLIKE '%{$eexnomeestabelecimento}%' ";

if($eexid_pesq)
	$where[] = "e.eexid = {$eexid_pesq}";	
		
if($esdid)	 	
	$where[] = $esdid != 'naoiniciado' ? "est.esdid = '{$esdid}'" : "e.docid is null";
	
if($status)
	$where[] = "e.eexstatus = 'I'";
else
	$where[] = "e.eexstatus = 'A'";

if($eteid)
	$where[] = "ete2.eteid = '{$eteid}'";
/*
if( $_GET['emianoreferencia'] == $_SESSION["exercicio"] ) {
	$where[] = " emi.entcodent not in (select mem.entcodent from escolasexterior.escolasexterior mem where mem.emianoreferencia = ".$anoAnterior." and mem.eexstatus = 'A')";
}else if( $_GET['emianoreferencia'] == $anoAnterior ) {
	$where[] = " emi.entcodent in (select mem.entcodent from escolasexterior.escolasexterior mem where mem.emianoreferencia = ".$anoAnterior." and mem.eexstatus = 'A')";
}
*/
		
		
$join = "";

/*	
	if ( checkPerfil(PERFIL_SEC_ESTADUAL) && checkPerfil(PERFIL_SEC_MUNICIPAL) ){
		$join = " INNER JOIN em.usuarioresponsabilidade ur ON ur.rpustatus = 'A' AND 
					ur.pflcod IN (".implode(',',$perfil).") AND
					ur.usucpf = '".$_SESSION['usucpf']."' AND
					(
					 (ur.muncod = m.muncod AND 
					  e.tpcid = 3) OR
 					 ur.entid  = e.entid OR
 					 (ur.estuf  = m.estuf AND
 					  e.tpcid = 1)
 					)"; 
	} 
*/

$selectExtra = "";
// ver($eteid,d);
if( $eteid ){
	$join .= " INNER JOIN escolasexterior.etapaensescolaext eee ON eee.eexid = e.eexid AND eee.eteid = '{$eteid}' ";
	$selectExtra .= " 
		, array_to_string(array(
			SELECT eeesub.etedsc 
			FROM escolasexterior.etapaensino eeesub
			JOIN escolasexterior.etapaensescolaext eeex ON eeesub.eteid = eeex.eteid and eeex.eexid = e.eexid AND eeex.eteid = '{$eteid}'
			ORDER BY eeesub.etedsc
		),', ') AS etapaensino ";
}else{
	$selectExtra .= " 
		, array_to_string(array(
			SELECT eeesub.etedsc 
			FROM escolasexterior.etapaensino eeesub
			INNER JOIN escolasexterior.etapaensescolaext eeex ON eeesub.eteid = eeex.eteid and eeex.eexid = e.eexid
			ORDER BY eeesub.etedsc
		),', ') AS etapaensino ";
}

/**
 * Verifica se o usuário tem perfil de 'Super Usuário' ou 'Administrador'
 * e habilita a opção de alterar o eexstatus da escola
 */
if( checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR))){
    	
	$acao = "CASE WHEN e.eexstatus = 'A'
				THEN '<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" id=\"eexid=' || e.eexid || '\"><img class=\"btnDadosEscola\" src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
    			 	  <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" id=\"' || e.eexid || '\"><img class=\"btnInativarEscola\" src=\"/imagens/valida6.gif\" border=0 title=\"Inativar Escola\"></a>'
				ELSE
    			 	 '<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" id=\"eexid=' || e.eexid || '\"><img class=\"btnDadosEscola\" src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>
    			 	  <a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" onclick=\"ativarEscola(' || e.eexid || ')\"><img src=\"/imagens/valida1.gif\" border=0 title=\"Ativar Escola\"></a>'
				END";
    	
}else{    	
	
	$acao = "'<a style=\"margin: 0 -5px 0 5px;\" href=\"javascript:void(0);\" id=\"eexid=' || e.eexid || '\"><img class=\"btnDadosEscola\" src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>'";
	
}
 
 // ver($whereExterno,$where,d);
// -------------------------------------- LISTA   
/*$sql = "SELECT * FROM(
					SELECT DISTINCT
						 {$acao} as acao,
						 '' as entcodent,
						 e.eexid || ' ',
						 e.eexnomeestabelecimento,
						 e.eexcidade,
						 est.esddsc
						 {$selectExtra}
					FROM escolasexterior.escolasexterior e    						 	
					LEFT JOIN workflow.documento d 			  ON d.docid = e.docid
					LEFT JOIN workflow.estadodocumento est 	  ON est.esdid = d.esdid
					$join
					WHERE 1 = 1					
					".($where ? ' AND '.implode(' AND ',$where) : '')."
			) as foo ".($whereExterno ? ' WHERE '.implode(' AND ',$whereExterno) : '');*/
						 
						 
						 
$sql = "select
			{$acao} as acao,
			'' as entcodent,
			e.eexid || ' ',
			e.eexnomeestabelecimento,
			e.eexcidade,
			est.esddsc,
			case 
				when count(ete.*) = 0 then 'Não existem etapas cadastradas.'
				when count(ete.*) != 0 then 
					case 
						when count(ete.*) > 1 then (select ete.etedsc || ' / outras' from escolasexterior.etapaensinoescola etp inner join escolasexterior.etapaensino ete on ete.eteid = etp.eteid where eexid = e.eexid order by etpid limit 1)
						when count(ete.*) = 1 then (select ete.etedsc from escolasexterior.etapaensinoescola etp inner join escolasexterior.etapaensino ete on ete.eteid = etp.eteid where eexid = e.eexid order by etpid limit 1)
					end
			end as etapaensino
		from escolasexterior.escolasexterior e
		left join workflow.documento d ON d.docid = e.docid
		left join workflow.estadodocumento est ON est.esdid = d.esdid
		left join escolasexterior.etapaensinoescola ete2 on ete2.eexid = e.eexid 
		left join escolasexterior.etapaensinoescola ete on ete.eexid = e.eexid
		left join escolasexterior.etapaensino et on ete.eteid = et.eteid
		where 1 = 1
		".($where ? ' AND '.implode(' AND ',$where) : '')."
		group by e.eexid, e.eexnomeestabelecimento, e.eexcidade, est.esddsc, e.eexstatus";
						 

						 
// dbg($sql,1);

$cabecalho = array( "Ação", "Cód. INEP", "Cód.",  "Escola", "Cidade", "Situação", "Etapas de Ensino" );
$larguras = array( "5%", "5%", "5%",  "10%", "10%", "15%", "40%" );
$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '', '', $larguras);


?>
<script type="text/javascript">
<!--

$(function(){

	$('#btnPesquisar').click(function(){
		$('#formulario').submit();
	});
	
		
	$('.btnInativarEscola').click(function(){
		if( confirm("Deseja inativar esta escola?") ){
			eexid = $(this).parent().attr('id');			
			$.ajax({
				url: 'escolasexterior.php?modulo=principal/lista&acao=A',
				type: 'post',
				data: 'inativar_escola=true&eexid='+eexid,
				success: function(res){
					if(res=='true'){
						alert('Escola inativada com sucesso.');
						document.location = document.location.href;
					}else{
						alert('Não foi possível inativar a escola!');
					}
				}
			});
		}
	});
	
	$('.btnDadosEscola').click(function(){	
		params = $(this).parent().attr('id');		
		document.location = 'escolasexterior.php?modulo=principal/lista&acao=A&'+params;
	});
	
});

function ativarEscola(eexid)
{
	if( confirm("Deseja ativar esta escola?") )
	{
		$.ajax({
			url: 'escolasexterior.php?modulo=principal/lista&acao=A',
			type: 'post',
			data: 'ativar_escola=true&eexid='+eexid,
			success: function(e){
				if(e=='true'){
					document.location = 'escolasexterior.php?modulo=lista&acao=E';
				}
			}
		});
	}
}
</script>