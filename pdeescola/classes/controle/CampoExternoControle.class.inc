<?php

class CampoExternoControle{

	private $htm;
	private static $camposDefault;

	//Função obrigatória. Nessa função deverá ser montado a questão que aparecerá dentro do campo externo. Deverá ser colocada dentro do $this->htm.
	public function montaNovoCampo( $perid, $qrpid, $percent = 90 ){
		global $db;

		if($perid == 3466){ //3644 => 3466 =>3575 => 3466 
			$rep = carregarNomeCoordenador();
			$obQestionario = new QQuestionarioResposta();
			$queid = $obQestionario->pegaQuestionario( $qrpid );
			$habilitado = buscaPermissaoAcesso();
			$grid = carregarDuracaoAtividades($perid,$qrpid,$queid);
			$total = $grid['0']['resdesc'] + $grid['1']['resdesc'] + $grid['2']['resdesc'] + $grid['3']['resdesc'] + $grid['4']['resdesc'];
			
			$htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
			$htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";
			$htm .= "<input type='hidden' name='queid' id='queid' value='{$queid}'>";
			$htm .= "<input type='hidden' name='identExterno' id='identExterno' value='1'><center>";			
			$htm .= "<table id=\"atividade_desenvolvida\" name=\"atividade_desenvolvida\" border=\"0\" class=\"tabela\" style=\"width:100%;\">";
			$htm .= "<tr>";
			$htm .= "<th colspan=\"5\">QUANTIDADE HORA  POR DIA DA SEMANA</th>";
			$htm .= "<th width=\"12%\" rowspan=\"2\">TOTAL</th>";
			$htm .= "</tr>";
			$htm .= "<tr>";
			
			
			//$htm .= "<th width=\"28%\">DIAS DA SEMANA</th>";
			$htm .= "<th width=\"12%\">2a. Feira</th>";
			$htm .= "<th width=\"12%\">3a. Feira</th>";
			$htm .= "<th width=\"12%\">4a. Feira</th>";
			$htm .= "<th width=\"12%\">5a. Feira</th>";
			$htm .= "<th width=\"12%\">6a. Feira</th>";
			//$htm .= "<th width=\"12%\">TOTAL</th>";
			$htm .= "</tr>";	
			$htm .= "<tr>";
			//$htm .= "<td>Duração das Atividades (em horas)</td>";
			$htm .= "<td align=\"center\"><input id='semana2' type='text' class='CampoEstilo' name='semana2' size='5' value='{$grid['0']['resdesc']}'></td>";
			$htm .= "<td align=\"center\"><input id='semana3' type='text' class='CampoEstilo' name='semana3' size='5' value='{$grid['1']['resdesc']}'></td>";
			$htm .= "<td align=\"center\"><input id='semana4' type='text' class='CampoEstilo' name='semana4' size='5' value='{$grid['2']['resdesc']}'></td>";
			$htm .= "<td align=\"center\"><input id='semana5' type='text' class='CampoEstilo' name='semana5' size='5' value='{$grid['3']['resdesc']}'></td>";
			$htm .= "<td align=\"center\"><input id='semana6' type='text' class='CampoEstilo' name='semana6' size='5' value='{$grid['4']['resdesc']}'></td>";
			$htm .= "<td align=\"center\"><input id='total' type='text' class='CampoEstilo' name='total' size='5' value='{$total}'></td>";
			$htm .= "</tr>";	
			$htm .= "</table>";		
					
			
		} elseif($perid == 3465){ //3464 => 3465
			$rep = carregarNomeCoordenador();
			$obQestionario = new QQuestionarioResposta();
			$queid = $obQestionario->pegaQuestionario( $qrpid );
			$habilitado = buscaPermissaoAcesso();
			$dados_grid = carregarAtividadesDesenvolvidas($qrpid);
			
			$htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
			$htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";
			$htm .= "<input type='hidden' name='queid' id='queid' value='{$queid}'>";
			$htm .= "<input type='hidden' name='identExterno' id='identExterno' value='1'><center>";
			$htm .= "<table id=\"atividade_desenvolvida\" name=\"atividade_desenvolvida\" class=\"tabela\" style=\"width:100%;\">";
			$htm .= "<tr>";
			$htm .= "<th width=\"26%\" rowspan=\"2\">Macrocampo/Atividade</th>";
			$htm .= "<th colspan=\"9\"width=\"50%\">Ensino Fundamental </th>";
			$htm .= "<th width=\"10%\" rowspan=\"2\">Total de Alunos</th>";
			$htm .= "<th width=\"14%\" rowspan=\"2\">Iniciou em 2012 </th>";
			$htm .= "</tr>";
			$htm .= "<tr>";
			$htm .= "<th>1&ordm;</th>";
			$htm .= "<th>2&ordm;</th>";
			$htm .= "<th>3&ordm;</th>";
			$htm .= "<th>4&ordm;</th>";
			$htm .= "<th>5&ordm;</th>";
			$htm .= "<th>6&ordm;</th>";
			$htm .= "<th>7&ordm;</th>";
			$htm .= "<th>8&ordm;</th>";
			$htm .= "<th>9&ordm;</th>";
			$htm .= "</tr>";
			$htm .= "</tr>";
			//ver($dados_grid, d);
			$c = 0;
			if($dados_grid){
				foreach($dados_grid as $grid){
					if( ($c % 2) == 0 ){
						$cor = "#FFFAFA";
					}else{
						$cor = "#D3D3D3";
					}
					
					$total = $grid['0'] + $grid['1'] + $grid['2'] + $grid['3'] + $grid['4'] + $grid['5'] + $grid['6'] + $grid['7'] + $grid['8'];
					
					$htm .= '<tr style="background-color:'.$cor.';">';
					$htm .= "<td>".$grid['11']." / ".$grid['12']."</td>";
					$htm .= "<td align=\"center\">".$grid['0']."</td>";
					$htm .= "<td align=\"center\">".$grid['1']."</td>";
					$htm .= "<td align=\"center\">".$grid['2']."</td>";
					$htm .= "<td align=\"center\">".$grid['3']."</td>";
					$htm .= "<td align=\"center\">".$grid['4']."</td>";
					$htm .= "<td align=\"center\">".$grid['5']."</td>";
					$htm .= "<td align=\"center\">".$grid['6']."</td>";
					$htm .= "<td align=\"center\">".$grid['7']."</td>";
					$htm .= "<td align=\"center\">".$grid['8']."</td>";
					$htm .= "<td align=\"center\">".$total."</td>";
					$htm .= "<td>";
					$htm .= "<input id='inicio{$c}' type='radio' name='inicio{$c}' value='S' ".(($grid['13'] == 'S' ) ? 'checked="checked"' : '').">Sim";
					$htm .= "<input id='inicio{$c}' type='radio' name='inicio{$c}' value='N' ".(($grid['13'] == 'N' ) ? 'checked="checked"' : '').">Não";					
					$htm .= "<input id='atividade{$c}' type='hidden' name='atividade{$c}' value='{$grid['9']}'>";
					$htm .= "<input id='macrocampo{$c}' type='hidden' name='macrocampo{$c}' value='{$grid['10']}'>";
					$htm .= "</td>";
					$htm .= "</tr>";
					$c = $c + 1;
				}
			}
			$htm .= "<input id='quantidade' type='hidden' name='quantidade' value='{$c}'>";
			$htm .= "</table>";
			
		}elseif( $perid == 3464 ){ //3460 => 3464
			
			$rep = carregarNomeCoordenador();
			$obQestionario = new QQuestionarioResposta();
			$queid = $obQestionario->pegaQuestionario( $qrpid );
			$dados = carregarRespostaRecursosPDDE($qrpid, $perid);
			$valor_1 = carregarValorPagoPDDE();
			if(!$valor_1 || $valor_1 == 0 || $valor_1 == ',00') $valor_1 = '0,00';
			$valor_2 = carregarSaldoPDDE();
			if(!$valor_2 || $valor_2 == 0 || $valor_2 == ',00') $valor_2 = '0,00';
			$habilitado = buscaPermissaoAcesso();
			
			if(!empty($dados)){
				
				foreach($dados as $d){
					/*
					if($d['qeord'] == '21'){
						$valor_2 = $d['valor'];					
					}
					*/
					if($d['qeord'] == '31'){
						$valor_3 = $d['valor'];					
					}
					
					if($d['qeord'] == '41'){
						$valor_4 = $d['valor'];					
					}
					/*
					if($d['qeord'] == '51'){
						$valor_5 = $d['valor'];					
					}
					*/
				}
				
				if($valor_3) $valor_3 = number_format($valor_3,2,",",".");
				if($valor_4) $valor_4 = number_format($valor_4,2,",",".");
				
			}
			
			$htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
			$htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";
			$htm .= "<input type='hidden' name='queid' id='queid' value='{$queid}'>";
			$htm .= "<input type='hidden' name='identExterno' id='identExterno' value='1'><center>";
			
			$htm .= "<table id=\"recurdo_pdde\" name=\"recurdo_pdde\" class=\"tabela\" style=\"width:60%;\">";
			$htm .= "<tr>";
			$htm .= "<th width=\"25%\">Ano</th>";
			$htm .= "<th width=\"75%\" colspan=\"2\">2012</th>";
			$htm .= "</tr>";
			$htm .= "<tr>";
			$htm .= "<th rowspan=\"5\">Descri&ccedil;&atilde;o de Valores</th>";
			$htm .= "<th width=\"30%\" style=\"text-align:left;\">Valor pago </th>";
			$htm .= "<td width=\"45%\" align=\"right\">".campo_texto('valor_1','S','N','',24,14,'[.###],##','', 'right', '', '', 'id="valor_1"', '', $valor_1)."</td>";
			$htm .= "</tr>";
			$htm .= "<tr>";
			$htm .= "<th style=\"text-align:left;\">Saldo 31.12.2012</th>";
			$htm .= "<td readonly align=\"right\">".campo_texto('valor_2','S','N','',24,14,'[.###],##','', 'right', '', '', 'id="valor_2"', '', $valor_2)."</td>";
			$htm .= "</tr>";				
			$htm .= "<tr>";
			$htm .= "<th style=\"text-align:left;\">Valor Capital </th>";
			$htm .= "<td align=\"right\">".campo_texto('valor_3','S',$habilitado,'',24,14,'[.###],##','', 'right', '', '', 'id="valor_3"', 'calcularCapitalCusteio();', $valor_3)."</td>";
			$htm .= "</tr>";
			$htm .= "<tr>";
			$htm .= "<th style=\"text-align:left;\">Valor Custeio</th>";
			$htm .= "<td align=\"right\">".campo_texto('valor_4','S',$habilitado,'',24,14,'[.###],##','', 'right', '', '', 'id="valor_4"', 'calcularCapitalCusteio();', $valor_4)."</td>";
			$htm .= "</tr>";
			$htm .= "<tr>";
			//$htm .= "<th style=\"text-align:left;\">Saldo Atual</th>";
			//$htm .= "<td align=\"right\">".campo_texto('valor_5','S','N','',24,14,'[.###],##','', 'right', '', '', 'id="valor_5"', '', $valor_5)."</td>";
			//$htm .= "</tr>";
			$htm .= "</table>";
		
		}elseif( $perid == 3516 ){ //3335 => 3224 => 3516
			$rep = carregarNomeCoordenador();
			
			$obQestionario = new QQuestionarioResposta();
				
			$queid = $obQestionario->pegaQuestionario( $qrpid );

			$dados = carregarResposta($qrpid, $perid);	

			$htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
			$htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";
			$htm .= "<input type='hidden' name='queid' id='queid' value='{$queid}'>";
			$htm .= "<input type='hidden' name='identExterno' id='identExterno' value='1'>";
			
			$htm .= "<table id=\"cooperacao\" name=\"cooperacao\" class=\"tabela\" style=\"width: 100%;\">";
			$htm .= "<tr>";
			$htm .= "<th style=\"width: 35%;\">Função</th><th style=\"width: 45%;\">Nome Completo</th><th style=\"width: 20%;\">Segmento representativo</th>";
			$htm .= "</tr>";
			
			$htm .= "<tr>";
			$htm .= "<th style=\"text-align:left;\">Coordenador Geral</th>";
			$htm .= "<td><b align=\"center\">$rep</b></td>";
			$htm .= "<td>&nbsp;</td>";
			$htm .= "</tr>";
			
			#Primeira Linha
			$htm .= "<tr>";
				$htm .= "<th style=\"text-align:left;\">Repres. Pedagógico</th>";
				if($dados[0]['resdesc'] != '' && $dados[0]['qeord'] == 11 ){
					$htm .= "<td><input type='text' class='CampoEstilo' name='representante_1' size='45' value='".$dados[0]['resdesc']."'></td>";
				}else{
					$htm .= "<td><input type='text' class='CampoEstilo' name='representante_1' size='45'></td>";
				}
				$htm .= "<td id='representantepedagogico'>";
				$htm .= "<select class='CampoEstilo' style=\"width:145;\" name='seguimento_1' id='seguimento_1' onfocus=\"atualizaComboRepresentante(this.id)\">";
				if($dados[1]['resdesc'] != '' && $dados[1]['qeord'] == 12 ){
					$htm .= "<option value='".$dados[1]['resdec_cod']."' selected>".$dados[1]['resdesc']."</option>";
				}else{
					$htm .= "<option value='0' selected>Selecione...</option>";
				}
				$htm .= "</select>";
				$htm .= "</td>";
			$htm .= "</tr>";
			
			#Segunda Linha
			$htm .= "<tr>";
				$htm .= "<th style=\"text-align:left;\">Repres. de assuntos comunitários</th>";
				if($dados[2]['resdesc'] != '' && $dados[2]['qeord'] == 21 ){
					$htm .= "<td><input type='text' class='CampoEstilo' name='representante_2' size='45' value='".$dados[2]['resdesc']."'></td>";
				}else{
					$htm .= "<td><input type='text' class='CampoEstilo' name='representante_2' size='45'></td>";
				}
				$htm .= "<td id='representanteassuntocomunitario' align='right'>";
				$htm .= "<select class='CampoEstilo' style=\"width:145;\" name='seguimento_2' id='seguimento_2' onfocus=\"atualizaComboRepresentante(this.id)\">";
				if($dados[3]['resdesc'] != '' && $dados[3]['qeord'] == 22 ){
					$htm .= "<option value='".$dados[3]['resdec_cod']."' selected>".$dados[3]['resdesc']."</option>";
				}else{
					$htm .= "<option value='0' selected>Selecione...</option>";
				}
				$htm .= "</select>";
				$htm .= "</td>";
			$htm .= "</tr>";
			
			#terceira Linha
			$htm .= "<tr>";
				$htm .= "<th style=\"text-align:left;\">Repres. para Execução Financeira</th>";
				if($dados[4]['resdesc'] != '' && $dados[4]['qeord'] == 31 ){
					$htm .= "<td><input type='text' class='CampoEstilo' name='representante_3' size='45' value='".$dados[4]['resdesc']."'></td>";
				}else{
					$htm .= "<td><input type='text' class='CampoEstilo' name='representante_3' size='45'></td>";
				}
				$htm .= "<td id='representanteexecucaofinanceira' align='right'>";
				$htm .= "<select class='CampoEstilo' style=\"width:145;\" name='seguimento_3' id='seguimento_3' onfocus=\"atualizaComboRepresentante(this.id)\">";
				if($dados[5]['resdesc'] != '' && $dados[5]['qeord'] == 32 ){
					$htm .= "<option value='".$dados[5]['resdec_cod']."' selected>".$dados[5]['resdesc']."</option>";
				}else{
					$htm .= "<option value='0' selected>Selecione...</option>";
				}
				$htm .= "</select>";
				$htm .= "</td>";
			$htm .= "</tr>";
			
			$htm .= "</table>";
		
		}elseif( $perid == 3305 ){
			
			$obQestionario = new QQuestionarioResposta();
			
			$queid = $obQestionario->pegaQuestionario( $qrpid );
			
			$dados = carregarResposta($qrpid, $perid);
			
			$htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
			$htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";
			$htm .= "<input type='hidden' name='queid' id='queid' value='{$queid}'>";
			$htm .= "<input type='hidden' name='identExterno' id='identExterno' value='1'>";
			
			$htm .= " <table class=\"tabela\" border=\"0\"> ";
			$htm .= " <tr> ";
			$htm .= " <th rowspan=2>ORD</th><th rowspan=2>Área Temática</th><th rowspan=2>Atividade</th><th colspan=2>Iniciou no MaisEducação?</th> ";
			$htm .= " </tr> ";
			$htm .= " <tr> ";
			$htm .= " <th>SIM</th><th>NÃO</th> ";
			$htm .= " </tr> ";
			$htm .= " <tr> ";
			
			$mac = 0; $ati = 1; $ini = 2;
			
			for($i = 1; $i <=6; $i++){
				$htm .= " <td>$i</td> ";
				
				#Área temática
				$htm .= "<td id=\"td_macrocampo_$i\">";
				$htm .= '<select name="macrocampo_'.$i.'" id="macrocampo_'.$i.'" style="width:230px;" class="CampoEstilo" onchange="atualizaComboAtividade(this.value, this.id);">';
				$htm .= '<option value="">Selecione...</option>';
					
				$sql = " Select mtmid as codigo, mtmdescricao as descricao From pdeescola.metipomacrocampo Where mtmanoreferencia = 2012; ";
				$macroCampo = $db->carregar($sql);
				
				for($x = 0; $x < count($macroCampo); $x++) {
					$htm .= "<option value=\"".$macroCampo[$x]["codigo"]."\" ".( ( $dados[$mac]['qeord'] == $i.'1' && $dados[$mac]['resdec_cod'] == $macroCampo[$x]["codigo"] )? 'selected="selected"' : '').">".$macroCampo[$x]["descricao"]."</option>";
				}

				$htm .= '</select>';
				$htm .= "</td>";
				
				#Atividade
				$htm .= "<td id=\"td_atividade_$i\" name=\"td_atividade_$i\">";
				$htm .= '<select name="atividade_'.$i.'" id="atividade_'.$i.'" style="width:200px;" class="CampoEstilo" >';
				$htm .= '<option value="">Selecione...</option>';
				
				$sql = "Select mtaid as codigo, mtadescricao as descricao from pdeescola.metipoatividade where mtaanoreferencia = 2012;";
				$atividade = $db->carregar($sql);
				
				for($y = 0; $y < count($atividade); $y++) {
					$htm .= "<option value=\"".$atividade[$y]["codigo"]."\" ".( ( $dados[$ati]['qeord'] == $i.'2' && $dados[$ati]['resdec_cod'] == $atividade[$y]["codigo"] ) ? 'selected="selected"' : '').">".$atividade[$y]["descricao"]."</option>";
				}
				
				$htm .= '</select>';
				$htm .= "</td>";
				$htm .= " <td><input type='radio' name='inicio_mais_edu_$i' value='S' ".( ( $dados[$ini]['qeord'] == $i.'3' && $dados[$ini]['resdec_cod'] == 'S' ) ? 'checked="checked"' : '')."></td> ";
				$htm .= " <td><input type='radio' name='inicio_mais_edu_$i' value='N' ".( ( $dados[$ini]['qeord'] == $i.'3' && $dados[$ini]['resdec_cod'] == 'N' ) ?  'checked="checked"' : '')."></td> ";
				$htm .= " </tr> ";
				
				$mac = $mac + 3;
				$ati = $ati + 3;
				$ini = $ini + 3;
			}
			$htm .= " </table> ";
		} else {
			$bgcolor1 = "#A8A8A8";
			$bgcolor2 = "#CCCCCC";
			
			$obQestionario = new QQuestionarioResposta();
	
			$queid = $obQestionario->pegaQuestionario( $qrpid );
	
			$classificacaoEscola = $db->pegaUm("SELECT memclassificacaoescola FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']." AND memstatus = 'A'");
			if(!$classificacaoEscola) $classificacaoEscola = "U";
			
			$qtdAlunos = $db->pegaUm("SELECT memquantidadealuno FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']." AND memstatus = 'A'");
			if($qtdAlunos > 100) $maisDeCem = "true";
			else $maisDeCem = "false";
			
			$bgcolor = "#A8A8A8";
			$htm .= "<input type='hidden' name='queid' id='queid' value='{$queid}'>";
			$htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
			$htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";
			
			$htm .= "<input type='hidden' name='identExterno' id='identExterno' value='1'>";
	
			$sql = "SELECT * FROM par.questionarioprofuncionariotutor WHERE qrpid = ".$qrpid;
			$arrDados = $db->carregar( $sql );
	
			$htm .= "<input type='hidden' name='promun' id='promun' value='1'>";
			$htm .= "<input type='hidden' name='quantidadeBanco' id='quantidadeBanco' value='{$arrDados[0]['qptqnt']}'>";
	
	    	//$htm .= "<table class='tabela' style='width:80%;' align='center'>";
			$htm .= "<tr>";
			$htm .= "<td bgcolor='$bgcolor1'><b>Macro-campo</b></td>";
			$htm .= "<td bgcolor='$bgcolor1'><b>Atividade</b></td>";
			$htm .= "<td bgcolor='$bgcolor1'><b>Nº de Participantes</b></td>";
			$htm .= "</tr>";
			
			$linhas = array(0,1,2,3,4,5);
			
			//Escreve as seis linhas da tabela
			foreach( $linhas as $linha ){
			
				$sql = "SELECT * FROM pdeescola.mequestionario WHERE qrpid = ".$qrpid." and linha = ".$linha;
				$dados = $db->pegaLinha( $sql );
				
				$htm .= "<tr>";
				//macrocampo
				$htm .= "<td bgcolor='$bgcolor2'>";
				$htm .= '<b>'.($linha + 1).' - </b><select name="macrocampo_'.$linha.'" id="macrocampo_'.$linha.'" style="width:200px;" class="CampoEstilo" onchange="comboAtividade(this);">';
				$htm .= '<option value="">Selecione o Macrocampo</option>';
						$whEsporteLazer = ($_SESSION["boAtivNaoPagas2009"]) ? " AND UPPER(mtm.mtmdescricao) like '%ESPORTE E LAZER%' " : "";
						
						$whRuralMenosDeCem = ($classificacaoEscola == 'R' && $maisDeCem = 'false') ? " AND mtm.mtmid not in (48,50,51) " : "";
						
						$anoAnterior = $_SESSION["exercicio"] - 1;
						
						$sql = "SELECT
									mtm.mtmid AS codigo, mtm.mtmdescricao AS descricao
								FROM
									pdeescola.metipomacrocampo mtm
								WHERE
									mtm.mtmsituacao = 'A' AND
									mtm.mtmanoreferencia = " . $anoAnterior . " 
									".$whEsporteLazer.$whRuralMenosDeCem."";
						$selMacroCampo = $db->carregar($sql);
						
						for($i=0; $i<count($selMacroCampo); $i++) {
							$htm .= "<option value=\"".$selMacroCampo[$i]["codigo"]."\" ".($dados['mtmid'] == $selMacroCampo[$i]["codigo"] ? 'selected' : '').">".$selMacroCampo[$i]["descricao"]."</option>";
						}
		
				$htm .= '</select>';
				$htm .= "</td>";
		
				//atividade
				$htm .= "<td bgcolor='$bgcolor2'>";
				if( $dados['mtmid'] ){
					$sql = "SELECT
								mta.mtaid AS codigo, 
								mta.mtadescricao AS descricao
							FROM
								pdeescola.metipoatividade mta
							WHERE
								mta.mtasituacao = 'A' AND
								mta.mtaanoreferencia = ".$anoAnterior." AND
								mta.mtmid = ".$dados['mtmid'];
					
					$selAtividade = $db->carregar($sql);
					
					$htm .= '<select name="atividade[]" id="atividade[]" style="width:200px;" class="CampoEstilo">';
					for($i=0; $i<count($selAtividade); $i++) {
						$htm .= "<option value=\"".$selAtividade[$i]["codigo"]."\" ".($dados['mtaid'] == $selAtividade[$i]["codigo"] ? 'selected' : '').">".$selAtividade[$i]["descricao"]."</option>";
					}
					$htm .= '</select>';
				} else {
					$htm .= '<select name="atividade_'.$linha.'" id="atividade_'.$linha.'" style="width:200px;" class="CampoEstilo">';
					$htm .= '<option value="">Selecione a Atividade</option>';
					$htm .= '</select>';
				}
				$htm .= "</td>";
				
				//numero de participantes
				$htm .= "<td bgcolor='$bgcolor2'>".campo_texto('campo_'.$linha,'N','S','',15,60,'[#]','','','','','campo_'.$linha, '', $dados['numpart'], "this.value=mascaraglobal('[#]',this.value);")."</td>";
				$htm .= "</tr>";
	
			}
			
			$htm .= "</table>";
			
		}
		$this->htm = $htm;
		
	}
	
	function retornoRelatorio( $qrpid, $perid ){

		global $db;

		$bgcolor1 = "#A8A8A8";
		$bgcolor2 = "#CCCCCC";
		
		$obQestionario = new QQuestionarioResposta();

		$queid = $obQestionario->pegaQuestionario( $qrpid );

		$classificacaoEscola = $db->pegaUm("SELECT memclassificacaoescola FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']." AND memstatus = 'A'");
		if(!$classificacaoEscola) $classificacaoEscola = "U";
		
		$qtdAlunos = $db->pegaUm("SELECT memquantidadealuno FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION['memid']." AND memstatus = 'A'");
		if($qtdAlunos > 100) $maisDeCem = "true";
		else $maisDeCem = "false";
		
		$bgcolor = "#A8A8A8";
		$htm .= "<input type='hidden' name='perid' id='perid' value='{$perid}'>";
		$htm .= "<input type='hidden' name='qrpid' id='qrpid' value='{$qrpid}'>";
		$htm .= "<input type='hidden' name='queid' id='queid' value='{$queid}'>";
		$htm .= "<input type='hidden' name='identExterno' id='identExterno' value='1'>";

		$sql = "SELECT * FROM par.questionarioprofuncionariotutor WHERE qrpid = ".$qrpid;
		$arrDados = $db->carregar( $sql );

		$htm .= "<input type='hidden' name='promun' id='promun' value='1'>";
		$htm .= "<input type='hidden' name='quantidadeBanco' id='quantidadeBanco' value='{$arrDados[0]['qptqnt']}'>";

    	$htm .= "<table class='tabela' style='width:80%;' align='center'>";
		$htm .= "<tr>";
		$htm .= "<td bgcolor='$bgcolor1'><b>Macro-campo</b></td>";
		$htm .= "<td bgcolor='$bgcolor1'><b>Atividade</b></td>";
		$htm .= "<td bgcolor='$bgcolor1'><b>Nº de Participantes</b></td>";
		$htm .= "</tr>";
		
		$linhas = array(0,1,2,3,4,5);
		
		//Escreve as seis linhas da tabela
		foreach( $linhas as $linha ){
		
			$sql = "SELECT * FROM pdeescola.mequestionario WHERE qrpid = ".$qrpid." and linha = ".$linha;
			$dados = $db->pegaLinha( $sql );
			
			$htm .= "<tr>";
			//macrocampo
			$htm .= "<td bgcolor='$bgcolor2'>";
			$htm .= '<b>'.($linha + 1).' - </b><select name="macrocampo_'.$linha.'" id="macrocampo_'.$linha.'" disabled="disabled" style="width:200px;" class="CampoEstilo" onchange="comboAtividade(this);">';
			$htm .= '<option value="">Selecione o Macrocampo</option>';
					$whEsporteLazer = ($_SESSION["boAtivNaoPagas2009"]) ? " AND UPPER(mtm.mtmdescricao) like '%ESPORTE E LAZER%' " : "";
					
					$whRuralMenosDeCem = ($classificacaoEscola == 'R' && $maisDeCem = 'false') ? " AND mtm.mtmid not in (48,50,51) " : "";
					
					$anoAnterior = $_SESSION["exercicio"] - 1;
					
					$sql = "SELECT
								mtm.mtmid AS codigo, mtm.mtmdescricao AS descricao
							FROM
								pdeescola.metipomacrocampo mtm
							WHERE
								mtm.mtmsituacao = 'A' AND
								mtm.mtmanoreferencia = " . $anoAnterior . " 
								".$whEsporteLazer.$whRuralMenosDeCem."";
					$selMacroCampo = $db->carregar($sql);
					
					for($i=0; $i<count($selMacroCampo); $i++) {
						$htm .= "<option value=\"".$selMacroCampo[$i]["codigo"]."\" ".($dados['mtmid'] == $selMacroCampo[$i]["codigo"] ? 'selected' : '').">".$selMacroCampo[$i]["descricao"]."</option>";
					}
	
			$htm .= '</select>';
			$htm .= "</td>";
	
			//atividade
			$htm .= "<td bgcolor='$bgcolor2'>";
			if( $dados['mtmid'] ){
				$sql = "SELECT
							mta.mtaid AS codigo, 
							mta.mtadescricao AS descricao
						FROM
							pdeescola.metipoatividade mta
						WHERE
							mta.mtasituacao = 'A' AND
							mta.mtaanoreferencia = ".$anoAnterior." AND
							mta.mtmid = ".$dados['mtmid'];
				
				$selAtividade = $db->carregar($sql);
				
				$htm .= '<select name="atividade[]" id="atividade[]" disabled="disabled" style="width:200px;" class="CampoEstilo">';
				for($i=0; $i<count($selAtividade); $i++) {
					$htm .= "<option value=\"".$selAtividade[$i]["codigo"]."\" ".($dados['mtaid'] == $selAtividade[$i]["codigo"] ? 'selected' : '').">".$selAtividade[$i]["descricao"]."</option>";
				}
				$htm .= '</select>';
			} else {
				$htm .= '<select name="atividade_'.$linha.'" id="atividade_'.$linha.'" disabled="disabled" style="width:200px;" class="CampoEstilo">';
				$htm .= '<option value="">Selecione a Atividade</option>';
				$htm .= '</select>';
			}
			$htm .= "</td>";
			
			//numero de participantes
			$htm .= "<td bgcolor='$bgcolor2'>".campo_texto('campo_'.$linha,'N','N','',15,60,'[#]','','','','','campo_'.$linha, '', $dados['numpart'], "this.value=mascaraglobal('[#]',this.value);")."</td>";
			$htm .= "</tr>";

		}
		
		$htm .= "</table>";
		
		echo $htm;
	}

	//Função que salva o resultado do campo externo. Não deverá ser salvo no esquema do questionário, e sim no esquema do próprio sistema.
	function salvar() {
		global $db;
		
		if($_POST['perid'] == 3574){ //3644 => 3466 => 3574
		
			#valor fixo,devido o junção de uma pergunta interna e externa. 
			$_REQUEST['perid'] = 3466; 
			
			for($i = 2; $i <= 6; $i++){
				$sql_verifica = "SELECT 	qexid 
								 FROM 		pdeescola.mequestionarioexterno 
								 WHERE		queid = {$_REQUEST['queid']} AND qrpid = {$_REQUEST['qrpid']} 
								 			AND perid = {$_REQUEST['perid']} AND qeord = {$i}";
				$result = $db->pegaLinha($sql_verifica);				
				
				if(!empty($_REQUEST['semana'.$i])){
					if(empty($result)){
						$sql = "INSERT INTO pdeescola.mequestionarioexterno
								(queid,perid,qrpid,resdesc,qeord)
	    						VALUES 
	    						({$_REQUEST['queid']},{$_REQUEST['perid']},{$_REQUEST['qrpid']},'{$_REQUEST['semana'.$i]}',{$i});";
						$valida = 0;
						$db->executar($sql);
					} else {
						$sql = "UPDATE 	pdeescola.mequestionarioexterno
								SET 	resdesc = '{$_REQUEST['semana'.$i]}'
								WHERE	queid = {$_REQUEST['queid']} AND perid = {$_REQUEST['perid']} AND qrpid = {$_REQUEST['qrpid']} AND qeord = {$i}";
						$valida = 0;
						$db->executar($sql);
					}	
				}	
			}	

			if($valida == 0){
				$db->commit();
				echo "<script>alert('Dados salvo com sucesso!');</script>";
			} else {
				$db->rollback();
				echo "<script>alert('Dados não foram salvo!');</script>";
			}			
		}

		if( $_POST['perid'] == 3465 ){ //3464 => 3465
			if($_REQUEST['quantidade']){
				for ($i = 0; $i < $_REQUEST['quantidade']; $i++) {
					$sql_verifica = "SELECT 	mqaid 
									 FROM 		pdeescola.mequestionarioexternoatividade 
									 WHERE		mtmid = {$_REQUEST['macrocampo'.$i]} AND mtaid = {$_REQUEST['atividade'.$i]} AND qrpid = {$_REQUEST['qrpid']}";
					$result = $db->pegaLinha($sql_verifica);
					
					if(!empty($_REQUEST['inicio'.$i])){
						if(empty($result)){
							$sql = "INSERT INTO pdeescola.mequestionarioexternoatividade
									( qrpid, mtmid, mtaid, mqinicio, mqano )
									VALUES
									( {$_REQUEST['qrpid']},{$_REQUEST['macrocampo'.$i]},{$_REQUEST['atividade'.$i]},'{$_REQUEST['inicio'.$i]}','2012');";
							$valida = 0;
							$db->executar($sql);
						} else {
							$sql = "UPDATE 	pdeescola.mequestionarioexternoatividade
									SET 	mqinicio = '{$_REQUEST['inicio'.$i]}'
									WHERE	mtmid = {$_REQUEST['macrocampo'.$i]} AND mtaid = {$_REQUEST['atividade'.$i]} AND qrpid = {$_REQUEST['qrpid']}";
							$valida = 0;
							$db->executar($sql);
						}
					}
				}
			}
			
			if($valida == 0){
				$db->commit();
				echo "<script>alert('Dados salvo com sucesso!');</script>";
			} else {
				$db->rollback();
				echo "<script>alert('Dados não foram salvo!');</script>";
			}
		}
		
		#Foi Criado uma tabela no esquema pdeescola, esta tabela é composta: pelos campos que identificão o questionario, a pergunta e quem esta respondendo além é claro,
		#das respostas referentes a pergunta, que é feito da seguinte forma: campo descritivo da resposta e o campo "qeord" que identifica a linha e coluna da resposta.
		#Ex: na questão 3460 tem 4 linhas sendo elas os valores e o saldo sendo enumeradas de 1 a 4 respectivamente e uma unica coluna sendo ela definida como coluna 1.
		#Essas linhas e colunas são salvos na coluna qeord da seguinte forma 11, 21, 31 e 41, dessa forma podendo identificar as respostas.		
		if( $_POST['perid'] == 3464 ){ //3460 => 3464
			$valida = 0;
			
			//verifica valores
			$_POST['valor_2'] ? $valor_2 = formata_valor_sql($_POST['valor_2']) : $valor_2 = 0;
			$_POST['valor_3'] ? $valor_3 = formata_valor_sql($_POST['valor_3']) : $valor_3 = 0;
			$_POST['valor_4'] ? $valor_4 = formata_valor_sql($_POST['valor_4']) : $valor_4 = 0;
			
			if( ( (float)($valor_2) - (float)($valor_3 + $valor_4) ) >= 0.0001 ){
				echo "<script>alert('A soma dos valores Capital e Custeio deve ser igual ao Saldo 31.12.2012!');</script>";
				$valida = 1;
			}
			if( (float)($valor_3) == 0 || (float)($valor_4) == 0 ){
				$valida = 0;
			}
			
			if($valida == 0){
				//verifica se existe valores salvos
				$sql = "SELECT qrpid FROM pdeescola.mequestionarioexterno WHERE qrpid = {$_POST['qrpid']} AND perid = {$_POST['perid']} LIMIT 1";
				if( $db->pegaUm( $sql ) ){
					$sql = "DELETE FROM pdeescola.mequestionarioexterno WHERE qrpid = {$_POST['qrpid']} AND perid = {$_POST['perid']}";
					$db->executar($sql);
				}
	
				//insere novos valores
				$linhas = array(1=>1,2=>2,3=>3,4=>4);
				$sql = "";
				foreach($linhas as $linha){
					if( $_POST['valor_'.$linha] ){
						$sql .= " INSERT INTO pdeescola.mequestionarioexterno( queid, perid, qrpid, resdesc, qeord )VALUES( {$_POST['queid']},{$_POST['perid']},{$_POST['qrpid']},'".formata_valor_sql($_POST['valor_'.$linha])."',".$linha.'1'." ); ";
					} else {
						$alerta =  "<script>alert('A campo \"Valor\" é obrigatório!');</script>";
						$valida = 1;				
					}
				}
				//ver($sql);
			}
			
			if( $valida == 0 ){
				$db->executar($sql);
				$db->commit();
				echo "<script>alert('Dados salvo com sucesso!');</script>";
			} else {
				$db->rollback();
				echo "<script>alert('Dados não foram salvo!');</script>";
			}	
			
		}
		
		if($_POST['perid'] == 3305){
			$valida = 0;
			
			$sql = "SELECT qrpid FROM pdeescola.mequestionarioexterno WHERE qrpid = ".$_POST['qrpid']." and perid = ".$_POST['perid']." LIMIT 1;";
			if( $db->pegaUm( $sql ) ){
				$sql = "DELETE FROM pdeescola.mequestionarioexterno WHERE qrpid = ".$_POST['qrpid']." and perid = ".$_POST['perid'].";";
				$db->executar( $sql );
			}

			$linhas = array(1=>1,2=>2,3=>3,4=>4,5=>5,6=>6);
							
			foreach( $linhas as $linha => $conteudo){
				if( $valida == 0 ){
					if( $_POST['macrocampo_'.$linha] ){
						$sql .= "INSERT INTO pdeescola.mequestionarioexterno( queid, perid, qrpid, resdesc, qeord )VALUES( ".$_POST['queid'].",".$_POST['perid'].",".$_POST['qrpid'].",'".$_POST['macrocampo_'.$linha]."',".$linha.'1'." );";
						
					}else{
						$alerta =  "<script>alert('A campo Área tematica é obrigatório!');</script>";
						$v = 1;
					}
					if( $_POST['atividade_'.$linha] ){
							$sql .= "INSERT INTO pdeescola.mequestionarioexterno( queid, perid, qrpid, resdesc, qeord )VALUES( ".$_POST['queid'].",".$_POST['perid'].",".$_POST['qrpid'].",'".$_POST['atividade_'.$linha]."',".$linha.'2'." );";
							
					}else{
						$alerta =  "<script>alert('A campo Atividade é obrigatório!');</script>";
						$v = 1;
					}			
					if( $_POST['inicio_mais_edu_'.$linha] != ''){
						$sql .= "INSERT INTO pdeescola.mequestionarioexterno( queid, perid, qrpid, resdesc, qeord )VALUES( ".$_POST['queid'].",".$_POST['perid'].",".$_POST['qrpid'].",'".$_POST['inicio_mais_edu_'.$linha]."',".$linha.'3'." );";
						
					}else{
						$alerta =  "<script>alert('A campo Iniciou no MaisEducação é obrigatório!');</script>";
						$v = 1;
					}
				}else{
					echo $alerta;
					$v = 1;
				}			
			} 
			
			if( $valida == 0 ){
				$db->carregar( $sql );
				$db->commit();
				echo "<script>alert('Dados salvo com sucesso!');</script>";
			} else {
				$db->rollback();
				echo "<script>alert('Dados não foram salvo!');</script>";
			}	
		}	
		
		if($_POST['perid'] == 3515){ //3490 => 3223 => 3515
			$valida = 0;
			
			#valor fixo,devido o junção de uma pergunta interna e externa. 
			$perid = 3516; //3335 => 3224 => 3516
			
			$sql = "SELECT qrpid FROM pdeescola.mequestionarioexterno WHERE qrpid = ".$_POST['qrpid']." and perid = ".$perid." LIMIT 1;";
			if( $db->pegaUm( $sql ) ){
				$sql = "DELETE FROM pdeescola.mequestionarioexterno WHERE qrpid = ".$_POST['qrpid']." and perid = ".$perid.";";
				$db->executar( $sql );
			}
		
			$linhas = array(1=>1,2=>2,3=>3);
				
			foreach( $linhas as $linha ){
				if( $valida == 0 ){
					if( $_POST['representante_'.$linha] ){
						$sql .= "INSERT INTO pdeescola.mequestionarioexterno( queid, perid, qrpid, resdesc, qeord )VALUES( ".$_POST['queid'].",".$perid.",".$_POST['qrpid'].",'".utf8_decode($_POST['representante_'.$linha])."',".$linha.'1'." );";
		
					}else{
						$alerta =  "<script>alert('A campo Nome do representante é obrigatório!');</script>";
						$v = 1;
					}
					if( $_POST['seguimento_'.$linha] ){
						$sql .= "INSERT INTO pdeescola.mequestionarioexterno( queid, perid, qrpid, resdesc, qeord )VALUES( ".$_POST['queid'].",".$perid.",".$_POST['qrpid'].",'".$_POST['seguimento_'.$linha]."',".$linha.'2'." );";
							
					}else{
						$alerta =  "<script>alert('A campo Segmento representativo é obrigatório!');</script>";
						$v = 1;
					}
				}else{
					echo $alerta;
					$v = 1;
				}
			}

			if( $valida == 0 ){
				$db->carregar( $sql );
				$db->commit();
				echo "<script>alert('Dados salvo com sucesso!');</script>";
			} else {
				$db->rollback();
				echo "<script>alert('Dados não foram salvo!');</script>";
			}
		}		
		
		if( $_POST ){
			$v = 0;
			
			$sql = "SELECT qrpid FROM pdeescola.mequestionario WHERE qrpid = ".$_POST['qrpid']." LIMIT 1";
			if( $db->pegaUm( $sql ) ){
				$sql = "DELETE FROM pdeescola.mequestionario WHERE qrpid = ".$_POST['qrpid'];
				$db->executar( $sql );
			}

			$linhas = array(0,1,2,3,4,5);
			
			foreach( $linhas as $linha ){
				if( $v == 0 ){
					if( $_POST['macrocampo_'.$linha] && $_POST['atividade'][$linha] && $_POST['campo_'.$linha] ){
						$sql = "INSERT INTO pdeescola.mequestionario ( qrpid, linha, mtmid, mtaid, numpart ) 
															VALUES ( {$_POST['qrpid']}, {$linha}, {$_POST['macrocampo_'.$linha]}, {$_POST['atividade'][$linha]}, {$_POST['campo_'.$linha]} )";
						$db->carregar( $sql );
					} else {
						echo "<script>alert('Todos os dados são obrigatórios');</script>";
						$v = 1;
					}
				}
			}

			if( $v == 0 ){
				$db->commit();
			} else {
				$db->rollback();
			}
		}
	}

	//Função obrigatória. Dá echo no $this->htm para imprimir o campo externo na tela.
	function show(){
		echo $this->htm;
	}
}