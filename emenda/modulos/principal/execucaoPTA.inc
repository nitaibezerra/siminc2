<?php
require_once APPRAIZ . "emenda/classes/WSEmpenho.class.inc";
require_once APPRAIZ . "emenda/classes/ExecucaoFinanceira.class.inc";

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];
$ptrid = $_SESSION["emenda"]["ptridAnalise"];

$obPTA = new PTA();

$obPTA->validaSessionPTA( $ptrid );
$federal = $obPTA->buscaEmendaDescentraliza( $ptrid );

if( $federal == 'S' ){
	$label = 'Nota de Crédito';
} else {
	$label = 'Nº Empenho Original';
}

unset( $_SESSION['emenda']['msgErro'] );
$_SESSION['emenda']['msgErro'] = array();

include_once APPRAIZ . 'includes/workflow.php';
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );
montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);

// Cria o título da tela
monta_titulo( 'Execução do PTA...', "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");

echo cabecalhoPlanoTrabalho($ptrid, true);

montaVisualizarPTA( $ptrid, false, $boReformulacao );

$ptrnumprocessoempenho = $db->pegaUm("SELECT ptrnumprocessoempenho FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");

$permissao = verificaPermissaoPerfil( 'analise', 'boolean' );

$sql = "SELECT distinct em.empnumeroprocesso||'&nbsp;' as processo, em.empcodigoptres||'&nbsp;' as ptres, em.empcodigopi, 
			em.empcentrogestaosolic||'&nbsp;' as centrogestao, em.empnumero, em.empsituacao,
		    e.eobvalorempenho, e.eobpercentualemp
		FROM par.subacaoemendapta sep
			inner join par.subacaodetalhe sd on sd.sbdid = sep.sbdid
			inner join par.subacao s on s.sbaid = sd.sbaid
			inner join par.empenhosubacao e on e.sbaid = s.sbaid and eobstatus = 'A' -- se tiver nesta tabela teve empenho
			inner join par.empenho em on em.empid = e.empid and empstatus = 'A'
			inner join emenda.ptemendadetalheentidade ped on ped.ptrid = sep.ptrid
			inner join emenda.v_emendadetalheentidade ved on ved.edeid = ped.edeid
		WHERE 
			sep.ptrid = $ptrid
			and sep.sepstatus = 'A'
		    and ved.edestatus = 'A'";
		    
$arrDadosEmpPar = $db->carregar( $sql );
$arrDadosEmpPar = $arrDadosEmpPar ? $arrDadosEmpPar : array();

$boContaCorrente = $db->pegaUm("select count(c.cocid) from emenda.contacorrente c where ptrid = $ptrid");

