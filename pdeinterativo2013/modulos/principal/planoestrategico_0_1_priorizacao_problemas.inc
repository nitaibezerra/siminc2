<?php
$estado = $db->pegaUm("SELECT d.esdid FROM pdeinterativo2013.pdinterativo p INNER JOIN workflow.documento d ON p.docid=d.docid WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
if(!$db->testa_superuser() && in_array(PDEESC_PERFIL_DIRETOR,pegaPerfilGeral()) && $estado != WF_ESD_ELABORACAO && $estado != WF_ESD_ELABORACAO_SEMPDE && $estado != WF_ESD_ELABORACAO_FEDERAL): ?>
<p align="center"><font color=red>PDE não esta em elaboração. Não é permitido alteração. Por favor visualize na aba <b>1.3 Visualizar PDE</b></font></p>
<? else: ?>
<style>
.bordapreto {
border: 1px solid #000;
height:32px;
background-color:#ffffff;
}
.branco{background-color:#FFFFFF}
</style>
<script>
function calculaNumProblemasCriticos() {
	var numTotal = jQuery("[type=checkbox]").length;
	
	if(numTotal > 0){
		var num = numTotal*0.3;
		num = num.toFixed(1);
		num +='';
		if(num.search(".") >= 0){
			var NewNum = num.split(".");
			if( ((NewNum[1])*1) >= 5 ){
				num = (NewNum[0]*1);
				num = num + 1;
			}else{
				num = (NewNum[0]*1);
			}
		}
		if(num == 0){
			num = 1;
		}
		jQuery("#num_problemas_possiveis").html(num);
	}
}

function verificaCheckBox(obj) {
	if(obj.checked == true) {
		var num = jQuery("#num_problemas_possiveis").html();
		var numMarcados = jQuery("[type=checkbox]:checked").length;
		num = num*1;
		numMarcados = numMarcados*1;
		if(numMarcados > num)
		{
			jQuery("[name='" + obj.name + "']").attr("checked","");
			alert('Selecione no máximo ' + num + ' problema(s) como crítico(s)!');
		}
	}
}

function popUpDimensao2(funcao)
{
	janela('pdeinterativo2013.php?modulo=principal/popUpSinteseDimensao2&acao=A&requisicao=' + funcao ,500,300,'Turmas Críticas');
}

function exibeTurma(id)
{
	jQuery("#" + id).show();
	jQuery( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
}

jQuery(document).ready(function() {
	calculaNumProblemasCriticos();
});

function salvarPriorizacaoProblemas(continuar)
{
	var num = jQuery("#num_problemas_possiveis").html();
	var numMarcados = jQuery("[type=checkbox]:checked").length;
	num = num*1;
	numMarcados = numMarcados*1; 
	
	if(continuar == 'continuar'){
		jQuery("#hdn_redirect").val("C");
	}else{
		jQuery("#hdn_redirect").val("");
	}
	
	if(num > 0){
		if(numMarcados > 0){
			divCarregando();
			document.getElementById('formulario').submit();
		}else {
			alert('Marque pelo menos um problema');
			return false;
		}
	}else{
		divCarregando();
		document.getElementById('formulario').submit();
	}
}

</script>
<form method="post" id="formulario" >
<input type="hidden" name="togo" id="togo" value="/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_2_planoacao">
<input type="hidden" name="requisicao" value="salvarPriorizacaoProblemas">
<input type="hidden" name="hdn_redirect" id="hdn_redirect" value="">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td class="SubTituloDireita" width="10%"><font class="blue">Orientações:</font></td>
	<td class="blue" colspan="2">
		Aguardando texto...
	</td>
</tr>
<tr>
	<td class="SubTituloDireita bold center" >Dimensão</td>
	<td class="SubTituloDireita bold center" width="60%" >Problemas Identificados</td>
	<td class="SubTituloDireita bold center" width="15%" >Problema(s) Críticos<br/>(Máximo <span id="num_problemas_possiveis">X</span>)</td>
</tr>
<tr>
	<td class="SubTituloCentro bold direita" >Indicadores e Taxas</td>
	<td class="center" >
	<?php
	// Avaliado os problemas do IDEB	
	$sql = "SELECT * FROM pdeinterativo2013.respostaideb WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
	$dadosideb = $db->pegaLinha($sql);
	
	if($dadosideb['ridinicialum']=="N") {
		$problemasideb['ridinicialum'] = "O IDEB dos anos iniciais da escola não melhorou nas duas últimas medições disponíveis";
	}
	if($dadosideb['ridinicialdois']=="N") {
		$problemasideb['ridinicialdois'] = "Não há evidências de que a escola alcançará a próxima meta do IDEB, para os Anos Iniciais";
	}
	if($dadosideb['ridfinalum']=="N") {
		$problemasideb['ridfinalum'] = "O IDEB dos anos finais da escola não melhorou nas duas últimas medições disponíveis";
	}
	if($dadosideb['ridfinaldois']=="N") {
		$problemasideb['ridfinaldois'] = "Não há evidências de que a escola alcançará a próxima meta do IDEB, para os Anos Finais";
	}
	echo "<table width=100% >";
	if(count($problemasideb) > 0) {
		foreach($problemasideb as $indice => $pideb) {
			echo "<tr><td class='bordapreto'>".$pideb."</td><input type=hidden name=respostaideb[".$indice."critico] value=FALSE></td></tr>";	
		}
	}
	// Avaliando os problemas das taxas de rendimento	
	$sql = "SELECT * FROM pdeinterativo2013.respostataxarendimento WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
	$dadostx = $db->pegaLinha($sql);
	
	if($dadostx['rtrfunaprova']=="N") {
		$problemastx['rtrfunaprova'] = "A taxa de aprovação da escola, no ensino fundamental, não melhorou nos últimos dois anos";
	}
	if($dadostx['rtrfunreprova']=="N") {
		$problemastx['rtrfunreprova'] = "A taxa de reprovação da escola, no ensino fundamental, não melhorou nos últimos dois anos";
	}
	if($dadostx['rtrfunabandono']=="N") {
		$problemastx['rtrfunabandono'] = "A taxa de abandono da escola, no ensino fundamental, não melhorou nos últimos dois anos";
	}
	if($dadostx['rtrmedaprova']=="N") {
		$problemastx['rtrmedaprova'] = "A taxa de aprovação da escola, no ensino médio, não melhorou nos últimos dois anos";
	}
	if($dadostx['rtrmedreprova']=="N") {
		$problemastx['rtrmedreprova'] = "A taxa de reprovação da escola, no ensino médio, não melhorou nos últimos dois anos";
	}
	if($dadostx['rtrmedabandono']=="N") {
		$problemastx['rtrmedabandono'] = "A taxa de abandono da escola, no ensino médio, não melhorou nos últimos dois anos";
	}
	if(count($problemastx) > 0) {
		foreach($problemastx as $indice => $ptx) {
			echo "<tr><td class='bordapreto'>".$ptx."</td><input type=hidden name=respostataxarendimento[".$indice."critico] value=FALSE></td></tr>";	
		}
	}
	// Avaliando os problemas da prova brasil	
	$sql = "SELECT * FROM pdeinterativo2013.respostaprovabrasil WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
	$dadospb = $db->pegaLinha($sql);
	
	if($dadospb['rpbinicialport']=="N") {
		$problemaspb['rpbinicialport'] = "Os resultados de Língua Portuguesa na Prova Brasil, nos anos iniciais, não demonstram evolução nas duas últimas medições";
	}
	if($dadospb['rpbinicialmat']=="N") {
		$problemaspb['rpbinicialmat'] = "Os resultados de Matemática  na Prova Brasil, nos anos iniciais, não demonstram evolução nas duas últimas medições";
	}
	if($dadospb['rpbfinalport']=="N") {
		$problemaspb['rpbfinalport'] = "Os resultados de Língua Portuguesa na Prova Brasil, nos anos finais, não demonstram evolução nas duas últimas medições";
	}
	if($dadospb['rpbfinalmat']=="N") {
		$problemaspb['rpbfinalmat'] = "Os resultados de Matemática  na Prova Brasil, nos anos finais, não demonstram evolução nas duas últimas medições";
	}
	if(count($problemaspb) > 0) {
		foreach($problemaspb as $indice => $ppb) {
			echo "<tr><td class='bordapreto'>".$ppb."</td><input type=hidden name=respostaprovabrasil[".$indice."critico] value=FALSE></td></tr>";	
		}
	}
	if(count($problemaspb) == 0 && count($problemastx) == 0 && count($problemaspb) == 0) {
		echo "<tr><td class='bordapreto' >A evolução desse(s) indicador(es) não apresentou problemas.</td></tr>";
	}
	
	echo "</table>";
	?>
	</td>
	<td class="center" >
	<?php
		echo "<table width=100% >";
		if(count($problemasideb) > 0) {
			foreach($problemasideb as $indice => $pideb) {
				echo "<tr><td class='bordapreto center' ><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($dadosideb[$indice."critico"]=="t")?"checked":"")." name=respostaideb[".$indice."critico] value=TRUE></td></tr>";	
			}
		}
		if(count($problemastx) > 0) {
			foreach($problemastx as $indice => $ptx) {
				echo "<tr><td class='bordapreto center' ><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($dadostx[$indice."critico"]=="t")?"checked":"")." name=respostataxarendimento[".$indice."critico] value=TRUE></td></tr>";	
			}
		}
		if(count($problemaspb) > 0) {
			foreach($problemaspb as $indice => $ppb) {
				echo "<tr><td class='bordapreto center' ><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($dadospb[$indice."critico"]=="t")?"checked":"")." name=respostaprovabrasil[".$indice."critico] value=TRUE></td></tr>";	
			}
		}
		echo "</table>";
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro bold direita branco" >Distorção e Aproveitamento</td>
	<td class="center branco" >
		<?php 
			$problema_da = false;
			echo "<table width=100% >"; 
			$arrDistorcao1 = carregaDistorcaoDiagnosticoMatricula(false,"M",null);
			if($arrDistorcao1){
				 echo "<tr><td class=bordapreto>Em 2012, a escola possuía ".count($arrDistorcao1)." turma(s) com nº de matrículas superior ao parâmetro do CNE.<a href=\"javascript:popUpDimensao2('exibeTurmasCNE')\" >Clique aqui para exibir a(s) turma(s).</a> </tr>";
				 $problema_da = true;
			}
			$arrDistorcao2 = carregaDistorcaoDiagnosticoMatricula(null,"D","D");
			if($arrDistorcao2){
				 echo "<tr><td class=bordapreto>Em 2012, a escola possuía ".count($arrDistorcao2)." turma(s) com taxa de distorção superior à média do Brasil.<a href=\"javascript:popUpDimensao2('exibeTurmasCriticas')\" >Clique aqui para exibir a(s) turma(s).</a> </tr>";
				 $problema_da = true;
			}
			$arrRespEscola = recuperaRespostasEscola(null,"D","D",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')"));
			if($arrRespEscola){
				foreach($arrRespEscola as $resp){
					echo "<tr><td class=bordapreto>".str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc'])."</td></tr>";
					$problema_da = true;
				}
			}
			$arrDistorcaoReprovacao = carregaDistorcaoDiagnosticoMatricula(null,"A","R");
			if($arrDistorcaoReprovacao){
				 echo "<tr><td class=bordapreto>Em 2012, a escola possuía ".count($arrDistorcaoReprovacao)." turma(s) com taxa de reprovação superior à média do Brasil.<a href=\"javascript:popUpDimensao2('exibeTurmasCriticasReprovacao')\" >Clique aqui para exibir a(s) turma(s).</a> </tr>";
				 $problema_da = true;
			}
			$arrDistorcaoAbandono = carregaDistorcaoDiagnosticoMatricula(null,"A","A");
			if($arrDistorcaoAbandono){
				 echo "<tr><td class=bordapreto>Em 2012, a escola possuía ".count($arrDistorcaoAbandono)." turma(s) com taxa de abandono superior à média do Brasil.<a href=\"javascript:popUpDimensao2('exibeTurmasCriticasAbandono')\" >Clique aqui para exibir a(s) turma(s).</a> </tr>";
				 $problema_da = true;
			}
			$arrRespEscola2 = recuperaRespostasEscola(null,"D","A",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')"));
			if($arrRespEscola2){
				foreach($arrRespEscola2 as $resp){
					echo "<tr><td class=bordapreto>".str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc'])."</td></tr>";
					$problema_da = true;
				}
			}
			$arrTurmas = recuperaTurmasCriticasPorEscola();
			$arrTaxaReprovacaoBrasil = carregaTaxa();
			$arrDisciplinas = retornaDisciplinasTurma();
			$arrDistorcaoTaxaReprovacaoDisciplina = carregaDistorcaoTaxaReprovacaoDisciplina();
			$arrDistorcaoTaxa = carregaDistorcaoDiagnosticoTaxaEscolar();
			if($arrTurmas){
				if($arrTurmas['Ensino Fundamental']){
					foreach($arrTurmas['Ensino Fundamental'] as $em){
						if($arrDisciplinas[$em['pk_cod_turma']]){
							foreach($arrDisciplinas[$em['pk_cod_turma']] as $ds){
								if( $arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']] != "" && $em['nummatricula'] ){
									if( round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0) > $arrTaxaReprovacaoBrasil['U']){
										$arrTurmasCriticasReprovacao["U"][$ds['pk_cod_disciplina']][] = $em['pk_cod_turma'];
										$arrNomeDisciplina[$ds['pk_cod_disciplina']] = $ds['no_disciplina'];
										$arrTurmasC["U"][$ds['pk_cod_disciplina']][$em['pk_cod_turma']] = array( 
																											"disciplina" => $ds['no_disciplina'],
																											"serie" => $em['serie'],
																											"turma" => $em['turma'],
																											"horario" => $em['hrinicio']." - ".$em['hrfim'],
																											"taxaBrasil" => $arrTaxaReprovacaoBrasil['U']." %",
																											"taxa" => round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0)." %"
																										   );
									}
								}
							}
						}
					}
				}
				if($arrTurmas['Ensino Médio']){
					foreach($arrTurmas['Ensino Médio'] as $em){
						if($arrDisciplinas[$em['pk_cod_turma']]){
							foreach($arrDisciplinas[$em['pk_cod_turma']] as $ds){
								if( $arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']] != "" && $em['nummatricula'] ){
									if( round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0) > $arrTaxaReprovacaoBrasil['M']){
										$arrTurmasCriticasReprovacao["M"][$ds['pk_cod_disciplina']][] = $em['pk_cod_turma'];
										$arrNomeDisciplina[$ds['pk_cod_disciplina']] = $ds['no_disciplina'];
										$arrTurmasC["M"][$ds['pk_cod_disciplina']][$em['pk_cod_turma']] = array( 
																											"disciplina" => $ds['no_disciplina'],
																											"serie" => $em['serie'],
																											"turma" => $em['turma'],
																											"horario" => $em['hrinicio']." - ".$em['hrfim'],
																											"taxaBrasil" => $arrTaxaReprovacaoBrasil['M']." %",
																											"taxa" => round((($arrDistorcaoTaxaReprovacaoDisciplina[$em['pk_cod_turma']][$ds['pk_cod_disciplina']]/$em['nummatricula'])*100),0)." %"
																										   );
									}
								}
							}
						}
					}
				}
			}
			if($arrTurmasCriticasReprovacao){
				if($arrTurmasCriticasReprovacao["U"]){
					foreach($arrTurmasCriticasReprovacao["U"] as $disciplina => $arrTurmas){
						echo "<tr><td class=bordapreto>".count($arrTurmas)." turma(s) do ensino fundamental aprensentou(aram) taxa de reprovação em ".$arrNomeDisciplina[$disciplina]." superior(es) à média do Brasil. <a href=\"javascript:exibeTurma('u_$disciplina')\" >Clique aqui para exibir a(s) turma(s).</td></tr>";
						$problema_da = true;
					}
				}
				if($arrTurmasCriticasReprovacao["M"]){
					foreach($arrTurmasCriticasReprovacao["M"] as $disciplina => $arrTurmas){
						echo "<tr><td class=bordapreto>".count($arrTurmas)." turma(s) do ensino médio aprensentou(aram) taxa de reprovação em ".$arrNomeDisciplina[$disciplina]." superior(es) à média do Brasil. <a href=\"javascript:exibeTurma('m_$disciplina')\" >Clique aqui para exibir a(s) turma(s).</td></tr>";
						$problema_da = true;
					}
				}
			}
			$arrRespEscola3 = recuperaRespostasEscola(null,"D","C",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')"));
			if($arrRespEscola3){
				foreach($arrRespEscola3 as $resp){
					echo "<tr><td class=bordapreto>".str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc'])."</td></tr>";
					$problema_da = true;
				}
			}
			if(!$problema_da) {
				echo "<tr><td class=bordapreto>A evolução desse(s) indicador(es) não apresentou problemas.</td></tr>";	
			}
			echo "</table>";
		?>
	</td>
	<td valign="top" >
		<?php
		echo "<table width=100% >"; 
		if($arrDistorcao1){
			 echo "<tr><td class='bordapreto center'><input type=\"checkbox\" ".(verificaTurmasCNE(array_unique($arrDistorcao1)) ? "checked='checked'" : "")." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[matricula]\" value=\"".implode(",",$arrDistorcao1)."\" /></td></tr>";
		}
		if($arrDistorcao2){
			 echo "<tr><td class='bordapreto center'><input type=\"checkbox\" ".(verificaCheckBoxTaxa("distorcao",array_unique($arrDistorcao2)) ? "checked='checked'" : "")." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[taxa][distorcao][".count($arrDistorcao2)."]\" value=\"".implode(",",$arrDistorcao2)."\" /></td></tr>";
		}
		if($arrRespEscola){
			foreach($arrRespEscola as $resp){ 
				 echo "<tr><td class='bordapreto center'>
				 	<input type=\"checkbox\" ".(verificaCheckBoxPergunta($resp['repid']) ? "checked='checked'" : "")." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[opcao][".$resp['repid']."]\" value=\"1\" />
					<input type=\"hidden\" name=\"arrRepid[".$resp['repid']."]\" value=\"".$resp['repid']."\" />
				 	</td>
				 </tr>";
			}
		}
		if($arrDistorcaoReprovacao){
			 echo "<tr><td class='bordapreto center'>
			 		<input type=\"checkbox\" ".(verificaCheckBoxTaxa("reprovacao",array_unique($arrDistorcaoReprovacao)) ? "checked='checked'" : "")." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[taxa][reprovacao][".count($arrDistorcaoReprovacao)."]\" value=\"".implode(",",$arrDistorcaoReprovacao)."\" />
					<input type=\"hidden\" name=\"arrTurmasTaxa[]\" value=\"".implode(",",$arrDistorcaoReprovacao)."\" />
					</td>
				 </tr>";
		}
		if($arrDistorcaoAbandono){
			 echo "<tr><td class='bordapreto center'>
			 		<input type=\"checkbox\" ".(verificaCheckBoxTaxa("abandono",array_unique($arrDistorcaoAbandono)) ? "checked='checked'" : "")." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[taxa][abandono][".count($arrDistorcaoAbandono)."]\" value=\"".implode(",",$arrDistorcaoAbandono)."\" />
					<input type=\"hidden\" name=\"arrTurmasTaxa[]\" value=\"".implode(",",$arrDistorcaoAbandono)."\" />
					</td>
				 </tr>";
		}
		if($arrRespEscola2){
			foreach($arrRespEscola2 as $resp){ 
				 echo "<tr><td class='bordapreto center'>
				 	<input type=\"checkbox\" ".(verificaCheckBoxPergunta($resp['repid']) ? "checked='checked'" : "")." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[opcao][".$resp['repid']."]\" value=\"1\" />
					<input type=\"hidden\" name=\"arrRepid[".$resp['repid']."]\" value=\"".$resp['repid']."\" />
				 	</td>
				 </tr>";
			}
		}
		if($arrTurmasCriticasReprovacao){
			if($arrTurmasCriticasReprovacao["U"]){
				foreach($arrTurmasCriticasReprovacao["U"] as $disciplina => $arrTurmas){
					 echo "<tr><td class='bordapreto center'>
						<input type=\"checkbox\" ".(verificaCheckBoxDisciplina($disciplina,$arrTurmas) ? "checked='checked'" : "")." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[disciplina][".$disciplina."][".count($arrTurmas)."]\" value=\"".implode(",",$arrTurmas)."\" />
						<input type=\"hidden\" name=\"arrTurmasDisciplina[]\" value=\"".implode(",",$arrTurmas)."\" />
						</td>
				 	</tr>";
				}
			}
			if($arrTurmasCriticasReprovacao["M"]){
				foreach($arrTurmasCriticasReprovacao["M"] as $disciplina => $arrTurmas){
					echo "<tr><td class='bordapreto center'>
						<input type=\"checkbox\" ".(verificaCheckBoxDisciplina($disciplina,$arrTurmas) ? "checked='checked'" : "")." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[disciplina][".$disciplina."][".count($arrTurmas)."]\" value=\"".implode(",",$arrTurmas)."\" />
						<input type=\"hidden\" name=\"arrTurmasDisciplina[]\" value=\"".implode(",",$arrTurmas)."\" />
						</td>
				 	</tr>";
				}
			}
		}
		if($arrRespEscola3){
			foreach($arrRespEscola3 as $resp){ 
				 echo "<tr><td class='bordapreto center'>
				 	<input type=\"checkbox\" ".(verificaCheckBoxPergunta($resp['repid']) ? "checked='checked'" : "")." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[opcao][".$resp['repid']."]\" value=\"1\" />
					<input type=\"hidden\" name=\"arrRepid[".$resp['repid']."]\" value=\"".$resp['repid']."\" />
				 	</td>
				 </tr>";
			}
		}
		echo "</table>";
		?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro bold direita" >Ensino e Aprendizagem</td>
	<td class="center" >
	<?php
	$problema_ea = false;
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and (prgdetalhe='Projeto Pedagógico - Educação Infantil' or prgdetalhe='Projeto Pedagógico')) AND
				  rp.oppid IN(5,6)";
	$pergs_projetospedagogicos = $db->carregar($sql);
	
	if($pergs_projetospedagogicos[0]) {
		foreach($pergs_projetospedagogicos as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasprojpedag[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	echo "<table width=100% >";
	if(count($problemasprojpedag) > 0) {
		foreach($problemasprojpedag as $indice => $projpedag) {
			echo "<tr><td class=bordapreto>".$projpedag."</td></tr>";
			$problema_ea = true;	
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, op.oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and (prgdetalhe='Currículo - Educação Infantil' or prgdetalhe='Currículo')) AND
				  rp.oppid IN(5,6)";
	
	$pergs_curriculo = $db->carregar($sql);
	
	
	if($pergs_curriculo[0]) {
		foreach($pergs_curriculo as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemascurriculo[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemascurriculo) > 0) {
		foreach($problemascurriculo as $indice => $curriculo) {
			echo "<tr><td class=bordapreto>".$curriculo."</td></tr>";
			$problema_ea = true;	
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'P' and prgstatus = 'A' and (prgdetalhe='Avaliações - Educação Infantil' or prgdetalhe='Avaliações')) AND
				  rp.oppid IN(5,6)";
	
	$pergs_avaliacoes = $db->carregar($sql);
	
	
	if($pergs_avaliacoes[0]) {
		foreach($pergs_avaliacoes as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasavaliacoes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemasavaliacoes) > 0) {
		foreach($problemasavaliacoes as $indice => $avaliacoes) {
			echo "<tr><td class=bordapreto>".$avaliacoes."</td></tr>";
			$problema_ea = true;	
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'E' and prgsubmodulo = 'T' and prgstatus = 'A' and prgdetalhe='Tempo de Aprendizagem') AND
				  rp.oppid IN(5,6)";
	
	$pergs_tempoaprendizagem = $db->carregar($sql);
	
	
	if($pergs_tempoaprendizagem[0]) {
		foreach($pergs_tempoaprendizagem as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemastempaprend[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemastempaprend) > 0) {
		foreach($problemastempaprend as $indice => $tempoaprendizagem) {
			echo "<tr><td class=bordapreto>".$tempoaprendizagem."</td></tr>";
			$problema_ea = true;	
		}
	}
	
	$sql = "SELECT * FROM pdeinterativo2013.respostatempoaprendizagem WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
	$respostatempoaprendizagem = $db->pegaLinha($sql);
	
	$_PORQUE = array("espacofisico" => "Não possui espaço físico",
					 "profissionais" => "Não dispoe de profissionais para coordenar as atividades",
					 "recursosmateriais" => "Não dispoe de recursos materiais",
					 "outro" => "Outro");
	if($respostatempoaprendizagem['rtacaso']=="N") {
		$rtaporque = explode(";",$respostatempoaprendizagem['rtaporque']);
		if($rtaporque) {
			
			foreach($rtaporque as $pq) {
				$probl[] = $_PORQUE[$pq];	
			}
			$problemajornadaampliada['rtaporque'] = "A escola não desenvolve ações de educação integral porque: ".implode(", ",$probl);
		}
	}
	
	if(count($problemajornadaampliada) > 0) {
		foreach($problemajornadaampliada as $indice => $jornadaampliada) {
			echo "<tr><td class=bordapreto>".$jornadaampliada."</td></tr>";
			$problema_ea = true;	
		}
	}
	
	if(!$problema_ea) {
		echo "<tr><td class=bordapreto>A evolução desse(s) indicador(es) não apresentou problemas.</td></tr>";	
	}
	echo "</table>";?>
	</td>
	<td valign="top" >
	<?php 
	echo "<table width=100% >";
	if(count($problemasprojpedag) > 0) {
		foreach($problemasprojpedag as $indice => $projpedag) {
			echo "<tr><td class='bordapreto center'><input type=hidden name=critico[".$indice."] value=\"FALSE\"><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=\"TRUE\"></td></tr>";
		}
	}
	if(count($problemascurriculo) > 0) {
		foreach($problemascurriculo as $indice => $curriculo) {
			echo "<tr><td class='bordapreto center'><input type=hidden name=critico[".$indice."] value=\"FALSE\"><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=\"TRUE\"></td></tr>";
		}
	}
	if(count($problemasavaliacoes) > 0) {
		foreach($problemasavaliacoes as $indice => $avaliacoes) {
			echo "<tr><td class='bordapreto center'><input type=hidden name=critico[".$indice."] value=\"FALSE\"><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=\"TRUE\"></td></tr>";
		}
	}
	if(count($problemastempaprend) > 0) {
		foreach($problemastempaprend as $indice => $tempoaprendizagem) {
			echo "<tr><td class='bordapreto center'><input type=hidden name=critico[".$indice."] value=\"FALSE\"><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=\"TRUE\"></td></tr>";
		}
	}
	if(count($problemajornadaampliada) > 0) {
		foreach($problemajornadaampliada as $indice => $jornadaampliada) {
			echo "<tr><td class='bordapreto center'><input type=hidden name=respostatempoaprendizagem[".$indice."critico] value=\"FALSE\"><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($respostatempoaprendizagem[$indice.'critico']=="t")?"checked":"")." name=respostatempoaprendizagem[".$indice."critico] value=\"TRUE\"></td></tr>";
		}
	}
	echo "</table>";
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro bold direita branco" >Gestão</td>
	<td class="center branco" >
	<?php
	$sql = "SELECT tp.tpeid, tp.tpedesc, count(p.pesid) as qtd FROM pdeinterativo2013.pessoa p 
			LEFT JOIN pdeinterativo2013.pessoatipoperfil ptp ON ptp.pesid = p.pesid 
			LEFT JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = ptp.pdeid 
			LEFT JOIN pdeinterativo2013.detalhepessoa dp ON dp.pesid = p.pesid 
			LEFT JOIN pdeinterativo2013.tipoperfil tp ON tp.tpeid = ptp.tpeid 
			WHERE pd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND dp.tenid IN(2,3) AND tp.tpeid IN(SELECT tpeid FROM pdeinterativo2013.perfilarea WHERE apeid IN('".APE_EQUIPEPEDAGOGICA."','".APE_DIRETOR."','".APE_VICEDIRETOR."','".APE_SECRETARIA."'))
			GROUP BY tp.tpeid, tp.tpedesc";
	
	$pessoas = $db->carregar($sql);
	
	if($pessoas[0]) {
		foreach($pessoas as $pes) {
			$sql = "SELECT p.pesid, p.critico FROM pdeinterativo2013.pessoa p 
					LEFT JOIN pdeinterativo2013.pessoatipoperfil ptp ON ptp.pesid = p.pesid
					LEFT JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = ptp.pdeid 
					LEFT JOIN pdeinterativo2013.detalhepessoa dp ON dp.pesid = p.pesid 
					WHERE pd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND dp.tenid IN(2,3) AND ptp.tpeid='".$pes['tpeid']."'";
			
			$p = $db->carregar($sql);
			unset($pesids);
			if($p[0]) {
				foreach($p as $po) {
					$pesids[] = $po['pesid'];
				}
			}
			$isCriticoP[implode(",",$pesids)] = (($po['critico']=="t")?TRUE:FALSE);
			$problemasdirecao1[implode(",",$pesids)] = "Há ".$pes['qtd']." ".$pes['tpedesc']." que não possui(em) graduação."; 
		}
	}
	
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'G' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Liderança') AND
				  rp.oppid IN(5,6)";
	
	$pergs_lideranca = $db->carregar($sql);

	if($pergs_lideranca[0]) {
		foreach($pergs_lideranca as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdirecao2[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'G' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Acompanhamento') AND
				  rp.oppid IN(5,6)";
	
	$pergs_acompanhamento = $db->carregar($sql);
	if($pergs_acompanhamento[0]) {
		foreach($pergs_acompanhamento as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdirecao2[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	echo "<table width=100%>";
	$problema_dim_4 = false;
	if(count($problemasdirecao1) > 0 || count($problemasdirecao2) > 0) {
		if($problemasdirecao1) {
			foreach($problemasdirecao1 as $indice => $prodirecao) {
				echo "<tr><td class=bordapreto>".$prodirecao."</td></tr>";
				$problema_dim_4 = true;	
			}
		}
		if($problemasdirecao2) {
			foreach($problemasdirecao2 as $indice => $prodirecao) {
				echo "<tr><td class=bordapreto>".$prodirecao."</td></tr>";
				$problema_dim_4 = true;	
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Planejamento') AND
				  rp.oppid IN(5,6)";
	
	$pergs_planejamento = $db->carregar($sql);
	
	if($pergs_planejamento[0]) {
		foreach($pergs_planejamento as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasprocessos[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Rotinas') AND
				  rp.oppid IN(5,6)";
	
	$pergs_rotinas = $db->carregar($sql);
	
	if($pergs_rotinas[0]) {
		foreach($pergs_rotinas as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasprocessos[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'G' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Normas e Regulamentos') AND
				  rp.oppid IN(5,6)";
	
	$pergs_normas = $db->carregar($sql);
	
	if($pergs_normas[0]) {
		foreach($pergs_normas as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasprocessos[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemasprocessos) > 0) {
		foreach($problemasprocessos as $indice => $processo) {
			echo "<tr><td class=bordapreto>".$processo."</td></tr>";
			$problema_dim_4 = true;	
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'G' and prgsubmodulo = 'F' and prgstatus = 'A' and prgdetalhe='Gestão Financeira') AND
				  rp.oppid IN(5,6)";
	
	$pergs_financeiras = $db->carregar($sql);
	
	if($pergs_financeiras[0]) {
		foreach($pergs_financeiras as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasfinancas[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	if(count($problemasfinancas) > 0) {
		foreach($problemasfinancas as $indice => $financa) {
			echo "<tr><td class=bordapreto>".$financa."</td></tr>";
			$problema_dim_4 = true;
		}
	}
	
	if(!$problema_dim_4) {
		echo "<tr><td class=bordapreto>A evolução desse(s) indicador(es) não apresentou problemas.</td></tr>";	
	}
	echo "</table>";
	?>
	</td>
	<td valign="top" >
	<?php
	echo "<table width=100%>";
	if(count($problemasdirecao1) > 0 || count($problemasdirecao2) > 0) {
		if($problemasdirecao1) {
			foreach($problemasdirecao1 as $indice => $prodirecao) {
				echo "<tr><td class='bordapreto center'><input type=hidden name=pessoas[".$indice."] value=FALSE><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCriticoP[$indice]==true)?"checked":"")." name=pessoas[".$indice."] value=TRUE></td></tr>";
			}
		}
		if($problemasdirecao2) {
			foreach($problemasdirecao2 as $indice => $prodirecao) {
				echo "<tr><td class='bordapreto center'><input type=hidden name=critico[".$indice."] value=FALSE><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=TRUE></td></tr>";
			}
		}
	}
	if(count($problemasprocessos) > 0) {
		foreach($problemasprocessos as $indice => $processo) {
			echo "<tr><td class='bordapreto center'><input type=hidden name=critico[".$indice."] value=FALSE><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=TRUE></td></tr>";
		}
	}
	if(count($problemasfinancas) > 0) {
		foreach($problemasfinancas as $indice => $financa) {
			echo "<tr><td class='bordapreto center'><input type=hidden name=critico[".$indice."] value=FALSE><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=TRUE></td></tr>";
		}
	}
	echo "</table>";
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro bold direita" >Comunidade Escolar</td>
	<td class="center" >
	<?php
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and (prgdetalhe='Compromisso' or prgdetalhe='Compromisso - Educação Infantil')) AND
				  rp.oppid IN(5,6)";
	
	$pergs_compromisso = $db->carregar($sql);
	
	if($pergs_compromisso[0]) {
		foreach($pergs_compromisso as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasestudantes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and (prgdetalhe='Protagonismo e Participação' or prgdetalhe='Protagonismo e Participação - Educação Infantil')) AND
				  rp.oppid IN(5,6)";
	
	$pergs_protagonismo = $db->carregar($sql);
	if($pergs_protagonismo[0]) {
		foreach($pergs_protagonismo as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasestudantes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and (prgdetalhe='Saúde e Bem-estar' or prgdetalhe='Saúde e Bem-estar - Educação Infantil')) AND
				  rp.oppid IN(5,6)";
	
	$pergs_saude = $db->carregar($sql);
	if($pergs_saude[0]) {
		foreach($pergs_saude as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasestudantes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'E' and prgstatus = 'A' and prgdetalhe='Segurança - Educação Infantil') AND
				  rp.oppid IN(8)";
	
	$pergs_saude = $db->carregar($sql);
	if($pergs_saude[0]) {
		foreach($pergs_saude as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasestudantes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$problema_dim_5 = false;
	echo "<table width=100%>";
	if(count($problemasestudantes) > 0) {
		foreach($problemasestudantes as $indice => $proestudante) {
			echo "<tr><td class=bordapreto>".$proestudante."</td></tr>";
			$problema_dim_5 = true;	
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Práticas') AND
				  rp.oppid IN(5,6)";
	
	$pergs_praticas = $db->carregar($sql);
	
	
	if($pergs_praticas[0]) {
		foreach($pergs_praticas as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdocentes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'D' and prgstatus = 'A' and prgdetalhe='Experiencia e Auto-Confiança') AND
				  rp.oppid IN(5,6)";
	
	$pergs_experiencia = $db->carregar($sql);
	
	if($pergs_experiencia[0]) {
		foreach($pergs_experiencia as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdocentes[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rd.rdoid FROM pdeinterativo2013.respostadocente rd 
			INNER JOIN pdeinterativo2013.respostadocenteformacao rf ON rf.rdoid = rd.rdoid 
			INNER JOIN educacenso_2010.tab_docente td ON td.pk_cod_docente = rd.pk_cod_docente 
			WHERE rd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND rdodeseja=FALSE AND rdostatus='A' 
			GROUP BY rd.rdoid";
	
	$docentesformacao = $db->carregarColuna($sql);
	
	if($docentesformacao) {
		$num = $db->pegaUm("SELECT COUNT(rdoid) as num FROM pdeinterativo2013.respostadocente WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND rdocritico=TRUE AND rdostatus='A'");
		if($num == count($docentesformacao)) {
			$isCritico[implode(",",$docentesformacao)] = true;
		}
		$problemasdocentes2[implode(",",$docentesformacao)] = count($docentesformacao)." docentes consideram que sua formação não é apropriada para ministrar a área de conhecimento/ disciplina que atuam. <a href=\"javascript:onclick=exibeTurma('docentes2');\" style=\"cursor:pointer;font-weight:bold\" >Clique aqui</a> para ver os nomes dos docentes.";
	}
	
	if(count($problemasdocentes) > 0 || count($problemasdocentes2) > 0) {
		if($problemasdocentes) {
			foreach($problemasdocentes as $indice => $prodocente) {
				echo "<tr><td class=bordapreto>".$prodocente."</td></tr>";
				$problema_dim_5 = true;	
			}
		}
		if($problemasdocentes2) {
			foreach($problemasdocentes2 as $indice => $prodocente2) {
				echo "<tr><td class=bordapreto>".$prodocente2."</td></tr>";
				$problema_dim_5 = true;	
			}
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Cooperação e Respeito') AND
				  rp.oppid IN(5,6)";
	
	$pergs_cooperacao = $db->carregar($sql);
	
	
	if($pergs_cooperacao[0]) {
		foreach($pergs_cooperacao as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdemaisprofissionais[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'P' and prgstatus = 'A' and prgdetalhe='Motivação') AND
				  rp.oppid IN(5,6)";
	
	$pergs_motivacao = $db->carregar($sql);
	
	if($pergs_motivacao[0]) {
		foreach($pergs_motivacao as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemasdemaisprofissionais[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	
	if(count($problemasdemaisprofissionais) > 0) {	
		foreach($problemasdemaisprofissionais as $indice => $dprofissionais) {
			echo "<tr><td class=bordapreto>".$dprofissionais."</td></tr>";
			$problema_dim_5 = true;	
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'C' and prgstatus = 'A' and prgdetalhe='Comunicação') AND
				  rp.oppid IN(5,6)";
	
	$pergs_comunicacao = $db->carregar($sql);
	
	if($pergs_comunicacao[0]) {
		foreach($pergs_comunicacao as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemaspaisecomunidade[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT rp.repid, pe.prgdesc, LOWER(op.oppdesc) as oppdesc, rp.critico FROM pdeinterativo2013.respostapergunta rp 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = rp.pdeid 
			INNER JOIN pdeinterativo2013.pergunta pe ON pe.prgid = rp.prgid 
			INNER JOIN pdeinterativo2013.opcaopergunta op ON op.oppid = rp.oppid
			WHERE rp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND 
				  rp.prgid IN(select prgid from pdeinterativo2013.pergunta where prgmodulo = 'C' and prgsubmodulo = 'C' and prgstatus = 'A' and prgdetalhe='Participação') AND
				  rp.oppid IN(5,6)";
	
	$pergs_participacao = $db->carregar($sql);
	
	if($pergs_participacao[0]) {
		foreach($pergs_participacao as $pergs) {
			$isCritico[$pergs['repid']] = (($pergs['critico']=="t")?TRUE:FALSE);
			$problemaspaisecomunidade[$pergs['repid']] = str_replace(array("(*)","?"), array($pergs['oppdesc'],"."), $pergs['prgdesc']);
		}
	}
	
	$sql = "SELECT * FROM pdeinterativo2013.respostapaiscomunidade WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
	$respostapaiscomunidade = $db->pegaLinha($sql);
	
	if($respostapaiscomunidade) {
		if($respostapaiscomunidade['rpcpossuiconcelho']=="f") {
			$problemaspaisecomunidade2['rpcpossuiconcelho'] = "A escola não possui Conselho Escolar";
		}
	}
	
	if(count($problemaspaisecomunidade) > 0 || count($problemaspaisecomunidade2) > 0) {
		if(count($problemaspaisecomunidade) > 0) {
			foreach($problemaspaisecomunidade as $indice => $paisecomunidade) {
				echo "<tr><td class=bordapreto>".$paisecomunidade."</td></tr>";
				$problema_dim_5 = true;	
			}
		}
		if(count($problemaspaisecomunidade2) > 0) {
			foreach($problemaspaisecomunidade2 as $indice => $paisecomunidade) {
				echo "<tr><td class=bordapreto>".$paisecomunidade."</td></tr>";
				$problema_dim_5 = true;	
			}
		}
	}
	
	if(!$problema_dim_5) {
		echo "<tr><td class=bordapreto>A evolução desse(s) indicador(es) não apresentou problemas.</td></tr>";	
	}
	echo "</table>";
	?>
	</td>
	<td valign="top" >
	<?php
	echo "<table width=100%>";
	if(count($problemasestudantes) > 0) {
		foreach($problemasestudantes as $indice => $proestudante) {
			echo "<tr><td class='bordapreto center'><input type=hidden name=critico[".$indice."] value=\"FALSE\"><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=\"TRUE\"></td></tr>";
		}
	}
	if(count($problemasdocentes) > 0 || count($problemasdocentes2) > 0) {
		if($problemasdocentes) {
			foreach($problemasdocentes as $indice => $prodocente) {
				echo "<tr><td class='bordapreto center'><input type=hidden name=critico[".$indice."] value=\"FALSE\"><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=\"TRUE\"></td></tr>";
			}
		}
		if($problemasdocentes2) {
			foreach($problemasdocentes2 as $indice => $prodocente2) {
				echo "<tr><td class='bordapreto center'><input type=hidden name=respostadocente[".$indice."] value=\"FALSE\"><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=respostadocente[".$indice."] value=\"TRUE\"></td></tr>";
			}
		}
	}
	if(count($problemasdemaisprofissionais) > 0) {	
		foreach($problemasdemaisprofissionais as $indice => $dprofissionais) {
			echo "<tr><td class='bordapreto center'><input type=hidden name=critico[".$indice."] value=\"FALSE\"><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=\"TRUE\"></td></tr>";
		}
	}
	if(count($problemaspaisecomunidade) > 0 || count($problemaspaisecomunidade2) > 0) {
		if(count($problemaspaisecomunidade) > 0) {
			foreach($problemaspaisecomunidade as $indice => $paisecomunidade) {
				echo "<tr><td class='bordapreto center'><input type=hidden name=critico[".$indice."] value=\"FALSE\"><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($isCritico[$indice]==true)?"checked":"")." name=critico[".$indice."] value=\"TRUE\"></td></tr>";
			}
		}
		if(count($problemaspaisecomunidade2) > 0) {
			foreach($problemaspaisecomunidade2 as $indice => $paisecomunidade) {
				echo "<tr><td class='bordapreto center'><input type=hidden name=respostapaiscomunidade[".$indice."critico] value=\"FALSE\"><input onclick=\"verificaCheckBox(this);\" type=checkbox ".(($respostapaiscomunidade[$indice.'critico'] == "t")?"checked":"")." name=respostapaiscomunidade[".$indice."critico] value=\"TRUE\"></td></tr>";
			}
		}
	}
	echo "</table>";
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro bold direita branco" >Infraestrutura</td>
	<td class="center branco" >
	<?php
	$arrInstalacoesNecessarias = recuperaInstalacoesNecessarias();
	echo "<table width=100%>";
	$problema_dim_6 = false;
	if($arrInstalacoesNecessarias){
		foreach($arrInstalacoesNecessarias as $infra){
			echo "<tr><td class=bordapreto >A escola não possui {$infra['ifidesc']}.</td></tr>";
			$problema_dim_6 = true;
		}
	}
	
	$arrInstalacoesInadequadas = recuperaInstalacoesInadequadas();
	if($arrInstalacoesInadequadas){
		foreach($arrInstalacoesInadequadas as $infra){
			echo "<tr><td class=bordapreto >A escola considera que as instalações do(a) {$infra['ifidesc']} estão inadequados(as).</td></tr>";
			$problema_dim_6 = true;
		}
	}
	
	$arrRespEscola1 = recuperaRespostasEscola(null,"I","I",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')"));
	if($arrRespEscola1){
		foreach($arrRespEscola1 as $resp){
			echo "<tr><td class=bordapreto >".str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc'])."</td></tr>";
			$problema_dim_6 = true;
		}
	}
	
	$arrEquipamentosRuins = recuperaEquipamentosRuins();
	if($arrEquipamentosRuins){
		foreach($arrEquipamentosRuins as $equip){
			echo "<tr><td class=bordapreto >A escola considera que o estado de conservação de ".number_format($equip['rmeqtdruin'],"",2,".")." {$equip['tmedesc']} é/são ruim(ns).</td></tr>";
			$problema_dim_6 = true;
		}
	}
	
	$arrRespEscola2 = recuperaRespostasEscola(null,"I","E",null,array("(op.oppdesc ilike 'Nunca' or op.oppdesc ilike 'Raramente')"));
	if($arrRespEscola2){
		foreach($arrRespEscola2 as $resp){
			echo "<tr><td class=bordapreto >".str_replace( array("(*)","?") , array(strtolower($resp['oppdesc']),".") ,$resp['prgdesc'])."</td></tr>";
			$problema_dim_6 = true;
		}
	}
	
	if(!$problema_dim_6) {
		echo "<tr><td class=bordapreto>A evolução desse(s) indicador(es) não apresentou problemas.</td></tr>";	
	}
	echo "</table>";
	?>
	</td>
	<td valign="top" >
	<?php
	echo "<table width=100%>";
	if($arrInstalacoesNecessarias){
		foreach($arrInstalacoesNecessarias as $infra){
			echo "<tr><td class='bordapreto center' >
			<input type=\"checkbox\" ".($infra['rifcritico'] == "t" ? "checked='checked'" : "")." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[instalacao][".$infra['ifiid']."]\" value=\"1\" />
			</td></tr>";
		}
	}
	if($arrInstalacoesInadequadas){
		foreach($arrInstalacoesInadequadas as $infra){
			echo "<tr><td class='bordapreto center' >
			<input type=\"checkbox\" ".($infra['rifcritico'] == "t" ? "checked='checked'" : "" )." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[instalacao][".$infra['ifiid']."]\" value=\"1\" />
			</td></tr>";
		}
	}
	if($arrRespEscola1){
		foreach($arrRespEscola1 as $resp){
			echo "<tr><td class='bordapreto center' >
			<input type=\"checkbox\" ".(verificaCheckBoxPergunta($resp['repid']) ? "checked='checked'" : "")." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[opcao][".$resp['repid']."]\" value=\"1\" />
			<input type=\"hidden\" name=\"arrRepid[".$resp['repid']."]\" value=\"".$resp['repid']."\" />
			</td></tr>";
		}
	}
	if($arrEquipamentosRuins){
		foreach($arrEquipamentosRuins as $equip){
			echo "<tr><td class='bordapreto center' >
			<input type=\"checkbox\" ".($equip['remcritico'] == "t" ? "checked='checked'" : "" )." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[equipamento][".$equip['tmeid']."]\" value=\"1\" />
			</td></tr>";
		}
	}
	if($arrRespEscola2){
		foreach($arrRespEscola2 as $resp){
			echo "<tr><td class='bordapreto center' >
			<input type=\"checkbox\" ".(verificaCheckBoxPergunta($resp['repid']) ? "checked='checked'" : "")." onclick=\"verificaCheckBox(this)\" name=\"chk_problemas[opcao][".$resp['repid']."]\" value=\"1\" />
			<input type=\"hidden\" name=\"arrRepid[".$resp['repid']."]\" value=\"".$resp['repid']."\" />
			</td></tr>";
		}
	}
	echo "</table>";
	?>
	</td>
</tr>
</table>
</form>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
<tr>
	<td class="SubTituloDireita" width="20%">O que deseja fazer agora?</td>
	<td>
		<input type="button" name="btn_anterior" value="Anterior" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_0_orientacoes';" >
		<input type="button" name="btn_continuar" value="Salvar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_1_grandesdesafios';salvarPriorizacaoProblemas();">
		<input type="button" name="btn_continuar" value="Salvar e Continuar" onclick="document.getElementById('togo').value='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_2_planoacao';salvarPriorizacaoProblemas('continuar');">
		<input type="button" name="btn_proximo" value="Próximo" onclick="divCarregando();window.location='/pdeinterativo2013/pdeinterativo2013.php?modulo=principal/planoestrategico&acao=A&aba=planoestrategico_0_pdeescola&aba1=planoestrategico_0_2_planoacao';" >
	</td>
</tr>
</table>
<style>
	.PopUphidden{display:none;width:500px;height:300px;position:absolute;z-index:0;top:50%;left:50%;margin-top:-150;margin-left:-250;border:solid 2px black;background-color:#FFFFFF;}
</style>
<?php if($arrTurmasC["U"]): ?>
	<?php foreach($arrTurmasC["U"] as $cod_disciplina => $arrTurma): ?>
		<div class="PopUphidden" id="<?php echo "u_$cod_disciplina" ?>">
			<div style="width:100%;text-align:right">
				<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo "u_$cod_disciplina" ?>').style.display='none'" />
			</div>
			<div style="padding:5px;overflow:auto;width:95%;height:90%" >	
				<center><b>Ensino Fundamental</b></center><br />	
				<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center" >
					<thead>
						<tr>
							<td class="bold center" >Série / Ano</td>
							<td class="bold center" >Horário</td>
							<td class="bold center" >Turma(s)</td>
							<td class="bold center" >Disciplina(s)</td>
							<td class="bold center" >Taxa de Reprovação da Disciplina (em %)</td>
							<td class="bold center" >Taxa de Reprovação do Brasil (em %)</td>
						</tr>
					</thead>
				<?php $x=0;foreach($arrTurma as $turma): ?>
					<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
					<tr bgcolor="<?php echo $cor ?>" >
						<td><?php echo $turma['serie'] ?></td>
						<td><?php echo $turma['horario'] ?></td>
						<td><?php echo $turma['turma'] ?></td>
						<td><?php echo $turma['disciplina'] ?></td>
						<td><?php echo $turma['taxa'] ?></td>
						<td><?php echo $turma['taxaBrasil'] ?></td>
					</tr>
				<?php $x++;endforeach; ?>
				</table>
			</div>
		</div>
	<?php endforeach; ?>
<?php endif; ?>
<?php if($arrTurmasC["M"]): ?>
	<?php foreach($arrTurmasC["M"] as $cod_disciplina => $arrTurma): ?>
		<div class="PopUphidden" id="<?php echo "m_$cod_disciplina" ?>">
			<div style="width:100%;text-align:right">
				<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo "m_$cod_disciplina" ?>').style.display='none'" />
			</div>
			<div style="padding:5px;overflow:auto;width:95%;height:90%" >
				<center><b>Ensino Médio</b></center><br />	
				<table class="listagem" cellSpacing="1" width="100%" cellPadding="3" align="center" >
					<thead>
						<tr>
							<td class="bold center" >Série / Ano</td>
							<td class="bold center" >Horário</td>
							<td class="bold center" >Turma(s)</td>
							<td class="bold center" >Disciplina(s)</td>
							<td class="bold center" >Taxa de Reprovação da Disciplina (em %)</td>
							<td class="bold center" >Taxa de Reprovação do Brasil (em %)</td>
						</tr>
					</thead>
				<?php $x=0;foreach($arrTurma as $turma): ?>
					<?php $cor = $x%2==0 ? "#FFFFFF" : "#F5F5F5" ?>
					<tr bgcolor="<?php echo $cor ?>" >
						<td><?php echo $turma['serie'] ?></td>
						<td><?php echo $turma['horario'] ?></td>
						<td><?php echo $turma['turma'] ?></td>
						<td><?php echo $turma['disciplina'] ?></td>
						<td><?php echo $turma['taxa'] ?></td>
						<td><?php echo $turma['taxaBrasil'] ?></td>
					</tr>
				<?php $x++;endforeach; ?>
				</table>
			</div>
		</div>
	<?php endforeach; ?>
<?php endif; ?>
<?php endif; ?>