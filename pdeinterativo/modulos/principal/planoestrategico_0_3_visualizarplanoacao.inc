<?
$sql = "SELECT d.docid, p.pdicodinep, p.pdeano, p.pdeid, d.esdid 
		FROM pdeinterativo.pdinterativo p 
		LEFT JOIN workflow.documento d ON d.docid = p.docid 
		WHERE p.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
$pdinterativo = $db->pegaLinha($sql);

$docid = $pdinterativo['docid'];
$esdid = $pdinterativo['esdid'];

if(!$docid) {
	$esdid = $db->pegaUm("SELECT esdid FROM workflow.estadodocumento WHERE tpdid='".TPD_WF_FLUXO."' AND esdordem='1'");
	$docid = $db->pegaUm("INSERT INTO workflow.documento(
	            		  tpdid, esdid, docdsc, docdatainclusao)
	    				  VALUES ('".TPD_WF_FLUXO."', '".$esdid."', 'PDE Interativo ".$pdinterativo['pdicodinep']."/".(($pdinterativo['pdeano'])?$pdinterativo['pdeano']:"XXXX")." ".$pdinterativo['pdeid']."', NOW()) RETURNING docid;");
	$db->executar("UPDATE pdeinterativo.pdinterativo SET docid='".$docid."' WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
	$db->commit();
}

?>
<script>

function gerenciarEstrategias(abaid,papid) {
	window.open('pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=gerenciarEstrategias&abaid='+abaid+'&papid='+papid,'Estrategias','scrollbars=yes,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function gerenciarAcao(paeid,paaid) {
	window.open('pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=gerenciarAcao&paeid='+paeid+'&paaid='+paaid,'Ação','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function excluirAcao(paaid) {
	var conf = confirm('Deseja realmente apagar a ação?');
	if(conf) {
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo.php?modulo=principal/planoestrategico&acao=A",
	   		data: "requisicao=excluirAcao&paaid="+paaid,
	   		async: false,
	   		success: function(msg){alert(msg);}
	 		});
		carregarPlanoEstrategicoDimensao();
	}
}

function carregarPlanoEstrategicoDimensao() {
	jQuery.ajax({
   		type: "POST",
   		url: "pdeinterativo.php?modulo=principal/planoestrategico&acao=A",
   		data: "requisicao=carregarPlanoEstrategicoDimensao",
   		async: false,
   		success: function(msg){document.getElementById('planoestrategico').innerHTML = msg;}
 		});
}

function filtrarPlanoEstrategico() {
	selectAllOptions( document.getElementById( 'abaid' ) );
	selectAllOptions( document.getElementById( 'papid' ) );
	selectAllOptions( document.getElementById( 'paeid' ) );
	
	document.getElementById('formulario').submit();

}

function gerenciarAnalises() {
	window.open('pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=gerenciarAnalises','Analises','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

function visualizarAnalises() {
	window.open('pdeinterativo.php?modulo=principal/planoestrategico&acao=A&requisicao=visualizarAnalises','Analises','scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}

jQuery(document).ready(function() {
	jQuery("[name^='metas[']").attr("disabled", true);;
	jQuery("[name^='metas2[']").attr("disabled", true);;
});

</script>
<style>
.bordapreto {
border: 1px solid #000;
}
</style>

<?
$perfis = pegaPerfilGeral();
// regra se for comite municipal ou estadual
if(($db->testa_superuser() || in_array(PDEINT_PERFIL_EQUIPE_MEC,$perfis)) && $esdid == WF_ESD_MEC) {
	$flagMEC = true;	
}
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações:</font></td>
	<td class="blue">
	<p>A inclusão ou exclusão de problemas modifica o respectivo Plano de Ação. Não esqueça de revisar também as estratégias, ações, itens e valores programados, se necessário.</p>
	<p>Parabéns! Vocês acabaram de concluir a elaboração do Plano de Desenvolvimento da <?=$_SESSION['pdeinterativo_vars']['pdenome'] ?>. Leia com cuidado tudo o que foi descrito e, se concordar, clique em “Enviar para o Comitê”. Caso discorde de alguma parte, retorne aos Planos de Ação ou ao Diagnóstico e faça os ajustes, até construir um plano que o Grupo de Trabalho concorde e que a escola se identifique.</p> 
	<p>Lembre-se: depois de enviar o plano para o Comitê, ele não poderá ser alterado, exceto se o Comitê devolve-lo, também via SIMEC, e solicitar algum ajuste. Caso contrário, o mesmo seguirá para análise do MEC, que também poderá solicitar alterações ou validar e solicitar a transferência dos recursos ao FNDE.</p>
	<p>Portanto, fique atento(a) às próximas etapas! Acesse periodicamente o SIMEC/ PDE Interativo e veja, na tela principal, em que fase o plano se encontra. O Ministério da Educação parabeniza a equipe pelo esforço e deseja sorte na execução, monitoramento e avaliação sistemática do plano!</p>
	</td>
</tr>
<? if($flagMEC): ?>
<tr>
	<td class="SubTituloDireita" style="background-color:#FFFFE0;" width="10%"><b>Analisar preços e Categorias</b></td>
	<td style="background-color:#FFFFE0;"><input type="button" value="Analisar" onclick="gerenciarAnalises();"> <input type="button" value="Visualizar" onclick="visualizarAnalises();"></td>
</tr>
<? endif; ?>
<tr>
	<td class="SubTituloDireita" width="10%">Filtros:</td>
	<td>
	<form method="post" id="formulario">
	<table>
	<tr>
		<td>
		<p><b>Dimensões</b></p>
		<? 
		$sql = "SELECT abaid as codigo, UPPER(abadescricao) as descricao FROM pdeinterativo.aba 
				WHERE abaidpai = '".ABA_DIAGNOSTICO."' AND abaid != ".ABA_DIAGNOSTICO_TAXASINDICADORES." AND abatipo IS NULL ORDER BY abaid";

		combo_popup( "abaid", $sql, "Dimensões", "400x500", 0, array(), "", "S", false, false, 5, 300 );				
		?>
		</td>
		<td>
		<p><b>Problemas</b></p>
		<? 
		$sql = "SELECT papid as codigo, UPPER(ab.abadescricao) || ' : ' ||papdescricao as descricao 
						 FROM pdeinterativo.planoacaoproblema pap 
						 LEFT JOIN pdeinterativo.aba ab ON ab.abacod = pap.abacod 
						 WHERE pap.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND pap.papstatus='A'
						 ORDER BY abaid, pap.papid";
		combo_popup( "papid", $sql, "Problemas", "400x500", 0, array(), "", "S", false, false, 5, 300 );				
		?>
		</td>
		<td>
		<p><b>Estratégias</b></p>
		<? 
		$sql = "SELECT paeid as codigo, epadesc as descricao 
						 FROM pdeinterativo.planoacaoestrategia pae 
						 LEFT JOIN pdeinterativo.estrategiaplanoacaoapoio epa ON epa.epaid = pae.paeid 
						 INNER JOIN pdeinterativo.planoacaoproblema pap ON pap.papid = pae.papid   
						 WHERE pap.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND pap.papstatus='A' AND pae.paestatus='A'
						 ORDER BY pap.papid, pae.paeid";
		combo_popup( "paeid", $sql, "Estratégias", "400x500", 0, array(), "", "S", false, false, 5, 300 );				
		?>
		</td>
	</tr>
	<tr>
		<td colspan="3" class="SubTituloDireita"><input type="button" name="sub" value="Filtrar" onclick="filtrarPlanoEstrategico();"> <input type="button" name="vertodos" value="Todos" onclick="window.location='pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_3_visualizarplanoacao';"></td>
	</tr>
	</table>
	</form>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2">Plano de Desenvolvimento da Escola</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Dados da Escola:</td>
	<td>
	<? 
	$escola = $db->pegaLinha("SELECT a.no_escola, a.tp_dependencia, m.mundescricao, a.sg_uf FROM educacenso_2010.tb_escola_inep_2010 a 
							  INNER JOIN territorios.municipio m ON m.muncod = a.co_municipio::character(7) 
							  WHERE a.pk_cod_entidade='".$_SESSION['pdeinterativo_vars']['pdicodinep']."'");
	?>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3">
	<tr>
		<td class="SubTituloDireita">Escola:</td>
		<td><?=$escola['no_escola'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Esfera:</td>
		<td><?=$escola['tp_dependencia'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Munícipio:</td>
		<td><?=$escola['mundescricao'] ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">UF:</td>
		<td><?=$escola['sg_uf'] ?></td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" width="10%">Grandes desafios:</td>
	<td>
	<?
	$dados['desabilitar'] = true;
	$dados['return_arr'] = true;
	$dados['filtro'] = "critico";
	$lista_c = (array) listasGrandesDesafios($dados);
	$dados['filtro'] = "desafiooutros";
	$lista_d = (array) listasGrandesDesafios($dados);
	$lista_f = array_merge($lista_c, $lista_d);
	echo implode("<br>",$lista_f);
	?>
	</td>
</tr>
</table>

<script>
function planoestrategico_0_3_visualizarplanoacao() {

	document.getElementById('formulario2').submit();
	
}
</script>

<form method="post" id="formulario2">
<input type="hidden" name="togo" id="togo" value="/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_3_visualizarplanoestrategico">
<input type="hidden" name="requisicao" value="planoestrategico_0_3_visualizarplanoacao">
<?

// regra se for comite municipal ou estadual
if(($db->testa_superuser() || in_array(PDEINT_PERFIL_EQUIPE_MEC,$perfis) || in_array(PDEINT_PERFIL_COMITE_ESTADUAL,$perfis) || in_array(PDEINT_PERFIL_COMITE_MUNICIPAL,$perfis) ) && $esdid == WF_ESD_COMITE) {
	$flagComite = true;	
}


if(verificaFlagPDEInterativo('atualizaplano')) {
	atualizarProblemasDimensao(array());
	$db->executar("UPDATE pdeinterativo.flag SET atualizaplano=FALSE WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
	$db->commit();
}


$sql = "SELECT UPPER(abadescricao) as abadescricao, abacod, abaid FROM pdeinterativo.aba 
		WHERE abaidpai = '".ABA_DIAGNOSTICO."' AND abaid != ".ABA_DIAGNOSTICO_TAXASINDICADORES." AND abatipo IS NULL ".(($_REQUEST['abaid'][0])?"AND abaid IN(".implode(",",$_REQUEST['abaid']).")":"")." ORDER BY abaid";

$abas = $db->carregar($sql);

if($abas[0]) {

	foreach($abas as $aba) {
		echo "<br>";
		echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
		echo "<tr><td class=SubTituloCentro colspan=2>".$aba['abadescricao']."</td></tr>";

		$aoadesc = $db->pegaUm("SELECT aoadesc FROM pdeinterativo.objetivoplanoacao opa 
					 			LEFT JOIN pdeinterativo.apoioobjetivoplanoacao aop ON aop.aoaid = opa.aoaid 
					 			WHERE opa.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND opa.abaid='".$aba['abaid']."'");
	
		echo "<tr><td class=SubTituloDireita width=10%><b>Objetivo</b></td><td>".$aoadesc."</td></tr>";
		
		$dados_metas['somente_resposta'] = true;
		$funcao = "listametas_".$aba['abacod'];
		$metas = $funcao($dados_metas);
		
		if($metas) {
			echo "<tr><td class=SubTituloDireita width=10%><b>Metas</b></td><td>".(($metas)?implode("<br>", $metas):"")."</td></tr>";
		} else {
			echo "<tr><td class=SubTituloDireita colspan=2>Não foram identificados metas</td></tr>";
		}
		echo "</table>";
		
		$sql = "SELECT prb.papid, prb.papdescricao FROM pdeinterativo.planoacaoproblema prb 
				WHERE prb.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND prb.papstatus='A' AND prb.abacod='".$aba['abacod']."' ".(($_REQUEST['papid'][0])?"AND prb.papid IN(".implode(",",$_REQUEST['papid']).")":"")." 
				ORDER BY prb.papid";
		$problemas = $db->carregar($sql);
		
		if($problemas[0]) {
			
			echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
		
			foreach($problemas as $p) {
				
				echo "<tr><td class=SubTituloDireita width=10% style='border: 1px solid #000;'>Problema</td><td style='border: 1px solid #000;'>".$p['papdescricao']."</td></tr>";
				echo "<tr><td class=SubTituloDireita width=10%>Plano de ação</td><td>";
				
				$sql = "SELECT * FROM pdeinterativo.planoacaoestrategia pae 
						INNER JOIN pdeinterativo.estrategiaplanoacaoapoio epa ON epa.epaid = pae.epaid 
						WHERE pae.papid='".$p['papid']."' AND pae.paestatus='A' ".(($_REQUEST['paeid'][0])?"AND pae.paeid IN(".implode(",",$_REQUEST['paeid']).")":"");
				
				$estrategias = $db->carregar($sql);
				
				if($estrategias[0]) {
					
					echo "<table class=listagem width=100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
					echo "<thead><tr><td width=20%>Estratégia</td><td align=right>&nbsp;</td></tr></thead>";
		
					foreach($estrategias as $e) {
						if($e['respid']) {
							if(substr($e['respid'],0,1)=="D") {
								$respnome = $db->pegaUm("SELECT no_docente FROM educacenso_2010.tab_docente WHERE pk_cod_docente='".substr($e['respid'],2)."'");
							} else {
								$respnome = $db->pegaUm("SELECT pesnome FROM pdeinterativo.pessoa WHERE pesid='".substr($e['respid'],2)."'");
							}
						}
						echo "<tr><td>".$e['epadesc']."<br/><br/><b>Responsável:</b> ".$respnome."</td><td>";
						
						$sql = "SELECT paa.paaid, aca.aapdesc, paa.paaacaoqtd, oba.oapdesc, paa.paadetalhamento, to_char(paaperiodoinicio,'dd/mm/YYYY') as paaperiodoinicio, to_char(paaperiodofim,'dd/mm/YYYY') as paaperiodofim FROM pdeinterativo.planoacaoacao paa 
								LEFT JOIN pdeinterativo.acaoapoio aca ON aca.aapid = paa.aapid 
								LEFT JOIN pdeinterativo.objetoapoio oba ON oba.oapid = paa.oapid
								WHERE paa.paeid='".$e['paeid']."' AND paa.paastatus='A'";
						
						$acoes = $db->carregar($sql);
						
						if($acoes[0]) {
							
							echo "<table class=listagem width=100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
							echo "<thead><tr><td>Ação</td><td>Detalhamento</td><td>Período</td><td>&nbsp;</td></tr></thead>";
							
							foreach($acoes as $a) {
								
								echo "<tr>
										<td width=12%>".$a['aapdesc']." ".$a['paaacaoqtd']." ".$a['oapdesc']."</td>
										<td width=12%>".$a['paadetalhamento']."</td>
										<td width=12%>".$a['paaperiodoinicio']." a ".$a['paaperiodofim']."</td>
										<td>";
								
								$sql = "SELECT pab.pabcomiteanalise, pab.pabid, pab.pabqtd, urf.uredesc, cia.ciadesc, pab.pabvalorcapital, pab.pabvalorcusteiro,
				  					   CASE WHEN pabfonte='P' THEN 'PDDE/PDE Escola' ELSE 'Outras' END as pabfonte,
		  							   CASE WHEN pabparcela='P' THEN '1ª Parcela (2011)' 
		  							   		WHEN pabparcela='S' THEN '2ª Parcela (2012)' 
		  							   		ELSE '' END as pabparcela
										FROM pdeinterativo.planoacaobemservico pab 
										LEFT JOIN pdeinterativo.unidadereferencia urf ON urf.ureid = pab.ureid 
										LEFT JOIN pdeinterativo.categoriaitemacao cia ON cia.ciaid = pab.ciaid
										WHERE pab.paaid='".$a['paaid']."' AND pab.pabstatus='A'";
								
								$bensservicos = $db->carregar($sql);
								
								if($bensservicos[0]) {
									echo "<table class=listagem width=100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
									echo "<thead><tr><td width=30%>Bens e Serviços</td><td width=15%>Capital(R$)</td><td width=15%>Custeio(R$)</td><td width=20%>Fonte</td><td width=20%>Parcela</td>
										  ".(($flagComite)?"<td style=background-color:#FFFFE0;text-align:center; onmouseover=\"return escape('Assinale os itens que o Comitê considerar aprovados');\"><font size=1>Validar item</font></td>":"")."</tr></thead>";
									
									foreach($bensservicos as $b) {
										echo "<tr>
												<td>".$b['pabqtd']." ".$b['uredesc']." ".$b['ciadesc']."</td>
												<td>".(($b['pabvalorcapital'])?number_format($b['pabvalorcapital'],2,",","."):"-")."</td>
												<td>".(($b['pabvalorcusteiro'])?number_format($b['pabvalorcusteiro'],2,",","."):"-")."</td>
												<td>".$b['pabfonte']."</td>
												<td>".$b['pabparcela']."</td>
												".(($flagComite)?"<td style=background-color:#FFFFE0;><input type=hidden name=planoacaobemservico[comite][".$b['pabid']."] value=\"FALSE\"><input type=checkbox name=planoacaobemservico[comite][".$b['pabid']."] value=\"TRUE\" ".(($b['pabcomiteanalise']=="t")?"checked":"")."></td>":"")."
											  </tr>";								
									}
									
									echo "</table>";
									
								} else {
									echo "<table class=listagem width=100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
									echo "<tr><td class=SubTituloCentro>Não existem bens e serviços cadastradas na ação</td></tr>";
									echo "</table>";
								}
								
								echo "</td></tr>";
								
							}
							
							echo "</table>";
							
						} else {
							echo "<table class=listagem width=100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
							echo "<tr><td class=SubTituloCentro>Não existem ações cadastradas na estratégia</td></tr>";
							echo "</table>";
						}
		
						echo "</td></tr>";
					}
					
					echo "</table>";
					
				} else {
					echo "<table class=listagem width=100% bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
					echo "<tr><td class=SubTituloCentro>Não existem estratégias cadastradas neste problema</td></tr>";
					echo "</table>";
				}
				
				echo "</td></tr>";
				
			}
			
			if($flagComite) :
			
				echo "<tr><td style=\"background-color:#FFFFE0;\" colspan=2 align=right><input type=\"button\" name=\"btnanalisesalvar\" value=\"Salvar Análise\" onclick=\"document.getElementById('togo').value='/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_3_visualizarplanoacao';planoestrategico_0_3_visualizarplanoacao();\"></td></tr>";
				
			endif;
			
			echo "</table>";
			
		} else {
			echo "<table class=tabela bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3 align=center>";
			echo "<tr><td class=SubTituloEsquerda>Não existem problemas nesta dimensão</td></tr>";
			echo "</table>";
		}
	}
}

?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita">
	O que deseja fazer agora?
	</td>
	<td>
	<?
	include APPRAIZ ."includes/workflow.php";
	/* Barra de estado atual e ações e Historico */
	
	// criando tabela de diagnostico para ser visuzalido no popup de comentario
	if($flagComite) :
		$html .= "<table class=listagem cellSpacing=1 cellPadding=3 align=center width=100%>";
		$html .= "<tr>";
		$html .= "<td class=SubTituloDireita>Parecer Diagnóstico</td>";
		$sql = "SELECT a.abadescricao, p.prcparecer FROM pdeinterativo.parecer p 
				INNER JOIN pdeinterativo.aba a ON a.abaid = p.abaid 
				WHERE p.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND p.prcstatus='A' AND p.prcparecer IS NOT NULL ORDER BY a.abaid";
		$parecediag = $db->carregar($sql);
		if($parecediag[0]) {
			foreach($parecediag as $pardiag) {
				$_diagnostico[$pardiag['abadescricao']][] = $pardiag['prcparecer']; 
			}
		}
		
		$html .= "<td><p>O Diagnóstico deverá ser ajustado de acordo com os seguintes comentários:</p>";
		
		if($_diagnostico) {
			foreach($_diagnostico as $abadescricao => $pareceres) {
				$html .= "<b>".$abadescricao."</b><br>";
				foreach($pareceres as $parecer) {
					$html .= "- ".$parecer."<br>";
				}
			}
		}
		unset($_diagnostico);
		
		$html .= "</td>";
		$html .= "</tr>";
		$html .= "<tr>";
		$html .= "<td class=SubTituloDireita>Parecer Planos de Ação</td>";
		$html .= "<td><p>Os itens abaixo precisam ser revistos (valores, quantidades e/ou o próprio item):</p>";
		
		$sql = "select ps.pabqtd||' '||ci.ciadesc as item, abadescricao as dimensao from pdeinterativo.planoacaoproblema pa 
				inner join pdeinterativo.aba ab on ab.abacod=pa.abacod
				inner join pdeinterativo.planoacaoestrategia pe on pe.papid=pa.papid 
				inner join pdeinterativo.planoacaoacao pc on pc.paeid=pe.paeid 
				inner join pdeinterativo.planoacaobemservico ps on ps.paaid=pc.paaid 
				inner join pdeinterativo.categoriaitemacao ci on ci.ciaid=ps.ciaid
				where pa.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' and 
					  pa.papstatus='A' and 
					  pe.paestatus='A' AND 
					  pc.paastatus='A' AND 
					  ps.pabstatus='A' AND 
					  (ps.pabcomiteanalise IS NULL OR ps.pabcomiteanalise=FALSE)";

		$placao = $db->carregar($sql);
		
		if($placao[0]) {
			foreach($placao as $pa) {
				$_diagnostico[$pa['dimensao']][] = $pa['item'];
			}
		}
		
		if($_diagnostico) {
			foreach($_diagnostico as $abadescricao => $pareceres) {
				$html .= "<b>".$abadescricao."</b><br>";
				foreach($pareceres as $parecer) {
					$html .= "- ".$parecer."<br>";
				}
			}
		}
		unset($_diagnostico);
		
		$html .= "<p>Em caso de dúvida, procure o Comitê de Análise e Aprovação da sua Secretaria.</p></td>";
		
		$html .= "</tr>";
		$html .= "</table>";
		
		$_SESSION['pdeinterativo_vars']['workflow_informacoes'] = $html;
		
	endif;
	// FIM - criando tabela de diagnostico para ser visuzalido no popup de comentario
	
	if($flagMEC) :
	
		$sql = "SELECT cac.cacdesc, cia.ciadesc, ure.uredesc, pab.pabqtd, pab.pabvalor, pab.pabvalorcapital, pab.pabvalorcusteiro,
					   CASE WHEN pabfonte='P' THEN 'PDDE/PDE Escola' ELSE 'Outras' END as pabfonte,
  					   CASE WHEN pabparcela='P' THEN '1ª Parcela (2011)' ELSE '2ª Parcela (2012)' END as parcela
				FROM pdeinterativo.planoacaoproblema pap 
				INNER JOIN pdeinterativo.planoacaoestrategia pae ON pae.papid = pap.papid 
				INNER JOIN pdeinterativo.planoacaoacao paa ON paa.paeid = pae.paeid 
				INNER JOIN pdeinterativo.planoacaobemservico pab ON pab.paaid = paa.paaid 
				LEFT JOIN pdeinterativo.categoriaitemacao cia ON cia.ciaid = pab.ciaid 
				LEFT JOIN pdeinterativo.unidadereferencia ure ON ure.ureid = cia.ureid 
				LEFT JOIN pdeinterativo.categoriaacao cac ON cac.cacid = cia.cacid
				WHERE pab.pabstatus='A' AND paa.paastatus='A' AND pae.paestatus='A' AND pap.papstatus='A' AND pap.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND pab.pabvalor > cia.ciaprecomaximo AND (pabmecanalise=FALSE OR pabmecanalise IS NULL)";
		
		$itensmaximo = $db->carregar($sql);
		
		$sql = "SELECT cac.cacdesc, cia.ciadesc, ure.uredesc, pab.pabqtd, pab.pabvalor, pab.pabvalorcapital, pab.pabvalorcusteiro,
					   CASE WHEN pabfonte='P' THEN 'PDDE/PDE Escola' ELSE 'Outras' END as pabfonte,
  					   CASE WHEN pabparcela='P' THEN '1ª Parcela (2011)' ELSE '2ª Parcela (2012)' END as parcela 
				FROM pdeinterativo.planoacaoproblema pap 
				INNER JOIN pdeinterativo.planoacaoestrategia pae ON pae.papid = pap.papid 
				INNER JOIN pdeinterativo.planoacaoacao paa ON paa.paeid = pae.paeid 
				INNER JOIN pdeinterativo.planoacaobemservico pab ON pab.paaid = paa.paaid 
				LEFT JOIN pdeinterativo.categoriaitemacao cia ON cia.ciaid = pab.ciaid 
				LEFT JOIN pdeinterativo.unidadereferencia ure ON ure.ureid = cia.ureid 
				LEFT JOIN pdeinterativo.categoriaacao cac ON cac.cacid = cia.cacid
				WHERE pab.pabstatus='A' AND paa.paastatus='A' AND pae.paestatus='A' AND pap.papstatus='A' AND pap.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND pab.pabvalor < cia.ciaprecominimo AND (pabmecanalise=FALSE OR pabmecanalise IS NULL)";
		
		$itensminimo = $db->carregar($sql);
		
		$html .= "<table class=listagem cellSpacing=1 cellPadding=3 align=center width=100%>";
		$html .= "<tr>";
		$html .= "<td class=SubTituloDireita>Parecer</td>";
		$html .= "<td>";
		
		if($itensmaximo[0]) {
			$html .= "Itens com valores unitários superiores ao parâmetro do MEC não foram validados:<br><br>";
			foreach($itensmaximo as $ima) {
				$html .= "- <b>Categoria</b> : ".$ima['cacdesc']." - <b>Item</b> : ".$ima['ciadesc']." - <b>Unidade</b> : ".$ima['uredesc']." - <b>Fonte</b> : ".$ima['pabfonte']." - <b>Parcela</b> : ".$ima['parcela'].";<br>";
			}
		}

		if($itensminimo[0]) {
			$html .= "<br>Itens com valores unitários inferiores ao parâmetro do MEC não foram validados:<br><br>";
			foreach($itensminimo as $imo) {
				$html .= "- <b>Categoria</b> : ".$imo['cacdesc']." - <b>Item</b> : ".$imo['ciadesc']." - <b>Unidade</b> : ".$imo['uredesc']." - <b>Fonte</b> : ".$imo['pabfonte']." - <b>Parcela</b> : ".$imo['parcela'].";<br>";
			}
		}
		
		
		$html .= "</td>";
		$html .= "</tr>";
		$html .= "</table>"; 
		
		$_SESSION['pdeinterativo_vars']['workflow_informacoes'] = $html;
		
	endif;
	
	wf_desenhaBarraNavegacaoPdeInterativo( $docid, array() );			
	
	?>
	<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo/pdeinterativo.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_2_planoacao';" >
	</td>
</tr>
</table>
</form>
<?
function wf_desenhaBarraNavegacaoPdeInterativo( $docid, array $dados, $ocultar = null )
{
	/*
	 * $ocultar - Define quais areas serão ocultadas. ex.: $ocultar['historico'] = true;
	 * 
	 * --- Definidas ---
	 * historico       : Oculta linha contendo informações obre o historico 
	 */
	
	global $db;
	$docid = (integer) $docid;
	
	// captura dados gerais
	$documento = wf_pegarDocumento( $docid );
	if ( !$documento )
	{
		?>
		<table align="center" border="0" cellpadding="5" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #d0d0d0; width: 80px;">
			<tr>
				<td style="text-align: center;">
					Documento inexistente!
				</td>
			</tr>
		</table>
		<br/><br/>
		<?php
		return;
	}

	$estadoAtual = wf_pegarEstadoAtual( $docid );
	//$estados = wf_pegarProximosEstadosPossiveis( $docid, $dados );
	$estados = wf_pegarProximosEstados( $docid, $dados );
	$modificacao = wf_pegarUltimaDataModificacao( $docid );
	$usuario = wf_pegarUltimoUsuarioModificacao( $docid );
	$comentario = trim( substr( wf_pegarComentarioEstadoAtual( $docid ), 0, 50 ) ) . "...";
	
	$dadosHtml = serialize( $dados );
	?>
	<script type="text/javascript">
		
		function wf_atualizarTela( mensagem, janela )
		{
			janela.close();
			alert( mensagem );
			window.location.reload();
		}
		
		function wf_alterarEstado( aedid, docid, esdid, acao )
		{
			if(acao) acao = acao.toLowerCase();
			if ( !confirm( 'Deseja realmente ' + acao + ' ?' ) )
			{
				return;
			}
			var url = '/pdeinterativo/workflow/alterar_estado_pdeinterativo.php' +
				'?aedid=' + aedid +
				'&docid=' + docid +
				'&esdid=' + esdid +
				'&verificacao=<?php echo urlencode( $dadosHtml ); ?>';
			var janela = window.open(
				url,
				'alterarEstado',
				'width=550,height=650,scrollbars=yes,scrolling=no,resizebled=no'
			);
			janela.focus();
		}
		
		function wf_exibirHistorico( docid )
		{
			var url = 'http://pdeinterativo.mec.gov.br/geral/workflow/historico.php' +
				'?modulo=principal/tramitacao' +
				'&acao=C' +
				'&docid=' + docid;
			window.open(
				url,
				'alterarEstado',
				'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
			);
		}
		
	</script>
	<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
		<tr>
		<?php if ( count( $estados ) ) : ?>
			<?php $nenhumaacao = true; ?>
			<?php foreach ( $estados as $estado ) : 
						$action = wf_acaoPossivelPdeInterativo( $docid, $estado['aedid'], $dados );?>
				<?php if($action === true) : ?>
				<?php $nenhumaacao = false; ?>

					<td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;" onmouseover="this.style.backgroundColor='#ffffdd';" onmouseout="this.style.backgroundColor='';">
						<input type="button" name="realizando" value="<?php echo $estado['aeddscrealizar'] ?>" onclick="wf_alterarEstado( '<?php echo $estado['aedid'] ?>', '<?php echo $docid ?>', '<?php echo $estado['esdid'] ?>', '<?php echo $estado['aeddscrealizar'] ?>' );">
					</td>
				<?php else :?>
				
					<? if($action === false) : ?>
					
						<? #Oculta linha contendo a ação cuja a condição para tramitação não esteja atendida 
						if( $estado['aedcodicaonegativa'] == 't') : ?>
						<?php $nenhumaacao = false; ?>
								<td style="font-size: 7pt; color: #909090; border-top: 2px solid #d0d0d0; text-align: center;" onmouseover="return escape('<? echo $estado['aedobs']; ?>');">
								<input type="button" name="realizando" value="<?php echo $estado['aeddscrealizar'] ?>" onclick="alert( '<?php echo $estado['aedobs']; ?>' )">
								</td>
						<? endif; ?>
						
					<?php else :?>
					<?php $nenhumaacao = false; ?>
						<td style="font-size: 7pt; color: #909090; border-top: 2px solid #d0d0d0; text-align: center;" onmouseover="return escape('<? echo $action; ?>');">
							<input type="button" name="realizando" value="<?php echo $estado['aeddscrealizar'] ?>" onclick="alert( '<?php echo $action; ?>' )">
						</td>
					<?php endif; ?>
					
				<?php endif; ?>
			<?php endforeach; ?>
			<?php if($nenhumaacao) : ?>
				<td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;">
					Nenhuma ação disponível para o documento
				</td>
			<?php endif; ?>
		<?php else: ?>
				<td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;">
					Nenhuma ação disponível para o documento
				</td>
		<?php endif; ?>
			<td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;">
				<input type="button" name="btn_imprimir" value="Imprimir" onclick="window.print();" >
			</td>

		<? if(!$ocultar['historico']) { ?>
			<td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;">
				<span title="estado atual" style="cursor:pointer;" onclick="wf_exibirHistorico( '<?php echo $docid ?>' );">
					<b>[Histórico]</b>
				</span>
			</td>
		<? } ?>
		</tr>
	</table>
	<br/>
	<?php
}


function wf_acaoPossivelPdeInterativo( $docid, $aedid, array $dados )
{
	global $db;
	// verifica se usuário possui perfil da ação
	wf_verificarPerfil( $aedid );
	// realiza condição extra
	
	return wf_realizarVerificacao( $aedid, $dados );
}
?>