?>
<html>
<script src="../includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
<link rel="stylesheet" type="text/css" href="css/emenda.css"/>
<body>
	<?php abrePTAconsolidado($ptrid); ?>
	<?if( $arrDadosEmpPar[0] ){ ?>
		<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<th colspan="2" class="subtitulocentro">Empenho PAR</th>
		</tr>
		<tr>
			<th class="subtitulocentro" colspan="2" style="color: red;">Este plano de trabalho foi empenhado como transferência direta no sistema PAR.</th>
		</tr>
		<tr>
			<th colspan="2" class="subtitulocentro">Dados do empenho PAR</th>
		</tr>
		</table>
	<? 
		$cabecalho = array('Número do Processo', 'Número do PTRES', 'Número do PI', 'Centro de Gestão', 'Empenho', 'Situação', 'Valor', '% Empenhado');
		$db->monta_lista_array($arrDadosEmpPar, $cabecalho, 5000, 20, 'S', 'center', '');
		
	}else{ ?>
		<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
			<tr>
				<td colspan="2" class="subtitulocentro">Dados para o empenho</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 19%;"><b>Número do Processo:</b></td>
				<td><?php
					$boSolicitado = $db->pegaUm("select 
													case when count(exfnumsolempenho) = 0 then 'S'
													else 'N'
													end as solicitado
												 from emenda.execucaofinanceira where exfnumsolempenho is not null and exfnumsolempenho <> '' and exfstatus = 'A' and ptrid = $ptrid");
					$ptrnumprocessoempenho = (($ptrnumprocessoempenho)?substr($ptrnumprocessoempenho,0,5)."." .substr($ptrnumprocessoempenho,5,6)."/".substr($ptrnumprocessoempenho,11,4)."-".substr($ptrnumprocessoempenho,15,2):"");
					echo campo_texto( 'ptrnumprocessoempenho', 'S', "$boSolicitado", '', 52, 50, '#####.######/####-##', '', '', '', 0, 'id="ptrnumprocessoempenho"' );			
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 19%;"><b>Verificado no SIAFI:</b></td>
				<td><?php echo "<input type=\"checkbox\" name=\"verificadoSiafi\" id=\"verificadoSiafi\" value=\"1\">"; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width: 19%;"><b>Verificado no CADIN:</b></td>
				<td><?php echo "<input type=\"checkbox\" name=\"verificadoCadin\" id=\"verificadoCadin\" value=\"1\">"; ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="middle"><b>Situação Habilitação:</b></td>
				<td>
				<?php
					$obHabilita = new Habilita();
					$cnpj = $obHabilita->pegaCnpj($ptrid);
					echo $obHabilita->consultaHabilita($cnpj,true);
				?>
				</td>
			</tr><!--
			<tr>
				<td class="SubTituloDireita" valign="middle"><b>Cronograma Desembolso:</b></td>
				<td >
					<a href="#" onclick="popUpIncluirCronogramaDesenbolso();" style="cursor:pointer">
						<label id="labelCronograma">Clique aqui para preencher o Cronograma Desembolso do Convenio</label>
					</a>
					<input type="hidden" name="boCronograma" id="boCronograma" value="<?=($prdid > 0 ? 'true' : 'false'); ?>" />
				</td>
			</tr>
		-->
<?	if( $permissao ){ ?>
			<tr>
				<td class="SubTituloDireita" valign="middle"><b>Solicitar Conta Corrente:</b></td>
				<td colspan="3">
					<input type="radio" value="S" id="solicitarconta_sim" name="solicitarconta" checked="checked" /> Sim
					<input type="radio" value="N" id="solicitarconta_nao" name="solicitarconta" /> Não
				</td>
			</tr>
<?php	if( $boContaCorrente > 0 ){ ?>
			<tr>
				<td valign="middle" class="subtitulodireita" style="width: 25%"><b>Deseja atualizar dados da Conta Corrente:</b></td>
				<td><input type="button" id="btnCC" name="btnCC" value="Clique Aqui" style="cursor: pointer;" onclick="ConsultarAndamentocc(<?=$ptrid; ?>);"></td>
			</tr>
<?php 	} else {?>
			<tr>
				<td valign="middle" class="subtitulodireita" style="width: 25%"><b>Dados Conta Corrente:</b></td>
				<td><input type="button" id="btnCC" name="btnCC" value="Solicitar Conta Corrente" style="cursor: pointer;" onclick="solicitarContaCorrente(<?=$ptrid; ?>);"></td>
			</tr>
<?php	}?>
<?php } ?>
		</table>
		<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
			<tr>
				<td>
		<?
		
		if( $_SESSION['exercicio'] > 2014 )	$filtroImp = ' and ei.edeid = vede.edeid';
		
		$sql = "SELECT distinct
					vede.acaid, 
				    fup.fupfuncionalprogramatica, 
				    vede.emecod,
				    vede.emeid, 
				    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome, 
				    vede.gndcod, 
					vede.mapcod, 
					vede.foncod,
					vede.emetipo,
					(CASE WHEN vede.emetipo = 'E' THEN 'Emenda'
				       ELSE 'Complemento' END) as tipoemenda,
				       exf.tpeid,
					--per.pedid, 
					tpe.tpedsc,
					--per.perid,
					--sum(per.pervalor) as pervalor,
					((select sum(pervalor) from emenda.ptiniesprecurso where pedid = pte.pteid and pedid = ped.pedid) - coalesce(ei.edivalor, 0)) as pervalor,
					pli.plicod, 
					pli.plititulo, 
					pt.ptres as pliptres, 
					esf.esfcod, 
					esf.esfdsc,
					(CASE WHEN vede.mapcod = '90' THEN '0 - federal'
				    	  WHEN vede.mapcod = '30' THEN '1 - estadual'
				          WHEN vede.mapcod = '40' THEN '2 - Municipal'
				          WHEN (vede.mapcod = '50' or vede.mapcod = '60') THEN '3 - Particular'
				     ELSE '' END) as esferaadm,
	                 (CASE WHEN vede.gndcod = '3' THEN '3.3.'||vede.mapcod||'.41'
	                 	   WHEN vede.gndcod = '4' THEN '4.4.'||vede.mapcod||'.42'
	                  END) as naturezaDesp,
	                 exf.exfid,
	                 exf.exfcodfontesiafi,
	                 exf.exfnumsolempenho,
	                 (exf.exfvalor - coalesce(ei.edivalor, 0)) as exfvalor,
	                 case when vede.emdliberado = 'N' then 'Não' else 'Sim' end as casacivil,
	                 ptr.ptrpropostasiconv
				FROM 
					emenda.planotrabalho ptr
					inner join emenda.ptiniciativa pti on pti.ptrid = ptr.ptrid
					inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
					inner join emenda.ptemendadetalheentidade ped on ped.ptrid = pti.ptrid
					inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
					inner join emenda.autor aut on aut.autid = vede.autid
					inner join emenda.v_funcionalprogramatica fup on fup.acaid = vede.acaid
					inner join emenda.execucaofinanceira exf on pti.ptrid = exf.ptrid and ped.pedid = exf.pedid  and exf.exfstatus = 'A'
				    inner join emenda.tipoensino tpe on exf.tpeid = tpe.tpeid
					inner join monitora.pi_planointerno pli on pli.plicod = exf.plicod and /*pli.pliano >= '".$_SESSION['exercicio']."'*/ cast(pli.pliano as integer) = vede.emeano and pli.plistatus = 'A'
					--inner join monitora.pi_planointernoptres plpt on pli.pliid = plpt.pliid
					left join monitora.ptres pt on pt.ptres = exf.ptres --and pt.acaid = fup.acaid 
					inner join public.esfera esf on fup.esfcod = esf.esfcod 
					left join emenda.emendadetalheimpositivo ei on ei.emdid = vede.emdid and ei.edistatus = 'A'  $filtroImp
				where 
					pti.ptrid = $ptrid and exfidpai is null
				ORDER BY 
					--per.pedid, 
					tpe.tpedsc, 
					vede.emecod, 
					vede.emeid, 
					vede.gndcod, 
					vede.mapcod, 
					vede.foncod, 
					fup.fupfuncionalprogramatica, 
					vede.acaid";
		
		montaTabelaSolicitacaoEmpenho($sql, $ptrid, $_GET['boAlterarEmpenho'], $boReformulacao);
		
		$acaoAlterarHabilitada = "'<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterarEmpenho( \''||ef.exfid||'\');\" border=0 alt=\"Ir\" title=\"Alterar\"> '";
		$acaoAlterarNaoHabilitada = "'<center><img src=\"/imagens/alterar_01.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Alterar\"> '";
		
		$acaoConsultarHabilitada = "'<img src=\"/imagens/refresh2.gif\" style=\"cursor: pointer\" onclick=\"consultarEmpenho( \''||pt.ptrid||'\', \''||ef.exfid||'\' );\" border=0 alt=\"Ir\" title=\"Consultar Empenho\"> '";
		$acaoConsultarNaoHabilitada = "'<img src=\"/imagens/refresh2_01.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Consultar Empenho\"> '";
		
		$acaoConsultarExecucao = "'<img src=\"/imagens/consultar.gif\" style=\"cursor: pointer\" onclick=\"consultarExecucao( \''||pt.ptrid||'\', \''||ef.exfid||'\');\" border=0 alt=\"Ir\" title=\"Consultar Histórico Empenho\"> '";
		
		$acaoAnulacaoTotalHabilitada = "'<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"alteraValor(\''||pt.ptrid||'\', \''||ef.exfid||'\', 1, \''||ef.exfvalor||'\');\" border=0 alt=\"Ir\" title=\"Anulação Total do Empenho\"></center>'";
		$acaoAnulacaoTotalNaoHabilitada = "'<img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Anulação Total do Empenho\"></center>'";
		
		$ptrcod = pegaNumPTA( $ptrid );
		
		if( !$permissao ){
			$acaoAlterarHabilitada = $acaoAlterarNaoHabilitada;
			$acaoConsultarHabilitada = $acaoAlterarNaoHabilitada;
			$acaoConsultarHabilitada = $acaoConsultarNaoHabilitada;
			$acaoAnulacaoTotalHabilitada = $acaoAnulacaoTotalNaoHabilitada;
		}
		
		/*if( !empty($ptrcod) ) $filtro = "pti.ptrid in (select ptrid from emenda.planotrabalho where ptrcod = ".$ptrcod.")";
		else*/ $filtro = "pti.ptrid = $ptrid";
		
		$sql  = "select distinct
					(CASE WHEN ef.exfnumempenhooriginal <> '' 
							AND ef.exfnumempenhooriginal is not null 
							AND ((SELECT count(1) as count FROM emenda.execucaofinanceira WHERE exfidpai = ef.exfid and exfstatus = 'A') = 0) THEN $acaoAlterarHabilitada
				       ELSE $acaoAlterarNaoHabilitada END) 
					||
					(CASE WHEN ((SELECT count(1) as count FROM emenda.execucaofinanceira WHERE exfidpai = ef.exfid and exfstatus = 'A') = 0) THEN $acaoConsultarHabilitada
				       ELSE $acaoConsultarNaoHabilitada END) 
				    ||
						$acaoConsultarExecucao
					||
					(CASE WHEN ef.exfnumempenhooriginal <> '' 
							AND ef.exfnumempenhooriginal is not null 
							AND ((SELECT count(1) as count FROM emenda.execucaofinanceira WHERE exfidpai = ef.exfid and exfstatus = 'A') = 0)
							AND	ef.exfvalor > 0 THEN $acaoAnulacaoTotalHabilitada
				       ELSE $acaoAnulacaoTotalNaoHabilitada END)					
				      as acao, 
					fup.fupfuncionalprogramatica,
					vede.emecod,
					case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
					ef.exfvalor as valor,
					CASE WHEN exfespecieempenho = '01' THEN 'Original'
				         WHEN exfespecieempenho = '02' THEN 'Reforço'
				         WHEN exfespecieempenho = '03' THEN 'Anulação'
				         WHEN exfespecieempenho = '13' THEN 'Anulação'
				    END AS especieempenho,
					ef.exfnumsolempenho,
					ef.exfnumempenhooriginal,
					to_char(exfdatainclusao, 'DD/MM/YYYY HH24:mi:ss') as exfdatainclusao,
					se.semcod || ' - ' ||se.semdesc as situacao,
					(SELECT  
	                    CASE WHEN  count(1) = 1 THEN 'Pendente'
	                    WHEN  ef.exfstatus = 'A' THEN 'Ativo'
	                    WHEN  ef.exfstatus = 'I' THEN 'Inativo'
	                    END as status 
	                  FROM emenda.execucaofinanceira  WHERE exfidpai = ef.exfid and exfstatus = 'A')  as status,
	                (CASE WHEN pt.ptridpai is null THEN
						'Original'
					ELSE
						'Reformulação'
					END) as tipoempenho,
					pt.ptrid
				from emenda.planotrabalho pt
					inner join emenda.ptiniciativa pti on pti.ptrid = pt.ptrid
					inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
					inner join emenda.ptiniesprecurso per on per.pteid = pte.pteid
					inner join emenda.ptemendadetalheentidade ped on ped.pedid = per.pedid
					inner join emenda.tipoensino tpe on pti.tpeid = tpe.tpeid
					inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
					inner join emenda.autor aut on aut.autid = vede.autid
					inner join emenda.v_funcionalprogramatica fup on fup.acaid = vede.acaid
					inner join emenda.execucaofinanceira ef on pti.ptrid = ef.ptrid and ped.pedid = ef.pedid and ef.exfstatus = 'A'
					inner join emenda.situacaoempenho se on ef.semid = se.semid
					inner join monitora.pi_planointerno pli on pli.plicod = ef.plicod
					inner join monitora.pi_planointernoptres plpt on pli.pliid = plpt.pliid
					inner join monitora.ptres ptr on ptr.ptrid = plpt.ptrid
					inner join public.esfera esf on fup.esfcod = esf.esfcod
				where 
					$filtro
				group by
					pt.ptrid,
					ef.exfid,
					fup.fupfuncionalprogramatica,
					vede.emecod,
					aut.autnome,
					vede.emerelator,
					ef.exfvalor,
					ef.exfnumsolempenho,
					especieempenho,
					ef.exfnumempenhooriginal,
					exfdatainclusao,
					situacao,
					pervalor,
					ef.exfstatus,
					status,
					pt.ptridpai
				order by exfdatainclusao";
						
		$arDadosEmpenho = $db->carregar( $sql );
		$arDadosEmpenho = ( $arDadosEmpenho ? $arDadosEmpenho : array() );
	
		print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
		print '<tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" class="subtitulocentro" >Lista Empenhados</td></tr></table>';
		 
		$cabecalho 	    = array("Ação","Funcional", "Código", "Autor", "Valor", "Espécie Empenho", "Num Sol. Empenho", $label, "Data Inclusão", "Situação", "Status"); //, "Tipo Empenho"
		$tamanho		= array( '7%', '7%', '7%', '12%', '7%', '10%', '15%', '15%', '10%', '15%', '15%' );															
		$alinhamento	= array( 'center', 'left', 'right', 'center', 'right', 'center', 'right', 'center', 'center', 'center', 'center' );													
		//$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
		
		$ptaPai = pegaPaiPTA( $ptrid );
		
		foreach ($arDadosEmpenho as $key => $v) {
			
			$acoes = $v['acao'];
			if( !empty($ptaPai) && $v["tipoempenho"] == 'Original' ){
				$acoes = "<center><img src=\"/imagens/alterar_01.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Alterar\">
						  <img src=\"/imagens/refresh2_01.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Consultar Empenho\">
						  <img src=\"/imagens/consultar.gif\" style=\"cursor: pointer\" onclick=\"consultarExecucao( '".$v['ptrid']."', '".$v['exfid']."');\" border=0 alt=\"Ir\" title=\"Consultar Histórico Empenho\">
						  <img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Anulação Total do Empenho\"></center>";
			}
			
			$dados_array[$key] = array("acoes" => $acoes,
									   "Funcional" => $v['fupfuncionalprogramatica'],
									   "Código" => $v['emecod'].'',
									   "Autor" => $v['autnome'],
									   "Valor" => $v['valor'],
									   "Espécie Empenho" => $v['especieempenho'],
									   "Num Sol. Empenho" => $v['exfnumsolempenho'],
									   $label => $v['exfnumempenhooriginal'],
									   "Data Inclusão" => $v['exfdatainclusao'],
									   "Situação" => $v['situacao'],
									   "Status" => $v['status']
									   
									   );
									   //"Tipo Empenho" => $v['tipoempenho']
		}
		$db->monta_lista_array($dados_array, $cabecalho, 5000, 20, '', 'center', '');
	}
	?>
	</td>
	<td>
		<?php 
		$docid = criarDocumento( $ptrid );
		wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid, 'tipo' => "" ) );
		?>
	</td>
	</tr></table>
	<div id="erro"></div>
