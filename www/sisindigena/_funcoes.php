<?
include_once '_funcoes_avaliacoes.php';


function removerTipoPerfil($dados) {
	global $db;
	
	// verificando pagamento
	$sql = "SELECT p.pboid FROM sisindigena.tipoperfil t 
			INNER JOIN sisindigena.pagamentobolsista p ON p.tpeid = t.tpeid  
			WHERE t.iusd='".$dados['iusd']."' AND t.pflcod='".$dados['pflcod']."'";
	
	$pboid = $db->pegaUm($sql);
	
	if($pboid) {
		if(!$dados['naoredirecionar']) {
			if($dados['picid']) $al = array("alert"=>"Coordenador Local ja possui pagamento e não pode ser removido, somente substituido","location"=>"sisindigena.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=".$dados['picid']);
			if($dados['uncid']) $al = array("alert"=>"Coordenador IES ja possui pagamento e não pode ser removido, somente substituido","location"=>"sisindigena.php?modulo=principal/universidade/gerenciarcoordenadories&acao=A&uncid=".$dados['uncid']);
			
			if($al) alertlocation($al); 
			else excluirUsuarioPerfil($dados);
			
		} else {
			return false;
		}
	}
	
	$existe_equipe = $db->pegaUm("SELECT count(*) as nu FROM sisindigena.orientadorturma WHERE turid IN(SELECT turid FROM sisindigena.turmas WHERE iusd='".$dados['iusd']."')");
	
	if($existe_equipe) {
		
		$al = array("alert"=>"Perfil selecionado possui equipe cadastrada e não pode ser removido, somente substituido","location"=>"sisindigena.php?modulo=".$dados['modulo']."&acao=A&aba=".$dados['aba']);
		alertlocation($al);
		
	} else {
		
		$sql = "DELETE FROM sisindigena.turmas WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
		
	}
	
	$sql = "DELETE FROM sisindigena.tipoperfil WHERE iusd='".$dados['iusd']."' AND pflcod='".$dados['pflcod']."'";
	$db->executar($sql);
	
	$usucpf = $db->pegaUm("SELECT iuscpf FROM sisindigena.identificacaousuario WHERE iusd='".$dados['iusd']."'");
	
	if($usucpf) {
		$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$usucpf."' AND pflcod='".$dados['pflcod']."'";
		$db->executar($sql);
		$sql = "DELETE FROM sisindigena.usuarioresponsabilidade WHERE usucpf='".$usucpf."' AND pflcod='".$dados['pflcod']."'";
		$db->executar($sql);
	}
	
	$sql = "DELETE FROM sisindigena.orientadorturma WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	
	$sql = "INSERT INTO sisindigena.historicoidentificaousuario(
            iusd, hiudatainc, hiucpf, hiulog, hiustatus, hiutipo)
    		VALUES ('".$dados['iusd']."', NOW(), '".$_SESSION['usucpf']."', '".str_replace(array("'"),array(""),simec_json_encode($dados))."', 'A', 'removerTipoPerfil');";
	$db->executar($sql);
	
	$db->executar("UPDATE sisindigena.identificacaousuario SET iusstatus='I' WHERE iusd='".$dados['iusd']."'");
	
	$db->commit();
	
	if(!$dados['naoredirecionar']) {
		$al = array("alert"=>"Membro removido com sucesso","location"=>"sisindigena.php?modulo=".$dados['modulo']."&acao=A".(($dados['picid'])?"&picid=".$dados['picid']:"").(($dados['aba'])?"&aba=".$dados['aba']:""));
		alertlocation($al);
	}
	
}

function verificaPermissao() {
	global $db;
	$perfis = pegaPerfilGeral();
	$sql = "SELECT * FROM sisindigena.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND rpustatus='A'";
	$ur = $db->carregar($sql);

	if($db->testa_superuser()) {
		return false;
	}
	
	if(in_array(PFL_COORDENADORLOCAL,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_COORDENADORLOCAL && $urr['muncod']==$_SESSION['sisindigena']['coordenadorlocal'][$_SESSION['sisindigena']['esfera']]['muncod']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_EQUIPEMUNICIPALAP,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_EQUIPEMUNICIPALAP && $urr['muncod']==$_SESSION['sisindigena']['coordenadorlocal'][$_SESSION['sisindigena']['esfera']]['muncod']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_CONSULTAMUNICIPAL,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_CONSULTAMUNICIPAL && $urr['muncod']==$_SESSION['sisindigena']['coordenadorlocal'][$_SESSION['sisindigena']['esfera']]['muncod']) {
					return true;
				}
			}
		}
	}
	
	if(in_array(PFL_EQUIPEESTADUALAP,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_EQUIPEESTADUALAP && $urr['estuf']==$_SESSION['sisindigena']['coordenadorlocal'][$_SESSION['sisindigena']['esfera']]['estuf']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_CONSULTAESTADUAL,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_CONSULTAESTADUAL && $urr['estuf']==$_SESSION['sisindigena']['coordenadorlocal'][$_SESSION['sisindigena']['esfera']]['estuf']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_COORDENADORIES,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_COORDENADORIES && $urr['uncid']==$_SESSION['sisindigena']['universidade']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_COORDENADORADJUNTOIES,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_COORDENADORADJUNTOIES && $urr['uncid']==$_SESSION['sisindigena']['coordenadoradjuntoies']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_PROFESSORALFABETIZADOR,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_PROFESSORALFABETIZADOR && $urr['uncid']==$_SESSION['sisindigena']['professoralfabetizador']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_ORIENTADORESTUDO,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_ORIENTADORESTUDO && $urr['uncid']==$_SESSION['sisindigena']['orientadorestudo']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_CONTEUDISTA,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_CONTEUDISTA && $urr['uncid']==$_SESSION['sisindigena']['conteudista']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_PESQUISADOR,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_PESQUISADOR && $urr['uncid']==$_SESSION['sisindigena']['pesquisador']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_FORMADORIES,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_FORMADORIES && $urr['uncid']==$_SESSION['sisindigena']['formadories']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_SUPERVISORIES,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_SUPERVISORIES && $urr['picid']==$_SESSION['sisindigena']['supervisories']['picid']) {
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


function montaAbasSisIndigena($abapai, $abaativa) {
	global $db;
	
	$sql = "SELECT abaordem, abadsc, abaendereco, abafuncaomostrar, abapai FROM sisindigena.abas WHERE abapai='".$abapai."' ORDER BY abaordem";
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
	
	echo montarAbasArray($menu, $abaativa);
}

function carregarDadosIdentificacaoUsuario($dados) {
	global $db;
	
	if(!$dados['pflcod']) {
		$al = array("alert"=>"Problemas para carregar os dados usuário","location"=>"sisindigena.php?modulo=inicio&acao=C");
		alertlocation($al);
	}
	
	$sql = "SELECT i.cadastradosgb, i.uncid, i.iusd, i.iuscpf, i.iusnome, i.iusdatanascimento, i.iusnomemae, i.iustipoprofessor, i.iusnaodesejosubstituirbolsa,
				   i.iussexo, i.eciid, i.nacid, i.iusnomeconjuge, i.iusagenciasugerida, i.iusagenciaend, i.iusformacaoinicialorientador,
				   i.iusemailprincipal, i.iusemailopcional, i.iustipoorientador, to_char(i.iusdatainclusao,'YYYY-mm-dd') as iusdatainclusao, i.iustermocompromisso,  
				   i.tvpid, i.funid, i.foeid, f.iufid, f.cufid, f.iufdatainiformacao, f.iufdatafimformacao, f.iufsituacaoformacao,
				   m.estuf as estuf_nascimento, m.muncod as muncod_nascimento, ma.estuf||' / '||ma.mundescricao as municipiodescricaoatuacao, ma.muncod as muncodatuacao, 
				   d.itdid, d.tdoid, d.itdufdoc, d.itdnumdoc, d.itddataexp, d.itdnoorgaoexp,
				   e.ienid, mm.muncod as muncod_endereco, mm.estuf as estuf_endereco,
				   e.ientipo, e.iencep, e.iencomplemento, e.iennumero, e.ienlogradouro, e.ienbairro, cf.cufcodareageral, to_char(t.tpeatuacaoinicio,'YYYY-mm-dd') as tpeatuacaoinicio, to_char(t.tpeatuacaofim,'YYYY-mm-dd') as tpeatuacaofim, i.iusserieprofessor   
			FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			LEFT  JOIN territorios.municipio m ON m.muncod = i.muncod 
			LEFT  JOIN sisindigena.identiusucursoformacao f ON f.iusd = i.iusd 
			LEFT  JOIN sisindigena.identusutipodocumento d ON d.iusd = i.iusd 
			LEFT  JOIN sisindigena.identificaoendereco e ON e.iusd = i.iusd 
			LEFT  JOIN territorios.municipio mm ON mm.muncod = e.muncod 
			LEFT  JOIN territorios.municipio ma ON ma.muncod = i.muncodatuacao
			LEFT  JOIN sisindigena.cursoformacao cf ON cf.cufid = f.cufid 
			LEFT  JOIN sisindigena.orientadorturma ot ON ot.iusd = i.iusd 
			WHERE t.pflcod='".$dados['pflcod']."' ".(($dados['uncid'])?" AND i.uncid='".$dados['uncid']."'":"")." ".(($dados['picid'])?" AND i.picid='".$dados['picid']."'":"")." ".(($dados['turid'])?" AND ot.turid='".$dados['turid']."'":"")." ".(($dados['iustipoorientador'])?" AND i.iustipoorientador='".$dados['iustipoorientador']."'":"")." ".(($dados['tpejustificativaformadories'])?" AND t.tpejustificativaformadories IS NOT NULL":"")." ".(($dados['iusd'])?" AND i.iusd='".$dados['iusd']."'":"")." AND iusstatus='A' ORDER BY i.iusd";
	
	$identificacaousuario = $db->carregar($sql);
	
	if($identificacaousuario[0]) {

		foreach($identificacaousuario as $key => $iu) {
			
			$idusuarios[$key] = $iu;
			unset($telefones);
			$sql = "SELECT itetipo, itedddtel, itenumtel FROM sisindigena.identificacaotelefone WHERE iusd='".$iu['iusd']."'";
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
	
	$sql = "UPDATE seguranca.usuario_sistema SET suscod='A' WHERE usucpf='".$dados['usucpf']."' AND sisid='".SIS_INDIGENA."'";
	$db->executar($sql);
	
	$db->commit();
	
	$arrUsu = $db->pegaLinha("SELECT usunome, usuemail FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['usucpf'])."'");
	
	$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO SISINDÍGENA","email" => $arrUsu['usuemail']);
 	$destinatario = $arrUsu['usuemail'];
 	$usunome = $arrUsu['usunome'];
 	
 	$assunto = "Atualização de senha no SIMEC - MÓDULO SISINDÍGENA";
 	$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
	$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo SISINDÍGENA. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
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
	
	$al = array("alert"=>"Senha reiniciada com sucesso","location"=>"sisindigena.php?modulo=".$dados['modulo']."&acao=A&aba=".$dados['aba']);
	alertlocation($al);
	
	
}

function pegarDadosUsuarioPorCPF($dados) {
	global $db;
	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf='".$dados['cpf']."'";
	$usuemail = $db->pegaUm($sql);
	
	$sql = "SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf='".$dados['cpf']."' AND sisid='".SIS_INDIGENA."'";
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
	
	$iusagenciasugerida_atual = $db->pegaUm("SELECT iusagenciasugerida FROM sisindigena.identificacaousuario WHERE iusd='".$dados['iusd']."'");
	if($iusagenciasugerida_atual != substr($dados['iusagenciasugerida'],0,4)) {
		$sqlsgb = "cadastradosgb=FALSE,"; 
	}

	$sql = "UPDATE sisindigena.identificacaousuario SET
			iusdatanascimento = '".formata_data_sql($dados['iusdatanascimento'])."',
			iusnomemae		  = '".$dados['iusnomemae']."',
			iussexo 		  = '".$dados['iussexo']."',
			muncod		  	  = '".$dados['muncod_nascimento']."',
			eciid 		  	  = '".$dados['eciid']."',
			nacid		  	  = '".$dados['nacid']."',
			iusnomeconjuge	  = '".$dados['iusnomeconjuge']."',
			iusagenciasugerida = '".substr($dados['iusagenciasugerida'],0,4)."',
			iusagenciaend = '".substr($dados['iusagenciaend'],0,250)."',
			tvpid = '".$dados['tvpid']."',
			funid = '".$dados['funid']."',
			foeid = '".$dados['foeid']."',
			iusemailprincipal = '".$dados['iusemailprincipal']."',
			iusemailopcional=".(($dados['iusemailopcional'])?"'".$dados['iusemailopcional']."'":"NULL").",
			iusnaodesejosubstituirbolsa=".(($dados['iusnaodesejosubstituirbolsa']=='TRUE')?"TRUE":"FALSE").",
			{$sqlsgb}
			iustermocompromisso=TRUE
			WHERE iusd='".$dados['iusd']."'";
	
	$db->executar($sql);
	
	$erros = validarFormacao($dados);
	
	if($erros) {
		$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erros),"location"=>$dados['goto']);
		alertlocation($al);
	}
	

	$iufid = $db->pegaUm("SELECT iufid FROM sisindigena.identiusucursoformacao WHERE iusd='".$dados['iusd']."'");
	
	// controlando formação
	if($iufid) {
		
		$sql = "UPDATE sisindigena.identiusucursoformacao SET
		            cufid=".(($dados['cufid'])?"'".$dados['cufid']."'":"NULL").", 
		            iufdatainiformacao='".formata_data_sql($dados['iufdatainiformacao'])."', 
		            iufdatafimformacao=".(($dados['iufdatafimformacao'])?"'".formata_data_sql($dados['iufdatafimformacao'])."'":"NULL").", 
		            iufsituacaoformacao='".$dados['iufsituacaoformacao']."'
		        WHERE iufid='".$iufid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sisindigena.identiusucursoformacao(
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
	
	
	$itdid = $db->pegaUm("SELECT itdid FROM sisindigena.identusutipodocumento WHERE iusd='".$dados['iusd']."'");
	
	// controlando documento
	if($itdid) {
		
		$sql = "UPDATE sisindigena.identusutipodocumento SET
            	tdoid='".$dados['tdoid']."', itdufdoc='".$dados['itdufdoc']."', 
            	itdnumdoc='".$dados['itdnumdoc']."', itddataexp='".formata_data_sql($dados['itddataexp'])."', 
            	itdnoorgaoexp='".$dados['itdnoorgaoexp']."'		
		        WHERE itdid='".$itdid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sisindigena.identusutipodocumento(
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
	
	$ienid = $db->pegaUm("SELECT ienid FROM sisindigena.identificaoendereco WHERE iusd='".$dados['iusd']."'");
	
	// controlando endereço
	if($ienid) {
		
		$sql = "UPDATE sisindigena.identificaoendereco SET
            	muncod='".substr($dados['muncod_endereco'],0,7)."', ientipo='".$dados['ientipo']."', 
            	iencep='".str_replace(array("-"),array(""),$dados['iencep'])."', iencomplemento=".(($dados['iencomplemento'])?"'".$dados['iencomplemento']."'":"NULL").", 
            	iennumero=".((!is_null($dados['iennumero']) && is_numeric($dados['iennumero']))?"'".$dados['iennumero']."'":"NULL").", ienlogradouro='".$dados['ienlogradouro']."', 
            	ienbairro='".$dados['ienbairro']."' 		
		        WHERE ienid='".$ienid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sisindigena.identificaoendereco(
            	muncod, iusd, ientipo, iencep, iencomplemento, iennumero, 
            	iensatatus, ienlogradouro, ienbairro)
    			VALUES ('".substr($dados['muncod_endereco'],0,7)."', '".$dados['iusd']."', '".$dados['ientipo']."', '".str_replace(array("-"),array(""),$dados['iencep'])."', 
    					".(($dados['iencomplemento'])?"'".$dados['iencomplemento']."'":"NULL").", ".((!is_null($dados['iennumero']))?"'".str_replace(array(" "),array(""),$dados['iennumero'])."'":"NULL").", 'A', '".$dados['ienlogradouro']."', '".substr($dados['ienbairro'],0,60)."');";
		
		$db->executar($sql);
		
	}
	
	// controlando telefones
	$db->executar("DELETE FROM sisindigena.identificacaotelefone WHERE iusd='".$dados['iusd']."'");
	
	$tipos = array("R","T","C","F");
	
	foreach($tipos as $tipo) {
		
		$sql = "INSERT INTO sisindigena.identificacaotelefone(
            	iusd, itedddtel, itenumtel, itetipo, itestatus)
    			VALUES ('".$dados['iusd']."', ".(($dados['itedddtel'][$tipo])?"'".$dados['itedddtel'][$tipo]."'":"NULL").", ".(($dados['itenumtel'][$tipo])?"'".$dados['itenumtel'][$tipo]."'":"NULL").", '".$tipo."', 'A');";
		
		$db->executar($sql);
		
	}
	
	$sql = "INSERT INTO sisindigena.historicoidentificaousuario(
            iusd, hiudatainc, hiucpf, hiulog, hiustatus, hiutipo)
    		VALUES ('".$dados['iusd']."', NOW(), '".$_SESSION['usucpf']."', '".str_replace(array("'"),array(""),simec_json_encode($dados))."', 'A', 'atualizarDadosIdentificacaoUsuario');";
	$db->executar($sql);
	
	$sql = "UPDATE sisindigena.tipoperfil SET tpeatuacaoinicio=".(($dados['tpeatuacaoinicio_mes'] && $dados['tpeatuacaoinicio_ano'])?"'".$dados['tpeatuacaoinicio_ano']."-".$dados['tpeatuacaoinicio_mes']."-01'":"NULL").", 
										   tpeatuacaofim=".(($dados['tpeatuacaofim_mes'] && $dados['tpeatuacaofim_ano'])?"'".$dados['tpeatuacaofim_ano']."-".$dados['tpeatuacaofim_mes']."-01'":"NULL")." WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	$db->commit();
	
	sincronizarUsuariosSIMEC(array('cpf' => $dados['iuscpf']));
	
	$al = array("alert"=>$dados['mensagemalert'],"location"=>$dados['goto']);
	alertlocation($al);
	
}



function carregarOrientacao($endereco) {
	global $db;
	
	$sql = "SELECT oabdesc FROM sisindigena.abas a 
			INNER JOIN sisindigena.orientacaoaba o ON o.abaid = a.abaid 
			WHERE a.abaendereco='".$endereco."'";
	
	$orientacao = $db->pegaUm($sql);
	
	return (($orientacao)?$orientacao:"Orientação não foi cadastrada");
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

	$sql = "SELECT * FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd WHERE i.iusd='".$dados['iusdantigo']."'";
	$identificacaousuario_antigo = $db->pegaLinha($sql);
	
	if($identificacaousuario_antigo['pflcod']==PFL_PROFESSORALFABETIZADOR) {
		
		$pboid = $db->pegaUm("SELECT pboid FROM sisindigena.pagamentobolsista WHERE tpeid='".$identificacaousuario_antigo['tpeid']."'");
		
		if($pboid) {
			$al = array("alert"=>"Não é possível efetuar a substituição, pois o professor alfabetizador (".$identificacaousuario_antigo['iusnome'].") ja recebeu bolsa","location"=>$_SERVER['HTTP_REFERER']);
			alertlocation($al);
			
		}
		
	}
	
	if(!$identificacaousuario_antigo) {
		$al = array("alert"=>"Usuário a ser substituido não foi encontrado","location"=>$_SERVER['HTTP_REFERER']);
		alertlocation($al);
	}
	
	$sql = "SELECT COUNT(*) as t FROM sisindigena.mensario m 
			INNER JOIN sisindigena.mensarioavaliacoes ma ON ma.menid = m.menid
			INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO."
			WHERE m.iusd='".$identificacaousuario_antigo['iusd']."' and mavtotal>=7 AND d.esdid=".ESD_ENVIADO_MENSARIO;
	
	$is_apto = $db->pegaUm($sql);

	if($is_apto) {
		$al = array("alert"=>"O usuário (".$identificacaousuario_antigo['iusnome'].") não pode ser substituido pois se encontra APTO A RECER BOLSA(Avaliações positivas) em alguns períodos. Solicite ao Coordenador GERAL/ADJUNTO que acesse a aba Aprovar Equipe, e aprove sua bolsa. Após este procedimento, este usuário estará disponível para troca.","location"=>$_SERVER['HTTP_REFERER']);
		alertlocation($al);
	}
	
	
	if(!$identificacaousuario_antigo['uncid']) $identificacaousuario_antigo['uncid'] = $dados['uncid'];
	
	$sql = "SELECT i.iusd, t.tpeid, i.iusnome FROM sisindigena.identificacaousuario i LEFT JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd WHERE i.iuscpf='".str_replace(array(".","-"),array("",""),$dados['iuscpf_'])."'";
	$identificacaousuario_novo = $db->pegaLinha($sql);
	
	if($identificacaousuario_novo['tpeid']) {
		if(!$dados['noredirect']) {
	 		$al = array("alert"=>"Novo Usuário (".$identificacaousuario_novo['iusnome'].") ja possui atribuções no SISINDÍGENA, por isso não pode ser inserido","location"=>$_SERVER['HTTP_REFERER']);
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
     	$sql = "INSERT INTO sisindigena.identificacaousuario(
 	            picid, uncid, iuscpf, iusnome, iusemailprincipal, muncodatuacao,  
 	            iusdatainclusao, iusstatus, iusformacaoinicialorientador, iustipoprofessor, iustipoorientador)
 			    VALUES (".(($identificacaousuario_antigo['picid'])?"'".$identificacaousuario_antigo['picid']."'":"NULL").", ".(($identificacaousuario_antigo['uncid'])?"'".$identificacaousuario_antigo['uncid']."'":"NULL").", '".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."', '".$dados['iusnome_']."', '".$dados['iusemailprincipal_']."',".(($identificacaousuario_antigo['muncodatuacao'])?"'".$identificacaousuario_antigo['muncodatuacao']."'":"NULL").",  
 			            NOW(), 'A', ".(($identificacaousuario_antigo['iusformacaoinicialorientador'])?$identificacaousuario_antigo['iusformacaoinicialorientador']:"NULL").", 
 			            ".(($identificacaousuario_antigo['iustipoprofessor'])?"'".$identificacaousuario_antigo['iustipoprofessor']."'":"NULL").",
 			            ".(($identificacaousuario_antigo['iustipoorientador'])?"'".$identificacaousuario_antigo['iustipoorientador']."'":"NULL").") returning iusd;";
     	$identificacaousuario_novo['iusd'] = $db->pegaUm($sql);
	} else {
		$sql = "UPDATE sisindigena.identificacaousuario SET iusstatus='A', picid=".(($identificacaousuario_antigo['picid'])?"'".$identificacaousuario_antigo['picid']."'":"NULL").", uncid=".(($identificacaousuario_antigo['uncid'])?"'".$identificacaousuario_antigo['uncid']."'":"NULL").", 
														 iusformacaoinicialorientador=".(($identificacaousuario_antigo['iusformacaoinicialorientador'])?$identificacaousuario_antigo['iusformacaoinicialorientador']:"NULL").", 
														 iustipoprofessor=".(($identificacaousuario_antigo['iustipoprofessor'])?"'".$identificacaousuario_antigo['iustipoprofessor']."'":"NULL").",
														 iustipoorientador=".(($identificacaousuario_antigo['iustipoorientador'])?"'".$identificacaousuario_antigo['iustipoorientador']."'":"NULL")."
														 WHERE iusd='".$identificacaousuario_novo['iusd']."'";
		$db->executar($sql);
	}
	
	$sql = "DELETE FROM sisindigena.usuarioresponsabilidade WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."'";
	$db->executar($sql);
	
	$sql = "UPDATE sisindigena.usuarioresponsabilidade SET usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."', uncid=".(($identificacaousuario_antigo['uncid'])?"'".$identificacaousuario_antigo['uncid']."'":"NULL")." WHERE rpustatus='A' AND usucpf='".$identificacaousuario_antigo['usucpf']."' AND pflcod='".$identificacaousuario_antigo['pflcod']."'";
	$db->executar($sql);
	
	$sql = "UPDATE sisindigena.tipoperfil SET iusd='".$identificacaousuario_novo['iusd']."' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
	$db->executar($sql);
	
	$sql = "UPDATE sisindigena.turmas SET iusd='".$identificacaousuario_novo['iusd']."' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
	$db->executar($sql);
	
	$sql = "UPDATE sisindigena.orientadorturma SET iusd='".$identificacaousuario_novo['iusd']."' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
	$db->executar($sql);
	
	$sql = "UPDATE sisindigena.identificacaousuario SET iusstatus='I' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$identificacaousuario_antigo['iuscpf']."' AND pflcod='".$dados['pflcod_']."'";
	$db->executar($sql);
	
	// removendo avaliações não concluidas
	$sql = "SELECT m.menid FROM sisindigena.mensario m 
			INNER JOIN workflow.documento d ON d.docid = m.docid 
			WHERE iusd='".$identificacaousuario_antigo['iusd']."' AND d.esdid!='".ESD_APROVADO_MENSARIO."'";
	
	$menids = $db->carregarColuna($sql);
	
	if($menids) {
		
		$sql = "SELECT mavid FROM sisindigena.mensarioavaliacoes WHERE menid IN('".implode("','",$menids)."')";
		$mavids = $db->carregarColuna($sql);
		
		if($mavids) {
			$db->executar("DELETE FROM sisindigena.historicoreaberturanota WHERE mavid IN('".implode("','",$mavids)."')");
			$db->executar("DELETE FROM sisindigena.mensarioavaliacoes WHERE mavid IN('".implode("','",$mavids)."')");
		}
	}
	
	$sql = "INSERT INTO sisindigena.historicotrocausuario(iusdnovo, iusdantigo, pflcod, hstdata, usucpf, uncid)
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
				
				$sql = "SELECT * FROM sisindigena.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$cpf)."'";
 				$identificacaousuario = $db->pegaLinha($sql);

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
			    
		 		$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO SISINDÍGENA","email" => $identificacaousuario['iusemailprincipal']);
 				$destinatario = $identificacaousuario['iusemailprincipal'];
 				$usunome = $identificacaousuario['iusnome'];
 				$assunto = "Cadastro no SIMEC - MÓDULO SISINDÍGENA";
 				$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
		 		$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo SISINDÍGENA. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
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
		 		
			    $existe_sis = $db->pegaUm("select usucpf from seguranca.usuario_sistema where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and sisid='".SIS_INDIGENA."'");
			    	
			    if(!$existe_sis) {
			    		
			    	$sql = "INSERT INTO seguranca.usuario_sistema(
			         	    usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
			     			VALUES ('".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', ".SIS_INDIGENA.", 'A', NULL, NOW(), 'A');";
				    	
			     	$db->executar($sql);
				    	
			    } else {
		 	    	$sql = "UPDATE seguranca.usuario_sistema SET susstatus='A', suscod='".$dados['suscod']."' WHERE usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' AND sisid='".SIS_INDIGENA."'";
		 	    	$db->executar($sql);
			    }
			    
			    $sql = "INSERT INTO seguranca.historicousuario(htudsc, htudata, usucpf, sisid, suscod, usucpfadm)
    					VALUES ('Mudança realizada pela ferramenta de gerencia do SISINDÍGENA.', 
    							NOW(), 
    							'".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', 
    							'".$_SESSION['sisid']."', '".$dados['suscod']."', '".$_SESSION['usucpf']."');";
			    
	 	    	$db->executar($sql);
			    
			    $existe_pfl = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and pflcod='".$pflcod."'");
    	
			    if(!$existe_pfl) {
			    	$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', '".$pflcod."');";
			     	$db->executar($sql);
			    }

			    $rpuid = $db->pegaUm("select rpuid from sisindigena.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and pflcod='".$pflcod."' AND rpustatus='A'");
			    
			    if($dados['uncid']) {
	    
				    if(!$rpuid) {
				    	$sql = "INSERT INTO sisindigena.usuarioresponsabilidade(
				            		pflcod, usucpf, rpustatus, rpudata_inc, uncid)
				 			    VALUES ('".$pflcod."', '".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', 'A', NOW(), '".$dados['uncid']."');";
				    	
				    	$db->executar($sql);
				    } else {
				    	$sql = "UPDATE sisindigena.usuarioresponsabilidade SET uncid='".$dados['uncid']."' WHERE rpuid='".$rpuid."'";
				    	$db->executar($sql);
				    }
				    
			    }
			    
			    $rpuid = $db->pegaUm("select rpuid from sisindigena.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and pflcod='".$pflcod."' AND rpustatus='A'");
			    
			    if($identificacaousuario['picid']) {
			    	
			    	$sql = "SELECT * FROM sisindigena.nucleouniversidade WHERE picid='".$identificacaousuario['picid']."'";
			    	$pactoidadecerta = $db->pegaLinha($sql);
			    	
			    	$cl  = "picid='".$pactoidadecerta['picid']."'";
		    		$ur  = "picid";
		    		$ur2 = "'".$pactoidadecerta['picid']."'";
	    
				    if(!$rpuid) {
				    	$sql = "INSERT INTO sisindigena.usuarioresponsabilidade(
				            		pflcod, usucpf, rpustatus, rpudata_inc, {$ur})
				 			    VALUES ('".$pflcod."', '".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', 'A', NOW(), {$ur2});";
				    	
				    	$db->executar($sql);
				    } else {
				    	$sql = "UPDATE sisindigena.usuarioresponsabilidade SET {$cl} WHERE rpuid='".$rpuid."'";
				    	$db->executar($sql);
				    }
				    
			    }
			    
    			$db->commit();
			}
			
		}
		
		
	}

    

	if(!$dados['noredirect']) {    
	 	$al = array("alert"=>"Gerenciamento executado com sucesso","location"=>$_SERVER['REQUEST_URI']);
	 	alertlocation($al);
	}
	
}

