<?
include_once '_funcoes_avaliacoes.php';


function removerTipoPerfil($dados) {
	global $db;
	
	// verificando pagamento
	$sql = "SELECT p.pboid FROM sismedio.tipoperfil t 
			INNER JOIN sismedio.pagamentobolsista p ON p.tpeid = t.tpeid  
			WHERE t.iusd='".$dados['iusd']."' AND t.pflcod='".$dados['pflcod']."'";
	
	$pboid = $db->pegaUm($sql);
	
	if($pboid) {
		if(!$dados['naoredirecionar']) {
			if($dados['picid']) $al = array("alert"=>"Coordenador Local ja possui pagamento e não pode ser removido, somente substituido","location"=>"sismedio.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=".$dados['picid']);
			if($dados['uncid']) $al = array("alert"=>"Coordenador IES ja possui pagamento e não pode ser removido, somente substituido","location"=>"sismedio.php?modulo=principal/universidade/gerenciarcoordenadories&acao=A&uncid=".$dados['uncid']);
			alertlocation($al);
		} else {
			return false;
		}
	}
	
	$sql = "DELETE FROM sismedio.tipoperfil WHERE iusd='".$dados['iusd']."' AND pflcod='".$dados['pflcod']."'";
	$db->executar($sql);
	
	$usucpf = $db->pegaUm("SELECT iuscpf FROM sismedio.identificacaousuario WHERE iusd='".$dados['iusd']."'");
	
	if($usucpf) {
		$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$usucpf."' AND pflcod='".$dados['pflcod']."'";
		$db->executar($sql);
		$sql = "DELETE FROM sismedio.usuarioresponsabilidade WHERE usucpf='".$usucpf."' AND pflcod='".$dados['pflcod']."'";
		$db->executar($sql);
	}
	
	$sql = "DELETE FROM sismedio.orientadorturma WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	
	$sql = "INSERT INTO sismedio.historicoidentificaousuario(
            iusd, hiudatainc, hiucpf, hiulog, hiustatus, hiutipo)
    		VALUES ('".$dados['iusd']."', NOW(), '".$_SESSION['usucpf']."', '".str_replace(array("'"),array(""),simec_json_encode($dados))."', 'A', 'removerTipoPerfil');";
	$db->executar($sql);
	
	$db->commit();
	
	if(!$dados['naoredirecionar']) {
		if($dados['picid']) $al = array("alert"=>"Coordenador Local removido com sucesso","location"=>"sismedio.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=".$dados['picid']);
		if($dados['uncid']) $al = array("alert"=>"Coordenador IES removido com sucesso","location"=>"sismedio.php?modulo=principal/universidade/gerenciarcoordenadories&acao=A&uncid=".$dados['uncid']);
		alertlocation($al);
	}
	
}

function verificaPermissao() {
	global $db;
	$perfis = pegaPerfilGeral();
	
	if(!$perfis) $perfis = array();
	
	$sql = "SELECT * FROM sismedio.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND rpustatus='A'";
	$ur = $db->carregar($sql);
	
	if($db->testa_superuser()) {
		return false;
	}
	
	if(in_array(PFL_GESTORESCOLA,$perfis)) {
		if($_REQUEST['modulo'] == 'principal/escola/escola') {
			return false;
		}

	}
	
	if(in_array(PFL_COORDENADORLOCAL,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_COORDENADORLOCAL && $urr['muncod']==$_SESSION['sismedio']['coordenadorlocal'][$_SESSION['sismedio']['esfera']]['muncod']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_EQUIPEMUNICIPALAP,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_EQUIPEMUNICIPALAP && $urr['muncod']==$_SESSION['sismedio']['coordenadorlocal'][$_SESSION['sismedio']['esfera']]['muncod']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_CONSULTAMUNICIPAL,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_CONSULTAMUNICIPAL && $urr['muncod']==$_SESSION['sismedio']['coordenadorlocal'][$_SESSION['sismedio']['esfera']]['muncod']) {
					return true;
				}
			}
		}
	}
	
	if(in_array(PFL_EQUIPEESTADUALAP,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_EQUIPEESTADUALAP && $urr['estuf']==$_SESSION['sismedio']['coordenadorlocal'][$_SESSION['sismedio']['esfera']]['estuf']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_CONSULTAESTADUAL,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_CONSULTAESTADUAL && $urr['estuf']==$_SESSION['sismedio']['coordenadorlocal'][$_SESSION['sismedio']['esfera']]['estuf']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_COORDENADORIES,$perfis)) {
		
		$parts = explode("/",$_REQUEST['modulo']);
		
		if($parts[1] != 'universidade') {
			return true;
		}
		
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_COORDENADORIES && $urr['uncid']==$_SESSION['sismedio']['universidade']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_COORDENADORADJUNTOIES,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_COORDENADORADJUNTOIES && $urr['uncid']==$_SESSION['sismedio']['coordenadoradjuntoies']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_PROFESSORALFABETIZADOR,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_PROFESSORALFABETIZADOR && $urr['uncid']==$_SESSION['sismedio']['professoralfabetizador']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_COORDENADORPEDAGOGICO,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_COORDENADORPEDAGOGICO && $urr['uncid']==$_SESSION['sismedio']['coordenadorpedagogico']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_ORIENTADORESTUDO,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_ORIENTADORESTUDO && $urr['uncid']==$_SESSION['sismedio']['orientadorestudo']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_FORMADORIES,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_FORMADORIES && $urr['uncid']==$_SESSION['sismedio']['formadories']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_FORMADORREGIONAL,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_FORMADORREGIONAL && $urr['uncid']==$_SESSION['sismedio']['formadorregional']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_SUPERVISORIES,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_SUPERVISORIES && $urr['uncid']==$_SESSION['sismedio']['supervisories']['uncid']) {
					return false;
				}
			}
		}
	}
	
	return true;
	
}


function carregarMunicipiosPorUF($dados) {
	global $db;
	$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$dados['estuf']."' ORDER BY mundescricao";
	$combo = $db->monta_combo($dados['name'], $sql, 'S', 'Selecione', (($dados['onclick'])?$dados['onclick']:''), '', '', '200', 'S', $dados['id'], true, $dados['valuecombo']);
	
	if($dados['returncombo']) return $combo;
	else echo $combo;
}

function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}

function progressBar($percentage) {
	
	global $db;
	
	$percentage = round($percentage,0);
	
	$percentage = $percentage > 100 ? 100 : $percentage;
	
	if($percentage==100) {
		$color = "#0000FF";
		print "<center><font color={$color}>Concluído</font></center>";
	} elseif($percentage==0) {
		$color = "#FF0000";
		print "<center><font color={$color}>Não iniciado</font></center>";
	} else {
		$color = "#215E21";
		print "<center><font color={$color}>Em Andamento</font></center>";
	}
	
	print "<div id=\"progress-bar\" style=\"-webkit-border-radius: 5px;-moz-border-radius: 5px;border-radius: 5px;width: 100px;margin: 0 auto;background: #cccccc;border: 3px solid #f2f2f2;\">\n";
	print "<div id=\"progress-bar-percentage\" class=\"all-rounded\" style=\"background: $color;padding: 1px 0px;color: #FFF;font-weight: bold;text-align: center;width: $percentage%\">";
		if ($percentage > 10) {
			print "&nbsp;$percentage%";
			print "</div></div>";
		} else {
			print "<div style=\"display: block;\">&nbsp;</div><div style=\"position:absolute;color:$color;margin-top:-14px;margin-left:".($percentage+10)."px;\" >$percentage%</div>";
			print "</div></div>";
		}
	
}


function montaAbasSismedio($abapai, $abaativa) {
	global $db;
	
	$sql = "SELECT abaordem, abadsc, abaendereco, abafuncaomostrar, abapai FROM sismedio.abas WHERE abapai='".$abapai."' ORDER BY abaordem";
	$abas = $db->carregar($sql);
	
	if($abas[0]) {
		foreach($abas as $aba) {
			
			$mostrar = true;
			
			if($aba['abafuncaomostrar']) {
				if(function_exists($aba['abafuncaomostrar'])) $mostrar = $aba['abafuncaomostrar']($aba); 
			}
			
			if($mostrar) $menu[] = array("id" => $aba['abaordem'], "descricao" => $aba['abadsc'], "link" => $aba['abaendereco']);
		}
	}
	
	echo "<br>";
	
	?>
	<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
	<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
	<div id="modalOrientacaoAdm" style="display:none;">
	<form method="post" id="formulario_orientacao" name="formulario_orientacao">
	<input type="hidden" name="abaid" id="abaid">
	<input type="hidden" name="requisicao" value="salvarOrientacaoAdm">
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloCentro" colspan="2">Orientação</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%"></td>
		<td><? echo campo_textarea( 'oabdesc', 'S', 'S', '', '70', '4', '5000'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" name="salvarorientacao" value="Salvar Orientação" onclick="salvarOrientacaoAdm();"></td>
	</tr>
	</table>
	</form>
	</div>
	<?
	
	echo montarAbasArray($menu, $abaativa);
}

function carregarDadosIdentificacaoUsuario($dados) {
	global $db;
	
	if(!$dados['pflcod']) {
		$al = array("alert"=>"Problemas para carregar os dados usuário","location"=>"sismedio.php?modulo=inicio&acao=C");
		alertlocation($al);
	}
	
	$sql = "SELECT i.cadastradosgb, i.uncid, i.iusd, i.iuscpf, i.iusnome, i.iusdatanascimento, i.iusnomemae, i.iustipoprofessor, i.iusnaodesejosubstituirbolsa,
				   i.iussexo, i.eciid, i.nacid, i.iusnomeconjuge, i.iusagenciasugerida, i.iusagenciaend, i.iusformacaoinicialorientador,
				   i.iusemailprincipal, i.iusemailopcional, i.iustipoorientador, to_char(i.iusdatainclusao,'YYYY-mm-dd') as iusdatainclusao, i.iustermocompromisso,  
				   i.tvpid, i.funid, i.foeid, f.iufid, f.cufid, f.iufdatainiformacao, f.iufdatafimformacao, f.iufsituacaoformacao,
				   m.estuf as estuf_nascimento, m.muncod as muncod_nascimento, ma.estuf||' / '||ma.mundescricao as municipiodescricaoatuacao, ma.muncod as muncodatuacao, 
				   d.itdid, d.tdoid, d.itdufdoc, d.itdnumdoc, d.itddataexp, d.itdnoorgaoexp,
				   e.ienid, mm.muncod as muncod_endereco, mm.estuf as estuf_endereco,
				   e.ientipo, e.iencep, e.iencomplemento, e.iennumero, e.ienlogradouro, e.ienbairro, cf.cufcodareageral, to_char(t.tpeatuacaoinicio,'YYYY-mm-dd') as tpeatuacaoinicio, to_char(t.tpeatuacaofim,'YYYY-mm-dd') as tpeatuacaofim, i.iusserieprofessor, pf.pfldsc, pf.pflcod   
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			LEFT  JOIN seguranca.perfil pf ON pf.pflcod = t.pflcod   
			LEFT  JOIN territorios.municipio m ON m.muncod = i.muncod 
			LEFT  JOIN sismedio.identiusucursoformacao f ON f.iusd = i.iusd 
			LEFT  JOIN sismedio.identusutipodocumento d ON d.iusd = i.iusd 
			LEFT  JOIN sismedio.identificaoendereco e ON e.iusd = i.iusd 
			LEFT  JOIN territorios.municipio mm ON mm.muncod = e.muncod 
			LEFT  JOIN territorios.municipio ma ON ma.muncod = i.muncodatuacao
			LEFT  JOIN sismedio.cursoformacao cf ON cf.cufid = f.cufid 
			LEFT  JOIN sismedio.orientadorturma ot ON ot.iusd = i.iusd 
			WHERE t.pflcod ".((is_array($dados['pflcod']))?"IN('".implode("','",$dados['pflcod'])."')":"='".$dados['pflcod']."'")." 
				  ".(($dados['iuscodigoinep'])?" AND i.iuscodigoinep='".$dados['iuscodigoinep']."'":"")." 
				  ".(($dados['uncid'])?" AND i.uncid='".$dados['uncid']."'":"")."
				  ".(($dados['turid'])?" AND ot.turid='".$dados['turid']."'":"")." 
				  ".(($dados['iustipoprofessor'])?" AND i.iustipoprofessor='".$dados['iustipoprofessor']."'":"")."
				  ".(($dados['iustipoorientador'])?" AND i.iustipoorientador='".$dados['iustipoorientador']."'":"")." 
				  ".(($dados['tpejustificativaformadories'])?" AND t.tpejustificativaformadories IS NOT NULL":"")." 
				  ".(($dados['iusd'])?" AND i.iusd='".$dados['iusd']."'":"")." AND iusstatus='A' ORDER BY i.iusd";
	
	$identificacaousuario = $db->carregar($sql);
	
	if($identificacaousuario[0]) {

		foreach($identificacaousuario as $key => $iu) {
			
			$idusuarios[$key] = $iu;
			unset($telefones);
			$sql = "SELECT itetipo, itedddtel, itenumtel FROM sismedio.identificacaotelefone WHERE iusd='".$iu['iusd']."'";
			$tels = $db->carregar($sql);
			if($tels[0]) {
				foreach($tels as $tel) {
					$telefones[$tel['itetipo']] = array("itedddtel"=>$tel['itedddtel'],"itenumtel"=>$tel['itenumtel']);
				}
				$idusuarios[$key]['telefones'] = $telefones; 
			}
		}
		
		
	}
	
	return $idusuarios;
	
}

function reiniciarSenha($dados) {
	global $db;
	
	$sql = "UPDATE seguranca.usuario SET ususenha='".md5_encrypt_senha("simecdti","")."' WHERE usucpf='".$dados['usucpf']."'";
	$db->executar($sql);
	
	$sql = "UPDATE seguranca.usuario_sistema SET suscod='A' WHERE usucpf='".$dados['usucpf']."' AND sisid='".SIS_MEDIO."'";
	$db->executar($sql);
	
	$db->commit();
	
	$arrUsu = $db->pegaLinha("SELECT usunome, usuemail FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['usucpf'])."'");
	
	$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO SISMÉDIO","email" => $arrUsu['usuemail']);
 	$destinatario = $arrUsu['usuemail'];
 	$usunome = $arrUsu['usunome'];
 	
 	$assunto = "Atualização de senha no SIMEC - MÓDULO SISMÉDIO";
 	$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
	$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo sismedio. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
 							  <p>Se for o seu primeiro acesso, o sistema solicitará que você crie uma nova senha. Se você já tiver cadastro no SIMEC, insira o seu CPF e senha. Caso tenha esquecido a sua senha de acesso ao SIMEC, clique em \"Esqueceu a senha?\" e insira o seu CPF. O sistema enviará a sua nova senha para o e-mail que você cadastrou. Em caso de dúvida, entre em contato com a sua Secretaria de Educação.</p>
 							  <p>Sua Senha de acesso é: %s</p>
 							  <br><br>* Caso você já alterou a senha acima, favor desconsiderar este e-mail.",
 			'Prezado(a)',
 			$usunome,
 			"simecdti"	
 			);
		
	if(!strstr($_SERVER['HTTP_HOST'],"simec-local")){
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );
	}
	
	$al = array("alert"=>"Senha reiniciada com sucesso","location"=>"sismedio.php?modulo=".$dados['modulo']."&acao=A&aba=".$dados['aba']);
	alertlocation($al);
	
	
}

function pegarDadosUsuarioPorCPF($dados) {
	global $db;
	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf='".$dados['cpf']."'";
	$usuemail = $db->pegaUm($sql);
	
	$sql = "SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf='".$dados['cpf']."' AND sisid='".SIS_MEDIO."'";
	$suscod = $db->pegaUm($sql);
	
	
	echo $usuemail."||".(($suscod)?$suscod:"NC");
}

function validarIdentificacaoUsuario($dados) {

	if(!$dados['iusdatanascimento']) {
		$erro[] = "Data de Nascimento em branco";
	}
	if(!$dados['iusnomemae']) {
		$erro[] = "Nome da mãe em branco";
	}
	if(!$dados['iussexo']) {
		$erro[] = "Sexo em branco";
	}
	if(!$dados['muncod_nascimento']) {
		$erro[] = "Município - Local Nascimento em branco";
	}
	if(!$dados['eciid']) {
		$erro[] = "Estado Civil em branco";
	}
	if(!$dados['nacid']) {
		$erro[] = "Nacionalidade em branco";
	}
	if(!$dados['iusagenciasugerida']) {
		$erro[] = "Agência em branco";
	}
	if(!$dados['iusagenciaend']) {
		$erro[] = "Endereço em branco";
	}
	if(!$dados['tvpid']) {
		$erro[] = "Vínculo em branco";
	}
	if(!$dados['funid']) {
		$erro[] = "Função em branco";
	}
	if(!$dados['foeid']) {
		$erro[] = "Formação (Escolaridade) em branco";
	}
	if(!$dados['iusemailprincipal']) {
		$erro[] = "Email Principal em branco";
	}
	
	return $erro;
}

function validarFormacao($dados) {
	if(!$dados['iufdatainiformacao']) {
		$erro[] = "Início - Formação em branco";
	}
	if(!$dados['iufsituacaoformacao']) {
		$erro[] = "Situação formação em branco";
	}
	
	return $erro;
	
}

function validarDocumento($dados) {
 	
	if(!$dados['tdoid']) {
		$erro[] = "Tipo - Documento em branco";
	}
	if(!$dados['itdufdoc']) {
		$erro[] = "Estado - Documento em branco";
	}
	if(!$dados['itdnumdoc']) {
		$erro[] = "Número do Documento em branco";
	}
	if(!$dados['itddataexp']) {
		$erro[] = "Data Expedição em branco";
	}
	if(!$dados['itdnoorgaoexp']) {
		$erro[] = "Orgão Expedidor em branco";
	}
	
	return $erro;
	
}

function validarEndereco($dados) {
	
	if(!substr($dados['muncod_endereco'],0,7)) {
		$erro[] = "Município - Endereço em branco";
	}
	if(!$dados['ientipo']) {
		$erro[] = "Tipo - Endereço em branco";
	}
	if(!str_replace(array("-"),array(""),$dados['iencep'])) {
		$erro[] = "CEP em branco";
	}
	if(!$dados['ienlogradouro']) {
		$erro[] = "Logradouro em branco";
	}
	if(!$dados['ienbairro']) {
		$erro[] = "Bairro em branco";
	}
	
	return $erro;
	
}

function atualizarDadosIdentificacaoUsuario($dados) {
	global $db;
	$erros = validarIdentificacaoUsuario($dados);
	
	if($erros) {
		$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erros),"location"=>$dados['goto']);
		alertlocation($al);
	}
	
	$iusagenciasugerida_atual = $db->pegaUm("SELECT iusagenciasugerida FROM sismedio.identificacaousuario WHERE iusd='".$dados['iusd']."'");
	if($iusagenciasugerida_atual != substr($dados['iusagenciasugerida'],0,4)) {
		$sqlsgb = "cadastradosgb=FALSE,";
	}
	

	$sql = "UPDATE sismedio.identificacaousuario SET
			iusdatanascimento = '".formata_data_sql($dados['iusdatanascimento'])."',
			iusnomemae		  = '".addslashes($dados['iusnomemae'])."',
			iussexo 		  = '".$dados['iussexo']."',
			muncod		  	  = '".$dados['muncod_nascimento']."',
			eciid 		  	  = '".$dados['eciid']."',
			nacid		  	  = '".$dados['nacid']."',
			iusnomeconjuge	  = '".addslashes($dados['iusnomeconjuge'])."',
			iusagenciasugerida = '".substr($dados['iusagenciasugerida'],0,4)."',
			iusagenciaend = '".substr(addslashes($dados['iusagenciaend']),0,250)."',
			{$sqlsgb}
			tvpid = '".$dados['tvpid']."',
			funid = '".$dados['funid']."',
			foeid = '".$dados['foeid']."',
			iusemailprincipal = '".$dados['iusemailprincipal']."',
			iusemailopcional=".(($dados['iusemailopcional'])?"'".$dados['iusemailopcional']."'":"NULL").",
			iusnaodesejosubstituirbolsa=".(($dados['iusnaodesejosubstituirbolsa']=='TRUE')?"TRUE":"FALSE").",
			iustermocompromisso=TRUE
			WHERE iusd='".$dados['iusd']."'";
	
	$db->executar($sql);
	
	$erros = validarFormacao($dados);
	
	if($erros) {
		$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erros),"location"=>$dados['goto']);
		alertlocation($al);
	}
	

	$iufid = $db->pegaUm("SELECT iufid FROM sismedio.identiusucursoformacao WHERE iusd='".$dados['iusd']."'");
	
	// controlando formação
	if($iufid) {
		
		$sql = "UPDATE sismedio.identiusucursoformacao SET
		            cufid=".(($dados['cufid'])?"'".$dados['cufid']."'":"NULL").", 
		            iufdatainiformacao='".formata_data_sql($dados['iufdatainiformacao'])."', 
		            iufdatafimformacao=".(($dados['iufdatafimformacao'])?"'".formata_data_sql($dados['iufdatafimformacao'])."'":"NULL").", 
		            iufsituacaoformacao='".$dados['iufsituacaoformacao']."'
		        WHERE iufid='".$iufid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sismedio.identiusucursoformacao(
		            iusd, cufid, iufdatainiformacao, iufdatafimformacao, iufsituacaoformacao, 
		            iufstatus)
		    VALUES ('".$dados['iusd']."', 
		    		".(($dados['cufid'])?"'".$dados['cufid']."'":"NULL").", 
		    		'".formata_data_sql($dados['iufdatainiformacao'])."', 
		    		".(($dados['iufdatafimformacao'])?"'".formata_data_sql($dados['iufdatafimformacao'])."'":"NULL").", 
		    		'".$dados['iufsituacaoformacao']."', 
		            'A');";
		
		$db->executar($sql);
		
	}
	
	$erros = validarDocumento($dados);
	
	if($erros) {
		$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erros),"location"=>$dados['goto']);
		alertlocation($al);
	}
	
	
	$itdid = $db->pegaUm("SELECT itdid FROM sismedio.identusutipodocumento WHERE iusd='".$dados['iusd']."'");
	
	// controlando documento
	if($itdid) {
		
		$sql = "UPDATE sismedio.identusutipodocumento SET
            	tdoid='".$dados['tdoid']."', itdufdoc='".$dados['itdufdoc']."', 
            	itdnumdoc='".$dados['itdnumdoc']."', itddataexp='".formata_data_sql($dados['itddataexp'])."', 
            	itdnoorgaoexp='".$dados['itdnoorgaoexp']."'		
		        WHERE itdid='".$itdid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sismedio.identusutipodocumento(
            	iusd, tdoid, itdufdoc, itdnumdoc, itddataexp, itdnoorgaoexp, itdstatus)
    			VALUES ('".$dados['iusd']."', '".$dados['tdoid']."', '".$dados['itdufdoc']."', '".$dados['itdnumdoc']."', 
    			'".formata_data_sql($dados['itddataexp'])."', '".$dados['itdnoorgaoexp']."', 'A');";
		
		$db->executar($sql);
		
	}
	
	$erros = validarEndereco($dados);
	
	if($erros) {
		$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erros),"location"=>$dados['goto']);
		alertlocation($al);
	}
	
	$ienid = $db->pegaUm("SELECT ienid FROM sismedio.identificaoendereco WHERE iusd='".$dados['iusd']."'");
	
	// controlando endereço
	if($ienid) {
		
		$sql = "UPDATE sismedio.identificaoendereco SET
            	muncod='".substr($dados['muncod_endereco'],0,7)."', ientipo='".$dados['ientipo']."', 
            	iencep='".str_replace(array("-"),array(""),$dados['iencep'])."', iencomplemento=".(($dados['iencomplemento'])?"'".addslashes($dados['iencomplemento'])."'":"NULL").", 
            	iennumero=".((!is_null($dados['iennumero']) && is_numeric($dados['iennumero']))?"'".$dados['iennumero']."'":"NULL").", ienlogradouro='".addslashes($dados['ienlogradouro'])."', 
            	ienbairro='".addslashes($dados['ienbairro'])."' 		
		        WHERE ienid='".$ienid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sismedio.identificaoendereco(
            	muncod, iusd, ientipo, iencep, iencomplemento, iennumero, 
            	iensatatus, ienlogradouro, ienbairro)
    			VALUES ('".substr($dados['muncod_endereco'],0,7)."', '".$dados['iusd']."', '".$dados['ientipo']."', '".str_replace(array("-"),array(""),$dados['iencep'])."', 
    					".(($dados['iencomplemento'])?"'".addslashes($dados['iencomplemento'])."'":"NULL").", ".((!is_null($dados['iennumero']))?"'".str_replace(array(" "),array(""),$dados['iennumero'])."'":"NULL").", 'A', '".addslashes($dados['ienlogradouro'])."', '".substr(addslashes($dados['ienbairro']),0,60)."');";
		
		$db->executar($sql);
		
	}
	
	// controlando telefones
	$db->executar("DELETE FROM sismedio.identificacaotelefone WHERE iusd='".$dados['iusd']."'");
	
	$tipos = array("R","T","C","F");
	
	foreach($tipos as $tipo) {
		
		$sql = "INSERT INTO sismedio.identificacaotelefone(
            	iusd, itedddtel, itenumtel, itetipo, itestatus)
    			VALUES ('".$dados['iusd']."', ".(($dados['itedddtel'][$tipo])?"'".$dados['itedddtel'][$tipo]."'":"NULL").", ".(($dados['itenumtel'][$tipo])?"'".$dados['itenumtel'][$tipo]."'":"NULL").", '".$tipo."', 'A');";
		
		$db->executar($sql);
		
	}
	
	$sql = "INSERT INTO sismedio.historicoidentificaousuario(
            iusd, hiudatainc, hiucpf, hiulog, hiustatus, hiutipo)
    		VALUES ('".$dados['iusd']."', NOW(), '".$_SESSION['usucpf']."', '".str_replace(array("'"),array(""),simec_json_encode($dados))."', 'A', 'atualizarDadosIdentificacaoUsuario');";
	$db->executar($sql);
	
	$sql = "UPDATE sismedio.tipoperfil SET tpeatuacaoinicio=".(($dados['tpeatuacaoinicio_mes'] && $dados['tpeatuacaoinicio_ano'])?"'".$dados['tpeatuacaoinicio_ano']."-".$dados['tpeatuacaoinicio_mes']."-01'":"NULL").", 
										   tpeatuacaofim=".(($dados['tpeatuacaofim_mes'] && $dados['tpeatuacaofim_ano'])?"'".$dados['tpeatuacaofim_ano']."-".$dados['tpeatuacaofim_mes']."-01'":"NULL")." WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	$db->commit();
	
	sincronizarUsuariosSIMEC(array('cpf' => $dados['iuscpf']));
	
	$al = array("alert"=>$dados['mensagemalert'],"location"=>$dados['goto']);
	alertlocation($al);
	
}

function salvarOrientacaoAdm($dados) {
	global $db;
	
	$oabid = $db->pegaUm("SELECT oabid FROM sismedio.orientacaoaba WHERE abaid='".$dados['abaid']."'");
	
	if($oabid) {
		
		$sql = "UPDATE sismedio.orientacaoaba SET oabdesc='".$dados['oabdesc']."' WHERE oabid='".$oabid."'";
		$db->executar($sql);
		
	} else {
	
		$sql = "INSERT INTO sismedio.orientacaoaba(
	            abaid, oabdesc, oabstatus)
	    		VALUES ('".$dados['abaid']."', '".$dados['oabdesc']."', 'A');";
		$db->executar($sql);
	
	}
	
	$db->commit();
	
	$al = array("alert"=>"Orientação gravada com sucesso.","location"=>$_SERVER['REQUEST_URI']);
	alertlocation($al);
	
}

function carregarOrientacaoPorFiltro($dados) {
	global $db;
	
	$sql = "SELECT oabdesc FROM sismedio.orientacaoaba WHERE abaid='".$dados['abaid']."'";
	$oabdesc = $db->pegaUm($sql);
	
	echo $oabdesc;
}

function carregarOrientacao($endereco) {
	global $db;
	
	$sql = "SELECT a.abaid, o.oabdesc FROM sismedio.abas a 
			LEFT JOIN sismedio.orientacaoaba o ON o.abaid = a.abaid 
			WHERE a.abaendereco='".$endereco."'";

	$abas = $db->pegaLinha($sql);
	
	$orientacao = $abas['oabdesc'];
	$abaid      = $abas['abaid'];
	
	if($db->testa_superuser()) {
		$htmladm = "<br><img src=\"../imagens/page_attach.png\" style=\"cursor:pointer;\" onclick=\"mostrarOrientacaoAdm('".$abaid."');\">";
	}
	
	return (($orientacao)?nl2br($orientacao):"Orientação não foi cadastrada").$htmladm;
}

function efetuarTrocaUsuarioPerfil($dados) {
	global $db;
	
	if(!$dados['iuscpf_']) $erro[] = "CPF em branco";
	if(!$dados['iusnome_']) $erro[] = "Nome em branco";
	if(!$dados['iusemailprincipal_']) $erro[] = "Email em branco";
	
	if($erro) {
		$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erro),"location"=>$_SERVER['HTTP_REFERER']);
		alertlocation($al);
	}

	$sql = "SELECT * FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd WHERE i.iusd='".$dados['iusdantigo']."'";
	$identificacaousuario_antigo = $db->pegaLinha($sql);
	
	if($identificacaousuario_antigo['pflcod']==PFL_PROFESSORALFABETIZADOR || $identificacaousuario_antigo['pflcod']==PFL_COORDENADORPEDAGOGICO) {
		
		$docids = $db->carregarColuna("SELECT docid FROM sismedio.pagamentobolsista WHERE tpeid='".$identificacaousuario_antigo['tpeid']."'");
		
		$possuipagamento = false;
		
		if($docids) {
			
			foreach($docids as $docid) {
				$esdid_pag = $db->pegaUm("SELECT d.esdid FROM workflow.documento d WHERE d.docid='".$docid."'");
				if($esdid_pag != ESD_PAGAMENTO_APTO) {
					$possuipagamento = true;
				}
			}
			
			if($possuipagamento) {
				$al = array("alert"=>"Não é possível efetuar a substituição, pois o professor alfabetizador (".$identificacaousuario_antigo['iusnome'].") ja recebeu bolsa","location"=>$_SERVER['HTTP_REFERER']);
				alertlocation($al);
			} else {
				$db->executar("DELETE FROM sismedio.pagamentobolsista WHERE docid IN('".implode("','",$docids)."')");				
			}
		}
		
	}
	
	if(!$identificacaousuario_antigo) {
		$al = array("alert"=>"Usuário a ser substituido não foi encontrado","location"=>$_SERVER['HTTP_REFERER']);
		alertlocation($al);
	}
	
	if($identificacaousuario_antigo['pflcod'] == PFL_ORIENTADORESTUDO) $having_orientador = " HAVING COUNT(*) > 1";
	
	$sql = "SELECT COUNT(*) as t FROM sismedio.mensario m 
			INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = m.menid
			INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO."
			WHERE m.iusd='".$identificacaousuario_antigo['iusd']."' AND mavtotal>=7 AND d.esdid=".ESD_ENVIADO_MENSARIO." ".$having_orientador;
	
	$is_apto = $db->pegaUm($sql);

	if($is_apto) {
		$al = array("alert"=>"O usuário (".$identificacaousuario_antigo['iusnome'].") não pode ser substituido pois se encontra APTO A RECER BOLSA(Avaliações positivas) em alguns períodos. Solicite ao Coordenador GERAL/ADJUNTO que acesse a aba Aprovar Equipe, e aprove sua bolsa. Após este procedimento, este usuário estará disponível para troca.","location"=>$_SERVER['HTTP_REFERER']);
		alertlocation($al);
	}
	
	
	if(!$identificacaousuario_antigo['uncid']) $identificacaousuario_antigo['uncid'] = $dados['uncid'];
	
	$sql = "SELECT i.iusd, t.tpeid, i.iusnome FROM sismedio.identificacaousuario i LEFT JOIN sismedio.tipoperfil t ON t.iusd = i.iusd WHERE i.iuscpf='".str_replace(array(".","-"),array("",""),$dados['iuscpf_'])."'";
	$identificacaousuario_novo = $db->pegaLinha($sql);
	
	if($identificacaousuario_novo['tpeid']) {
		if(!$dados['noredirect']) {
	 		$al = array("alert"=>"Novo Usuário (".$identificacaousuario_novo['iusnome'].") ja possui atribuções no SISMÉDIO, por isso não pode ser inserido","location"=>$_SERVER['HTTP_REFERER']);
	 		alertlocation($al);
		} else {
			return false;
		}
	}
	
	if($identificacaousuario_antigo['iusformacaoinicialorientador']) {
		if($identificacaousuario_antigo['iusformacaoinicialorientador']=='t') {
			$identificacaousuario_antigo['iusformacaoinicialorientador'] = 'TRUE';
		}
		
		if($identificacaousuario_antigo['iusformacaoinicialorientador']=='f') {
			$identificacaousuario_antigo['iusformacaoinicialorientador'] = 'FALSE';
		}
	}
	
	if(!$identificacaousuario_novo['iusd']) {
     	$sql = "INSERT INTO sismedio.identificacaousuario(
 	            picid, uncid, iuscpf, iusnome, iusemailprincipal, muncodatuacao,  
 	            iusdatainclusao, iusstatus, iusformacaoinicialorientador, iustipoprofessor, iuscodigoinep)
 			    VALUES (".(($identificacaousuario_antigo['picid'])?"'".$identificacaousuario_antigo['picid']."'":"NULL").", ".(($identificacaousuario_antigo['uncid'])?"'".$identificacaousuario_antigo['uncid']."'":"NULL").", '".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."', '".$dados['iusnome_']."', '".$dados['iusemailprincipal_']."',".(($identificacaousuario_antigo['muncodatuacao'])?"'".$identificacaousuario_antigo['muncodatuacao']."'":"NULL").",  
 			            NOW(), 'A', ".(($identificacaousuario_antigo['iusformacaoinicialorientador'])?$identificacaousuario_antigo['iusformacaoinicialorientador']:"NULL").", 
 			            ".(($identificacaousuario_antigo['iustipoprofessor'])?"'".$identificacaousuario_antigo['iustipoprofessor']."'":"NULL").",
 			            ".(($identificacaousuario_antigo['iuscodigoinep'])?"'".$identificacaousuario_antigo['iuscodigoinep']."'":"NULL").") returning iusd;";
     	$identificacaousuario_novo['iusd'] = $db->pegaUm($sql);
	} else {
		$sql = "UPDATE sismedio.identificacaousuario SET iusstatus='A', picid=".(($identificacaousuario_antigo['picid'])?"'".$identificacaousuario_antigo['picid']."'":"NULL").", uncid=".(($identificacaousuario_antigo['uncid'])?"'".$identificacaousuario_antigo['uncid']."'":"NULL").", 
														 iusformacaoinicialorientador=".(($identificacaousuario_antigo['iusformacaoinicialorientador'])?$identificacaousuario_antigo['iusformacaoinicialorientador']:"NULL").", 
														 iustipoprofessor=".(($identificacaousuario_antigo['iustipoprofessor'])?"'".$identificacaousuario_antigo['iustipoprofessor']."'":"NULL").",
														 iuscodigoinep=".(($identificacaousuario_antigo['iuscodigoinep'])?"'".$identificacaousuario_antigo['iuscodigoinep']."'":"NULL")."
														 WHERE iusd='".$identificacaousuario_novo['iusd']."'";
		$db->executar($sql);
	}
	
	$sql = "DELETE FROM sismedio.usuarioresponsabilidade WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."'";
	$db->executar($sql);
	
	$sql = "UPDATE sismedio.usuarioresponsabilidade SET usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."', uncid=".(($identificacaousuario_antigo['uncid'])?"'".$identificacaousuario_antigo['uncid']."'":"NULL")." WHERE rpustatus='A' AND usucpf='".$identificacaousuario_antigo['usucpf']."' AND pflcod='".$identificacaousuario_antigo['pflcod']."'";
	$db->executar($sql);
	
	$sql = "UPDATE sismedio.tipoperfil SET iusd='".$identificacaousuario_novo['iusd']."' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
	$db->executar($sql);
	
	$sql = "UPDATE sismedio.turmas SET iusd='".$identificacaousuario_novo['iusd']."' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
	$db->executar($sql);
	
	$sql = "UPDATE sismedio.orientadorturma SET iusd='".$identificacaousuario_novo['iusd']."' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
	$db->executar($sql);
	
	$sql = "UPDATE sismedio.identificacaousuario SET iusstatus='I' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$identificacaousuario_antigo['iuscpf']."' AND pflcod='".$dados['pflcod_']."'";
	$db->executar($sql);
	
	// removendo avaliações não concluidas
	$sql = "SELECT m.menid FROM sismedio.mensario m 
			INNER JOIN workflow.documento d ON d.docid = m.docid 
			WHERE iusd='".$identificacaousuario_antigo['iusd']."' AND d.esdid!='".ESD_APROVADO_MENSARIO."'";
	
	$menids = $db->carregarColuna($sql);
	
	if($menids) {
		
		$sql = "SELECT mavid FROM sismedio.mensarioavaliacoes WHERE menid IN('".implode("','",$menids)."')";
		$mavids = $db->carregarColuna($sql);
		
		if($mavids) {
			$db->executar("DELETE FROM sismedio.historicoreaberturanota WHERE mavid IN('".implode("','",$mavids)."')");
			$db->executar("DELETE FROM sismedio.mensarioavaliacoes WHERE mavid IN('".implode("','",$mavids)."')");
		}
	}
	
	$sql = "INSERT INTO sismedio.historicotrocausuario(iusdnovo, iusdantigo, pflcod, hstdata, usucpf, uncid)
    		VALUES ('".$identificacaousuario_novo['iusd']."', '".$identificacaousuario_antigo['iusd']."', '".$dados['pflcod_']."', NOW(), '".$_SESSION['usucpf']."', ".(($identificacaousuario_antigo['uncid'])?"'".$identificacaousuario_antigo['uncid']."'":"NULL").");";
	$db->executar($sql);
	
	$db->commit();
		
	gerarVersaoProjetoUniversidade(array('uncid' => $identificacaousuario_antigo['uncid']));
	
	if(!$dados['noredirect']) {
	 	$al = array("alert"=>"Troca efetuada com sucesso.","location"=>$_SERVER['HTTP_REFERER']);
	 	alertlocation($al);
	} else {
		return true;
	}
	
	
}

function ativarEquipe($dados) {
	global $db;

	if($dados['chk']) {
		
		foreach($dados['chk'] as $pflcod => $cpfs) {
			
			foreach($cpfs as $cpf) {
				
				$sql = "SELECT * FROM sismedio.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$cpf)."'";
 				$identificacaousuario = $db->pegaLinha($sql);
 				
 				if(!$dados['uncid']) $uncid = $identificacaousuario['uncid'];
 				else $uncid = $dados['uncid'];

			    $existe_usu = $db->pegaUm("select usucpf from seguranca.usuario where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."'");
    	
   				if(!$existe_usu) {
    	
				   	$sql = "INSERT INTO seguranca.usuario(
			             	usucpf, usunome, usuemail, usustatus, ususenha, suscod)
			     			VALUES ('".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', '".addslashes($identificacaousuario['iusnome'])."', '".$identificacaousuario['iusemailprincipal']."', 'A', '".md5_encrypt_senha("simecdti","")."', 'A');";
			     	$db->executar($sql);
    	
			    } else {
    	
			    	$sql = "UPDATE seguranca.usuario SET usustatus='A', suscod='A', usuemail='".$identificacaousuario['iusemailprincipal']."' WHERE usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."'";
    				$db->executar($sql);
			    }
			    
		 		$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO SISMÉDIO","email" => $identificacaousuario['iusemailprincipal']);
 				$destinatario = $identificacaousuario['iusemailprincipal'];
 				$usunome = $identificacaousuario['iusnome'];
 				$assunto = "Cadastro no SIMEC - MÓDULO SISMÉDIO";
 				$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
		 		$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo sismedio. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
 							  <p>Se for o seu primeiro acesso, o sistema solicitará que você crie uma nova senha. Se você já tiver cadastro no SIMEC, insira o seu CPF e senha. Caso tenha esquecido a sua senha de acesso ao SIMEC, clique em \"Esqueceu a senha?\" e insira o seu CPF. O sistema enviará a sua nova senha para o e-mail que você cadastrou. Em caso de dúvida, entre em contato com a sua Secretaria de Educação.</p>
 							  <p>Sua Senha de acesso é: %s</p>
 							  <br><br>* Caso você já alterou a senha acima, favor desconsiderar este e-mail.",
					 			'Prezado(a)',
					 			$identificacaousuario['iusnome'],
					 			md5_decrypt_senha($db->pegaUm("SELECT ususenha FROM seguranca.usuario WHERE usucpf='".$identificacaousuario['iuscpf']."'"),'')	
					 			);
		
		 		if(!strstr($_SERVER['HTTP_HOST'],"simec-local")){
		 			enviar_email( $remetente, $destinatario, $assunto, $conteudo );
		 		}
		 		
			    $existe_sis = $db->pegaUm("select usucpf from seguranca.usuario_sistema where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and sisid='".SIS_MEDIO."'");
			    	
			    if(!$existe_sis) {
			    		
			    	$sql = "INSERT INTO seguranca.usuario_sistema(
			         	    usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
			     			VALUES ('".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', ".SIS_MEDIO.", 'A', NULL, NOW(), 'A');";
				    	
			     	$db->executar($sql);
				    	
			    } else {
		 	    	$sql = "UPDATE seguranca.usuario_sistema SET susstatus='A', suscod='".$dados['suscod']."' WHERE usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' AND sisid='".SIS_MEDIO."'";
		 	    	$db->executar($sql);
			    }
			    
			    $sql = "INSERT INTO seguranca.historicousuario(htudsc, htudata, usucpf, sisid, suscod, usucpfadm)
    					VALUES ('Mudança realizada pela ferramenta de gerencia do sismedio.', 
    							NOW(), 
    							'".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', 
    							'".$_SESSION['sisid']."', '".$dados['suscod']."', '".$_SESSION['usucpf']."');";
			    
	 	    	$db->executar($sql);
			    
			    $existe_pfl = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and pflcod='".$pflcod."'");
    	
			    if(!$existe_pfl) {
			    	$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', '".$pflcod."');";
			     	$db->executar($sql);
			    }
			    
			    $rpuid = $db->pegaUm("select rpuid from sismedio.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and pflcod='".$pflcod."' AND rpustatus='A'");
			    
			    if($uncid) {
	    
				    if(!$rpuid) {
				    	$sql = "INSERT INTO sismedio.usuarioresponsabilidade(
				            		pflcod, usucpf, rpustatus, rpudata_inc, uncid)
				 			    VALUES ('".$pflcod."', '".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', 'A', NOW(), '".$uncid."');";
				    	
				    	$db->executar($sql);
				    } else {
				    	$sql = "UPDATE sismedio.usuarioresponsabilidade SET uncid='".$uncid."' WHERE rpuid='".$rpuid."'";
				    	$db->executar($sql);
				    }
				    
			    }
			    
			    $db->executar("delete from sismedio.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and pflcod!='".$pflcod."'");
			    
    			$db->commit();
    			
			}
			
		}
		
		
	}

    

    
 	$al = array("alert"=>"Gerenciamento executado com sucesso","location"=>$_SERVER['REQUEST_URI']);
 	alertlocation($al);
	
}

function verificarFormacaoCompleta($dados) {
	global $db;
	$sql = "SELECT foecompleto FROM sismedio.formacaoescolaridade WHERE foeid='".$dados['foeid']."'";
	$foecompleto = $db->pegaUm($sql);
	echo (($foecompleto=="t")?"T":"");
	echo (($foecompleto=="f")?"F":"");
}

function listarCursosFormacao($dados) {
	global $db;
	$sql = "SELECT cufid as codigo, cufcursodesc as descricao FROM sismedio.cursoformacao WHERE cufstatus='A' AND cufcodareageral='".$dados['cufcodareageral']."' ORDER BY cufcursodesc";
	$db->monta_combo('cufid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'cufid', '');
	
}

function alertlocation($dados) {
	
	die("<script>
		".(($dados['alert'])?"alert('".$dados['alert']."');":"")."
		".(($dados['location'])?"window.location='".$dados['location']."';":"")."
		".(($dados['javascript'])?$dados['javascript']:"")."
		 </script>");
}

function anexarDocumentoDesignacao($dados) {
	global $db;
	
   	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $campos = array("iusd" => "'".$dados['iusd']."'");
    $file = new FilesSimec( "identificacaousuarioanexo", $campos, "sismedio" );
    $file->setUpload( NULL, "arquivo" );
    
	$al = array("alert"=>"Documento de Designação gravada com sucesso","location"=>$dados['goto']);
	alertlocation($al);
    
	
}

function downloadDocumentoDesignacao($dados) {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec( "identificacaousuarioanexo", NULL, "sismedio" );
    $file->getDownloadArquivo( $dados['arqid'] );
}

function removerDocumentoDesignacao($dados) {
	global $db;
	$sql = "DELETE FROM sismedio.identificacaousuarioanexo WHERE iuaid='".$dados['iuaid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Anexo excluído com sucesso","location"=>$_SERVER['HTTP_REFERER']);
	alertlocation($al);
	
}

function listarAgencias($dados) {
	global $db;
	if($dados['muncod']) {
		$codIbge 	= $dados['muncod'];
		$nuRaioKm 	= $db->pegaUm("SELECT munmedraio FROM territorios.municipio WHERE muncod='".$dados['muncod']."'");
		
		$cliente = new SoapClient( "http://ws.mec.gov.br/AgenciasBb/wsdl",array(
																					'exceptions'	=> 0,
																					'trace'			=> true,
																					'encoding'		=> 'ISO-8859-1',
																					'cache_wsdl'    => WSDL_CACHE_NONE
		)) ;
		
		$xmlDeRespostaDoServidor = $cliente->getMunicipio( $codIbge, $nuRaioKm);
		$agencias = new SimpleXMLElement($xmlDeRespostaDoServidor);
		if($agencias->NODELIST) {
			foreach ($agencias->NODELIST as $agencia) {
				$agnum = (string) $agencia->co_agencia;
				$agcep = (string) $agencia->nu_cep_agencia;
				$agnom = (string) $agencia->no_agencia;
		        $l_agencias[$agnum] = array("codigo" =>$agnum.'_'.$agcep, "descricao" => $agnum.' - '.$agnom);    
			}
			ksort($l_agencias);
			echo '<select id="dados_agencia" onchange="" style="width: auto" class="CampoEstilo obrigatorio" name="dados_agencia">';
			echo '<option value="">SELECIONE</option>';
			foreach ($l_agencias as $agencia) {
		        echo '<option value="'.$agencia['codigo'].'">'.utf8_encode($agencia['descricao'].'').'</option>';    
			}
			echo '</select>';
		} else {
			echo "Não há agências do BB cadastradas no município escolhido. Escolha um município próximo.";
		}
	
	}
	
}

function atualizarInfoSubAtividades($subatividades) {
	if($subatividades[0]) :
		foreach($subatividades as $subatividade) :
			if(function_exists($subatividade['suafuncaosituacao'])) $subatividade['suafuncaosituacao']($subatividade);
		endforeach;
	endif;
}

function downloadDocumento($dados) {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec( "documentoatividade", NULL, "sismedio" );
    $file->getDownloadArquivo( $dados['arqid'] );
}

function removerAnexoPortaria($dados) {
	global $db;
	$sql = "DELETE FROM sismedio.portarianomeacao WHERE ponid='".$dados['ponid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Anexo excluído com sucesso","location"=>$_SERVER['HTTP_REFERER']);
	alertlocation($al);
	
	
}

function carregarDadosTurma($dados) {
	global $db;
	$sql = "SELECT * FROM sismedio.turmas t
			LEFT JOIN sismedio.identificacaousuario i ON i.iusd = t.iusd 
			LEFT JOIN territorios.municipio m ON m.muncod = t.muncod 
			WHERE t.turid='".$dados['turid']."'";
	$turma = $db->pegaLinha($sql);
	
	if($dados['return']=='json') {
		echo simec_json_encode($turma);
	} else {
		return $turma;
	}
	
}

function carregarAlunosTurma($dados) {
	global $db;
	if($dados['turid']) {
		$sql = "SELECT '<center>".((!$dados['consulta'])?"<img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirAlunoTurma('||i.iusd||');\">":"")." ".(($dados['formacaoinicial'])?"'|| CASE WHEN SUBSTR(i.iuscpf,1,3)!='SIS' THEN '<input type=radio name=\"iusd['||i.iusd||']\" value=\"TRUE\" '||CASE WHEN i.iusformacaoinicialorientador=true THEN 'checked' ELSE '' END||'> Presente <input type=radio name=\"iusd['||i.iusd||']\" value=\"FALSE\" '||CASE WHEN i.iusformacaoinicialorientador=false THEN 'checked' ELSE '' END||'> Ausente' ELSE '' END ||'":"")."</center>' as acao, i.iuscpf, i.iusnome, i.iusemailprincipal, l.lemnomeescola, tu.turdesc FROM sismedio.orientadorturma ot 
				INNER JOIN sismedio.identificacaousuario i ON i.iusd = ot.iusd 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN sismedio.turmas tu ON tu.turid = ot.turid 
				INNER JOIN sismedio.listaescolasensinomedio l ON l.lemcodigoinep::bigint = i.iuscodigoinep  
				WHERE ot.turid='".$dados['turid']."' ORDER BY i.iusnome";
		
		$cabecalho = array("&nbsp;","CPF","Nome","Email","Escola","Turma");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	} else {
		echo "<p>Nenhuma turma foi selecionada</p>";
	}
}

function criarMensario($dados) {
	global $db;
	
	$sql = "SELECT m.menid, 
    			   d.docid, 
    			   d.esdid, 
    			   m.menencontro, 
    			   m.menencontroqtd, 
    			   m.menencontrocargahoraria 
    		FROM sismedio.mensario m 
    		INNER JOIN workflow.documento d ON d.docid = m.docid 
    		WHERE m.iusd='".$dados['iusd']."' AND m.fpbid='".$dados['fpbid']."'";
	
	$mensario = $db->pegaLinha($sql);
	
	$menid 		 = $mensario['menid'];
	$docid 		 = $mensario['docid'];
	$esdid 	     = $mensario['esdid'];
	$menencontro = $mensario['menencontro'];
	$menencontroqtd = $mensario['menencontroqtd'];
	$menencontrocargahoraria = $mensario['menencontrocargahoraria'];
	
	if(!$menid) {
		
		$arrUs    = $db->pegaLinha("SELECT i.iusnome, p.pfldsc 
									FROM sismedio.identificacaousuario i 
									INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
									INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
									WHERE i.iusd='".$dados['iusd']."'");
		
		$iusnome  = $arrUs['iusnome'];
		$pfldsc   = $arrUs['pfldsc'];
		
		$referencia = $db->pegaUm("SELECT fpbmesreferencia || ' / ' || fpbanoreferencia as descricao FROM sismedio.folhapagamento WHERE fpbid='".$dados['fpbid']."'");
		
		$docid = wf_cadastrarDocumento( TPD_FLUXOMENSARIO, 'Mensário : '.$iusnome.' - '.$pfldsc.' Ref.'.$referencia );
		$esdid = ESD_EM_ABERTO_MENSARIO;
		
		$sql = "INSERT INTO sismedio.mensario(
            	iusd, fpbid, docid, pflcod)
    			SELECT '".$dados['iusd']."', 
					   '".$dados['fpbid']."', 
					   '".$docid."', 
					   (SELECT pflcod FROM sismedio.tipoperfil WHERE iusd='".$dados['iusd']."') 
		 		WHERE (SELECT menid FROM sismedio.mensario WHERE fpbid='".$dados['fpbid']."' AND iusd='".$dados['iusd']."') IS NULL
			    RETURNING menid;";
		
		$menid = $db->pegaUm($sql);
		$db->commit();
		
		if(!$menid) {
			$menid = $db->pegaUm("SELECT menid FROM sismedio.mensario WHERE fpbid='".$dados['fpbid']."' AND iusd='".$dados['iusd']."'");
		}
		
	}
	
	return array("memid"=>$menid,"docid"=>$docid,"esdid"=>$esdid,"menencontro"=>$menencontro, "menencontroqtd"=>$menencontroqtd,"menencontrocargahoraria"=>$menencontrocargahoraria);
	
}

function montaComboAvaliacao($dados) {
	global $OPT_AV;
	
	$combo .= '<select '.(($dados['consulta'])?'disabled':'').' name="'.$dados['tipo'].'[\'||foo.iusd||\']" class="CampoEstilo obrigatorio" style="width: auto;font-size:x-small" onchange="calcularNotaFinal(\'||foo.iusd||\')" id="'.$dados['tipo'].'_\'||foo.iusd||\'" \'||CASE WHEN (SELECT esdid FROM workflow.documento WHERE docid=foo.docid) IN('.ESD_APROVADO_MENSARIO.') THEN \'disabled\' ELSE \'\' END ||\'>';
	
	if($OPT_AV[$dados['tipo']]) {
		$combo .= '\'||CASE WHEN ( me.mavid IS NULL OR me.mav'.$dados['tipo'].' IS NULL ) THEN \'<option value="">Selecione</option>\' ELSE \'\' END||\'';
		foreach($OPT_AV[$dados['tipo']] as $op) {
			$combo .= '<option value="'.$op['codigo'].'" \'|| CASE WHEN me.mav'.$dados['tipo'].'=\''.$op['codigo'].'\' THEN \'selected\' ELSE \'\' END ||\'>'.$op['descricao'].'</option>';
		}
	}
	
	$combo .= '</select>';
	
	return $combo;
	

}

function avaliarEquipe($dados) {
	global $db;
	
	if($dados['iusd_avaliados']) {
		foreach($dados['iusd_avaliados'] as $iusd) {
			$dadosmensario = criarMensario(array("iusd"=>$iusd,"fpbid"=>$dados['fpbid']));

			
			if($dadosmensario['esdid']!=ESD_APROVADO_MENSARIO) {
				
				$sql = "SELECT mavid, mavfrequencia, mavatividadesrealizadas FROM sismedio.mensarioavaliacoes WHERE menid='".$dadosmensario['memid']."'";
				$ma = $db->pegaLinha($sql);
				$mavid = $ma['mavid'];
				
				if($mavid) {

					
					if(is_numeric($dados['frequencia'][$iusd]) || is_numeric($dados['atividadesrealizadas'][$iusd])) {
						
						if($dados['frequencia'][$iusd]!=$ma['mavfrequencia'] || $dados['atividadesrealizadas'][$iusd]!=$ma['mavatividadesrealizadas']) {
							$upt = "iusdavaliador='".$dados['iusdavaliador']."',";
						}
					
						$sql = "UPDATE sismedio.mensarioavaliacoes SET mavfrequencia=".((is_numeric($dados['frequencia'][$iusd]))?"'".$dados['frequencia'][$iusd]."'":"NULL").", 
															 		   mavatividadesrealizadas=".((is_numeric($dados['atividadesrealizadas'][$iusd]))?"'".$dados['atividadesrealizadas'][$iusd]."'":"NULL").",
				    												   {$upt} 
															 		   mavmonitoramento=".((is_numeric($dados['monitoramento'][$iusd]))?"'".$dados['monitoramento'][$iusd]."'":"NULL").", 
															 		   mavtotal=".((is_numeric(($dados['total'][$iusd])))?"'".($dados['total'][$iusd])."'":"NULL")." WHERE mavid='".$mavid."'";
						$db->executar($sql);
					
					}
					
				} else {

					
					if(is_numeric($dados['frequencia'][$iusd]) || is_numeric($dados['atividadesrealizadas'][$iusd])) {
					
						$sql = "INSERT INTO sismedio.mensarioavaliacoes(
		            			iusdavaliador, mavfrequencia, mavatividadesrealizadas, 
		            			mavmonitoramento, mavtotal, menid)
		    					VALUES ('".$dados['iusdavaliador']."', 
		    							 ".((is_numeric($dados['frequencia'][$iusd]))?"'".$dados['frequencia'][$iusd]."'":"NULL").", 
		    							 ".((is_numeric($dados['atividadesrealizadas'][$iusd]))?"'".$dados['atividadesrealizadas'][$iusd]."'":"NULL").", 
		            					 ".((is_numeric($dados['monitoramento'][$iusd]))?"'".$dados['monitoramento'][$iusd]."'":"NULL").", 
		            					 ".((is_numeric(($dados['total'][$iusd])))?"'".($dados['total'][$iusd])."'":"NULL").", '".$dadosmensario['memid']."') RETURNING mavid;";
						
						$mavid = $db->pegaUm($sql);
					
					}
					
				}
				
				$sql = "UPDATE sismedio.mensarioavaliacoes ma SET mavtotal=foo.total FROM (
						SELECT * FROM (
						SELECT 
						m.menid,
						mavid,
						mavtotal,
						(COALESCE((mavfrequencia*fatfrequencia),0) + COALESCE((mavatividadesrealizadas*fatatividadesrealizadas),0) + COALESCE(mavmonitoramento,0)) as total
						FROM sismedio.mensarioavaliacoes ma 
						INNER JOIN sismedio.mensario m ON m.menid = ma.menid 
						INNER JOIN sismedio.identificacaousuario u ON u.iusd = m.iusd 
						INNER JOIN sismedio.tipoperfil t ON t.iusd = u.iusd 
						INNER JOIN sismedio.fatoresdeavaliacao f ON f.fatpflcodavaliado = t.pflcod 
						) fee
						WHERE fee.mavtotal != total
						) foo 
						WHERE ma.menid = foo.menid and ma.menid='".$dadosmensario['memid']."'";
				
				$db->executar($sql);
				
				if($mavid && $dados['cpfresponsavel'][$iusd] && $dados['mavdsc'][$iusd]) {
					$sql = "INSERT INTO sismedio.historicoreaberturanota(
					            mavid, hrnfrequencia, hrnatividadesrealizadas, hrnmonitoramento, 
					            hrncpfresponsavel, hrnjustificativa, hrndata)
					    VALUES ('".$mavid."', 
					    		".((is_numeric($dados['frequencia'][$iusd]))?"'".$dados['frequencia'][$iusd]."'":"NULL").", 
					    		".((is_numeric($dados['atividadesrealizadas'][$iusd]))?"'".$dados['atividadesrealizadas'][$iusd]."'":"NULL").", 
					    		".((is_numeric($dados['monitoramento'][$iusd]))?"'".$dados['monitoramento'][$iusd]."'":"NULL").", 
					            '".$dados['cpfresponsavel'][$iusd]."', '".$dados['mavdsc'][$iusd]."', NOW());";
					$db->executar($sql);
				}
				
				$db->commit();
			
			}
		}
	}
	
	$al = array("alert"=>"Avaliações gravadas com sucesso","location"=>$_SERVER['REQUEST_URI']);
	alertlocation($al);
	
	
	
}



function condicaoEnviarMensario($fpbid,$pflcod=null) {
	global $db;
	
	if($pflcod == PFL_ORIENTADORESTUDO) {
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sismedio.mensario me 
							INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid
							INNER JOIN sismedio.identificacaousuario i ON i.iusd = me.iusd 
							INNER JOIN sismedio.orientadorturma ot ON ot.iusd = me.iusd 
							INNER JOIN sismedio.turmas tt ON tt.turid = ot.turid 		
							WHERE tt.iusd='".$_SESSION['sismedio']['orientadorestudo']['iusd']."' AND me.fpbid='".$fpbid."'");
		if(!$tot) {
			return 'Nenhuma avaliação foi salva';
		} else {
			
			$sql = "SELECT i.iusnome FROM sismedio.orientadorturma ot 
					INNER JOIN sismedio.turmas tt ON tt.turid = ot.turid 
					INNER JOIN sismedio.identificacaousuario i ON i.iusd = ot.iusd 
					LEFT JOIN sismedio.mensario me ON me.iusd = i.iusd AND me.fpbid=".$fpbid." 
					LEFT JOIN workflow.documento d ON d.docid = me.docid  AND d.esdid != ".ESD_APROVADO_MENSARIO."
					LEFT JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid
					WHERE tt.iusd='".$_SESSION['sismedio']['orientadorestudo']['iusd']."' AND ma.mavtotal IS NULL AND i.iusstatus='A'";
			
			$iusnome = $db->carregarColuna($sql);
			
			if($iusnome) {
				return 'Existem Professores Alfabetizadores sem avaliação: \n\n'.implode('\n<br>',$iusnome);
			}
			
		}
		
		$sql = "SELECT count(*) as tot FROM sismedio.respostasavaliacaocomplementar WHERE iusdavaliador='".$_SESSION['sismedio']['orientadorestudo']['iusd']."' AND fpbid='".$fpbid."'";
		$existe_respostasavaliacaocomplementar = $db->pegaUm($sql);
		
		if(!$existe_respostasavaliacaocomplementar) {
			return 'É necessário preencher a Avaliação Complementar';
		}
		
		return true;
	
	}
	
	if($pflcod == PFL_FORMADORREGIONAL) {
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sismedio.mensario me 
							INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid
							INNER JOIN sismedio.identificacaousuario i ON i.iusd = me.iusd 
							INNER JOIN sismedio.orientadorturma ot ON ot.iusd = me.iusd 
							INNER JOIN sismedio.turmas tt ON tt.turid = ot.turid 		
							WHERE tt.iusd='".$_SESSION['sismedio']['formadorregional']['iusd']."' AND me.fpbid='".$fpbid."'");
		if(!$tot) {
			return 'Nenhuma avaliação foi salva';
		} else {
			
			$sql = "SELECT i.iusnome FROM sismedio.mensario me 
					INNER JOIN workflow.documento d ON d.docid = me.docid
					LEFT JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid AND ma.iusdavaliador='".$_SESSION['sismedio']['formadorregional']['iusd']."'
					INNER JOIN sismedio.identificacaousuario i ON i.iusd = me.iusd 
					INNER JOIN sismedio.orientadorturma ot ON ot.iusd = me.iusd 
					INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_ORIENTADORESTUDO."
					INNER JOIN sismedio.turmas tt ON tt.turid = ot.turid 		
					WHERE tt.iusd='".$_SESSION['sismedio']['formadorregional']['iusd']."' AND me.fpbid='".$fpbid."' AND ma.mavtotal IS NULL AND d.esdid != ".ESD_APROVADO_MENSARIO." 
					ORDER BY i.iusnome";
			
			$iusnome = $db->carregarColuna($sql);
			
			if($iusnome) {
				return 'Existem Orientadores de Estudo sem avaliação: \n\n'.implode('\n<br>',$iusnome);
			}
			
		}
		
		
		return true;
	
	}
	
	if($pflcod == PFL_SUPERVISORIES) {
		
		$funcaoavaliacao = $db->pegaUm("SELECT tpatipoavaliacao FROM sismedio.tipoavaliacaoperfil WHERE pflcod='".$pflcod."' AND uncid='".$_SESSION['sismedio']['supervisories']['uncid']."' AND fpbid='".$fpbid."'");
		
		// tratando condição de tipos de monitoramento
		if($funcaoavaliacao=='monitoramentoTextual') return true;
		
		$sql_tot = sqlAvaliacaoSupervisor(array('uncid'=>$_SESSION['sismedio']['supervisories']['uncid'],'iusd'=>$_SESSION['sismedio']['supervisories']['iusd'],'fpbid'=>$fpbid));
		
		$sql = "SELECT COUNT(*) FROM ({$sql_tot}) ff";
		$navals = $db->pegaUm($sql);
		
		if(!$navals) {
			return true;
		}
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sismedio.mensario me 
							INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid
							WHERE me.fpbid='".$fpbid."' AND ma.iusdavaliador='".$_SESSION['sismedio']['supervisories']['iusd']."' AND ma.mavtotal IS NOT NULL");
		
		if(!$tot) {
			return 'É necessário avaliar um membro';
		}
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORADJUNTOIES) {
		
		$sql_tot = sqlAvaliacaoCoordenadorAdjuntoIES(array('uncid'=>$_SESSION['sismedio']['coordenadoradjuntoies']['uncid'],'iusd'=>$_SESSION['sismedio']['coordenadoradjuntoies']['iusd'],'fpbid'=>$fpbid));
		
		$sql = "SELECT COUNT(*) FROM ({$sql_tot}) ff";
		$navals = $db->pegaUm($sql);
		
		if(!$navals) {
			return true;
		}
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sismedio.mensario me 
							INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid
							WHERE me.fpbid='".$fpbid."' AND ma.iusdavaliador='".$_SESSION['sismedio']['coordenadoradjuntoies']['iusd']."' AND ma.mavtotal IS NOT NULL");
		
		if(!$tot) {
			return 'É necessário avaliar um membro';
		}
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORIES) {
		
		$sql_tot = sqlAvaliacaoCoordenadorIES(array('uncid'=>$_SESSION['sismedio']['universidade']['uncid'],'iusd'=>$_SESSION['sismedio']['universidade']['iusd'],'fpbid'=>$fpbid));
		
		$sql = "SELECT COUNT(*) FROM ({$sql_tot}) ff";
		$navals = $db->pegaUm($sql);
		
		if(!$navals) {
			return true;
		}
		
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sismedio.mensario me 
							INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid
							WHERE me.fpbid='".$fpbid."' AND ma.iusdavaliador='".$_SESSION['sismedio']['universidade']['iusd']."' AND ma.mavtotal IS NOT NULL");
		
		if(!$tot) {
			return 'É necessário avaliar um membro';
		}
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORLOCAL) {
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sismedio.mensario me 
							INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid
							WHERE me.fpbid='".$fpbid."' AND ma.iusdavaliador='".$_SESSION['sismedio']['coordenadorlocal']['iusd']."' AND ma.mavtotal IS NOT NULL");
		
		if(!$tot) {
			return 'É necessário avaliar um membro';
		}
		
		return true;
	
	}
	
	
	return true;

}

function posEnviarMensario($fpbid, $pflcod=null) {
	global $db;
	
	if($pflcod == PFL_ORIENTADORESTUDO) {
		
		$sql = "SELECT i.iusnome, me.docid, ma.mavtotal FROM sismedio.mensario me 
				INNER JOIN workflow.documento dc ON dc.docid = me.docid AND dc.tpdid=".TPD_FLUXOMENSARIO." 
				INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sismedio.identificacaousuario i ON i.iusd = me.iusd 
				INNER JOIN sismedio.orientadorturma ot ON ot.iusd = me.iusd 
				INNER JOIN sismedio.turmas tt ON tt.turid = ot.turid 		
				WHERE tt.iusd='".$_SESSION['sismedio']['orientadorestudo']['iusd']."' AND dc.esdid='".ESD_EM_ABERTO_MENSARIO."' AND ma.iusdavaliador='".$_SESSION['sismedio']['orientadorestudo']['iusd']."' AND me.fpbid='".$fpbid."'";
		
		
		$arrMensario = $db->carregar($sql);
		
		if($arrMensario[0]) {
			foreach($arrMensario as $mensario) {
				wf_alterarEstado( $mensario['docid'], AED_ENVIAR_MENSARIO, '', array('fpbid'=>$fpbid));
			}
		}
		
		
		$sql = "UPDATE sismedio.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sismedio.mensario me 
				INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sismedio.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sismedio']['orientadorestudo']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_FORMADORREGIONAL) {
		
		$sql = "UPDATE sismedio.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sismedio.mensario me 
				INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sismedio.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sismedio']['formadorregional']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_SUPERVISORIES) {
		
		$sql = "UPDATE sismedio.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sismedio.mensario me 
				INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sismedio.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sismedio']['supervisories']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORADJUNTOIES) {

		
		$sql = "UPDATE sismedio.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sismedio.mensario me 
				INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sismedio.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sismedio']['coordenadoradjuntoies']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORIES) {
		
		$sql = "UPDATE sismedio.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sismedio.mensario me 
				INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sismedio.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sismedio']['universidade']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORLOCAL) {
		
		$sql = "UPDATE sismedio.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sismedio.mensario me 
				INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sismedio.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sismedio']['coordenadorlocal'][$_SESSION['sismedio']['esfera']]['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	return true;

}

function carregarAjudaAvaliacao($dados) {
	global $db;
	
	$sql = "SELECT * FROM sismedio.fatoresdeavaliacao WHERE fatid='".$dados['fatid']."'";
	$fatoresdeavaliacao = $db->pegaLinha($sql);
	
	echo $fatoresdeavaliacao['fatquadrodetalhe'];
}


function carregarAvaliacaoEquipe($dados) {
	global $db;
	
	if($dados['filtro']) {
		if($dados['filtro']['iuscpf']) {
			$where[] = "foo.iuscpf='".str_replace(array(".","-"),array("",""),$dados['filtro']['iuscpf'])."'";
		}
		if($dados['filtro']['iusnome']) {
			$where[] = "removeacento(foo.iusnome) ilike removeacento('%".$dados['filtro']['iusnome']."%')";
		}
		if($dados['filtro']['pfldsc']) {
			$where[] = "foo.pfldsc='".$dados['filtro']['pfldsc']."'";
		}
	}

	$combofr = montacomboavaliacao(array('tipo'=>'frequencia','consulta'=>$dados['consulta']));
	$comboat = montacomboavaliacao(array('tipo'=>'atividadesrealizadas','consulta'=>$dados['consulta']));
	$campotl = '<input '.(($dados['consulta'])?'disabled':'').' readonly="" style="text-align:right;border-left:#888888 3px solid;color:#808080;font-size:x-small;" type="text" id="total_\'||foo.iusd||\'" name="total[\'||foo.iusd||\']" size="6" maxlength="6" value="\'||CASE WHEN me.mavtotal IS NULL THEN \'\' ELSE me.mavtotal::character varying(10) END||\'" class="CampoEstilo">';
	$campomt = '<input readonly="" style="text-align:right;border-left:#888888 3px solid;color:#808080;font-size:x-small;" type="hidden" id="monitoramento_\'||foo.iusd||\'" name="monitoramento[\'||foo.iusd||\']" size="6" maxlength="6" value="\'||CASE WHEN foo.mon=\'TRUE\' THEN fat.fatmonitoramento ELSE \'0\' END||\'" class="CampoEstilo">\'||CASE WHEN foo.mon=\'TRUE\' THEN \'<center><font style=color:blue;font-size:x-small;>Sim</font></center>\' ELSE \'<center><font style=color:red;font-size:x-small;>Não</font></center>\' END||\' ';
	
	$perfis = pegaPerfilGeral();
	if($db->testa_superuser() || in_array(PFL_EQUIPEMEC,$perfis) ||  in_array(PFL_ADMINISTRADOR,$perfis)) {
		$imgexcluir = "<img src=\"../imagens/excluir.gif\" onmouseover=\"return escape(\'Excluir avaliação\');\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirAvaliacao(\''||coalesce(me.mavid,0)||'\');\">"; 
	}

	$sql = "WITH tmp_avaliacao AS (
				SELECT m.iusd, mavfrequencia, mavatividadesrealizadas, mavmonitoramento, mavtotal, ma.mavid, CASE WHEN ma.iusdavaliador!='".$dados['iusd']."' THEN 'Avaliado pelo '||i.iusnome ELSE '' END as obs 
									   FROM sismedio.mensario m 
									   INNER JOIN sismedio.identificacaousuario ius ON ius.iusd = m.iusd
									   INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = m.menid 
									   LEFT JOIN sismedio.identificacaousuario i ON i.iusd = ma.iusdavaliador 
									   WHERE ius.uncid='".$dados['uncid']."' AND m.fpbid='".$dados['fpbid']."'
			)
			SELECT DISTINCT CASE WHEN foo.mais = '' THEN '' ELSE '<img align=\"absmiddle\" style=\"cursor:pointer\" src=\"../imagens/mais.gif\" title=\"mais\" onclick=\"exibirAvaliacaoSub(\''||foo.mais||'\', this)\"> ' END, '<img align=\"absmiddle\" src=\"../imagens/'|| CASE WHEN (SELECT esdid FROM workflow.documento WHERE docid=foo.docid) IN(".ESD_APROVADO_MENSARIO.") THEN 'money.gif' ELSE CASE WHEN me.mavtotal IS NULL THEN 'valida5.gif' WHEN me.mavtotal < 7 THEN 'valida6.gif' ELSE 'valida4.gif' END END ||'\" id=\"img_'||foo.iusd||'\"> <img align=\"absmiddle\" src=\"../imagens/ajuda.png\" width=\"16\" height=\"16\" style=\"cursor:pointer;\" onclick=\"verAjuda(\''||fat.fatid||'\');\"> 
					'||CASE WHEN (SELECT esdid FROM workflow.documento WHERE docid=foo.docid) IN(".ESD_ENVIADO_MENSARIO.",".ESD_EM_ABERTO_MENSARIO.") OR (SELECT esdid FROM workflow.documento WHERE docid=foo.docid) IS NULL THEN '{$imgexcluir} ".(($dados['consulta'] || $dados['esdid']==ESD_EM_ABERTO_MENSARIO)?"":"<img align=\"absmiddle\" src=\"../imagens/send.png\" onmouseover=\"return escape(\'Reavaliar usuário\');\" width=\"16\" height=\"16\" style=\"cursor:pointer;\" id=\"corrigir_'||foo.iusd||'\" onclick=\"mostrarCorrecaoAprovado(\''||foo.iusd||'\');\">")." ' 
							ELSE '' END||'".(($dados['consulta'])?"":"<input type=\"hidden\" name=\"iusd_avaliados[]\" value=\"'||foo.iusd||'\"><input type=\"hidden\" id=\"pfreq_'||foo.iusd||'\" value=\"'||COALESCE(fat.fatfrequencia,0)||'\"><input type=\"hidden\" id=\"pativ_'||foo.iusd||'\" value=\"'||COALESCE(fat.fatatividadesrealizadas,0)||'\">")."' as acao, 
				   replace(to_char(foo.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, 
				   foo.iusnome||CASE WHEN me.obs!='' THEN '<br><img src=../imagens/seta_filho.gif><span style=font-size:xx-small;>'||me.obs||'</span>' ELSE '' END as iusnome, 
				   foo.iusemailprincipal, 
				   foo.pfldsc,
				   CASE WHEN fat.fatfrequencia 			 IS NULL THEN '<center><span style=color:red;font-size:x-small;>Não se aplica</center>' ELSE '$combofr' END as frequencia,
				   CASE WHEN fat.fatatividadesrealizadas IS NULL THEN '<center><span style=color:red;font-size:x-small;>Não se aplica</center>' ELSE '$comboat' END as atividades,
				   CASE WHEN fat.fatmonitoramento 		 IS NULL THEN '<center><span style=color:red;font-size:x-small;>Não se aplica</center>' ELSE '$campomt' END as monitoramento,
				   '$campotl' as total 
			FROM (
			(
			
			{$dados['sql']}
			
			)

			) foo 
			INNER JOIN sismedio.fatoresdeavaliacao fat ON fat.fatpflcodavaliado = foo.pflcod
			LEFT JOIN tmp_avaliacao me ON me.iusd = foo.iusd 
			".(($where)?"WHERE ".implode(" AND ",$where):"")." 
			ORDER BY 4   
			";
			
	$arrAvaliacao = $db->carregar($sql);
	
	if($arrAvaliacao[0]) {					
	
		$cabecalho = array("&nbsp;","&nbsp;","CPF","Nome","E-mail","Perfil","Frequência","Atividades Realizadas","Monitoramento","Nota Final");
		$db->monta_lista_simples($arrAvaliacao,$cabecalho,5000,10,'N','100%',$par2);
	
	} else {
		
		if($dados['sis']=='supervisories') {
			
			echo "<p>Art. 14. Os supervisores da formação, responsáveis pela articulação entre as IES e as secretarias estaduais e distrital de educação, serão selecionados pelo dirigente da secretaria estadual ou distrital de educação e pelo Coordenador-Geral das IES, respeitandose os pré-requisitos estabelecidos para a função quanto à formação e à experiência exigidas, entre candidatos que reúnem, no mínimo, as seguintes características cumulativas:</p>
		  		  <p>I - ter Licenciatura ou Complementação Pedagógica;<br>
				     II - ser professor/coordenador pedagógico efetivo da rede de ensino, se supervisor selecionado pela secretaria estadual ou distrital;<br>
		             III - ser professor de instituição de ensino superior, ou estar cursando mestrado e/ou doutorado na área educacional, se supervisor selecionado pelo Coordenador-Geral da IES;<br>
		             IV - possuir titulação de especialização, mestrado ou doutorado; e<br>
		             V - ter disponibilidade de 20 horas semanais para dedicar-se à função, podendo ser cedido pela secretaria estadual ou distrital.</p>
				<p>Parágrafo único. Os requisitos previstos no caput deverão ser documentalmente comprovados pelo(a) supervisor(a) no ato da inscrição na IES responsável pela formação.</p>";
			
			if(!$dados['consulta']) {
				echo "<p><input type=checkbox id=declaro name=declaro onclick=\"declaracaoatribuicoes(this);\" > Declaro ter ciência das minhas atribuições.</p>";
				
				echo "<script>
						jQuery(document).ready(function() {
							jQuery(\"#salvarcontinuar\").css('display','none');
							jQuery(\"#salvar\").css('display','none');
			    			if(document.getElementById('td_acao_".AED_ENVIAR_MENSARIO."')) {
							jQuery(\"[id^='td_acao_".AED_ENVIAR_MENSARIO."']\").css('display','none');
			    			} else {
			    			jQuery(\"#declaro\").attr('disabled', 'disabled');
			    			jQuery(\"#declaro\").attr('checked', true);
			    			}
						});
					
						function declaracaoatribuicoes(obj) {
							if(obj.checked) {
								jQuery(\"[id^='td_acao_".AED_ENVIAR_MENSARIO."']\").css('display','');
							} else {
								jQuery(\"[id^='td_acao_".AED_ENVIAR_MENSARIO."']\").css('display','none');
							}
						}
					  </script>";
			}
			
			
			
		}
		
		
		if($dados['sis']=='coordenadoradjuntoies') {
			echo "	<p>Conforme o inciso II do artigo 15 da Resolução nº 4 de 27 de fevereiro de 2013, são atribuições do Coordenador-Adjunto da IES:</p> 
					<p>a) coordenar a implementação da formação e as ações de suporte tecnológico e logístico;</p>
					<p>b) organizar, em articulação com as secretarias de Educação e os coordenadores das ações do Pacto nos estados, Distrito Federal e municípios, os encontros presenciais, as atividades pedagógicas, o calendário acadêmico e administrativo, dentre outras atividades necessárias à realização da Formação;</p> 
					<p>c) exercer a coordenação acadêmica da formação;</p>
					<p>d) homologar os cadastros dos orientadores de estudo e dos professores alfabetizadores nos sistemas disponibilizados pelo MEC;</p> 
					<p>e) indicar ao coordenador-geral da IES a manutenção ou o desligamento de bolsistas;</p>
					<p>f) assegurar, juntamente com o coordenador-geral da IES, a imediata substituição de formadores que sofram qualquer impedimento no decorrer do curso, registrando-as nos sistemas disponibilizados pelo MEC;</p> 
					<p>g) recomendar a manutenção ou o desligamento dos coordenadores das ações do Pacto nos estados, Distrito Federal e municípios, dos orientadores de estudo e dos professores alfabetizadores, em articulação com as respectivas Secretarias de Educação, comunicando-as ao coordenador-geral da IES;</p> 
					<p>h) solicitar, durante a duração do curso, os pagamentos mensais aos bolsistas que tenham feito jus ao recebimento de sua respectiva bolsa, por intermédio do SGB;</p>
					<p>i) organizar o seminário final do estado, juntamente com o coordenador-geral da IES;</p> 
					<p>j) incumbir-se, na condição de pesquisador, de desenvolver, adequar e sugerir modificações na metodologia de ensino adotada, bem como conduzir análises e estudos sobre a implementação da formação, divulgando seus resultados; e</p> 
					<p>k) substituir o coordenador-geral nos impedimentos deste;</p>
					<p><input type=checkbox id=declaro name=declaro onclick=\"declaracaoatribuicoes(this);\" > Declaro ter ciência das minhas atribuições.</p>";
			
			echo "<script>
					jQuery(document).ready(function() {
						jQuery(\"#salvarcontinuar\").css('display','none');
						jQuery(\"#salvar\").css('display','none');
		    			if(document.getElementById('td_acao_".AED_ENVIAR_MENSARIO."')) {
						jQuery(\"[id^='td_acao_".AED_ENVIAR_MENSARIO."']\").css('display','none');
		    			} else {
		    			jQuery(\"#declaro\").attr('disabled', 'disabled');
		    			jQuery(\"#declaro\").attr('checked', true);
		    			}
					});
					
					function declaracaoatribuicoes(obj) {
						if(obj.checked) {
							jQuery(\"[id^='td_acao_".AED_ENVIAR_MENSARIO."']\").css('display','');
						} else {
							jQuery(\"[id^='td_acao_".AED_ENVIAR_MENSARIO."']\").css('display','none');
						}
					}
				  </script>";
			
		}
		

		
		
	}

}

function carregarAvaliacaoEquipeSub($dados) {
	global $db;
	$dados['fpbid'] = str_replace(array("#"),array(""),$dados['fpbid']);
	$sql_avaliacao = $dados['functionavaliacao']($dados);
	
	switch($dados['functionavaliacao']) {
		case 'sqlAvaliacaoSupervisor';
			$dados['sis'] = 'supervisories';
			break;
	}
	
	carregarAvaliacaoEquipe(array("sql"=>$sql_avaliacao,"fpbid"=>$dados['fpbid'],"iusd"=>$dados['iusd'],"uncid"=>$dados['uncid'],"consulta"=>$dados['consulta'],"sis"=>$dados['sis']));

}

function inserirDadosLog($dados) {
	global $db;
	
	$sql = "INSERT INTO log_historico.logsgb_sismedio(
            pboid, logrequest, logresponse, logcpf, logcnpj, logservico, 
            logdata, logerro, remid)
    		VALUES (".(($dados['pboid'])?"'".$dados['pboid']."'":"NULL").", 
    				".(($dados['logrequest'])?"'".addslashes($dados['logrequest'])."'":"NULL").", 
    				".(($dados['logresponse'])?"'".addslashes($dados['logresponse'])."'":"NULL").", 
    				".(($dados['logcpf'])?"'".$dados['logcpf']."'":"NULL").", 
    				".(($dados['logcnpj'])?"'".$dados['logcnpj']."'":"NULL").",
    				".(($dados['logservico'])?"'".$dados['logservico']."'":"NULL").", 
    				NOW(),
    				".(($dados['logerro'])?$dados['logerro']:"NULL").",
    				".(($dados['remid'])?$dados['remid']:"NULL").");";
	
	$db->executar($sql);
	$db->commit();
}

function analisaCodXML($xml,$cod) {
	if(strpos($xml, $cod.':')) {
		return 'FALSE';
	} else {
		return 'TRUE';
	}
	
}

function analisaErro($xml) {
	
	if(analisaCodXML($xml,'00015')=='FALSE') {
		return 'Função não cadastrada para o Programa';
	}

	return 'Erro SGB<br><br>'.$xml;
	
}

function sincronizarDadosUsuarioSGB($dados) {
	global $db;
	
	set_time_limit( 0 );
	
	ini_set( 'soap.wsdl_cache_enabled', '0' );
	ini_set( 'soap.wsdl_cache_ttl', 0 );
	
	$opcoes = Array(
	                'exceptions'	=> 0,
	                'trace'			=> true,
	                //'encoding'		=> 'UTF-8',
	                'encoding'		=> 'ISO-8859-1',
	                'cache_wsdl'    => WSDL_CACHE_NONE
	);
	        
	$soapClient = new SoapClient( WSDL_CAMINHO_CADASTRO, $opcoes );
	
	libxml_use_internal_errors( true );
	
	
	$sql = "SELECT i.iuscpf, i.nacid, i.iusnome, i.iusdatanascimento, i.iusnomemae, i.iussexo, m.muncod as co_municipio_ibge_nascimento, m.estuf as sg_uf_nascimento, 
			   i.eciid, lpad(i.iusagenciasugerida,4,'0') as iusagenciasugerida, m2.muncod as co_municipio_ibge, m2.estuf as sg_uf, ie.ienlogradouro, ie.iencomplemento, 
			   ie.iennumero, ie.iencep, ie.ienbairro, it.itdufdoc, it.tdoid, it.itdnumdoc, it.itddataexp, it.itdnoorgaoexp, i.iusemailprincipal
		FROM sismedio.identificacaousuario i 
		LEFT JOIN territorios.municipio m ON m.muncod = i.muncod 
		LEFT JOIN sismedio.identificaoendereco ie ON ie.iusd = i.iusd 
		LEFT JOIN territorios.municipio m2 ON m2.muncod = ie.muncod 
		LEFT JOIN sismedio.identusutipodocumento it ON it.iusd = i.iusd 
		WHERE i.iusd='".$dados['iusd']."'";
	
	$dadosusuario = $db->pegaLinha($sql);
	
	if($dadosusuario) {
		
		// consultando se cpf existe no SGB
    	$xmlRetorno = $soapClient->lerDadosBolsista( 
    	array('sistema' => SISTEMA_SGB,
              'login'   => USUARIO_SGB,
              'senha'   => SENHA_SGB,
              'nu_cpf'  => $dadosusuario['iuscpf']
    	) 
    	);
    	
    	if(!$dados['sincronizacao']) $lnscpf = $db->carregarColuna("SELECT lnscpf FROM sismedio.listanegrasgb");
    	else $lnscpf = array();
    	
    	if(!in_array($dadosusuario['iuscpf'],$lnscpf)) {
    		inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcpf'=>$dadosusuario['iuscpf'],'logservico'=>'lerDadosBolsista'));
    	} else {
    		inserirDadosLog(array('logrequest'=>'Bolsista com problemas de characteres especiais no SGB. Adicionado a lista negra.','logresponse'=>'Bolsista com problemas de characteres especiais no SGB. Adicionado a lista negra.','logcpf'=>$dadosusuario['iuscpf'],'logservico'=>'lerDadosBolsista'));
    	}
		
    	preg_match("/<nu_cpf>(.*)<\\/nu_cpf>/si", $xmlRetorno, $match);
    	
        //$xml = new SimpleXMLElement( $xmlRetorno );
        //$existecpf = (string) $xml->nu_cpf;
    	$existecpf = (string) $match[1];
    	
    	if($existecpf) $ac = 'A';
    	else $ac = 'I';
    		
    	// gravando dados do bolsista, se existir atualizar senão inserir
    	$xmlRetorno_gravarDadosBolsista = $soapClient->gravarDadosBolsista( 
    	array('sistema'  => SISTEMA_SGB,
              'login'    => USUARIO_SGB,
              'senha'    => SENHA_SGB,
           	  'acao'     => $ac,
              'dt_envio' => date( 'Y-m-d' ),
              'pessoa'   => array('nu_cpf'                        => $dadosusuario['iuscpf'],
              				      'no_pessoa'                     => removeAcentos( addslashes($dadosusuario['iusnome']) ),
                    			  'dt_nascimento' 				  => $dadosusuario['iusdatanascimento'],
                    			  'no_pai'        				  => '',
                    			  'no_mae'        				  => removeAcentos( str_replace(array("'"),array(" "),$dadosusuario['iusnomemae']) ),
                    			  'sg_sexo'       				  => $dadosusuario['iussexo'],
                    			  'co_municipio_ibge_nascimento'  => (($dadosusuario['co_municipio_ibge_nascimento'])?$dadosusuario['co_municipio_ibge_nascimento']:$dadosusuario['co_municipio_ibge']),
                    			  'sg_uf_nascimento'              => (($dadosusuario['sg_uf_nascimento'])?$dadosusuario['sg_uf_nascimento']:$dadosusuario['sg_uf']),
                    			  'co_estado_civil'               => $dadosusuario['eciid'],
                    			  'co_nacionalidade'              => $dadosusuario['nacid'],
                    			  'co_situacao_pessoa'            => 1,
                    			  'no_conjuge'                    => $dadosusuario['iusnomeconjuge'],
                    			  'ds_endereco_web'               => '',
                    			  'co_agencia_sugerida'           => $dadosusuario['iusagenciasugerida'],
								  'enderecos' 					  => array(array('co_municipio_ibge'       => $dadosusuario['co_municipio_ibge'],
																				 'sg_uf'                   => $dadosusuario['sg_uf'],
																				 'ds_endereco'             => removeAcentos( str_replace(array("'"),array(" "),$dadosusuario['ienlogradouro']) ),
																				 'ds_endereco_complemento' => removeAcentos( str_replace(array("'"),array(" "),$dadosusuario['iencomplemento']) ),
																				 'nu_endereco'             => removeAcentos( (($dadosusuario['iennumero'])?$dadosusuario['iennumero']:'0') ),
																				 'nu_cep'                  => $dadosusuario['iencep'],
																				 'no_bairro'               => removeAcentos( addslashes($dadosusuario['ienbairro']) ),
																				 'tp_endereco'             => 'R'
    																	   )
													   				 ),
			                      'documentos' 				  	  => array(array('uf_documento'       => $dadosusuario['itdufdoc'],
																			     'co_tipo_documento'  => $dadosusuario['tdoid'],
																			     'nu_documento'       => str_replace(array("\'","'"),array(" "," "),$dadosusuario['itdnumdoc']),
																			     'dt_expedicao'       => $dadosusuario['itddataexp'],
																			     'no_orgao_expedidor' => removeAcentos(str_replace(array("'"),array(" "),$dadosusuario['itdnoorgaoexp']))
													                       )
								                       				 ),
		                       	  'emails'                        => array(array('ds_email' => $dadosusuario['iusemailprincipal']
								                       				 	   ) 
								                       				 ),
           						  'formacoes'                     => array( ),
                    			  'experiencias'                  => array( ),
                    			  'telefones'                     => array( ),
                    			  'vinculacoes' 				  => array( )
			                )
		) 
		);
		
		$logerro_gravarDadosBolsista = analisaCodXML($xmlRetorno_gravarDadosBolsista,'10001');
		
    	if(!in_array($dadosusuario['iuscpf'],$lnscpf)) {
    		inserirDadosLog(array('logerro'=>$logerro_gravarDadosBolsista,'logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcpf'=>$dadosusuario['iuscpf'],'logservico'=>'gravarDadosBolsista'));
    	} else {
    		inserirDadosLog(array('logerro'=>$logerro_gravarDadosBolsista,'logrequest'=>'Bolsista com problemas de characteres especiais no SGB. Adicionado a lista negra.','logresponse'=>'Bolsista com problemas de characteres especiais no SGB. Adicionado a lista negra.','logcpf'=>$dadosusuario['iuscpf'],'logservico'=>'gravarDadosBolsista'));
    	}
    	
    	$sql = "UPDATE sismedio.identificacaousuario SET cadastradosgb=".(($logerro_gravarDadosBolsista=='TRUE')?'FALSE':'TRUE')." WHERE iusd='".$dados['iusd']."'";
    	$db->executar($sql);
    	$db->commit();
		
	}
	
}

function sincronizarDadosEntidadeSGB($dados) {
	global $db;
	
	set_time_limit( 0 );
	
	ini_set( 'soap.wsdl_cache_enabled', '0' );
	ini_set( 'soap.wsdl_cache_ttl', 0 );
	
	$opcoes = Array(
	                'exceptions'	=> 0,
	                'trace'			=> true,
	                //'encoding'		=> 'UTF-8',
	                'encoding'		=> 'ISO-8859-1',
	                'cache_wsdl'    => WSDL_CACHE_NONE
	);
	        
	$soapClient = new SoapClient( WSDL_CAMINHO_CADASTRO , $opcoes );
	
	libxml_use_internal_errors( true );
	
	$sql = "SELECT un.unicnpj, un.uninome, un.muncod, un.uniuf
			FROM sismedio.universidadecadastro u 
			INNER JOIN sismedio.universidade un ON un.uniid = u.uniid  
			WHERE u.uncid='".$dados['uncid']."'";
	
	$dadosentidade = $db->pegaLinha($sql);
	
    $xmlRetornoEntidade = $soapClient->lerDadosEntidade( array('sistema'           => SISTEMA_SGB,
                                                               'login'            => USUARIO_SGB,
                                                               'senha'            => SENHA_SGB,
                                                               'nu_cnpj_entidade' => $dadosentidade['unicnpj']
                                                               ) );
                                                               
	inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcnpj'=>$dadosentidade['unicnpj'],'logservico'=>'lerDadosEntidade'));
	
    preg_match("/<nu_cnpj_entidade>(.*)<\\/nu_cnpj_entidade>/si", $xmlRetornoEntidade, $match);
    
   	$existecnpj = (string) $match[1];
	
    $dadosEntidade = array( 'sistema'          => SISTEMA_SGB,
                            'login'            => USUARIO_SGB,
                            'senha'            => SENHA_SGB,
                            'nu_cnpj_entidade' => $dadosentidade['unicnpj'],
                            'co_tipo_entidade' => '1',
                            'no_entidade'      => $dadosentidade['uninome'],
                            'sg_entidade'      => '',
                            'co_municipio'     => $dadosentidade['muncod'],
                            'sg_uf'            => $dadosentidade['uniuf']
                                    );

    $xmlRetorno_gravaDadosEntidade   = $soapClient->gravaDadosEntidade( $dadosEntidade );
    
	$logerro_gravaDadosEntidade = analisaCodXML($xmlRetorno_gravaDadosEntidade,'10001');
    
    inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcnpj'=>$dadosentidade['unicnpj'],'logservico'=>'gravaDadosEntidade','logerro' => $logerro_gravaDadosEntidade));
    
    if($existecnpj) $logerro_gravaDadosEntidade = 'FALSE';
    if(analisaCodXML($xmlRetorno_gravaDadosEntidade,'00036') == 'FALSE') $logerro_gravaDadosEntidade = 'FALSE';
    	
   	$sql = "UPDATE sismedio.universidadecadastro SET cadastrosgb=".(($logerro_gravaDadosEntidade=='TRUE')?'FALSE':'TRUE')." WHERE uncid='".$dados['uncid']."'";
   	$db->executar($sql);
   	$db->commit();
	
}

	
function montarAvaliacaoComplementar($dados) {
	global $_respostaac;
	if($dados['itensavaliacaocomplementarcriterio'][0]) {
		foreach($dados['itensavaliacaocomplementarcriterio'] as $icc) {
			if($dados['print']=='label') echo "<td class=\"SubTituloCentro\">".$icc['iccdsc']."</td>";
			if($dados['print']=='radio') echo "<td align=center><input ".(($dados['consulta_av_com'])?"disabled":"")." type=radio name=icc[".$dados['iacid']."] ".(($_respostaac[$dados['iacid']]==$icc['iccid'])?"checked":"")." value=\"".$icc['iccid']."\"> ".(($_respostaac[$dados['iacid']]==$icc['iccid'] && $dados['consulta_av_com'])?"<input type=hidden name=icc[".$dados['iacid']."] value=\"".$icc['iccid']."\">":"")."</td>";
		}
	}
}

function avaliarComplementarEquipe($dados) {
	global $db;
	
	$esdid_mens = $db->pegaUm("SELECT d.esdid FROM sismedio.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid WHERE iusd='".$dados['iusdavaliador']."' AND fpbid='".$dados['fpbid']."'");
	
	if($esdid_mens==ESD_APROVADO_MENSARIO && $dados['menencontro']=="FALSE") {
		$al = array("alert"=>"Sua avaliação foi aprovada, e sua avaliação complementar não pode ser alterada","location"=>$dados['goto']."&fpbid=".$dados['fpbid']);
		alertlocation($al);
	}
	
	$sql = "UPDATE sismedio.mensario SET menencontro=".(($dados['menencontro'])?$dados['menencontro']:"NULL").", 
			   							 menencontroqtd=".(($dados['menencontroqtd'])?"'".$dados['menencontroqtd']."'":"NULL").", 
				 						 menencontrocargahoraria=".(($dados['menencontrocargahoraria'])?"'".$dados['menencontrocargahoraria']."'":"NULL")." 
			WHERE iusd='".$dados['iusdavaliador']."' AND fpbid='".$dados['fpbid']."'";
	
	$db->executar($sql);
	
	$sql = "DELETE FROM sismedio.respostasavaliacaocomplementar WHERE iusdavaliador='".$dados['iusdavaliador']."' AND fpbid='".$dados['fpbid']."'";
	$db->executar($sql);
	
	if($dados['icc']) {
		foreach($dados['icc'] as $iacid => $iccid) {
			if($iccid) {
				$racid = $db->pegaUm("SELECT racid FROM sismedio.respostasavaliacaocomplementar WHERE iusdavaliador='".$dados['iusdavaliador']."' AND iacid='".$iacid."' AND fpbid='".$dados['fpbid']."'");
				
				if($racid) {
					
					$sql = "UPDATE sismedio.respostasavaliacaocomplementar SET 
							iusdavaliador='".$dados['iusdavaliador']."', 
							iusdavaliado=".(($dados['iusavaliado'][$iacid])?"'".$dados['iusavaliado'][$iacid]."'":"NULL").", 
							iacid='".$iacid."', 
							iccid='".$iccid."', 
							fpbid='".$dados['fpbid']."' 
							WHERE racid='".$racid."'";
					
					$db->executar($sql);
					
				} else {
				
					$sql = "INSERT INTO sismedio.respostasavaliacaocomplementar(
				            iusdavaliador, iusdavaliado, iacid, iccid, fpbid)
				    		VALUES ('".$dados['iusdavaliador']."', ".(($dados['iusavaliado'][$iacid])?"'".$dados['iusavaliado'][$iacid]."'":"NULL").", '".$iacid."', '".$iccid."', '".$dados['fpbid']."');";
					
					$db->executar($sql);
				
				}
			}			
		}
	}
	
	$db->commit();
	
	$al = array("alert"=>"Avaliações Complementares gravadas com sucesso","location"=>$dados['goto']."&fpbid=".$dados['fpbid']);
	alertlocation($al);
	

}

function verificarTermoCompromisso($dados) {
	global $db;
	// se for equipe do mec, não precisa verificar termo
	if($dados['pflcod'] == PFL_EQUIPEMEC) return true;
	
	// verificando se coordenador local aceitou o termo de compromisso
	$termo = carregarDadosIdentificacaoUsuario(array("iusd"=>$dados['iusd'],"pflcod"=>$dados['pflcod']));
	
	if($termo) {
		$termo = current($termo);
	}
	
	if($termo['iustermocompromisso']!="t") {
		$al = array("alert"=>"Por favor preencha todos os campos obrigatórios da tela “Dados”.","location"=>"sismedio.php?modulo=principal/{$dados['sis']}/{$dados['sis']}&acao=A&aba=dados");
		alertlocation($al);
	}
}

function gerarVersaoProjetoUniversidade($dados) {
	global $db;
	
	include_once '_funcoes_universidade.php';
	ob_start();
	$versao_html = true;
	if($dados['uncid']) carregarCoordenadorIES(array('uncid'=>$dados['uncid']));
	include APPRAIZ.'sismedio/modulos/principal/universidade/visualizacao_projeto.inc';
	$html = ob_get_contents();
	ob_clean();
		
	$sql = "INSERT INTO sismedio.versoesprojetouniversidade(
            	uncid, usucpf, vpndata, vpnhtml)
    			VALUES ('".$dados['uncid']."', '".$_SESSION['usucpf']."', NOW(), '".addslashes($html)."');";
	$db->executar($sql);
	$db->commit();
	
	
}

function carregarMudancasTroca($dados) {
	global $db;
	$sql = "SELECT CASE WHEN h.hstacao='T' THEN 'Troca'
						WHEN h.hstacao='R' THEN 'Remoção' 
						WHEN h.hstacao='I' THEN 'Inserção'
						WHEN h.hstacao='F' THEN 'Mudança de turma'
						END as acao, 
			i2.iuscpf||' - '||i2.iusnome as nome_antigo, 
			i.iuscpf||' - '||i.iusnome as nome_novo,
			t1.turdesc ||' ( '||i3.iusnome||' )' as turma_antigo,
			t2.turdesc ||' ( '||i4.iusnome||' )' as turma_novo,
			p.pfldsc, u.usucpf||' - '||u.usunome as responsavel, 
			to_char(h.hstdata,'dd/mm/YYYY HH24:MI') as hstdata 
			FROM sismedio.historicotrocausuario h 
			LEFT JOIN sismedio.identificacaousuario i ON i.iusd = h.iusdnovo 
			LEFT JOIN sismedio.identificacaousuario i2 ON i2.iusd = h.iusdantigo 
			LEFT JOIN seguranca.perfil p ON p.pflcod = h.pflcod 
			LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf 
			LEFT JOIN sismedio.turmas t1 ON t1.turid = h.turidantigo 
			LEFT JOIN sismedio.identificacaousuario i3 ON i3.iusd = t1.iusd 
			LEFT JOIN sismedio.turmas t2 ON t2.turid = h.turidnovo 
			LEFT JOIN sismedio.identificacaousuario i4 ON i4.iusd = t2.iusd
			WHERE h.uncid='".$dados['uncid']."' ORDER BY h.hstdata";
	
	$mudancas = $db->carregar($sql);
	
	return $mudancas;

}



function verificarValidacaoVisualizacaoProjeto($dados) {

	$maxCoordenadorAjunto = numeroMaximoCoordenadorAjuntoIES($dados);
	$numCoordenadorAjunto = numeroCoordenadorAdjuntoIES($dados);
	if($numCoordenadorAjunto>$maxCoordenadorAjunto) {
		$al = array("alert"=>"Há mais Coordenadores Adjuntos do que o limite permitido pelo MEC. Reveja a composição da Equipe da IES.","location"=>"sismedio.php?modulo=principal/universidade/universidade&acao=A&aba=recursos_humanos");
		alertlocation($al);
	}

	
}

function processarPagamentoBolsistaSGB($dados) {
	global $db;
	
	$sql = "SELECT * FROM sismedio.pagamentobolsista WHERE pboid='".$dados->id."'";
	$pagamentobolsista = $db->pegaLinha($sql);
	
	if($dados->situacao->codigo!='') {
		if($dados->situacao->codigo=='10001' || 
		   $dados->situacao->codigo=='00023' || 
		   $dados->situacao->codigo=='00025') {
			echo wf_alterarEstado( $pagamentobolsista['docid'], AED_ENVIAR_PAGAMENTO_SGB, $cmddsc = '', array());
		} elseif($dados->situacao->codigo=='10002') {
			echo wf_alterarEstado( $pagamentobolsista['docid'], AED_NAOAUTORIZAR_PAGAMENTO, $cmddsc = 'Erro retornado pelo FNDE: '.$dados->situacao->codigo.' / '.$dados->situacao->descricao, array());
		} elseif($dados->situacao->codigo=='00058') {
			
			if($pagamentobolsista['pboparcela']) {
				
				$novaparcela = ($pagamentobolsista['pboparcela']+1);
				
			} else {
			
				$novaparcela = $db->pegaUm("SELECT (rfuparcela+1) as novaparcela FROM sismedio.folhapagamentouniversidade f 
							 				INNER JOIN sismedio.universidadecadastro u ON u.uncid = f.uncid 
							 				WHERE u.uniid='".$pagamentobolsista['uniid']."' AND f.fpbid='".$pagamentobolsista['fpbid']."'");
			}
			
			$sql = "UPDATE sismedio.pagamentobolsista SET remid=null, pboparcela='".$novaparcela."' WHERE pboid='".$pagamentobolsista['pboid']."'";
			$db->executar($sql);
			$db->commit();
			
		} else {
			echo wf_alterarEstado( $pagamentobolsista['docid'], AED_RECUSAR_PAGAMENTO, $cmddsc = 'Erro retornado pelo FNDE: '.$dados->situacao->codigo.' / '.$dados->situacao->descricao, array());
			$sql = "UPDATE sismedio.pagamentobolsista SET remid=null WHERE pboid='".$pagamentobolsista['pboid']."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	
}

function consultarDetalhesAvaliacoes($dados) {
	global $db;
  
	$sql = "SELECT CASE WHEN ptc.muncod IS NOT NULL THEN 'Municipal ('|| mun.estuf ||' / '|| mun.mundescricao ||')'
						WHEN ptc.estuf IS NOT NULL THEN 'Estadual ('|| est.estuf || ' / ' || estdescricao ||')' 
						ELSE '' END as esfera, 
					uni.unisigla || ' - ' || uni.uninome as universidade, 
					per.pfldsc, doc.docid, esd.esddsc, m.fpbid, m.iusd, m.menid, i.iusnome, me.mesdsc||'/'||fa.fpbanoreferencia as periodo, f.fatfrequencia, f.fatatividadesrealizadas, f.fatmonitoramento FROM sismedio.mensario m 
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = m.iusd 
			LEFT JOIN sismedio.universidadecadastro unc ON unc.uncid = i.uncid 
			LEFT JOIN sismedio.universidade uni ON uni.uniid = unc.uniid 
			LEFT JOIN sismedio.pactoidadecerta ptc ON ptc.picid = i.picid 
			LEFT JOIN territorios.municipio mun ON mun.muncod = ptc.muncod 
			LEFT JOIN territorios.estado est ON est.estuf = ptc.estuf  
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil per ON per.pflcod = t.pflcod
			INNER JOIN sismedio.fatoresdeavaliacao f ON f.fatpflcodavaliado = t.pflcod
			INNER JOIN sismedio.folhapagamento fa ON fa.fpbid = m.fpbid 
			INNER JOIN public.meses me ON me.mescod::integer = fa.fpbmesreferencia
			INNER JOIN workflow.documento doc ON doc.docid = m.docid AND doc.tpdid=".TPD_FLUXOMENSARIO."
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE menid='".$dados['menid']."'";
	
	$mensario = $db->pegaLinha($sql);
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	echo "<tr><td class=SubTituloDireita width=25%>Avaliado:</td><td>".$mensario['iusnome']."</td></tr>";
	echo "<tr><td class=SubTituloDireita width=25%>Perfil:</td><td>".$mensario['pfldsc']."</td></tr>";
	if($mensario['universidade']) echo "<tr><td class=SubTituloDireita width=25%>Universidade:</td><td>".$mensario['universidade']."</td></tr>";
	if($mensario['esfera']) echo "<tr><td class=SubTituloDireita width=25%>Esfera:</td><td>".$mensario['esfera']."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Período:</td><td>".$mensario['periodo']."</td></tr>";
	echo "</table>";
	

	$sql = "SELECT foo.iusnome,
				   foo.pfldsc,
				   foo.frequencia,
				   ".montarCaseAvaliacao(array("criterio"=>"frequencia"))." as fr, 
				   foo.atividadesrealizadas,
				   ".montarCaseAvaliacao(array("criterio"=>"atividadesrealizadas"))." as at,
				   foo.monitoramento as mt,
				   CASE WHEN foo.mavfrequencia IS NULL OR foo.frequencia='<span style=color:red;>Não se aplica</font>' THEN '0' ELSE foo.frequencia END::numeric + CASE WHEN foo.mavatividadesrealizadas IS NULL THEN '0' ELSE foo.atividadesrealizadas END::numeric + CASE WHEN foo.mavmonitoramento IS NULL OR foo.monitoramento='<span style=color:red;>Não se aplica</font>' THEN '0' ELSE foo.monitoramento END::numeric as to
			FROM (
			SELECT i.iusnome, 
				   p.pfldsc,
				   m.mavfrequencia,
				   m.mavatividadesrealizadas,
				   m.mavmonitoramento,
				   CASE WHEN '".(($mensario['fatfrequencia'])?$mensario['fatfrequencia']:"")."'='' THEN '<span style=color:red;>Não se aplica</font>' ELSE ROUND((m.mavfrequencia*".(($mensario['fatfrequencia'])?$mensario['fatfrequencia']:"0")."),2)::text END as frequencia, 
				   CASE WHEN '".(($mensario['fatatividadesrealizadas'])?$mensario['fatatividadesrealizadas']:"")."'='' THEN '<span style=color:red;>Não se aplica</font>' ELSE ROUND((m.mavatividadesrealizadas*".(($mensario['fatatividadesrealizadas'])?$mensario['fatatividadesrealizadas']:"0")."),2)::text END as atividadesrealizadas,
				   CASE WHEN '".(($mensario['fatmonitoramento'])?$mensario['fatmonitoramento']:"")."'='' THEN '<span style=color:red;>Não se aplica</font>' ELSE ROUND((m.mavmonitoramento),2)::text END as monitoramento 
		    FROM sismedio.mensarioavaliacoes m
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = m.iusdavaliador 
			LEFT JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			LEFT JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE m.menid='".$mensario['menid']."') foo ORDER BY foo.iusnome";
	
	echo "<p align=center><b>Nota Avaliação</b></p>";	
	if($mensario['menid']) {
		$cabecalho = array("Avaliador","Perfil","Frequencia","Op. Frequencia","Atividades realizadas","Op. Atividades realizadas","Monitoramento","Nota Final");
		$db->monta_lista_simples($sql,$cabecalho,5000,10,'N','95%','center');
	} else echo "<p align=center style=color:red;>Não existem avaliações</p>";

	echo "<p align=center><b>Fluxo da avaliação</b></p>";	
	if($mensario['docid']) {
		fluxoWorkflowInterno(array('docid'=>$mensario['docid']));
	} else echo "<p align=center style=color:red;>Não existem avaliações</p>";
	

	if($mensario['iusd'] && $mensario['fpbid']) {
	
		$sql_atv_com = "SELECT i.iusnome, p.pfldsc, ia.iacdsc, ic.iccdsc, ic.iccvalor FROM sismedio.respostasavaliacaocomplementar r 
				INNER JOIN sismedio.identificacaousuario i ON i.iusd = r.iusdavaliador 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				INNER JOIN sismedio.itensavaliacaocomplementar ia ON ia.iacid = r.iacid 
				INNER JOIN sismedio.itensavaliacaocomplementarcriterio ic ON ic.iccid = r.iccid 
				WHERE r.iusdavaliado='".$mensario['iusd']."' AND r.fpbid='".$mensario['fpbid']."' ORDER BY ia.iacdsc, i.iusnome";
		
		$existe = $db->pegaUm("SELECT count(*) FROM (".$sql_atv_com.") foo");
		
		$sql = "(
				{$sql_atv_com}
				) UNION ALL (
				SELECT '', '', '', CASE WHEN AVG(ic.iccvalor) > 0 THEN 'Média' ELSE '<span style=color:red;>Não existem avaliações complementares</span>' END as l, AVG(ic.iccvalor) as media FROM sismedio.respostasavaliacaocomplementar r 
				INNER JOIN sismedio.identificacaousuario i ON i.iusd = r.iusdavaliador 
				INNER JOIN sismedio.itensavaliacaocomplementar ia ON ia.iacid = r.iacid 
				INNER JOIN sismedio.itensavaliacaocomplementarcriterio ic ON ic.iccid = r.iccid 
				WHERE r.iusdavaliado='".$mensario['iusd']."' AND r.fpbid='".$mensario['fpbid']."'
				)";
			
	}
	
	if($existe) {
		echo "<br>";
		echo "<p align=center><b>Nota Avaliação Complementar</b></p>";
		echo "<div style=height:300px;overflow:auto;>";
		$cabecalho = array("Avaliador","Perfil","Critério","Avaliação","Valor da opção");
		$db->monta_lista_simples($sql,$cabecalho,5000,10,'N','95%','center');
		echo "</div>";
	}
	
}


function fluxoWorkflowInterno($dados) {
	global $db;
	$documento = wf_pegarDocumento( $dados['docid'] );
	$atual = wf_pegarEstadoAtual( $dados['docid'] );
	$historico = wf_pegarHistorico( $dados['docid'] );
	
	?>
			<script type="text/javascript">
			
			IE = !!document.all;
			
			function exebirOcultarComentario( docid, linha )
			{
				id = 'comentario_' + docid + '_' + linha;
				div = document.getElementById( id );
				if ( !div )
				{
					return;
				}
				var display = div.style.display != 'none' ? 'none' : 'table-row';
				if ( display == 'table-row' && IE == true )
				{
					display = 'block';
				}
				div.style.display = display;
			}
			
		</script>
	<table class="listagem" cellspacing="0" cellpadding="3" align="center" style="width: 95%;">
		<thead>
			<?php if ( count( $historico ) ) : ?>
				<tr>
					<td style="width: 20px;text-align:center;">Seq.</td>
					<td style="width: 200px;text-align:center;"">Estado do pagamento</td>
					<td style="width: 90px;text-align:center;"">Quem fez</td>
					<td style="width: 120px;text-align:center;"">Quando fez</td>
					<td style="width: 17px;text-align:center;"">&nbsp;</td>
				</tr>
			<?php endif; ?>
		</thead>
		<?php $i = 1; ?>
		<?php foreach ( $historico as $item ) : ?>
			<?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
			<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
				<td align="right"><?=$i?>.</td>
				<td>
					<?php echo $item['esddsc']; ?>
				</td>
				<td>
					<?php echo $item['usunome']; ?>
				</td>
				<td>
					<?php echo $item['htddata']; ?>
				</td>
				<td style="text-align: center;">
					<?php if( $item['cmddsc'] ) : ?>
						<img
							align="middle"
							style="cursor: pointer;"
							src="http://<?php echo $_SERVER['SERVER_NAME'] ?>/imagens/restricao.png"
							onclick="exebirOcultarComentario( '<?php echo $dados['docid']; ?>', '<?php echo $i; ?>' );"
						/>
					<?php endif; ?>
				</td>
			</tr>
			<tr id="comentario_<? echo $dados['docid']; ?>_<?php echo $i; ?>" style="display: none;" bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
				<td colspan="5">
					<div >
						<?php echo simec_htmlentities( $item['cmddsc'] ); ?>
					</div>
				</td>
			</tr>
			<?php $i++; ?>
		<?php endforeach; ?>
		<?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
		<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
			<td style="text-align: right;" colspan="5">
				Estado atual: <span style="font-size: 13px;"><b><?php echo $atual['esddsc']; ?></b></span>
			</td>
		</tr>
	</table>
	<?
	
}

function montarCaseAvaliacao($dados) {
	global $OPT_AV;
	if($OPT_AV[$dados['criterio']]) {
		$case = "CASE ";
		foreach($OPT_AV[$dados['criterio']] as $reg) {
			$case .= "WHEN foo.mav".$dados['criterio']."='".$reg['codigo']."' THEN '".$reg['descricao']."' ";
		}
		$case .= "ELSE '<span style=color:red;>Não se aplica</font>' ";
		$case .= "END";
	}
	
	return $case;

}

function aprovarEquipe($dados) {
	global $db;
	
	if($dados['menid']) {
		foreach($dados['menid'] as $menid) {
			
			$sql = "SELECT * FROM sismedio.mensario m 
					INNER JOIN sismedio.tipoperfil t ON t.iusd = m.iusd 
					INNER JOIN workflow.documento d ON d.docid = m.docid 
					WHERE menid='".$menid."'";
			
			$arrMensario = $db->pegaLinha($sql);
			
			if(($arrMensario['pflcod']==PFL_PROFESSORALFABETIZADOR || $arrMensario['pflcod']==PFL_FORMADORIES || $arrMensario['pflcod']==PFL_COORDENADORPEDAGOGICO) && $arrMensario['esdid']==ESD_EM_ABERTO_MENSARIO) {
				$result = wf_alterarEstado( $arrMensario['docid'], AED_APROVAR_EMABERTO_MENSARIO, $cmddsc = '', array('fpbid'=>$arrMensario['fpbid'],'pflcod'=>$arrMensario['pflcod'],'menid'=>$menid));
			} else {
				$result = wf_alterarEstado( $arrMensario['docid'], AED_APROVAR_MENSARIO, $cmddsc = '', array('menid'=>$menid));
			}
			
		}
	}

	$al = array("alert"=>"Equipe aprovada com sucesso","location"=>"sismedio.php?modulo=".$dados['modulo']."&acao=A&aba=aprovarusuario&fpbid=".$dados['fpbid']."&pflcodaprovar=".$dados['pflcodaprovar']);
	alertlocation($al);
	
}

function posAprovarMensario($menid) {
	global $db;
	
	$sql = "SELECT	t.tpeid, m.iusd, m.fpbid, p.pflcod, p.pfldsc, i.iuscpf, i.iusnaodesejosubstituirbolsa, i.iusnome, f.fpbmesreferencia, f.fpbanoreferencia, pp.plpvalor, un.uniid FROM sismedio.mensario m 
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = m.iusd
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sismedio.folhapagamento f ON f.fpbid = m.fpbid 
			INNER JOIN sismedio.pagamentoperfil pp ON pp.pflcod = t.pflcod 
			INNER JOIN sismedio.universidadecadastro un ON un.uncid = i.uncid 
			WHERE m.menid='".$menid."'";
	
	$arrInfo = $db->pegaLinha($sql);
	
		
	$sql = "SELECT 'Não foi possível criar o registro de bolsa para ".str_replace(array("'"),array(" "),$arrInfo['iusnome']).", pois a bolsa ja foi paga para ' || i.iusnome || ' => ' || 'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao FROM sismedio.pagamentobolsista p 
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = p.iusd 
			INNER JOIN sismedio.folhapagamento f ON f.fpbid = p.fpbid 
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE p.tpeid='".$arrInfo['tpeid']."' AND p.fpbid='".$arrInfo['fpbid']."'";
	
	$descricao = $db->pegaUm($sql);
	
	if($descricao) {
		die("<script>alert('".$descricao."');window.close();</script>");
	} else {
		$docid = wf_cadastrarDocumento(TPD_PAGAMENTOBOLSA, "Pagamento - ".$arrInfo['pfldsc']." - (".$arrInfo['iuscpf'].")".$arrInfo['iusnome']." - ".$arrInfo['fpbmesreferencia']."/".$arrInfo['fpbanoreferencia']);
		
		$sql = "INSERT INTO sismedio.pagamentobolsista(
	            iusd, fpbid, docid, cpfresponsavel, pbodataenvio, pbovlrpagamento, 
	            pflcod, uniid, tpeid)
	    VALUES ('".$arrInfo['iusd']."', '".$arrInfo['fpbid']."', '".$docid."', '".$_SESSION['usucpf']."', NOW(), '".$arrInfo['plpvalor']."', 
	            '".$arrInfo['pflcod']."', '".$arrInfo['uniid']."', '".$arrInfo['tpeid']."');";
		
		$db->executar($sql);
		$db->commit();
		
		return false;
		
	}
	
	return false;
	
	
}

function calculaPorcentagemUsuarioAtivos($dados) {
	global $db;
	
	if($_REQUEST['modulo']=='principal/universidade/universidadeexecucao') {
		$sql_equipe = sqlEquipeCoordenadorIES(array("uncid"=>$_SESSION['sismedio']['universidade']['uncid']));
		$sis = 'universidade';
	}
	
	if($_REQUEST['modulo']=='principal/coordenadoradjuntoies/coordenadoradjuntoies') {
		$sql_equipe = sqlEquipeCoordenadorAdjunto(array("uncid"=>$_SESSION['sismedio']['coordenadoradjuntoies']['uncid']));
		$sis = 'coordenadoradjuntoies';
	}
	
	if($_REQUEST['modulo']=='principal/supervisories/supervisories') {
		$sql_equipe = sqlEquipeSupervisor(array("uncid"=>$_SESSION['sismedio']['supervisories']['uncid']));
		$sis = 'supervisories';
	}
	
	if($_REQUEST['modulo']=='principal/formadories/formadories') {
		$sql_equipe = sqlEquipeFormador(array("iusd"=>$_SESSION['sismedio']['formadories']['iusd'],"uncid"=>$_SESSION['sismedio']['formadories']['uncid']));
		$sis = 'formadories';
	}
	
	if($_REQUEST['modulo']=='principal/orientadorestudo/orientadorestudo') {
		$sql_equipe = sqlEquipeOrientador(array("iusd"=>$_SESSION['sismedio']['orientadorestudo']['iusd'],"uncid"=>$_SESSION['sismedio']['orientadorestudo']['uncid']));
		$sis = 'orientadorestudo';
	}
	
	if($_REQUEST['modulo']=='principal/coordenadorlocal/coordenadorlocalexecucao') {
		$sql_equipe_p = sqlEquipeCoordenadorLocal(array("picid"=>$_SESSION['sismedio']['coordenadorlocal'][$_SESSION['sismedio']['esfera']]['picid']));
		$sis = 'coordenadorlocal';
	}
	
	if($sql_equipe_p) {
		$sql_total = "SELECT COUNT(*) as tot FROM ({$sql_equipe_p}) foo WHERE foo.status='A' AND CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." OR foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN foo.iusd in( SELECT i.iusd FROM sismedio.identificacaousuario i INNER JOIN sismedio.pactoidadecerta p ON p.picid = i.picid INNER JOIN workflow.documento d ON d.docid = p.docidturma WHERE d.esdid='".ESD_FECHADO_TURMA."') ELSE true END";
		$total = $db->pegaUm($sql_total);
		
		$sql_total_a = "SELECT COUNT(*) as tot FROM ({$sql_equipe_p}) foo WHERE foo.status='A' AND CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." OR foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN foo.iusd in( SELECT i.iusd FROM sismedio.identificacaousuario i INNER JOIN sismedio.pactoidadecerta p ON p.picid = i.picid INNER JOIN workflow.documento d ON d.docid = p.docidturma WHERE d.esdid='".ESD_FECHADO_TURMA."') ELSE true END";
		$total_a = $db->pegaUm($sql_total_a);
		
		if($total) $apassituacao = round(($total_a/$total)*100);
		
		gerenciarAtividadePacto(array('iusd'=>$_SESSION['sismedio']['coordenadorlocal'][$_SESSION['sismedio']['esfera']]['iusd'],'apadatainicio'=>$apadatainicio,'apadatafim'=>$apadatafim,'apassituacao'=>$apassituacao,'suaid'=>$dados['suaid'],'picid'=>$_SESSION['sismedio']['coordenadorlocal'][$_SESSION['sismedio']['esfera']]['picid']));
	}
	
	if($sql_equipe) {
		$sql_total = "SELECT COUNT(*) as tot FROM ({$sql_equipe}) foo WHERE CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." OR foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN foo.iusd in( SELECT i.iusd FROM sismedio.identificacaousuario i INNER JOIN sismedio.pactoidadecerta p ON p.picid = i.picid INNER JOIN workflow.documento d ON d.docid = p.docidturma WHERE d.esdid='".ESD_FECHADO_TURMA."') ELSE true END";
		$total = $db->pegaUm($sql_total);
		
		$sql_total_a = "SELECT COUNT(*) as tot FROM ({$sql_equipe}) foo WHERE foo.status='A' AND CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." OR foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN foo.iusd in( SELECT i.iusd FROM sismedio.identificacaousuario i INNER JOIN sismedio.pactoidadecerta p ON p.picid = i.picid INNER JOIN workflow.documento d ON d.docid = p.docidturma WHERE d.esdid='".ESD_FECHADO_TURMA."') ELSE true END";
		$total_a = $db->pegaUm($sql_total_a);
		
		$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sismedio'][$sis]['uncid']));
		
		if($total) $aunsituacao = round(($total_a/$total)*100);
		gerenciarAtividadeUniversidade(array('iusd'=>$_SESSION['sismedio'][$sis]['iusd'],'aundatafim'=>$aundatafim,'aundatainicio'=>$aundatainicio,'aunsituacao'=>$aunsituacao,'ecuid'=>$ecuid,'suaid'=>$dados['suaid']));
	}
	
	
}

function gerenciarAtividadeUniversidade($dados) {
	global $db;
	
	$sql = "SELECT aunid FROM sismedio.atividadeuniversidade a 
			WHERE suaid='".$dados['suaid']."' AND ecuid='".$dados['ecuid']."'".(($dados['iusd'])?" AND iusd='".$dados['iusd']."'":"");
	
	$aunid = $db->pegaUm($sql);
	
	if($aunid) {
		
		$sql = "UPDATE sismedio.atividadeuniversidade SET 
				aunsituacao=".(($dados['aunsituacao'])?"'".$dados['aunsituacao']."'":"0").", 
				aundatainicio=".(($dados['aundatainicio'])?"'".$dados['aundatainicio']."'":"NULL").", 
				aundatafim=".(($dados['aundatafim'])?"'".$dados['aundatafim']."'":"NULL")."
			    WHERE aunid='".$aunid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sismedio.atividadeuniversidade(
	            suaid, aunsituacao, aundatainicio, aundatafim, aunstatus, 
	            ecuid
	            ".(($dados['iusd'])?",iusd":"").")
			    VALUES (".(($dados['suaid'])?"'".$dados['suaid']."'":"0").", 
			    		".(($dados['aunsituacao'])?"'".$dados['aunsituacao']."'":"NULL").", 
			    		".(($dados['aundatainicio'])?"'".$dados['aundatainicio']."'":"NULL").", 
			    		".(($dados['aundatafim'])?"'".$dados['aundatafim']."'":"NULL").", 
			    		'A', 
			    		'".$dados['ecuid']."'
			    		".(($dados['iusd'])?",'".$dados['iusd']."'":"").");";
		
		$db->executar($sql);
		
	}
	
	$db->commit();
	
}

function gerenciarAtividadePacto($dados) {
	global $db;
	
	$sql = "SELECT apaid FROM sismedio.atividadepacto WHERE suaid='".$dados['suaid']."' AND picid='".$dados['picid']."'".(($dados['iusd'])?" AND iusd='".$dados['iusd']."'":"");
	$apaid = $db->pegaUm($sql);
	
	if($apaid) {
		
		$sql = "UPDATE sismedio.atividadepacto SET 
				apassituacao=".(($dados['apassituacao'])?"'".$dados['apassituacao']."'":"NULL").", 
				apadatainicio=".(($dados['apadatainicio'])?"'".$dados['apadatainicio']."'":"NULL").",
				apadatafim=".(($dados['apadatafim'])?"'".$dados['apadatafim']."'":"NULL")."
			    WHERE apaid='".$apaid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sismedio.atividadepacto(
	            suaid, picid, apassituacao, apadatainicio, apadatafim, 
	            apastatus
	            ".(($dados['iusd'])?",iusd":"").")
			    VALUES ('".$dados['suaid']."', '".$dados['picid']."', 
			    		".(($dados['apassituacao'])?"'".$dados['apassituacao']."'":"NULL").", 
			    		".(($dados['apadatainicio'])?"'".$dados['apadatainicio']."'":"NULL").", 
			    		".(($dados['apadatafim'])?"'".$dados['apadatafim']."'":"NULL").", 'A'
			    		".(($dados['iusd'])?",'".$dados['iusd']."'":"").");";
		
		$db->executar($sql);
		
	}
	
	$db->commit();
	
}

function carregarExecucaoAtividadeUniversidade($dados) {
	global $db;
	
	$execucao_atividade = $db->pegaLinha("SELECT ROUND(AVG(aunsituacao)) as apassituacao, 
											  to_char(MIN(aundatainicio),'dd/mm/YYYY') as apadatainicio, 
											  to_char(MAX(aundatafim),'dd/mm/YYYY') as apadatafim 
											  FROM sismedio.subatividades su
										  	  INNER JOIN sismedio.atividadeuniversidade ap ON su.suaid = ap.suaid 
										  	  INNER JOIN sismedio.estruturacurso es ON es.ecuid = ap.ecuid 
										   	  WHERE su.atiid='".$dados['atiid']."' AND es.uncid='".$dados['uncid']."'".(($dados['iusd'])?" AND ap.iusd='".$dados['iusd']."'":""));
		
	return $execucao_atividade;
	
}

function carregarExecucaoSubAcaoUniversidade($dados) {
	global $db;
		
	$atividadeuni = $db->pegaLinha("SELECT aunid,
											 aunsituacao as apassituacao, 
										     to_char(aundatainicio,'dd/mm/YYYY') as apadatainicio,
											 to_char(aundatafim,'dd/mm/YYYY') as apadatafim,
										     to_char(aundatainicioprev,'dd/mm/YYYY') as apadatainicioprev,
											 to_char(aundatafimprev,'dd/mm/YYYY') as apadatafimprev
									  FROM sismedio.atividadeuniversidade au 
									  INNER JOIN sismedio.estruturacurso es ON es.ecuid = au.ecuid
									  WHERE suaid='".$dados['suaid']."' AND es.uncid='".$dados['uncid']."'".(($dados['iusd'])?" AND au.iusd='".$dados['iusd']."'":""));
	
	return $atividadeuni;
	
	
}

function pegarEstruturaCurso($dados) {
	global $db;
	$sql = "SELECT ecuid FROM sismedio.estruturacurso WHERE uncid='".$dados['uncid']."'";
	$ecuid = $db->pegaUm($sql);
	
	if(!$ecuid) {
		
		$sql = "INSERT INTO sismedio.estruturacurso(
        	    uncid, muncod, ecustatus)
    			VALUES ('".$dados['uncid']."', NULL, 'A') RETURNING ecuid;";
		
		$ecuid = $db->pegaUm($sql);
		$db->commit();
		
	}
	
	return $ecuid;
	
}

function carregarPeriodoReferencia($dados) {
	global $db;
	
	if($dados['pflcod_avaliador'] == PFL_EQUIPEMEC) $dados['pflcod_avaliador'] = null;
	
	if($dados['fpbid'] && !is_numeric($dados['fpbid'])) {
		$al = array("alert"=>"Período de Referência não identificado. Tente novamente","location"=>"sismedio.php?modulo=".$_REQUEST['modulo']."&acao=".$_REQUEST['acao']."&aba=".$_REQUEST['aba']);
		alertlocation($al);
	}
	
	$perfis = pegaPerfilGeral();
	if(!$perfis) $perfis = array();
	
	if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis)) {
		$fpbstatus = "f.fpbstatus IN('A','I')";
	} elseif($dados['fpbano2015']) {
		$fpbstatus = "f.fpbstatus IN('A','I') AND fpbanoreferencia='2015'";
	} else {
		$fpbstatus = "f.fpbstatus='A'";
	}
	
	if($dados['fpbid']) {
		$existe = $db->pegaUm("SELECT fpbid FROM sismedio.folhapagamento f WHERE {$fpbstatus} AND fpbid='".$dados['fpbid']."'");
		if(!$existe) {
			$al = array("alert"=>"Período de Referência não identificado. Tente novamente","location"=>"sismedio.php?modulo=".$_REQUEST['modulo']."&acao=".$_REQUEST['acao']."&aba=".$_REQUEST['aba']);
			alertlocation($al);
		}
	
	}
	
	
	
	$sql = "SELECT f.fpbid as codigo, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao 
			FROM sismedio.folhapagamento f 
			INNER JOIN sismedio.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid AND rf.rfustatus='A'
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE {$fpbstatus} AND rf.uncid='".$dados['uncid']."' ".(($dados['pflcod_avaliador'])?"AND rf.pflcod='".$dados['pflcod_avaliador']."'":"AND rf.pflcod IS NULL")." AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd') 
			ORDER BY to_char((fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date,'YYYY-mm-dd')
			{$limit}";
	
	$sql_tot = "SELECT count(*) as tot FROM ($sql) as foo";
	$tot = $db->pegaUm($sql_tot);
	
	if(!$tot) {
		echo "<br><fieldset><legend>Aviso</legend>Não existem períodos de referências habilitados.</fieldset><br>";
	} else {
		if(!$dados['somentecombo']) echo "Selecione período de referência : ";
		$db->monta_combo('fpbid', $sql, 'S', 'Selecione', 'selecionarPeriodoReferencia', '', '', '', 'S', 'fpbid','', $dados['fpbid']);
	}
	
}

function exibirSituacaoMensario($dados) {
	global $db;
	$acao = "'' as acao,";
	if($dados['uncid']) {
		$wh[] = "i.uncid='".$dados['uncid']."'";
		$wh2[] = "i.uncid='".$dados['uncid']."'";
	} else {
		$acao = "'<img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"detalharAvaliacoesUsuario('||per.pflcod||','||foo3.fpbid||',this);\">' as acao,";
	}
	
	if($dados['fpbid']) {
		$wh[] = "m.fpbid='".$dados['fpbid']."'";
		$wh2[] = "c.fpbid='".$dados['fpbid']."'";
	}
	
	$sql = "SELECT {$acao} foo3.periodo, per.pflcod, per.pfldsc, SUM(napto) as na, SUM(apto) as ap, SUM(aprov) as ar FROM (
	SELECT foo2.fpbid, 'Ref.'||m.mesdsc||'/'||f.fpbanoreferencia as periodo, foo2.pflcod,  CASE WHEN foo2.resultado='Não Apto' THEN 1 ELSE 0 END as napto, CASE WHEN foo2.resultado='Apto' THEN 1 ELSE 0 END as apto, CASE WHEN foo2.resultado='Aprovado' THEN 1 ELSE 0 END as aprov
	FROM sismedio.folhapagamento f 
	INNER JOIN public.meses m ON m.mescod::numeric = f.fpbmesreferencia 
	INNER JOIN (
	
	SELECT foo.pflcod,
			".criteriosAprovacao('restricao3').", foo.fpbid FROM (
	SELECT 
	COALESCE((SELECT AVG(mavtotal) FROM sismedio.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
	(SELECT COUNT(mapid) FROM sismedio.materiaisprofessores mp WHERE mp.iusd=m.iusd) as totalmateriaisprofessores,
 	COALESCE((SELECT AVG(iccvalor) FROM sismedio.respostasavaliacaocomplementar r INNER JOIN sismedio.itensavaliacaocomplementarcriterio ic ON ic.iccid = r.iccid WHERE r.iusdavaliado=i.iusd AND r.fpbid = m.fpbid),0.00) as notacomplementar,
	i.iusdocumento,
	i.iustermocompromisso,
	i.iusnaodesejosubstituirbolsa,
	m.fpbid,
	d.esdid,
	t.pflcod,
	i.iustipoprofessor,
 	fu.rfuparcela,
 	cr.resposta,
    pp.plpmaximobolsas,
 	(SELECT COUNT(pboid) FROM sismedio.pagamentobolsista p INNER JOIN workflow.documento d ON d.docid = p.docid WHERE pflcod=".PFL_FORMADORIES." AND d.esdid!=".ESD_PAGAMENTO_NAO_AUTORIZADO." AND p.uniid=unc.uniid) as numerobolsasformadoriespories,
	(SELECT COUNT(mavid) FROM sismedio.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes,
	(SELECT COUNT(pboid) FROM sismedio.pagamentobolsista pg WHERE pg.iusd=i.iusd) as numeropagamentos,
	(SELECT COUNT(pboid) FROM sismedio.pagamentobolsista pg WHERE pg.tpeid=t.tpeid) as numeropagamentosvaga,
	(SELECT COUNT(mavid) FROM sismedio.mensarioavaliacoes ma  WHERE ma.menid=m.menid AND ma.mavfrequencia=0) as numeroausencia
	FROM sismedio.mensario m
	INNER JOIN sismedio.identificacaousuario i ON i.iusd = m.iusd 
	INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
 	INNER JOIN sismedio.universidadecadastro unc ON unc.uncid = i.uncid  
	INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO." 
	INNER JOIN sismedio.folhapagamentouniversidade fu ON fu.uncid = unc.uncid AND fu.fpbid = m.fpbid AND fu.pflcod = t.pflcod 
	INNER JOIN sismedio.pagamentoperfil pp ON pp.pflcod = t.pflcod 
	LEFT JOIN (
	SELECT COUNT(DISTINCT c.carid) as resposta, c.iusd FROM sismedio.cadernoatividadesrespostas c INNER JOIN sismedio.identificacaousuario i ON i.iusd = c.iusd WHERE caroeproposatividadecadernoformacao IS NOT NULL ".(($wh2)?"AND ".implode(" AND ",$wh2):"")." GROUP BY c.iusd
	) cr ON cr.iusd = m.iusd 
			
	".(($wh)?"WHERE ".implode(" AND ",$wh):"")." 
	) foo
	
	) foo2 ON foo2.fpbid = f.fpbid 
	
	) foo3
	INNER JOIN seguranca.perfil per ON per.pflcod = foo3.pflcod 
	GROUP BY foo3.periodo, per.pflcod, per.pfldsc, foo3.fpbid 
	ORDER BY foo3.fpbid DESC, per.pfldsc";
	
	if($dados['retornarsql']) return $sql;
	
	$cabecalho = array("&nbsp;","Referência","Cod.","Perfil","Não Apto","Apto","Aprovadas");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	
}

function exibirAcessoUsuarioSimec($dados) {
	global $db;
	
	if($dados['uncid']) {
		$wh[] = "u.uncid='".$dados['uncid']."'";
	}
	
	
	$sql = "SELECT 
			'".(($wh)?"":"<img src=\"../imagens/mais.gif\" title=\"mais\" style=\"cursor:pointer;\" onclick=\"detalharStatusUsuarios('||foo.pflcod||',this);\">")."' as acao,
			foo.pfldsc, 
			SUM(foo.total) as total, 
			SUM(foo.ativo) as ativo, 
			SUM(foo.pendente) as pendente, 
			SUM(foo.bloqueado) as bloqueado, 
			SUM(foo.naocadastrado) as naocadastrado 
			FROM (
			SELECT p.pfldsc, p.pflcod, 
			       1 as total, 
			       CASE WHEN us.suscod='A' AND usu.suscod='A' THEN 1 ELSE 0 END as ativo,
			       CASE WHEN us.suscod='P' THEN 1 ELSE 0 END as pendente,
			       CASE WHEN us.suscod='B' THEN 1 ELSE 0 END as bloqueado,
			       CASE WHEN us.suscod is null THEN 1 ELSE 0 END as naocadastrado
			FROM sismedio.identificacaousuario u 
			INNER JOIN sismedio.tipoperfil t on t.iusd=u.iusd 
			INNER JOIN seguranca.perfil p on p.pflcod = t.pflcod 
			LEFT JOIN seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_MEDIO." 
			LEFT JOIN seguranca.usuario usu on usu.usucpf = u.iuscpf
			WHERE u.iusstatus='A' AND 
				  t.pflcod in(
						".PFL_COORDENADORPEDAGOGICO.",
						".PFL_PROFESSORALFABETIZADOR.",
						".PFL_FORMADORREGIONAL.",
						".PFL_ORIENTADORESTUDO.",
						".PFL_COORDENADORIES.",
						".PFL_COORDENADORADJUNTOIES.",
						".PFL_SUPERVISORIES.",
						".PFL_FORMADORIES.") ".(($wh)?"AND ".implode(" AND ",$wh):"")."
			) foo 
			GROUP BY foo.pfldsc, foo.pflcod";
	
	$cabecalho = array("&nbsp;","Perfil","Total","Ativos","Pendentes","Bloqueados","Não cadastrados");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	
}

function exibirSituacaoPagamento($dados) {
	global $db;
	
	$acao = "<img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"detalharDetalhesPagamentosUsuarios('||foo.pflcod||', this);\">";
	
	if($dados['uncid']) {
		$wh[] = "un.uncid='".$dados['uncid']."'";
		$acao = "";
	}
	if($dados['fpbid']) $wh[] = "pb.fpbid='".$dados['fpbid']."'";
	
	
	$sql = "SELECT '{$acao}' as acao,
				   foo.pfldsc, 
				   foo.ag_autorizacao, 
				   (foo.ag_autorizacao*pp.plpvalor) as rs_ag_autorizacao,
				   foo.autorizado,
				   (foo.autorizado*pp.plpvalor) as rs_autorizado,
				   foo.ag_autorizacao_sgb,
				   (foo.ag_autorizacao_sgb*pp.plpvalor) as rs_ag_autorizacao_sgb,
				   foo.ag_pagamento,
				   (foo.ag_pagamento*pp.plpvalor) as rs_ag_pagamento,
				   foo.enviadobanco, 
				   (foo.enviadobanco*pp.plpvalor) as rs_enviadobanco,
				   foo.pg_efetivado,
				   (foo.pg_efetivado*pp.plpvalor) as rs_pg_efetivado,
				   foo.pg_recusado,
				   (foo.pg_recusado*pp.plpvalor) as rs_pg_recusado,
				   foo.pg_naoautorizado,
				   (foo.pg_naoautorizado*pp.plpvalor) as rs_pg_naoautorizado
				   
			FROM (

			SELECT fee.pflcod, 
			       fee.pfldsc, 
			       SUM(ag_autorizacao) as ag_autorizacao,
			       SUM(autorizado) as autorizado,
			       SUM(ag_autorizacao_sgb) as ag_autorizacao_sgb,
			       SUM(ag_pagamento) as ag_pagamento,
			       SUM(enviadobanco) as enviadobanco,
			       SUM(pg_efetivado) as pg_efetivado,
			       SUM(pg_recusado) as pg_recusado,
			       SUM(pg_naoautorizado) as pg_naoautorizado

			FROM (
			
			SELECT 
			p.pflcod,
			p.pfldsc,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_APTO."' THEN 1 ELSE 0 END ag_autorizacao,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_AUTORIZADO."' THEN 1 ELSE 0 END autorizado,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_AG_AUTORIZACAO_SGB."' THEN 1 ELSE 0 END ag_autorizacao_sgb,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_AGUARDANDO_PAGAMENTO."' THEN 1 ELSE 0 END ag_pagamento,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_ENVIADOBANCO."' THEN 1 ELSE 0 END enviadobanco,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_EFETIVADO."' THEN 1 ELSE 0 END pg_efetivado,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_RECUSADO."' THEN 1 ELSE 0 END pg_recusado,
			CASE WHEN dc.esdid='".ESD_PAGAMENTO_NAO_AUTORIZADO."' THEN 1 ELSE 0 END pg_naoautorizado

			
			
			FROM seguranca.perfil p 
			INNER JOIN sismedio.pagamentobolsista pb ON pb.pflcod = p.pflcod 
			INNER JOIN sismedio.universidadecadastro un ON un.uniid = pb.uniid 
			INNER JOIN workflow.documento dc ON dc.docid = pb.docid AND dc.tpdid=".TPD_PAGAMENTOBOLSA." 
			WHERE p.pflcod IN(
			".PFL_PROFESSORALFABETIZADOR.",
		    ".PFL_COORDENADORPEDAGOGICO.",
			".PFL_COORDENADORLOCAL.",
			".PFL_ORIENTADORESTUDO.",
			".PFL_COORDENADORIES.",
			".PFL_COORDENADORADJUNTOIES.",
			".PFL_SUPERVISORIES.",
			".PFL_FORMADORIES.",
			".PFL_FORMADORREGIONAL.") ".(($wh)?" AND ".implode(" AND ",$wh):"")."

			) fee 

			GROUP BY fee.pflcod, fee.pfldsc
			
			) foo
			
			INNER JOIN sismedio.pagamentoperfil pp ON pp.pflcod = foo.pflcod";
	
	
	$cabecalho = array("&nbsp;","Perfil","Aguardando autorização IES","R$","Autorizado IES","R$","Aguardando autorização SGB","R$","Aguardando pagamento","R$","Enviado ao Banco","R$","Pagamento efetivado","R$","Pagamento recusado","R$","Pagamento não autorizado FNDE","R$");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	
}

function sqlEquipeMEC($dados) {
	global $db;
	
	$sql = "SELECT  i.iusd, 
					i.iuscpf, 
					i.iusnome, 
					i.iusemailprincipal,
					i.iusformacaoinicialorientador, 
					p.pflcod,
					p.pfldsc, 
					(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_MEDIO.") as status,
					(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod = t.pflcod) as perfil,
					(SELECT usucpf FROM sismedio.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					CASE WHEN pic.picid IS NOT NULL THEN 
														CASE WHEN pic.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN pic.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
					
			FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sismedio.pactoidadecerta pic ON pic.picid = i.picid 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = pic.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE t.pflcod IN('".PFL_FORMADORIES."','".PFL_FORMADORREGIONAL."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."','".PFL_COORDENADORLOCAL."','".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORPEDAGOGICO."','".PFL_COORDENADORIES."','".PFL_ORIENTADORESTUDO."') AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome";
	
	return $sql;
}

function cadastrarPeriodoReferencia($dados) {
	global $db;
	
	$uncids = $dados['uncid_atualizar'];
	
	if($uncids) {
		foreach($uncids as $uncid) {

			$inativos = $db->carregar("SELECT pflcod, fpbid FROM sismedio.folhapagamentouniversidade WHERE uncid='".$uncid."' AND rfustatus='I'");
			
			$sql = "DELETE FROM sismedio.folhapagamentouniversidade WHERE uncid='".$uncid."'";
			$db->executar($sql);
			
			$sql = "select foo.fpbid from (
					select fpbid, fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0') as dt from sismedio.folhapagamento ) foo
					where foo.dt >= '".$dados['sanoinicio'][$uncid]."-".str_pad($dados['smesini'][$uncid],2,"0", STR_PAD_LEFT)."' AND foo.dt <= '".$dados['sanofim'][$uncid]."-".str_pad($dados['smesfim'][$uncid],2,"0", STR_PAD_LEFT)."'";
			
			$fpbids = $db->carregarColuna($sql);
			
			if($fpbids) {
				foreach($fpbids as $key => $fpbid) {
					$sql = "INSERT INTO sismedio.folhapagamentouniversidade(
	            			uncid, fpbid, rfuparcela)
						    VALUES ('".$uncid."', '".$fpbid."', '".($key+1)."');";
					
					$db->executar($sql);
					
				}
			}

			$perfis = $db->carregarColuna("SELECT p.pflcod FROM seguranca.perfil p
												   INNER JOIN sismedio.pagamentoperfil pp ON pp.pflcod = p.pflcod
												   ORDER BY p.pflnivel");
				
			foreach($perfis as $pflcod) {

				$sql = "select foo.fpbid from (
									select fpbid, fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0') as dt from sismedio.folhapagamento ) foo
									where foo.dt >= '".$dados['sanoiniciop'][$uncid][$pflcod]."-".str_pad($dados['smesinip'][$uncid][$pflcod],2,"0", STR_PAD_LEFT)."' AND foo.dt <= '".$dados['sanofimp'][$uncid][$pflcod]."-".str_pad($dados['smesfimp'][$uncid][$pflcod],2,"0", STR_PAD_LEFT)."'";
					
				$fpbids = $db->carregarColuna($sql);
				
					
				if($fpbids) {

					foreach($fpbids as $key => $fpbid) {

						$sql = "INSERT INTO sismedio.folhapagamentouniversidade(
													            			uncid, fpbid, rfuparcela, pflcod)
																		    VALUES ('".$uncid."', '".$fpbid."', '".($key+1)."', '".$pflcod."');";
							
						$db->executar($sql);
							
					}
				}
					
			}
			
			if($inativos[0]) {
				foreach($inativos as $inativo) {
					$sql = "UPDATE sismedio.folhapagamentouniversidade SET rfustatus='I' WHERE uncid='".$uncid."' AND fpbid='".$inativo['fpbid']."' AND pflcod".(($inativo['pflcod'])?"='".$inativo['pflcod']."'":" IS NULL")."";
					$db->executar($sql);
				}
			}
				
			
		}
	}
	
	$db->commit();
	
	$sql = "select p.uncid, p.fpbid as f1, p2.fpbid as f2, p.pflcod, t.tpaid from sismedio.folhapagamentouniversidade p
			left join sismedio.folhapagamentouniversidade p2 on p2.uncid = p.uncid and p2.pflcod=1190 and p2.rfuparcela=1
			left join sismedio.tipoavaliacaoperfil t on t.uncid = p.uncid and p.pflcod = t.pflcod and p.fpbid = t.fpbid
			where p.rfuparcela=1 and p.pflcod is not null and p.pflcod=".PFL_SUPERVISORIES." and p.fpbid != p2.fpbid and tpaid is null";
	
	$ajustes = $db->carregar($sql);
	
	if($ajustes[0]) {
		foreach($ajustes as $aju) {
			for($pr=$aju['f1'];$pr<$aju['f2'];$pr++) {

				$sql = "INSERT INTO sismedio.tipoavaliacaoperfil(
            			fpbid, pflcod, uncid, tpatipoavaliacao)
    					VALUES ('".$pr."', '".$aju['pflcod']."', '".$aju['uncid']."', 'monitoramentoTextual');";
				
				$db->executar($sql);
				
				
			}
		}
		
		$db->commit();
	}
	
	$al = array("alert"=>"Período de referência aprovado com sucesso","location"=>"sismedio.php?modulo=principal/mec/mec&acao=A&aba=configuracoes");
	alertlocation($al);
	
	
}

function carregarLogCadastroSGB($dados) {
	global $db;
	
	$iusd = $db->pegaUm("SELECT iusd FROM sismedio.identificacaousuario WHERE iuscpf='".$dados['usucpf']."'");
	
	if($iusd) echo "<input type=hidden name=iusd id=iusd_log value=\"".$iusd."\">";
	
	$sql = "SELECT u.iuscpf, u.iusnome, to_char(logdata,'dd/mm/YYYY HH24:MI') as data, logresponse FROM log_historico.logsgb_sismedio l 
			INNER JOIN sismedio.identificacaousuario u ON u.iuscpf = l.logcpf 
			WHERE logcpf='".$dados['usucpf']."' AND logservico='gravarDadosBolsista' ORDER BY l.logdata DESC LIMIT 5";
	$cabecalho = array("CPF","Nome","Data","Erro");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true,false,false,true);
	
}

function visualizarPeriodoTrava($dados) {
	global $db;
	
	$tipoperfil = $db->pegaLinha("SELECT fpbidini, fpbidfim FROM sismedio.tipoperfil WHERe tpeid='".$dados['tpeid']."'");
	
	echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="20%">Período de trava</td>';
	echo '<td>';
						
	$sql = "SELECT fpbid as codigo, lpad(fpbmesreferencia::text,2,'0')||'/'||fpbanoreferencia as descricao FROM sismedio.folhapagamento";
	$db->monta_combo('fpbidini', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'fpbidini','', $tipoperfil['fpbidini']);
	
	echo ' até ';
	
	$sql = "SELECT fpbid as codigo, lpad(fpbmesreferencia::text,2,'0')||'/'||fpbanoreferencia as descricao FROM sismedio.folhapagamento";
	$db->monta_combo('fpbidfim', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'fpbidfim','', $tipoperfil['fpbidfim']);
	
	echo '</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Atualizar" onclick="atualizarPeriodoTrava('.$dados['tpeid'].');"></td>';
	echo '</tr>';
	echo '</table>';

}

function atualizarPeriodoTrava($dados) {
	global $db;
	$sql = "UPDATE sismedio.tipoperfil SET fpbidini='".$dados['fpbidini']."',fpbidfim='".$dados['fpbidfim']."' WHERE tpeid='".$dados['tpeid']."'";
	$db->executar($sql);
	$db->commit();
}

function criteriosAprovacao($cla) {
	global $db;
	
	$cl['restricao1'] = "CASE 
						 WHEN foo.iusnaodesejosubstituirbolsa=true      THEN '<span style=color:red;font-size:x-small;>Bolsista do FNDE/MEC e não deseja substituir bolsa atual pela bolsa do PACTO</span>' 
						 WHEN foo.fpbidini IS NOT NULL AND foo.fpbidfim IS NOT NULL AND ((SELECT fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0')||'-01' FROM sismedio.folhapagamento WHERE fpbid=foo.fpbid)::date < (SELECT fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0')||'-01' FROM sismedio.folhapagamento WHERE fpbid=foo.fpbidini)::date OR (SELECT fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0')||'-01' FROM sismedio.folhapagamento WHERE fpbid=foo.fpbid)::date > (SELECT fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0')||'-01' FROM sismedio.folhapagamento WHERE fpbid=foo.fpbidfim)::date) THEN '<span style=color:red;font-size:x-small;>Este período de referência não esta habilitado para pagamento ".(($db->testa_superuser())?"<img src=../imagens/arrow_v.png style=cursor:pointer; align=absmiddle onclick=\"visualizarPeriodoTrava('||foo.tpeid||');\">":"")."</span>'
    					 WHEN foo.numeropagamentos >= foo.plpmaximobolsas THEN '<span style=color:red;font-size:x-small;>Atingiu o número máximo ('||foo.plpmaximobolsas||' bolsas)</span>'
    					 WHEN foo.numeropagamentosvaga >= foo.plpmaximobolsas THEN '<span style=color:red;font-size:x-small;>Atingiu o número máximo ('||foo.plpmaximobolsas||' bolsas)</span>'
						 WHEN foo.pflcod=".PFL_FORMADORIES." THEN 
						                                                  CASE  WHEN foo.numerobolsasformadoriespories >= ".MAX_FORMADORIES." THEN '<center><span style=color:red;font-size:x-small;>Total máximo de bolsas de formadores da IES foi atingida</span></center>'
						   													  	ELSE '<span style=color:blue;font-size:x-small;>Nenhuma restrição</span>' 
						   												  END
						 WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." THEN 
						                                                  CASE  WHEN foo.iusdocumento=false   THEN '<center><span style=color:red;font-size:x-small;>Possui problemas na documentação</span></center>'
						   													  	--WHEN foo.numeroavaliacoes < 2 THEN '<span style=color:red;font-size:x-small;>Para o perfil de Orientador de Estudo, número de avaliadores('||foo.numeroavaliacoes||') é Insuficiente </span>' 
						   													  	WHEN foo.numeroausencia > 0 THEN '<span style=color:red;font-size:x-small;>Ausência na Universidade e/ou Município</span>'
						   													  	ELSE '<span style=color:blue;font-size:x-small;>Nenhuma restrição</span>' 
						   												  END
				   		 WHEN foo.pflcod=".PFL_COORDENADORPEDAGOGICO." THEN 
					   														CASE WHEN foo.notacomplementar = 0 THEN '<span style=color:red;font-size:x-small;>Avaliação Complementar do mês referente não cadastrada</span>'
																				 WHEN foo.resposta IS NULL  THEN '<span style=color:red;font-size:x-small;>Atividades obrigatórias não realizadas</span>' 
					   															 ELSE '<span style=color:blue;font-size:x-small;>Nenhuma restrição</span>' END
				   		 WHEN foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN 
					   														CASE WHEN foo.notacomplementar = 0 THEN '<span style=color:red;font-size:x-small;>Avaliação Complementar do mês referente não cadastrada</span>'
																				 WHEN foo.resposta IS NULL  THEN '<span style=color:red;font-size:x-small;>Atividades obrigatórias não realizadas</span>' 
					   														WHEN foo.iustipoprofessor = 'censo' THEN '<span style=color:blue;font-size:x-small;>Nenhuma restrição</span>'
					   														ELSE '<span style=color:red;font-size:x-small;>Professor Alfabetizador não cadastrado no censo 2013</span>' END 
				   		 ELSE '<span style=color:blue;font-size:x-small;>Nenhuma restrição</span>' END as restricao";
	
	$cl['restricao2'] = "CASE WHEN foo.mensarionota >= 7  AND foo.iustermocompromisso=true AND foo.numeropagamentos < foo.plpmaximobolsas AND foo.numeropagamentosvaga < foo.plpmaximobolsas AND
						(CASE WHEN foo.fpbidini IS NOT NULL AND foo.fpbidfim IS NOT NULL AND ((SELECT fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0')||'-01' FROM sismedio.folhapagamento WHERE fpbid=foo.fpbid)::date < (SELECT fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0')||'-01' FROM sismedio.folhapagamento WHERE fpbid=foo.fpbidini)::date OR (SELECT fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0')||'-01' FROM sismedio.folhapagamento WHERE fpbid=foo.fpbid)::date > (SELECT fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0')||'-01' FROM sismedio.folhapagamento WHERE fpbid=foo.fpbidfim)::date) THEN false ELSE true END) AND
						(CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." THEN 
																																					CASE WHEN foo.iusdocumento=false THEN false 
																																						 WHEN foo.numeroausencia > 0 THEN false
																																					ELSE true 
																																						 END 
									 WHEN foo.pflcod=".PFL_FORMADORIES." THEN 
						                                                  CASE  WHEN foo.numerobolsasformadoriespories >= ".MAX_FORMADORIES." THEN false
						   													  	ELSE true 
						   												  END
			
																							   		 WHEN foo.pflcod=".PFL_COORDENADORPEDAGOGICO." THEN 
					   																																	CASE WHEN foo.notacomplementar = 0 THEN false 
																																							 WHEN foo.resposta IS NULL  THEN false
					   															 																			ELSE true END
																									WHEN foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN 
																																						CASE WHEN foo.notacomplementar = 0 THEN false 
	 																																						 WHEN foo.resposta IS NULL THEN false
																																						WHEN foo.iustipoprofessor = 'censo' THEN true 
																																						ELSE false END
																									ELSE true END) AND foo.iusnaodesejosubstituirbolsa=false  THEN CASE WHEN foo.notacomplementar >= 7 THEN 'checked' ELSE '' END
																								
																								ELSE 'disabled' END";
	
	$cl['restricao3'] = "CASE WHEN foo.iusnaodesejosubstituirbolsa=true THEN 'Não Apto'
	    					  WHEN foo.numeropagamentos >= foo.plpmaximobolsas THEN 'Não Apto'
    					 	  WHEN foo.numeropagamentosvaga >= foo.plpmaximobolsas THEN 'Não Apto'
							  WHEN foo.esdid=".ESD_APROVADO_MENSARIO." THEN 'Aprovado'
						 	  WHEN foo.mensarionota >= 7  AND foo.iustermocompromisso=true AND (CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." THEN 
																																					CASE WHEN foo.iusdocumento=false THEN false 
																																						 WHEN foo.numeroausencia > 0 THEN false
																																						 ELSE true END
															 										 WHEN foo.pflcod=".PFL_FORMADORIES." THEN 
																				                                                  CASE  WHEN foo.numerobolsasformadoriespories >= ".MAX_FORMADORIES." THEN false
																				   													  	ELSE true 
																				   												  END
	 				
																							   		 WHEN foo.pflcod=".PFL_COORDENADORPEDAGOGICO." THEN 
					   																																	CASE WHEN foo.notacomplementar = 0 THEN false 
 			            																																	 WHEN foo.resposta IS NULL  THEN false
					   															 																			ELSE true END
				
																									WHEN foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN 
																																						CASE WHEN foo.notacomplementar = 0 THEN false 
																																							 WHEN foo.resposta IS NULL  THEN false
																																						WHEN foo.iustipoprofessor = 'censo' THEN true 
																																						ELSE false END
																									ELSE true END) THEN 'Apto' 
		    ELSE 'Não Apto' END resultado";
	
	
	return $cl[$cla];
	
}

function verificarEmailUnico($dados) {
	global $db;
	
	$sql = "SELECT '- '||i.iusnome||' ( '||p.pfldsc||' );' as descricao FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
			WHERE i.iusemailprincipal='".$dados['iusemailprincipal']."' AND i.iusd!='".$dados['iusd']."' AND i.iusstatus='A'";
	
	$nomes = $db->carregarColuna($sql);
	
	echo implode('\n', $nomes);
	
	
}

function consultarDetalhesPagamento($dados) {
	global $db;
	$sql = "SELECT i.iusnome, me.mesdsc||'/'||fa.fpbanoreferencia as periodo, e.esddsc, p.pbovlrpagamento, pp.pfldsc, uni.uninome, uni.unicnpj, p.docid FROM sismedio.pagamentobolsista p 
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = p.iusd 
			INNER JOIN sismedio.folhapagamento fa ON fa.fpbid = p.fpbid 
			INNER JOIN public.meses me ON me.mescod::integer = fa.fpbmesreferencia 
			INNER JOIN workflow.documento d ON d.docid = p.docid  AND d.tpdid=".TPD_PAGAMENTOBOLSA."
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = p.pflcod 
			INNER JOIN sismedio.universidade uni ON uni.uniid = p.uniid  
			WHERE pboid='".$dados['pboid']."'";
	$pagamentobolsista = $db->pegaLinha($sql);
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	echo "<tr><td class=SubTituloDireita width=25%>Beneficiário : </td><td>".$pagamentobolsista['iusnome']."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Período : </td><td>".$pagamentobolsista['periodo']."</td></tr>";
	echo "<tr><td class=SubTituloDireita>Valor(R$) : </td><td>".number_format($pagamentobolsista['pbovlrpagamento'],2,",",".")." (".$pagamentobolsista['pfldsc'].")</td></tr>";
	echo "<tr><td class=SubTituloDireita>Universidade pagante : </td><td>".$pagamentobolsista['uninome']." ( Cnpj . ".mascaraglobal($pagamentobolsista['unicnpj'],"##.###.###/####-##").")</td></tr>";
	echo "</table>";
	
	echo "<p align=center><b>Fluxo do pagamento</b></p>";
	fluxoWorkflowInterno(array('docid'=>$pagamentobolsista['docid']));
	
	

	
}

function carregarMateriais($dados) {
	global $db;
	if($dados['uncid']) {
		
		$sql = "SELECT * FROM (
				(
				SELECT count(p.picid) as tot, ".$dados['group']." as grouper FROM sismedio.materiais m 
				INNER JOIN sismedio.pactoidadecerta p ON p.picid = m.picid 
				INNER JOIN sismedio.abrangencia a ON a.muncod = p.muncod 
				INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='M' and e.uncid='".$dados['uncid']."' GROUP BY ".$dados['group']."
				) UNION ALL (
				SELECT count(p.picid) as tot, ".$dados['group']." as grouper FROM sismedio.materiais m 
				INNER JOIN sismedio.pactoidadecerta p ON p.picid = m.picid 
				INNER JOIN territorios.municipio mu ON mu.estuf = p.estuf  
				INNER JOIN sismedio.abrangencia a ON a.muncod = mu.muncod 
				INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='E' and e.uncid='".$dados['uncid']."' GROUP BY ".$dados['group']."
				)
				) foo";
		
		$materiais = $db->carregar($sql);
		
	} else {
		
		$materiais = $db->carregar("SELECT count(picid) as tot, ".$dados['group']." as grouper FROM sismedio.materiais GROUP BY ".$dados['group']);
				
	}

	if($materiais[0]) {
		foreach($materiais as $mat) {
			$info[$mat['grouper']] = (($mat['tot'])?$mat['tot']:"0");
		}
	}
	return $info;
}

function carregarMateriaisProfessores($dados) {
	global $db;
	$materiais = $db->carregar("SELECT count(iusd) as tot, ".$dados['group']." as grouper FROM sismedio.materiaisprofessores GROUP BY ".$dados['group']);

	if($materiais[0]) {
		foreach($materiais as $mat) {
			$info[$mat['grouper']] = (($mat['tot'])?$mat['tot']:"0");
			$tot += (($mat['tot'])?$mat['tot']:"0");
		}
		$info['total'] = $tot;
	}
	return $info;
}

function excluirUsuarioPerfil($dados) {
	global $db;
	include_once '_funcoes_universidade.php';
	
	$npagamentos = $db->pegaUm("SELECT COUNT(*) FROM sismedio.pagamentobolsista WHERE iusd='".$dados['iusd']."'");
	
	if($npagamentos > 0) {
		
		$identificacaousuario = $db->pegaLinha("SELECT i.iuscpf, t.pflcod, p.pfldsc FROM sismedio.identificacaousuario i 
												INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
												INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod  
												WHERE i.iusd='".$dados['iusd']."'");

		if(substr($identificacaousuario['iuscpf'],0,3)=='REM') $pr = 'RE2';
		else {
			$pr = 'REM';
			$cpf_gerado = $db->pegaUm("SELECT '{$pr}'||SUBSTR(iuscpf,4,8) as iuscpf FROM sismedio.identificacaousuario WHERE iusd='".$dados['iusd']."'");
			$existe = $db->pegaUm("SELECT iusd FROM sismedio.identificacaousuario WHERE iuscpf='".$cpf_gerado."'");
			if($existe) $pr = 'RE2';
		} 
		
		
		$sql = "INSERT INTO sismedio.identificacaousuario(
	            picid, muncod, eciid, nacid, fk_cod_docente, iuscpf, iusnome, 
	            iussexo, iusdatanascimento, iusnomemae, iusnomeconjuge, iusagenciasugerida, 
	            iusemailprincipal, iusemailopcional, iusdatainclusao, iuscadastrovalidadoies, 
	            iussituacao, iusstatus, funid, iusagenciaend, iustipoorientador, 
	            foeid, iustermocompromisso, tvpid, muncodatuacao, uncid, iusserieprofessor, 
	            iusformacaoinicialorientador, cadastradosgb, iustipoprofessor, 
	            iusdocumento, iusnaodesejosubstituirbolsa)
				SELECT picid, muncod, eciid, nacid, fk_cod_docente, '{$pr}'||SUBSTR(iuscpf,4,8) as iuscpf, iusnome || ' - {$identificacaousuario['pfldsc']} - REMOVIDO' as iusnome, 
				       iussexo, iusdatanascimento, iusnomemae, iusnomeconjuge, iusagenciasugerida, 
				       iusemailprincipal, iusemailopcional, iusdatainclusao, iuscadastrovalidadoies, 
				       iussituacao, 'I' as iusstatus, funid, iusagenciaend, iustipoorientador, 
				       foeid, iustermocompromisso, tvpid, muncodatuacao, uncid, iusserieprofessor, 
				       iusformacaoinicialorientador, cadastradosgb, iustipoprofessor, 
				       iusdocumento, iusnaodesejosubstituirbolsa
				  FROM sismedio.identificacaousuario where iusd='".$dados['iusd']."'
				RETURNING iusd;";
		
		$iusd_novo = $db->pegaUm($sql);
		
		
		$sql = "DELETE FROM sismedio.usuarioresponsabilidade  WHERE rpustatus='A' AND usucpf='".$identificacaousuario['iuscpf']."' AND pflcod='".$identificacaousuario['pflcod']."'";
		$db->executar($sql);
	
		$sql = "UPDATE sismedio.tipoperfil SET iusd='".$iusd_novo."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	
		$sql = "UPDATE sismedio.turmas SET iusd='".$iusd_novo."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	
		$sql = "UPDATE sismedio.orientadorturma SET iusd='".$iusd_novo."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	
		$sql = "UPDATE sismedio.identificacaousuario SET iusstatus='I' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	
		$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$identificacaousuario['iuscpf']."' AND pflcod='".$identificacaousuario['pflcod']."'";
		$db->executar($sql);
	
		// removendo avaliações não concluidas
		$sql = "SELECT m.menid FROM sismedio.mensario m 
				INNER JOIN workflow.documento d ON d.docid = m.docid 
				WHERE iusd='".$dados['iusd']."' AND d.esdid!='".ESD_APROVADO_MENSARIO."'";
	
		$menids = $db->carregarColuna($sql);
		
		if($menids) {
			
			$sql = "SELECT mavid FROM sismedio.mensarioavaliacoes WHERE menid IN('".implode("','",$menids)."')";
			$mavids = $db->carregarColuna($sql);
			
			if($mavids) {
				$db->executar("DELETE FROM sismedio.historicoreaberturanota WHERE mavid IN('".implode("','",$mavids)."')");
				$db->executar("DELETE FROM sismedio.mensarioavaliacoes WHERE mavid IN('".implode("','",$mavids)."')");
			}
		}
		
		
	} else {
	
		removerTipoPerfil(array('iusd'=>$dados['iusd'],'pflcod'=>$dados['pflcod'],'naoredirecionar'=>true));
	
	}
	
	if(!$dados['uncid']) $dados['uncid'] = $db->pegaUm("SELECT uncid FROM sismedio.identificacaousuario WHERE iusd='".$dados['iusd']."'");

	$sql = "INSERT INTO sismedio.historicotrocausuario(
            iusdantigo, pflcod, hstdata, usucpf, uncid, 
            hstacao)
    		VALUES ('".$dados['iusd']."', '".$dados['pflcod']."', NOW(), '".$_SESSION['usucpf']."', '".$dados['uncid']."', 'R');";
	
	$db->executar($sql);
	$db->commit();
	
	gerarVersaoProjetoUniversidade(array('uncid'=>$dados['uncid']));
	
	$al = array("alert"=>"Exclusão ocorrida com sucesso","location"=>"sismedio.php?modulo=".$dados['modulo']."&acao=".$dados['acao']."&aba=gerenciarusuario&uncid=".$dados['uncid']);
	alertlocation($al);
	
	
}


function exibirMateriais($dados) {
	global $db;
?>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro">Pergunta</td>
	<td class="SubTituloCentro">Sim, totalmente</td>
	<td class="SubTituloCentro">Sim, parcialmente</td>
	<td class="SubTituloCentro">Não</td>
</tr>
<?

$recebeumaterialpacto = carregarMateriais(array('group' => 'recebeumaterialpacto','uncid' => $dados['uncid']));
$distribuiumaterialpacto = carregarMateriais(array('group' => 'distribuiumaterialpacto','uncid' => $dados['uncid']));
$recebeumaterialpnld = carregarMateriais(array('group' => 'recebeumaterialpnld','uncid' => $dados['uncid']));
$recebeulivrospnld = carregarMateriais(array('group' => 'recebeulivrospnld','uncid' => $dados['uncid']));
$recebeumaterialpnbe = carregarMateriais(array('group' => 'recebeumaterialpnbe','uncid' => $dados['uncid']));
$criadocantinholeitura = carregarMateriais(array('group' => 'criadocantinholeitura','uncid' => $dados['uncid']));

?>
<tr>
	<td><font size=1>Número de estados/municípios que receberam o material da formação do Pacto</font></td>
	<td align="right"><?=$recebeumaterialpacto['1'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('recebeumaterialpacto','1','<?=$dados['uncid'] ?>');"></td>
	<td align="right"><?=$recebeumaterialpacto['2'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('recebeumaterialpacto','2','<?=$dados['uncid'] ?>');"></td>
	<td align="right" nowrap><?=$recebeumaterialpacto['3'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('recebeumaterialpacto','3','<?=$dados['uncid'] ?>');"></td>
</tr>
	<tr>
	<td><font size=1>Número de estados/municípios que distribuiram entre orientadores de estudo e professores alfabetizadores o material da formação do Pacto</font></td>
	<td align="right"> <?=$distribuiumaterialpacto['1'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('distribuiumaterialpacto','1','<?=$dados['uncid'] ?>');"></td>
	<td align="right"> <?=$distribuiumaterialpacto['2'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('distribuiumaterialpacto','2','<?=$dados['uncid'] ?>');"></td>
	<td align="right" nowrap> <?=$distribuiumaterialpacto['3'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('distribuiumaterialpacto','3','<?=$dados['uncid'] ?>');"></td>
	</tr>
	<tr>
	<td><font size=1>Número de estados/municípios que receberam o material referente ao Programa Nacional do Livro Didático (PNLD) em cada escola</font></td>
	<td align="right"><?=$recebeumaterialpnld['1'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('recebeumaterialpnld','1','<?=$dados['uncid'] ?>');"></td>
	<td align="right"><?=$recebeumaterialpnld['2'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('recebeumaterialpnld','2','<?=$dados['uncid'] ?>');"></td>
	<td align="right" nowrap><?=$recebeumaterialpnld['3'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('recebeumaterialpnld','3','<?=$dados['uncid'] ?>');"></td>
	</tr>
	<tr>
	<td><font size=1>Número de estados/municípios que receberam os livros do PNLD - Obras Complementares específico para cada sala de aula de alfabetização</font></td>
	<td align="right"><?=$recebeulivrospnld['1'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('recebeulivrospnld','1','<?=$dados['uncid'] ?>');"></td>
	<td align="right"><?=$recebeulivrospnld['2'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('recebeulivrospnld','2','<?=$dados['uncid'] ?>');"></td>
	<td align="right" nowrap><?=$recebeulivrospnld['3'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('recebeulivrospnld','3','<?=$dados['uncid'] ?>');"></td>
	</tr>
	<tr>
	<td><font size=1>Número de estados/municípios que receberam os livros do Programa Nacional Biblioteca da Escola (PNBE) específico para cada sala de aula de alfabetização</font></td>
	<td align="right"><?=$recebeumaterialpnbe['1'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('recebeumaterialpnbe','1','<?=$dados['uncid'] ?>');"></td>
	<td align="right"><?=$recebeumaterialpnbe['2'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('recebeumaterialpnbe','2','<?=$dados['uncid'] ?>');"></td>
	<td align="right" nowrap><?=$recebeumaterialpnbe['3'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('recebeumaterialpnbe','3','<?=$dados['uncid'] ?>');"></td>
	</tr>
	<tr>
	<td><font size=1>Número de estados/municípios que criaram um cantinho de leitura em cada sala de aula de alfabetização com o material do PNBE</font></td>
	<td align="right"> <?=$criadocantinholeitura['1'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('criadocantinholeitura','1','<?=$dados['uncid'] ?>');"></td>
	<td align="right"> <?=$criadocantinholeitura['2'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('criadocantinholeitura','2','<?=$dados['uncid'] ?>');"></td>
	<td align="right" nowrap><?=$criadocantinholeitura['3'] ?> <img src="../imagens/mais.gif" style="cursor:pointer;" onclick="carregarMunicipioMateriais('criadocantinholeitura','3','<?=$dados['uncid'] ?>');"></td>
	</tr>
	<tr>
	<td colspan="4">
	<fieldset><legend>Fotos cantinho de leitura</legend>
	<div style="overflow:auto;width:500px;height:100px;">
	<?
	echo "<table>";
	echo "<tr>";
	
	$_SESSION['imgparams']['tabela'] = "sismedio.materiaisfotos";
	$_SESSION['imgparams']['filtro'] = "1=1";
	
	if($dados['uncid']) {
		$sql = "SELECT m.arqid, m.matid, m.mafdsc FROM sismedio.materiaisfotos m
				INNER JOIN sismedio.materiais ma ON ma.matid = m.matid 
				INNER JOIN sismedio.pactoidadecerta p ON p.picid = ma.picid 
				INNER JOIN sismedio.abrangencia a ON a.muncod = p.muncod 
				INNER JOIN territorios.municipio mu ON mu.muncod = a.muncod
				INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='M' and e.uncid='".$dados['uncid']."' 
				ORDER BY random() LIMIT 6";
	} else {
		$sql = "SELECT m.arqid, m.matid, m.mafdsc FROM sismedio.materiaisfotos m 
				INNER JOIN sismedio.materiais ma ON ma.matid = m.matid 
				INNER JOIN sismedio.pactoidadecerta p ON p.picid = ma.picid 
				ORDER BY random() LIMIT 6";
	}
	$fotos = $db->carregar($sql);
	if($fotos) {
		foreach($fotos as $ft) {
			echo "<td><img id=".$ft['arqid']." onmouseover=\"return escape('".$ft['mafdsc']."');\" src=\"../slideshow/slideshow/verimagem.php?arqid=".$ft['arqid']."&newwidth=70&newheight=70\" class=\"imageBox_theImage\" onclick=\"javascript:window.open('../slideshow/slideshow/index.php?pagina=&amp;arqid=".$ft['arqid']."&amp;_sisarquivo=sismedio&amp;getFiltro=true&amp;matid=".$ft['matid']."','imagem','width=850,height=600,resizable=yes');\"></td>";
		}
	} else {
		echo "<td>Não existem fotos cadastradas</td>";
	}
	echo "</tr>";
	echo "</table>";
	?>
	</fieldset>
	</td>
	</tr>
	</table>
<?
	
}


function exibirMateriaisProfessores($dados) {
	global $db;
	$recebeumaterialpacto = carregarMateriaisProfessores(array('group' => 'recebeumaterialpacto'));
	$recebeumaterialpnld = carregarMateriaisProfessores(array('group' => 'recebeumaterialpnld'));
	$recebeulivrospnld = carregarMateriaisProfessores(array('group' => 'recebeulivrospnld'));
	$recebeumaterialpnbe = carregarMateriaisProfessores(array('group' => 'recebeumaterialpnbe'));
	$criadocantinholeitura = carregarMateriaisProfessores(array('group' => 'criadocantinholeitura'));
	
?>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloEsquerda" colspan="4">Total de professores que responderam: <?=$recebeumaterialpacto['total'] ?></td>
</tr>
<tr>
	<td class="SubTituloCentro">Pergunta</td>
	<td class="SubTituloCentro">&nbsp;</td>
	<td class="SubTituloCentro">&nbsp;</td>
	<td class="SubTituloCentro">&nbsp;</td>
</tr>
<tr>
	<td><font size=1>Professor, você recebeu o material de formação do Pacto?</font></td>
	<td><b>(<?=$recebeumaterialpacto['1']." / ".round(($recebeumaterialpacto['1']/$recebeumaterialpacto['total'])*100,1) ?>%)</b> Sim, receberam o material fornecido pelo MEC</td>
	<td><b>(<?=$recebeumaterialpacto['2']." / ".round(($recebeumaterialpacto['2']/$recebeumaterialpacto['total'])*100,1) ?>%)</b> Sim, receberam uma cópia do material providenciada pelo município</td>
	<td nowrap><b>(<?=$recebeumaterialpacto['3']." / ".round(($recebeumaterialpacto['3']/$recebeumaterialpacto['total'])*100,1) ?>%)</b> Não</td>
</tr>

	<tr>
	<td><font size=1>A sua escola recebeu o material referente ao Programa Nacional do Livro Didático (PNLD)</font></td>
	<td><b>(<?=$recebeumaterialpnld['1']." / ".round(($recebeumaterialpnld['1']/$recebeumaterialpnld['total'])*100,1) ?>%)</b> Sim, recebemos o material integralmente</td>
	<td><b>(<?=$recebeumaterialpnld['2']." / ".round(($recebeumaterialpnld['2']/$recebeumaterialpnld['total'])*100,1) ?>%)</b> Sim, recebemos parte do material</td>
	<td nowrap><b>(<?=$recebeumaterialpnld['3']." / ".round(($recebeumaterialpnld['3']/$recebeumaterialpnld['total'])*100,1) ?>%)</b> Não</td>
	</tr>
	<tr>
	<td><font size=1>A sua escola recebeu os livros do PNLD - Obras Complementares específico para cada sala de aula de alfabetização?</font></td>
	<td><b>(<?=$recebeulivrospnld['1']." / ".round(($recebeulivrospnld['1']/$recebeulivrospnld['total'])*100,1) ?>%)</b> Sim, recebemos o material integralmente</td>
	<td><b>(<?=$recebeulivrospnld['2']." / ".round(($recebeulivrospnld['2']/$recebeulivrospnld['total'])*100,1) ?>%)</b> Sim, recebemos parte do material</td>
	<td nowrap><b>(<?=$recebeulivrospnld['3']." / ".round(($recebeulivrospnld['3']/$recebeulivrospnld['total'])*100,1) ?>%)</b> Não</td>
	</tr>
	<tr>
	<td><font size=1>A turma da qual você é regente recebeu os livros do Programa Nacional Biblioteca da Escola (PNBE), específico para cada sala de aula de alfabetização?</font></td>
	<td><b>(<?=$recebeumaterialpnbe['1']." / ".round(($recebeumaterialpnbe['1']/$recebeumaterialpnbe['total'])*100,1) ?>%)</b> Sim, recebemos o material integralmente</td>
	<td><b>(<?=$recebeumaterialpnbe['2']." / ".round(($recebeumaterialpnbe['2']/$recebeumaterialpnbe['total'])*100,1) ?>%)</b> Sim, recebemos parte do material</td>
	<td nowrap><b>(<?=$recebeumaterialpnbe['3']." / ".round(($recebeumaterialpnbe['3']/$recebeumaterialpnbe['total'])*100,1) ?>%)</b> Não</td>
	</tr>
	<tr>
	<td><font size=1>Na turma da qual você é regente, foi criado um cantinho de leitura em cada sala de aula de alfabetização com o material do PNBE?</font></td>
	<td><b>(<?=$criadocantinholeitura['1']." / ".round(($criadocantinholeitura['1']/$criadocantinholeitura['total'])*100,1) ?>%)</b> Sim, criamos o cantinho de leitura</td>
	<td>&nbsp;</td>
	<td nowrap><b>(<?=$criadocantinholeitura['3']." / ".round(($criadocantinholeitura['3']/$criadocantinholeitura['total'])*100,1) ?>%)</b> Não</td>
	</tr>
	<tr>
	<td colspan="4">
	<fieldset><legend>Fotos cantinho de leitura</legend>
	<div style="overflow:auto;width:500px;height:100px;">
	<?
	echo "<table>";
	echo "<tr>";
	
	$_SESSION['imgparams']['tabela'] = "sismedio.materiaisprofessoresfotos";
	$_SESSION['imgparams']['filtro'] = "1=1";
	
	$sql = "SELECT m.arqid, m.mapid, m.mpfdsc FROM sismedio.materiaisprofessoresfotos m 
			INNER JOIN sismedio.materiaisprofessores ma ON ma.mapid = m.mapid 
			ORDER BY random() LIMIT 6";

	$fotos = $db->carregar($sql);
	if($fotos) {
		foreach($fotos as $ft) {
			echo "<td><img id=".$ft['arqid']." onmouseover=\"return escape('".$ft['mafdsc']."');\" src=\"../slideshow/slideshow/verimagem.php?arqid=".$ft['arqid']."&newwidth=70&newheight=70\" class=\"imageBox_theImage\" onclick=\"javascript:window.open('../slideshow/slideshow/index.php?pagina=&amp;arqid=".$ft['arqid']."&amp;_sisarquivo=sismedio&amp;getFiltro=true&amp;mapid=".$ft['mapid']."','imagem','width=850,height=600,resizable=yes');\"></td>";
		}
	} else {
		echo "<td>Não existem fotos cadastradas</td>";
	}
	echo "</tr>";
	echo "</table>";
	?>
	</fieldset>
	</td>
	</tr>
	</table>
<?
	
}

function verMunicipioMateriais($dados) {
	global $db;
	echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
	
	if($dados['uncid']) {
		
		$sql = "SELECT * FROM (
				(
				SELECT 
				'Estadual' as esfera,
				es.estuf || ' / ' || es.estdescricao as descricao,
				COALESCE(array_to_string(array(SELECT i.iusnome FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as coordenadorlocal,
				COALESCE(array_to_string(array(SELECT i.iusemailprincipal FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as emailcoordenadorlocal,
				COALESCE(array_to_string(array(SELECT '('||itedddtel||') '||itenumtel as tel FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON i.iusd=t.iusd INNER JOIN sismedio.identificacaotelefone it ON it.iusd = i.iusd AND it.itetipo='C' WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'-') as telefonecoordenadorlocal
				FROM sismedio.materiais m 
				INNER JOIN sismedio.pactoidadecerta p ON p.picid = m.picid 
				INNER JOIN territorios.municipio mu ON mu.estuf = p.estuf 
				INNER JOIN territorios.estado es ON es.estuf = p.estuf 
				INNER JOIN sismedio.abrangencia a ON a.muncod = mu.muncod 
				INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='E' and e.uncid='".$dados['uncid']."' and {$dados['campo']}='{$dados['opcao']}'
				ORDER BY 1,2
				) UNION ALL (
				SELECT 
				'Municipal' as esfera,
				mu.estuf || ' / ' || mu.mundescricao as descricao,
				COALESCE(array_to_string(array(SELECT i.iusnome FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as coordenadorlocal,
				COALESCE(array_to_string(array(SELECT i.iusemailprincipal FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as emailcoordenadorlocal,
				COALESCE(array_to_string(array(SELECT '('||itedddtel||') '||itenumtel as tel FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON i.iusd=t.iusd INNER JOIN sismedio.identificacaotelefone it ON it.iusd = i.iusd AND it.itetipo='C' WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'-') as telefonecoordenadorlocal
				 
				
				FROM sismedio.materiais m 
				INNER JOIN sismedio.pactoidadecerta p ON p.picid = m.picid 
				INNER JOIN sismedio.abrangencia a ON a.muncod = p.muncod 
				INNER JOIN territorios.municipio mu ON mu.muncod = a.muncod
				INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='M' and e.uncid='".$dados['uncid']."' and {$dados['campo']}='{$dados['opcao']}'
				ORDER BY 1,2
				)
				) foo";
		
		
	} else {
	
		$sql = "SELECT 
				CASE WHEN p.muncod IS NOT NULL THEN 'Municipal' ELSE 'Estadual' END as esfera,
				CASE WHEN p.muncod IS NOT NULL THEN mu.estuf || ' / ' || mu.mundescricao ELSE es.estuf || ' / ' || es.estdescricao END as descricao,
				COALESCE(array_to_string(array(SELECT i.iusnome FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as coordenadorlocal,
				COALESCE(array_to_string(array(SELECT i.iusemailprincipal FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as emailcoordenadorlocal,
				COALESCE(array_to_string(array(SELECT '('||itedddtel||') '||itenumtel as tel FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON i.iusd=t.iusd INNER JOIN sismedio.identificacaotelefone it ON it.iusd = i.iusd AND it.itetipo='C' WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'-') as telefonecoordenadorlocal
				FROM sismedio.materiais m 
				INNER JOIN sismedio.pactoidadecerta p ON p.picid = m.picid 
				LEFT JOIN territorios.municipio mu ON mu.muncod = p.muncod 
				LEFT JOIN territorios.estado es ON es.estuf = p.estuf 
				WHERE {$dados['campo']}='{$dados['opcao']}' ORDER BY 1,2";
	}
	
	$cabecalho = array("Esfera","Descrição","Coordenador Local","Email","Telefone");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true);
	
}

function carregarInformes($dados) {
	global $db;
	echo '<p><b>Informes</b></p>';
	
	echo '<div style="background-color:white;height:150px;overflow:auto;">';
	
	$informes = $db->carregar("SELECT inpdescricao, to_char(inpdatainserida,'dd/mm/YYYY HH24:MI') as inpdatainserida FROM sismedio.informespacto WHERE pflcoddestino='".$dados['pflcoddestino']."' AND inpstatus='A' ORDER BY inpdatainserida DESC");
	
	if($informes[0]) {
		foreach($informes as $inf) {
			echo " - ".$inf['inpdescricao']." ( <b>Inserida em ".$inf['inpdatainserida']."</b> )<br>";
		}
	} else {
		echo " - Não existem informes cadastrados";
	}
	
	echo '</div>';
	
	
}

function carregarHistoricoUsuario($dados) {
	global $db;
	
	$sql = "SELECT us.usunome, to_char(htudata,'dd/mm/YYYY HH24:MI') as data, hu.htudsc, hu.suscod, us2.usunome as resp FROM seguranca.historicousuario hu 
			INNER JOIN seguranca.usuario us ON us.usucpf = hu.usucpf 
			LEFT JOIN seguranca.usuario us2 ON us2.usucpf = hu.usucpfadm
			WHERE hu.usucpf='".$dados['usucpf']."' AND hu.sisid='".SIS_MEDIO."' ORDER BY htudata DESC";
	
	$cabecalho = array("Nome","Data","Justificativa","Situação","Responsável");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true);
	
}


function trocarTurmas($dados) {
	global $db;
	if($dados['troca']) {
		foreach($dados['troca'] as $iusd => $turid) {

			if($turid) {
				
				$existe = $db->pegaUm("SELECT otuid FROM sismedio.orientadorturma WHERE iusd='".$iusd."'");
				
				if($existe) {

					$db->executar("UPDATE sismedio.orientadorturma SET turid='".$turid."' WHERE iusd='".$iusd."'");
					
					$db->executar("INSERT INTO sismedio.historicotrocausuario(
	            				   iusdantigo, pflcod, hstdata, usucpf, uncid, hstacao, turidnovo, turidantigo)
	    						   VALUES ('".$iusd."', 
	    						   		   (SELECT pflcod FROM sispacto2.tipoperfil WHERE iusd='".$iusd."'), 
	    						   		    NOW(), 
	    						   		    '".$_SESSION['usucpf']."', 
	    						   		    '".$dados['uncid']."', 
	    						   		    'F', 
	    						   		    '".$turid."', 
	            							'".$dados['turidantigo']."');");
				} else {

					$db->executar("INSERT INTO sismedio.orientadorturma(turid, iusd, otustatus, otudata)
    							   VALUES ('".$turid."', '".$iusd."', 'A', NOW());");

					$db->executar("INSERT INTO sismedio.historicotrocausuario(
						            				   iusdantigo, pflcod, hstdata, usucpf, uncid, hstacao, turidnovo, turidantigo)
						    						   VALUES ('".$iusd."',
						    						   		   (SELECT pflcod FROM sispacto2.tipoperfil WHERE iusd='".$iusd."'),
						    						   		    NOW(),
						    						   		    '".$_SESSION['usucpf']."',
						    						   		    '".$dados['uncid']."',
						    						   		    'F',
						    						   		    '".$turid."',
						            							null);");

				}
				
			}
		}
		$db->commit();
	}
	
	
	$al = array("alert"=>"Trocas efetivadas com sucesso","location"=>$_SERVER['REQUEST_URI']);
	alertlocation($al);
	
}

function atualizarEmail($dados) {
	global $db;
	
	$sql = "UPDATE sismedio.identificacaousuario SET iusemailprincipal='".$dados['iusemailprincipal']."' WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	$db->commit();
	
}

function exibirPorcentagemPagamentoPerfil($dados) {
	global $db;
	
	if($dados['uncid']) $wh[] = "i.uncid='".$dados['uncid']."'";
	
	$sql = "SELECT p.pflcod, p.pfldsc FROM sismedio.pagamentoperfil pp 
			INNER JOIN seguranca.perfil p ON p.pflcod = pp.pflcod";
	$perfil = $db->carregar($sql);
	
	echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
	if($perfil) {
		
		echo '<tr>';
		echo '<td class="SubTituloCentro">Perfil</td>';
		echo '<td class="SubTituloCentro">Total bolsistas</td>';
		echo '<td class="SubTituloCentro">Total em pagamento</td>';
		echo '<td class="SubTituloCentro">&nbsp;</td>';
		echo '<td class="SubTituloCentro">Total concluído</td>';
		echo '<td class="SubTituloCentro">&nbsp;</td>';
		echo '</tr>';
		
		foreach($perfil as $p) {
			
			echo '<tr>';
			
			$sql = "SELECT count(*) as tot FROM sismedio.identificacaousuario i 
					INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
					INNER JOIN sismedio.pagamentoperfil p ON p.pflcod = t.pflcod 
					WHERE i.iusstatus='A' AND t.pflcod='".$p['pflcod']."' AND CASE WHEN t.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN i.iustipoprofessor='censo' ELSE true END AND CASE WHEN t.pflcod=".PFL_ORIENTADORESTUDO." THEN i.iusformacaoinicialorientador=true ELSE true END AND i.uncid IN( SELECT rf.uncid FROM sismedio.folhapagamentouniversidade rf WHERE rf.fpbid='".$dados['fpbid']."' ) ".(($wh)?" AND ".implode(" AND ", $wh):"");
			
			$totalus = $db->pegaUm($sql);
			
			$sql = "SELECT count(*) as tot FROM sismedio.pagamentobolsista p 
					INNER JOIN sismedio.universidadecadastro i ON i.uniid = p.uniid 
					WHERE p.pflcod='".$p['pflcod']."' AND p.fpbid='".$dados['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sismedio.folhapagamentouniversidade rf WHERE rf.fpbid='".$dados['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");
			
			$totalpag = $db->pegaUm($sql);
			
			
			$sql = "SELECT count(*) as tot FROM sismedio.pagamentobolsista p 
					INNER JOIN sismedio.universidadecadastro i ON i.uniid = p.uniid 
					INNER JOIN workflow.documento d ON d.docid = p.docid 
					WHERE d.esdid IN('".ESD_PAGAMENTO_EFETIVADO."','".ESD_PAGAMENTO_NAO_AUTORIZADO."') AND p.pflcod='".$p['pflcod']."' AND p.fpbid='".$dados['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sismedio.folhapagamentouniversidade rf WHERE rf.fpbid='".$dados['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");
			
			$totalpagef = $db->pegaUm($sql);
			
			echo '<td>'.$p['pfldsc'].'</td>';
			echo '<td align=right>'.$totalus.'</td>';
			
			echo '<td align=right>'.(($totalpag)?$totalpag:'0').'</td>';
			if($totalus) $porc = round(($totalpag/$totalus)*100,0);
			else $porc = 0;
			echo '<td>';
			progressBar($porc);
			echo '</td>';
			
			echo '<td align=right>'.(($totalpagef)?$totalpagef:'0').'</td>';
			if($totalus) $porc = round(($totalpagef/$totalus)*100,0);
			else $porc = 0;
			echo '<td>';
			progressBar($porc);
			echo '</td>';
			
			
			echo '</tr>';
			
		}
	}
	echo '</table>';
}

function exibirPorcentagemPagamento($dados) {
	global $db;
	
	if($dados['uncid']) $wh[] = "i.uncid='".$dados['uncid']."'";
	if($dados['fpbid']) $wh1[] = "f.fpbid='".$dados['fpbid']."'";
	
	$sql = "SELECT f.fpbid, 'Ref.'||m.mesdsc||'/'||f.fpbanoreferencia as periodo FROM sismedio.folhapagamento f
			INNER JOIN public.meses m ON m.mescod::numeric = f.fpbmesreferencia 
			".(($dados['uncid'])?"INNER JOIN sismedio.folhapagamentouniversidade fp ON fp.fpbid = f.fpbid AND fp.uncid='".$dados['uncid']."' AND fp.pflcod IS NULL":"")."
			".(($wh1)?"WHERE ".implode(" AND ", $wh1):"");
	
	$folhapagamento = $db->carregar($sql);
	
	echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
	if($folhapagamento[0]) {
		
		echo '<tr>';
		echo '<td class="SubTituloCentro">&nbsp;</td>';
		echo '<td class="SubTituloCentro">Período de Referência</td>';
		echo '<td class="SubTituloCentro">Total bolsistas</td>';
		echo '<td class="SubTituloCentro">Total em pagamento</td>';
		echo '<td class="SubTituloCentro">&nbsp;</td>';
		echo '<td class="SubTituloCentro">Total concluído</td>';
		echo '<td class="SubTituloCentro">&nbsp;</td>';
		echo '<td class="SubTituloCentro">Restante</td>';
		echo '</tr>';
		
		foreach($folhapagamento as $fp) {
			echo '<tr>';
			
			$uncids = $db->carregarColuna("SELECT DISTINCT uncid FROM sismedio.tipoavaliacaoperfil WHERE fpbid <='".$fp['fpbid']."'");
			
			$sql = "SELECT count(*) as tot, sum(foo.plpvalor) as vlr FROM (
 			
 					SELECT i.iusd, i.uncid,  t.pflcod, (SELECT count(*) FROM sismedio.mensario WHERE iusd=i.iusd) as numerobolsas, p.plpvalor 
 					FROM sismedio.identificacaousuario i 
					INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
					INNER JOIN sismedio.pagamentoperfil p ON p.pflcod = t.pflcod 
					INNER JOIN sismedio.folhapagamentouniversidade rf ON rf.uncid = i.uncid AND rf.pflcod = t.pflcod AND rf.fpbid='".$fp['fpbid']."'
					LEFT JOIN sismedio.universidadecadastro u ON u.uncid = i.uncid 
					LEFT JOIN workflow.documento dc ON dc.docid = u.docidturmaorientadoresestudo 
				  	LEFT JOIN workflow.documento dc2 ON dc2.docid = u.docidturmaformadoresregionais
					WHERE i.iusstatus='A' AND CASE WHEN t.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN i.iustipoprofessor='censo' AND dc.esdid='".ESD_TURMA_FECHADA."' ELSE true END AND CASE WHEN t.pflcod=".PFL_ORIENTADORESTUDO." THEN dc2.esdid='".ESD_TURMA_FECHADA."' ELSE true END ".(($wh)?" AND ".implode(" AND ", $wh):"")."
 					
 					) foo 
					{$f}";
					
			$totalinsc = $db->pegaLinha($sql);
			$totalus = $totalinsc['tot'];
			
			$sql = "SELECT count(*) as tot, sum(p.pbovlrpagamento) as vlr FROM sismedio.pagamentobolsista p 
					INNER JOIN sismedio.universidadecadastro i ON i.uniid = p.uniid 
					WHERE p.fpbid='".$fp['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sismedio.folhapagamentouniversidade rf WHERE rf.fpbid='".$fp['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");
			
			$totalempag = $db->pegaLinha($sql);
			$totalpag = $totalempag['tot'];
			
			$sql = "SELECT count(*) as tot, sum(p.pbovlrpagamento) as vlr FROM sismedio.pagamentobolsista p 
					INNER JOIN sismedio.universidadecadastro i ON i.uniid = p.uniid 
					INNER JOIN workflow.documento d ON d.docid = p.docid 
					WHERE d.esdid IN('".ESD_PAGAMENTO_EFETIVADO."','".ESD_PAGAMENTO_NAO_AUTORIZADO."') AND p.fpbid='".$fp['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sismedio.folhapagamentouniversidade rf WHERE rf.fpbid='".$fp['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");
			
			$totalpagefetivado = $db->pegaLinha($sql);
			$totalpagef = $totalpagefetivado['tot'];
			
			
			echo '<td><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick="detalharPorcentagemPerfil('.$fp['fpbid'].',\''.$dados['uncid'].'\',this);"></td>';
			echo '<td>'.$fp['periodo'].'</td>';
			echo '<td align=right>'.$totalus.'</td>';
			
			echo '<td align=right>'.(($totalpag)?$totalpag:'0').'</td>';
			if($totalus) $porc = round(($totalpag/$totalus)*100,0);
			else $porc = 0;
			echo '<td>';
			progressBar($porc);
			echo '</td>';
			
			echo '<td align=right>'.(($totalpagef)?$totalpagef:'0').'</td>';
			if($totalus) $porc = round(($totalpagef/$totalus)*100,0);
			else $porc = 0;
			echo '<td>';
			progressBar($porc);
			echo '</td>';
			
			$totalbolsasrestante += ($totalus-$totalpagef);
			$totalvalorrestante += ($totalinsc['vlr']-$totalpagefetivado['vlr']);
			
			echo '<td nowrap align=right style=font-size:x-small;>'.number_format($totalus-$totalpagef,0,",",".").'<br>R$ '.number_format($totalinsc['vlr']-$totalpagefetivado['vlr'],2,",",".").'</td>';
			
			echo '</tr>';
			
		}
		
		echo '<tr>';
		echo '<td colspan=7 class="SubTituloDireita">Total</td>';
		echo '<td nowrap align=right style=font-size:x-small;>'.number_format($totalbolsasrestante,0,",",".").'<br>R$ '.number_format($totalvalorrestante,2,",",".").'</td>';
		echo '</tr>';
	}
	echo '</table>';
	
}

function carregarDetalhesPerfil($dados) {
	global $db;
	
	if($dados['pflcod_']==PFL_ORIENTADORESTUDO) {
		
		echo '<p align=center><b>Informações Orientador de Estudo</b></p>';
		echo '<input type="hidden" name="iustipoorientador__" value="profissionaismagisterio">';
		echo '<input type="hidden" name="iusformacaoinicialorientador__" value="TRUE">';
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		echo '<tr>';
		echo '<td class="SubTituloDireita">Escola:</td>';
		echo '<td>';
		
		$sql = "select lm.lemcodigoinep as codigo, lm.lemnomeescola as descricao from sismedio.listaescolasensinomedio lm 
				inner join sismedio.abrangencia a on a.lemcodigoinep = lm.lemcodigoinep 
				inner join sismedio.estruturacurso e on e.ecuid = a.ecuid 
				where e.uncid='".$dados['uncid']."' 
				";
		$db->monta_combo('lemcodigoinep__', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'lemcodigoinep__','', $dados['lemcodigoinep__']);
		
		echo '</td>';
	
		echo '</tr>';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">Turma:</td>';
		echo '<td>';
		$sql = "SELECT turid as codigo,
					   turdesc AS descricao
				FROM sismedio.turmas p 
				INNER JOIN sismedio.identificacaousuario i ON i.iusd = p.iusd 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_FORMADORREGIONAL."
				WHERE p.uncid='".$dados['uncid']."'
				ORDER BY 2";
		$db->monta_combo('turid__', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'turid__','', $dados['turid']);
		echo '</td>';
		
		echo '</tr>';
		
		echo '</table>';
	}
	
	if($dados['pflcod_']==PFL_PROFESSORALFABETIZADOR) {
		
		echo '<p align=center><b>Informações Professor Alfabetizador</b></p>';
		echo '<input type="hidden" name="iustipoprofessor__" value="cpflivre">';
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		echo '<tr>';
		echo '<td class="SubTituloDireita">Escola:</td>';
		echo '<td>';
		
		$sql = "select lm.lemcodigoinep as codigo, lm.lemnomeescola as descricao from sismedio.listaescolasensinomedio lm 
				inner join sismedio.abrangencia a on a.lemcodigoinep = lm.lemcodigoinep 
				inner join sismedio.estruturacurso e on e.ecuid = a.ecuid 
				where e.uncid='".$dados['uncid']."' 
				";
		$db->monta_combo('lemcodigoinep__', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'lemcodigoinep__','', $dados['lemcodigoinep__']);
		echo '</td>';
	
		echo '</tr>';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">Turma:</td>';
		echo '<td>';
		$sql = "SELECT turid as codigo,
					   i.iusnome ||' ( '||turdesc||' )' AS descricao
				FROM sismedio.turmas p 
				INNER JOIN sismedio.identificacaousuario i ON i.iusd = p.iusd 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_ORIENTADORESTUDO."
				WHERE i.uncid='".$dados['uncid']."'
				ORDER BY 2";
		
		$db->monta_combo('turid__', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'turid__','', $dados['turid__']);
		echo '</td>';
		
		echo '</tr>';
		
		echo '</table>';
	}
	
	if($dados['pflcod_']==PFL_COORDENADORLOCAL) {
		
		echo '<p align=center><b>Informações Coordenador Local</b></p>';
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		echo '<tr>';
		echo '<td class="SubTituloDireita">Esfera:</td>';
		echo '<td>';
		$sql = "(
				SELECT p.picid as codigo,
					   'Municipal : '|| m.estuf || ' / ' || m.mundescricao AS descricao
				FROM sismedio.pactoidadecerta p 
				INNER JOIN sismedio.abrangencia a ON a.muncod = p.muncod AND a.esfera='M'
				INNER JOIN sismedio.estruturacurso es ON es.ecuid = a.ecuid 
				INNER JOIN territorios.municipio m ON m.muncod = p.muncod 
				WHERE es.uncid='".$dados['uncid']."'
				ORDER BY 2
				) UNION ALL (
				SELECT  DISTINCT p.picid as codigo,
					   'Estadual : '|| e.estuf || ' / ' || e.estdescricao AS descricao
				FROM sismedio.pactoidadecerta p 
				INNER JOIN territorios.municipio m ON m.estuf = p.estuf
				INNER JOIN sismedio.abrangencia a ON a.muncod = m.muncod AND a.esfera='E'
				INNER JOIN sismedio.estruturacurso es ON es.ecuid = a.ecuid 
				INNER JOIN territorios.estado e ON e.estuf = p.estuf 
				WHERE es.uncid='".$dados['uncid']."'
				ORDER BY 2
				
				)";
		
		$db->monta_combo('picid__', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'picid__','', $dados['picid__']);
		echo '</td>';
	
		echo '</tr>';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">Restrição de bolsas:</td>';
		echo '<td>';
		
		$sql = "SELECT f.fpbid as codigo, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao 
				FROM sismedio.folhapagamento f 
				INNER JOIN sismedio.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
				INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
				WHERE f.fpbstatus='A' AND rf.uncid='".$dados['uncid']."'";
		
		$db->monta_combo('fpbidini', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'fpbidini','');
		
		echo ' à ';
		
		$db->monta_combo('fpbidfim', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'fpbidfim','');
		
		echo '</td>';
	
		echo '</tr>';
		
		echo '</table>';
	}
	
	if($dados['pflcod_']==PFL_FORMADORIES) {
		
		echo '<p align=center><b>Informações Formador IES</b></p>';
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">Restrição de bolsas:</td>';
		echo '<td>';
		
		$sql = "SELECT f.fpbid as codigo, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao 
				FROM sismedio.folhapagamento f 
				INNER JOIN sismedio.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
				INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
				WHERE f.fpbstatus='A' AND rf.uncid='".$dados['uncid']."'";
		
		$db->monta_combo('fpbidini', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'fpbidini','');
		
		echo ' à ';
		
		$db->monta_combo('fpbidfim', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'fpbidfim','');
		
		echo '</td>';
	
		echo '</tr>';
		
		echo '</table>';
	}
	
	
	if($dados['pflcod_']==PFL_SUPERVISORIES) {
		
		echo '<p align=center><b>Informações Supervisor IES</b></p>';
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">Restrição de bolsas:</td>';
		echo '<td>';
		
		$sql = "SELECT f.fpbid as codigo, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao 
				FROM sismedio.folhapagamento f 
				INNER JOIN sismedio.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
				INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
				WHERE f.fpbstatus='A' AND rf.uncid='".$dados['uncid']."'";
		
		$db->monta_combo('fpbidini', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'fpbidini','');
		
		echo ' à ';
		
		$db->monta_combo('fpbidfim', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'fpbidfim','');
		
		echo '</td>';
	
		echo '</tr>';
		
		echo '</table>';
	}
	
	
	

}

function exibirMunicipiosNaoFechados($dados) {
	global $db;
	
	$sql = "SELECT  m.estuf,
					m.mundescricao,
			    COALESCE(array_to_string(array(SELECT iusnome FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=pic.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'Coordenador Local não cadastrado') as coordenadorlocal, 
			    COALESCE(array_to_string(array(SELECT iusemailprincipal FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=pic.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'Coordenador Local não cadastrado') as emailcoordenador
			FROM sismedio.abrangencia a 
			INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sismedio.pactoidadecerta pic ON pic.muncod = a.muncod 
			LEFT JOIN workflow.documento d ON d.docid = pic.docidturma 
			WHERE e.uncid='".$dados['uncid']."' AND a.esfera='M' AND (d.esdid!='".ESD_FECHADO_TURMA."' OR d.esdid IS NULL) ORDER BY 1,2";
	
	$cabecalho = array("UF","Município","Coordenador Local","E-mail");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true,false,false,true);
	
	
}



function efetuarInsercaoUsuarioPerfil($dados) {
	global $db;
	
	
	$iusd = $db->pegaUm("SELECT iusd FROM sismedio.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array("",""),$dados['iuscpf__'])."'");
	
	if($iusd) {
		
		$sql = "UPDATE sismedio.identificacaousuario SET 
	    		iuscpf  =".(($dados['iuscpf__'])?"'".str_replace(array(".","-"),array("",""),$dados['iuscpf__'])."'":"NULL").", 
	    		iusnome =".(($dados['iusnome__'])?"'".$dados['iusnome__']."'":"NULL").", 
	    		iusemailprincipal =".(($dados['iusemailprincipal__'])?"'".$dados['iusemailprincipal__']."'":"NULL").", 
	    		iusstatus ='A', 
    			iuscodigoinep=".(($dados['lemcodigoinep__'])?"'".$dados['lemcodigoinep__']."'":"NULL").",
	    		iustipoorientador =".(($dados['iustipoorientador__'])?"'".$dados['iustipoorientador__']."'":"NULL").",
	    		muncodatuacao =".(($dados['muncodatuacao__'])?"'".$dados['muncodatuacao__']."'":"NULL").", 
	            uncid =".(($dados['uncid'])?"'".$dados['uncid']."'":"NULL").", 
	            iusformacaoinicialorientador =".(($dados['iusformacaoinicialorientador__'])?$dados['iusformacaoinicialorientador__']:"NULL").", 
	            iustipoprofessor =".(($dados['iustipoprofessor__'])?"'".$dados['iustipoprofessor__']."'":"NULL")."
				WHERE iusd='".$iusd."'";
		
		$db->executar($sql);
		
	} else {
	
		$sql = "INSERT INTO sismedio.identificacaousuario(
	            iuscpf, iusnome, 
	            iusemailprincipal, iusdatainclusao,  
	            iusstatus, iustipoorientador, 
	            muncodatuacao, uncid,  
	            iusformacaoinicialorientador, iustipoprofessor, iuscodigoinep)
	    VALUES (".(($dados['iuscpf__'])?"'".str_replace(array(".","-"),array("",""),$dados['iuscpf__'])."'":"NULL").", 
	    		".(($dados['iusnome__'])?"'".$dados['iusnome__']."'":"NULL").", 
	    		".(($dados['iusemailprincipal__'])?"'".$dados['iusemailprincipal__']."'":"NULL").", 
	    		NOW(), 
	    		'A', 
	    		".(($dados['iustipoorientador__'])?"'".$dados['iustipoorientador__']."'":"NULL").",
	    		".(($dados['muncodatuacao__'])?"'".$dados['muncodatuacao__']."'":"NULL").", 
	            ".(($dados['uncid'])?"'".$dados['uncid']."'":"NULL").", 
	            ".(($dados['iusformacaoinicialorientador__'])?$dados['iusformacaoinicialorientador__']:"NULL").", 
	            ".(($dados['iustipoprofessor__'])?"'".$dados['iustipoprofessor__']."'":"NULL").",
				".(($dados['lemcodigoinep__'])?"'".$dados['lemcodigoinep__']."'":"NULL").") RETURNING iusd";
		
		$iusd = $db->pegaUm($sql);
	
	}
	
	$sql = "SELECT p.pfldsc, p.pflcod FROM sismedio.tipoperfil t INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod WHERE t.iusd='".$iusd."'";
	$arrPf = $db->pegaLinha($sql);
	
	if($arrPf['pfldsc'] && $arrPf['pflcod']!=$dados['pflcod__']) {
		$al = array("alert"=>"Inserção não efetivada com sucesso. O usuário ja esta cadastrado com o perfil : ".$arrPf['pfldsc'],"location"=>$_SERVER['REQUEST_URI']);
		alertlocation($al);
	}
	
	$tpeid = $db->pegaUm("SELECT tpeid FROM sismedio.tipoperfil WHERE iusd='".$iusd."'");
	
	if(!$tpeid) {
		$sql = "INSERT INTO sismedio.tipoperfil(
	            iusd, pflcod, fpbidini, fpbidfim)
	    		VALUES ('".$iusd."', '".$dados['pflcod__']."', ".(($dados['fpbidini'])?"'".$dados['fpbidini']."'":"NULL").", ".(($dados['fpbidfim'])?"'".$dados['fpbidfim']."'":"NULL").");";
		
		$db->executar($sql);
	}
	
	if($dados['turid__']) {
		$otuid = $db->pegaUm("SELECT otuid FROM sismedio.orientadorturma WHERE iusd='".$iusd."'");
		
		if(!$otuid) {
			$sql = "INSERT INTO sismedio.orientadorturma(
	            	turid, iusd)
	    			VALUES ('".$dados['turid__']."', '".$iusd."');";
	
			$db->executar($sql);
		} else {
			
			$sql = "UPDATE sismedio.orientadorturma SET turid='".$dados['turid__']."' WHERE iusd='".$iusd."';";
			$db->executar($sql);
			
		}
	}
	
	$db->executar("INSERT INTO sismedio.historicotrocausuario(
            				   iusdnovo, pflcod, hstdata, usucpf, uncid, hstacao, turidnovo, turidantigo)
    						   VALUES ('".$iusd."', 
    						   		   '".$dados['pflcod__']."', 
    						   		    NOW(), 
    						   		    '".$_SESSION['usucpf']."', 
    						   		    '".$dados['uncid']."', 
    						   		    'I', 
    						   		    ".(($dados['turid__'])?"'".$dados['turid__']."'":"NULL").", 
            							NULL);");
	
	$db->commit();
	
	gerarVersaoProjetoUniversidade(array('uncid' => $dados['uncid']));
	
	$al = array("alert"=>"Inserção efetivada com sucesso","location"=>$_SERVER['REQUEST_URI']);
	alertlocation($al);
	
	
}

function recuperarSenhaSIMEC($dados) {
	global $db;
	echo "SENHA : ".md5_decrypt_senha($db->pegaUm("SELECT ususenha FROM seguranca.usuario WHERE usucpf='".$dados['cpf']."'"),'')."<br>";	
}

function sincronizarUsuariosSIMEC($dados) {
	global $db;
	$sql = "UPDATE seguranca.usuario u SET 
			usufoneddd=CASE WHEN (foo.usufoneddd IS NULL OR foo.usufoneddd='55') THEN foo.dddtel::character(2) ELSE foo.usufoneddd END,
			usufonenum=CASE WHEN (foo.usufonenum IS NULL OR foo.usufonenum='5555-5555') THEN foo.tel ELSE foo.usufonenum END,
			muncod=CASE WHEN (foo.muncod_segur IS NULL OR foo.muncod_segur='5300108') THEN foo.muncod_pacto ELSE foo.muncod_segur END,
			regcod=CASE WHEN (foo.estuf_segu IS NULL OR foo.estuf_segu='DF') THEN foo.estuf_pacto ELSE foo.estuf_segu END,
			tpocod=CASE WHEN foo.tpocod IS NULL THEN '1' ELSE foo.tpocod END,
			entid=CASE WHEN foo.entid IS NULL AND (foo.orgcod IS NULL OR foo.orgcod='Não registrado') THEN 390402 ELSE foo.entid END,
			usudatanascimento=CASE WHEN foo.usudatanascimento IS NULL THEN foo.iusdatanascimento ELSE foo.usudatanascimento END,
			carid=CASE WHEN foo.carid IS NULL THEN 9 ELSE foo.carid END,
			usufuncao=CASE WHEN foo.funcao_segur IS NULL THEN foo.funcao_pacto ELSE foo.funcao_segur END,
			ususexo=foo.iussexo,
			usunomeguerra=CASE WHEN foo.apelido_segur IS NULL THEN foo.apelido_pacto ELSE foo.apelido_segur END
			FROM(
			SELECT 
			i.iuscpf,
			(SELECT itedddtel FROM sismedio.identificacaotelefone WHERE iusd=i.iusd AND itetipo='T') as dddtel,
			u.usufoneddd,
			(SELECT itenumtel FROM sismedio.identificacaotelefone WHERE iusd=i.iusd AND itetipo='T') as tel,
			u.usufonenum,
			i.muncodatuacao as muncod_pacto,
			u.muncod as muncod_segur,
			m.estuf as estuf_pacto,
			u.regcod as estuf_segu,
			u.tpocod,
			u.entid,
			u.orgcod,
			i.iusdatanascimento,
			u.usudatanascimento,
			u.carid,
			u.usufuncao as funcao_segur,
			p.pfldsc || ' - SISMédio' as funcao_pacto,
			u.ususexo,
			i.iussexo,
			split_part(i.iusnome, ' ', 1) as apelido_pacto,
			u.usunomeguerra as apelido_segur
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.usuario u ON u.usucpf = i.iuscpf 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
			INNER JOIN territorios.municipio m ON m.muncod = i.muncodatuacao 
			WHERE i.iuscpf='".str_replace(array(".","-"),array("",""),$dados['cpf'])."'
			)foo WHERE foo.iuscpf = u.usucpf ";
	
	$db->executar($sql);
	$db->commit();
	
}


function carregarOrientadoresSISPorMunicipio($dados) {
	global $db;
	
	include_once '_funcoes_coordenadorlocal.php';
	
	///////////////////////////////////////////////////////////
	$p = $db->pegaLinha("SELECT p.picid, 
														  p.muncod, 
														  p.estuf, 
														  CASE WHEN m.muncod IS NOT NULL THEN m.estuf||' / '||m.mundescricao ELSE e.estuf||' / '||e.estdescricao END as descricao
												   FROM sismedio.pactoidadecerta p 
						    		  		   	   INNER JOIN workflow.documento d ON d.docid = p.docid 
						    		  		   	   LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
						    		  		   	   LEFT JOIN territorios.estado e ON e.estuf = p.estuf 
						    		  		   	   WHERE p.picid='".$dados['picid']."'");
	
	if($p) {
		
		$db->executar("UPDATE sismedio.pactoidadecerta SET picselecaopublica=true, picincluirprofessorrede=false WHERE picid='".$p['picid']."'");
		$db->commit();
			
		$ar = array("estuf" 	  => $p['estuf'],
					"muncod" 	  => $p['muncod'],
					"dependencia" => (($p['muncod'])?'municipal':'estadual'));
		
		$totalalfabetizadores = carregarTotalAlfabetizadores($ar);
		
		$orientadoresestudo = carregarDadosIdentificacaoUsuario(array("picid"=>$p['picid'],"pflcod"=>PFL_ORIENTADORESTUDO));
		
		if($totalalfabetizadores['total_orientadores_a_serem_cadastrados'] > count($orientadoresestudo)) {
			$restantes = ($totalalfabetizadores['total_orientadores_a_serem_cadastrados']-count($orientadoresestudo));
			for($i = 0;$i < $restantes;$i++) {
				
				$num_ius = $db->pegaUm("SELECT substr(iuscpf, 8) as num FROM sismedio.identificacaousuario WHERE picid='".$p['picid']."' AND iuscpf ilike 'SIS%' ORDER BY iusd DESC");
				if($num_ius) $num_ius++;
				else $num_ius=1;
				
				$iuscpf  		   = "SIS".str_pad($p['picid'], 4, "0", STR_PAD_LEFT).str_pad($num_ius, 4, "0", STR_PAD_LEFT);
				$iusnome 		   = "Orientador de Estudo - ".str_replace("'"," ",$p['descricao'])." - ".str_pad($num_ius, 4, "0", STR_PAD_LEFT);
				$iusemailprincipal = "noemail@noemail.com";
				
				if($p['muncod']) {
					$uncid = $db->pegaUm("SELECT e.uncid FROM sismedio.abrangencia a 
										  INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
										  WHERE a.muncod='".$p['muncod']."' AND esfera='M'");
				} elseif($p['estuf']) {
					$uncid = $db->pegaUm("SELECT e.uncid FROM sismedio.abrangencia a 
										  INNER JOIN territorios.municipio m ON m.muncod = a.muncod
										  INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
										  WHERE m.estuf='".$p['estuf']."' AND esfera='E'");
					
				}
				
				$sql = "INSERT INTO sismedio.identificacaousuario(picid, 
																  muncod, 
																  iuscpf, 
																  iusnome, 
            													  iusemailprincipal, 
            													  iustipoorientador, 
            													  muncodatuacao,
            													  iusdatainclusao,
            													  uncid
            													   )
					    VALUES ('".$p['picid']."', 
					    		".(($p['muncod'])?"'".$p['muncod']."'":"NULL").", 
					    		'".$iuscpf."', 
					    		'".$iusnome."', 
					    		'".$iusemailprincipal."', 
					    		'profissionaismagisterio', 
					    		".(($p['muncod'])?"'".$p['muncod']."'":"NULL").",
					    		NOW(),
					    		".(($uncid)?"'".$uncid."'":"NULL").") RETURNING iusd;";
				
				$iusd = $db->pegaUm($sql);
				
				$sql = "INSERT INTO sismedio.tipoperfil( iusd, pflcod, tpestatus)
    					VALUES ( '".$iusd."', '".PFL_ORIENTADORESTUDO."', 'A');";
				
				$db->executar($sql);
				
				if($uncid) {
					$turid = $db->pegaUm("SELECT t.turid FROM sismedio.turmas t 
										  INNER JOIN sismedio.identificacaousuario i ON i.iusd = t.iusd 
										  INNER JOIN sismedio.tipoperfil tt ON tt.iusd = i.iusd 
										  WHERE tt.pflcod='".PFL_FORMADORIES."' AND i.uncid='".$uncid."' LIMIT 1");
					
					if($turid) {
						$db->executar("INSERT INTO sismedio.orientadorturma(
									            turid, iusd, otustatus, otudata)
									    VALUES ('".$turid."', '".$iusd."', 'A', NOW());");
					}
				}
				
				
			}
			
			$db->commit();
		} else {
			$al = array("alert"=>"O município selecionado não possui vagas para Orientadores de Estudo.","location"=>"sismedio.php?modulo=principal/mec/mec&acao=A");
			alertlocation($al);
		}
	
	}
	
	$al = array("alert"=>"Foram inseridos {$restantes} Orientadores de Estudo SIS.","location"=>"sismedio.php?modulo=principal/mec/mec&acao=A");
	alertlocation($al);
	
	
}

function invalidarMensario($dados) {
	global $db;
	$sql = "SELECT d.esdid FROM sismedio.mensario m 
			INNER JOIN workflow.documento d ON d.docid = m.docid 
			WHERE m.docid='".$dados['docidmensario']."'";
	$esdidorigem = $db->pegaUm($sql);
	
	$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem='".$esdidorigem."' AND esdiddestino='".ESD_INVALIDADO_MENSARIO."' AND aedstatus='A'";
	$aedid = $db->pegaUm($sql);
	
	if($aedid) {
		wf_alterarEstado( $dados['docidmensario'], $aedid, $dados['cmddsc'], array());
	}

	$al = array("alert"=>"Mensário invalidado com sucesso","location"=>"sismedio.php?modulo={$dados['modulo']}&acao=A&aba=aprovarusuario&fpbid=".$dados['fpbid']."&pflcodaprovar=".$dados['pflcodaprovar']);
	alertlocation($al);
	
}

function corrigirAcessoUniversidade($dados) {
	global $db;
	$sql = "SELECT i.uncid, i.iuscpf, i.picid, t.pflcod, i.muncodatuacao, i.iusd FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = t.iusd
			WHERE iuscpf='".$_SESSION['usucpf']."'";
	
	$identificacaousuario = $db->pegaLinha($sql);
	
	if($identificacaousuario['uncid']) {
		
		$sql = "UPDATE sismedio.usuarioresponsabilidade SET uncid='".$identificacaousuario['uncid']."' WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".$identificacaousuario['pflcod']."' AND rpustatus='A'";
		$db->executar($sql);
		$db->commit();

		if($dados['sis']) $_SESSION['sismedio'][$dados['sis']]['uncid'] = $identificacaousuario['uncid'];
		
	} elseif($identificacaousuario['picid']) {
		
		$sql = "SELECT * FROM sismedio.pactoidadecerta WHERE picid=".$identificacaousuario['picid'];
		$pactoidadecerta = $db->pegaLinha($sql);
		
		if($pactoidadecerta['estuf'] && $identificacaousuario['muncodatuacao']) {
			$esfera = "E";
			$muncod = $identificacaousuario['muncodatuacao'];
		}
		
		if($pactoidadecerta['muncod']) {
			$esfera = "M";
			$muncod = $pactoidadecerta['muncod'];
		}
		
		$sql = "SELECT uncid FROM sismedio.abrangencia a 
				INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.muncod='".$muncod."' AND a.esfera='".$esfera."'";
		
		$uncid = $db->pegaUm($sql);
		
		if($uncid) {
			$db->executar("UPDATE sismedio.identificacaousuario SET uncid='".$uncid."' WHERE iusd='".$identificacaousuario['iusd']."'");
			$db->executar("UPDATE sismedio.usuarioresponsabilidade SET uncid='".$uncid."' WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".$identificacaousuario['pflcod']."' AND rpustatus='A'");
			$db->commit();
		}
		
		if($dados['sis']) $_SESSION['sismedio'][$dados['sis']]['uncid'] = $uncid;
		
	}
}

function criarDocumentosPagamentos($dados) {
	global $db;
	
	$pagamentos = $db->carregar("SELECT p.pboid, pf.pfldsc, i.iuscpf, i.iusnome, f.fpbmesreferencia, f.fpbanoreferencia FROM sismedio.pagamentobolsista p 
								 INNER JOIN seguranca.perfil pf ON pf.pflcod = p.pflcod 
								 INNER JOIN sismedio.identificacaousuario i ON i.iusd = p.iusd 
								 INNER JOIN sismedio.folhapagamento f ON f.fpbid = p.fpbid 
								 WHERE docid IS NULL");
	
	if($pagamentos[0]) {
		foreach($pagamentos as $arrInfo) {
			$docid = wf_cadastrarDocumento(TPD_PAGAMENTOBOLSA, "Pagamento - ".$arrInfo['pfldsc']." - (".$arrInfo['iuscpf'].")".$arrInfo['iusnome']." - ".$arrInfo['fpbmesreferencia']."/".$arrInfo['fpbanoreferencia']);
			$db->executar("UPDATE sismedio.pagamentobolsista SET docid='".$docid."' WHERE pboid='".$arrInfo['pboid']."'");
		}
		
		$db->commit();
	}
	
	echo "Número de documento atualizados : ".count($pagamentos);
	
}

function atualizarNomeUsuario($dados) {
	global $db;
	
	include_once '../includes/webservice/cpf.php';
	
	$objPessoaFisica = new PessoaFisicaClient("http://ws.mec.gov.br/PessoaFisica/wsdl");
	$xml 			 = $objPessoaFisica->solicitarDadosPessoaFisicaPorCpf($dados['iuscpf']);
		
	$obj = (array) simplexml_load_string($xml);
	
	if($obj['PESSOA']->no_pessoa_rf) {
		$db->executar("UPDATE sismedio.identificacaousuario SET iusnome='".$obj['PESSOA']->no_pessoa_rf."' WHERE iuscpf='".$dados['iuscpf']."'");
		$db->executar("UPDATE seguranca.usuario SET usunome='".$obj['PESSOA']->no_pessoa_rf."' WHERE usucpf='".$dados['iuscpf']."'");
		$db->commit();
	}
	
	$al = array("alert"=>"Nome atualizado com sucesso","location"=>"sismedio.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=dados");
	alertlocation($al);
	
	
	
}

function aprovarTrocaNomesSGB($dados) {
	global $db;
	
	if($dados['cpf']) {
		foreach($dados['cpf'] as $cpf) {
			if($dados['nome_receita'][$cpf]) {
				$sql = "UPDATE sismedio.identificacaousuario SET iusnome='".$dados['nome_receita'][$cpf]."' WHERE iuscpf='".str_replace(array(".","-"),array("",""),$cpf)."'";
				$db->executar($sql);
			}	
		}
		$db->commit();
	}
	
	$al = array("alert"=>"Trocas realizadas com sucesso","location"=>"sismedio.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=aprovarnomes");
	alertlocation($al);
	
}

function pegarRestricaoPagamento($dados) {
	global $db;
	
	$sql = "SELECT 			   CASE WHEN foo.mensarionota < 7		       THEN '<span style=color:red;>Avaliação do usuário não atingiu a nota mínima de 7(sete) ou não foi concluída</span>'
			WHEN foo.iustermocompromisso        =false     THEN '<span style=color:red;>Bolsista não preencheu o termo de compromisso</span>'
			WHEN foo.iusnaodesejosubstituirbolsa=true      THEN '<span style=color:red;>Bolsista do FNDE/MEC e não deseja substituir bolsa atual pela bolsa do PACTO</span>' 
			WHEN foo.fpbidini IS NOT NULL AND foo.fpbidfim IS NOT NULL AND (foo.fpbid < foo.fpbidini OR foo.fpbid > foo.fpbidfim) THEN '<span style=color:red;>Este período de referência não esta habilitado para pagamento</span>'
			WHEN foo.numeropagamentos >= foo.plpmaximobolsas THEN '<span style=color:red;font-size:x-small;>Atingiu o número máximo ('||foo.plpmaximobolsas||' bolsas)</span>'
			WHEN foo.numeropagamentosvaga >= foo.plpmaximobolsas THEN '<span style=color:red;font-size:x-small;>Atingiu o número máximo ('||foo.plpmaximobolsas||' bolsas)</span>'
			WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." THEN CASE  WHEN foo.iusdocumento=false   THEN '<center><span style=color:red;>Possui problemas na documentação</span></center>'
					   				   -- WHEN foo.numeroavaliacoes < 2 THEN '<span style=color:red;>Para o perfil de Orientador de Estudo, número de avaliadores('||foo.numeroavaliacoes||') é Insuficiente </span>' 
					   				    ELSE '<span style=color:blue;>Nenhuma restrição - Aguardando aprovação do Coordenador Geral/Adjunto</span>' END
	   		 WHEN foo.pflcod=".PFL_COORDENADORPEDAGOGICO." THEN 
					   														CASE WHEN foo.notacomplementar = 0 THEN '<span style=color:red;font-size:x-small;>Avaliação Complementar do mês referente não cadastrada</span>'
			    																 WHEN foo.resposta IS NULL  THEN '<span style=color:red;font-size:x-small;>Atividades obrigatórias não realizadas</span>' 
					   															 ELSE '<span style=color:blue;font-size:x-small;>Nenhuma restrição</span>' END 
		 	    			
			WHEN foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN 
								      CASE WHEN foo.resposta IS NULL  THEN '<span style=color:red;font-size:x-small;>Atividades obrigatórias não realizadas</span>'
			    						WHEN foo.iustipoprofessor = 'censo' THEN '<span style=color:blue;>Nenhuma restrição - Aguardando aprovação do Coordenador Geral/Adjunto</span>' 
				   				      ELSE '<span style=color:red;>Professor/Coordenador pedagógico não cadastrado no censo 2013</span>' END 
			ELSE '<span style=color:blue;>Nenhuma restrição - Aguardando aprovação do Coordenador Geral/Adjunto</span>' END as restricao
		FROM (
		SELECT  m.menid,
				i.iustermocompromisso, 
				m.fpbid,
				p.pflcod,
				i.iusdocumento,
				i.iustipoprofessor,
				i.iusnaodesejosubstituirbolsa,
			    (SELECT COUNT(pboid) FROM sismedio.pagamentobolsista p INNER JOIN workflow.documento d ON d.docid = p.docid WHERE pflcod=".PFL_FORMADORIES." AND d.esdid!=".ESD_PAGAMENTO_NAO_AUTORIZADO." AND p.uniid=unc.uniid) as numerobolsasformadoriespories,
				(SELECT COUNT(mavid) FROM sismedio.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes,
				COALESCE((SELECT AVG(mavtotal) FROM sismedio.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
			    COALESCE((SELECT AVG(iccvalor) FROM sismedio.respostasavaliacaocomplementar r INNER JOIN sismedio.itensavaliacaocomplementarcriterio ic ON ic.iccid = r.iccid WHERE r.iusdavaliado=i.iusd AND r.fpbid = m.fpbid),0.00) as notacomplementar,
				(SELECT COUNT(pboid) FROM sismedio.pagamentobolsista pg WHERE pg.iusd=i.iusd) as numeropagamentos,
			    (SELECT COUNT(pboid) FROM sismedio.pagamentobolsista pg WHERE pg.tpeid=t.tpeid) as numeropagamentosvaga,
    			pp.plpmaximobolsas,
				t.fpbidini,
				t.fpbidfim,
				fpu.rfuparcela,
				cr.resposta 
		FROM sismedio.mensario m 
		INNER JOIN sismedio.identificacaousuario i ON i.iusd = m.iusd 
		INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
    	INNER JOIN sismedio.pagamentoperfil pp ON pp.pflcod = p.pflcod
		INNER JOIN sismedio.universidadecadastro unc ON unc.uncid = i.uncid 
		INNER JOIN sismedio.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = t.pflcod AND fpu.fpbid = m.fpbid 		    			 
		LEFT JOIN (
		SELECT COUNT(DISTINCT c.carid) as resposta, c.iusd FROM sismedio.cadernoatividadesrespostas c WHERE caroeproposatividadecadernoformacao IS NOT NULL AND c.iusd='".$dados['iusd']."' AND c.fpbid='".$dados['fpbid']."' GROUP BY c.iusd
		) cr ON cr.iusd = m.iusd
		WHERE i.iusd='".$dados['iusd']."' AND m.fpbid='".$dados['fpbid']."'
		) foo";
	
	$restricao = $db->pegaUm($sql);
	
	return $restricao;
}

function reenviarPagamentos($dados) {
	global $db;
	
	if($dados['doc']) {
		foreach($dados['doc'] as $docid) {
			
			$sql = "SELECT a.aedid FROM workflow.documento d 
					INNER JOIN workflow.acaoestadodoc a ON a.esdidorigem = d.esdid AND a.esdiddestino='".ESD_PAGAMENTO_AUTORIZADO."' 
					WHERE d.docid='".$docid."'";
			$aedid = $db->pegaUm($sql);
			
			if($aedid) {
				$result = wf_alterarEstado( $docid, $aedid, $cmddsc = '', $dados);
				$db->executar("UPDATE sismedio.pagamentobolsista SEt remid=null WHERE docid='".$docid."'");
				$db->commit();
			}
			
		}
	}
	
	$al = array("alert"=>"Reenvio agendado com sucesso","location"=>"sismedio.php?modulo=principal/mec/mec&acao=A&aba=reenviarpagamentos");
	alertlocation($al);
	
	
}

function excluirAvaliacoesMensario($dados) {
	global $db;
	
	$db->executar("DELETE FROM sismedio.historicoreaberturanota WHERE mavid='".$dados['mavid']."'");
	$db->executar("DELETE FROM sismedio.mensarioavaliacoes WHERE mavid='".$dados['mavid']."'");
	
	$db->commit();
	
	$al = array("alert"=>"Avaliação apagada","location"=>"sismedio.php?modulo=".$dados['modulo']."&acao=A&aba=".$dados['aba']."&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
	
	
}

function exibirMunicipiosAtuacao($dados) {
	global $db;
	
	$identificacaousuario = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf, i.muncodatuacao, p.estuf 
											FROM sismedio.identificacaousuario i 
											INNER JOIN sismedio.pactoidadecerta p ON p.picid = i.picid
											WHERE iuscpf='".$dados['iuscpf']."'");
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr>';
	echo '<td class="SubTituloCentro" colspan="2">Município de atuação</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="25%">Usuário</td>';
	echo '<td>'.$identificacaousuario['iuscpf'].' - '.$identificacaousuario['iusnome'].'</td>';
	echo '</tr>';	
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="25%">UF</td>';
	echo '<td>'.$identificacaousuario['estuf'].'</td>';
	echo '</tr>';	
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="25%">Município</td>';
	echo '<td>';
	
	$sql = "SELECT 
				m.muncod as codigo, m.mundescricao as descricao 
			FROM territorios.municipio m 
			INNER JOIN sismedio.pactoidadecerta p ON p.estuf = m.estuf 
			INNER JOIN sismedio.identificacaousuario i ON i.picid = p.picid 
			WHERE i.iuscpf='".$dados['iuscpf']."' ORDER BY m.mundescricao";
	
	$db->monta_combo('muncodatuacao', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'muncodatuacao','', $identificacaousuario['muncodatuacao']);
	
	echo '</td>';
	echo '</tr>';	
	echo '<tr>';
	echo '<td class="SubTituloCentro" colspan="2"><input type="button" name="atualizar" value="Atualizar" onclick="atualizarMunicipioAtuacao(\''.$identificacaousuario['iusd'].'\',document.getElementById(\'muncodatuacao\').value);"></td>';
	echo '</tr>';
	echo '</table>';
		
}

function atualizarMunicipioAtuacao($dados) {
	global $db;
	
	$db->executar("UPDATE sismedio.identificacaousuario SET muncodatuacao='".$dados['muncod']."' WHERE iusd='".$dados['iusd']."'");
	$db->commit();
	
}

function carregarTurmasUniversidade($dados) {
	global $db;
	
	if($dados['pflcod']) {

	    $sql = "SELECT turid as codigo, i.iusnome || ' ( '||tu.turdesc||' )' as descricao 
	    		FROM sismedio.turmas tu 
	    		INNER JOIN sismedio.identificacaousuario i ON i.iusd = tu.iusd 
	    		INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
	    		INNER JOIN sismedio.universidadecadastro unc ON unc.uncid = i.uncid 
	    		INNER JOIN sismedio.universidade uni ON uni.uniid = unc.uniid 
	    		WHERE pflcod='".$dados['pflcod']."' AND unc.uncid='".$dados['uncid']."' ORDER BY descricao";
	    
	    $db->monta_combo('turid_destino', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'turid_destino','', $_REQUEST['turid']);
	    
    } 
	
}

function trocarUniversidade($dados) {
	global $db;
	
	$sql = "UPDATE sismedio.identificacaousuario SET uncid='".$dados['uncid']."' WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	$sql = "UPDATE sismedio.usuarioresponsabilidade SET uncid='".$dados['uncid']."' WHERE usucpf=(SELECT iuscpf FROM sismedio.identificacaousuario WHERE iusd='".$dados['iusd']."') AND rpustatus='A'";
	$db->executar($sql);
	$sql = "UPDATE sismedio.turmas SET uncid='".$dados['uncid']."' WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	if($dados['turid_destino']) {
		$sql = "UPDATE sismedio.orientadorturma SET turid='".$dados['turid_destino']."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	}
	
	$db->commit();
	
	$sql = "SELECT turid FROM sismedio.turmas WHERE iusd='".$dados['iusd']."'";
	$turid = $db->pegaUm($sql);
	
	if($turid) {
		$sql = "SELECT * FROM sismedio.orientadorturma WHERE turid='".$turid."'";
		$mt = $db->carregar($sql);
	}
	
	if($mt[0]) {
		foreach($mt as $m) {
			$msg .= trocarUniversidade(array('iusd' => $m['iusd'],'uncid' => $dados['uncid'],'return' => true));
		}
	}
	
	$iu = $db->pegaLinha("SELECT iusnome, pfldsc FROM sismedio.identificacaousuario i 
					INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
					INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod  
					WHERE i.iusd='".$dados['iusd']."'");

	$msg = $iu['iusnome']."( ".$iu['pfldsc']." ) foi atualizado com sucesso;";

	if($dados['return']) {
		return $msg;
	} else {
		$al = array("alert"=>str_replace(";",'\n',$msg),"location"=>"sismedio.php?modulo=principal/mec/mec&acao=A&aba=trocaruniversidade");
		alertlocation($al);

	}
	
	
}


function esconderAba($dados) {
	return false;
}

function condicaoComposicaoTurma($tipo) {
	global $db;
	
	$esdid = $db->pegaUm("SELECT d.esdid FROM sismedio.universidadecadastro c INNER JOIN workflow.documento d ON d.docid = c.docid WHERE c.uncid='".$_SESSION['sismedio']['uncid']."'");
	
	if($esdid != ESD_VALIDADO_COORDENADOR_IES) return 'Projeto não foi validado pelo MEC';
	
	switch($tipo) {
		case 'oe':
			$num_pf = $db->pegaUm("SELECT count(*) FROM sismedio.identificacaousuario i 
									 INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
									 LEFT JOIN sismedio.orientadorturma ot ON ot.iusd = i.iusd  
									 WHERE t.pflcod IN('".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORPEDAGOGICO."') AND i.iusstatus='A' AND i.uncid='".$_SESSION['sismedio']['uncid']."' AND ot.otuid IS NULL");
			
			if($num_pf) return 'Existem '.$num_pf.' professores que não foram alocados em turmas';
			else return true;
			
			break;
		case 'fr':
			$num_oe = $db->pegaUm("SELECT count(*) FROM sismedio.identificacaousuario i
									 INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd
									 LEFT JOIN sismedio.orientadorturma ot ON ot.iusd = i.iusd
									 WHERE t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.iusstatus='A' AND i.uncid='".$_SESSION['sismedio']['uncid']."' AND ot.otuid IS NULL");
				
			if($num_oe) return 'Existem '.$num_oe.' professores que não foram alocados em turmas';
			else return true;
				
			break;
		default:
			return 'Não foi identificado o tipo da composição da turma';
			
	}

}


function criarBotoesNavegacao($dados) {
	global $db;

	if($dados['url']) {

		$aba = $db->pegaLinha("SELECT abaendereco, abaordem, abapai, abafuncaomostrar FROM sismedio.abas WHERE abaendereco='".$dados['url']."'");

		$abaanterior = $db->pegaLinha("SELECT abaendereco, abafuncaomostrar FROM sismedio.abas WHERE abaordem='".($aba['abaordem']-1)."' AND abapai='".$aba['abapai']."'");
		$abaproxima = $db->pegaLinha("SELECT abaendereco, abafuncaomostrar FROM sismedio.abas WHERE abaordem='".($aba['abaordem']+1)."' AND abapai='".$aba['abapai']."'");

		if($abaanterior) {
			$mostrar = true;
			if($abaanterior['abafuncaomostrar']) {
				if(function_exists($abaanterior['abafuncaomostrar'])) $mostrar = $abaanterior['abafuncaomostrar']($abaanterior);
			}

			if($mostrar) echo "<input type=button value=Anterior onclick=\"divCarregando();window.location='".$abaanterior['abaendereco']."';\">";
		}

		if($dados['funcao']) {
			echo "<input type=button name=salvar id=salvar value=Salvar onclick=\"".$dados['funcao']."('".$aba['abaendereco']."');\">";
				
			if($abaproxima) {

				$mostrar = true;
				if($abaproxima['abafuncaomostrar']) {
					if(function_exists($abaproxima['abafuncaomostrar'])) $mostrar = $abaproxima['abafuncaomostrar']($abaproxima);
				}
					
				if($mostrar) echo "<input type=button name=salvarcontinuar id=salvarcontinuar value=\"Salvar e Continuar\" onclick=\"".$dados['funcao']."('".$abaproxima['abaendereco']."');\">";
			}
				
		}

		if($abaproxima) {
			$mostrar = true;
			if($abaproxima['abafuncaomostrar']) {
				if(function_exists($abaproxima['abafuncaomostrar'])) $mostrar = $abaproxima['abafuncaomostrar']($abaproxima);
			}

			if($mostrar) echo "<input type=button value=Próximo onclick=\"divCarregando();window.location='".$abaproxima['abaendereco']."';\">";
		}

	}

}

function monitoramentoTextual($dados) {

	if($dados['sis']=='orientadorestudo') {
		echo "
<p>Conforme o inciso V do artigo 15 da Resolução nº 4 de 27 de fevereiro de 2013, são atribuições do Orientador de Estudo:</p>
<p>a) participar dos encontros presenciais junto às IES, alcançando no mínimo 75% de presença;</p>
<p>b) assegurar que todos os professores alfabetizadores sob sua responsabilidade assinem o Termo de Compromisso do Bolsista (Anexo I), encaminhando-os ao coordenador-geral da Formação na IES;</p>
<p>c) ministrar a formação aos professores alfabetizadores em seu município ou polo de formação;</p>
<p>d) planejar e avaliar os encontros de formação junto aos professores alfabetizadores;</p>
<p>e) acompanhar a prática pedagógica dos professores alfabetizadores;</p>
<p>f) avaliar os professores alfabetizadores cursistas quanto à frequência, à participação e ao acompanhamento dos estudantes, registrando as informações no SISMédio;</p>
<p>g) efetuar e manter atualizados os dados cadastrais dos professores alfabetizadores;</p>
<p>h) analisar os relatórios das turmas de professores alfabetizadores e orientar os encaminhamentos;</p>
<p>i) manter registro de atividades dos professores alfabetizadores em suas turmas de alfabetização;</p>
<p><input type=checkbox id=declaro name=declaro onclick=\"declaracaoatribuicoes(this);\" > Declaro ter ciência das minhas atribuições.</p>
				";
	}

	if($dados['sis']=='coordenadorlocal') {
		echo "
<p>Conforme o inciso V do artigo 15 da Resolução nº 4 de 27 de fevereiro de 2013, são atribuições do coordenador das ações do Pacto nos estados, Distrito Federal e municípios:</p>
<p>a) dedicar-se às Ações do Pacto e atuar na Formação na qualidade de gestor das ações;</p>
<p>b) cadastrar os orientadores de estudo e os professores alfabetizadores no SISMédio e no SGB;</p>
<p>c) monitorar a realização dos encontros presenciais ministrados pelos orientadores de estudo junto aos professores alfabetizadores;</p>
<p>d) apoiar as IES na organização do calendário acadêmico, na definição dos polos de formação e na adequação das instalações físicas para a realização dos encontros presenciais;</p>
<p>e) assegurar, junto à respectiva secretaria de Educação, as condições de deslocamento e hospedagem para participação nos encontros presenciais dos orientadores de estudo e dos professores alfabetizadores, sempre que necessário;</p>
<p>f) articular-se com os gestores escolares e coordenadores pedagógicos visando ao fortalecimento da Formação Continuada de Professores Alfabetizadores;</p>
<p>g) organizar e coordenar o seminário de socialização de experiências em seu âmbito de atuação (municipal, estadual ou distrital);</p>
<p>h) monitorar o recebimento e devida utilização dos materiais pedagógicos previstos nas ações do Pacto;</p>
<p>i) acompanhar as ações da secretaria de Educação na aplicação das avaliações diagnósticas, e assegurar que os professores alfabetizadores registrem os resultados obtidos pelos alunos no SISMédio;</p>
<p>j) acompanhar as ações da Secretaria de Educação na aplicação das avaliações externas, assegurando as condições logísticas necessárias;</p>
<p>k) manter canal de comunicação permanente com o Conselho Estadual ou Municipal de Educação e com os Conselhos Escolares, visando a disseminar as ações do Pacto, prestar os esclarecimentos necessários e encaminhar eventuais demandas junto à secretaria de Educação e à SEB/MEC; e</p>
<p>l) reunir-se constantemente com o titular da secretaria de Educação para avalia</p>
<p><input type=checkbox id=declaro name=declaro onclick=\"declaracaoatribuicoes(this);\" > Declaro ter ciência das minhas atribuições.</p>
			";
	}

	if($dados['sis']=='formadories') {
		echo "
<p>Conforme o inciso IV do artigo 15 da Resolução nº 4 de 27 de fevereiro de 2013, são atribuições do Formador da IES:</p>
<p>a) planejar e avaliar as atividades da Formação;</p>
<p>b) ministrar a Formação aos orientadores de estudo;</p>
<p>c) validar, junto ao coordenador-adjunto, os cadastros dos orientadores de estudo e dos professores alfabetizadores nos sistemas do MEC e do FNDE;</p>
<p>d) monitorar a frequência, a participação e as avaliações dos orientadores de estudo no SISMédio;</p>
<p>e) acompanhar as atividades dos orientadores de estudo junto aos professores alfabetizadores;</p>
<p>f) organizar os seminários ou encontros com os orientadores de estudo para acompanhamento e avaliação da Formação;</p>
<p>g) analisar e discutir os relatórios de formação com os orientadores de estudo;</p>
<p>h) elaborar e encaminhar ao supervisor da Formação os relatórios dos encontros presenciais;</p>
<p>i) analisar, em conjunto com os orientadores de estudo, os relatórios das turmas de professores alfabetizadores e orientar os encaminhamentos;</p>
<p>j) encaminhar a documentação necessária para a certificação dos orientadores de estudo e dos professores alfabetizadores; e</p>
<p>k) acompanhar, no SISMédio, o desempenho das atividades de formação previstas para os orientadores de estudo sob sua responsabilidade, informando ao supervisor sobre eventuais ocorrências que interfiram no pagamento da bolsa no período.</p>
<p><input type=checkbox id=declaro name=declaro onclick=\"declaracaoatribuicoes(this);\" > Declaro ter ciência das minhas atribuições.</p>
			";
	}
	
	if($dados['sis']=='supervisories') {
	echo "<p>Art. 14. Os supervisores da formação, responsáveis pela articulação entre as IES e as secretarias estaduais e distrital de educação, serão selecionados pelo dirigente da secretaria estadual ou distrital de educação e pelo Coordenador-Geral das IES, respeitandose os pré-requisitos estabelecidos para a função quanto à formação e à experiência exigidas, entre candidatos que reúnem, no mínimo, as seguintes características cumulativas:</p>
		  <p>I - ter Licenciatura ou Complementação Pedagógica;<br>
		     II - ser professor/coordenador pedagógico efetivo da rede de ensino, se supervisor selecionado pela secretaria estadual ou distrital;<br>
             III - ser professor de instituição de ensino superior, ou estar cursando mestrado e/ou doutorado na área educacional, se supervisor selecionado pelo Coordenador-Geral da IES;<br>
             IV - possuir titulação de especialização, mestrado ou doutorado; e<br>
             V - ter disponibilidade de 20 horas semanais para dedicar-se à função, podendo ser cedido pela secretaria estadual ou distrital.</p> 
		<p>Parágrafo único. Os requisitos previstos no caput deverão ser documentalmente comprovados pelo(a) supervisor(a) no ato da inscrição na IES responsável pela formação.</p>
		<p><input type=checkbox id=declaro name=declaro onclick=\"declaracaoatribuicoes(this);\" > Declaro ter ciência das minhas atribuições.</p>";
	
	}

	echo "<script>
			jQuery(document).ready(function() {
				jQuery(\"#salvarcontinuar\").css('display','none');
				jQuery(\"#salvar\").css('display','none');
    			if(document.getElementById('td_acao_".AED_ENVIAR_MENSARIO."')) {
				jQuery(\"[id^='td_acao_".AED_ENVIAR_MENSARIO."']\").css('display','none');
    			} else {
    			jQuery(\"#declaro\").attr('disabled', 'disabled');
    			jQuery(\"#declaro\").attr('checked', true);
    			}
			});
		
			function declaracaoatribuicoes(obj) {
				if(obj.checked) {
					jQuery(\"[id^='td_acao_".AED_ENVIAR_MENSARIO."']\").css('display','');
				} else {
					jQuery(\"[id^='td_acao_".AED_ENVIAR_MENSARIO."']\").css('display','none');
				}
			}
		  </script>";

}

function listaAtividadesCadernoFormacao($dados) {
	global $db;

	if(!$dados['iusd']) {
		die("<p align=center>PROBLEMAS PARA CARREGAR A TELA. RECARREGUE A TELA E TENTE NOVAMENTE.</p>");
	}

	$sql = "SELECT '<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirAtividadeCadernoFormacao('||carid||');\">' as acao,
				   '<span style=font-size:x-small;>'||a2.caddsc||'</span>' as caderno,
				   '<span style=font-size:x-small;>'||a.caddsc||'</span>' as atividade,
				   '<span style=font-size:x-small;><input type=\"radio\" name=\"carformarealizacao['||carid||']\" value=\"C\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carformarealizacao\',\'C\')\" '||CASE WHEN carformarealizacao='C' THEN 'checked' ELSE '' END||' > Coletivamente <input type=\"radio\" name=\"carformarealizacao['||carid||']\" value=\"I\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carformarealizacao\',\'I\')\" '||CASE WHEN carformarealizacao='I' THEN 'checked' ELSE '' END||' > Individualmente<div id=dv_carformarealizacao_'||carid||' style='||CASE WHEN carformarealizacao='I' THEN '' ELSE 'display:none;' END||'><br><br>A atividade realizada por você foi apresentada e debatida coletivamente nos encontros da formação?<br><input type=\"radio\" name=\"cardebatidacoletivamente['||carid||']\" value=\"S\" onclick=\"atualizarAtividadeCaderno('||carid||',\'cardebatidacoletivamente\',\'S\')\" '||CASE WHEN cardebatidacoletivamente='S' THEN 'checked' ELSE '' END||' > Sim <input type=\"radio\" name=\"cardebatidacoletivamente['||carid||']\" value=\"N\" onclick=\"atualizarAtividadeCaderno('||carid||',\'cardebatidacoletivamente\',\'N\')\" '||CASE WHEN cardebatidacoletivamente='N' THEN 'checked' ELSE '' END||' > Não</div></span>' as formarealizacao,
				   '<span style=font-size:x-small;><input type=\"radio\" name=\"caravaliacaooe['||carid||']\" value=\"S\" onclick=\"atualizarAtividadeCaderno('||carid||',\'caravaliacaooe\',\'S\')\" '||CASE WHEN caravaliacaooe='S' THEN 'checked' ELSE '' END||'> Sim <input type=\"radio\" name=\"caravaliacaooe['||carid||']\" value=\"N\" onclick=\"atualizarAtividadeCaderno('||carid||',\'caravaliacaooe\',\'N\')\" '||CASE WHEN caravaliacaooe='N' THEN 'checked' ELSE '' END||'> Não</span>' as caravaliacaooe
			FROM sismedio.cadernoatividadesrespostas c
			INNER JOIN sismedio.cadernoatividades a ON a.cadid = c.cadid
			INNER JOIN sismedio.cadernoatividades a2 ON a2.cadid = a.cadidpai
			WHERE c.iusd='".$dados['iusd']."' AND c.fpbid='".$dados['fpbid']."'
			ORDER BY a2.cadid, a.cadid";

	$cabecalho = array("&nbsp;","<span style=font-size:x-small;>Caderno</span>","<span style=font-size:x-small;>Atividade(s) Realizada(s)</span>","<span style=font-size:x-small;>Forma de realização</span>","<span style=font-size:x-small;>Atividade realizada foi avaliada pelo Orientador de Estudo?</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, false);

	echo '<br>';
	echo '<p>Pontue a atividade, considerando os critérios a seguir e a escala de 1 a 5 (Considere: 1 – Muito fraca/ 5 – Muito boa)</p>';

	$sql = "SELECT '<span style=font-size:x-small;>'||a2.caddsc||'</span>' as caderno,
				   '<span style=font-size:x-small;>'||a.caddsc||'</span>' as atividade,
				   '<span style=\"font-size:x-small;white-space: nowrap;\"><input type=\"radio\" name=\"carconteudocadernoformacao['||carid||']\" value=\"1\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carconteudocadernoformacao\',\'1\')\" '||CASE WHEN carconteudocadernoformacao='1' THEN 'checked' ELSE '' END||' > 1 <input type=\"radio\" name=\"carconteudocadernoformacao['||carid||']\" value=\"2\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carconteudocadernoformacao\',\'2\')\" '||CASE WHEN carformarealizacao='2' THEN 'checked' ELSE '' END||' > 2 <input type=\"radio\" name=\"carconteudocadernoformacao['||carid||']\" value=\"3\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carconteudocadernoformacao\',\'3\')\" '||CASE WHEN carconteudocadernoformacao='3' THEN 'checked' ELSE '' END||' > 3 <input type=\"radio\" name=\"carconteudocadernoformacao['||carid||']\" value=\"4\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carconteudocadernoformacao\',\'4\')\" '||CASE WHEN carconteudocadernoformacao='4' THEN 'checked' ELSE '' END||' > 4 <input type=\"radio\" name=\"carconteudocadernoformacao['||carid||']\" value=\"5\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carconteudocadernoformacao\',\'5\')\" '||CASE WHEN carconteudocadernoformacao='5' THEN 'checked' ELSE '' END||' > 5' as carconteudocadernoformacao,
				   '<span style=\"font-size:x-small;white-space: nowrap;\"><input type=\"radio\" name=\"carclarezainstrucoesatividades['||carid||']\" value=\"1\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carclarezainstrucoesatividades\',\'1\')\" '||CASE WHEN carclarezainstrucoesatividades='1' THEN 'checked' ELSE '' END||' > 1 <input type=\"radio\" name=\"carclarezainstrucoesatividades['||carid||']\" value=\"2\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carclarezainstrucoesatividades\',\'2\')\" '||CASE WHEN carclarezainstrucoesatividades='2' THEN 'checked' ELSE '' END||' > 2 <input type=\"radio\" name=\"carclarezainstrucoesatividades['||carid||']\" value=\"3\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carclarezainstrucoesatividades\',\'3\')\" '||CASE WHEN carclarezainstrucoesatividades='3' THEN 'checked' ELSE '' END||' > 3 <input type=\"radio\" name=\"carclarezainstrucoesatividades['||carid||']\" value=\"4\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carclarezainstrucoesatividades\',\'4\')\" '||CASE WHEN carclarezainstrucoesatividades='4' THEN 'checked' ELSE '' END||' > 4 <input type=\"radio\" name=\"carclarezainstrucoesatividades['||carid||']\" value=\"5\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carclarezainstrucoesatividades\',\'5\')\" '||CASE WHEN carclarezainstrucoesatividades='5' THEN 'checked' ELSE '' END||' > 5' as carclarezainstrucoesatividades,
				   '<span style=\"font-size:x-small;white-space: nowrap;\"><input type=\"radio\" name=\"carclarezaobjetivosatividades['||carid||']\" value=\"1\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carclarezaobjetivosatividades\',\'1\')\" '||CASE WHEN carclarezaobjetivosatividades='1' THEN 'checked' ELSE '' END||' > 1 <input type=\"radio\" name=\"carclarezaobjetivosatividades['||carid||']\" value=\"2\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carclarezaobjetivosatividades\',\'2\')\" '||CASE WHEN carclarezaobjetivosatividades='2' THEN 'checked' ELSE '' END||' > 2 <input type=\"radio\" name=\"carclarezaobjetivosatividades['||carid||']\" value=\"3\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carclarezaobjetivosatividades\',\'3\')\" '||CASE WHEN carclarezaobjetivosatividades='3' THEN 'checked' ELSE '' END||' > 3 <input type=\"radio\" name=\"carclarezaobjetivosatividades['||carid||']\" value=\"4\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carclarezaobjetivosatividades\',\'4\')\" '||CASE WHEN carclarezaobjetivosatividades='4' THEN 'checked' ELSE '' END||' > 4 <input type=\"radio\" name=\"carclarezaobjetivosatividades['||carid||']\" value=\"5\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carclarezaobjetivosatividades\',\'5\')\" '||CASE WHEN carclarezaobjetivosatividades='5' THEN 'checked' ELSE '' END||' > 5' as carclarezaobjetivosatividades,
				   '<span style=\"font-size:x-small;white-space: nowrap;\"><input type=\"radio\" name=\"carvinculacaocontexto['||carid||']\" value=\"1\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carvinculacaocontexto\',\'1\')\" '||CASE WHEN carvinculacaocontexto='1' THEN 'checked' ELSE '' END||' > 1 <input type=\"radio\" name=\"carvinculacaocontexto['||carid||']\" value=\"2\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carvinculacaocontexto\',\'2\')\" '||CASE WHEN carvinculacaocontexto='2' THEN 'checked' ELSE '' END||' > 2 <input type=\"radio\" name=\"carvinculacaocontexto['||carid||']\" value=\"3\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carvinculacaocontexto\',\'3\')\" '||CASE WHEN carvinculacaocontexto='3' THEN 'checked' ELSE '' END||' > 3 <input type=\"radio\" name=\"carvinculacaocontexto['||carid||']\" value=\"4\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carvinculacaocontexto\',\'4\')\" '||CASE WHEN carvinculacaocontexto='4' THEN 'checked' ELSE '' END||' > 4 <input type=\"radio\" name=\"carvinculacaocontexto['||carid||']\" value=\"5\" onclick=\"atualizarAtividadeCaderno('||carid||',\'carvinculacaocontexto\',\'5\')\" '||CASE WHEN carvinculacaocontexto='5' THEN 'checked' ELSE '' END||' > 5' as carvinculacaocontexto,
				   '<span style=\"font-size:x-small;white-space: nowrap;\"><input type=\"radio\" name=\"cararticulacaoteoriapratica['||carid||']\" value=\"1\" onclick=\"atualizarAtividadeCaderno('||carid||',\'cararticulacaoteoriapratica\',\'1\')\" '||CASE WHEN cararticulacaoteoriapratica='1' THEN 'checked' ELSE '' END||' > 1 <input type=\"radio\" name=\"cararticulacaoteoriapratica['||carid||']\" value=\"2\" onclick=\"atualizarAtividadeCaderno('||carid||',\'cararticulacaoteoriapratica\',\'2\')\" '||CASE WHEN cararticulacaoteoriapratica='2' THEN 'checked' ELSE '' END||' > 2 <input type=\"radio\" name=\"cararticulacaoteoriapratica['||carid||']\" value=\"3\" onclick=\"atualizarAtividadeCaderno('||carid||',\'cararticulacaoteoriapratica\',\'3\')\" '||CASE WHEN cararticulacaoteoriapratica='3' THEN 'checked' ELSE '' END||' > 3 <input type=\"radio\" name=\"cararticulacaoteoriapratica['||carid||']\" value=\"4\" onclick=\"atualizarAtividadeCaderno('||carid||',\'cararticulacaoteoriapratica\',\'4\')\" '||CASE WHEN cararticulacaoteoriapratica='4' THEN 'checked' ELSE '' END||' > 4 <input type=\"radio\" name=\"cararticulacaoteoriapratica['||carid||']\" value=\"5\" onclick=\"atualizarAtividadeCaderno('||carid||',\'cararticulacaoteoriapratica\',\'5\')\" '||CASE WHEN cararticulacaoteoriapratica='5' THEN 'checked' ELSE '' END||' > 5' as cararticulacaoteoriapratica
			FROM sismedio.cadernoatividadesrespostas c
			INNER JOIN sismedio.cadernoatividades a ON a.cadid = c.cadid
			INNER JOIN sismedio.cadernoatividades a2 ON a2.cadid = a.cadidpai
			WHERE c.iusd='".$dados['iusd']."' AND c.fpbid='".$dados['fpbid']."'
			ORDER BY a2.cadid, a.cadid";

	$cabecalho = array("<span style=font-size:x-small;>Caderno</span>","<span style=font-size:x-small;>Atividade(s) Realizada(s)</span>","<span style=font-size:x-small;>Relação com o conteúdo do Caderno de Formação</span>","<span style=font-size:x-small;>Clareza das instruções para realização da atividade</span>","<span style=font-size:x-small;>Clareza do objetivo da realização da atividade</span>","<span style=font-size:x-small;>Vinculação ao contexto de sala de aula/cotidiano escolar</span>","<span style=font-size:x-small;>Articulação entre teoria e prática</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, false);

	echo '<br>';
	echo '<p>Temas que gostaria que fossem aprofundados em outros cursos.</p>';

	$sql = "SELECT '<span style=font-size:x-small;>'||a2.caddsc||'</span>' as caderno,
					array_to_string(array(SELECT '<span style=\"font-size:x-small;white-space: nowrap;\"><input type=checkbox name=catid[] value=\"'||catid||'\" onclick=\"atualizarAtividadeCadernoTema(this);\" '|| CASE WHEN (SELECT carid FROM sismedio.cadernoatividadesrespostas WHERE iusd='".$dados['iusd']."' AND fpbid='".$dados['fpbid']."' AND catid=t.catid) IS NOT NULL THEN 'checked' ELSE '' END||'> '||catdsc||'</span>' FROM sismedio.cadernoatividadestema t WHERE cadid=a2.cadid), '<br>') as temas
			FROM sismedio.cadernoatividadesrespostas c
			INNER JOIN sismedio.cadernoatividades a ON a.cadid = c.cadid
			INNER JOIN sismedio.cadernoatividades a2 ON a2.cadid = a.cadidpai
			WHERE c.iusd='".$dados['iusd']."' AND c.fpbid='".$dados['fpbid']."'
			GROUP BY a2.cadid, a2.caddsc
			ORDER BY a2.cadid";

	$cabecalho = array("<span style=font-size:x-small;>Caderno</span>","<span style=font-size:x-small;>Temas</span>");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, false);


}


function gravarOEProposAtividadeCadernoFormacao($dados) {
	global $db;


	$carid = $db->pegaUm("SELECT carid FROM sismedio.cadernoatividadesrespostas WHERE caroeproposatividadecadernoformacao IS NOT NULL AND fpbid='".$dados['fpbid']."' AND iusd='".$dados['iusd']."'");

	if($carid) {

		$sql = "UPDATE sismedio.cadernoatividadesrespostas SET caroeproposatividadecadernoformacao='".$dados['caroeproposatividadecadernoformacao']."' WHERE carid={$carid}";

	} else {

		$sql = "INSERT INTO sismedio.cadernoatividadesrespostas(
	            fpbid, iusd, caroeproposatividadecadernoformacao)
	    		VALUES ('".$dados['fpbid']."', '".$dados['iusd']."', '".$dados['caroeproposatividadecadernoformacao']."');";

	}

	$db->executar($sql);
	$db->commit();

}

function carregarAtividadesCaderno($dados) {
	global $db;

	$sql = "SELECT cadid as codigo, caddsc as descricao FROM sismedio.cadernoatividades
			WHERE cadidpai='".$dados['cadidpai']."' AND cadid NOT IN(SELECT cadid FROM sismedio.cadernoatividadesrespostas WHERE iusd='".$dados['iusd']."' AND cadid IS NOT NULL)
			ORDER BY cadid";
	$db->monta_combo('cadid', $sql, 'S', 'Selecione', '', '', '', '600', 'N', 'cadid','');

}

function inserirAtividadeCaderno($dados) {
	global $db;

	if(is_numeric($dados['cadid'])) {
		$carid = $db->pegaUm("SELECT carid FROM sismedio.cadernoatividadesrespostas WHERE fpbid='".$dados['fpbid']."' AND iusd='".$dados['iusd']."' AND cadid='".$dados['cadid']."'");
	}

	if(!$carid) {
		$sql = "INSERT INTO sismedio.cadernoatividadesrespostas(
		            fpbid, iusd, cadid)
		    		VALUES ('".$dados['fpbid']."', '".$dados['iusd']."', '".$dados['cadid']."');";

		$db->executar($sql);
		$db->commit();
	}

}

function atualizarAtividadeCaderno($dados) {
	global $db;

	$sql = "UPDATE sismedio.cadernoatividadesrespostas SET ".$dados['campo']."='".$dados['valor']."' WHERE carid='".$dados['carid']."'";
	$db->executar($sql);
	$db->commit();
}

function excluirAtividadeCaderno($dados) {
	global $db;

	$sql = "DELETE FROM sismedio.cadernoatividadesrespostas WHERE carid='".$dados['carid']."'";
	$db->executar($sql);
	$db->commit();
}


function gravarAtividadeCadernoTema($dados) {
	global $db;

	switch($dados['tipo']) {
		case 'remover':

			$sql = "DELETE FROM sismedio.cadernoatividadesrespostas WHERE fpbid='".$dados['fpbid']."' AND iusd='".$dados['iusd']."' AND catid='".$dados['catid']."'";
			$db->executar($sql);
			$db->commit();

			break;

		case 'inserir':
				
			$carid = $db->pegaUm("SELECT carid FROM sismedio.cadernoatividadesrespostas WHERE fpbid='".$dados['fpbid']."' AND iusd='".$dados['iusd']."' AND catid='".$dados['catid']."'");
				
			if(!$carid) {
					
				$sql = "INSERT INTO sismedio.cadernoatividadesrespostas(
			            fpbid, iusd, catid)
			    		VALUES ('".$dados['fpbid']."', '".$dados['iusd']."', '".$dados['catid']."');";

				$db->executar($sql);
				$db->commit();
					
			}

			break;

	}


}

function carregarComboProfessores($dados) {
	global $db;
	
	$sql = "SELECT i.iusd as codigo, i.iusnome as descricao FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd AND t.pflcod IN('".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORPEDAGOGICO."') 
			WHERE i.iusstatus='A' AND iuscodigoinep IN(SELECT iuscodigoinep FROM sismedio.identificacaousuario WHERE iusd='".$dados['iusd']."')";
	
	$db->monta_combo('iusdnovo', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'iusdnovo','');
	
}

function solicitarTrocaUsuarioPerfil($dados) {
	global $db;
	
	$sql = "INSERT INTO sismedio.solicitacaotroca(
            iusdantigo, iusdnovo, uncid, iusdformadorreg, sotdescricaoformadorreg, 
            sotdtformadorreg, sotdesejacontinuarcursando)
    VALUES ('".$dados['iusdantigo']."', '".$dados['iusdnovo']."', '".$dados['uncid']."', '".$dados['iusdformadorreg']."', '".substr($dados['sotdescricaoformadorreg'],0,300)."', NOW(), ".(($dados['sotdesejacontinuarcursando'])?"TRUE":"FALSE").");";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Solicitação foi gravada com sucesso. Aguarde a autorização do Supervisor e a efetivação do Coordenador Geral","location"=>"sismedio.php?modulo=".$dados['modulo']."&acao=A&aba=".$dados['aba']);
	alertlocation($al);
	

}

function existeSolicitacaoTroca($dados) {
	global $db;
	
	$sql = "SELECT 
		    	reg.iusnome as formadorregional, to_char(sotdtformadorreg,'dd/mm/YYYY HH24:MI') as sotdtformadorreg,
		    	sup.iusnome as supervisories, to_char(sotdtsupervisories,'dd/mm/YYYY HH24:MI') as sotdtsupervisories,
		    	coo.iusnome as coordenadories, to_char(sotdtcoordenadories,'dd/mm/YYYY HH24:MI') as sotdtcoordenadories
		    FROM sismedio.solicitacaotroca s 
		    LEFT JOIN sismedio.identificacaousuario reg ON reg.iusd = s.iusdformadorreg
		    LEFT JOIN sismedio.identificacaousuario sup ON sup.iusd = s.iusdsupervisories 
		    LEFT JOIN sismedio.identificacaousuario coo ON coo.iusd = s.iusdcoordenadories
		    WHERE iusdantigo='".$dados['iusd']."' AND sotstatus='A'";
	$solicitacaotroca = $db->pegaLinha($sql);
	
	if($solicitacaotroca) {
		echo "Existe uma solicitação em andamento...
			FORMADOR REGIONAL : ".$solicitacaotroca['formadorregional']." ( ".$solicitacaotroca['sotdtformadorreg']." )
			SUPERVISOR : ".(($solicitacaotroca['supervisories'])?$solicitacaotroca['supervisories']." ( ".$solicitacaotroca['sotdtsupervisories']." )":"Aguardando homologação")."
    		COORDENADOR GERAL : ".(($solicitacaotroca['coordenadories'])?$solicitacaotroca['coordenadories']." ( ".$solicitacaotroca['sotdtcoordenadories']." )":"Aguardando efetivação")."";
		
	}
}

function exibirDetalhesSubstituicao($dados) {
	global $db;
	
	$sql = "SELECT 	s.sotid, 
    				replace(to_char(i1.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||i1.iusnome||' ( '||p1.pfldsc||' )' as sai,
					replace(to_char(i2.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||i2.iusnome||' ( '||p2.pfldsc||' )' as entra,
    				replace(to_char(ifor.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||ifor.iusnome||' ( '||to_char(sotdtformadorreg,'dd/mm/YYYY HH24:MI')||' )' as formador,
    				replace(to_char(isup.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||isup.iusnome||' ( '||to_char(sotdtsupervisories,'dd/mm/YYYY HH24:MI')||' )' as supervisor,
					CASE WHEN sotdesejacontinuarcursando=true THEN 'Substituído assumirá a vaga do Substituto' ELSE 'Substituído será removido do programa' END as obs,
    				sotdescricaoformadorreg, sotdescricaosupervisories, uni.unisigla||' - '||uni.uninome as universidade
			FROM sismedio.solicitacaotroca s
			INNER JOIN sismedio.identificacaousuario i1 ON i1.iusd = s.iusdantigo
			INNER JOIN sismedio.tipoperfil t1 ON t1.iusd = i1.iusd
			INNER JOIN seguranca.perfil p1 ON p1.pflcod = t1.pflcod
			INNER JOIN sismedio.identificacaousuario i2 ON i2.iusd = s.iusdnovo
			INNER JOIN sismedio.tipoperfil t2 ON t2.iusd = i2.iusd
			INNER JOIN seguranca.perfil p2 ON p2.pflcod = t2.pflcod 
			INNER JOIN sismedio.universidadecadastro unc ON unc.uncid = s.uncid 
			INNER JOIN sismedio.universidade uni ON uni.uniid = unc.uniid 
    		LEFT JOIN sismedio.identificacaousuario ifor ON ifor.iusd = s.iusdformadorreg 
    		LEFT JOIN sismedio.identificacaousuario isup ON isup.iusd = s.iusdsupervisories
			WHERE sotid='".$dados['sotid']."'";
	
	$solicitacaotroca = $db->pegaLinha($sql);
	echo '<form method="post" id="formulario_justificativa" enctype="multipart/form-data">';
	echo '<input type="hidden" name="requisicao" value="atualizarSolicitacaoTroca">';
	echo '<input type="hidden" name="sotid" value="'.$dados['sotid'].'">';
	echo '<input type="hidden" name="situacao" id="situacao" value="">';
	echo '<input type="hidden" name="pflabrev" value="'.$dados['pflabrev'].'">';
	echo '<input type="hidden" name="iusd_homologar" value="'.$dados['iusd_homologar'].'">';
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr><td class="SubTituloDireita" width="30%"><span style="font-size:x-small;">Universidade</td><td><span style="font-size:x-small;">'.$solicitacaotroca['universidade'].'</span></td></tr>';
	echo '<tr><td class="SubTituloDireita" width="30%"><span style="font-size:x-small;">Substituído (Sai)</td><td><span style="font-size:x-small;">'.$solicitacaotroca['sai'].'</span></td></tr>';
	echo '<tr><td class="SubTituloDireita"><span style="font-size:x-small;">Substituto (Entra)</td><td><span style="font-size:x-small;">'.$solicitacaotroca['entra'].'</span></td></tr>';
	echo '<tr><td class="SubTituloDireita"><span style=font-size:x-small;>Detalhamento</td><td><span style=font-size:x-small;>'.$solicitacaotroca['obs'].'</span></td></tr>';
	echo '<tr><td class="SubTituloDireita"><span style=font-size:x-small;>Formador Regional (Solicitou)</span></td><td><span style=font-size:x-small;>'.$solicitacaotroca['formador'].'</span></td></tr>';
	echo '<tr><td class="SubTituloDireita"><span style=font-size:x-small;>Justificativa (Formador Regional)</td><td><span style=font-size:x-small;>'.$solicitacaotroca['sotdescricaoformadorreg'].'</span></td></tr>';
	if($solicitacaotroca['supervisor']) {
		echo '<tr><td class="SubTituloDireita"><span style=font-size:x-small;>Supervisor (Homologou)</span></td><td><span style=font-size:x-small;>'.$solicitacaotroca['supervisor'].'</span></td></tr>';
		echo '<tr><td class="SubTituloDireita"><span style=font-size:x-small;>Justificativa (Supervisor)</td><td><span style=font-size:x-small;>'.$solicitacaotroca['sotdescricaosupervisories'].'</span></td></tr>';
	}
	
	
	echo '<tr><td class="SubTituloDireita"><span style=font-size:x-small;>Seu parecer</td><td>';
	echo campo_textarea( 'sotdescricao', 'S', 'S', '', '70', '4', '5000');
	echo '</td></tr>';
	
	if($dados['pflabrev']=='supervisor') $btn = "Homologar";
	if($dados['pflabrev']=='coordenadories') $btn = "Efetivar";
	
	
	echo '<tr><td class="SubTituloCentro" colspan=2><input type="button" name="aprovar" value="'.$btn.' substituição" onclick="atualizarSolicitacaoTroca('.$solicitacaotroca['sotid'].', \'A\');"> <input type="button" name="recusar" value="Recusar substituição" onclick="atualizarSolicitacaoTroca('.$solicitacaotroca['sotid'].', \'I\');"></td></tr>';
	echo '</table>';
	echo '</form>';

}

function atualizarSolicitacaoTroca($dados) {
	global $db;
	
	if(!$dados['somenteEfetuarTroca']) {

		if($dados['situacao']=='A') $sql = "UPDATE sismedio.solicitacaotroca SET iusd{$dados['pflabrev']}='".$dados['iusd_homologar']."', sotdescricao{$dados['pflabrev']}='".substr($dados['sotdescricao'],0,300)."', sotdt{$dados['pflabrev']}=NOW() WHERE sotid='".$dados['sotid']."'";
		elseif($dados['situacao']=='I') $sql = "UPDATE sismedio.solicitacaotroca SET iusd{$dados['pflabrev']}='".$dados['iusd_homologar']."', sotdescricao{$dados['pflabrev']}='".substr($dados['sotdescricao'],0,300)."', sotdt{$dados['pflabrev']}=NOW(), sotstatus='I' WHERE sotid='".$dados['sotid']."'";
		$db->executar($sql);
		
		$uncid = $db->pegaUm("SELECT uncid FROM sismedio.identificacaousuario WHERE iusd='".$dados['iusd_homologar']."'");
	
	}
	
	
	$sql = "SELECT * FROM sismedio.solicitacaotroca WHERE sotstatus='A' AND iusdcoordenadories IS NOT NULL AND iusdsupervisories IS NOT NULL".(($uncid)?" AND uncid='".$uncid."'":"");
	$solicitacoestrocas = $db->carregar($sql);
	
	if($solicitacoestrocas[0]) {
		foreach($solicitacoestrocas as $solicitacaotroca) {

			$identificacaousuario_novo = $db->pegaLinha("SELECT i.iuscpf, t.pflcod, i.iusd, t.tpeid, i.uncid, tu.turid, ot.otuid, i.iuscodigoinep  
														 FROM sismedio.identificacaousuario i 
														 INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
														 LEFT JOIN sismedio.turmas tu ON tu.iusd = i.iusd 
														 LEFT JOIN sismedio.orientadorturma ot ON ot.iusd = i.iusd 
														 WHERE i.iusd='".$solicitacaotroca['iusdnovo']."'");

			$identificacaousuario_antigo = $db->pegaLinha("SELECT i.iuscpf, t.pflcod, i.iusd, t.tpeid, i.uncid, tu.turid, ot.otuid, d.doeid, i.iuscodigoinep  
														   FROM sismedio.identificacaousuario i 
														   INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
														   LEFT JOIN sismedio.turmas tu ON tu.iusd = i.iusd 
														   LEFT JOIN sismedio.orientadorturma ot ON ot.iusd = i.iusd 
														   LEFT JOIN sismedio.definicaoorientadoresestudo d ON d.iusd = i.iusd  
														   WHERE i.iusd='".$solicitacaotroca['iusdantigo']."'");

			
			$sql = "UPDATE sismedio.usuarioresponsabilidade SET pflcod='".$identificacaousuario_antigo['pflcod']."' WHERE rpustatus='A' AND usucpf='".$identificacaousuario_novo['iuscpf']."' AND pflcod='".$identificacaousuario_novo['pflcod']."'";
			$db->executar($sql);
			
			// tipoperfil
			$sql = "UPDATE sismedio.tipoperfil SET iusd=null WHERE tpeid='".$identificacaousuario_novo['tpeid']."'";
			$db->executar($sql);
			
			$sql = "UPDATE sismedio.tipoperfil SET iusd='".$identificacaousuario_novo['iusd']."' WHERE tpeid='".$identificacaousuario_antigo['tpeid']."'";
			$db->executar($sql);
			//tipoperfil
			
			//turmas
			if($identificacaousuario_novo['turid']) {
				$sql = "UPDATE sismedio.turmas SET iusd=null WHERE turid='".$identificacaousuario_novo['turid']."'";
				$db->executar($sql);
			}
			
			if($identificacaousuario_antigo['turid']) {
				$sql = "UPDATE sismedio.turmas SET iusd='".$identificacaousuario_novo['iusd']."' WHERE turid='".$identificacaousuario_antigo['turid']."'";
				$db->executar($sql);
			}
			//turmas
			
			// orientadorturma
			$sql = "UPDATE sismedio.orientadorturma SET iusd=null WHERE otuid='".$identificacaousuario_novo['otuid']."'";
			$db->executar($sql);
			
			$sql = "UPDATE sismedio.orientadorturma SET iusd='".$identificacaousuario_novo['iusd']."' WHERE otuid='".$identificacaousuario_antigo['otuid']."'";
			$db->executar($sql);
			// orientadorturma
			
			// definicaoorientadoresestudo
			
			$sql = "DELETE FROM sismedio.definicaoorientadoresestudo WHERE iusd='".$identificacaousuario_novo['iusd']."'";
			$db->executar($sql);
			
			$sql = "DELETE FROM sismedio.definicaoorientadoresestudo WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
			$db->executar($sql);

			$sql = "INSERT INTO sismedio.definicaoorientadoresestudo(
				            iusd, doedtinsercao, doecpf, doecodigoinep)
				    VALUES ('".$identificacaousuario_novo['iusd']."', NOW(), '".$_SESSION['usucpf']."', '".$identificacaousuario_antigo['iuscodigoinep']."');";
			
			$db->executar($sql);

			// definicaoorientadoresestudo
			
			$sql = "UPDATE seguranca.perfilusuario SET pflcod='".$identificacaousuario_antigo['pflcod']."' WHERE usucpf='".$identificacaousuario_novo['iuscpf']."' AND pflcod='".$identificacaousuario_novo['pflcod']."'";
			$db->executar($sql);
			
			$sql = "INSERT INTO sismedio.historicotrocausuario(iusdnovo, iusdantigo, pflcod, hstdata, usucpf, uncid)
    				VALUES ('".$identificacaousuario_novo['iusd']."', '".$identificacaousuario_antigo['iusd']."', '".$identificacaousuario_antigo['pflcod']."', NOW(), '".$_SESSION['usucpf']."', ".(($identificacaousuario_antigo['uncid'])?"'".$identificacaousuario_antigo['uncid']."'":"NULL").");";
			$db->executar($sql);
				
			
			if($solicitacaotroca['sotdesejacontinuarcursando']=='t') {

				$sql = "UPDATE sismedio.usuarioresponsabilidade SET pflcod='".$identificacaousuario_novo['pflcod']."' WHERE rpustatus='A' AND usucpf='".$identificacaousuario_antigo['iuscpf']."' AND pflcod='".$identificacaousuario_antigo['pflcod']."'";
				$db->executar($sql);
				
				$sql = "UPDATE sismedio.tipoperfil SET iusd='".$identificacaousuario_antigo['iusd']."' WHERE tpeid='".$identificacaousuario_novo['tpeid']."'";
				$db->executar($sql);
				
				if($identificacaousuario_novo['turid']) {
					$sql = "UPDATE sismedio.turmas SET iusd='".$identificacaousuario_antigo['iusd']."' WHERE turid='".$identificacaousuario_novo['turid']."'";
					$db->executar($sql);
				}
				
				$sql = "UPDATE sismedio.orientadorturma SET iusd='".$identificacaousuario_antigo['iusd']."' WHERE otuid='".$identificacaousuario_novo['otuid']."'";
				$db->executar($sql);
				
				$sql = "UPDATE seguranca.perfilusuario SET pflcod='".$identificacaousuario_novo['pflcod']."' WHERE usucpf='".$identificacaousuario_antigo['iuscpf']."' AND pflcod='".$identificacaousuario_antigo['pflcod']."'";
				$db->executar($sql);
				
				$sql = "INSERT INTO sismedio.historicotrocausuario(iusdnovo, iusdantigo, pflcod, hstdata, usucpf, uncid)
    				VALUES ('".$identificacaousuario_antigo['iusd']."', '".$identificacaousuario_novo['iusd']."', '".$identificacaousuario_novo['pflcod']."', NOW(), '".$_SESSION['usucpf']."', ".(($identificacaousuario_novo['uncid'])?"'".$identificacaousuario_novo['uncid']."'":"NULL").");";
				$db->executar($sql);
				
			
			} else {

				$sql = "UPDATE sismedio.identificacaousuario SET iusstatus='I' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
				$db->executar($sql);

				$sql = "DELETE FROM sismedio.usuarioresponsabilidade WHERE rpustatus='A' AND usucpf='".$identificacaousuario_antigo['iuscpf']."' AND pflcod='".$identificacaousuario_antigo['pflcod']."'";
				$db->executar($sql);
				
				$existe_pagamento = $db->pegaUm("SELECT count(pboid) as num FROM sismedio.pagamentobolsista WHERE tpeid='".$identificacaousuario_novo['tpeid']."'");
				
				if(!$existe_pagamento) {
					$sql = "DELETE FROM sismedio.tipoperfil WHERE tpeid='".$identificacaousuario_novo['tpeid']."'";
					$db->executar($sql);
				}
				
				if($identificacaousuario_novo['turid']) {
					$sql = "DELETE FROM sismedio.turmas WHERE iusd='".$identificacaousuario_novo['turid']."'";
					$db->executar($sql);
				}
				
				if($identificacaousuario_novo['otuid']) {
					$sql = "DELETE FROM sismedio.orientadorturma WHERE otuid='".$identificacaousuario_novo['otuid']."'";
					$db->executar($sql);
				}
				
				$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$identificacaousuario_antigo['iuscpf']."' AND pflcod='".$identificacaousuario_antigo['pflcod']."'";
				$db->executar($sql);

			}

			$db->executar("UPDATE sismedio.solicitacaotroca SET sotstatus='F' WHERE sotid='".$solicitacaotroca['sotid']."'");
			
			gerarVersaoProjetoUniversidade(array('uncid' => $identificacaousuario_antigo['uncid']));

		}
	}
	
	$db->commit();

	
	$al = array("alert"=>"Solicitação foi atualizada com sucesso","location"=>"sismedio.php?modulo=".$dados['modulo']."&acao=A&aba=".$dados['aba']);
	alertlocation($al);
	
	
}

function exibirPeriodosReferenciaUniversidade($dados) {
	global $db;
	
	$sql = "SELECT '<input type=checkbox name=\"rfuid[]\" value=\"'||fu.rfuid||'\" '||CASE WHEN fu.rfustatus='A' THEN '' ELSE 'checked' END||' onclick=\"atualizarFolhaPagamento('||fu.rfuid||', this);\">' as acao, un.unisigla||' - '||un.uninome as universidade, 
					p.pfldsc, 
					fu.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as periodo,
					CASE WHEN fu.rfustatus='A' THEN 'Ativo' ELSE 'Inativo' END as status,
				    '<input type=checkbox onclick=\"inserirTipoAvaliacao('||fu.uncid||','||fu.pflcod||','||f.fpbid||',this);\" '||CASE WHEN (SELECT count(*) FROM sismedio.tipoavaliacaoperfil WHERE uncid=fu.uncid AND pflcod=fu.pflcod AND fpbid=f.fpbid)=0 THEN '' ELSE 'checked' END||'>' as avalia
			FROM sismedio.folhapagamentouniversidade fu 
			INNER JOIN sismedio.folhapagamento f ON f.fpbid = fu.fpbid 
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
			INNER JOIN sismedio.universidadecadastro u ON u.uncid = fu.uncid 
			INNER JOIN sismedio.universidade un ON un.uniid = u.uniid 
			INNER JOIN seguranca.perfil p ON p.pflcod = fu.pflcod 
			WHERE fu.uncid='".$dados['uncid']."' AND fu.pflcod='".$dados['pflcod']."' ORDER BY f.fpbid";
	
	$cabecalho = array("&nbsp;","IES","Perfil","Período","Situação","Bloqueia avaliação");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	
	
}

function atualizarPeriodosReferenciaUniversidade($dados) {
	global $db;
	
	$sql = "UPDATE sismedio.folhapagamentouniversidade SET rfustatus='".$dados['status']."' WHERE rfuid='".$dados['rfuid']."'";
	$db->executar($sql);
	$db->commit();	
}

function retornarEscolaValidada() {
	$perfis = pegaPerfilGeral();
	
	// regra solicitada pela Manuelita/Mirna (24/11/14)
	if(in_array(PFL_SUPERVISORIES,$perfis)) {
		if($_SESSION['sismedio']['supervisories']['estuf']=='MG') return true;
		else return false;
	}
	
	return true;
	
}

function detalharAreaFormacao($dados) {
	global $db;
	
	if($dados['pflcod']) {
		$f_pfl = "and t.pflcod='".$dados['pflcod']."'";
	}
	
	$sql = "select m.estuf, count(i.iusd), round(( (count(i.iusd)*100)::numeric / (SELECT count(*) FROM sismedio.identificacaousuario i inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl} inner join sismedio.identiusucursoformacao ic on ic.iusd = i.iusd where iustermocompromisso=true and ic.cufid='".$dados['cufid']."')::numeric ),2) as porcent
	from sismedio.identificacaousuario i
	inner join sismedio.tipoperfil t on t.iusd = i.iusd {$f_pfl} 
	inner join territorios.municipio m on m.muncod = i.muncodatuacao 
	inner join sismedio.identiusucursoformacao ic on ic.iusd = i.iusd
	where iustermocompromisso=true and ic.cufid='".$dados['cufid']."'
	group by m.estuf
	order by 2 desc";
	
	$cabecalho = array("UF","Total","%");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','N');
	
	
}


function gravarAvaliacaoFinalOE($dados) {
	global $db;
	
	$aoeid = $db->pegaUm("SELECT aoeid FROM sismedio.avaliacaofinaloe WHERE iusd='".$_SESSION['sismedio']['orientadorestudo']['iusd']."'");
	
	if($aoeid) {

		$sql = "UPDATE sismedio.avaliacaofinaloe SET aoeinstalacoes1=".(($dados['aoeinstalacoes1'])?"'".implode(";",$dados['aoeinstalacoes1'])."'":"NULL").", aoeencontropresenciais1=".(($dados['aoeencontropresenciais1'])?"'".$dados['aoeencontropresenciais1']."'":"NULL").", aoeencontropresenciais2=".(($dados['aoeencontropresenciais2'])?"'".$dados['aoeencontropresenciais2']."'":"NULL").", 
            aoeencontropresenciais3=".(($dados['aoeencontropresenciais3'])?"'".$dados['aoeencontropresenciais3']."'":"NULL").", aoeencontropresenciais4=".(($dados['aoeencontropresenciais4'])?"'".$dados['aoeencontropresenciais4']."'":"NULL").", aoecomunicacao1=".(($dados['aoecomunicacao1'])?"'".$dados['aoecomunicacao1']."'":"NULL").", 
            aoecomunicacao2=".(($dados['aoecomunicacao2'])?"'".$dados['aoecomunicacao2']."'":"NULL").", aoecomunicacao3=".(($dados['aoecomunicacao3'])?"'".$dados['aoecomunicacao3']."'":"NULL").", aoecomunicacao4=".(($dados['aoecomunicacao4'])?"'".$dados['aoecomunicacao4']."'":"NULL").", aoecomunicacao5=".(($dados['aoecomunicacao5'])?"'".$dados['aoecomunicacao5']."'":"NULL").", 
            aoecomunicacao6=".(($dados['aoecomunicacao6'])?"'".$dados['aoecomunicacao6']."'":"NULL").", aoecomunicacao7=".(($dados['aoecomunicacao7'])?"'".$dados['aoecomunicacao7']."'":"NULL").", aoecomunicacao8=".(($dados['aoecomunicacao8'])?"'".$dados['aoecomunicacao8']."'":"NULL").", aoecomunicacao9=".(($dados['aoecomunicacao9'])?"'".$dados['aoecomunicacao9']."'":"NULL").", 
            aoecomunicacao10=".(($dados['aoecomunicacao10'])?"'".$dados['aoecomunicacao10']."'":"NULL").", aoecomunicacao11=".(($dados['aoecomunicacao11'])?"'".$dados['aoecomunicacao11']."'":"NULL").", aoeorganizacao1=".(($dados['aoeorganizacao1'])?"'".$dados['aoeorganizacao1']."'":"NULL").", aoeorganizacao2=".(($dados['aoeorganizacao2'])?"'".$dados['aoeorganizacao2']."'":"NULL").", 
            aoeorganizacao3=".(($dados['aoeorganizacao3'])?"'".$dados['aoeorganizacao3']."'":"NULL").", aoeorganizacao4=".(($dados['aoeorganizacao4'])?"'".$dados['aoeorganizacao4']."'":"NULL").", aoeorganizacao5=".(($dados['aoeorganizacao5'])?"'".$dados['aoeorganizacao5']."'":"NULL").", aoeorganizacao6=".(($dados['aoeorganizacao6'])?"'".$dados['aoeorganizacao6']."'":"NULL").", 
            aoeorganizacao7=".(($dados['aoeorganizacao7'])?"'".$dados['aoeorganizacao7']."'":"NULL").", aoeorganizacao8=".(($dados['aoeorganizacao8'])?"'".$dados['aoeorganizacao8']."'":"NULL").", aoeorganizacao9=".(($dados['aoeorganizacao9'])?"'".$dados['aoeorganizacao9']."'":"NULL").", aoedocente1=".(($dados['aoedocente1'])?"'".$dados['aoedocente1']."'":"NULL").", 
            aoeinstalacao1=".(($dados['aoeinstalacao1'])?"'".$dados['aoeinstalacao1']."'":"NULL").", aoesuporte1=".(($dados['aoesuporte1'])?"'".$dados['aoesuporte1']."'":"NULL")." WHERE aoeid={$aoeid}";

	} else {
	
	
		$sql = "INSERT INTO sismedio.avaliacaofinaloe(
	            iusd, aoeinstalacoes1, aoeencontropresenciais1, aoeencontropresenciais2, 
	            aoeencontropresenciais3, aoeencontropresenciais4, aoecomunicacao1, 
	            aoecomunicacao2, aoecomunicacao3, aoecomunicacao4, aoecomunicacao5, 
	            aoecomunicacao6, aoecomunicacao7, aoecomunicacao8, aoecomunicacao9, 
	            aoecomunicacao10, aoecomunicacao11, aoeorganizacao1, aoeorganizacao2, 
	            aoeorganizacao3, aoeorganizacao4, aoeorganizacao5, aoeorganizacao6, 
	            aoeorganizacao7, aoeorganizacao8, aoeorganizacao9, aoedocente1, 
	            aoeinstalacao1, aoesuporte1)
	    VALUES ('".$_SESSION['sismedio']['orientadorestudo']['iusd']."', ".(($dados['aoeinstalacoes1'])?"'".implode(";",$dados['aoeinstalacoes1'])."'":"NULL").", ".(($dados['aoeencontropresenciais1'])?"'".$dados['aoeencontropresenciais1']."'":"NULL").", ".(($dados['aoeencontropresenciais2'])?"'".$dados['aoeencontropresenciais2']."'":"NULL").", 
	            ".(($dados['aoeencontropresenciais3'])?"'".$dados['aoeencontropresenciais3']."'":"NULL").", ".(($dados['aoeencontropresenciais4'])?"'".$dados['aoeencontropresenciais4']."'":"NULL").", ".(($dados['aoecomunicacao1'])?"'".$dados['aoecomunicacao1']."'":"NULL").", 
	            ".(($dados['aoecomunicacao2'])?"'".$dados['aoecomunicacao2']."'":"NULL").", ".(($dados['aoecomunicacao3'])?"'".$dados['aoecomunicacao3']."'":"NULL").", ".(($dados['aoecomunicacao4'])?"'".$dados['aoecomunicacao4']."'":"NULL").", ".(($dados['aoecomunicacao5'])?"'".$dados['aoecomunicacao5']."'":"NULL").", 
	            ".(($dados['aoecomunicacao6'])?"'".$dados['aoecomunicacao6']."'":"NULL").", ".(($dados['aoecomunicacao7'])?"'".$dados['aoecomunicacao7']."'":"NULL").", ".(($dados['aoecomunicacao8'])?"'".$dados['aoecomunicacao8']."'":"NULL").", ".(($dados['aoecomunicacao9'])?"'".$dados['aoecomunicacao9']."'":"NULL").", 
	            ".(($dados['aoecomunicacao10'])?"'".$dados['aoecomunicacao10']."'":"NULL").", ".(($dados['aoecomunicacao11'])?"'".$dados['aoecomunicacao11']."'":"NULL").", ".(($dados['aoeorganizacao1'])?"'".$dados['aoeorganizacao1']."'":"NULL").", ".(($dados['aoeorganizacao2'])?"'".$dados['aoeorganizacao2']."'":"NULL").", 
	            ".(($dados['aoeorganizacao3'])?"'".$dados['aoeorganizacao3']."'":"NULL").", ".(($dados['aoeorganizacao4'])?"'".$dados['aoeorganizacao4']."'":"NULL").", ".(($dados['aoeorganizacao5'])?"'".$dados['aoeorganizacao5']."'":"NULL").", ".(($dados['aoeorganizacao6'])?"'".$dados['aoeorganizacao6']."'":"NULL").", 
	            ".(($dados['aoeorganizacao7'])?"'".$dados['aoeorganizacao7']."'":"NULL").", ".(($dados['aoeorganizacao8'])?"'".$dados['aoeorganizacao8']."'":"NULL").", ".(($dados['aoeorganizacao9'])?"'".$dados['aoeorganizacao9']."'":"NULL").", ".(($dados['aoedocente1'])?"'".$dados['aoedocente1']."'":"NULL").", 
	            ".(($dados['aoeinstalacao1'])?"'".$dados['aoeinstalacao1']."'":"NULL").", ".(($dados['aoesuporte1'])?"'".$dados['aoesuporte1']."'":"NULL").");";
	
	}
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Avaliação final gravada com sucesso","location"=>"sismedio.php?modulo=principal/orientadorestudo/orientadorestudo&acao=A&aba=avaliacaofinal");
	alertlocation($al);
	

}

function ultimoPeriodoReferencia($dados) {
	global $db;
	
	$sql = "SELECT to_char(max((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date),'YYYYmmdd') as data FROM sismedio.folhapagamento f
			INNER JOIN sismedio.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid AND rf.rfustatus='A' 
			WHERE rf.uncid='".$_SESSION['sismedio'][$dados['abapai']]['uncid']."' AND rf.pflcod='".PFL_ORIENTADORESTUDO."'";
	
	$data = $db->pegaUm($sql);
	
	if($data<=date("Ymd")) return true;
	else return false;

}

function estruturaAvaliacaoOE($dados) {
	$es['texto1'] = array('texto' => '<span style=font-size:large><b>1. Espaço e tempo da formação</b></span>',
			'tipo' => 'textual',
	);

	$es['texto7'] = array('texto' => '<b>- Instalações e equipamentos</b>',
			'tipo' => 'textual',
	);

	$es['aoeinstalacoes1'] = array('texto' => 'Selecione as instalações e equipamento utilizados durante os encontros com os professores e coordenadores pedagógicos.',
			'tipo' => 'checkbox',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'Sala de professores da escola'),
					array('valor'=>'2','descricao'=>'Sala de aula na escola '),
					array('valor'=>'3','descricao'=>'Biblioteca da escola'),
					array('valor'=>'4','descricao'=>'Sala de informática da escola'),
					array('valor'=>'5','descricao'=>'Equipamentos de som e imagem'),
					array('valor'=>'6','descricao'=>'Equipamentos de informática'),
					array('valor'=>'7','descricao'=>'Tablet educacional adquirido pela Secretaria de Estado de Educação'),
					array('valor'=>'8','descricao'=>'Outros materiais pedagógicos')
			)
	);

	global $db,$modoRelatorio;
	if(!$modoRelatorio) {
		$menencontroqtd 		 = $db->pegaUm("SELECT sum(menencontroqtd) FROM sismedio.mensario WHERE iusd='".$_SESSION['sismedio']['orientadorestudo']['iusd']."'");
		$menencontrocargahoraria = $db->pegaUm("SELECT sum(menencontrocargahoraria) FROM sismedio.mensario WHERE iusd='".$_SESSION['sismedio']['orientadorestudo']['iusd']."'");
	} else {
		$menencontroqtd 		 = $db->pegaUm("SELECT round(sum(menencontroqtd)/count(distinct m.iusd),1) FROM sismedio.mensario m INNER JOIN sismedio.avaliacaofinaloe a ON a.iusd = m.iusd");
		$menencontrocargahoraria = $db->pegaUm("SELECT  round(sum(menencontrocargahoraria)/count(distinct m.iusd),1) FROM sismedio.mensario m INNER JOIN sismedio.avaliacaofinaloe a ON a.iusd = m.iusd");
	}

	$es['texto8'] = array('texto' => '<b>- Encontros Presenciais</b> ( Número de encontros presenciais realizados na escola : <b>'.(($menencontroqtd)?$menencontroqtd:'0').'</b> / Carga horária total : <b>'.(($menencontrocargahoraria)?$menencontrocargahoraria:'0').'</b> )',
			'tipo' => 'textual',
	);

	$es['aoeencontropresenciais1'] = array('texto' => 'Periodicidade de realização da MAIORIA dos encontros presenciais na escola.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Semanal (uma vez por semana)'),
					array('valor'=>'2','descricao'=>'Quinzenal (a cada quinze dias)'),
					array('valor'=>'3','descricao'=>'Mensal (uma vez ao mês)')
			)
	);

	$es['aoeencontropresenciais2'] = array('texto' => 'A MAIORIA dos encontros presenciais realizados na escola ocorreu',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'Durante a semana.'),
					array('valor'=>'2','descricao'=>'No fim de semana.')
			)
	);

	$es['aoeencontropresenciais3'] = array('texto' => 'A MAIORIA dos encontros presenciais realizados na escola ocorreu no turno',
			'tipo' => 'radio',
			'opcoes' => array(
					array('valor'=>'1','descricao'=>'Matutino'),
					array('valor'=>'2','descricao'=>'Vespertino'),
					array('valor'=>'3','descricao'=>'Noturno')
			)
	);

	$es['aoeencontropresenciais4'] = array('texto' => 'Em sua escola, os encontros de formação foram realizados no período da hora-atividade dos docentes, reservado a atividades extraclasse?',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Sim'),
					array('valor'=>'2','descricao'=>'Não'),
					array('valor'=>'3','descricao'=>'Parcialmente')
			)
	);

	$es['texto2'] = array('texto' => '<span style=font-size:large><b>2. Questionário</b></span>
									  <p>O questionário a seguir tem o objetivo de colher informações acerca de diferentes aspectos da formação do Pacto Nacional pelo Fortalecimento do Ensino Médio, visando ao contínuo aperfeiçoamento dos programas de formação continuada de profissionais da Educação Básica do Ministério da Educação.</p>
									  <p>Procure responder da forma mais precisa possível às questões que se seguem. A escala de resposta apresenta três opções: a) Atendeu plenamente, b) Atendeu parcialmente e c) Não atendeu. Leia atentamente os itens listados e avalie conforme as opções. Por favor, não deixe questões em branco.</p>',
			'tipo' => 'textual',
	);

	$es['texto3'] = array('texto' => '<b>- Quanto à comunicação ANTES do início da formação</b>',
			'tipo' => 'textual',
	);


	$es['aoecomunicacao1'] = array('texto' => '1. Sobre o público-alvo do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao2'] = array('texto' => '2. Sobre os objetivos da formação do Pacto Nacional pelo Ensino Médio.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao3'] = array('texto' => '3. Sobre as atividades formativas previstas.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao4'] = array('texto' => '4. Sobre a carga horária do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao5'] = array('texto' => '5. Sobre o tempo de duração do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao6'] = array('texto' => '6. Sobre os pré-requisitos estabelecidos para a função. ',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao7'] = array('texto' => '7. Sobre as atribuições e responsabilidades do orientador de estudo.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao8'] = array('texto' => '8. Sobre o processo de seleção/participação.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao9'] = array('texto' => '9. Sobre os direitos e deveres dos bolsistas.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao10'] = array('texto' => '10.	Sobre o recebimento de certificado de conclusão do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao11'] = array('texto' => '11. Sobre dispositivo legal e normativo do Pacto Nacional pelo Fortalecimento do Ensino Médio.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['texto9'] = array('texto' => '<b>- Organização didático-pedagógica</b>',
			'tipo' => 'textual',
	);

	$es['aoeorganizacao1'] = array('texto' => '12. Carga horária dos encontros (distribuição/volume dos conteúdos apresentados em relação à carga horária proposta).',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao2'] = array('texto' => '13. Adequação da metodologia de ensino à concepção do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao3'] = array('texto' => '14. Adequação dos conteúdos trabalhados aos objetivos da formação.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao4'] = array('texto' => '15. Coerência dos procedimentos de ensino e aprendizagem com a concepção do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao5'] = array('texto' => '16. Distribuição dos conteúdos durante o processo formativo de maneira a garantir o nexo sequencial ao curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao6'] = array('texto' => '17. Interação entre teoria e prática ao longo do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao7'] = array('texto' => '18. Adequação das atividades práticas de formação às suas necessidades de atuação como Orientador de Estudo.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao8'] = array('texto' => '19. Adequação do conteúdo teórico às suas expectativas e necessidades de formação.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao9'] = array('texto' => '20. Mecanismos efetivos de planejamento e acompanhamento de seu trabalho com os professores e coordenadores pedagógicos.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoedocente1'] = array('texto' => '21. Formador Regional com formação e experiência adequadas às unidades de estudo e atividades desenvolvidas no curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['texto5'] = array('texto' => '<b>- Instalações físicas</b>',
			'tipo' => 'textual',
	);

	$es['aoeinstalacao1'] = array('texto' => '22. Adequação das instalações (estrutura física: salas de aula, capacidade, conservação, acústica, acessibilidade, limpeza, iluminação, ventilação, mobiliário adequado etc.) e dos equipamentos (de som e imagem, de informática etc.) oferecidos pela Secretaria de Educação para os encontros de formação.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['texto6'] = array('texto' => '<b>- Suporte ao desenvolvimento da formação</b>',
			'tipo' => 'textual',
	);

	$es['aoesuporte1'] = array('texto' => '23. Ações desenvolvidas no âmbito da Secretaria de Educação para garantir sua participação nos eventos da formação.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);



	return $es;
}

function gravarEncontroPresencial($dados) {
	global $db;
	
	$sql = "INSERT INTO sismedio.avaliacaofinalfrencontropresencial(
            iusd, aofnome, aofdata, aofcargahoraria)
    		VALUES ('".$_SESSION['sismedio']['formadorregional']['iusd']."', '".utf8_decode($dados['aofnome'])."', '".formata_data_sql($dados['aofdata'])."', '".$dados['aofcargahoraria']."');";
	
	$db->executar($sql);
	$db->commit();
}

function gravarEncontroPresencialCG($dados) {
	global $db;

	$sql = "INSERT INTO sismedio.avaliacaofinalcgencontropresencial(
            iusd, aofnome, aofdata, aofcargahoraria)
    		VALUES ('".$_SESSION['sismedio']['universidade']['iusd']."', '".utf8_decode($dados['aofnome'])."', '".formata_data_sql($dados['aofdata'])."', '".$dados['aofcargahoraria']."');";

	$db->executar($sql);
	$db->commit();
}

function carregarEncontroPresencialFormadorRegional($dados) {
	global $db;
	$sql = "SELECT '<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"removerEncontroPresencial('||aofid||');\">' as acao, aofnome, to_char(aofdata,'dd/mm/YYYY') as aofdata, aofcargahoraria FROM sismedio.avaliacaofinalfrencontropresencial WHERE iusd='".$_SESSION['sismedio']['formadorregional']['iusd']."'";
	$cabecalho = array("&nbsp;","Nome do evento","Data do evento","Carga horária");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}

function carregarEncontroPresencialCoordenadorIES($dados) {
	global $db;
	
	if($_SESSION['sismedio']['universidade']['iusd']) {

		$sql = "SELECT '".(($dados['consulta'])?"":"<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"removerEncontroPresencial('||aofid||');\">")."' as acao, aofnome, to_char(aofdata,'dd/mm/YYYY') as aofdata, aofcargahoraria FROM sismedio.avaliacaofinalcgencontropresencial WHERE iusd='".$_SESSION['sismedio']['universidade']['iusd']."'";
		$cabecalho = array("&nbsp;","Nome do evento","Data do evento","Carga horária");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	
	} else {
		echo '<p>PROBLEMAS PARA CARREGAR OS ENCONTROS PRESENCIAIS. TENTE NOVAMENTE.</p>';
	}
}

function carregarRegionaisCoordenadorIES($dados) {
	global $db;
	
	$sql = "SELECT a.afrid, a.afrnome, m.estuf||'/'||m.mundescricao as municipiosede, a.afrnumformadoresreg FROM sismedio.avaliacaofinalcgregional a 
	        INNER JOIN territorios.municipio m ON m.muncod = a.afrmuncodsede 
	        WHERE iusd='".$_SESSION['sismedio']['universidade']['iusd']."'";
	
	$regionais = $db->carregar($sql);
	
	if($regionais[0]) {
		foreach($regionais as $key => $reg) {
			if($dados['consulta']) $arr[$key]['acao']          = "";
			else $arr[$key]['acao']          = "<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"removerRegional('".$reg['afrid']."');\">";
			$arr[$key]['afrnome']       = $reg['afrnome'];
			$arr[$key]['municipiosede'] = $reg['municipiosede'];
			
			$abrangencia = $db->carregarColuna("SELECT '<span style=font-size:x-small;>'||m.estuf||'/'||m.mundescricao||'</span>' as abr FROM sismedio.avaliacaofinalcgregionalabrangencia a 
												INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
												WHERE a.afrid='".$reg['afrid']."'");
			
			$arr[$key]['abr']           = (($abrangencia)?'<div style=height:120px;overflow:auto;>'.implode("<br>",$abrangencia).'</div>':'Não possui');
			$arr[$key]['afrnumformadoresreg'] = $reg['afrnumformadoresreg'];
			
		}
	}
	
	if(!$arr) $arr = array();
	
	$cabecalho = array("&nbsp;","Identificação da Regional (Nome)","Munícipio sede","Área de Abrangência(Municípios)","Número de turmas de Formadores Regionais");
	$db->monta_lista_simples($arr,$cabecalho,50,5,'N','100%',$par2);
	
}

function removerEncontroPresencialFormadorRegional($dados) {
	global $db;
	
	$sql = "DELETE FROM sismedio.avaliacaofinalfrencontropresencial WHERE aofid='".$dados['aofid']."'";
	$db->executar($sql);
	$db->commit();

}

function removerRegionalCoordenadorIES($dados) {
	global $db;
	
	$sql = "DELETE FROM sismedio.avaliacaofinalcgregionalabrangencia WHERE afrid='".$dados['afrid']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM sismedio.avaliacaofinalcgregional WHERE afrid='".$dados['afrid']."'";
	$db->executar($sql);
	$db->commit();

}

function removerEncontroPresencialCoordenadorIES($dados) {
	global $db;

	$sql = "DELETE FROM sismedio.avaliacaofinalcgencontropresencial WHERE aofid='".$dados['aofid']."'";
	$db->executar($sql);
	$db->commit();

}

function gravarAvaliacaoFinalFR($dados) {
	global $db;

	$aoeid = $db->pegaUm("SELECT aoeid FROM sismedio.avaliacaofinalfr WHERE iusd='".$_SESSION['sismedio']['formadorregional']['iusd']."'");

	if($aoeid) {

		$sql = "UPDATE sismedio.avaliacaofinalfr SET
    												aoeinstalacoes1=".(($dados['aoeinstalacoes1'])?"'".$dados['aoeinstalacoes1']."'":"NULL").", 
													aoecomunicacao1=".(($dados['aoecomunicacao1'])?"'".$dados['aoecomunicacao1']."'":"NULL").", 
			aoecomunicacao2=".(($dados['aoecomunicacao2'])?"'".$dados['aoecomunicacao2']."'":"NULL").", 
            aoecomunicacao3=".(($dados['aoecomunicacao3'])?"'".$dados['aoecomunicacao3']."'":"NULL").", 
			aoecomunicacao4=".(($dados['aoecomunicacao4'])?"'".$dados['aoecomunicacao4']."'":"NULL").", 
			aoecomunicacao5=".(($dados['aoecomunicacao5'])?"'".$dados['aoecomunicacao5']."'":"NULL").", 
			aoecomunicacao6=".(($dados['aoecomunicacao6'])?"'".$dados['aoecomunicacao6']."'":"NULL").", 
            aoecomunicacao7=".(($dados['aoecomunicacao7'])?"'".$dados['aoecomunicacao7']."'":"NULL").", 
			aoecomunicacao8=".(($dados['aoecomunicacao8'])?"'".$dados['aoecomunicacao8']."'":"NULL").", 
	        aoecomunicacao9=".(($dados['aoecomunicacao9'])?"'".$dados['aoecomunicacao9']."'":"NULL").", 
			aoecomunicacao10=".(($dados['aoecomunicacao10'])?"'".$dados['aoecomunicacao10']."'":"NULL").", 
            aoecomunicacao11=".(($dados['aoecomunicacao11'])?"'".$dados['aoecomunicacao11']."'":"NULL").", 
			aoeorganizacao1=".(($dados['aoeorganizacao1'])?"'".$dados['aoeorganizacao1']."'":"NULL").", 
			aoeorganizacao2=".(($dados['aoeorganizacao2'])?"'".$dados['aoeorganizacao2']."'":"NULL").", 
			aoeorganizacao3=".(($dados['aoeorganizacao3'])?"'".$dados['aoeorganizacao3']."'":"NULL").", 
            aoeorganizacao4=".(($dados['aoeorganizacao4'])?"'".$dados['aoeorganizacao4']."'":"NULL").", 
			aoeorganizacao5=".(($dados['aoeorganizacao5'])?"'".$dados['aoeorganizacao5']."'":"NULL").", 
			aoeorganizacao6=".(($dados['aoeorganizacao6'])?"'".$dados['aoeorganizacao6']."'":"NULL").", 
			aoeorganizacao7=".(($dados['aoeorganizacao7'])?"'".$dados['aoeorganizacao7']."'":"NULL").", 
            aoeorganizacao8=".(($dados['aoeorganizacao8'])?"'".$dados['aoeorganizacao8']."'":"NULL").", 
			aoeorganizacao9=".(($dados['aoeorganizacao9'])?"'".$dados['aoeorganizacao9']."'":"NULL").", 
			aoedocente1=".(($dados['aoedocente1'])?"'".$dados['aoedocente1']."'":"NULL").", 
			aoedocente2=".(($dados['aoedocente2'])?"'".$dados['aoedocente2']."'":"NULL").", 
			aoeinstalacao1=".(($dados['aoeinstalacao1'])?"'".$dados['aoeinstalacao1']."'":"NULL").", 
            aoeinstalacao2=".(($dados['aoeinstalacao2'])?"'".$dados['aoeinstalacao2']."'":"NULL").", 
			aoesuporte1=".(($dados['aoesuporte1'])?"'".$dados['aoesuporte1']."'":"NULL").", 
			aoesuporte2=".(($dados['aoesuporte2'])?"'".$dados['aoesuporte2']."'":"NULL").", 
			aoesuporte3=".(($dados['aoesuporte3'])?"'".$dados['aoesuporte3']."'":"NULL").", 
			aoesuporte4=".(($dados['aoesuporte4'])?"'".$dados['aoesuporte4']."'":"NULL").", 
            aoearticulacao1=".(($dados['aoearticulacao1'])?"'".$dados['aoearticulacao1']."'":"NULL").", 
			aoeconsideracoes=".(($dados['tx_aoeconsideracoes'])?"'".$dados['tx_aoeconsideracoes']."'":"NULL")." WHERE aoeid={$aoeid}";

	} else {


		$sql = "INSERT INTO sismedio.avaliacaofinalfr(
            iusd, aoeinstalacoes1, aoecomunicacao1, aoecomunicacao2, 
            aoecomunicacao3, aoecomunicacao4, aoecomunicacao5, aoecomunicacao6, 
            aoecomunicacao7, aoecomunicacao8, aoecomunicacao9, aoecomunicacao10, 
            aoecomunicacao11, aoeorganizacao1, aoeorganizacao2, aoeorganizacao3, 
            aoeorganizacao4, aoeorganizacao5, aoeorganizacao6, aoeorganizacao7, 
            aoeorganizacao8, aoeorganizacao9, aoedocente1, aoedocente2, aoeinstalacao1, 
            aoeinstalacao2, aoesuporte1, aoesuporte2, aoesuporte3, aoesuporte4, 
            aoearticulacao1, aoeconsideracoes)
    VALUES ('".$_SESSION['sismedio']['formadorregional']['iusd']."', ".(($dados['aoeinstalacoes1'])?"'".$dados['aoeinstalacoes1']."'":"NULL").", ".(($dados['aoecomunicacao1'])?"'".$dados['aoecomunicacao1']."'":"NULL").", ".(($dados['aoecomunicacao2'])?"'".$dados['aoecomunicacao2']."'":"NULL").", 
            ".(($dados['aoecomunicacao3'])?"'".$dados['aoecomunicacao3']."'":"NULL").", ".(($dados['aoecomunicacao4'])?"'".$dados['aoecomunicacao4']."'":"NULL").", ".(($dados['aoecomunicacao5'])?"'".$dados['aoecomunicacao5']."'":"NULL").", ".(($dados['aoecomunicacao6'])?"'".$dados['aoecomunicacao6']."'":"NULL").", 
            ".(($dados['aoecomunicacao7'])?"'".$dados['aoecomunicacao7']."'":"NULL").", ".(($dados['aoecomunicacao8'])?"'".$dados['aoecomunicacao8']."'":"NULL").", ".(($dados['aoecomunicacao9'])?"'".$dados['aoecomunicacao9']."'":"NULL").", ".(($dados['aoecomunicacao10'])?"'".$dados['aoecomunicacao10']."'":"NULL").", 
            ".(($dados['aoecomunicacao11'])?"'".$dados['aoecomunicacao11']."'":"NULL").", ".(($dados['aoeorganizacao1'])?"'".$dados['aoeorganizacao1']."'":"NULL").", ".(($dados['aoeorganizacao2'])?"'".$dados['aoeorganizacao2']."'":"NULL").", ".(($dados['aoeorganizacao3'])?"'".$dados['aoeorganizacao3']."'":"NULL").", 
            ".(($dados['aoeorganizacao4'])?"'".$dados['aoeorganizacao4']."'":"NULL").", ".(($dados['aoeorganizacao5'])?"'".$dados['aoeorganizacao5']."'":"NULL").", ".(($dados['aoeorganizacao6'])?"'".$dados['aoeorganizacao6']."'":"NULL").", ".(($dados['aoeorganizacao7'])?"'".$dados['aoeorganizacao7']."'":"NULL").", 
            ".(($dados['aoeorganizacao8'])?"'".$dados['aoeorganizacao8']."'":"NULL").", ".(($dados['aoeorganizacao9'])?"'".$dados['aoeorganizacao9']."'":"NULL").", ".(($dados['aoedocente1'])?"'".$dados['aoedocente1']."'":"NULL").", ".(($dados['aoedocente2'])?"'".$dados['aoedocente2']."'":"NULL").", ".(($dados['aoeinstalacao1'])?"'".$dados['aoeinstalacao1']."'":"NULL").", 
            ".(($dados['aoeinstalacao2'])?"'".$dados['aoeinstalacao2']."'":"NULL").", ".(($dados['aoesuporte1'])?"'".$dados['aoesuporte1']."'":"NULL").", ".(($dados['aoesuporte2'])?"'".$dados['aoesuporte2']."'":"NULL").", ".(($dados['aoesuporte3'])?"'".$dados['aoesuporte3']."'":"NULL").", ".(($dados['aoesuporte4'])?"'".$dados['aoesuporte4']."'":"NULL").", 
            ".(($dados['aoearticulacao1'])?"'".$dados['aoearticulacao1']."'":"NULL").", ".(($dados['tx_aoeconsideracoes'])?"'".$dados['tx_aoeconsideracoes']."'":"NULL").");";

	}

	$db->executar($sql);
	$db->commit();

	$al = array("alert"=>"Avaliação final gravada com sucesso","location"=>"sismedio.php?modulo=principal/formadorregional/formadorregional&acao=A&aba=avaliacaofinal");
	alertlocation($al);


}

function estruturaAvaliacaoFR($dados) {

	$es['texto0'] = array('texto' => '<span style=font-size:large><b>1. Organização do curso ministrado aos Orientadores de Estudo</b></span>',
			'tipo' => 'textual',
	);

	global $db, $modoRelatorio;
	
	if(!$modoRelatorio) {

		ob_start();
		echo "<script>
				function gravarEncontroPresencial() {
					if(jQuery('#aofnome').val()==''){alert('Preencha o nome do evento');return false;}
					if(jQuery('#aofdata').val()==''){alert('Preencha a data do evento');return false;}
					if(jQuery('#aofcargahoraria').val()==''){alert('Preencha a carga horária');return false;}
					ajaxatualizar('requisicao=gravarEncontroPresencial&aofnome='+jQuery('#aofnome').val()+'&aofdata='+jQuery('#aofdata').val()+'&aofcargahoraria='+jQuery('#aofcargahoraria').val(),'');
					ajaxatualizar('requisicao=carregarEncontroPresencialFormadorRegional','td_encontropresencialformadorregional');
 					jQuery('#aofnome').val('');
 					jQuery('#aofdata').val('');
 					jQuery('#aofcargahoraria').val('');
				}
			
				function removerEncontroPresencial(aofid) {
					var conf = confirm('Deseja realmente remover o encontro presencial?');
					if(conf) {
						ajaxatualizar('requisicao=removerEncontroPresencialFormadorRegional&aofid='+aofid,'');
						ajaxatualizar('requisicao=carregarEncontroPresencialFormadorRegional','td_encontropresencialformadorregional');
					}
				}
			  </script>";
	
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
		echo '<tr><td class="SubTituloDireita">Nome do evento:</td><td>'.campo_texto('aofnome', "N", "S", "Nome do evento", 30, 60, "", "", '', '', 0, 'id="aofnome"').'</td><td class="SubTituloDireita">Data do evento:</td><td>'.campo_data2('aofdata','S', 'S', 'Data do evento', 'S', '', '', '', '', '', 'aofdata').'</td><td class="SubTituloDireita">Carga horária:</td><td>'.campo_texto('aofcargahoraria', "N", "S", "Carga horária", 6, 10, "######", "", '', '', 0, 'id="aofcargahoraria"').'</td></tr>';
		echo '<tr><td class="SubTitulocentro" colspan="6"><input type="button" name="gravar" value="Gravar" onclick="gravarEncontroPresencial();"></td></tr>';
		echo '<tr><td colspan="6" id="td_encontropresencialformadorregional">';
	
		carregarEncontroPresencialFormadorRegional(array());
	
		echo '</td></tr>';
		echo '</table>';
	
		$dadosserv = ob_get_contents();
		ob_clean();

	} else {

		$sql = "SELECT round(AVG(foo.tota),2) as mediaeventos, round(AVG(foo.ch),2) as mediacargahoraria FROM (
				SELECT iusd, count(aofid) as tota, sum(aofcargahoraria) as ch FROM sismedio.avaliacaofinalfrencontropresencial GROUP BY iusd
				) foo ";
		
		$arr = $db->pegaLinha($sql);
		
		ob_start();
		
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
		echo '<tr><td class="SubTituloDireita">Número médio de eventos:</td><td>'.$arr['mediaeventos'].'</td><td class="SubTituloDireita">Média da carga horária:</td><td>'.$arr['mediacargahoraria'].'</td></tr>';
		echo '</table>';
		
		$dadosserv = ob_get_contents();
		ob_clean();
		
		
	}
	
	$es['texto1'] = array('texto' => $dadosserv,
			'tipo' => 'textual',
	);

	$es['texto2'] = array('texto' => '<span style=font-size:large><b>2. Questionário</b></span>
									  <p>O questionário a seguir tem o objetivo de colher informações acerca de diferentes aspectos da formação do Pacto Nacional pelo Fortalecimento do Ensino Médio, visando ao contínuo aperfeiçoamento dos programas de formação continuada de profissionais da Educação Básica do Ministério da Educação.</p>
<p>Procure responder da forma mais precisa possível às questões que se seguem. A escala de resposta apresenta três opções: a) Atendeu plenamente; b) Atendeu parcialmente; e c) Não atendeu. Leia atentamente os itens listados e avalie conforme as opções. Por favor, não deixe questões em branco.</p>',
			'tipo' => 'textual',
	);

	$es['texto3'] = array('texto' => '<b>- Quanto à comunicação ANTES do início da formação</b>',
			'tipo' => 'textual',
	);


	$es['aoecomunicacao1'] = array('texto' => '1. Sobre o público-alvo do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao2'] = array('texto' => '2. Sobre os objetivos da formação do Pacto Nacional pelo Fortalecimentos do Ensino Médio.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao3'] = array('texto' => '3. Sobre as atividades formativas previstas.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao4'] = array('texto' => '4. Sobre a carga horária do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao5'] = array('texto' => '5. Sobre o tempo de duração do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao6'] = array('texto' => '6. Sobre os pré-requisitos estabelecidos para a função. ',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao7'] = array('texto' => '7. Sobre as atribuições e responsabilidades do formador regional.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao8'] = array('texto' => '8. Sobre seu processo de seleção/participação.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao9'] = array('texto' => '9. Sobre os direitos e deveres dos bolsistas.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao10'] = array('texto' => '10.	Sobre o recebimento de certificado de conclusão do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoecomunicacao11'] = array('texto' => '11. Sobre dispositivo legal e normativo do Pacto Nacional pelo Fortalecimento do Ensino Médio (Portaria nº 1.140, de 22 de novembro de 2013; Resolução/CD/FNDE nº 51, de 11 de dezembro de 2013).',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['texto9'] = array('texto' => '<b>- Organização didático-pedagógica</b>',
			'tipo' => 'textual',
	);

	$es['aoeorganizacao1'] = array('texto' => '12. Carga horária dos encontros (distribuição/volume dos conteúdos apresentados em relação à carga horária proposta).',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao2'] = array('texto' => '13. Adequação da metodologia de ensino à concepção do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao3'] = array('texto' => '14. Adequação dos conteúdos trabalhados aos objetivos da formação.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao4'] = array('texto' => '15. Coerência dos procedimentos de ensino e aprendizagem com a concepção do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao5'] = array('texto' => '16. Distribuição dos conteúdos durante o processo formativo de maneira a garantir o nexo sequencial ao curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao6'] = array('texto' => '17. Interação entre teoria e prática ao longo do curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao7'] = array('texto' => '18. Adequação das atividades práticas de formação às suas necessidades de atuação como Formador Regional.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao8'] = array('texto' => '19. Adequação do conteúdo teórico às suas expectativas e necessidades de formação.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeorganizacao9'] = array('texto' => '20. Mecanismos efetivos de planejamento e acompanhamento de seu trabalho com os orientadores de estudo.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoedocente1'] = array('texto' => '21. Participação efetiva dos formadores da IES no desenvolvimento do curso (discussão de estratégias metodológicas, planejamento das ações formativas, definição de procedimentos de acompanhamento da prática pedagógica e de avaliação dos orientadores de estudo).',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoedocente2'] = array('texto' => '22. Formadores da IES com formação e experiência adequadas às unidades de estudo e atividades desenvolvidas no curso.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['texto5'] = array('texto' => '<b>- Instalações físicas</b>',
			'tipo' => 'textual',
	);

	$es['aoeinstalacao1'] = array('texto' => '23. Adequação das instalações (estrutura física: salas de aula, capacidade, conservação, acústica, acessibilidade, limpeza, iluminação, ventilação, mobiliário adequado etc.) e dos equipamentos (de som e imagem, de informática etc.) oferecidos pela Secretaria de Educação para os encontros de formação dos orientadores de estudo.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeinstalacao2'] = array('texto' => '24. Adequação das instalações (estrutura física: salas de aula, capacidade, conservação, acústica, acessibilidade, limpeza, iluminação, ventilação, mobiliário adequado etc.) e dos equipamentos (de som e imagem, de informática etc.) utilizados durante os encontros presenciais na IES.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['texto6'] = array('texto' => '<b>- Suporte ao desenvolvimento da formação</b>',
			'tipo' => 'textual',
	);

	$es['aoesuporte1'] = array('texto' => '25. Apoio da IES ao acompanhamento da prática pedagógica dos orientadores de estudo nas escolas.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoesuporte2'] = array('texto' => '26. Organização no âmbito acadêmico da IES para conhecer e solucionar problemas inerentes à execução da formação.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoesuporte3'] = array('texto' => '27. Ações desenvolvidas no âmbito da Secretaria de Educação para garantir a participação dos orientadores de estudo nas atividades e nos eventos da formação.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoesuporte4'] = array('texto' => '28. Periodicidade das reuniões com a equipe da Secretaria de Educação para acompanhamento da formação nas escolas, análise de avanços e dificuldades, definição e implantação de medidas corretivas necessárias.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['texto7'] = array('texto' => '<b>- Articulação institucional</b>',
			'tipo' => 'textual',
	);

	$es['aoearticulacao1'] = array('texto' => '29. Funcionamento de instância(s) coletiva(s) de deliberação e discussão de questões inerentes ao desenvolvimento da formação.',
			'tipo' => 'radio',
			'opcoes' => array(array('valor'=>'1','descricao'=>'Atendeu plenamente'),
					array('valor'=>'2','descricao'=>'Atendeu parcialmente'),
					array('valor'=>'3','descricao'=>'Não atendeu')
			)
	);

	$es['aoeconsideracoes'] = array('texto' => 'Se desejar, acrescente um comentário aos tópicos acima (até 200 caracteres).',
			'text' => array('maxsize'=>'200','rows'=>'4','cols'=>'40')
	);


	return $es;
}


function estruturaAvaliacaoCG($dados) {
	global $db, $modoRelatorio;
	
	ob_start();
	
	echo "<script>
																							   		 		
				function gravarRegional() {
					if(jQuery('#afrnome').val()==''){alert('Preencha Identificação da Regional ');return false;}
					if(jQuery('#afrmuncodsede').val()==''){alert('Selecione Munícipio sede');return false;}
					if(jQuery('#afrnumformadoresreg').val()==''){alert('Preencha número de turmas de Formadores Regionais');return false;}
																							   		 		
   		 			var muncodabr  = document.getElementById( 'muncodabr' );
					selectAllOptions( document.getElementById('muncodabr') );
																							   		 		
					ajaxatualizar('requisicao=gravarRegionalCG&afrnome='+jQuery('#afrnome').val()+'&afrmuncodsede='+jQuery('#afrmuncodsede').val()+'&afrnumformadoresreg='+jQuery('#afrnumformadoresreg').val()+'&'+jQuery('#muncodabr').serialize(),'');
					ajaxatualizar('requisicao=carregarRegionaisCoordenadorIES','td_regionais');
				}
																							   		 		
				function gravarEncontroPresencial() {
					if(jQuery('#aofnome').val()==''){alert('Preencha o nome do evento');return false;}
					if(jQuery('#aofdata').val()==''){alert('Preencha a data do evento');return false;}
					if(jQuery('#aofcargahoraria').val()==''){alert('Preencha a carga horária');return false;}
					ajaxatualizar('requisicao=gravarEncontroPresencialCG&aofnome='+jQuery('#aofnome').val()+'&aofdata='+jQuery('#aofdata').val()+'&aofcargahoraria='+jQuery('#aofcargahoraria').val(),'');
					ajaxatualizar('requisicao=carregarEncontroPresencialCoordenadorIES','td_encontropresencialcoordenadories');
				}
	
				function removerEncontroPresencial(aofid) {
					var conf = confirm('Deseja realmente remover o encontro presencial?');
					if(conf) {
						ajaxatualizar('requisicao=removerEncontroPresencialCoordenadorIES&aofid='+aofid,'');
						ajaxatualizar('requisicao=carregarEncontroPresencialCoordenadorIES','td_encontropresencialcoordenadories');
					}
				}
																																						 		
				function removerRegional(afrid) {
					var conf = confirm('Deseja realmente remover a regional?');
					if(conf) {
						ajaxatualizar('requisicao=removerRegionalCoordenadorIES&afrid='+afrid,'');
						ajaxatualizar('requisicao=carregarRegionaisCoordenadorIES','td_regionais');
					}
				}
								
								
				function calcularOrcamentoExecucao() {
				
					var totalvalorexecutado  = 0;
					var totalsaldo           = 0;
					
					jQuery(\"[id^='valorprevisto_']\").each(function() {
					
						var orcid = replaceAll(jQuery(this).attr('id'),'valorprevisto_','');
						
						var valorprevisto  = parseFloat(replaceAll(replaceAll(jQuery('#valorprevisto_'+orcid).val(),'.',''),',','.'));
						
						var valorexecutado = 0;
						if(jQuery('#valorexecutado_'+orcid).val()!='') {
							valorexecutado = parseFloat(replaceAll(replaceAll(jQuery('#valorexecutado_'+orcid).val(),'.',''),',','.'));
						}
				
						var saldo          = valorprevisto-valorexecutado;
						
						if(saldo < 0) {
							jQuery('#saldo_'+orcid).val('-'+mascaraglobal('###.###.###,##',saldo.toFixed(2)));
						} else {
							jQuery('#saldo_'+orcid).val(mascaraglobal('###.###.###,##',saldo.toFixed(2)));
						}
						
						totalvalorexecutado  += valorexecutado;
						totalsaldo           += saldo;
				
					});
					
					jQuery('#totalvalorexecutado').val(mascaraglobal('###.###.###,##',totalvalorexecutado.toFixed(2)));
					
					var totalvalorprevisto = parseFloat(replaceAll(replaceAll(jQuery('#totalvalorprevisto').val(),'.',''),',','.'));
					
					if(totalsaldo < 0) {
						jQuery('#totalsaldo').val('-'+mascaraglobal('###.###.###,##',totalsaldo.toFixed(2)));
					} else {
						jQuery('#totalsaldo').val(mascaraglobal('###.###.###,##',totalsaldo.toFixed(2)));
					}
					
				}
								
			  </script>";
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr><td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>1. Dados gerais</span></td></tr>';
	$ies = $db->pegaUm("SELECT unisigla||' - '||uninome as uni FROM sismedio.universidadecadastro u INNER JOIN sismedio.universidade uu ON uu.uniid = u.uniid WHERE u.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'");
	echo '<tr><td class="SubTituloDireita">Instituição:</td><td>'.$ies.'</td></tr>';
	echo '<tr><td class="SubTituloDireita">Curso:</td><td>206 - Formação Continuada de Professores e Coordenadores Pedagógicos do Ensino Médio</td></tr>';
	$coordenadories = $db->pegaUm("SELECT iusnome FROM sismedio.identificacaousuario WHERE iusd='".$_SESSION['sismedio']['universidade']['iusd']."'");
	echo '<tr><td class="SubTituloDireita">Coordenador(a):</td><td>'.$coordenadories.'</td></tr>';
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Equipe IES</td></tr>';
	
	echo '<tr><td colspan="2">';
	
	$sql = "SELECT i.iusnome, pe.pfldsc, fe.foedesc, tp.tvpdsc, (SELECT count(DISTINCT m.menid) FROM sismedio.mensario m INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = m.menid WHERE m.iusd = i.iusd ) as totalavaliacoes, (SELECT avg(ma.mavtotal) FROM sismedio.mensario m INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = m.menid WHERE m.iusd = i.iusd ) as notafinal FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd AND t.pflcod IN(".PFL_COORDENADORIES.",".PFL_COORDENADORADJUNTOIES.",".PFL_SUPERVISORIES.",".PFL_FORMADORIES.",".PFL_FORMADORREGIONAL.")
			LEFT JOIN seguranca.perfil pe ON pe.pflcod = t.pflcod 
			LEFT JOIN public.tipovinculoprofissional tp ON tp.tvpid = i.tvpid 
			LEFT JOIN sismedio.formacaoescolaridade fe ON fe.foeid = i.foeid 
			WHERE i.iusstatus='A' AND i.uncid='".$_SESSION['sismedio']['universidade']['uncid']."' ORDER BY pe.pflnivel, pe.pfldsc, i.iusnome";
	
	$cabecalho = array("Nome","Função","Titulação","Vínculo","Número de avaliações","Nota final");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true, false, false, true);
	
	$sql = "SELECT pe.pfldsc, count(*) as totalqtd FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd AND t.pflcod IN(".PFL_COORDENADORIES.",".PFL_COORDENADORADJUNTOIES.",".PFL_SUPERVISORIES.",".PFL_FORMADORIES.",".PFL_FORMADORREGIONAL.")
			INNER JOIN seguranca.perfil pe ON pe.pflcod = t.pflcod
			WHERE i.iusstatus='A' AND i.uncid='".$_SESSION['sismedio']['universidade']['uncid']."' 
			GROUP BY pe.pflnivel, pe.pfldsc			
			ORDER BY pe.pflnivel, pe.pfldsc";
	
	$cabecalho = array("Perfil","Quantitativos");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',false, false, false, false);
	
	
	echo '</td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Período de execução do projeto</td></tr>';
	$data_validada_mec = $db->pegaUm("SELECT to_char(uncdatainicioprojeto,'dd/mm/YYYY')||' até '||to_char(uncdatafimprojeto,'dd/mm/YYYY') as periodo FROM sismedio.universidadecadastro WHERE uncid='".$_SESSION['sismedio']['universidade']['uncid']."'");
	echo '<tr><td class="SubTituloDireita">Validado pelo MEC:</td><td>'.$data_validada_mec.'</td></tr>';
	echo '<tr><td class="SubTituloDireita">Executado pela IES:</td><td>';
	echo campo_data2('afcexecucacaoprojetoini','S', (($dados['consulta'])?'N':'S'), 'Data início', 'S', '', '', '', '', '', 'afcexecucacaoprojetoini');
	echo ' até ';
	echo campo_data2('afcexecucacaoprojetofim','S', (($dados['consulta'])?'N':'S'), 'Data fim', 'S', '', '', '', '', '', 'afcexecucacaoprojetofim');
	echo '</td></tr>';
	echo '</table>';
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr><td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>2. Organização do curso</span></td></tr>';
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Regionais</td></tr>';

	echo '<tr><td colspan="2">';
		
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">';
	
	if(!$dados['consulta']) {

		echo '<tr><td class="SubTituloDireita" width="30%">Identificação da Regional (Nome):</td><td>'.campo_texto('afrnome', "N", "S", "Identificação da Regional (Nome)", 30, 60, "", "", '', '', 0, 'id="afrnome"').'</td><tr>';
		echo '<tr><td class="SubTituloDireita">Munícipio sede:</td><td>';
		
		$sql = "SELECT distinct m.muncod as codigo, m.estuf||' / '||m.mundescricao as descricao FROM sismedio.abrangencia a 
				INNER JOIN sismedio.estruturacurso e on e.ecuid = a.ecuid 
				INNER JOIN sismedio.listaescolasensinomedio l on l.lemcodigoinep = a.lemcodigoinep 
				INNER JOIN territorios.municipio m on m.muncod = l.muncod 
				WHERE e.uncid=".$_SESSION['sismedio']['universidade']['uncid'];
		
		$db->monta_combo('afrmuncodsede', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'afrmuncodsede','', $_REQUEST['afrmuncodsede']);
		
		echo '</td></tr>';
		echo '<tr><td class="SubTituloDireita">Área de Abrangência(Municípios):</td><td>';
		
		$sql = "SELECT distinct m.muncod as codigo, m.estuf||' / '||m.mundescricao as descricao FROM sismedio.abrangencia a 
				INNER JOIN sismedio.estruturacurso e on e.ecuid = a.ecuid 
				INNER JOIN sismedio.listaescolasensinomedio l on l.lemcodigoinep = a.lemcodigoinep 
				INNER JOIN territorios.municipio m on m.muncod = l.muncod 
				WHERE e.uncid=".$_SESSION['sismedio']['universidade']['uncid'];
		
		combo_popup( "muncodabr", $sql, "Municípios", "192x400", 0, array(), "", "S", false, false, 5, 400 );
		
		
		echo '</td></tr>';
		echo '<tr><td class="SubTituloDireita">Número de turmas de Formadores Regionais:</td><td>'.campo_texto('afrnumformadoresreg', "N", "S", "Identificação da Regional (Nome)", 9, 9, "#########", "", '', '', 0, 'id="afrnumformadoresreg"').'</td></tr>';
		echo '<tr><td class="SubTitulocentro" colspan="6"><input type="button" name="gravar" value="Inserir Regional" onclick="gravarRegional();"></td></tr>';
	}
	
	echo '<tr><td colspan="6" id="td_regionais">';
	
	carregarRegionaisCoordenadorIES(array('consulta'=>$dados['consulta']));
	
	echo '</td></tr>';
	
	
	echo '</table>';
	
	echo '</td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Planejamento Pedagógico do Curso <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape(\'Descrever a organização do processo formativo dos Formadores Regionais, dos Orientadores de Estudo e dos Professores e Coordenadores pedagógicos na UF (os processos de discussão da proposta teórico-metodológica do curso, os agentes envolvidos nessa discussão, tramitação e indicadores da proposta aprovada na Universidade/Secretaria de Educação, sistemática de reuniões para o planejamento, definição de conteúdos e material didático complementar, estratégias pedagógicas e de avaliação etc.)\');"></td></tr>';
	
	echo '<tr><td colspan="2">';
	
	if($dados['consulta']) {
		global $afcplanejamentopedagogico;
		echo nl2br($afcplanejamentopedagogico);
	} else {
		echo campo_textarea( 'afcplanejamentopedagogico', 'S', 'S', '', '85', '6', '3000');
	}
	
	echo '</td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Organização Pedagógica do Curso <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape(\'Descrever periocidade das reuniões dos coordenadores, supervisores e formadores, encaminhamentos acadêmicos no que diz respeito à participação nas atividades, à recuperação de atividades etc.\');"></td></tr>';
	
	echo '<tr><td colspan="2">';
	
	if($dados['consulta']) {
		global $afcorganizacaopedagogica;
		echo nl2br($afcorganizacaopedagogica);
	} else {
		echo campo_textarea( 'afcorganizacaopedagogica', 'S', 'S', '', '85', '6', '3000');
	}
	
	echo '</td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Estrutura Física e Suporte</td></tr>';
	
	echo '<tr><td colspan="2">';
	echo '<p>Selecione as instalações e os equipamentos utilizados durante os encontros de formação com os Formadores Regionais.</p>';
	
	global $afcestruturafisicasuporte;
	$afcestruturafisicasuporte = explode(";",trim($afcestruturafisicasuporte));
	if(!$afcestruturafisicasuporte) $afcestruturafisicasuporte = array();
	
	echo '<table width=100%>';
	echo '<tr><td><input type=checkbox name="afcestruturafisicasuporte[]" value="1" '.((in_array('1',$afcestruturafisicasuporte))?'checked':'').' '.(($dados['consulta'])?'disabled':'').'></td><td>Sala de aula da IES</td><td><input type=checkbox name="afcestruturafisicasuporte[]" value="4" '.((in_array('4',$afcestruturafisicasuporte))?'checked':'').' '.(($dados['consulta'])?'disabled':'').'></td><td>Equipamentos de som e imagem</td>';
	echo '<tr><td><input type=checkbox name="afcestruturafisicasuporte[]" value="2" '.((in_array('2',$afcestruturafisicasuporte))?'checked':'').' '.(($dados['consulta'])?'disabled':'').'></td><td>Sala de aula de escola pública</td><td><input type=checkbox name="afcestruturafisicasuporte[]" value="5" '.((in_array('5',$afcestruturafisicasuporte))?'checked':'').' '.(($dados['consulta'])?'disabled':'').'></td><td>Equipamentos de informática</td>';
	echo '<tr><td><input type=checkbox name="afcestruturafisicasuporte[]" value="3" '.((in_array('3',$afcestruturafisicasuporte))?'checked':'').' '.(($dados['consulta'])?'disabled':'').'></td><td>Locação de espaço</td><td><input type=checkbox name="afcestruturafisicasuporte[]" value="6" '.((in_array('6',$afcestruturafisicasuporte))?'checked':'').' '.(($dados['consulta'])?'disabled':'').'></td><td>Outros materiais pedagógicos</td>';
	echo '</table>';
	
	echo '</td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Encontros Presenciais</td></tr>';
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	
	if(!$dados['consulta']) {
		echo '<tr><td class="SubTituloDireita">Nome do evento:</td><td>'.campo_texto('aofnome', "N", "S", "Nome do evento", 30, 60, "", "", '', '', 0, 'id="aofnome"').'</td><td class="SubTituloDireita">Data do evento:</td><td>'.campo_data2('aofdata','S', 'S', 'Data do evento', 'S', '', '', '', '', '', 'aofdata').'</td><td class="SubTituloDireita">Carga horária:</td><td>'.campo_texto('aofcargahoraria', "N", "S", "Carga horária", 6, 10, "######", "", '', '', 0, 'id="aofcargahoraria"').'</td></tr>';
		echo '<tr><td class="SubTitulocentro" colspan="6"><input type="button" name="gravar" value="Inserir Evento" onclick="gravarEncontroPresencial();"></td></tr>';
	}
	
	echo '<tr><td colspan="6" id="td_encontropresencialcoordenadories">';
	
	carregarEncontroPresencialCoordenadorIES(array('consulta'=>$dados['consulta']));
	
	echo '</td></tr>';
	
	
	echo '</table>';
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr><td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>3. Execução financeira</span></td></tr>';
	
	echo '<tr><td colspan="2">';
	
	carregarListaCustos(array('uncid' => $_SESSION['sismedio']['universidade']['uncid'],'execucao'=> true,'consulta'=>$dados['consulta']));
	
	echo '</td></tr>';
	
	echo '</table>';
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr><td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>4. Bolsas Pagas</span></td></tr>';
	
	echo '<tr><td colspan="2">';
	
	$sql = "SELECT pe.pfldsc, count(p.pboid) as qtd, sum(p.pbovlrpagamento) as valor FROM sismedio.pagamentobolsista p 
			INNER JOIN seguranca.perfil pe ON pe.pflcod = p.pflcod 
			INNER JOIN sismedio.universidadecadastro u ON u.uniid = p.uniid 
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			WHERE u.uncid='".$_SESSION['sismedio']['universidade']['uncid']."' AND d.esdid='".ESD_PAGAMENTO_EFETIVADO."' 
			GROUP BY pe.pfldsc, pe.pflnivel
			ORDER BY pe.pflnivel";
	
	$cabecalho = array("Perfil","Número de bolsas","Valor pago (R$)");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','',true, false, false, true);
	
	echo '</td></tr>';
	
	echo '</table>';
	
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr><td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>5. Indicadores de Desempenho</span></td></tr>';
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Taxa de conclusão</td></tr>';
	echo '<tr><td colspan="2">';
	
	$pflcodhieraquia = array(PFL_FORMADORREGIONAL => PFL_SUPERVISORIES,PFL_ORIENTADORESTUDO => PFL_FORMADORREGIONAL,PFL_PROFESSORALFABETIZADOR => PFL_ORIENTADORESTUDO,PFL_COORDENADORPEDAGOGICO => PFL_ORIENTADORESTUDO);
	
	$perfis = $db->carregar("SELECT pflcod, pfldsc FROM seguranca.perfil WHERE pflcod IN(".PFL_FORMADORREGIONAL.",".PFL_ORIENTADORESTUDO.",".PFL_PROFESSORALFABETIZADOR.",".PFL_COORDENADORPEDAGOGICO.")");
	
	if($perfis) {
		foreach($perfis as $perfil) {

			$qtd_total = $db->pegaUm("SELECT count(*) as total FROM sismedio.identificacaousuario i 
									  INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
									  WHERE t.pflcod='".$perfil['pflcod']."' AND i.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'");

			$qtd_certificados = $db->pegaUm("SELECT count(*) as certificados FROM (
			
											SELECT i.iusd, sum(mavfrequencia) as freq, avg(mavtotal) as tot FROM sismedio.identificacaousuario i
											INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd
											INNER JOIN sismedio.mensario m ON m.iusd = i.iusd
											INNER JOIN sismedio.mensarioavaliacoes ma ON ma.menid = m.menid
											INNER JOIN sismedio.identificacaousuario i2 ON i2.iusd = ma.iusdavaliador
											INNER JOIN sismedio.tipoperfil t2 ON t2.iusd = i2.iusd AND t2.pflcod='".$pflcodhieraquia[$perfil['pflcod']]."'
											WHERE t.pflcod='".$perfil['pflcod']."' AND i.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'
											GROUP BY i.iusd
											
											) foo WHERE foo.freq >= (SELECT (count(*)*0.75) as presencaminima FROM sismedio.folhapagamentouniversidade WHERE pflcod='".$perfil['pflcod']."' ANd uncid='".$_SESSION['sismedio']['universidade']['uncid']."') AND foo.tot >=7");

			$qtd_naocertificados = ($qtd_total-$qtd_certificados);


			$porc_certificados = round(($qtd_certificados/$qtd_total)*100,2);
			$porc_naocertificados = round(($qtd_naocertificados/$qtd_total)*100,2);

			$arrTxConclusao[] = array('perfil'=>$perfil['pfldsc'],'qtdtotal'=>$qtd_total,'qtdcertificados'=>$qtd_certificados,'porc_certificados'=>'<span style=float:right;>'.$porc_certificados.'</span>','qtd_naocertificados'=>$qtd_naocertificados,'porc_naocertificados'=>'<span style=float:right;>'.$porc_naocertificados.'</span>');

		}
	}
	
	$cabecalho = array("Perfil","Total de inscritos","Recomendados para certificação","%","Não recomendados para certificação","%");
	$db->monta_lista_simples($arrTxConclusao,$cabecalho,100000,5,'S','100%','');
	
	
	echo '</td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Taxa de ocupação</td></tr>';
	echo '<tr><td colspan="2">';
	
	$qtd_total_oe = $db->pegaUm("SELECT count(*) as total FROM sismedio.identificacaousuario i
									  INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd
									  WHERE t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'");

	$qtd_total_pp = $db->pegaUm("SELECT count(*) as total FROM sismedio.identificacaousuario i
							     INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd
								 WHERE t.pflcod IN('".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORPEDAGOGICO."') AND i.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'");


	$qtd_limite_oe = $db->pegaUm("SELECT sum(lemnumorientadores::numeric) as total FROM sismedio.abrangencia a 
								  INNER JOIN sismedio.listaescolasensinomedio l ON a.lemcodigoinep = l.lemcodigoinep 
								  INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
								  WHERE e.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'");

	$qtd_limite_pp = $db->pegaUm("SELECT sum(lemdoctotal::numeric) as total FROM sismedio.abrangencia a
								  INNER JOIN sismedio.listaescolasensinomedio l ON a.lemcodigoinep = l.lemcodigoinep
								  INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid
								  WHERE e.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'");

	$porc_ocupacao_oe = round(($qtd_total_oe/$qtd_limite_oe)*100,2);
	$porc_ocupacao_pp = round(($qtd_total_pp/$qtd_limite_pp)*100,2);

	$arrTxOcupacao[] = array('perfil'=>'Orientador de Estudo','vagasdisp'=>$qtd_limite_oe,'inscritos'=>$qtd_total_oe,'porc_ocupacao_oe'=>'<span style=float:right;>'.$porc_ocupacao_oe.'</span>');
	$arrTxOcupacao[] = array('perfil'=>'Professor /Coordenador Pedagógico','vagasdisp'=>$qtd_limite_pp,'inscritos'=>$qtd_total_pp,'porc_ocupacao_pp'=>'<span style=float:right;>'.$porc_ocupacao_pp.'</span>');
	
	$cabecalho = array("Perfil","Vagas disponíveis","Inscritos (cadastrados pelo gestor da escola)","%");
	$db->monta_lista_simples($arrTxOcupacao,$cabecalho,100000,5,'S','100%','');
	
	
	echo '</td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Relação entre o número de profissionais</td></tr>';
	echo '<tr><td colspan="2">';
	
	$qtd_perfil = $db->carregar("SELECT t.pflcod, count(*) as total FROM sismedio.identificacaousuario i
								   INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
								   WHERE i.iusstatus='A' AND t.pflcod IN(".PFL_SUPERVISORIES.",".PFL_FORMADORIES.",".PFL_FORMADORREGIONAL.",".PFL_ORIENTADORESTUDO.",".PFL_PROFESSORALFABETIZADOR.",".PFL_COORDENADORPEDAGOGICO.") AND i.uncid='".$_SESSION['sismedio']['universidade']['uncid']."' 
								   GROUP BY t.pflcod");

	if($qtd_perfil[0]) {
		foreach($qtd_perfil as $qtd) {
			$totais[$qtd['pflcod']] = $qtd['total'];
		}
	}
	
	$arrTxRelprof[] = array('perfil'=>'Formador IES / Formador Regional','rel'=>'<span style=float:right;>'.$totais[PFL_FORMADORIES].' / '.$totais[PFL_FORMADORREGIONAL].'</span>','tx'=>'<span style=float:right;>'.round(($totais[PFL_FORMADORIES]/$totais[PFL_FORMADORREGIONAL]),2).'</span>');
	$arrTxRelprof[] = array('perfil'=>'Supervisor IES / Formador Regional','rel'=>'<span style=float:right;>'.$totais[PFL_SUPERVISORIES].' / '.$totais[PFL_FORMADORREGIONAL].'</span>','tx'=>'<span style=float:right;>'.round(($totais[PFL_SUPERVISORIES]/$totais[PFL_FORMADORREGIONAL]),2).'</span>');
	$arrTxRelprof[] = array('perfil'=>'Formador Regional / Orientador de Estudo','rel'=>'<span style=float:right;>'.$totais[PFL_FORMADORREGIONAL].' / '.$totais[PFL_ORIENTADORESTUDO].'</span>','tx'=>'<span style=float:right;>'.round(($totais[PFL_FORMADORREGIONAL]/$totais[PFL_ORIENTADORESTUDO]),2).'</span>');
	$arrTxRelprof[] = array('perfil'=>'Orientador de Estudo / Professor e Coordenador Pedagógico','rel'=>'<span style=float:right;>'.$totais[PFL_ORIENTADORESTUDO].' / '.($totais[PFL_PROFESSORALFABETIZADOR]+$totais[PFL_COORDENADORPEDAGOGICO]).'</span>','tx'=>'<span style=float:right;>'.round(($totais[PFL_ORIENTADORESTUDO]/($totais[PFL_PROFESSORALFABETIZADOR]+$totais[PFL_COORDENADORPEDAGOGICO])),2).'</span>');

	
	$cabecalho = array("Relação","Valores","Taxa");
	$db->monta_lista_simples($arrTxRelprof,$cabecalho,100000,5,'S','100%','');
	
	echo '</td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Qualificação da equipe docente</td></tr>';
	echo '<tr><td colspan="2">';
	
	$sql = "SELECT foo.pfldsc, count(*), sum(foo.doutorado) as doutorado, '<span style=float:right>'||round((sum(foo.doutorado)::numeric/count(*)::numeric)*100,2)||'</span>' as porc_doutorado, sum(foo.mestrado) as mestrado, '<span style=float:right>'||round((sum(foo.mestrado)::numeric/count(*)::numeric)*100,2)||'</span>' as porc_mestrado, sum(foo.especializacao) as especializacao, '<span style=float:right>'||round((sum(foo.especializacao)::numeric/count(*)::numeric)*100,2)||'</span>' as porc_especializacao, sum(foo.graduacao) as graduacao, '<span style=float:right>'||round((sum(foo.graduacao)::numeric/count(*)::numeric)*100,2)||'</span>' as porc_graduacao 
			FROM (
			SELECT p.pfldsc, 
				   CASE WHEN i.foeid=".FOE_DOUTORADO." THEN 1 ELSE 0 END as doutorado, 
				   CASE WHEN i.foeid=".FOE_MESTRADO." THEN 1 ELSE 0 END as mestrado,
				   CASE WHEN i.foeid=".FOE_ESPECIALIZACAO." THEN 1 ELSE 0 END as especializacao,
				   CASE WHEN i.foeid IN(".FOE_SUPERIOR_COMPLETO_PEDAGOGIA.",".FOE_SUPERIOR_COMPLETO_LICENCIATURA.",".FOE_SUPERIOR_COMPLETO_OUTRO.") THEN 1 ELSE 0 END as graduacao 
			FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod IN(".PFL_FORMADORIES.",".PFL_FORMADORREGIONAL.",".PFL_ORIENTADORESTUDO.") AND i.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'
			) foo 
			GROUP BY foo.pfldsc 
			";	
	
	$cabecalho = array("Perfil","Total","Doutorado","%","Mestrado","%","Especialização","%","Graduação","%");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'S','100%','');
	
	echo '</td></tr>';
	
	echo '</table>';
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr><td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>6. Análise Crítica</span></td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Sobre o conteúdo do curso <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape(\'Analisar sinteticamente conteúdos que deveriam ser revisados, justificando a proposição.\');"></td></tr>';
	
	echo '<tr><td colspan="2">';
	
	if($dados['consulta']) {
		global $afcsobreconteudocurso;
		echo nl2br($afcsobreconteudocurso);
	} else {
		echo campo_textarea( 'afcsobreconteudocurso', 'S', 'S', '', '85', '6', '3000');
	}
	
	echo '</td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Sobre a metodologia <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape(\'Analisar sinteticamente a metodologia de trabalho, indicando pontos de melhoria, se for o caso.\');"></td></tr>';
	
	echo '<tr><td colspan="2">';
	
	if($dados['consulta']) {
		global $afcsobremetodologia;
		echo nl2br($afcsobremetodologia);
	} else {
		echo campo_textarea( 'afcsobremetodologia', 'S', 'S', '', '85', '6', '3000');
	}
	
	echo '</td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Sobre os critérios de avaliação <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape(\'Avaliar a conveniência e adequação dos critérios de avaliação adotados pelo MEC: Frequência, Atividades realizadas e Monitoramento.\');"></td></tr>';
	
	echo '<tr><td colspan="2">';
	
	if($dados['consulta']) {
		global $afccriteriosavaliacao;
		echo nl2br($afccriteriosavaliacao);
	} else {
		echo campo_textarea( 'afccriteriosavaliacao', 'S', 'S', '', '85', '6', '3000');
	}
	
	echo '</td></tr>';
	
	
	echo '</table>';
	
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr><td class="SubTituloEsquerda" colspan="2"><span style=font-size:large;>7. Comentários finais</span></td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Sobre a articulação com MEC <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape(\'Avaliar articulação institucional entre o MEC e a Universidade contemplando no mínimo os seguintes aspectos:<br>- Tempestividade nas respostas<br>- Agilidade no pagamento de bolsas<br>- Qualidade do suporte tecnológico<br>- Liberação de recursos.\');"></td></tr>';
	
	echo '<tr><td colspan="2">';
	
	if($dados['consulta']) {
		global $afcarticulacaomec;
		echo nl2br($afcarticulacaomec);
	} else {
		echo campo_textarea( 'afcarticulacaomec', 'S', 'S', '', '85', '6', '3000');
	}
	
	echo '</td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Lições Aprendidas <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape(\'Descrever eventos/ situações/ experiências que contribuem para o aperfeiçoamento do programa e do curso.\');"></td></tr>';
	
	echo '<tr><td colspan="2">';
	
	if($dados['consulta']) {
		global $afclicoesaprendidas;
		echo nl2br($afclicoesaprendidas);
	} else {
		echo campo_textarea( 'afclicoesaprendidas', 'S', 'S', '', '85', '6', '3000');
	}
	
	echo '</td></tr>';
	
	echo '<tr><td class="SubTituloEsquerda" colspan="2">Outros comentários <img src="../imagens/ajuda.gif" align="absmiddle" onmouseover="return escape(\'Inserir informações não contempladas neste relatório consideradas relevantes para o aperfeiçoamento do programa e do curso.\');"></td></tr>';
	
	echo '<tr><td colspan="2">';
	
	if($dados['consulta']) {
		global $afcoutroscomentarios;
		echo nl2br($afcoutroscomentarios);
	} else {
		echo campo_textarea( 'afcoutroscomentarios', 'S', 'S', '', '85', '6', '3000');
	}
	
	echo '</td></tr>';
	
	
	
	echo '</table>';
	
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	
	echo '<tr><td colspan="2" align="center">';
	
	echo '<br><br><br><br><br><br><br>';
	
	$reitor = $db->pegaUm("SELECT reinome||' - '||replace(to_char(reicpf::numeric, '000:000:000-00'), ':', '.') as reitor FROM sismedio.reitor r 
						   INNER JOIN sismedio.universidadecadastro u ON u.uniid = r.uniid
						   WHERE u.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'");
	
	
	echo '_____________________________________________________________________________________<br>';
	echo $reitor.'<br>';
	echo '<span style="font-size:x-small;">Reitor</span>';
	
	echo '<br><br><br><br><br><br><br>';
	
	$coordenadorgeral = $db->pegaUm("SELECT iusnome||' - '||replace(to_char(iuscpf::numeric, '000:000:000-00'), ':', '.') as reitor FROM sismedio.identificacaousuario i
						   INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd
						   WHERE i.iusd='".$_SESSION['sismedio']['universidade']['iusd']."'");
	
	
	echo '_____________________________________________________________________________________<br>';
	echo $coordenadorgeral.'<br>';
	echo '<span style="font-size:x-small;">Coordenador-Geral da IES</span>';
	
	
	
	echo '</td></tr>';
	
	
	
	echo '</table>';
	
	$dadosserv = ob_get_contents();
	ob_clean();
	
	$es['texto0'] = array('texto' => $dadosserv,
			'tipo' => 'textual',
	);


	return $es;
}

function gravarAvaliacaoFinalCG($dados) {
	global $db;

	if($dados['orcvalorexecutado']) {
		foreach($dados['orcvalorexecutado'] as $orcid => $vl) {
			$sql = "UPDATE sismedio.orcamento SET orcvlrexecutado=".(($vl)?"'".str_replace(array(".",","),array("","."),$vl)."'":"NULL")." WHERE orcid='".$orcid."'";
			$db->executar($sql);
		}
	}
	
	
	$afcid = $db->pegaUm("SELECT afcid FROM sismedio.avaliacaofinalcg WHERE iusd='".$_SESSION['sismedio']['universidade']['iusd']."'");
	
	if($afcid) {
	
			$sql = "UPDATE sismedio.avaliacaofinalcg SET
					afcexecucacaoprojetoini=".(($dados['afcexecucacaoprojetoini'])?"'".formata_data_sql($dados['afcexecucacaoprojetoini'])."'":"NULL").",
					afcexecucacaoprojetofim=".(($dados['afcexecucacaoprojetofim'])?"'".formata_data_sql($dados['afcexecucacaoprojetofim'])."'":"NULL").",
					afcestruturafisicasuporte=".(($dados['afcestruturafisicasuporte'])?"'".implode(";",$dados['afcestruturafisicasuporte'])."'":"NULL").",
            		afcsobreconteudocurso=".(($dados['afcsobreconteudocurso'])?"'".$dados['afcsobreconteudocurso']."'":"NULL").",
					afcsobremetodologia=".(($dados['afcsobremetodologia'])?"'".$dados['afcsobremetodologia']."'":"NULL").",
					afccriteriosavaliacao=".(($dados['afccriteriosavaliacao'])?"'".$dados['afccriteriosavaliacao']."'":"NULL").",
					afcarticulacaomec=".(($dados['afcarticulacaomec'])?"'".$dados['afcarticulacaomec']."'":"NULL").",
					afclicoesaprendidas=".(($dados['afclicoesaprendidas'])?"'".$dados['afclicoesaprendidas']."'":"NULL").",
					afcoutroscomentarios=".(($dados['afcoutroscomentarios'])?"'".addslashes($dados['afcoutroscomentarios'])."'":"NULL").",
					afcplanejamentopedagogico=".(($dados['afcplanejamentopedagogico'])?"'".addslashes($dados['afcplanejamentopedagogico'])."'":"NULL").",
					afcorganizacaopedagogica=".(($dados['afcorganizacaopedagogica'])?"'".addslashes($dados['afcorganizacaopedagogica'])."'":"NULL")."
					WHERE afcid={$afcid}";
	
		} else {

			$sql = "INSERT INTO sismedio.avaliacaofinalcg(
		            iusd, 
					afcexecucacaoprojetoini, 
					afcexecucacaoprojetofim, 
					afcestruturafisicasuporte,
		            afcsobreconteudocurso,
					afcsobremetodologia,
					afccriteriosavaliacao,
					afcarticulacaomec,
					afclicoesaprendidas,
					afcoutroscomentarios,
					afcplanejamentopedagogico,
					afcorganizacaopedagogica)
				    VALUES ('".$_SESSION['sismedio']['universidade']['iusd']."', 
							".(($dados['afcexecucacaoprojetoini'])?"'".formata_data_sql($dados['afcexecucacaoprojetoini'])."'":"NULL").", 
							".(($dados['afcexecucacaoprojetofim'])?"'".formata_data_sql($dados['afcexecucacaoprojetofim'])."'":"NULL").", 
							".(($dados['afcestruturafisicasuporte'])?"'".implode(";",$dados['afcestruturafisicasuporte'])."'":"NULL").",
				            ".(($dados['afcsobreconteudocurso'])?"'".$dados['afcsobreconteudocurso']."'":"NULL").",
							".(($dados['afcsobremetodologia'])?"'".$dados['afcsobremetodologia']."'":"NULL").",
							".(($dados['afccriteriosavaliacao'])?"'".$dados['afccriteriosavaliacao']."'":"NULL").",
						    ".(($dados['afcarticulacaomec'])?"'".$dados['afcarticulacaomec']."'":"NULL").",
							".(($dados['afclicoesaprendidas'])?"'".$dados['afclicoesaprendidas']."'":"NULL").",
							".(($dados['afcoutroscomentarios'])?"'".addslashes($dados['afcoutroscomentarios'])."'":"NULL").",
							".(($dados['afcplanejamentopedagogico'])?"'".addslashes($dados['afcplanejamentopedagogico'])."'":"NULL").",
							".(($dados['afcorganizacaopedagogica'])?"'".addslashes($dados['afcorganizacaopedagogica'])."'":"NULL")."
					);";
	
	}
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Avaliação final gravada com sucesso","location"=>"sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=avaliacaofinal");
	alertlocation($al);
	
	
}

function gravarRegionalCG($dados) {
	global $db;
	
	$sql = "INSERT INTO sismedio.avaliacaofinalcgregional(
            iusd, afrnome, afrmuncodsede, afrnumformadoresreg)
    		VALUES ('".$_SESSION['sismedio']['universidade']['iusd']."', '".$dados['afrnome']."', '".$dados['afrmuncodsede']."', '".$dados['afrnumformadoresreg']."') RETURNING afrid";

	$afrid = $db->pegaUm($sql);
	
	if($dados['muncodabr']) {
		foreach($dados['muncodabr'] as $muncodabr) {

			$sql = "INSERT INTO sismedio.avaliacaofinalcgregionalabrangencia(
			            afrid, muncod)
			    VALUES ('".$afrid."', '".$muncodabr."');";
			
			$db->executar($sql);

		}
	}
	
	$db->commit();

}

function carregaDetalhesPeriodoReferencia($dados) {
	global $db;
	
	$hab = 'S';
	
	$sql_mes = "SELECT mescod::integer as codigo, mesdsc as descricao FROM public.meses m INNER JOIN sismedio.folhapagamento f ON f.fpbmesreferencia=m.mescod::integer GROUP BY mescod::integer, mesdsc ORDER BY mescod::integer";
	$arrMES = $db->carregar($sql_mes);
	$sql_ano = "SELECT ano as codigo, ano as descricao FROM public.anos m INNER JOIN sismedio.folhapagamento f ON f.fpbanoreferencia=m.ano::integer GROUP BY m.ano ORDER BY m.ano";
	$arrANO = $db->carregar($sql_ano);
	
	$perfis = $db->carregar("SELECT * FROM seguranca.perfil p
							 INNER JOIN sismedio.pagamentoperfil pp ON pp.pflcod = p.pflcod
							 ORDER BY p.pflnivel");
	
	echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
	foreach($perfis as $pfl) {
	
		$arr = $db->pegaLinha("SELECT MIN((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanoinip, MAX((fpbanoreferencia||'-'||fpbmesreferencia||'-01')::date) as mesanofimp
								  FROM sismedio.folhapagamentouniversidade fu
								  INNER JOIN sismedio.folhapagamento f ON f.fpbid = fu.fpbid
								  WHERE fu.uncid='".$dados['uncid']."' AND fu.pflcod='".$pfl['pflcod']."'");
	
		$arrini = explode("-",$arr['mesanoinip']);
		$arrfim = explode("-",$arr['mesanofimp']);
	
	
	
		echo '<tr>';
		echo '<td align=right><img src=../imagens/report.gif style=cursor:pointer; onclick="exibirPeriodosReferencia('.$dados['uncid'].','.$pfl['pflcod'].');"> <img src=../imagens/seta_filho.gif> '.$pfl['pfldsc'].'</td>';
		echo '<td align="center">';
		$db->monta_combo($hab.'mesinip['.$dados['uncid'].']['.$pfl['pflcod'].']', $arrMES, $hab, 'Selecione', '', '', '', '', 'N', 'mesinip_'.$dados['uncid'].'_'.$pfl['pflcod'],'', $arrini[1]);
		echo ' / ';
		$db->monta_combo($hab.'anoiniciop['.$dados['uncid'].']['.$pfl['pflcod'].']', $arrANO, $hab, 'Selecione', '', '', '', '', 'N', 'anoiniciop','', $arrini[0]);
		echo '</td>';
		echo '<td align="center">';
		$db->monta_combo($hab.'mesfimp['.$dados['uncid'].']['.$pfl['pflcod'].']', $arrMES, $hab, 'Selecione', '', '', '', '', 'N', 'mesfimp_'.$dados['uncid'].'_'.$pfl['pflcod'],'', $arrfim[1]);
		echo ' / ';
		$db->monta_combo($hab.'anofimp['.$dados['uncid'].']['.$pfl['pflcod'].']', $arrANO, $hab, 'Selecione', '', '', '', '', 'N', 'anofimp','', $arrfim[0]);
		echo '</td>';
		echo '</tr>';
		
	}
	echo '</table>';
	
	
}

function inserirTipoAvaliacao($dados) {
	global $db;
	
	if($dados['tipo']=='inserir') {

		$sql = "INSERT INTO sismedio.tipoavaliacaoperfil(
	            fpbid, pflcod, uncid, tpatipoavaliacao)
	    		VALUES ('".$dados['fpbid']."', '".$dados['pflcod']."', '".$dados['uncid']."', 'monitoramentoTextual');";
		
		$db->executar($sql);
		$db->commit();
		
	}
	
	if($dados['tipo']=='remover') {
	
		$sql = "DELETE FROM sismedio.tipoavaliacaoperfil WHERE fpbid='".$dados['fpbid']."' AND pflcod='".$dados['pflcod']."' AND uncid='".$dados['uncid']."'";
	
		$db->executar($sql);
		$db->commit();
	
	}
	
}

?>