</body>
<!-- LINKS MODAL -->
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<?php
	$arFluxo = verificaFluxo( $_SESSION["emenda"]["ptridAnalise"] );
	
	if( $arFluxo ){
		$fluxo = implode(', ', $arFluxo);
	}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

	jQuery.noConflict();
	jQuery(document).ready(function(){
		jQuery.each([ <?=$fluxo ?> ], function( index, value ) {
			jQuery('#td_acao_'+value).css('display', 'none');
		});
	});
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url){
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(560,345);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function closeMessage(){
		messageObj.close();	
	}

	function ConsultarAndamentocc(ptrid){
		displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_consulta=consultarandamentocc&ptrid='+ptrid+'&tipo=e');
	}
	
	function solicitarContaCorrente(ptrid){
		displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_consulta=solicitarcontacorrente&ptrid='+ptrid+'&tipo=e');
	}

	function solicitarEmpenhoPTA(ptrid, exfid, tipo){
		var form = $('formExecPTA');
		var msg  = '';
			
		var ptrnumprocessoempenho = $('ptrnumprocessoempenho');

		
		if(ptrnumprocessoempenho.value.length != 20){
			msg += 'É necessário digitar o número do processo de empenho\n';
		}
		
		var verificadoCadin = $('verificadoCadin');
		if(verificadoCadin.checked == true){
			verificadoCadin = 'true'; 
		} else {
			verificadoCadin = 'false';
		}
		
		var verificadoSiafi = $('verificadoSiafi');
		if(verificadoSiafi.checked == true){
			verificadoSiafi = 'true'; 
		} else {
			verificadoSiafi = 'false';
		}
		
		
		if(msg){
			alert(msg);
			return false;			
		}
		
		for(i=0; i<form.length; i++){
			if( form.elements[i].type == 'select-one' ){
				if(form.elements[i].value == ""){
					//alert('Existe(m) campo(s) em branco!');
					$(form.elements[i].id).focus();
					//return false;
				}	
			}
		}
		var solicitarconta = 'S';
		if($('solicitarconta_sim').checked) {
			solicitarconta = 'S';
		} else {
			solicitarconta = 'N'
		}
		if( $('fontesiafi_'+exfid).value == "" ){
			alert('O campo Fonte SIAFI e obrigatório');
			$('fontesiafi_'+exfid).focus();
			return false;
		} else {

			var url = "ajax_emenda.php";
			var pars = 'execucaoPTA=salvar&ptrid='+ptrid+'&ptrnumprocessoempenho='+ptrnumprocessoempenho.value+'&'+ $('formExecPTA').serialize()+'&exfidexecucao='+exfid;
			var myAjax = new Ajax.Request(
					url, 
					{
						method: 'post', 
						parameters: pars, 
						asynchronous: false,
						onComplete: function(r)
						{
							if( trim(r.responseText) == '1' && ptrid){
								//window.open('emenda.php?modulo=principal/popUpSenhaFNDE&acao=A&especie_empenho=original&ptrid='+ptrid, 'Senha FNDE', 'scrollbars=yes,height=150,width=500,status=no,toolbar=no,menubar=no,location=no');
								displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_empenho='+tipo+'&ptrid='+ptrid+'&verificadoSiafi='+verificadoSiafi+'&verificadoCadin='+verificadoCadin+'&exfidexecucao='+exfid+'&solicitarconta='+solicitarconta);
							}else{
								document.getElementById('divDebug').innerHTML = r.responseText;
								alert('Falha na operação.');
							}
						}
					});	
		}
	}

	function alterarEmpenho(exfid){
		window.location.href = "/emenda/emenda.php?modulo=principal/execucaoPTA&acao=A&boAlterarEmpenho=1";			
	}

	function alteraValor(ptrid, exfid, anulacaoTotal, valor){
		var verificadoCadin = $('verificadoCadin');
		if(verificadoCadin.checked == true){
			verificadoCadin = 'true'; 
		} else {
			verificadoCadin = 'false';
		}
		
		var verificadoSiafi = $('verificadoSiafi');
		if(verificadoSiafi.checked == true){
			verificadoSiafi = 'true'; 
		} else {
			verificadoSiafi = 'false';
		}
		
		if(anulacaoTotal && valor){
			if(!confirm('Deseja realmente excluir o empenho totalmente?')){
				return false;
			}
			var valorOriginal = valor;
			var valorAtual    = valorOriginal;
		} else {
			var valorOriginal = $('pervalorOriginal_'+exfid).value;
			var valorAtual    = $('valor_'+exfid).value;
		}
		
		var boAnulacaoTotal = false;
		if(valorOriginal != '' && valorAtual != ''){
			if(valorAtual != valorOriginal){
				if(valorAtual > valorOriginal){
					var valorAtualLimpo    = mascaraglobal( "[###]", valorAtual );
		            var valorOriginalLimpo = mascaraglobal( "[###]", valorOriginal );
		            var novoValor 		   = mascaraglobal( "[###]###.##", parseInt( valorAtualLimpo ) - parseInt( valorOriginalLimpo ) );
					var especie_empenho = 'reforco';
				} else {
					var valorAtualLimpo    = mascaraglobal( "[###]", valorAtual );
		            var valorOriginalLimpo = mascaraglobal( "[###]", valorOriginal );
		            var novoValor          = mascaraglobal( "[###]###.##", parseInt( valorOriginalLimpo ) - parseInt( valorAtualLimpo ) );
					var especie_empenho = 'anulacao';
				}
			} else {
				// Anulação Total
				var valorOriginalLimpo = mascaraglobal( "[###]", valorOriginal );
				var novoValor       = mascaraglobal( "[###]###.##", valorOriginalLimpo );
				var especie_empenho = 'anulacao';
				var boAnulacaoTotal = true;
			}

			if(ptrid && exfid){
				displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_empenho='+especie_empenho+'&ptrid='+ptrid+'&exfid='+exfid+'&novoValor='+novoValor+'&boAnulacaoTotal='+boAnulacaoTotal+'&verificadoSiafi='+verificadoSiafi+'&verificadoCadin='+verificadoCadin);
			} else {
				alert('Operação Alterar o valor falhou.');
				return false;
			}
		}
		
	}

	function consultarEmpenho(ptrid,exfid){
		if(ptrid && exfid){
			var solicitarconta = 'S';
			if($('solicitarconta_sim').checked) {
				solicitarconta = 'S';
			} else {
				solicitarconta = 'N'
			}
			displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_empenho=consultaEmpenho&ptrid='+ptrid+'&exfid='+exfid+'&solicitarconta='+solicitarconta);
		} else {
			alert('Operação Consulta Situação Empenho falhou.');
			return false;
		}

	}
	
	function consultarExecucao(ptrid, exfid){
		if(ptrid){
			window.open('emenda.php?modulo=principal/popupConsultaExecucaoPTA&acao=A&ptrid='+ptrid+'&exfid='+exfid, 'Consulta Execução PTA', 'scrollbars=yes,height=800,width=1200,status=no,toolbar=no,menubar=no,location=no');
		} else {
			alert('Operação Consulta Execução falhou.');
			return false;
		}

	}

</script>
</html>
<div id="divDebug"></div>