function verificarFormacaoCompleta($dados) {
	global $db;
	$sql = "SELECT foecompleto FROM sisindigena.formacaoescolaridade WHERE foeid='".$dados['foeid']."'";
	$foecompleto = $db->pegaUm($sql);
	echo (($foecompleto=="t")?"T":"");
	echo (($foecompleto=="f")?"F":"");
}

function listarCursosFormacao($dados) {
	global $db;
	$sql = "SELECT cufid as codigo, cufcursodesc as descricao FROM sisindigena.cursoformacao WHERE cufstatus='A' AND cufcodareageral='".$dados['cufcodareageral']."' ORDER BY cufcursodesc";
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
    $file = new FilesSimec( "identificacaousuarioanexo", $campos, "sisindigena" );
    $file->setUpload( NULL, "arquivo" );
    
	$al = array("alert"=>"Documento de Designação gravada com sucesso","location"=>$dados['goto']);
	alertlocation($al);
    
	
}

function downloadDocumentoDesignacao($dados) {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec( "identificacaousuarioanexo", NULL, "sisindigena" );
    $file->getDownloadArquivo( $dados['arqid'] );
}

function removerDocumentoDesignacao($dados) {
	global $db;
	$sql = "DELETE FROM sisindigena.identificacaousuarioanexo WHERE iuaid='".$dados['iuaid']."'";
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
    $file = new FilesSimec( "documentoatividade", NULL, "sisindigena" );
    $file->getDownloadArquivo( $dados['arqid'] );
}

function removerAnexoPortaria($dados) {
	global $db;
	$sql = "DELETE FROM sisindigena.portarianomeacao WHERE ponid='".$dados['ponid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Anexo excluído com sucesso","location"=>$_SERVER['HTTP_REFERER']);
	alertlocation($al);
	
	
}

function carregarDadosTurma($dados) {
	global $db;
	$sql = "SELECT * FROM sisindigena.turmas t
			LEFT JOIN sisindigena.identificacaousuario i ON i.iusd = t.iusd 
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
		$sql = "SELECT '<center>".((!$dados['consulta'])?"<img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirAlunoTurma('||i.iusd||');\">":"")." ".(($dados['formacaoinicial'])?"'|| CASE WHEN SUBSTR(i.iuscpf,1,3)!='SIS' THEN '<input type=radio name=\"iusd['||i.iusd||']\" value=\"TRUE\" '||CASE WHEN i.iusformacaoinicialorientador=true THEN 'checked' ELSE '' END||'> Presente <input type=radio name=\"iusd['||i.iusd||']\" value=\"FALSE\" '||CASE WHEN i.iusformacaoinicialorientador=false THEN 'checked' ELSE '' END||'> Ausente' ELSE '' END ||'":"")."</center>' as acao, i.iuscpf, i.iusnome, i.iusemailprincipal, m.estuf || ' / ' || m.mundescricao as municipio, CASE WHEN pp.muncod IS NULL THEN 'Estadual' ELSE 'Municipal' END as esfera, tu.turdesc FROM sisindigena.orientadorturma ot 
				INNER JOIN sisindigena.identificacaousuario i ON i.iusd = ot.iusd 
				INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN sisindigena.turmas tu ON tu.turid = ot.turid 
				LEFT JOIN sisindigena.pactoidadecerta pp ON pp.picid = i.picid 
				LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
				WHERE ot.turid='".$dados['turid']."' ORDER BY i.iusnome";
		
		$cabecalho = array("&nbsp;","CPF","Nome","Email","UF/Município","Esfera","Turma");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	} else {
		echo "<p>Nenhuma turma foi selecionada</p>";
	}
}

function criarMensario($dados) {
	global $db;
	$sql = "SELECT m.menid, d.docid, d.esdid FROM sisindigena.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid WHERE m.iusd='".$dados['iusd']."' AND fpbid='".$dados['fpbid']."'";
	$mensario = $db->pegaLinha($sql);
	
	$menid = $mensario['menid'];
	$docid = $mensario['docid'];
	$esdid = $mensario['esdid'];
	
	if(!$menid) {
		
		$arrUs    = $db->pegaLinha("SELECT i.iusnome, p.pfldsc FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod WHERE i.iusd='".$dados['iusd']."'");
		$iusnome  = $arrUs['iusnome'];
		$pfldsc   = $arrUs['pfldsc'];
		
		$referencia = $db->pegaUm("SELECT fpbmesreferencia || ' / ' || fpbanoreferencia as descricao FROM sisindigena.folhapagamento WHERE fpbid='".$dados['fpbid']."'");
		
		$docid = wf_cadastrarDocumento( TPD_FLUXOMENSARIO, 'Mensário : '.$iusnome.' - '.$pfldsc.' Ref.'.$referencia );
		$esdid = ESD_EM_ABERTO_MENSARIO;
		
		$sql = "INSERT INTO sisindigena.mensario(
            	iusd, fpbid, docid)
    			VALUES ('".$dados['iusd']."', '".$dados['fpbid']."', '".$docid."') RETURNING menid;";
		
		$menid = $db->pegaUm($sql);
		$db->commit();
	}
	
	return array("memid"=>$menid,"docid"=>$docid,"esdid"=>$esdid);
	
}

