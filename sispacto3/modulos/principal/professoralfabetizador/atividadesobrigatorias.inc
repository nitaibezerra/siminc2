<? 
$_REQUEST['fpbid'] = preg_replace("/[^0-9]/", "", $_REQUEST['fpbid']);
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td colspan="2" class="SubTituloCentro">
	<?
	carregarPeriodoReferencia(array('uncid' => $_SESSION['sispacto3']['professoralfabetizador']['uncid'], 'fpbid' => $_REQUEST['fpbid'],'todosmeses' => true, 'pflcod_avaliador' => PFL_PROFESSORALFABETIZADOR));
	?>
	</td>
</tr>
</table>
<?
if($_REQUEST['fpbid']) :

	if(!$_SESSION['sispacto3']['professoralfabetizador']['uncid'] || !$_SESSION['sispacto3']['professoralfabetizador']['iusd']) {
		$al = array("alert"=>"Problemas para acessar essa tela, por favor faça o LOGOUT e acesse novamente.","location"=>"sispacto3.php?modulo=inicio&acao=C");
		alertlocation($al);		
	}

	
	$sql = "SELECT rfuparcela FROM sispacto3.folhapagamentouniversidade WHERE fpbid='".$_REQUEST['fpbid']."' AND pflcod='".PFL_PROFESSORALFABETIZADOR."' AND uncid='".$_SESSION['sispacto3']['professoralfabetizador']['uncid']."'";
	$rfuparcela = $db->pegaUm($sql);
	
	switch($rfuparcela) {
		case '1':
			include_once APPRAIZ_SISPACTO."/professoralfabetizador/dadosturmas.inc";
			break;
		case '2':
			$cattipo = "P";
			include_once APPRAIZ_SISPACTO."/professoralfabetizador/aprendizagemturma.inc";
			break;
		case '3':
			$cattipo = "M";
			include_once APPRAIZ_SISPACTO."/professoralfabetizador/aprendizagemturma.inc";
			break;
		case '4':
			echo '<p align=center>[ATIVIDADE NÃO IMPLEMENTADA]</p>';
			//include_once APPRAIZ_SISPACTO."/professoralfabetizador/materiais.inc";
			break;
		case '5':
			include_once APPRAIZ_SISPACTO."/professoralfabetizador/utilizacaorecursosdidaticos.inc";
			break;
		case '6':
			
			$es = estruturaRelatoExperiencia(array());
			
			$titulo 		 = 'Relato de experiência';
			$perguntainicial = 'Você tem certeza de que a experiência contribui para a aquisição da proficiência na escrita dos estudantes?';
			
			echo '<form method="post" id="formulario" enctype="multipart/form-data">';
			echo '<input type="hidden" name="requisicao" value="gravarRelatoExperiencia">';
			
			$relatoexperiencia = $db->pegaLinha("SELECT * FROM sispacto3.relatoexperiencia WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'");
			if($relatoexperiencia) extract($relatoexperiencia);
				
			
			include_once APPRAIZ_SISPACTO."/professoralfabetizador/montarQuestionario.inc";
			
			echo '</form>';
			
			break;
			
		case '7':
			
			$turmasprofessores = pegarTurmasProfessores(array('iusd' => $_SESSION['sispacto3']['professoralfabetizador']['iusd'],'tpastatus' => 'A','tpaconfirmaregencia' => 'TRUE'));
			
			if(!$turmasprofessores[0]) {
				$al = array("alert"=>"Não existem turmas regenciadas por você. Acesse o 1º mês de referência e defina as turmas regentes","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias");
				alertlocation($al);
			}
			
			echo '<form method="post" id="formulario" enctype="multipart/form-data">';
			echo '<input type="hidden" name="requisicao" value="gravarImpressoesANA">';
			
			$es = estruturaImpressaoANA(array());
				
			foreach($turmasprofessores as $turma) {
			 	
				if(($turma['tpatotalmeninos']+$turma['tpatotalmeninas'])==0) {
					$al = array("alert"=>"Não foram definidos valores da turma selecionada. Acesse o 1º mês de referência para realizar as definições.","location"=>"sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias");
					alertlocation($al);
				}
				
				echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';				
	
				echo '<tr>';
				echo '<td class="SubTituloDireita">UF / Município : </td>';
				echo '<td>'.$turma['estuf'].' / '.$turma['mundescricao'].'</td>';
				echo '</tr>';
				echo '<tr>';
				echo '<td class="SubTituloDireita">INEP / Escola : </td>';
				echo '<td>'.$turma['tpacodigoescola'].' / '.$turma['tpanomeescola'].'</td>';
				echo '</tr>';
				echo '<tr>';
				echo '<td class="SubTituloDireita">Turma : </td>';
				echo '<td>'.$turma['tpanometurma'].' / '.$turma['tpahorarioinicioturma'].' - '.$turma['tpahorariofimturma'].'</td>';
				echo '</tr>';
				
				echo '<tr>';
				echo '<td colspan=2>';

				echo '<input type=hidden name=tpaid[] value='.$turma['tpaid'].'>';
				
				$impressoesana = $db->pegaLinha("SELECT * FROM sispacto3.impressoesana WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."' AND tpaid='".$turma['tpaid']."'");
				if($impressoesana) extract($impressoesana);
				
				$htmlidmaster = '['.$turma['tpaid'].']';
				
				include APPRAIZ_SISPACTO."/professoralfabetizador/montarQuestionario.inc";
				
				unset($imaparticiparam,
					$imaacessoresultados,
					$imaresultadosescola,
					$imaaspectosorientacoes,
					$imaaspectostempoaplicacao,
					$imaaspectoshorarioaplicacao,
					$imaaspectosquantidadequestoes,
					$imaaspectosclarezaquestoes,
					$imaaspectosaplicadorexterno,
					$imaaspectoslocalavaliacao,
					$imaaspectosapresentacaoavaliacao,
					$imaaspectosapresentacaoresultados,
					$imafatoresgestaoescolar,
					$imafatoresformacaoprofessores,
					$imafatorespraticaspedagogicas,
					$imafatoresperfilalunos,
					$imafatoresrecursosdidaticos,
					$imafatoresestruturafisica,
					$imafatoresparticipacaofamilia,
					$imafatoresrelacoesinterpessoais);
				
				echo '</td>';
				echo '</tr>';
				
				echo '</table>';
			
			}
			
			echo '</form>';
				
			
			
			break;
			
		case '8':
					
			$es = estruturaQuestoesDiversas(array());
				
			$titulo 		 = 'Atividade 8';
			$perguntainicial = '';
				
			echo '<form method="post" id="formulario" enctype="multipart/form-data">';
			echo '<input type="hidden" name="requisicao" value="gravarQuestoesDiversas">';
				
			$questoesdiversasatv8 = $db->pegaLinha("SELECT * FROM sispacto3.questoesdiversasatv8 WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'");
			if($questoesdiversasatv8) extract($questoesdiversasatv8);
		
				
			include_once APPRAIZ_SISPACTO."/professoralfabetizador/montarQuestionario.inc";
				
			echo '</form>';
				
			break;
			
		case '9':
			
			if(!$_REQUEST['cattipo']) $_REQUEST['cattipo'] = "M";
			
			$menu[] = array("id" => "1", "descricao" => "Matemática", "link" => "/sispacto3/sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$_REQUEST['fpbid']."&cattipo=M");
			$menu[] = array("id" => "2", "descricao" => "Português", "link" => "/sispacto3/sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$_REQUEST['fpbid']."&cattipo=P");
			
			echo "<br>";
			
			$abaativa = "/sispacto3/sispacto3.php?modulo=principal/professoralfabetizador/professoralfabetizador&acao=A&aba=atividadesobrigatorias&fpbid=".$_REQUEST['fpbid']."&cattipo=".$_REQUEST['cattipo'];
			
			echo montarAbasArray($menu, $abaativa);
			
			$cattipo = $_REQUEST['cattipo'];
			include_once APPRAIZ_SISPACTO."/professoralfabetizador/aprendizagemturma2.inc";
			break;
			
			
		case '10':
					
			echo '<form method="post" id="formulario" enctype="multipart/form-data">';
			echo '<input type="hidden" name="requisicao" value="gravarContribuicaoPacto">';
			
			$titulo = "Avaliação do Pacto Nacional pela Alfabetização na Idade Certa";
					
			$es = estruturaContribuicaoPacto(array());
			
			$contribuicaopacto = $db->pegaLinha("SELECT * FROM sispacto3.contribuicaopacto WHERE iusd='".$_SESSION['sispacto3']['professoralfabetizador']['iusd']."'");
			if($contribuicaopacto) extract($contribuicaopacto);
				
		
			include APPRAIZ_SISPACTO."/professoralfabetizador/montarQuestionario.inc";
					
				
			echo '</form>';
		
				
				
			break;
					
					
					
		default:
			echo "<p align=center><b>Atividade ainda não foi disponibilizada para o mês de referência.</b></p>"; 
					
	}

endif;

?>