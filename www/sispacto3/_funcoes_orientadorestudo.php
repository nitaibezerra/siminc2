<?
function sqlEquipeOrientador($dados) {
	global $db;
	
	$sql = "SELECT i.iusd,
				   i.iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   pp.pflcod,
				   pp.pfldsc, 
				   '' as periodo,
				   0 as nmeses,
				   (SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
				   (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_PROFESSORALFABETIZADOR.") as perfil,
				   (SELECT usucpf FROM sispacto3.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					CASE WHEN p.picid IS NOT NULL THEN 
														CASE WHEN p.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN p.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
				   
			FROM sispacto3.identificacaousuario i 
			INNER JOIN sispacto3.professoralfabetizadorturma ot ON ot.iusd = i.iusd 
			INNER JOIN sispacto3.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sispacto3.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod 
			LEFT JOIN sispacto3.pactoidadecerta pic ON pic.picid = i.picid 
			LEFT JOIN workflow.documento d ON d.docid = pic.docid 
			LEFT JOIN workflow.documento d2 ON d2.docid = pic.docidturma 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = p.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE t.pflcod=".PFL_PROFESSORALFABETIZADOR." AND tt.iusd='".$dados['iusd']."' AND i.iusstatus='A' AND CASE WHEN pic.picid IS NOT NULL THEN d.esdid=".ESD_VALIDADO_COORDENADOR_LOCAL." AND d2.esdid=".ESD_FECHADO_TURMA." ELSE true END";
	
	return $sql;
}

function carregarOrientadorEstudo($dados) {
	global $db;
	
	$arr = $db->pegaLinha("SELECT u.uncid, re.reiid, su.uniuf, u.curid, u.docid, su.unisigla||' - '||su.uninome as descricao FROM sispacto3.universidadecadastro u 
					 	   INNER JOIN sispacto3.universidade su ON su.uniid = u.uniid
						   INNER JOIN sispacto3.reitor re on re.uniid = su.uniid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sispacto3.identificacaousuario i 
							   INNER JOIN sispacto3.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_ORIENTADORESTUDO."'");
	
	
	$_SESSION['sispacto3']['orientadorestudo'] = array("descricao" => $arr['descricao']." ( ".$infprof['iusnome']." )",
													  "curid" 	  => $arr['curid'], 
													  "uncid" 	  => $arr['uncid'], 
													  "reiid" 	  => $arr['reiid'], 
													  "estuf" 	  => $arr['uniuf'], 
													  "docid" 	  => $arr['docid'], 
													  "iusd" 	  => $infprof['iusd'],
													  "iuscpf"    => $infprof['iuscpf']);
	
	if($dados['direcionar']) {
		$al = array("location"=>"sispacto3.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function mostrarAbaAvaliacaoComplementar($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*) as tot FROM sispacto3.grupoitensavaliacaocomplementar g 
			INNER JOIN sispacto3.grupoitensavaliacaocomplementarperfil p ON  p.gicid = g.gicid 
			WHERE pflcod IN(SELECT pflcod FROM sispacto3.tipoperfil WHERe iusd='".$_SESSION['sispacto3'][$dados['abapai']]['iusd']."')";
	
	$tot = $db->pegaUm($sql);
	
	if($tot) return true;
	else return false;
	
}

function exibirRelatoExperiencia($dados) {
	global $db;
	
	include_once '_funcoes_professoralfabetizador.php';
	
	$es = estruturaRelatoExperiencia(array());
	
	$perguntainicial = 'Você tem certeza de que a experiência contribui para a aquisição da proficiência na escrita dos estudantes?';

	
	if($dados['reeid']) {
		$relatoexperiencia = $db->pegaLinha("SELECT * FROM sispacto3.relatoexperiencia WHERE reeid='".$dados['reeid']."'");
		if($relatoexperiencia) extract($relatoexperiencia);
	}
	
	if($es) {
	
		echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita" width="25%" style="font-size:x-small;">Orientações</td>';
		echo '<td style="font-size:x-small;">
				Prezado(a) Orientador(a),<br>
				Leia os relatos de experiência dos professores e analise-os considerando os critérios abaixo. Atenção: o conceito final não interfere no pagamento da bolsa. Ou seja, mesmo que o resultado da análise do relato seja “pouco satisfatório”, se o professor atingir a nota mínima na avaliação regular (7,0 ou mais), a bolsa será paga normalmente
			  </td>';
		echo '</tr>';
	
		foreach($es as $campo => $arr) {
				
			$cp = explode(";",$$campo);
			unset($ff);
			foreach($cp as $vv) {
				$pp = explode("||",$vv);
				$ff[] = $pp[0];
				$var = 'tx_'.$campo.'_'.$pp[0];
				$$var = $pp[1];
			}
				
			echo '<tr>';
			echo '<td class="SubTituloDireita" width="25%" style="font-size:x-small;">'.$arr['texto'].'</td>';
				
			echo '<td style="font-size:x-small;">';
				
			if($arr['text']) {
				echo '<div class="notprint"><textarea disabled id="tx_'.$campo.'" name="tx_'.$campo.'" cols="'.$arr['text']['cols'].'" rows="'.$arr['text']['rows'].'" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this ); textCounter( this.form.tx_'.$campo.', this.form.no_tx_'.$campo.', '.$arr['text']['maxsize'].');" style="width:70ex;" onkeydown="textCounter( this.form.tx_'.$campo.', this.form.no_tx_'.$campo.', '.$arr['text']['maxsize'].' );" onkeyup="textCounter( this.form.tx_'.$campo.', this.form.no_tx_'.$campo.', '.$arr['text']['maxsize'].');" class="txareanormal">'.$$campo.'</textarea><br><input readonly="" style="text-align:right;border-left:#888888 3px solid;color:#808080;" type="text" name="no_tx_'.$campo.'" size="6" maxlength="6" value="'.$arr['text']['maxsize'].'" class="CampoEstilo"><font color="red" size="1" face="Verdana"> máximo de caracteres</font></div>';
				echo (($arr['text']['dica'])?'<br><span style=font-size:x-small;>Dica : '.$arr['text']['dica'].'</span>':'');
			}
				
			if($arr['opcoes']) {
				foreach($arr['opcoes'] as $op) {
	
					if($arr['tipo']=='radio') echo '<input disabled type="radio" name="'.$campo.'" '.((in_array($op['valor'],$ff))?'checked':'').' value="'.$op['valor'].'" id="'.$campo.'_'.$op['valor'].'" onclick="exibir_dv_rd(\''.$campo.'\',this)"> '.$op['descricao'].'<br>';
					elseif($arr['tipo']=='checkbox') echo '<input disabled type="checkbox" '.((in_array($op['valor'],$ff))?'checked':'').' name="'.$campo.'[]" value="'.$op['valor'].'" '.(($op['complementotexto'])?'onclick="exibir_dv_chk(\''.$campo.'\',this)"':'').'> '.$op['descricao'].'<br>';
						
					if($op['complementotexto']) {
						$var2 = 'tx_'.$campo.'_'.$op['valor'];
						echo '<div style="width: 50%;padding: 10px;border: 3px solid gray;margin: 0px; '.((in_array($op['valor'],$ff))?'':'display:none;').'" id="dv_'.$campo.'_'.$op['valor'].'">';
						echo $op['complementotexto'].' <div class="notprint"><textarea disabled id="tx_'.$campo.'_'.$op['valor'].'" name="tx_'.$campo.'_'.$op['valor'].'" cols="20" rows="3" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this ); textCounter( this.form.tx_'.$campo.'_'.$op['valor'].', this.form.no_tx_'.$campo.'_'.$op['valor'].', 50);" style="width:70ex;" onkeydown="textCounter( this.form.tx_'.$campo.'_'.$op['valor'].', this.form.no_tx_'.$campo.'_'.$op['valor'].', 50 );" onkeyup="textCounter( this.form.tx_'.$campo.'_'.$op['valor'].', this.form.no_tx_'.$campo.'_'.$op['valor'].', 50);" class="txareanormal">'.$$var2.'</textarea><br><input readonly="" style="text-align:right;border-left:#888888 3px solid;color:#808080;" type="text" name="no_tx_'.$campo.'_'.$op['valor'].'" size="6" maxlength="6" value="50" class="CampoEstilo"><font color="red" size="1" face="Verdana"> máximo de caracteres</font></div>';
						echo '</div>';
	
					}
						
				}
			}
				
			if($arr['datas']) {
				echo '<table>';
				foreach($arr['datas'] as $dt) {
					echo '<tr>';
					echo '<td align="right" style="font-size:x-small;">'.$dt['descricao'].'</td>';
					echo '<td style="font-size:x-small;">';
					$dt = $campo.$dt['valor'];
					echo formata_data($$dt);
					echo '</td>';
					echo '</tr>';
				}
				echo '</table>';
			}
	
	
			echo '</td>';
				
			echo '</tr>';
	
		}
	
		echo '</table>';
		
		echo '<form method=post id=formCorrecao>';
		echo '<input type=hidden name=reeid id=reeid value="'.$reeid.'">';
		echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		
		echo '<tr>';
		echo '<td style="font-size:x-small;">
				1. Concisão, clareza e objetividade na descrição do resumo da atividade (item 11 do formulário)<br>
				<input type=radio name=correcao1 value="2.0" '.(($correcao1=='2.0')?'checked':'').'> Ótimo<br>
				<input type=radio name=correcao1 value="1.5" '.(($correcao1=='1.5')?'checked':'').'> Bom<br>
				<input type=radio name=correcao1 value="1.0" '.(($correcao1=='1.0')?'checked':'').'> Regular<br>
				<input type=radio name=correcao1 value="0.5" '.(($correcao1=='0.5')?'checked':'').'> Fraco
				<hr>
			  </td>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<td style="font-size:x-small;">
				2. Adequação do espaço e dos materiais utilizados (itens 8 e 9 do formulário):<br>
				<input type=radio name=correcao2 value="2.0" '.(($correcao2=='2.0')?'checked':'').'> O espaço e os materiais foram adequados para a realização da atividade.<br>
				<input type=radio name=correcao2 value="1.0" '.(($correcao2=='1.0')?'checked':'').'> O espaço foi adequado, mas os materiais não foram adequados para a realização da atividade.<br>
				<input type=radio name=correcao2 value="1.0" '.(($correcao2=='1.0')?'checked':'').'> Os materiais foram adequados, mas o espaço não foi adequado para a realização da atividade.<br>
				<input type=radio name=correcao2 value="0.5" '.(($correcao2=='0.5')?'checked':'').'> Tanto o espaço quanto os materiais não foram adequados para a realização da atividade.
				<hr> 
			  </td>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<td style="font-size:x-small;">
				3. Adequação da atividade em relação à turma/ faixa etária:<br>
				<input type=radio name=correcao3 value="1.0" '.(($correcao3=='1.0')?'checked':'').'> A atividade é compatível com a turma na qual foi aplicada.<br>
				<input type=radio name=correcao3 value="0.5" '.(($correcao3=='0.5')?'checked':'').'> A atividade não é compatível com a turma na qual foi aplicada.
				<hr>
			  </td>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<td style="font-size:x-small;">
				4. Potencial da atividade para a melhoria da proficiência na escrita:<br>
				<input type=radio name=correcao4 value="3.0" '.(($correcao4=='3.0')?'checked':'').'> A atividade contribui muito para a melhoria da proficiência na escrita.<br> 
				<input type=radio name=correcao4 value="2.0" '.(($correcao4=='2.0')?'checked':'').'> A atividade contribui razoavelmente para a melhoria da proficiência na escrita.<br>
				<input type=radio name=correcao4 value="1.0" '.(($correcao4=='1.0')?'checked':'').'> A atividade contribui pouco para a melhoria da proficiência na escrita.<br>
				<input type=radio name=correcao4 value="0.5" '.(($correcao4=='0.5')?'checked':'').'> A atividade não contribui para a melhoria da proficiência na escrita.
				<hr>
			  </td>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<td style="font-size:x-small;">
				5. Criatividade e inovação<br>
				<input type=radio name=correcao5 value="2.0" '.(($correcao5=='2.0')?'checked':'').'> A atividade incorpora muitos elementos inovadores e/ou criativos.<br>
				<input type=radio name=correcao5 value="1.5" '.(($correcao5=='1.5')?'checked':'').'> A atividade incorpora alguns elementos inovadores e/ou criativos.<br>
				<input type=radio name=correcao5 value="1.0" '.(($correcao5=='1.0')?'checked':'').'> A atividade incorpora poucos elementos inovadores e/ou criativos.<br>
				<input type=radio name=correcao5 value="0.5" '.(($correcao5=='0.5')?'checked':'').'> A atividade não traz elementos inovadores e/ou criativos.
			  </td>';
		echo '</tr>';
		
		echo '<tr>';
		echo '<td style="font-size:x-small;" align="center">
				<input type=button name=gravar value="Gravar" onclick="gravarCorrecoes();">
			  </td>';
		echo '</tr>';
		
		
		echo '</table>';
		echo '</form>';
	
	}
	
	
}

function listaProfessoresRelatoExperiencia($dados) {
	global $db;
	/*
	$sql = "SELECT '<img src=../imagens/'||CASE WHEN r.correcao1 IS NOT NULL AND r.correcao2 IS NOT NULL AND r.correcao3 IS NOT NULL AND r.correcao4 IS NOT NULL AND r.correcao5 IS NOT NULL THEN 'valida1.gif' ELSE 'valida2.gif' END||' onclick=\"exibirRelatoExperiencia('||reeid||')\" style=cursor:pointer;>' as acao, '<span style=font-size:x-small;>'||replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')||'</span>' as iuscpf, 
				   '<span style=font-size:x-small;>'||i.iusnome||'</span>' as iusnome,
				   CASE WHEN r.correcao1 IS NOT NULL AND r.correcao2 IS NOT NULL AND r.correcao3 IS NOT NULL AND r.correcao4 IS NOT NULL AND r.correcao5 IS NOT NULL THEN 
					   CASE WHEN correcao1+correcao2+correcao3+correcao4+correcao5 >= 7.5 THEN '<span style=font-size:x-small;>Experiência destaque</span>'
							WHEN correcao1+correcao2+correcao3+correcao4+correcao5 >= 5.0 THEN '<span style=font-size:x-small;>Experiência satisfatória</span>'
							ELSE '<span style=font-size:x-small;>Experiência pouco satisfatória</span>' END 
				   ELSE '<span style=font-size:x-small;>Não validado</span>' END as exp
			FROM sispacto3.turmas tu
			INNER JOIN sispacto3.orientadorturma ot ON ot.turid = tu.turid
			INNER JOIN sispacto3.identificacaousuario i ON i.iusd = ot.iusd
			INNER JOIN sispacto3.relatoexperiencia r ON i.iusd = r.iusd
			WHERE tu.iusd='".$dados['iusd']."'";
	
	
	$cabecalho = array("&nbsp;","<span style=font-size:x-small;>CPF</span>","<span style=font-size:x-small;>Nome</span>","<span style=font-size:x-small;>Conceitos</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	*/
	
}

function atualizarCorrecaoRelatoExperiencia($dados) {
	global $db;
	
	$sql = "UPDATE sispacto3.relatoexperiencia SET correcao1=".(($dados['correcao1'])?"'".$dados['correcao1']."'":"NULL").",
												   correcao2=".(($dados['correcao2'])?"'".$dados['correcao2']."'":"NULL").",
												   correcao3=".(($dados['correcao3'])?"'".$dados['correcao3']."'":"NULL").",
												   correcao4=".(($dados['correcao4'])?"'".$dados['correcao4']."'":"NULL").",
												   correcao5=".(($dados['correcao5'])?"'".$dados['correcao5']."'":"NULL")." WHERE reeid='".$dados['reeid']."'";
	
	$db->executar($sql);
	$db->commit();
}

?>