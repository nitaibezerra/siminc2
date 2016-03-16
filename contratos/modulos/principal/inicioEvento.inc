<?
$manutencao = "N";

if($manutencao == 'S' && !$db->testa_superuser()){
	
	$titulo_modulo = "Eventos";
	//Chamada de programa
	include  APPRAIZ."includes/cabecalho.inc";
	echo'<br>';
	
	echo '<p style="margin: 0 0 0 0;text-align: center; color: red; font-size: 30px">Sistema em manutenção.</p>
	<p style="text-align: center; font-size: 15px">Volte em alguns instantes</p>';
	
}else{
	
// CADASTRAR evento
if ( $_REQUEST['evento'] ) {
	ver(1,d);
	$evetitulo = $_REQUEST['evento'];
	if ( !empty( $evetitulo ) ) {
 		
		$numEventos = verificaEventos();
 		if( $numEventos < MAX_EVENTOS_SEM_NOTA )
		{ 
			$sql = "INSERT INTO evento.evento ( evetitulo, usucpf, sevid, evestatus ) VALUES ( '$evetitulo', '{$_SESSION['usucpf']}', 1, 'A' ) RETURNING eveid"; 
			if ( $eveid = $db->pegaUm( $sql ) ) {
				evtCriarDoc($eveid);
				//atividade_atribuir_responsavel( $atiid, PERFIL_GESTOR, array( $_SESSION['usucpf'] ) );
				$db->commit(); 
			}
		}
		else
		{
			echo '<script type="text/javascript"> 
		 			alert( "Relatórios Técnicos em Aberto" ); 
			 		window.location.href = "?modulo=inicio&acao=C" 
			 	 </script>';  
		}
	}
	
}

// REMOVER
if ( $_REQUEST['remover'] ) 
{
	$eveid = (integer) $_REQUEST['remover'];  
	$sql = "update evento.evento SET evestatus = 'I' WHERE eveid = " . $eveid;
	$db->executar( $sql );
	$db->commit();
	 
	redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
}
 

$ungcod = getUnidadeByCpf();
$perfis = arrayPerfil();
						 
if( ( ( in_array(PERFIL_SEE, $perfis ) ) || in_array(PERFIL_SAA, $perfis ) ) || ( in_array(PERFIL_SUPER_USUARIO, $perfis ) ) || in_array(PERFIL_EMPRESA, $perfis) || in_array(EVENTO_PERFIL_CONSULTA, $perfis) ){
	$condicao = '';
}elseif ( in_array(PERFIL_DEMANDANTE, $perfis) ){
 
	//$from     = "LEFT JOIN workflow.documento d ON d.docid = ev.docid ";
	//$condicao = "AND ev.ungcod = '$ungcod'";
	$condicao = " AND ( ( ev.ungcod = '$ungcod' ) OR ev.usucpf = '" . $_SESSION['usucpf'] . "' ) ";	
}else{
	$condicao = " AND (ev.ungcod = '$ungcod' OR ev.usucpf = '" . $_SESSION['usucpf'] . "' )";
}

$titulo_modulo = "Eventos";
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
?>
	<style type="text/css">	
		a.dcontexto{
		 position:relative; 
		 padding:0;
		 color:#039;
		 text-decoration: none;
		 cursor: pointer; 
		 margin-right: -1px;
		 z-index:24;
	}
	a.dcontexto:hover{
		background:transparent;
		text-decoration: none;
		z-index:25; 
	}
	a.dcontexto span{
		display: none;
		text-decoration: none;
	}
	a.dcontexto:hover span{
		display: block;
		position:absolute;
		width: 200px;
		font-size: 10px;
		top:2em;
		text-align:center;
		/*left:50px;*/
		padding:4px 0px;
		border:1px solid #000;
		background:#eee; 
		color:#000;
		text-decoration: none;
	}
	</style>
<?
monta_titulo( $titulo_modulo, 'Clique no nome do evento para iniciar o trabalho.' );
 
/*$res = array( 
 				0 => array ( "descricao" => "Lista",
						    "id" 		=> "4",
						    "link" 		=> "/evento/evento.php?modulo=inicio&acao=C&submod=evento"
				  		  )
				  		 );
echo montarAbasArray($res, $_REQUEST['org'] ? false : "/evento/evento.php?modulo=inicio&acao=C&submod=evento");*/

if( $_POST['filtro_evetitulo']){
	$where = " AND TRANSLATE(ev.evetitulo, 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') iLIKE TRANSLATE('%{$_POST['filtro_evetitulo']}%', 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') ";
} 
if( $_POST['filtro_reuniao']){
	$where .= " AND ev.rcoid = '{$_POST['filtro_reuniao']}' ";
}


if( $_POST['filtro_ano']){
	$where .= " AND to_char(ev.evedatainclusao, 'YYYY') = '{$_POST['filtro_ano']}' ";
}


if( $_POST['filtro_unidade']){
	$where .= " AND ev.ungcod = '{$_POST['filtro_unidade']}' ";
} 
if ( ( $_GET['estuf']) && ( !$_POST['estuf'] ) ){
	$where .= " AND  m.estuf = '".$_GET['estuf']."'";
}elseif( ( $_GET['estuf']) && ( $_POST['estuf'] != '' ) ){
	$where .= " AND  m.estuf = '".$_POST['estuf']."'";
}elseif( $_POST['estuf'] != '' ){
	$where .= " AND  m.estuf = '".$_POST['estuf']."'";
} 
if ( ( $_GET['muncod']) && ( !$_POST['muncod'] ) ){
	$where .= " AND  m.muncod = '".$_GET['muncod']."'";
}elseif( ( $_GET['muncod']) && ( $_POST['muncod'] != '' ) ){
	$where .= " AND  m.muncod = '".$_POST['muncod']."'";
}elseif( $_POST['muncod'] != '' ){
	$where .= " AND  m.muncod = '".$_POST['muncod']."'";
} 


if ( ( $_GET['esdid']) && ( !$_POST['filtro_situacao'] ) ){   
	$where .= " AND  est.esdid = ".$_GET['esdid'];
}elseif( ( $_GET['esdid']) && ( $_POST['filtro_situacao'] != '' ) ){ 
	$where .= " AND  est.esdid = ".$_POST['filtro_situacao'];
}elseif( $_POST['filtro_situacao'] != '' ){ 
	$where .= " AND  est.esdid = ".$_POST['filtro_situacao'];
}

if( $_REQUEST['eveurgente'] && $_REQUEST['eveurgente'] != 'T' )
{
	$urgente = ($_REQUEST['eveurgente'] == 'S') ? 't' : 'f';
	$where .= " AND ev.eveurgente = '".$urgente."' ";
}

if( $_POST['filtro_eveid'] )
{
	$where .= " AND ev.eveid = '{$_POST['filtro_eveid']}' ";
}

if( $_POST['filtro_evenumeroprocesso'])
{
	$where .= " AND TRANSLATE(ev.evenumeroprocesso, 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') iLIKE TRANSLATE('%{$_POST['filtro_evenumeroprocesso']}%', 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') ";
}

/*
if( $_REQUEST['adreferendum'] && $_REQUEST['adreferendum'] != 'T' )
{
	$adreferendum = ($_REQUEST['adreferendum'] == 'S') ? 't' : 'f';
	$where .= " AND aval.aevid ".($adreferendum == "t" ? " is not null " : " is null ");
}
*/

if( $_REQUEST['dtini'] && $_REQUEST['dtfim'] )
{
	$dtini = formata_data_sql($_REQUEST['dtini']);
	$where .= " AND hd.htddata >= '{$dtini} 00:00:00' ";

	$dtfim = formata_data_sql($_REQUEST['dtfim']);
	$where .= " AND hd.htddata <= '{$dtfim} 23:59:59' ";
	
	$where .= " AND  est.esdid = 115"; // Aprovado pelo Comitê
}

$camposSQL = "

SELECT
 			DISTINCT
 				'<div align=\"center\">
                    <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: selecionarEvento('||ev.eveid||');\" title=\"selecionar Evento\"> 
					<!--img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"removerEvento('||ev.eveid||',\''||replace(ev.evetitulo,chr(34),'')||'\');\" title=\"excluir Evento\" /-->
                 </div>' as eveid,
				 CASE
				  WHEN aev.eveid IS NULL THEN ''
				  WHEN aev.eveid IS NOT NULL THEN '<center><a class=\"dcontexto\" id=\"anexo_'||ev.eveid||'\" onmouseover=\"return toolTypeAjax('||ev.eveid||');\"> <img src=\"/imagens/anexo.gif\" style=\"cursor: pointer\" onclick=\"selecionaEvento('||ev.eveid||');\"> </img><span><div id=\"span_'||ev.eveid||'\"></div></span></a></center>' 
				 END as doc,	
				 /*
				 CASE
				  WHEN aval.aevid IS NULL THEN ''
				  WHEN aval.aevid IS NOT NULL THEN '<img src=\"/imagens/check_p.gif\" border=0 title=\"Preenchida\">' 
				 END as aval,
				 */
				 CASE
				  WHEN ev.eveurgente != 't' THEN ''
				  WHEN ev.eveurgente = 't' THEN '<img src=\"/imagens/check_p.gif\" border=0 title=\"Preenchida\">' 
				 END as referendun,
				 ev.eveid AS ID,
				 ev.evenumeroprocesso,		 
			   '<div align=\"left\"> <a
						href=\"javascript:selecionarEvento('||ev.eveid||');\"
						title=\"selecionar Evento\"
				 >
					  ' ||replace(ev.evetitulo,chr(34),'')||'   
				 </a>
				 <br/> 
					<a href=\"#\" onclick=\"enviar_email('|| 
						case 
							when coalesce(u.usucpf, '') = '' then 'Não há Gestor'
							else u.usucpf
						end						
					||' ); return false;\" style=\"text-decoration: none;\">
						<span style=\"font-size: 9px; font-weight: normal; color: #808080;\" title=\"Clique para enviar e-mail.\">
							Responsável: '|| 
								case 
									when coalesce (ev.everespnome, '') = '' then 'Não há Responsável'
									else ev.everespnome
								end
							||' 
						</span>
					</a> </div>'	as evetitulo,

					to_char(ev.evedatainclusao,'YYYY'),
					
				uni.ungdsc,
				
				
	cast( to_char( ev.evedatainicio, 'dd-mm-YYYY' ) as date )as datainicio,
	cast( to_char( ev.evedatafim, 'dd-mm-YYYY' ) as date )as datafim,
				
				
				(m.mundescricao||' - '||ev.estuf)as local,
				ev.evecustoprevisto,
				est.esddsc,
				CASE
					  WHEN hd.htddata IS NULL THEN '<div align=\"center\"> -- </div>'
					  ELSE '<span style=\"display:none\">'|| hd.htddata || 
							 '</span> '	|| to_char( hd.htddata, 'DD-MM-YYYY HH24:MI:SS' )	 
				END AS datatramite,
				CASE
					WHEN evedatainclusao IS NULL THEN '<div align=\"center\"> -- </div>'
					ELSE '<span style=\"display:none\">'|| evedatainclusao || 
							 '</span> '	|| to_char( evedatainclusao, 'DD-MM-YYYY HH24:MI:SS' )
				END AS evedatainclusao
";

if( !$_REQUEST['pesqAvancada'] ){
	 
 $sqlAll = " 
 			$camposSQL
		    FROM
		   	 evento.evento as ev
		     --INNER JOIN evento.situacaoevento AS se ON se.sevid = ev.sevid
		     INNER JOIN territorios.municipio m ON m.muncod = ev.muncod 
					 
		     " . $from . "
		     LEFT JOIN seguranca.usuario AS u ON u.usucpf = ev.usucpf 
		     LEFT JOIN evento.anexoevento AS aev ON aev.eveid = ev.eveid
		     LEFT JOIN workflow.documento d ON d.docid = ev.docid and d.tpdid = 36
		     LEFT JOIN public.unidadegestora AS uni ON ev.ungcod = uni.ungcod
		     LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid and est.tpdid = 36
		     LEFT JOIN evento.avaliacaoevento aval ON aval.eveid = ev.eveid
		     LEFT JOIN ( select docid, max(htddata) as htddata from workflow.historicodocumento
						 where docid in (select docid from workflow.documento where tpdid = 36) 
						 group by docid
					   ) hd ON hd.docid = d.docid
		   WHERE
		   	 ev.evestatus = 'A'
		     $condicao 
		     $where 
		   ORDER BY ID DESC 
		   
			"; 
//dbg( $sqlAll ,1);
//$rs = $db->carregar( $sqlAll );
 
}elseif( $_REQUEST['pesqAvancada'] != '' ){
	if( $_REQUEST['tipoPesq'] == 'data'){
 
		$sqlAll = "
		$camposSQL
		   FROM
		   	 evento.evento as ev
		     INNER JOIN territorios.municipio m ON m.muncod = ev.muncod 
		     " . $from . "
		     LEFT JOIN seguranca.usuario AS u ON u.usucpf = ev.usucpf 
		     LEFT JOIN evento.anexoevento AS aev ON aev.eveid = ev.eveid
		     LEFT JOIN workflow.documento d ON d.docid = ev.docid and d.tpdid = 36
		     LEFT JOIN public.unidadegestora AS uni ON ev.ungcod = uni.ungcod
		     LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid and est.tpdid = 36
		     LEFT JOIN evento.avaliacaoevento aval ON aval.eveid = ev.eveid
		     LEFT JOIN ( select docid, max(htddata) as htddata from workflow.historicodocumento
						 where docid in (select docid from workflow.documento where tpdid = 36) 
						 group by docid
					   ) hd ON hd.docid = d.docid
		    WHERE ev.sevid = 2 
			AND (ev.evedatainicio IS NULL OR ev.evedatafim IS NULL)
			AND ev.evestatus = 'A'";
		
		unset($_REQUEST['pesqAvancada']);
		unset($_REQUEST['tipoPesq']);
		
	} elseif( $_REQUEST['tipoPesq'] == 'infra'){
 
		$sqlAll = "
			$camposSQL		    
		   FROM
		   	 evento.evento as ev
		   	 LEFT JOIN evento.itemevento AS i ON i.eveid = ev.eveid
		   	 INNER JOIN evento.situacaoevento AS se ON se.sevid = ev.sevid
		   	 INNER JOIN territorios.municipio m ON m.muncod = ev.muncod 
		     " . $from . "
		     LEFT JOIN seguranca.usuario AS u ON u.usucpf = ev.usucpf 
		     LEFT JOIN evento.anexoevento AS aev ON aev.eveid = ev.eveid
		     LEFT JOIN workflow.documento d ON d.docid = ev.docid and d.tpdid = 36
		     LEFT JOIN public.unidadegestora AS uni ON ev.ungcod = uni.ungcod
		     LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid and est.tpdid = 36
		     LEFT JOIN evento.avaliacaoevento aval ON aval.eveid = ev.eveid
		     LEFT JOIN ( select docid, max(htddata) as htddata from workflow.historicodocumento
						 where docid in (select docid from workflow.documento where tpdid = 36) 
						 group by docid
					   ) hd ON hd.docid = d.docid
		    WHERE ev.sevid = 2 
			AND i.ievid IS NULL
			AND ev.evestatus = 'A'";
		
		unset($_REQUEST['pesqAvancada']);
		unset($_REQUEST['tipoPesq']);
	} elseif( $_REQUEST['tipoPesq'] == 'realizados'){
 
		 	$sqlAll = "
		   	 $camposSQL
		   FROM
		   	 evento.evento as ev
		   	 INNER JOIN evento.situacaoevento AS se ON se.sevid = ev.sevid
		   	 INNER JOIN territorios.municipio m ON m.muncod = ev.muncod 
		     " . $from . "
		     LEFT JOIN seguranca.usuario AS u ON u.usucpf = ev.usucpf 
		     LEFT JOIN evento.anexoevento AS aev ON aev.eveid = ev.eveid
		     LEFT JOIN workflow.documento d ON d.docid = ev.docid and d.tpdid = 36
		     LEFT JOIN public.unidadegestora AS uni ON ev.ungcod = uni.ungcod
		     LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid and est.tpdid = 36
		     LEFT JOIN evento.avaliacaoevento aval ON aval.eveid = ev.eveid
		     LEFT JOIN ( select docid, max(htddata) as htddata from workflow.historicodocumento
						 where docid in (select docid from workflow.documento where tpdid = 36) 
						 group by docid
					   ) hd ON hd.docid = d.docid
		   WHERE
		   	 ev.evestatus = 'A'
		     $condicao 
		     $where 
		   	 "; 
		   	 
		unset($_REQUEST['pesqAvancada']);
		unset($_REQUEST['tipoPesq']);
	}
} 
//dbg($sqlAll,1);
//$rsDadosEvento = $db->carregar( $sqlAll );
?>
<head>
   
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    </head>
    <script type="text/javascript">
	function removerFiltro(){
		document.formulario.filtro.value = "";
		document.formulario.estuf.selectedIndex = 0;
		document.formulario.submit();
	}
</script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="POST" name="formulario">
					<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
					<div style="float: left;">
						
						<table border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom">
									Título do Evento:
									<br/> 
									<?= campo_texto( 'filtro_evetitulo', 'N', 'S', '', 50, 200, '', '' ); ?>
								</td><!--
								<td valign="bottom" >
									Período:
									<br/>
									<?= campo_data( 'filtro_datai','N', 'S', '', 'S' ); ?> à  
                					<?= campo_data( 'filtro_dataf','N', 'S', '', 'S' ); ?>
								</td>							
								--><td>
									SItuação:
									<br>
									<?php
										$filtro_situacao = $_POST['filtro_situacao'];
										$sql = sprintf("SELECT
														 esdid AS codigo,
														 esddsc AS descricao
														FROM
														 workflow.estadodocumento et
														 INNER JOIN workflow.tipodocumento tp ON tp.tpdid = et.tpdid AND
														 	sisid = ".$_SESSION['sisid']." AND et.esdstatus = 'A'
														 	ORDER BY
														 esdordem");
										$db->monta_combo( "filtro_situacao", $sql, 'S', 'Selecione...', '', '', '', '', '', 'filtro_situacao' );
									?>				
								</td>
										<td valign="bottom" colspan="2-">
									Unidade Demandante:
									<br/>
									<?php
									$filtro_unidade = $_POST['filtro_unidade'];
									$sql = "SELECT 
					                            ungcod AS codigo, 
					                            ungdsc AS descricao
					                        FROM
					                            public.unidadegestora";
									$db->monta_combo( "filtro_unidade", $sql, 'S', 'Selecione...', '', '', '', '', '', 'filtro_unidade' );
									?>
								</td>
							</tr>
							<tr>
							

								</td>
								 <td valign="bottom">
									Ano de Inclusão
									<br/>
									<?php
									$inclusao = $_POST['filtro_ano'];
									$sql = "select distinct (to_char(evedatainclusao::date,'YYYY')) as codigo,
									(to_char(evedatainclusao::date,'YYYY')) as descricao
									from evento.evento
									where evedatainclusao is not null
									order by descricao";
									$db->monta_combo( "filtro_ano", $sql, 'S', 'Selecione...', '', '', '', '', '', 'filtro_ano' );
									?>
								</td>		
									
							
								<td>
									Número do evento:
									<br/> 
									<?= campo_texto( 'filtro_eveid', 'N', 'S', '', 10, 20, '##########', '' ); ?>
								</td>
								
								<td>
									Número do processo:
									<br/> 
									<?= campo_texto( 'filtro_evenumeroprocesso', 'N', 'S', '', 25, 20, '#####.######/####-##', '' ); ?>
								</td>
								
								</td>
								
						
								 <td valign="bottom">
									Estado
									<br/>
									<?php
									$estuf = $_REQUEST['estuf'];
									$sql = "select
											 e.estuf as codigo, e.estdescricao as descricao
											from
											 territorios.estado e 
											order by
											 e.estdescricao asc";
									$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', 'filtraTipo', '' );
									?>
								</td>
								
								<td align="left" valign="bottom" id="municipio" style="visibility:<?= $_REQUEST['estuf'] ? 'visible' : 'hidden'  ?>;">
									Município
									<br/>
									<?
									if ($_REQUEST['estuf']) {
										$muncod = $_REQUEST['muncod'];
										$sql = "select
												 muncod as codigo, 
												 mundescricao as descricao 
												from
												 territorios.municipio
												where
												 estuf = '".$_REQUEST['estuf']."' 
												order by
												 mundescricao asc";
										$db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '' );
									}
									?>	
								</td>									
							</tr>
							<tr>
								<td colspan="3">
									AD Referendum:
									<br />
									<input type="radio" name="eveurgente" value="T" <?=((!$_REQUEST['eveurgente'] || $_REQUEST['eveurgente'] == 'T') ? 'checked="checked"' : '')?> />Todos
									&nbsp;
									<input type="radio" name="eveurgente" value="S" <?=(($_REQUEST['eveurgente'] == 'S') ? 'checked="checked"' : '')?> />Sim
									&nbsp;
									<input type="radio" name="eveurgente" value="N" <?=(($_REQUEST['eveurgente'] == 'N') ? 'checked="checked"' : '')?> />Não
								</td>
							</tr>							
							<tr>
				                <td>
					                Período de Aprovação:  
					                <br />
					                <?= campo_data( 'dtini','N', 'S', '', 'S' ); ?> à  
					                <?= campo_data( 'dtfim','N', 'S', '', 'S' ); ?> 
				                </td> 
				            </tr>
							
					<?
					if( ( ( in_array(PERFIL_SEE, $perfis ) ) ||in_array(PERFIL_SAA, $perfis ) ) || ( in_array(PERFIL_SUPER_USUARIO, $perfis ) ) ){
						?>	
						<tr>
							<td  id="td_pesq_avancada" colspan="3">
								 
							</td>
						</tr>
						<tr >
							<td colspan="3" id="pesq_avancada">
								<br>
								<a href="javascript:void(0);" onclick="mostraPesquisaAvancada();">[+] Pesquisa Avançada</a>
							</td>
						</tr>
						<script> 
							if(document.selection)
								document.getElementById("pesq_avancada").style.display = "block";
							else
								document.getElementById("pesq_avancada").style.display = "table-row";
						</script>
					<? } ?>
						</table>
						<div align="" style="float: left;">
						<br>
							<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
						</div>	
					</div>	
				</form>
			</td>
		</tr>
	</tbody>
</table>
<br>
<? if( !temPerfilEmpresa() ){ ?> 
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:none;">
		<tr>
			<td style="padding: 10px;">
				<span style="cursor: pointer" onclick="cadastrarEvento();" title="novo Evento">
					<img align="absmiddle" src="/imagens/gif_inclui.gif"/> Cadastrar Evento
				</span>
			</td>
		</tr>
	</table>
<? } ?>
<?php
	$cabecalho = array("&nbsp;&nbsp;&nbsp;&nbsp;Ação", "Anexo","AD","Cód", "<div style='width:120px;'>Nº Processo</div>", "Evento", "Ano","Unidade Gestora","Início do Evento", "Fim do Evento", "Local", "Custo", "Situação", "Data da Situação", "Data da Inclusão" ); 
	// pre( $sqlAll,1 );
	$db->executar('SET datestyle TO POSTGRES, DMY');
	include_once(APPRAIZ.'includes/classes/MontaListaAjax.class.inc');
	$db->monta_lista( trim($sqlAll), $cabecalho, 25, 10, 'N', '', '');
?>

<script type="text/javascript">
	
function removerEvento( id, nome ) {
 
	if ( confirm( 'Deseja excluir o Evento \"\' '+ nome +' \'\"?' ) ) {
		window.location.href = '?modulo=principal/inicioEvento&acao=A&remover='+id;
	}
}

function selecionarEvento( eveid )
{  
	window.location.href = '?modulo=inicio&acao=C&selecionar='+eveid; 

}

function cadastrarEvento(){ 
	var req = new Ajax.Request('?modulo=principal/cadEvento&acao=A', {
				        method:     'POST',
				        parameters: 'ajaxunsetsession=1',							         
				        onComplete: function (res) { 
							window.location.href = '?modulo=principal/cadEvento&acao=A';
						}
	});  
}

function enviar_email( cpf ){
	var nome_janela = 'janela_enviar_emai_' + cpf;
	window.open(
		'/geral/envia_email.php?cpf=' + cpf,
		nome_janela,
		'width=650,height=557,scrollbars=yes,scrolling=yes,resizebled=yes'
	);
}

function validaForm()
{ 
  	document.formulario.submit(); 
}

function mostraPesquisaAvancada() {
	var td_pesq_avancada = document.getElementById('td_pesq_avancada');
	var pesq_avancada 	 = document.getElementById('pesq_avancada');
	var contentTdPesquisa = "<br><a href=\"javascript: pesquisaByFiltro('data');\">- Eventos cadastrados sem uma data</a><br><a href=\"javascript: pesquisaByFiltro('infra');\">- Eventos cadastrados sem Infra-Estrutura</a><br><a href=\"javascript: pesquisaByFiltro('realizados');\">- Eventos já realizados</a><br><br>";
	var contentLink       = "<br><br><a href=\"javascript:void(0);\" onclick=\"escondePesquisaAvancada();\">[-] Pesquisa Avançada</a><br>";
	td_pesq_avancada.innerHTML = contentTdPesquisa;
	pesq_avancada.innerHTML = contentLink;
}

function escondePesquisaAvancada() {
	var td_pesq_avancada = document.getElementById('td_pesq_avancada');
	var pesq_avancada    = document.getElementById('pesq_avancada');
	var contentTdPesquisa = "";
	var contentLink       = "<a href=\"javascript:void(0);\" onclick=\"mostraPesquisaAvancada();\">[+] Pesquisa Avançada</a>";
	td_pesq_avancada.innerHTML = contentTdPesquisa;
	pesq_avancada.innerHTML = contentLink;
	window.location.href="?modulo=inicio&acao=C";
}

function pesquisaByFiltro( tipo ){
	 if(tipo){
	 	window.location.href="?modulo=inicio&acao=C&pesqAvancada=t&tipoPesq="+tipo;
	 }
}

function selecionaEvento( id ){

	var req = new Ajax.Request('evento.php?modulo=inicio&acao=C', {
					        method:     'post',
					        parameters: '&ajaxsession=' + id,							         
					        onComplete: function (res) {	 
								window.location.href = '?modulo=principal/cadEventoAnexo&acao=A';
							}
		});
}
 

function filtraTipo(estuf)
{
	if( !estuf ){
		return false;
	}
	td 	   = document.getElementById('municipio');
	select = document.getElementsByName('muncod')[0];
	
	if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	
	
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('evento.php?modulo=inicio&acao=C', {
							        method:     'post',
							        parameters: '&ajaxestuf=' + estuf,
							        onComplete: function (res)
							        {			 
							        	var inner = 'Município<br/>';
										td.innerHTML = inner+res.responseText;
										td.style.visibility = 'visible';
							        }
							  });
    
}

function toolTypeAjax(id){
	if(!id){
		return false;
	}
	var anexo = document.getElementById('anexo_'+id);
	var req = new Ajax.Request('evento.php?modulo=inicio&acao=C', {
							        method:     'post',
							        parameters: '&toolTypeAjax='+id,
							        onComplete: function (res)
							        {		 //alert(res.responseText);
							        	//anexo.setAttribute('onMouseOver', escape(res.responseText));
							        	$('span_'+id).innerHTML = res.responseText;
							        	//alert($('span').innerHTML);
							        	//anexo.span = res.responseText;
							        	
							        }
							  });
}

</script>

<?php 
} // fim if($manutencao != 'S'){
?>