function montaComboAvaliacao($dados) {
	global $OPT_AV;
	
	$combo .= '<select '.(($dados['consulta'])?'disabled':'').' name="'.$dados['tipo'].'[\'||foo.iusd||\']" class="CampoEstilo obrigatorio" style="width: auto" onchange="calcularNotaFinal(\'||foo.iusd||\')" id="'.$dados['tipo'].'_\'||foo.iusd||\'" \'||CASE WHEN (SELECT esdid FROM workflow.documento WHERE docid=foo.docid) IN('.ESD_APROVADO_MENSARIO.') THEN \'disabled\' ELSE \'\' END ||\'>';
	
	if($OPT_AV[$dados['tipo']]) {
		$combo .= '<option value="">Selecione</option>';
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
				
				$sql = "SELECT mavid FROM sisindigena.mensarioavaliacoes WHERE menid='".$dadosmensario['memid']."' AND iusdavaliador='".$dados['iusdavaliador']."'";
				$mavid = $db->pegaUm($sql);
				
				if($mavid) {
					
					if(is_numeric($dados['frequencia'][$iusd]) || is_numeric($dados['atividadesrealizadas'][$iusd])) {
					
						$sql = "UPDATE sisindigena.mensarioavaliacoes SET mavfrequencia=".((is_numeric($dados['frequencia'][$iusd]))?"'".$dados['frequencia'][$iusd]."'":"NULL").", 
															 		   mavatividadesrealizadas=".((is_numeric($dados['atividadesrealizadas'][$iusd]))?"'".$dados['atividadesrealizadas'][$iusd]."'":"NULL").", 
															 		   mavmonitoramento=".((is_numeric($dados['monitoramento'][$iusd]))?"'".$dados['monitoramento'][$iusd]."'":"NULL").", 
															 		   mavtotal=".((is_numeric(($dados['total'][$iusd])))?"'".($dados['total'][$iusd])."'":"NULL")." WHERE mavid='".$mavid."'";
						$db->executar($sql);
					
					}
					
				} else {
					
					if(is_numeric($dados['frequencia'][$iusd]) || is_numeric($dados['atividadesrealizadas'][$iusd])) {
					
						$sql = "INSERT INTO sisindigena.mensarioavaliacoes(
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
				
				$sql = "UPDATE sisindigena.mensarioavaliacoes ma SET mavtotal=foo.total FROM (
						SELECT * FROM (
						SELECT 
						m.menid,
						mavid,
						mavtotal,
						(COALESCE((mavfrequencia*fatfrequencia),0) + COALESCE((mavatividadesrealizadas*fatatividadesrealizadas),0) + COALESCE(mavmonitoramento,0)) as total
						FROM sisindigena.mensarioavaliacoes ma 
						INNER JOIN sisindigena.mensario m ON m.menid = ma.menid 
						INNER JOIN sisindigena.identificacaousuario u ON u.iusd = m.iusd 
						INNER JOIN sisindigena.tipoperfil t ON t.iusd = u.iusd 
						INNER JOIN sisindigena.fatoresdeavaliacao f ON f.fatpflcodavaliado = t.pflcod 
						) fee
						WHERE fee.mavtotal != total
						) foo 
						WHERE ma.menid = foo.menid and ma.menid='".$dadosmensario['memid']."'";
				
				$db->executar($sql);
				
				if($mavid && $dados['cpfresponsavel'][$iusd] && $dados['mavdsc'][$iusd]) {
					$sql = "INSERT INTO sisindigena.historicoreaberturanota(
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
	
	$al = array("alert"=>"Avaliações gravadas com sucesso","location"=>$dados['goto']);
	alertlocation($al);
	
	
	
}



function condicaoEnviarMensario($fpbid,$pflcod=null) {
	global $db;
	
	if($pflcod == PFL_SUPERVISORIES) {
		
		$sql_tot = sqlAvaliacaoSupervisor(array('picid'=>$_SESSION['sisindigena']['supervisories']['picid'],'iusd'=>$_SESSION['sisindigena']['supervisories']['iusd'],'fpbid'=>$fpbid));
		
		$sql = "SELECT COUNT(*) FROM ({$sql_tot}) ff";
		$navals = $db->pegaUm($sql);
		
		if(!$navals) {
			return true;
		}
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sisindigena.mensario me 
							INNER JOIN sisindigena.mensarioavaliacoes ma ON ma.menid = me.menid
							WHERE me.fpbid='".$fpbid."' AND ma.iusdavaliador='".$_SESSION['sisindigena']['supervisories']['iusd']."' AND ma.mavtotal IS NOT NULL");
		
		if(!$tot) {
			return 'É necessário avaliar um membro';
		}
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORADJUNTOIES) {
		
		$sql_tot = sqlAvaliacaoCoordenadorAdjuntoIES(array('picid'=>$_SESSION['sisindigena']['coordenadoradjuntoies']['picid'],'iusd'=>$_SESSION['sisindigena']['coordenadoradjuntoies']['iusd'],'fpbid'=>$fpbid));
		
		$sql = "SELECT COUNT(*) FROM ({$sql_tot}) ff";
		$navals = $db->pegaUm($sql);
		
		if(!$navals) {
			return true;
		}
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sisindigena.mensario me 
							INNER JOIN sisindigena.mensarioavaliacoes ma ON ma.menid = me.menid
							WHERE me.fpbid='".$fpbid."' AND ma.iusdavaliador='".$_SESSION['sisindigena']['coordenadoradjuntoies']['iusd']."' AND ma.mavtotal IS NOT NULL");
		
		if(!$tot) {
			return 'É necessário avaliar um membro';
		}
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORIES) {
		
		$sql_tot = sqlAvaliacaoCoordenadorIES(array('uncid'=>$_SESSION['sisindigena']['universidade']['uncid'],'iusd'=>$_SESSION['sisindigena']['universidade']['iusd'],'fpbid'=>$fpbid));
		
		$sql = "SELECT COUNT(*) FROM ({$sql_tot}) ff";
		$navals = $db->pegaUm($sql);
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sisindigena.mensario me 
							INNER JOIN sisindigena.mensarioavaliacoes ma ON ma.menid = me.menid
							INNER JOIN sisindigena.identificacaousuario i ON i.iusd = me.iusd AND i.iusstatus='A'
							INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd
							WHERE me.fpbid='".$fpbid."' AND ma.iusdavaliador='".$_SESSION['sisindigena']['universidade']['iusd']."' AND ma.mavtotal IS NOT NULL");
		
		if($tot != $navals) {
			return 'É necessário avaliar todos Coordenadoes Adjuntos IES';
		}
		
		return true;
	
	}
	
	
	return true;

}

function posEnviarMensario($fpbid, $pflcod=null) {
	global $db;
	
	if($pflcod == PFL_ORIENTADORESTUDO) {
		
		$sql = "SELECT i.iusnome, me.docid, ma.mavtotal FROM sisindigena.mensario me 
				INNER JOIN workflow.documento dc ON dc.docid = me.docid AND dc.tpdid=".TPD_FLUXOMENSARIO." 
				INNER JOIN sisindigena.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sisindigena.identificacaousuario i ON i.iusd = me.iusd 
				INNER JOIN sisindigena.orientadorturma ot ON ot.iusd = me.iusd 
				INNER JOIN sisindigena.turmas tt ON tt.turid = ot.turid 		
				WHERE tt.iusd='".$_SESSION['sisindigena']['orientadorestudo']['iusd']."' AND dc.esdid='".ESD_EM_ABERTO_MENSARIO."' AND ma.iusdavaliador='".$_SESSION['sisindigena']['orientadorestudo']['iusd']."' AND me.fpbid='".$fpbid."'";
		
		
		$arrMensario = $db->carregar($sql);
		
		if($arrMensario[0]) {
			foreach($arrMensario as $mensario) {
				wf_alterarEstado( $mensario['docid'], AED_ENVIAR_MENSARIO, '', array('fpbid'=>$fpbid));
			}
		}
		
		
		$sql = "UPDATE sisindigena.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sisindigena.mensario me 
				INNER JOIN sisindigena.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sisindigena.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sisindigena.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sisindigena']['orientadorestudo']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_FORMADORIES) {
		
		$sql = "UPDATE sisindigena.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sisindigena.mensario me 
				INNER JOIN sisindigena.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sisindigena.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sisindigena.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sisindigena']['formadories']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_SUPERVISORIES) {
		
		$sql = "UPDATE sisindigena.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sisindigena.mensario me 
				INNER JOIN sisindigena.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sisindigena.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sisindigena.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sisindigena']['supervisories']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORADJUNTOIES) {

		
		$sql = "UPDATE sisindigena.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sisindigena.mensario me 
				INNER JOIN sisindigena.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sisindigena.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sisindigena.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sisindigena']['coordenadoradjuntoies']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORIES) {
		
		$sql = "UPDATE sisindigena.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sisindigena.mensario me 
				INNER JOIN sisindigena.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sisindigena.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sisindigena.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sisindigena']['universidade']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORLOCAL) {
		
		$sql = "UPDATE sisindigena.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sisindigena.mensario me 
				INNER JOIN sisindigena.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sisindigena.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sisindigena.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sisindigena']['coordenadorlocal'][$_SESSION['sisindigena']['esfera']]['iusd']."' AND me.fpbid='".$fpbid."'
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
	
	$sql = "SELECT * FROM sisindigena.fatoresdeavaliacao WHERE fatid='".$dados['fatid']."'";
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
	$campotl = '<input '.(($dados['consulta'])?'disabled':'').' readonly="" style="text-align:right;border-left:#888888 3px solid;color:#808080;" type="text" id="total_\'||foo.iusd||\'" name="total[\'||foo.iusd||\']" size="6" maxlength="6" value="\'||CASE WHEN me.mavtotal IS NULL THEN \'\' ELSE me.mavtotal::character varying(10) END||\'" class="CampoEstilo">';
	$campomt = '<input readonly="" style="text-align:right;border-left:#888888 3px solid;color:#808080;" type="hidden" id="monitoramento_\'||foo.iusd||\'" name="monitoramento[\'||foo.iusd||\']" size="6" maxlength="6" value="\'||CASE WHEN foo.mon=\'TRUE\' THEN fat.fatmonitoramento ELSE \'0\' END||\'" class="CampoEstilo">\'||CASE WHEN foo.mon=\'TRUE\' THEN \'<center><font style=color:blue;>Sim</font></center>\' ELSE \'<center><font style=color:red;>Não</font></center>\' END||\' ';
	
	$perfis = pegaPerfilGeral();
	if($db->testa_superuser() || in_array(PFL_EQUIPEMEC,$perfis) ||  in_array(PFL_ADMINISTRADOR,$perfis)) {
		$imgexcluir = "<img src=\"../imagens/excluir.gif\" onmouseover=\"return escape(\'Excluir avaliação\');\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirAvaliacao(\''||coalesce(me.mavid,0)||'\');\">"; 
	}

	$sql = "SELECT DISTINCT CASE WHEN foo.mais = '' THEN '' ELSE '<img align=\"absmiddle\" style=\"cursor:pointer\" src=\"../imagens/mais.gif\" title=\"mais\" onclick=\"exibirAvaliacaoSub(\''||foo.mais||'\', this)\"> ' END, '<span style=\"white-space: nowrap\"><img align=\"absmiddle\" src=\"../imagens/'|| CASE WHEN (SELECT esdid FROM workflow.documento WHERE docid=foo.docid) IN(".ESD_APROVADO_MENSARIO.") THEN 'money.gif' ELSE CASE WHEN me.mavtotal IS NULL THEN 'valida5.gif' WHEN me.mavtotal < 7 THEN 'valida6.gif' ELSE 'valida4.gif' END END ||'\" id=\"img_'||foo.iusd||'\"> <img align=\"absmiddle\" src=\"../imagens/ajuda.png\" width=\"16\" height=\"16\" style=\"cursor:pointer;\" onclick=\"verAjuda(\''||fat.fatid||'\');\"> 
					'||CASE WHEN (SELECT esdid FROM workflow.documento WHERE docid=foo.docid) IN(".ESD_ENVIADO_MENSARIO.",".ESD_EM_ABERTO_MENSARIO.") OR (SELECT esdid FROM workflow.documento WHERE docid=foo.docid) IS NULL THEN '{$imgexcluir} ".(($dados['consulta'] || $dados['esdid']==ESD_EM_ABERTO_MENSARIO)?"":"<img align=\"absmiddle\" src=\"../imagens/send.png\" onmouseover=\"return escape(\'Reavaliar usuário\');\" width=\"16\" height=\"16\" style=\"cursor:pointer;\" id=\"corrigir_'||foo.iusd||'\" onclick=\"mostrarCorrecaoAprovado(\''||foo.iusd||'\');\">")."</span>' 
							ELSE '' END||'".(($dados['consulta'])?"":"<input type=\"hidden\" name=\"iusd_avaliados[]\" value=\"'||foo.iusd||'\"><input type=\"hidden\" id=\"pfreq_'||foo.iusd||'\" value=\"'||COALESCE(fat.fatfrequencia,0)||'\"><input type=\"hidden\" id=\"pativ_'||foo.iusd||'\" value=\"'||COALESCE(fat.fatatividadesrealizadas,0)||'\">")."' as acao, 
				   replace(to_char(foo.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, 
				   foo.iusnome, 
				   foo.iusemailprincipal, 
				   foo.pfldsc,
				   CASE WHEN fat.fatfrequencia 			 IS NULL THEN '<center><span style=color:red;>Não se aplica</center>' ELSE '$combofr' END as frequencia,
				   CASE WHEN fat.fatatividadesrealizadas IS NULL THEN '<center><span style=color:red;>Não se aplica</center>' ELSE '$comboat' END as atividades,
				   CASE WHEN fat.fatmonitoramento 		 IS NULL THEN '<center><span style=color:red;>Não se aplica</center>' ELSE '$campomt' END as monitoramento,
				   '$campotl' as total 
			FROM (
			(
			
			{$dados['sql']}
			
			)

			) foo 
			INNER JOIN sisindigena.fatoresdeavaliacao fat ON fat.fatpflcodavaliado = foo.pflcod 
			LEFT JOIN (SELECT iusd, mavfrequencia, mavatividadesrealizadas, mavmonitoramento, mavtotal, ma.mavid FROM sisindigena.mensario m INNER JOIN sisindigena.mensarioavaliacoes ma ON ma.menid = m.menid WHERE m.fpbid='".$dados['fpbid']."' AND iusdavaliador='".$dados['iusd']."') me ON me.iusd = foo.iusd 
			".(($where)?"WHERE ".implode(" AND ",$where):"")." 
			ORDER BY foo.iusnome   
			";
	
	$cabecalho = array("&nbsp;","&nbsp;","CPF","Nome","E-mail","Perfil","Frequência","Atividades Realizadas","Monitoramento","Nota Final");
	$db->monta_lista_simples($sql,$cabecalho,5000,10,'N','100%',$par2);

}

function carregarAvaliacaoEquipeSub($dados) {
	global $db;
	$dados['fpbid'] = str_replace(array("#"),array(""),$dados['fpbid']);
	$sql_avaliacao = $dados['functionavaliacao']($dados);
	carregarAvaliacaoEquipe(array("sql"=>$sql_avaliacao,"fpbid"=>$dados['fpbid'],"iusd"=>$dados['iusd'],"consulta"=>$dados['consulta']));

}

function inserirDadosLog($dados) {
	global $db;
	
	$sql = "INSERT INTO log_historico.logsgb_sisindigena(
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
		FROM sisindigena.identificacaousuario i 
		LEFT JOIN territorios.municipio m ON m.muncod = i.muncod 
		LEFT JOIN sisindigena.identificaoendereco ie ON ie.iusd = i.iusd 
		LEFT JOIN territorios.municipio m2 ON m2.muncod = ie.muncod 
		LEFT JOIN sisindigena.identusutipodocumento it ON it.iusd = i.iusd 
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
   	
    	
    	inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcpf'=>$dadosusuario['iuscpf'],'logservico'=>'lerDadosBolsista'));
    	
		
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
		
    	inserirDadosLog(array('logerro'=>$logerro_gravarDadosBolsista,'logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcpf'=>$dadosusuario['iuscpf'],'logservico'=>'gravarDadosBolsista'));
    	
    	$sql = "UPDATE sisindigena.identificacaousuario SET cadastradosgb=".(($logerro_gravarDadosBolsista=='TRUE')?'FALSE':'TRUE')." WHERE iusd='".$dados['iusd']."'";
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
			FROM sisindigena.universidadecadastro u 
			INNER JOIN sisindigena.universidade un ON un.uniid = u.uniid  
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
    	
   	$sql = "UPDATE sisindigena.universidadecadastro SET cadastrosgb=".(($logerro_gravaDadosEntidade=='TRUE')?'FALSE':'TRUE')." WHERE uncid='".$dados['uncid']."'";
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
	
	$sql = "DELETE FROM sisindigena.respostasavaliacaocomplementar WHERE iusdavaliador='".$dados['iusdavaliador']."' AND fpbid='".$dados['fpbid']."'";
	$db->executar($sql);
	
	if($dados['icc']) {
		foreach($dados['icc'] as $iacid => $iccid) {
			if($iccid) {
				$racid = $db->pegaUm("SELECT racid FROM sisindigena.respostasavaliacaocomplementar WHERE iusdavaliador='".$dados['iusdavaliador']."' AND iacid='".$iacid."' AND fpbid='".$dados['fpbid']."'");
				
				if($racid) {
					
					$sql = "UPDATE sisindigena.respostasavaliacaocomplementar SET 
							iusdavaliador='".$dados['iusdavaliador']."', 
							iusdavaliado=".(($dados['iusavaliado'][$iacid])?"'".$dados['iusavaliado'][$iacid]."'":"NULL").", 
							iacid='".$iacid."', 
							iccid='".$iccid."', 
							fpbid='".$dados['fpbid']."' 
							WHERE racid='".$racid."'";
					
					$db->executar($sql);
					
				} else {
				
					$sql = "INSERT INTO sisindigena.respostasavaliacaocomplementar(
				            iusdavaliador, iusdavaliado, iacid, iccid, fpbid)
				    		VALUES ('".$dados['iusdavaliador']."', ".(($dados['iusavaliado'][$iacid])?"'".$dados['iusavaliado'][$iacid]."'":"NULL").", '".$iacid."', '".$iccid."', '".$dados['fpbid']."');";
					
					$db->executar($sql);
				
				}
			}			
		}
	}
	
	$db->commit();
	
	$al = array("alert"=>"Avaliações Complementares gravadas com sucesso","location"=>$dados['goto']);
	alertlocation($al);
	

}

function verificarTermoCompromisso($dados) {
	global $db;
	
	if($db->testa_superuser()) return true;
	
	// se for equipe do mec, não precisa verificar termo
	if($dados['pflcod'] == PFL_EQUIPEMEC) return true;
	
	// verificando se coordenador local aceitou o termo de compromisso
	$termo = carregarDadosIdentificacaoUsuario(array("iusd"=>$dados['iusd'],"pflcod"=>$dados['pflcod']));
	
	if($termo) {
		$termo = current($termo);
	}
	
	if($termo['iustermocompromisso']!="t") {
		$al = array("alert"=>"Por favor preencha todos os campos obrigatórios da tela “Dados”.","location"=>"sisindigena.php?modulo=principal/{$dados['sis']}/{$dados['sis']}&acao=A&aba=dados");
		alertlocation($al);
	}
}

function gerarVersaoProjetoUniversidade($dados) {
	global $db;
	include_once '_funcoes_universidade.php';
	ob_start();
	$versao_html = true;
	if($dados['uncid']) carregarCoordenadorIES(array('uncid'=>$dados['uncid']));
	include APPRAIZ.'sisindigena/modulos/principal/universidade/visualizacao_projeto.inc';
	$html = ob_get_contents();
	ob_clean();
		
	$sql = "INSERT INTO sisindigena.versoesprojetouniversidade(
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
			FROM sisindigena.historicotrocausuario h 
			LEFT JOIN sisindigena.identificacaousuario i ON i.iusd = h.iusdnovo 
			LEFT JOIN sisindigena.identificacaousuario i2 ON i2.iusd = h.iusdantigo 
			LEFT JOIN seguranca.perfil p ON p.pflcod = h.pflcod 
			LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf 
			LEFT JOIN sisindigena.turmas t1 ON t1.turid = h.turidantigo 
			LEFT JOIN sisindigena.identificacaousuario i3 ON i3.iusd = t1.iusd 
			LEFT JOIN sisindigena.turmas t2 ON t2.turid = h.turidnovo 
			LEFT JOIN sisindigena.identificacaousuario i4 ON i4.iusd = t2.iusd
			WHERE h.uncid='".$dados['uncid']."' ORDER BY h.hstdata";
	
	$mudancas = $db->carregar($sql);
	
	return $mudancas;

}



function verificarValidacaoVisualizacaoProjeto($dados) {
	
	$resp = validarOrientadoresCadastradosTurma($dados); 
	
	if($resp!==true) {
		$al = array("alert"=>"Há ".$resp." orientadores que não foram vinculados a nenhuma turma.","location"=>"sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=turmas");
		alertlocation($al);
	}
	
	$msgs = validarFormadoresTurmas($dados);
	if($msgs) {
		$al = array("alert"=>"Há ".count($msgs)." formador(es) que não foi/ foram vinculado(s) a nenhuma turma.","location"=>"sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=turmas");
		alertlocation($al);
	}
	
	$tso = validarTurmasSemOrientadores($dados);
	if($tso) {
		$al = array("alert"=>"Há ".count($tso)." turma(s) sem Orientadores de Estudo vinculados.","location"=>"sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=turmas");
		alertlocation($al);
	}
	
	$tso = validarTurmasSemOrientadores($dados);
	if($tso) {
		$al = array("alert"=>"Há ".count($tso)." turma(s) sem Orientadores de Estudo vinculados.","location"=>"sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=turmas");
		alertlocation($al);
	}
	
	$maxCoordenadorAjunto = numeroMaximoCoordenadorAjuntoIES($dados);
	$numCoordenadorAjunto = numeroCoordenadorAdjuntoIES($dados);
	if($numCoordenadorAjunto>$maxCoordenadorAjunto) {
		$al = array("alert"=>"Há mais Coordenadores Adjuntos do que o limite permitido pelo MEC. Reveja a composição da Equipe da IES.","location"=>"sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=turmas");
		alertlocation($al);
	}
	
	$nturmas =  numeroMaximoTurmas($dados);
	if(!$nturmas) {
		$al = array("alert"=>"Não existem turmas cadastradas. Reveja a Estrutura da Formação.","location"=>"sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=turmas");
		alertlocation($al);
	}
	
	$numFormadorIes = numeroFormadorIES($dados);
	if($numFormadorIes>$nturmas) {
		$al = array("alert"=>"Há mais formadores do que o número de turmas estimado e justificado. Reveja a composição da Equipe da IES.","location"=>"sisindigena.php?modulo=principal/universidade/universidade&acao=A&aba=turmas");
		alertlocation($al);
	}
	
}

function processarPagamentoBolsistaSGB($dados) {
	global $db;
	
	$sql = "SELECT * FROM sisindigena.pagamentobolsista WHERE pboid='".$dados->id."'";
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
			
				$novaparcela = $db->pegaUm("SELECT (rfuparcela+1) as novaparcela FROM sisindigena.folhapagamentouniversidade f 
							 				INNER JOIN sisindigena.universidadecadastro u ON u.uncid = f.uncid 
							 				WHERE u.uniid='".$pagamentobolsista['uniid']."' AND f.fpbid='".$pagamentobolsista['fpbid']."'");
			}
			
			$sql = "UPDATE sisindigena.pagamentobolsista SET remid=null, pboparcela='".$novaparcela."' WHERE pboid='".$pagamentobolsista['pboid']."'";
			$db->executar($sql);
			$db->commit();
			
		} else {
			echo wf_alterarEstado( $pagamentobolsista['docid'], AED_RECUSAR_PAGAMENTO, $cmddsc = 'Erro retornado pelo FNDE: '.$dados->situacao->codigo.' / '.$dados->situacao->descricao, array());
			$sql = "UPDATE sisindigena.pagamentobolsista SET remid=null WHERE pboid='".$pagamentobolsista['pboid']."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	
}

function consultarDetalhesAvaliacoes($dados) {
	global $db;
  
	$sql = "SELECT	uni.unisigla || ' - ' || uni.uninome as universidade, 
					per.pfldsc, 
					doc.docid, 
					esd.esddsc, 
					m.fpbid, 
					m.iusd, 
					m.menid, 
					i.iusnome, 
					me.mesdsc||'/'||fa.fpbanoreferencia as periodo, 
					f.fatfrequencia, 
					f.fatatividadesrealizadas, 
					f.fatmonitoramento 
			FROM sisindigena.mensario m 
			INNER JOIN sisindigena.identificacaousuario i ON i.iusd = m.iusd 
			LEFT JOIN sisindigena.universidadecadastro unc ON unc.uncid = i.uncid 
			LEFT JOIN sisindigena.universidade uni ON uni.uniid = unc.uniid 
			LEFT JOIN sisindigena.nucleouniversidade ptc ON ptc.picid = i.picid 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil per ON per.pflcod = t.pflcod
			INNER JOIN sisindigena.fatoresdeavaliacao f ON f.fatpflcodavaliado = t.pflcod
			INNER JOIN sisindigena.folhapagamento fa ON fa.fpbid = m.fpbid 
			INNER JOIN public.meses me ON me.mescod::integer = fa.fpbmesreferencia
			INNER JOIN workflow.documento doc ON doc.docid = m.docid AND doc.tpdid=".TPD_FLUXOMENSARIO."
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE menid='".$dados['menid']."'";
	
	$mensario = $db->pegaLinha($sql);
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	
	echo "<tr>
				<td class=SubTituloDireita width=25%>Avaliado:</td>
				<td>".$mensario['iusnome']."</td>
		  </tr>";
	
	echo "<tr>
				<td class=SubTituloDireita width=25%>Perfil:</td>
				<td>".$mensario['pfldsc']."</td>
		  </tr>";
	
	if($mensario['universidade']) 
			echo "<tr>
						<td class=SubTituloDireita width=25%>Universidade:</td>
						<td>".$mensario['universidade']."</td>
				  </tr>";
	
	echo "<tr>
				<td class=SubTituloDireita>Período:</td>
				<td>".$mensario['periodo']."</td>
		  </tr>";
	
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
		    FROM sisindigena.mensarioavaliacoes m
			INNER JOIN sisindigena.identificacaousuario i ON i.iusd = m.iusdavaliador 
			LEFT JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
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
	
		$sql_atv_com = "SELECT i.iusnome, p.pfldsc, ia.iacdsc, ic.iccdsc, ic.iccvalor FROM sisindigena.respostasavaliacaocomplementar r 
				INNER JOIN sisindigena.identificacaousuario i ON i.iusd = r.iusdavaliador 
				INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				INNER JOIN sisindigena.itensavaliacaocomplementar ia ON ia.iacid = r.iacid 
				INNER JOIN sisindigena.itensavaliacaocomplementarcriterio ic ON ic.iccid = r.iccid 
				WHERE r.iusdavaliado='".$mensario['iusd']."' AND r.fpbid='".$mensario['fpbid']."' ORDER BY ia.iacdsc, i.iusnome";
		
		$existe = $db->pegaUm("SELECT count(*) FROM (".$sql_atv_com.") foo");
		
		$sql = "(
				{$sql_atv_com}
				) UNION ALL (
				SELECT '', '', '', CASE WHEN AVG(ic.iccvalor) > 0 THEN 'Média' ELSE '<span style=color:red;>Não existem avaliações complementares</span>' END as l, AVG(ic.iccvalor) as media FROM sisindigena.respostasavaliacaocomplementar r 
				INNER JOIN sisindigena.identificacaousuario i ON i.iusd = r.iusdavaliador 
				INNER JOIN sisindigena.itensavaliacaocomplementar ia ON ia.iacid = r.iacid 
				INNER JOIN sisindigena.itensavaliacaocomplementarcriterio ic ON ic.iccid = r.iccid 
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
			
			$sql = "SELECT * FROM sisindigena.mensario m 
					INNER JOIN sisindigena.tipoperfil t ON t.iusd = m.iusd 
					INNER JOIN workflow.documento d ON d.docid = m.docid 
					WHERE menid='".$menid."'";
			
			$arrMensario = $db->pegaLinha($sql);
			
			$men_aberto = array(PFL_PROFESSORALFABETIZADOR, PFL_PESQUISADOR, PFL_CONTEUDISTA, PFL_ORIENTADORESTUDO, PFL_FORMADORIES, PFL_COORDENADORLOCAL);
			
			if(in_array($arrMensario['pflcod'], $men_aberto) && $arrMensario['esdid']==ESD_EM_ABERTO_MENSARIO) {
				$result = wf_alterarEstado( $arrMensario['docid'], AED_APROVAR_EMABERTO_MENSARIO, $cmddsc = '', array('fpbid'=>$arrMensario['fpbid'], 'pflcod'=>$arrMensario['pflcod'],'menid'=>$menid));
			} else {
				$result = wf_alterarEstado( $arrMensario['docid'], AED_APROVAR_MENSARIO, $cmddsc = '', array('menid'=>$menid));	
			}
			
		}
	}

	$al = array("alert"=>"Equipe aprovada com sucesso","location"=>"sisindigena.php?modulo=".$dados['modulo']."&acao=A&aba=aprovarusuario&fpbid=".$dados['fpbid']."&pflcodaprovar=".$dados['pflcodaprovar']);
	alertlocation($al);
	
}

function posAprovarMensario($menid) {
	global $db;
	
	$sql = "SELECT	t.tpeid, m.iusd, m.fpbid, p.pflcod, p.pfldsc, i.iuscpf, i.iusnaodesejosubstituirbolsa, i.iusnome, f.fpbmesreferencia, f.fpbanoreferencia, pp.plpvalor, un.uniid FROM sisindigena.mensario m 
			INNER JOIN sisindigena.identificacaousuario i ON i.iusd = m.iusd
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sisindigena.folhapagamento f ON f.fpbid = m.fpbid 
			INNER JOIN sisindigena.pagamentoperfil pp ON pp.pflcod = t.pflcod 
			INNER JOIN sisindigena.universidadecadastro un ON un.uncid = i.uncid 
			WHERE m.menid='".$menid."'";
	
	$arrInfo = $db->pegaLinha($sql);
	
	if($arrInfo['iusnaodesejosubstituirbolsa']!='t') {
		
		$sql = "SELECT 'Não foi possível criar o registro de bolsa para ".str_replace(array("'"),array(" "),$arrInfo['iusnome']).", pois a bolsa ja foi paga para ' || i.iusnome || ' => ' || 'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao FROM sisindigena.pagamentobolsista p 
				INNER JOIN sisindigena.identificacaousuario i ON i.iusd = p.iusd 
				INNER JOIN sisindigena.folhapagamento f ON f.fpbid = p.fpbid 
				INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
				WHERE p.tpeid='".$arrInfo['tpeid']."' AND p.fpbid='".$arrInfo['fpbid']."'";
		
		$descricao = $db->pegaUm($sql);
		
		if($descricao) {

			die("<script>
					alert('".$descricao."');
					window.close();
				 </script>");

		} else {
			$docid = wf_cadastrarDocumento(TPD_PAGAMENTOBOLSA, "Pagamento SISIndígena - ".$arrInfo['pfldsc']." - ( ".$arrInfo['iuscpf']." )".$arrInfo['iusnome']." - ".$arrInfo['fpbmesreferencia']."/".$arrInfo['fpbanoreferencia']);
			
			$sql = "INSERT INTO sisindigena.pagamentobolsista(
		            iusd, fpbid, docid, cpfresponsavel, pbodataenvio, pbovlrpagamento, 
		            pflcod, uniid, tpeid)
		    VALUES ('".$arrInfo['iusd']."', '".$arrInfo['fpbid']."', '".$docid."', '".$_SESSION['usucpf']."', NOW(), '".$arrInfo['plpvalor']."', 
		            '".$arrInfo['pflcod']."', '".$arrInfo['uniid']."', '".$arrInfo['tpeid']."');";
			
			$db->executar($sql);
			$db->commit();
			
			return true;
			
		}
		
	}
	
	return false;
	
}

function calculaPorcentagemUsuarioAtivos($dados) {
	global $db;
	
	if($_REQUEST['modulo']=='principal/universidade/universidadeexecucao') {
		$sql_equipe = sqlEquipeCoordenadorIES(array("uncid"=>$_SESSION['sisindigena']['universidade']['uncid']));
		$sis = 'universidade';
	}
	
	if($_REQUEST['modulo']=='principal/coordenadoradjuntoies/coordenadoradjuntoies') {
		$sql_equipe = sqlEquipeCoordenadorAdjunto(array("uncid"=>$_SESSION['sisindigena']['coordenadoradjuntoies']['uncid']));
		$sis = 'coordenadoradjuntoies';
	}
	
	if($_REQUEST['modulo']=='principal/supervisories/supervisories') {
		$sql_equipe = sqlEquipeSupervisor(array("uncid"=>$_SESSION['sisindigena']['supervisories']['uncid']));
		$sis = 'supervisories';
	}
	
	if($_REQUEST['modulo']=='principal/formadories/formadories') {
		$sql_equipe = sqlEquipeFormador(array("iusd"=>$_SESSION['sisindigena']['formadories']['iusd'],"uncid"=>$_SESSION['sisindigena']['formadories']['uncid']));
		$sis = 'formadories';
	}
	
	if($_REQUEST['modulo']=='principal/orientadorestudo/orientadorestudo') {
		$sql_equipe = sqlEquipeOrientador(array("iusd"=>$_SESSION['sisindigena']['orientadorestudo']['iusd'],"uncid"=>$_SESSION['sisindigena']['orientadorestudo']['uncid']));
		$sis = 'orientadorestudo';
	}
	
	if($_REQUEST['modulo']=='principal/coordenadorlocal/coordenadorlocalexecucao') {
		$sql_equipe_p = sqlEquipeCoordenadorLocal(array("picid"=>$_SESSION['sisindigena']['coordenadorlocal'][$_SESSION['sisindigena']['esfera']]['picid']));
		$sis = 'coordenadorlocal';
	}
	
	if($sql_equipe_p) {
		$sql_total = "SELECT COUNT(*) as tot FROM ({$sql_equipe_p}) foo WHERE foo.status='A' AND CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." OR foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN foo.iusd in( SELECT i.iusd FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.pactoidadecerta p ON p.picid = i.picid INNER JOIN workflow.documento d ON d.docid = p.docidturma WHERE d.esdid='".ESD_FECHADO_TURMA."') ELSE true END";
		$total = $db->pegaUm($sql_total);
		
		$sql_total_a = "SELECT COUNT(*) as tot FROM ({$sql_equipe_p}) foo WHERE foo.status='A' AND CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." OR foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN foo.iusd in( SELECT i.iusd FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.pactoidadecerta p ON p.picid = i.picid INNER JOIN workflow.documento d ON d.docid = p.docidturma WHERE d.esdid='".ESD_FECHADO_TURMA."') ELSE true END";
		$total_a = $db->pegaUm($sql_total_a);
		
		if($total) $apassituacao = round(($total_a/$total)*100);
		
		gerenciarAtividadePacto(array('iusd'=>$_SESSION['sisindigena']['coordenadorlocal'][$_SESSION['sisindigena']['esfera']]['iusd'],'apadatainicio'=>$apadatainicio,'apadatafim'=>$apadatafim,'apassituacao'=>$apassituacao,'suaid'=>$dados['suaid'],'picid'=>$_SESSION['sisindigena']['coordenadorlocal'][$_SESSION['sisindigena']['esfera']]['picid']));
	}
	
	if($sql_equipe) {
		$sql_total = "SELECT COUNT(*) as tot FROM ({$sql_equipe}) foo WHERE CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." OR foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN foo.iusd in( SELECT i.iusd FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.pactoidadecerta p ON p.picid = i.picid INNER JOIN workflow.documento d ON d.docid = p.docidturma WHERE d.esdid='".ESD_FECHADO_TURMA."') ELSE true END";
		$total = $db->pegaUm($sql_total);
		
		$sql_total_a = "SELECT COUNT(*) as tot FROM ({$sql_equipe}) foo WHERE foo.status='A' AND CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." OR foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN foo.iusd in( SELECT i.iusd FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.pactoidadecerta p ON p.picid = i.picid INNER JOIN workflow.documento d ON d.docid = p.docidturma WHERE d.esdid='".ESD_FECHADO_TURMA."') ELSE true END";
		$total_a = $db->pegaUm($sql_total_a);
		
		$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sisindigena'][$sis]['uncid']));
		
		if($total) $aunsituacao = round(($total_a/$total)*100);
		gerenciarAtividadeUniversidade(array('iusd'=>$_SESSION['sisindigena'][$sis]['iusd'],'aundatafim'=>$aundatafim,'aundatainicio'=>$aundatainicio,'aunsituacao'=>$aunsituacao,'ecuid'=>$ecuid,'suaid'=>$dados['suaid']));
	}
	
	
}

function gerenciarAtividadeUniversidade($dados) {
	global $db;
	
	$sql = "SELECT aunid FROM sisindigena.atividadeuniversidade a 
			WHERE suaid='".$dados['suaid']."' AND ecuid='".$dados['ecuid']."'".(($dados['iusd'])?" AND iusd='".$dados['iusd']."'":"");
	
	$aunid = $db->pegaUm($sql);
	
	if($aunid) {
		
		$sql = "UPDATE sisindigena.atividadeuniversidade SET 
				aunsituacao=".(($dados['aunsituacao'])?"'".$dados['aunsituacao']."'":"0").", 
				aundatainicio=".(($dados['aundatainicio'])?"'".$dados['aundatainicio']."'":"NULL").", 
				aundatafim=".(($dados['aundatafim'])?"'".$dados['aundatafim']."'":"NULL")."
			    WHERE aunid='".$aunid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sisindigena.atividadeuniversidade(
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
	
	$sql = "SELECT apaid FROM sisindigena.atividadepacto WHERE suaid='".$dados['suaid']."' AND picid='".$dados['picid']."'".(($dados['iusd'])?" AND iusd='".$dados['iusd']."'":"");
	$apaid = $db->pegaUm($sql);
	
	if($apaid) {
		
		$sql = "UPDATE sisindigena.atividadepacto SET 
				apassituacao=".(($dados['apassituacao'])?"'".$dados['apassituacao']."'":"NULL").", 
				apadatainicio=".(($dados['apadatainicio'])?"'".$dados['apadatainicio']."'":"NULL").",
				apadatafim=".(($dados['apadatafim'])?"'".$dados['apadatafim']."'":"NULL")."
			    WHERE apaid='".$apaid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sisindigena.atividadepacto(
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
											  FROM sisindigena.subatividades su
										  	  INNER JOIN sisindigena.atividadeuniversidade ap ON su.suaid = ap.suaid 
										  	  INNER JOIN sisindigena.estruturacurso es ON es.ecuid = ap.ecuid 
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
									  FROM sisindigena.atividadeuniversidade au 
									  INNER JOIN sisindigena.estruturacurso es ON es.ecuid = au.ecuid
									  WHERE suaid='".$dados['suaid']."' AND es.uncid='".$dados['uncid']."'".(($dados['iusd'])?" AND au.iusd='".$dados['iusd']."'":""));
	
	return $atividadeuni;
	
	
}

function pegarEstruturaCurso($dados) {
	global $db;
	$sql = "SELECT ecuid FROM sisindigena.estruturacurso WHERE uncid='".$dados['uncid']."'";
	$ecuid = $db->pegaUm($sql);
	
	if(!$ecuid) {
		
		$sql = "INSERT INTO sisindigena.estruturacurso(
        	    uncid, muncod, ecustatus)
    			VALUES ('".$dados['uncid']."', NULL, 'A') RETURNING ecuid;";
		
		$ecuid = $db->pegaUm($sql);
		$db->commit();
		
	}
	
	return $ecuid;
	
}

function carregarPeriodoReferencia($dados) {
	global $db;
	
	$perfis = pegaPerfilGeral();
	
	$sql = "SELECT DISTINCT f.fpbid as codigo, m.mesdsc || ' / ' || fpbanoreferencia as descricao, (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date as t 
			FROM sisindigena.folhapagamento f 
			INNER JOIN sisindigena.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE ".(($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis))?"":"f.fpbstatus='A' AND ")." rf.uncid='".$dados['uncid']."' ".(($dados['picid'])?"AND rf.picid='".$dados['picid']."'":"")." AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd') ORDER BY (fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date";

	$sql_tot = "SELECT count(*) as tot FROM ($sql) as foo";
	$tot = $db->pegaUm($sql_tot);
	
	if(!$tot) {
		echo "<br><fieldset><legend>Aviso</legend>Não existem períodos de referências cadastrados.</fieldset><br>";
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
	} else {
		$acao = "'<img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"detalharAvaliacoesUsuario('||per.pflcod||','||foo3.fpbid||',this);\">' as acao,";
	}
	
	if($dados['fpbid']) {
		$wh[] = "m.fpbid='".$dados['fpbid']."'";
	}
	
	if($dados['picid']) {
		$wh[] = "i.picid='".$dados['picid']."'";
	}
	
	$sql = "SELECT {$acao} foo3.periodo, per.pflcod, per.pfldsc, SUM(napto) as na, SUM(apto) as ap, SUM(aprov) as ar FROM (
	SELECT foo2.fpbid, 'Ref.'||m.mesdsc||'/'||f.fpbanoreferencia as periodo, foo2.pflcod,  CASE WHEN foo2.resultado='Não Apto' THEN 1 ELSE 0 END as napto, CASE WHEN foo2.resultado='Apto' THEN 1 ELSE 0 END as apto, CASE WHEN foo2.resultado='Aprovado' THEN 1 ELSE 0 END as aprov
	FROM sisindigena.folhapagamento f 
	INNER JOIN public.meses m ON m.mescod::numeric = f.fpbmesreferencia 
	INNER JOIN (
	
	SELECT foo.pflcod,
			".criteriosAprovacao('restricao3').", foo.fpbid FROM (
	SELECT 
	COALESCE((SELECT AVG(mavtotal) FROM sisindigena.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
	i.iusdocumento,
	i.iustermocompromisso,
	i.iusnaodesejosubstituirbolsa,
	m.fpbid,
	d.esdid,
	t.pflcod,
	i.iustipoprofessor,
	(SELECT COUNT(mavid) FROM sisindigena.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes,
	(SELECT COUNT(pboid) FROM sisindigena.pagamentobolsista pg WHERE pg.iusd=i.iusd) as numeropagamentos,
	(SELECT COUNT(pboid) FROM sisindigena.pagamentobolsista pg WHERE pg.tpeid=t.tpeid) as numeropagamentosvaga,
	pp.plpmaximobolsas
	FROM sisindigena.mensario m
	INNER JOIN sisindigena.identificacaousuario i ON i.iusd = m.iusd 
	INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd  
	INNER JOIN sisindigena.pagamentoperfil pp ON pp.pflcod = t.pflcod 
	INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO."
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
			       CASE WHEN us.suscod='P' OR usu.suscod='P' THEN 1 ELSE 0 END as pendente,
			       CASE WHEN us.suscod='B' OR usu.suscod='B' THEN 1 ELSE 0 END as bloqueado,
			       CASE WHEN us.suscod is null THEN 1 ELSE 0 END as naocadastrado
			FROM sisindigena.identificacaousuario u 
			INNER JOIN sisindigena.tipoperfil t on t.iusd=u.iusd 
			INNER JOIN seguranca.perfil p on p.pflcod = t.pflcod 
			LEFT JOIN seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_INDIGENA." 
			LEFT JOIN seguranca.usuario usu on usu.usucpf = u.iuscpf
			WHERE t.pflcod in(
						".PFL_PROFESSORALFABETIZADOR.",
						".PFL_COORDENADORLOCAL.",
						".PFL_ORIENTADORESTUDO.",
						".PFL_COORDENADORIES.",
						".PFL_COORDENADORADJUNTOIES.",
						".PFL_SUPERVISORIES.",
						".PFL_FORMADORIES.",
						".PFL_CONTEUDISTA.",
						".PFL_PESQUISADOR.") ".(($wh)?"AND ".implode(" AND ",$wh):"")."
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
			INNER JOIN sisindigena.pagamentobolsista pb ON pb.pflcod = p.pflcod 
			INNER JOIN sisindigena.universidadecadastro un ON un.uniid = pb.uniid 
			INNER JOIN workflow.documento dc ON dc.docid = pb.docid AND dc.tpdid=".TPD_PAGAMENTOBOLSA." 
			WHERE p.pflcod IN(
			".PFL_PROFESSORALFABETIZADOR.",
			".PFL_COORDENADORLOCAL.",
			".PFL_ORIENTADORESTUDO.",
			".PFL_COORDENADORIES.",
			".PFL_COORDENADORADJUNTOIES.",
			".PFL_SUPERVISORIES.",
			".PFL_FORMADORIES.",
			".PFL_PESQUISADOR.",
			".PFL_CONTEUDISTA.") ".(($wh)?" AND ".implode(" AND ",$wh):"")."

			) fee 

			GROUP BY fee.pflcod, fee.pfldsc
			
			) foo
			
			INNER JOIN sisindigena.pagamentoperfil pp ON pp.pflcod = foo.pflcod";
	
	
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
					(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_INDIGENA.") as status,
					(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod = t.pflcod) as perfil
			FROM sisindigena.identificacaousuario i
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod IN('".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."','".PFL_COORDENADORLOCAL."','".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORIES."','".PFL_ORIENTADORESTUDO."') AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome";
	
	return $sql;
}

function cadastrarPeriodoReferencia($dados) {
	global $db;
	
	$uncids = array_keys($dados['smesini']);
	
	if($uncids) {
		foreach($uncids as $uncid) {

			$picids = array_keys($dados['smesini'][$uncid]);
			
			if($picids) {
				
				foreach($picids as $picid) {
			
					$sql = "DELETE FROM sisindigena.folhapagamentouniversidade WHERE uncid='".$uncid."' AND picid='".$picid."'";
					$db->executar($sql);
					
					$sql = "select foo.fpbid from (
							select fpbid, fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0') as dt from sisindigena.folhapagamento ) foo
							where foo.dt >= '".$dados['sanoinicio'][$uncid][$picid]."-".str_pad($dados['smesini'][$uncid][$picid],2,"0", STR_PAD_LEFT)."' AND foo.dt <= '".$dados['sanofim'][$uncid][$picid]."-".str_pad($dados['smesfim'][$uncid][$picid],2,"0", STR_PAD_LEFT)."'";
					
					$fpbids = $db->carregarColuna($sql);
					
					if($fpbids) {
						foreach($fpbids as $key => $fpbid) {
							$sql = "INSERT INTO sisindigena.folhapagamentouniversidade(
			            			uncid, fpbid, rfuparcela, picid)
								    VALUES ('".$uncid."', '".$fpbid."', '".($key+1)."', '".$picid."');";
							
							$db->executar($sql);
							
						}
					}
			
				}
			
			}
			
		}
	}
	
	$db->commit();
	
	$al = array("alert"=>"Período de referência aprovado com sucesso","location"=>"sisindigena.php?modulo=principal/mec/mec&acao=A&aba=configuracoes");
	alertlocation($al);
	
	
}

function carregarLogCadastroSGB($dados) {
	global $db;
	
	$iusd = $db->pegaUm("SELECT iusd FROM sisindigena.identificacaousuario WHERE iuscpf='".$dados['usucpf']."'");
	
	if($iusd) echo "<input type=hidden name=iusd id=iusd_log value=\"".$iusd."\">";
	
	$sql = "SELECT u.iuscpf, u.iusnome, to_char(logdata,'dd/mm/YYYY HH24:MI') as logdata, logresponse FROM log_historico.logsgb_sisindigena l 
			INNER JOIN sisindigena.identificacaousuario u ON u.iuscpf = l.logcpf 
			WHERE logcpf='".$dados['usucpf']."' AND logservico='gravarDadosBolsista' ORDER BY logdata DESC LIMIT 5";
	$cabecalho = array("CPF","Nome","Data","Erro");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true,false,false,true);
	
}

function criteriosAprovacao($cla) {
	global $db;
	
	$cl['restricao1'] = "CASE 
    					 WHEN foo.numeropagamentos >= foo.plpmaximobolsas THEN '<span style=color:red;font-size:x-small;>Atingiu o número máximo ('||foo.plpmaximobolsas||' bolsas)</span>'
    					 WHEN foo.numeropagamentosvaga >= foo.plpmaximobolsas THEN '<span style=color:red;font-size:x-small;>Atingiu o número máximo ('||foo.plpmaximobolsas||' bolsas)</span>'
						 WHEN foo.iusnaodesejosubstituirbolsa=true      THEN '<span style=color:red;font-size:x-small;>Bolsista do FNDE/MEC e não deseja substituir bolsa atual pela bolsa do SISIndígena</span>' 
						 WHEN foo.fpbidini IS NOT NULL AND foo.fpbidfim IS NOT NULL AND (foo.fpbid < foo.fpbidini OR foo.fpbid > foo.fpbidfim) THEN '<span style=color:red;>Este período de referência não esta habilitado para pagamento</span>' 
				   		 ELSE '<span style=color:blue;font-size:x-small;>Nenhuma restrição</span>' END as restricao";
	
	$cl['restricao2'] = "CASE WHEN foo.mensarionota >= 7  AND foo.iustermocompromisso=true AND 
						foo.numeropagamentos < foo.plpmaximobolsas AND
						foo.numeropagamentosvaga < foo.plpmaximobolsas AND  
						(CASE WHEN foo.fpbidini IS NOT NULL AND foo.fpbidfim IS NOT NULL AND (foo.fpbid < foo.fpbidini OR foo.fpbid > foo.fpbidfim) THEN false ELSE true END) AND foo.iusnaodesejosubstituirbolsa=false  THEN 'checked'
						ELSE 'disabled' END";
	
	$cl['restricao3'] = "CASE WHEN foo.iusnaodesejosubstituirbolsa=true THEN 'Não Apto'
	    					  WHEN foo.numeropagamentos >= foo.plpmaximobolsas THEN 'Não Apto'
    					 	  WHEN foo.numeropagamentosvaga >= foo.plpmaximobolsas THEN 'Não Apto'
							  WHEN foo.esdid=".ESD_APROVADO_MENSARIO." THEN 'Aprovado'
						 	  WHEN foo.mensarionota >= 7  AND foo.iustermocompromisso=true THEN 'Apto' 
		    			ELSE 'Não Apto' END resultado";
	
	
	return $cl[$cla];
	
}

function verificarEmailUnico($dados) {
	global $db;
	
	$sql = "SELECT '- '||i.iusnome||' ( '||p.pfldsc||' );' as descricao FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
			WHERE i.iusemailprincipal='".$dados['iusemailprincipal']."' AND i.iusd!='".$dados['iusd']."'";
	
	$nomes = $db->carregarColuna($sql);
	
	echo implode('\n', $nomes);
	
	
}

function consultarDetalhesPagamento($dados) {
	global $db;
	$sql = "SELECT i.iusnome, me.mesdsc||'/'||fa.fpbanoreferencia as periodo, e.esddsc, p.pbovlrpagamento, pp.pfldsc, uni.uninome, uni.unicnpj, p.docid FROM sisindigena.pagamentobolsista p 
			INNER JOIN sisindigena.identificacaousuario i ON i.iusd = p.iusd 
			INNER JOIN sisindigena.folhapagamento fa ON fa.fpbid = p.fpbid 
			INNER JOIN public.meses me ON me.mescod::integer = fa.fpbmesreferencia 
			INNER JOIN workflow.documento d ON d.docid = p.docid  AND d.tpdid=".TPD_PAGAMENTOBOLSA."
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = p.pflcod 
			INNER JOIN sisindigena.universidade uni ON uni.uniid = p.uniid  
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
				SELECT count(p.picid) as tot, ".$dados['group']." as grouper FROM sisindigena.materiais m 
				INNER JOIN sisindigena.pactoidadecerta p ON p.picid = m.picid 
				INNER JOIN sisindigena.abrangencia a ON a.muncod = p.muncod 
				INNER JOIN sisindigena.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='M' and e.uncid='".$dados['uncid']."' GROUP BY ".$dados['group']."
				) UNION ALL (
				SELECT count(p.picid) as tot, ".$dados['group']." as grouper FROM sisindigena.materiais m 
				INNER JOIN sisindigena.pactoidadecerta p ON p.picid = m.picid 
				INNER JOIN territorios.municipio mu ON mu.estuf = p.estuf  
				INNER JOIN sisindigena.abrangencia a ON a.muncod = mu.muncod 
				INNER JOIN sisindigena.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='E' and e.uncid='".$dados['uncid']."' GROUP BY ".$dados['group']."
				)
				) foo";
		
		$materiais = $db->carregar($sql);
		
	} else {
		
		$materiais = $db->carregar("SELECT count(picid) as tot, ".$dados['group']." as grouper FROM sisindigena.materiais GROUP BY ".$dados['group']);
				
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
	$materiais = $db->carregar("SELECT count(iusd) as tot, ".$dados['group']." as grouper FROM sisindigena.materiaisprofessores GROUP BY ".$dados['group']);

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
	
	$npagamentos = $db->pegaUm("SELECT COUNT(*) FROM sisindigena.pagamentobolsista WHERE iusd='".$dados['iusd']."'");
	
	if($npagamentos > 0) {
		
		$identificacaousuario = $db->pegaLinha("SELECT i.iuscpf, t.pflcod, p.pfldsc FROM sisindigena.identificacaousuario i 
												INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
												INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod  
												WHERE i.iusd='".$dados['iusd']."'");
		
		
		$sql = "INSERT INTO sisindigena.identificacaousuario(
	            picid, muncod, eciid, nacid, fk_cod_docente, iuscpf, iusnome, 
	            iussexo, iusdatanascimento, iusnomemae, iusnomeconjuge, iusagenciasugerida, 
	            iusemailprincipal, iusemailopcional, iusdatainclusao, iuscadastrovalidadoies, 
	            iussituacao, iusstatus, funid, iusagenciaend, iustipoorientador, 
	            foeid, iustermocompromisso, tvpid, muncodatuacao, uncid, iusserieprofessor, 
	            iusformacaoinicialorientador, cadastradosgb, iustipoprofessor, 
	            iusdocumento, iusnaodesejosubstituirbolsa)
				SELECT picid, muncod, eciid, nacid, fk_cod_docente, 'REM'||SUBSTR(iuscpf,4,8) as iuscpf, iusnome || ' - {$identificacaousuario['pfldsc']} - REMOVIDO' as iusnome, 
				       iussexo, iusdatanascimento, iusnomemae, iusnomeconjuge, iusagenciasugerida, 
				       iusemailprincipal, iusemailopcional, iusdatainclusao, iuscadastrovalidadoies, 
				       iussituacao, 'I' as iusstatus, funid, iusagenciaend, iustipoorientador, 
				       foeid, iustermocompromisso, tvpid, muncodatuacao, uncid, iusserieprofessor, 
				       iusformacaoinicialorientador, cadastradosgb, iustipoprofessor, 
				       iusdocumento, iusnaodesejosubstituirbolsa
				  FROM sisindigena.identificacaousuario where iusd='".$dados['iusd']."'
				RETURNING iusd;";
		
		$iusd_novo = $db->pegaUm($sql);
		
		
		if($identificacaousuario) {
			$sql = "DELETE FROM sisindigena.usuarioresponsabilidade  WHERE rpustatus='A' AND usucpf='".$identificacaousuario['iuscpf']."' AND pflcod='".$identificacaousuario['pflcod']."'";
			$db->executar($sql);
		}
	
		$sql = "UPDATE sisindigena.tipoperfil SET iusd='".$iusd_novo."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	
		$sql = "UPDATE sisindigena.turmas SET iusd='".$iusd_novo."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	
		$sql = "UPDATE sisindigena.orientadorturma SET iusd='".$iusd_novo."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	
		$sql = "UPDATE sisindigena.identificacaousuario SET iusstatus='I' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	
		if($identificacaousuario) {
			$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$identificacaousuario['iuscpf']."' AND pflcod='".$identificacaousuario['pflcod']."'";
			$db->executar($sql);
		}
	
		// removendo avaliações não concluidas
		$sql = "SELECT m.menid FROM sisindigena.mensario m 
				INNER JOIN workflow.documento d ON d.docid = m.docid 
				WHERE iusd='".$dados['iusd']."' AND d.esdid!='".ESD_APROVADO_MENSARIO."'";
	
		$menids = $db->carregarColuna($sql);
		
		if($menids) {
			
			$sql = "SELECT mavid FROM sisindigena.mensarioavaliacoes WHERE menid IN('".implode("','",$menids)."')";
			$mavids = $db->carregarColuna($sql);
			
			if($mavids) {
				$db->executar("DELETE FROM sisindigena.historicoreaberturanota WHERE mavid IN('".implode("','",$mavids)."')");
				$db->executar("DELETE FROM sisindigena.mensarioavaliacoes WHERE mavid IN('".implode("','",$mavids)."')");
			}
		}
		
		
	} else {
	
		removerTipoPerfil(array('iusd'=>$dados['iusd'],'pflcod'=>$dados['pflcod'],'naoredirecionar'=>true));
	
	}
	
	if(!$dados['uncid']) $dados['uncid'] = $db->pegaUm("SELECT uncid FROM sisindigena.identificacaousuario WHERE iusd='".$dados['iusd']."'");

	$sql = "INSERT INTO sisindigena.historicotrocausuario(
            iusdantigo, pflcod, hstdata, usucpf, uncid, 
            hstacao)
    		VALUES ('".$dados['iusd']."', '".$dados['pflcod']."', NOW(), '".$_SESSION['usucpf']."', '".$dados['uncid']."', 'R');";
	
	$db->executar($sql);
	$db->commit();
	
	gerarVersaoProjetoUniversidade(array('uncid'=>$dados['uncid']));
	
	$al = array("alert"=>"Exclusão ocorrida com sucesso","location"=>"sisindigena.php?modulo=".$dados['modulo']."&acao=".$dados['acao']."&aba=".$dados['aba'].(($dados['uncid'])?"&uncid=".$dados['uncid']:""));
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
	
	$_SESSION['imgparams']['tabela'] = "sisindigena.materiaisfotos";
	$_SESSION['imgparams']['filtro'] = "1=1";
	
	if($dados['uncid']) {
		$sql = "SELECT m.arqid, m.matid, m.mafdsc FROM sisindigena.materiaisfotos m
				INNER JOIN sisindigena.materiais ma ON ma.matid = m.matid 
				INNER JOIN sisindigena.pactoidadecerta p ON p.picid = ma.picid 
				INNER JOIN sisindigena.abrangencia a ON a.muncod = p.muncod 
				INNER JOIN territorios.municipio mu ON mu.muncod = a.muncod
				INNER JOIN sisindigena.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='M' and e.uncid='".$dados['uncid']."' 
				ORDER BY random() LIMIT 6";
	} else {
		$sql = "SELECT m.arqid, m.matid, m.mafdsc FROM sisindigena.materiaisfotos m 
				INNER JOIN sisindigena.materiais ma ON ma.matid = m.matid 
				INNER JOIN sisindigena.pactoidadecerta p ON p.picid = ma.picid 
				ORDER BY random() LIMIT 6";
	}
	$fotos = $db->carregar($sql);
	if($fotos) {
		foreach($fotos as $ft) {
			echo "<td><img id=".$ft['arqid']." onmouseover=\"return escape('".$ft['mafdsc']."');\" src=\"../slideshow/slideshow/verimagem.php?arqid=".$ft['arqid']."&newwidth=70&newheight=70\" class=\"imageBox_theImage\" onclick=\"javascript:window.open('../slideshow/slideshow/index.php?pagina=&amp;arqid=".$ft['arqid']."&amp;_sisarquivo=sisindigena&amp;getFiltro=true&amp;matid=".$ft['matid']."','imagem','width=850,height=600,resizable=yes');\"></td>";
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
?>

<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloCentro">Pergunta</td>
	<td class="SubTituloCentro">&nbsp;</td>
	<td class="SubTituloCentro">&nbsp;</td>
	<td class="SubTituloCentro">&nbsp;</td>
</tr>
<?

$recebeumaterialpacto = carregarMateriaisProfessores(array('group' => 'recebeumaterialpacto'));
$recebeumaterialpnld = carregarMateriaisProfessores(array('group' => 'recebeumaterialpnld'));
$recebeulivrospnld = carregarMateriaisProfessores(array('group' => 'recebeulivrospnld'));
$recebeumaterialpnbe = carregarMateriaisProfessores(array('group' => 'recebeumaterialpnbe'));
$criadocantinholeitura = carregarMateriaisProfessores(array('group' => 'criadocantinholeitura'));

?>
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
	
	$_SESSION['imgparams']['tabela'] = "sisindigena.materiaisprofessoresfotos";
	$_SESSION['imgparams']['filtro'] = "1=1";
	
	$sql = "SELECT m.arqid, m.mapid, m.mpfdsc FROM sisindigena.materiaisprofessoresfotos m 
			INNER JOIN sisindigena.materiaisprofessores ma ON ma.mapid = m.mapid 
			ORDER BY random() LIMIT 6";

	$fotos = $db->carregar($sql);
	if($fotos) {
		foreach($fotos as $ft) {
			echo "<td><img id=".$ft['arqid']." onmouseover=\"return escape('".$ft['mafdsc']."');\" src=\"../slideshow/slideshow/verimagem.php?arqid=".$ft['arqid']."&newwidth=70&newheight=70\" class=\"imageBox_theImage\" onclick=\"javascript:window.open('../slideshow/slideshow/index.php?pagina=&amp;arqid=".$ft['arqid']."&amp;_sisarquivo=sisindigena&amp;getFiltro=true&amp;mapid=".$ft['mapid']."','imagem','width=850,height=600,resizable=yes');\"></td>";
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
				COALESCE(array_to_string(array(SELECT i.iusnome FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as coordenadorlocal,
				COALESCE(array_to_string(array(SELECT i.iusemailprincipal FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as emailcoordenadorlocal,
				COALESCE(array_to_string(array(SELECT '('||itedddtel||') '||itenumtel as tel FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON i.iusd=t.iusd INNER JOIN sisindigena.identificacaotelefone it ON it.iusd = i.iusd AND it.itetipo='C' WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'-') as telefonecoordenadorlocal
				FROM sisindigena.materiais m 
				INNER JOIN sisindigena.pactoidadecerta p ON p.picid = m.picid 
				INNER JOIN territorios.municipio mu ON mu.estuf = p.estuf 
				INNER JOIN territorios.estado es ON es.estuf = p.estuf 
				INNER JOIN sisindigena.abrangencia a ON a.muncod = mu.muncod 
				INNER JOIN sisindigena.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='E' and e.uncid='".$dados['uncid']."' and {$dados['campo']}='{$dados['opcao']}'
				ORDER BY 1,2
				) UNION ALL (
				SELECT 
				'Municipal' as esfera,
				mu.estuf || ' / ' || mu.mundescricao as descricao,
				COALESCE(array_to_string(array(SELECT i.iusnome FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as coordenadorlocal,
				COALESCE(array_to_string(array(SELECT i.iusemailprincipal FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as emailcoordenadorlocal,
				COALESCE(array_to_string(array(SELECT '('||itedddtel||') '||itenumtel as tel FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON i.iusd=t.iusd INNER JOIN sisindigena.identificacaotelefone it ON it.iusd = i.iusd AND it.itetipo='C' WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'-') as telefonecoordenadorlocal
				 
				
				FROM sisindigena.materiais m 
				INNER JOIN sisindigena.pactoidadecerta p ON p.picid = m.picid 
				INNER JOIN sisindigena.abrangencia a ON a.muncod = p.muncod 
				INNER JOIN territorios.municipio mu ON mu.muncod = a.muncod
				INNER JOIN sisindigena.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='M' and e.uncid='".$dados['uncid']."' and {$dados['campo']}='{$dados['opcao']}'
				ORDER BY 1,2
				)
				) foo";
		
		
	} else {
	
		$sql = "SELECT 
				CASE WHEN p.muncod IS NOT NULL THEN 'Municipal' ELSE 'Estadual' END as esfera,
				CASE WHEN p.muncod IS NOT NULL THEN mu.estuf || ' / ' || mu.mundescricao ELSE es.estuf || ' / ' || es.estdescricao END as descricao,
				COALESCE((SELECT iusnome FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."),'Coordenador Local não cadastrado') as coordenadorlocal,
				COALESCE((SELECT iusemailprincipal FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."),'Coordenador Local não cadastrado') as emailcoordenadorlocal,
				COALESCE((SELECT '('||itedddtel||') '||itenumtel as tel FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON i.iusd=t.iusd INNER JOIN sisindigena.identificacaotelefone it ON it.iusd = i.iusd AND it.itetipo='C' WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."),'-') as telefonecoordenadorlocal 
				FROM sisindigena.materiais m 
				INNER JOIN sisindigena.pactoidadecerta p ON p.picid = m.picid 
				LEFT JOIN territorios.municipio mu ON mu.muncod = p.muncod 
				LEFT JOIN territorios.estado es ON es.estuf = p.estuf 
				WHERE {$dados['campo']}='{$dados['opcao']}' ORDER BY 1,2";
	}
	
	$cabecalho = array("Esfera","Descrição","Coordenador Local","Email","Telefone");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true);
	
}

function carregarHistoricoUsuario($dados) {
	global $db;
	
	$sql = "SELECT us.usunome, to_char(htudata,'dd/mm/YYYY HH24:MI') as data, hu.htudsc, hu.suscod, us2.usunome as resp FROM seguranca.historicousuario hu 
			INNER JOIN seguranca.usuario us ON us.usucpf = hu.usucpf 
			LEFT JOIN seguranca.usuario us2 ON us2.usucpf = hu.usucpfadm
			WHERE hu.usucpf='".$dados['usucpf']."' AND hu.sisid='".SIS_INDIGENA."' ORDER BY htudata DESC";
	
	$cabecalho = array("Nome","Data","Justificativa","Situação","Responsável");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true);
	
}


function trocarTurmas($dados) {
	global $db;
	if($dados['troca']) {
		foreach($dados['troca'] as $iusd => $turid) {
			if($turid) {
				$db->executar("UPDATE sisindigena.orientadorturma SET turid='".$turid."' WHERE iusd='".$iusd."'");
				$db->executar("INSERT INTO sisindigena.historicotrocausuario(
            				   iusdantigo, pflcod, hstdata, usucpf, uncid, hstacao, turidnovo, turidantigo)
    						   VALUES ('".$iusd."', 
    						   		   (SELECT pflcod FROM sisindigena.tipoperfil WHERE iusd='".$iusd."'), 
    						   		    NOW(), 
    						   		    '".$_SESSION['usucpf']."', 
    						   		    '".$dados['uncid']."', 
    						   		    'F', 
    						   		    '".$turid."', 
            							'".$dados['turidantigo']."');");
			}
		}
		$db->commit();
	}
	
	
	$al = array("alert"=>"Trocas efetivadas com sucesso","location"=>$_SERVER['REQUEST_URI']);
	alertlocation($al);
	
}

function atualizarEmail($dados) {
	global $db;
	
	$sql = "UPDATE sisindigena.identificacaousuario SET iusemailprincipal='".$dados['iusemailprincipal']."' WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	$db->commit();
	
}

function exibirPorcentagemPagamentoPerfil($dados) {
	global $db;
	
	if($dados['uncid']) $wh[] = "i.uncid='".$dados['uncid']."'";
	
	$sql = "SELECT p.pflcod, p.pfldsc FROM sisindigena.pagamentoperfil pp 
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
			
			$sql = "SELECT count(*) as tot FROM sisindigena.identificacaousuario i 
					INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
					INNER JOIN sisindigena.pagamentoperfil p ON p.pflcod = t.pflcod 
					LEFT JOIN sisindigena.nucleouniversidade pa ON pa.picid = i.picid 
					WHERE t.pflcod='".$p['pflcod']."' AND CASE WHEN t.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN i.iustipoprofessor='censo' ELSE true END AND i.uncid IN( SELECT rf.uncid FROM sisindigena.folhapagamentouniversidade rf WHERE rf.fpbid='".$dados['fpbid']."' ) ".(($wh)?" AND ".implode(" AND ", $wh):"");
			
			$totalus = $db->pegaUm($sql);
			
			$sql = "SELECT count(*) as tot FROM sisindigena.pagamentobolsista p 
					INNER JOIN sisindigena.universidadecadastro i ON i.uniid = p.uniid 
					WHERE p.pflcod='".$p['pflcod']."' AND p.fpbid='".$dados['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sisindigena.folhapagamentouniversidade rf WHERE rf.fpbid='".$dados['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");
			
			$totalpag = $db->pegaUm($sql);
			
			
			$sql = "SELECT count(*) as tot FROM sisindigena.pagamentobolsista p 
					INNER JOIN sisindigena.universidadecadastro i ON i.uniid = p.uniid 
					INNER JOIN workflow.documento d ON d.docid = p.docid 
					WHERE d.esdid='".ESD_PAGAMENTO_EFETIVADO."' AND p.pflcod='".$p['pflcod']."' AND p.fpbid='".$dados['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sisindigena.folhapagamentouniversidade rf WHERE rf.fpbid='".$dados['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");
			
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
	
	$sql = "SELECT f.fpbid, 'Ref.'||m.mesdsc||'/'||f.fpbanoreferencia as periodo FROM sisindigena.folhapagamento f
			INNER JOIN public.meses m ON m.mescod::numeric = f.fpbmesreferencia 
			".(($dados['uncid'])?"INNER JOIN sisindigena.folhapagamentouniversidade fp ON fp.fpbid = f.fpbid AND fp.uncid='".$dados['uncid']."'":"")."
			WHERE (fpbanoreferencia||'-'||fpbmesreferencia||'-'||'01')::date <= '".date("Y-m")."-01' ORDER BY (fpbanoreferencia||'-'||fpbmesreferencia||'-'||'01')::date";
	
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
		echo '</tr>';
		
		foreach($folhapagamento as $fp) {
			echo '<tr>';
			
			$sql = "SELECT count(*) as tot FROM sisindigena.identificacaousuario i 
					INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
					INNER JOIN sisindigena.pagamentoperfil p ON p.pflcod = t.pflcod 
					LEFT JOIN sisindigena.nucleouniversidade pa ON pa.picid = i.picid 
					WHERE CASE WHEN t.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN i.iustipoprofessor='censo' ELSE true END AND i.uncid IN( SELECT rf.uncid FROM sisindigena.folhapagamentouniversidade rf WHERE rf.fpbid='".$fp['fpbid']."' ) ".(($wh)?" AND ".implode(" AND ", $wh):"");
			
			$totalus = $db->pegaUm($sql);
			
			$sql = "SELECT count(*) as tot FROM sisindigena.pagamentobolsista p 
					INNER JOIN sisindigena.universidadecadastro i ON i.uniid = p.uniid 
					WHERE p.fpbid='".$fp['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sisindigena.folhapagamentouniversidade rf WHERE rf.fpbid='".$fp['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");
			
			$totalpag = $db->pegaUm($sql);
			
			$sql = "SELECT count(*) as tot FROM sisindigena.pagamentobolsista p 
					INNER JOIN sisindigena.universidadecadastro i ON i.uniid = p.uniid 
					INNER JOIN workflow.documento d ON d.docid = p.docid 
					WHERE d.esdid='".ESD_PAGAMENTO_EFETIVADO."' AND p.fpbid='".$fp['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sisindigena.folhapagamentouniversidade rf WHERE rf.fpbid='".$fp['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");
			
			$totalpagef = $db->pegaUm($sql);
			
			
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
			
			
			echo '</tr>';
			
		}
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
		echo '<td class="SubTituloDireita">Esfera:</td>';
		echo '<td>';
		$sql = "(
				SELECT p.picid as codigo,
					   'Municipal : '|| m.estuf || ' / ' || m.mundescricao AS descricao
				FROM sisindigena.pactoidadecerta p 
				INNER JOIN sisindigena.abrangencia a ON a.muncod = p.muncod
				INNER JOIN sisindigena.estruturacurso es ON es.ecuid = a.ecuid 
				INNER JOIN territorios.municipio m ON m.muncod = p.muncod 
				WHERE es.uncid='".$dados['uncid']."'
				ORDER BY 2
				) UNION ALL (
				SELECT  DISTINCT p.picid as codigo,
					   'Estadual : '|| e.estuf || ' / ' || e.estdescricao AS descricao
				FROM sisindigena.pactoidadecerta p 
				INNER JOIN territorios.municipio m ON m.estuf = p.estuf
				INNER JOIN sisindigena.abrangencia a ON a.muncod = m.muncod
				INNER JOIN sisindigena.estruturacurso es ON es.ecuid = a.ecuid 
				INNER JOIN territorios.estado e ON e.estuf = p.estuf 
				WHERE es.uncid='".$dados['uncid']."'
				ORDER BY 2
				
				)";
		$db->monta_combo('picid__', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'picid__','', $dados['picid']);
		echo '</td>';
	
		echo '</tr>';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">Turma:</td>';
		echo '<td>';
		$sql = "SELECT turid as codigo,
					   turdesc AS descricao
				FROM sisindigena.turmas p 
				INNER JOIN sisindigena.identificacaousuario i ON i.iusd = p.iusd 
				INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_FORMADORIES."
				WHERE p.uncid='".$dados['uncid']."'
				ORDER BY 2";
		$db->monta_combo('turid__', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'turid__','', $dados['turid']);
		echo '</td>';
		
		echo '</tr>';
		
		echo '</table>';
	}
	
	if($dados['pflcod_']==PFL_PROFESSORALFABETIZADOR) {

		echo '<p align=center><b>Informações Professor Alfabetizador</b></p>';
		echo '<input type="hidden" name="iustipoprofessor__" value="censo">';
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		echo '<tr>';
		echo '<td class="SubTituloDireita">Núcleo:</td>';
		echo '<td>';
		echo '<input type="hidden" name="picid__" id="picid__" value="'.$dados['picid'].'">';
		echo $db->pegaUm("SELECT u.unisigla||' - '||u.uninome AS descricao FROM sisindigena.nucleouniversidade p INNER JOIN sisindigena.universidade u ON u.uniid = p.uniid WHERE p.picid='".$dados['picid']."'");
		echo '</td>';
	
		echo '</tr>';

		echo '<tr>';
		echo '<td class="SubTituloDireita">Turma:</td>';
		echo '<td>';
		$sql = "SELECT turid as codigo,
					   turdesc ||' ( '||i.iusnome||' )' AS descricao
				FROM sisindigena.turmas p 
				INNER JOIN sisindigena.identificacaousuario i ON i.iusd = p.iusd 
				INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_ORIENTADORESTUDO."
				WHERE p.picid='".$dados['picid']."'
				ORDER BY 2";
		
		$db->monta_combo('turid__', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'turid__','', $dados['turid__']);
		echo '</td>';
		
		echo '</tr>';
		
		echo '</table>';
	}
	
	if($dados['pflcod_']==PFL_COORDENADORLOCAL) {
		
		echo '<p align=center><b>Informações Coordenador Local</b></p>';
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		echo '<tr>';
		echo '<td class="SubTituloDireita">Núcleo:</td>';
		echo '<td>';
		echo '<input type="hidden" name="picid__" id="picid__" value="'.$dados['picid'].'">';
		echo $db->pegaUm("SELECT u.unisigla||' - '||u.uninome AS descricao FROM sisindigena.nucleouniversidade p INNER JOIN sisindigena.universidade u ON u.uniid = p.uniid WHERE p.picid='".$dados['picid']."'");
		echo '</td>';
	
		echo '</tr>';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">Rede:</td>';
		echo '<td>';
		carregarRedeTerritorio(array());
		echo '</td>';
		
		echo '</tr>';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">Restrição de bolsas:</td>';
		echo '<td>';
		
		$sql = "SELECT f.fpbid as codigo, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao 
				FROM sisindigena.folhapagamento f 
				INNER JOIN sisindigena.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
				INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
				WHERE f.fpbstatus='A' AND rf.uncid='".$dados['uncid']."'";
		
		$db->monta_combo('fpbidini', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'fpbidini','');
		
		echo ' à ';
		
		$db->monta_combo('fpbidfim', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'fpbidfim','');
		
		echo '</td>';
	
		echo '</tr>';
		
		echo '</table>';
	}
	
	if($dados['pflcod_']==PFL_CONTEUDISTA) {
	
		echo '<p align=center><b>Informações Conteudista</b></p>';
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		echo '<tr>';
		echo '<td class="SubTituloDireita">Núcleo:</td>';
		echo '<td>';
		echo '<input type="hidden" name="picid__" id="picid__" value="'.$dados['picid'].'">';
		echo $db->pegaUm("SELECT u.unisigla||' - '||u.uninome AS descricao FROM sisindigena.nucleouniversidade p INNER JOIN sisindigena.universidade u ON u.uniid = p.uniid WHERE p.picid='".$dados['picid']."'");
		echo '</td>';
	
		echo '</tr>';
	
		echo '<tr>';
		echo '<td class="SubTituloDireita">Restrição de bolsas:</td>';
		echo '<td>';
	
		$sql = "SELECT f.fpbid as codigo, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao
				FROM sisindigena.folhapagamento f
				INNER JOIN sisindigena.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid
				INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
				WHERE f.fpbstatus='A' AND rf.uncid='".$dados['uncid']."'";
	
		$db->monta_combo('fpbidini', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'fpbidini','');
	
		echo ' à ';
	
		$db->monta_combo('fpbidfim', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'fpbidfim','');
	
		echo '</td>';
	
		echo '</tr>';
	
		echo '</table>';
	}
	
	if($dados['pflcod_']==PFL_PESQUISADOR) {
	
		echo '<p align=center><b>Informações Pesquisador</b></p>';
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		echo '<tr>';
		echo '<td class="SubTituloDireita">Núcleo:</td>';
		echo '<td>';
		echo '<input type="hidden" name="picid__" id="picid__" value="'.$dados['picid'].'">';
		echo $db->pegaUm("SELECT u.unisigla||' - '||u.uninome AS descricao FROM sisindigena.nucleouniversidade p INNER JOIN sisindigena.universidade u ON u.uniid = p.uniid WHERE p.picid='".$dados['picid']."'");
		echo '</td>';
		
		echo '</tr>';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">Restrição de bolsas:</td>';
		echo '<td>';
	
		$sql = "SELECT f.fpbid as codigo, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao
				FROM sisindigena.folhapagamento f
				INNER JOIN sisindigena.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid
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
				FROM sisindigena.folhapagamento f 
				INNER JOIN sisindigena.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
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
				FROM sisindigena.folhapagamento f 
				INNER JOIN sisindigena.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
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
			    COALESCE(array_to_string(array(SELECT iusnome FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=pic.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'Coordenador Local não cadastrado') as coordenadorlocal, 
			    COALESCE(array_to_string(array(SELECT iusemailprincipal FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=pic.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'Coordenador Local não cadastrado') as emailcoordenador
			FROM sisindigena.abrangencia a 
			INNER JOIN sisindigena.estruturacurso e ON e.ecuid = a.ecuid 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sisindigena.pactoidadecerta pic ON pic.muncod = a.muncod 
			LEFT JOIN workflow.documento d ON d.docid = pic.docidturma 
			WHERE e.uncid='".$dados['uncid']."' AND a.esfera='M' AND (d.esdid!='".ESD_FECHADO_TURMA."' OR d.esdid IS NULL) ORDER BY 1,2";
	
	$cabecalho = array("UF","Município","Coordenador Local","E-mail");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true,false,false,true);
	
	
}



function efetuarInsercaoUsuarioPerfil($dados) {
	global $db;
	
	$iusd = $db->pegaUm("SELECT iusd FROM sisindigena.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array("",""),$dados['iuscpf__'])."'");

	if($iusd) {

		$sql = "SELECT
					p.pfldsc,
					p.pflcod,
				 	i.picid,
					u.unisigla ||' - '|| u.uninome as uni
				FROM sisindigena.tipoperfil t
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
				INNER JOIN sisindigena.identificacaousuario i ON i.iusd = t.iusd
				LEFT  JOIN sisindigena.nucleouniversidade n ON n.picid = i.picid
				LEFT JOIN sisindigena.universidade u ON u.uniid = n.uniid
				WHERE t.iusd='".$iusd."'";
		$arrPf = $db->pegaLinha($sql);
		
		if($arrPf['pfldsc'] && ($arrPf['pflcod']!=$dados['pflcod__'] || $arrPf['picid']!=$dados['picid__'])) {
		
			$al = array("alert"    => "Inserção não efetivada com sucesso. O usuário ja esta cadastrado com o perfil : ".$arrPf['pfldsc'].", Universidade : ".$arrPf['uni'],
					"location" => $_SERVER['REQUEST_URI']);
		
			alertlocation($al);
		}
	}
	
	if($iusd) {
		
		$sql = "UPDATE sisindigena.identificacaousuario SET 
				picid   =".(($dados['picid__'])?"'".$dados['picid__']."'":"NULL").", 
	    		iuscpf  =".(($dados['iuscpf__'])?"'".str_replace(array(".","-"),array("",""),$dados['iuscpf__'])."'":"NULL").", 
	    		iusnome =".(($dados['iusnome__'])?"'".$dados['iusnome__']."'":"NULL").", 
	    		iusemailprincipal =".(($dados['iusemailprincipal__'])?"'".$dados['iusemailprincipal__']."'":"NULL").", 
	    		iusstatus ='A', 
	    		retid =".(($dados['retid'])?"'".$dados['retid']."'":"NULL").",
	    		iustipoorientador =".(($dados['iustipoorientador__'])?"'".$dados['iustipoorientador__']."'":"NULL").",
	    		muncodatuacao =".(($dados['muncodatuacao__'])?"'".$dados['muncodatuacao__']."'":"NULL").", 
	            uncid =".(($dados['uncid'])?"'".$dados['uncid']."'":"NULL").", 
	            iusformacaoinicialorientador =".(($dados['iusformacaoinicialorientador__'])?$dados['iusformacaoinicialorientador__']:"NULL").", 
	            iustipoprofessor =".(($dados['iustipoprofessor__'])?"'".$dados['iustipoprofessor__']."'":"NULL")."
				WHERE iusd='".$iusd."'";
		
		$db->executar($sql);
		
	} else {
	
		$sql = "INSERT INTO sisindigena.identificacaousuario(
	            picid, iuscpf, iusnome, 
	            iusemailprincipal, iusdatainclusao,  
	            iusstatus, retid)
	    VALUES (".(($dados['picid__'])?"'".$dados['picid__']."'":"NULL").", 
	    		".(($dados['iuscpf__'])?"'".str_replace(array(".","-"),array("",""),$dados['iuscpf__'])."'":"NULL").", 
	    		".(($dados['iusnome__'])?"'".$dados['iusnome__']."'":"NULL").", 
	    		".(($dados['iusemailprincipal__'])?"'".$dados['iusemailprincipal__']."'":"NULL").", 
	    		NOW(), 
	    		'A',
	    		".(($dados['retid'])?"'".$dados['retid']."'":"NULL").") RETURNING iusd";
		
		$iusd = $db->pegaUm($sql);
	
	}

	
	$tpeid = $db->pegaUm("SELECT tpeid FROM sisindigena.tipoperfil WHERE iusd='".$iusd."'");
	
	if(!$tpeid) {
		$sql = "INSERT INTO sisindigena.tipoperfil(
	            iusd, pflcod, fpbidini, fpbidfim)
	    		VALUES ('".$iusd."', '".$dados['pflcod__']."', ".(($dados['fpbidini'])?"'".$dados['fpbidini']."'":"NULL").", ".(($dados['fpbidfim'])?"'".$dados['fpbidfim']."'":"NULL").");";
		
		$db->executar($sql);
	}
	
	if($dados['iusdsup']) {
		
		$dados['turid__'] = $db->pegaUm("SELECT turid FROM sisindigena.turmas WHERE iusd='".$dados['iusdsup']."'");
		if(!$dados['turid__']) {
			
			$sql = "INSERT INTO sisindigena.turmas(
            		iusd, turdesc, turstatus, picid)
    				VALUES ('".$dados['iusdsup']."', 'Turma ".$dados['iusdsup']."', 'A', '".$dados['picid__']."') RETURNING turid;";
			
			$dados['turid__'] = $db->pegaUm($sql);
		}
	}
	
	if($dados['turid__']) {
		$otuid = $db->pegaUm("SELECT otuid FROM sisindigena.orientadorturma WHERE iusd='".$iusd."'");
		
		if(!$otuid) {
			$sql = "INSERT INTO sisindigena.orientadorturma(
	            	turid, iusd)
	    			VALUES ('".$dados['turid__']."', '".$iusd."');";
	
			$db->executar($sql);
		} else {
			
			$sql = "UPDATE sisindigena.orientadorturma SET turid='".$dados['turid__']."' WHERE iusd='".$iusd."';";
			$db->executar($sql);
			
		}
	}
	
	$db->commit();
	
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
			usufoneddd=CASE WHEN foo.usufoneddd IS NULL THEN foo.dddtel::character(2) ELSE foo.usufoneddd END,
			usufonenum=CASE WHEN foo.usufonenum IS NULL THEN foo.tel ELSE foo.usufonenum END,
			muncod=CASE WHEN foo.muncod_segur IS NULL THEN foo.muncod_pacto ELSE foo.muncod_segur END,
			regcod=CASE WHEN foo.estuf_segu IS NULL THEN foo.estuf_pacto ELSE foo.estuf_segu END,
			tpocod=CASE WHEN foo.tpocod IS NULL THEN '1' ELSE foo.tpocod END,
			entid=CASE WHEN foo.entid IS NULL AND foo.orgcod IS NULL THEN 390402 ELSE foo.entid END,
			usudatanascimento=CASE WHEN foo.usudatanascimento IS NULL THEN foo.iusdatanascimento ELSE foo.usudatanascimento END,
			carid=CASE WHEN foo.carid IS NULL THEN 9 ELSE foo.carid END,
			usufuncao=CASE WHEN foo.funcao_segur IS NULL THEN foo.funcao_pacto ELSE foo.funcao_segur END,
			ususexo=CASE WHEN foo.ususexo IS NULL THEN foo.iussexo ELSE foo.ususexo END,
			usunomeguerra=CASE WHEN foo.apelido_segur IS NULL THEN foo.apelido_pacto ELSE foo.apelido_segur END
			FROM(
			SELECT 
			i.iuscpf,
			(SELECT itedddtel FROM sisindigena.identificacaotelefone WHERE iusd=i.iusd AND itetipo='T') as dddtel,
			u.usufoneddd,
			(SELECT itenumtel FROM sisindigena.identificacaotelefone WHERE iusd=i.iusd AND itetipo='T') as tel,
			u.usufonenum,
			i.muncod as muncod_pacto,
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
			p.pfldsc || ' - SISINDÍGENA' as funcao_pacto,
			u.ususexo,
			i.iussexo,
			split_part(i.iusnome, ' ', 1) as apelido_pacto,
			u.usunomeguerra as apelido_segur
			FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.usuario u ON u.usucpf = i.iuscpf 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
			LEFT JOIN territorios.municipio m ON m.muncod = i.muncod 
			WHERE i.iuscpf='".str_replace(array(".","-"),array("",""),$dados['cpf'])."'
			)foo WHERE foo.iuscpf = u.usucpf";
	
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
												   FROM sisindigena.pactoidadecerta p 
						    		  		   	   INNER JOIN workflow.documento d ON d.docid = p.docid 
						    		  		   	   LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
						    		  		   	   LEFT JOIN territorios.estado e ON e.estuf = p.estuf 
						    		  		   	   WHERE p.picid='".$dados['picid']."'");
	
	if($p) {
		
		$db->executar("UPDATE sisindigena.pactoidadecerta SET picselecaopublica=true, picincluirprofessorrede=false WHERE picid='".$p['picid']."'");
		$db->commit();
			
		$ar = array("estuf" 	  => $p['estuf'],
					"muncod" 	  => $p['muncod'],
					"dependencia" => (($p['muncod'])?'municipal':'estadual'));
		
		$totalalfabetizadores = carregarTotalAlfabetizadores($ar);
		
		$orientadoresestudo = carregarDadosIdentificacaoUsuario(array("picid"=>$p['picid'],"pflcod"=>PFL_ORIENTADORESTUDO));
		
		if($totalalfabetizadores['total_orientadores_a_serem_cadastrados'] > count($orientadoresestudo)) {
			$restantes = ($totalalfabetizadores['total_orientadores_a_serem_cadastrados']-count($orientadoresestudo));
			for($i = 0;$i < $restantes;$i++) {
				
				$num_ius = $db->pegaUm("SELECT substr(iuscpf, 8) as num FROM sisindigena.identificacaousuario WHERE picid='".$p['picid']."' AND iuscpf ilike 'SIS%' ORDER BY iusd DESC");
				if($num_ius) $num_ius++;
				else $num_ius=1;
				
				$iuscpf  		   = "SIS".str_pad($p['picid'], 4, "0", STR_PAD_LEFT).str_pad($num_ius, 4, "0", STR_PAD_LEFT);
				$iusnome 		   = "Orientador de Estudo - ".str_replace("'"," ",$p['descricao'])." - ".str_pad($num_ius, 4, "0", STR_PAD_LEFT);
				$iusemailprincipal = "noemail@noemail.com";
				
				if($p['muncod']) {
					$uncid = $db->pegaUm("SELECT e.uncid FROM sisindigena.abrangencia a 
										  INNER JOIN sisindigena.estruturacurso e ON e.ecuid = a.ecuid 
										  WHERE a.muncod='".$p['muncod']."' AND esfera='M'");
				} elseif($p['estuf']) {
					$uncid = $db->pegaUm("SELECT e.uncid FROM sisindigena.abrangencia a 
										  INNER JOIN territorios.municipio m ON m.muncod = a.muncod
										  INNER JOIN sisindigena.estruturacurso e ON e.ecuid = a.ecuid 
										  WHERE m.estuf='".$p['estuf']."' AND esfera='E'");
					
				}
				
				$sql = "INSERT INTO sisindigena.identificacaousuario(picid, 
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
				
				$sql = "INSERT INTO sisindigena.tipoperfil( iusd, pflcod, tpestatus)
    					VALUES ( '".$iusd."', '".PFL_ORIENTADORESTUDO."', 'A');";
				
				$db->executar($sql);
				
				if($uncid) {
					$turid = $db->pegaUm("SELECT t.turid FROM sisindigena.turmas t 
										  INNER JOIN sisindigena.identificacaousuario i ON i.iusd = t.iusd 
										  INNER JOIN sisindigena.tipoperfil tt ON tt.iusd = i.iusd 
										  WHERE tt.pflcod='".PFL_FORMADORIES."' AND i.uncid='".$uncid."' LIMIT 1");
					
					if($turid) {
						$db->executar("INSERT INTO sisindigena.orientadorturma(
									            turid, iusd, otustatus, otudata)
									    VALUES ('".$turid."', '".$iusd."', 'A', NOW());");
					}
				}
				
				
			}
			
			$db->commit();
		} else {
			$al = array("alert"=>"O município selecionado não possui vagas para Orientadores de Estudo.","location"=>"sisindigena.php?modulo=principal/mec/mec&acao=A");
			alertlocation($al);
		}
	
	}
	
	$al = array("alert"=>"Foram inseridos {$restantes} Orientadores de Estudo SIS.","location"=>"sisindigena.php?modulo=principal/mec/mec&acao=A");
	alertlocation($al);
	
	
}

function invalidarMensario($dados) {
	global $db;
	$sql = "SELECT d.esdid FROM sisindigena.mensario m 
			INNER JOIN workflow.documento d ON d.docid = m.docid 
			WHERE m.docid='".$dados['docidmensario']."'";
	$esdidorigem = $db->pegaUm($sql);
	
	$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem='".$esdidorigem."' AND esdiddestino='".ESD_INVALIDADO_MENSARIO."' AND aedstatus='A'";
	$aedid = $db->pegaUm($sql);
	
	if($aedid) {
		wf_alterarEstado( $dados['docidmensario'], $aedid, $dados['cmddsc'], array());
	}

	$al = array("alert"=>"Mensário invalidado com sucesso","location"=>"sisindigena.php?modulo={$dados['modulo']}&acao=A&aba=aprovarusuario&fpbid=".$dados['fpbid']."&pflcodaprovar=".$dados['pflcodaprovar']);
	alertlocation($al);
	
}

function corrigirAcessoUniversidade($dados) {
	global $db;
	$sql = "SELECT i.uncid, i.iuscpf, i.picid, t.pflcod, i.muncodatuacao, i.iusd FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = t.iusd
			WHERE iuscpf='".$_SESSION['usucpf']."'";
	
	$identificacaousuario = $db->pegaLinha($sql);
	
	if($identificacaousuario['uncid']) {
		
		$sql = "UPDATE sisindigena.usuarioresponsabilidade SET uncid='".$identificacaousuario['uncid']."' WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".$identificacaousuario['pflcod']."' AND rpustatus='A'";
		$db->executar($sql);
		$db->commit();

		if($dados['sis']) $_SESSION['sisindigena'][$dados['sis']]['uncid'] = $identificacaousuario['uncid'];
		
	} elseif($identificacaousuario['picid']) {
		
		$sql = "SELECT * FROM sisindigena.pactoidadecerta WHERE picid=".$identificacaousuario['picid'];
		$pactoidadecerta = $db->pegaLinha($sql);
		
		if($pactoidadecerta['estuf'] && $identificacaousuario['muncodatuacao']) {
			$esfera = "E";
			$muncod = $identificacaousuario['muncodatuacao'];
		}
		
		if($pactoidadecerta['muncod']) {
			$esfera = "M";
			$muncod = $pactoidadecerta['muncod'];
		}
		
		$sql = "SELECT uncid FROM sisindigena.abrangencia a 
				INNER JOIN sisindigena.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.muncod='".$muncod."' AND a.esfera='".$esfera."'";
		
		$uncid = $db->pegaUm($sql);
		
		if($uncid) {
			$db->executar("UPDATE sisindigena.identificacaousuario SET uncid='".$uncid."' WHERE iusd='".$identificacaousuario['iusd']."'");
			$db->executar("UPDATE sisindigena.usuarioresponsabilidade SET uncid='".$uncid."' WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".$identificacaousuario['pflcod']."' AND rpustatus='A'");
			$db->commit();
		}
		
		if($dados['sis']) $_SESSION['sisindigena'][$dados['sis']]['uncid'] = $uncid;
		
	}
}

function criarDocumentosPagamentos($dados) {
	global $db;
	
	$pagamentos = $db->carregar("SELECT p.pboid, pf.pfldsc, i.iuscpf, i.iusnome, f.fpbmesreferencia, f.fpbanoreferencia FROM sisindigena.pagamentobolsista p 
								 INNER JOIN seguranca.perfil pf ON pf.pflcod = p.pflcod 
								 INNER JOIN sisindigena.identificacaousuario i ON i.iusd = p.iusd 
								 INNER JOIN sisindigena.folhapagamento f ON f.fpbid = p.fpbid 
								 WHERE docid IS NULL");
	
	if($pagamentos[0]) {
		foreach($pagamentos as $arrInfo) {
			$docid = wf_cadastrarDocumento(TPD_PAGAMENTOBOLSA, "Pagamento - ".$arrInfo['pfldsc']." - (".$arrInfo['iuscpf'].")".$arrInfo['iusnome']." - ".$arrInfo['fpbmesreferencia']."/".$arrInfo['fpbanoreferencia']);
			$db->executar("UPDATE sisindigena.pagamentobolsista SET docid='".$docid."' WHERE pboid='".$arrInfo['pboid']."'");
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
		$db->executar("UPDATE sisindigena.identificacaousuario SET iusnome='".$obj['PESSOA']->no_pessoa_rf."' WHERE iuscpf='".$dados['iuscpf']."'");
		$db->executar("UPDATE seguranca.usuario SET usunome='".$obj['PESSOA']->no_pessoa_rf."' WHERE usucpf='".$dados['iuscpf']."'");
		$db->commit();
	}
	
	$al = array("alert"=>"Nome atualizado com sucesso","location"=>"sisindigena.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=dados");
	alertlocation($al);
	
	
	
}

function aprovarTrocaNomesSGB($dados) {
	global $db;
	
	if($dados['cpf']) {
		foreach($dados['cpf'] as $cpf) {
			if($dados['nome_receita'][$cpf]) {
				$sql = "UPDATE sisindigena.identificacaousuario SET iusnome='".$dados['nome_receita'][$cpf]."' WHERE iuscpf='".str_replace(array(".","-"),array("",""),$cpf)."'";
				$db->executar($sql);
			}	
		}
		$db->commit();
	}
	
	$al = array("alert"=>"Trocas realizadas com sucesso","location"=>"sisindigena.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=aprovarnomes");
	alertlocation($al);
	
}

function pegarRestricaoPagamento($dados) {
	global $db;
	
	$sql = "SELECT 			   CASE WHEN foo.mensarionota < 7		       THEN '<span style=color:red;>A valiação do usuário não atingiu a nota mínima de 7(sete)</span>'
			WHEN foo.iustermocompromisso        =false     THEN '<span style=color:red;>Bolsista não preencheu o termo de compromisso</span>'
			WHEN foo.iusnaodesejosubstituirbolsa=true      THEN '<span style=color:red;>Bolsista do FNDE/MEC e não deseja substituir bolsa atual pela bolsa do PACTO</span>' 
			WHEN foo.fpbidini IS NOT NULL AND foo.fpbidfim IS NOT NULL AND (foo.fpbid < foo.fpbidini OR foo.fpbid > foo.fpbidfim) THEN '<span style=color:red;>Este período de referência não esta habilitado para pagamento</span>' 
			WHEN foo.numeropagamentos >= foo.plpmaximobolsas THEN '<span style=color:red;font-size:x-small;>Atingiu o número máximo ('||foo.plpmaximobolsas||' bolsas)</span>'
			WHEN foo.numeropagamentosvaga >= foo.plpmaximobolsas THEN '<span style=color:red;font-size:x-small;>Atingiu o número máximo ('||foo.plpmaximobolsas||' bolsas)</span>'
			WHEN foo.iusnaodesejosubstituirbolsa=true      THEN '<span style=color:red;font-size:x-small;>Bolsista do FNDE/MEC e não deseja substituir bolsa atual pela bolsa do SISIndígena</span>' 
			ELSE '<span style=color:blue;>Nenhuma restrição - Aguardando aprovação do Coordenador Geral/Adjunto</span>' END as restricao
		FROM (
		SELECT  m.menid,
				i.iustermocompromisso, 
				m.fpbid,
				p.pflcod,
				i.iusdocumento,
				i.iustipoprofessor,
				i.iusnaodesejosubstituirbolsa,
				(SELECT COUNT(mavid) FROM sisindigena.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes,
				COALESCE((SELECT AVG(mavtotal) FROM sisindigena.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
				(SELECT COUNT(pboid) FROM sisindigena.pagamentobolsista pg WHERE pg.iusd=i.iusd) as numeropagamentos,
			    (SELECT COUNT(pboid) FROM sisindigena.pagamentobolsista pg WHERE pg.tpeid=t.tpeid) as numeropagamentosvaga,
				t.fpbidini,
				t.fpbidfim,
				pp.plpmaximobolsas
		FROM sisindigena.mensario m 
		INNER JOIN sisindigena.identificacaousuario i ON i.iusd = m.iusd 
		INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
		INNER JOIN sisindigena.pagamentoperfil pp ON pp.pflcod = t.pflcod
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
				$db->executar("UPDATE sisindigena.pagamentobolsista SEt remid=null WHERE docid='".$docid."'");
				$db->commit();
			}
			
		}
	}
	
	$al = array("alert"=>"Reenvio agendado com sucesso","location"=>"sisindigena.php?modulo=principal/mec/mec&acao=A&aba=reenviarpagamentos");
	alertlocation($al);
	
	
}

function excluirAvaliacoesMensario($dados) {
	global $db;
	
	$db->executar("DELETE FROM sisindigena.historicoreaberturanota WHERE mavid='".$dados['mavid']."'");
	$db->executar("DELETE FROM sisindigena.mensarioavaliacoes WHERE mavid='".$dados['mavid']."'");
	
	$db->commit();
	
	$al = array("alert"=>"Avaliação apagada","location"=>"sisindigena.php?modulo=".$dados['modulo']."&acao=A&aba=".$dados['aba']."&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
	
	
}

function carregarInformes($dados) {
	global $db;
	echo '<p><b>Informes</b></p>';
	
	echo '<div style="background-color:white;height:150px;overflow:auto;">';
	
	$informes = $db->carregar("SELECT inpdescricao, to_char(inpdatainserida,'dd/mm/YYYY HH24:MI') as inpdatainserida FROM sisindigena.informespacto WHERE pflcoddestino='".$dados['pflcoddestino']."' AND inpstatus='A'");
	
	if($informes[0]) {
		foreach($informes as $inf) {
			echo " - ".$inf['inpdescricao']." ( <b>Inserida em ".$inf['inpdatainserida']."</b> )<br>";
		}
	} else {
		echo " - Não existem informes cadastrados";
	}
	
	echo '</div>';
	
	
}


function carregarNucleosUniversidades($dados) {
	global $db;
	
	$sql = "SELECT ".(($dados['consulta'])?"'<img src=\"../imagens/mais.gif\" style=\"cursor:pointer;\" id=\"img2_'||foo.picid||'\" title=mais onclick=\"detalharNucleoUniversidade('||foo.picid||',\''||foo.picsede||'\',this);\">' as acao1":"foo.acao1").",
				   ".(($dados['consulta'])?"'' as acao2":"foo.acao2").",
				   foo.uninome,
				   foo.estuf,
				   foo.mundescricao,
				   foo.coordenadoradjunto
			FROM(
			SELECT '<center><img src=\"../imagens/excluir.gif\" border=0 style=\"cursor:pointer;\" onclick=\"excluirNucleoUniversidade('||u.picid||');\"></center>' as acao1,
				   '<center><img src=\"../imagens/usuario.gif\" border=\"0\" onclick=\"gerenciarCoordenadorAdjunto(\''||u.picid||'\');\" style=\"cursor:pointer;\"></center>' as acao2,
					su.uninome,
					COALESCE((SELECT iusnome FROM sisindigena.identificacaousuario i INNER JOIN sisindigena.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=u.picid AND t.pflcod=".PFL_COORDENADORADJUNTOIES."),'Coordenador Adjunto não cadastrado') as coordenadoradjunto,
					su.uniuf as estuf,
					m.muncod,
					m.mundescricao,
					u.picid,
					CASE WHEN u.picsede=true THEN 't' ELSE 'f' END as picsede
			FROM sisindigena.nucleouniversidade u 
			INNER JOIN sisindigena.universidade su ON su.uniid = u.uniid
			LEFT JOIN territorios.municipio m ON m.muncod = su.muncod 
			WHERE u.uncid='".$dados['uncid']."' AND u.picstatus='A'
			) foo
			ORDER BY foo.uninome";
	
	$cabecalho = array("&nbsp;","&nbsp;","Universidade","UF","Município","Coordenador Adjunto IES");
	$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
		
}


function posDevolverCoordenadorIES() {
	global $db;
	
	$dadosVerificacao = unserialize( stripcslashes( $_REQUEST['verificacao'] ) );
	
	if($dadosVerificacao['picid'] && $_SESSION['sisindigena']['coordenadoradjuntoies']) {
		$iusd   = $_SESSION['sisindigena']['coordenadoradjuntoies']['iusd'];
		$pflcod = PFL_COORDENADORADJUNTOIES; 
		$docid  = $_SESSION['sisindigena']['coordenadoradjuntoies']['docid'];
	}
	
	if($dadosVerificacao['picid'] && $_SESSION['sisindigena']['supervisories']) {
		$iusd   = $_SESSION['sisindigena']['supervisories']['iusd'];
		$pflcod = PFL_SUPERVISORIES;
		$docid  = $_SESSION['sisindigena']['supervisories']['docid'];
	}
	
	if($dadosVerificacao['uncid']) {
		$iusd   = $_SESSION['sisindigena']['universidade']['iusd'];
		$pflcod = PFL_COORDENADORIES;
		$docid  = $_SESSION['sisindigena']['universidade']['docid'];
	}
	
	if(!strstr($_SERVER['HTTP_HOST'],"simec-local")) {
	
		$sql = "SELECT c.cmddsc FROM workflow.documento d 
				INNER JOIN workflow.comentariodocumento c ON c.hstid = d.hstid 
				WHERE d.docid='".$docid."'";
		
		$cmddsc = $db->pegaUm($sql);
		
		$sql = "SELECT i.iusnome, i.iusemailprincipal FROM sisindigena.identificacaousuario i 
				INNER JOIN sisindigena.tipoperfil t  ON t.iusd = i.iusd AND t.pflcod='".$pflcod."'
				WHERE i.iusd='".$iusd."'";
		
		$identificacaousuario = $db->pegaLinha($sql);
		
		require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
		require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISINDIGENA - Devolução do Cadastramento para alterações";
		
		$mensagem->AddAddress( $identificacaousuario['iusemailprincipal'], $identificacaousuario['iusnome'] );
		$mensagem->AddAddress( $_SESSION['email_sistema'] );
		
			
		$mensagem->Body = "<p>Prezado(a) ".$identificacaousuario['iusnome']."</p>
						   <p>Seu trabalho no SISINDIGENA foi analisado e foi devolvido para alterações.</p>
						   <p>As observações do avaliador foram:</p>".$cmddsc."<br/><p>Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão<br/>Ministério da Educação</p>";
		
		$mensagem->IsHTML( true );
		$mensagem->Send();
	
	}
	
	return true;
	
}

function carregarUsuarioPerfil($dados) {
	global $db;
	
	$sql = "SELECT i.iusd as codigo, i.iusnome as descricao FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sisindigena.orientadorturma o ON o.iusd = i.iusd 
			INNER JOIN sisindigena.turmas tu ON tu.turid = o.turid 
			INNER JOIN sisindigena.identificacaousuario i2 ON i2.iusd = tu.iusd
			WHERE t.pflcod='".$dados['pflcod']."' AND i.picid='".$dados['picid']."' AND i2.iusd='".$dados['iusd']."'";
	
	$db->monta_combo('iusdsup', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'iusdsup','', '');
}


function exibirCadastramentoSupervisor($dados) {
	global $db;
	
	?>
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td width="50%"><img src="../imagens/seta_filho.gif" align="absmiddle"> <b>Orientadores de Estudo</b></td>
			<td class="SubTituloDireita">&nbsp;</td>
		</tr>
	</table>
	<?
	$sql = "SELECT CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END as iuscpf, i.iusnome, i.iusemailprincipal, CASE WHEN i.iustermocompromisso=true THEN '<img src=../imagens/valida1.gif>' ELSE '<img src=../imagens/valida3.gif>' END as termo, i2.iusnome as supnome
			FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sisindigena.orientadorturma o ON o.iusd = i.iusd 
			INNER JOIN sisindigena.turmas tu ON tu.turid = o.turid 
			INNER JOIN sisindigena.identificacaousuario i2 ON i2.iusd = tu.iusd
			WHERE t.pflcod='".PFL_ORIENTADORESTUDO."' AND i2.iusd='".$dados['iusd']."' AND i.picid='".$dados['picid']."' ORDER BY i.iusnome";
	
	$cabecalho = array("CPF","Nome","Email","Termo","Supervisor");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
	?>


	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td width="50%"><img src="../imagens/seta_filho.gif" align="absmiddle"> <b>Professores Alfabetizadores</b></td>
			<td class="SubTituloDireita">&nbsp;</td>
		</tr>
	</table>
	<?
	$sql = "SELECT CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END as iuscpf, i.iusnome, i.iusemailprincipal, CASE WHEN i.iustermocompromisso=true THEN '<img src=../imagens/valida1.gif>' ELSE '<img src=../imagens/valida3.gif>' END as termo, i2.iusnome as orinome 
			FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sisindigena.orientadorturma o ON o.iusd = i.iusd
			INNER JOIN sisindigena.turmas tu ON tu.turid = o.turid 
			INNER JOIN sisindigena.identificacaousuario i2 ON i2.iusd = tu.iusd
			INNER JOIN sisindigena.orientadorturma o2 ON o2.iusd = i2.iusd
			INNER JOIN sisindigena.turmas tu2 ON tu2.turid = o2.turid 
			INNER JOIN sisindigena.identificacaousuario i3 ON i3.iusd = tu2.iusd
			WHERE t.pflcod='".PFL_PROFESSORALFABETIZADOR."' AND i.picid='".$dados['picid']."' AND i3.iusd='".$dados['iusd']."' 
			ORDER BY i2.iusnome, i.iusnome";
	
	$cabecalho = array("CPF","Nome","Email","Termo","Orientador");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
	
}


function exibirCadastramentoCoordenadorAdjunto($dados) {
	global $db;
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Projeto Pedagógico</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%">Abrangência da Ação Saberes Indígenas</td>
			<td>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
			<tr>
				<td class="SubTituloEsquerda">Território Etnoeducacionais</td>
			</tr>
			<tr>
				<td><? quadroAbrangenciaAcao(array('laatipo'=>'etnoeducacionais','picid'=>$dados['picid'],'visrelatorio'=>true)) ?></td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda">Povos</td>
			</tr>
			<tr>
				<td><? quadroAbrangenciaAcao(array('laatipo'=>'povos','picid'=>$dados['picid'],'visrelatorio'=>true)) ?></td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda">Aldeias</td>
			</tr>
			<tr>
				<td id="td_aldeia"><? quadroAbrangenciaAcao(array('grid'=>'aldeia','picid'=>$dados['picid'],'visrelatorio'=>true)) ?></td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda">Línguas Faladas</td>
			</tr>
			<tr>
				<td><? quadroAbrangenciaAcao(array('laatipo'=>'lingua','picid'=>$dados['picid'],'visrelatorio'=>true)) ?></td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda">Mapas</td>
			</tr>
			<tr>
				<td id="td_mapa"><? quadroAbrangenciaAcao(array('grid'=>'mapa','picid'=>$dados['picid'],'visrelatorio'=>true)) ?></td>
			</tr>
			<tr>
				<td class="SubTituloEsquerda">Escolas Atendidas</td>
			</tr>
			<tr>
				<td id="td_escola_atendida"><? quadroAbrangenciaAcao(array('grid'=>'escola_atendida','picid'=>$dados['picid'],'visrelatorio'=>true)) ?></td>
			</tr>
	
	
			</table>
		</tr>	
		
		<tr>
			<td class="SubTituloDireita" width="20%">Eixos da Formação Continuada Desenvolvidos</td>
			<td>
			<? 
	 		$sql = "SELECT * FROM sisindigena.eixospedagogicos WHERE expstatus='A'";
	 		$eixospedagogicos = $db->carregar($sql);
			?>
			
			<? if($eixospedagogicos[0]) : ?>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
			<? foreach($eixospedagogicos as $ep) : ?>
			<?
			$eixospedagogicosuniversidade = $db->pegaLinha("SELECT epuid, eprobs FROM sisindigena.eixospedagogicosnucleo WHERE expid='".$ep['expid']."' AND picid='".$dados['picid']."'");
			$obs = 'eprobs_'.$ep['expid'];
			$$obs = $eixospedagogicosuniversidade['eprobs'];
			?>
			<tr>
				<td width="5"><input type="checkbox" disabled name="expid[]" value="<?=$ep['expid'] ?>" <?=(($eixospedagogicosuniversidade['epuid'])?"checked":"") ?> onclick="selecionarEixoPedagogico(this);"></td>
				<td><?=$ep['expdsc'] ?></td>
			</tr>
			<tr id="tr_eprobs_<?=$ep['expid'] ?>" <?=(($eixospedagogicosuniversidade['epuid'])?"":"style=\"display:none;\"") ?> >
				<td colspan="2">
				<? $o = "eprobs_".$ep['expid']; ?>
				<p><? echo $$o; ?></p>
				</td>
			</tr>
			<? endforeach; ?>
			</table>
			<? else : ?>
			Não existem eixos pedagógicos
			<? endif; ?>
			</td>
		</tr>	
		
		<? 
		
		$nucleouniversidade = $db->pegaLinha("SELECT picmetodologiaaplicada, 
													 picmetodologiaavaliacao, 
													 picmetodologiaacompanhamento 
											  FROM sisindigena.nucleouniversidade 
											  WHERE picid='".$dados['picid']."'");
	
		extract($nucleouniversidade);
		?>
		
		<tr>
			<td class="SubTituloDireita" width="20%">Metodologia</td>
			<td>
			<p><? echo $picmetodologiaaplicada; ?></p>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita" width="20%">Processo de Avaliação</td>
			<td>
			<p><b>Avaliação do Curso</b></p>
			<p><? echo $picmetodologiaavaliacao; ?></p>
			<br>
			<p><b>Avaliação do Cursista</b></p>
			<p><? echo $picmetodologiaacompanhamento; ?></p>
			</td>
		</tr>
	
	</table>

	<table align="center" width="95%">
	<tr><td>
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td class="SubTituloCentro">Definição da Equipe</td>
		</tr>
	</table>


	<? if($dados['picsede'] == 't') : ?>
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td width="50%"><img src="../imagens/seta_filho.gif" align="absmiddle"> <b>Formador Pesquisador</b></td>
			<td class="SubTituloDireita">&nbsp;</td>
		</tr>
	</table>
	<?
	$sql = "SELECT CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END as iuscpf, i.iusnome, i.iusemailprincipal 
			FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			WHERE t.pflcod='".PFL_PESQUISADOR."' AND i.picid='".$dados['picid']."'";
	
	$cabecalho = array("CPF","Nome","Email");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
	?>
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td width="50%"><img src="../imagens/seta_filho.gif" align="absmiddle"> <b>Formador Conteudista</b></td>
			<td class="SubTituloDireita">&nbsp;</td>
		</tr>
	</table>
	<?
	$sql = "SELECT CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END as iuscpf, i.iusnome, i.iusemailprincipal 
			FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			WHERE t.pflcod='".PFL_CONTEUDISTA."' AND i.picid='".$dados['picid']."'";
	
	$cabecalho = array("CPF","Nome","Email");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
	?>

	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td width="50%"><img src="../imagens/seta_filho.gif" align="absmiddle"> <b>Formador</b></td>
			<td class="SubTituloDireita">&nbsp;</td>
		</tr>
	</table>
	<?
	$sql = "SELECT CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END as iuscpf, i.iusnome, i.iusemailprincipal 
			FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			WHERE t.pflcod='".PFL_FORMADORIES."' AND i.picid='".$dados['picid']."'";
	
	$cabecalho = array("CPF","Nome","Email");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
	?>
	
	
	<? endif; ?>
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td width="50%"><img src="../imagens/seta_filho.gif" align="absmiddle"> <b>Coordenador da Ação</b></td>
			<td class="SubTituloDireita">&nbsp;</td>
		</tr>
	</table>
	<?
	$sql = "SELECT CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END as iuscpf, 
				   i.iusnome, 
				   i.iusemailprincipal, 
				   CASE WHEN m.muncod IS NOT NULL THEN m.estuf ||' / ' || m.mundescricao || ' ( Municipal )' 
				   		WHEN e.estuf IS NOT NULL THEN e.estuf ||' / ' || e.estdescricao || ' ( Estadual )' END as rede
																																		 
			FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sisindigena.redeterritorios r ON r.retid = i.retid 
			LEFT JOIN territorios.municipio m ON m.muncod = r.muncod 
			LEFT JOIN territorios.estado e ON e.estuf = r.estuf 
			WHERE t.pflcod='".PFL_COORDENADORLOCAL."' AND i.picid='".$dados['picid']."'";
	
	$cabecalho = array("CPF","Nome","Email","Rede");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
	?>
	
	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
		<tr>
			<td width="50%"><img src="../imagens/seta_filho.gif" align="absmiddle"> <b>Supervisor</b></td>
			<td class="SubTituloDireita">&nbsp;</td>
		</tr>
	</table>
	<?
	$sql = "SELECT '<img src=../imagens/mais.gif id=\"img_'||i.iusd||'\" title=mais onclick=\"detalharSupervisor('||i.iusd||','||i.picid||',this);\">' as mais, replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.'), i.iusnome, i.iusemailprincipal 
			FROM sisindigena.identificacaousuario i 
			INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd 
			WHERE t.pflcod='".PFL_SUPERVISORIES."' AND i.picid='".$dados['picid']."'";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email");
	$db->monta_lista_simples($sql,$cabecalho,10000,5,'N','100%','N',true,false,false,true);
	?>
	</td>
	</tr>
	</table>
	<? 
}

function esconderAba($dados) {
	return false;
}

function verificarValidacao($dados) {
	global $db;
	
	if(!$dados['docid']) {
		$al = array("alert"=>"O Fluxo de cadastramento não foi encontrado","location"=>$_SERVER['HTTP_REFERER']);
		alertlocation($al);
	}
	
	$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$dados['docid']."'");
	
	if($esdid != ESD_VALIDADO_COORDENADOR_IES) {

		$al = array("alert"=>"O Fluxo de cadastramento não foi validado pelo MEC","location"=>$_SERVER['HTTP_REFERER']);
		alertlocation($al);
		
	}
}

function condicaoRetornarMEC() {
	global $db;
	
	
	if($_SESSION['sisindigena']['coordenadoradjuntoies']['picid']) {
		$sql = "SELECT count(*) as numero FROM sisindigena.folhapagamentouniversidade WHERE picid='".$_SESSION['sisindigena']['coordenadoradjuntoies']['picid']."'";
		$numero = $db->pegaUm($sql);
		
		if($numero) {
			return 'Não é possível retornar para análise, pois o núcleo esta em execução';
		}
	}
	
	if($_SESSION['sisindigena']['supervisories']['picid']) {
		$sql = "SELECT count(*) as numero FROM sisindigena.folhapagamentouniversidade WHERE picid='".$_SESSION['sisindigena']['supervisories']['picid']."'";
		$numero = $db->pegaUm($sql);
		
		if($numero) {
			return 'Não é possível retornar para análise, pois o núcleo esta em execução';
		}
	}
	
	if($_SESSION['sisindigena']['universidade']['uncid']) {
		$sql = "SELECT count(*) as numero FROM sisindigena.folhapagamentouniversidade WHERE uncid='".$_SESSION['sisindigena']['universidade']['uncid']."'";
		$numero = $db->pegaUm($sql);
	
		if($numero) {
			return 'Não é possível retornar para análise, pois existem núcleos em execução';
		}
	}
	
	
	return true;
}


function inserirDocumento($dados) {
	global $db;
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$campos = array("picid"   => (($_SESSION['sisindigena']['coordenadoradjuntoies']['picid'])?"'".$_SESSION['sisindigena']['coordenadoradjuntoies']['picid']."'":"NULL"),
					"uncid"   => (($_SESSION['sisindigena']['universidade']['uncid'])?"'".$_SESSION['sisindigena']['universidade']['uncid']."'":"NULL"),
					"domdsc"  => "'".$_REQUEST['observacoes']."'",
					"domtipo" => "'".$_REQUEST['tipo']."'");

	$file = new FilesSimec( "documentos", $campos, "sisindigena" );
	$file->setUpload( NULL, "arquivo" );
	
	$al = array("alert" => "Documento salvo com sucesso","location" => "sisindigena.php?modulo=".$dados['modulo']."&acao=A&aba=documentos");
	alertlocation($al);

}

function excluirDocumento($dados) {
	global $db;
	
	$db->executar("DELETE FROM sisindigena.documentos WHERE domid='".$dados['domid']."'");
	$db->commit();
	
	$al = array("alert" => "Documento excluído com sucesso","location" => "sisindigena.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoiesexecucao&acao=A&aba=documentos");
	alertlocation($al);
	
	
}
?>