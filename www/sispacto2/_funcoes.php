<?
include_once '_funcoes_avaliacoes.php';


function removerTipoPerfil($dados) {
	global $db;
	
	// verificando pagamento
	$sql = "SELECT p.pboid FROM sispacto2.tipoperfil t 
			INNER JOIN sispacto2.pagamentobolsista p ON p.tpeid = t.tpeid  
			WHERE t.iusd='".$dados['iusd']."' AND t.pflcod='".$dados['pflcod']."'";
	
	$pboid = $db->pegaUm($sql);
	
	if($pboid) {
		if(!$dados['naoredirecionar']) {
			if($dados['picid']) $al = array("alert"=>"Coordenador Local ja possui pagamento e não pode ser removido, somente substituido","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=".$dados['picid']);
			if($dados['uncid']) $al = array("alert"=>"Coordenador IES ja possui pagamento e não pode ser removido, somente substituido","location"=>"sispacto2.php?modulo=principal/universidade/gerenciarcoordenadories&acao=A&uncid=".$dados['uncid']);
			alertlocation($al);
		} else {
			return false;
		}
	}
	
	$sql = "DELETE FROM sispacto2.tipoperfil WHERE iusd='".$dados['iusd']."' AND pflcod='".$dados['pflcod']."'";
	$db->executar($sql);
	
	$usucpf = $db->pegaUm("SELECT iuscpf FROM sispacto2.identificacaousuario WHERE iusd='".$dados['iusd']."'");
	
	if($usucpf) {
		$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$usucpf."' AND pflcod='".$dados['pflcod']."'";
		$db->executar($sql);
		$sql = "DELETE FROM sispacto2.usuarioresponsabilidade WHERE usucpf='".$usucpf."' AND pflcod='".$dados['pflcod']."'";
		$db->executar($sql);
	}
	
	$sql = "DELETE FROM sispacto2.orientadorturma WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM sispacto2.orientadorturmaoutros WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	$sql = "INSERT INTO sispacto2.historicoidentificaousuario(
            iusd, hiudatainc, hiucpf, hiulog, hiustatus, hiutipo)
    		VALUES ('".$dados['iusd']."', NOW(), '".$_SESSION['usucpf']."', '".addslashes(str_replace(array("'"),array(""),simec_json_encode($dados)))."', 'A', 'removerTipoPerfil');";
	$db->executar($sql);
	
	$db->commit();
	
	if(!$dados['naoredirecionar']) {
		if($dados['picid']) $al = array("alert"=>"Coordenador Local removido com sucesso","location"=>"sispacto2.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=".$dados['picid']);
		if($dados['uncid']) $al = array("alert"=>"Coordenador IES removido com sucesso","location"=>"sispacto2.php?modulo=principal/universidade/gerenciarcoordenadories&acao=A&uncid=".$dados['uncid']);
		alertlocation($al);
	}
	
}

function verificaPermissao() {
	global $db;
	$perfis = pegaPerfilGeral();
	$sql = "SELECT * FROM sispacto2.usuarioresponsabilidade WHERE usucpf='".$_SESSION['usucpf']."' AND rpustatus='A'";
	$ur = $db->carregar($sql);
	
	if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis)) {
		return false;
	}
	
	if(in_array(PFL_COORDENADORLOCAL,$perfis)) {
		if($_REQUEST['modulo']=='principal/coordenadorlocal/coordenadorlocal' || $_REQUEST['modulo']=='principal/coordenadorlocal/coordenadorlocalexecucao') {
			if($ur[0]) {
				foreach($ur as $urr) {
					if($urr['pflcod']==PFL_COORDENADORLOCAL && $urr['muncod']==$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['muncod']) {
						return false;
					}
				}
			}
		}
	}
	
	if(in_array(PFL_EQUIPEMUNICIPALAP,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_EQUIPEMUNICIPALAP && $urr['muncod']==$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['muncod']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_CONSULTAMUNICIPAL,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_CONSULTAMUNICIPAL && $urr['muncod']==$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['muncod']) {
					return true;
				}
			}
		}
	}
	
	if(in_array(PFL_EQUIPEESTADUALAP,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_EQUIPEESTADUALAP && $urr['estuf']==$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['estuf']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_CONSULTAESTADUAL,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_CONSULTAESTADUAL && $urr['estuf']==$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['estuf']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_COORDENADORIES,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_COORDENADORIES && $urr['uncid']==$_SESSION['sispacto2']['universidade']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_COORDENADORADJUNTOIES,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_COORDENADORADJUNTOIES && $urr['uncid']==$_SESSION['sispacto2']['coordenadoradjuntoies']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_PROFESSORALFABETIZADOR,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_PROFESSORALFABETIZADOR && $urr['uncid']==$_SESSION['sispacto2']['professoralfabetizador']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_ORIENTADORESTUDO,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_ORIENTADORESTUDO && $urr['uncid']==$_SESSION['sispacto2']['orientadorestudo']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_FORMADORIES,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_FORMADORIES && $urr['uncid']==$_SESSION['sispacto2']['formadories']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_FORMADORIESP,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_FORMADORIESP && $urr['uncid']==$_SESSION['sispacto2']['formadoriesp']['uncid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_SUPERVISORIES,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_SUPERVISORIES && $urr['uncid']==$_SESSION['sispacto2']['supervisories']['uncid']) {
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


function montaAbasSispacto($abapai, $abaativa) {
	global $db;
	
	$sql = "SELECT abaordem, abadsc, abaendereco, abafuncaomostrar, abapai FROM sispacto2.abas WHERE abapai='".$abapai."' ORDER BY abaordem";
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

function carregarOrientacaoPorFiltro($dados) {
	global $db;

	$sql = "SELECT oabdesc FROM sispacto2.orientacaoaba WHERE abaid='".$dados['abaid']."'";
	$oabdesc = $db->pegaUm($sql);

	echo $oabdesc;
}

function carregarDadosIdentificacaoUsuario($dados) {
	global $db;
	
	if(!$dados['pflcod']) {
		$al = array("alert"=>"Problemas para carregar os dados usuário","location"=>"sispacto2.php?modulo=inicio&acao=C");
		alertlocation($al);
	}
	
	$sql = "SELECT i.cadastradosgb, i.uncid, i.iusd, i.iuscpf, i.iusnome, i.iusdatanascimento, i.iusnomemae, i.iustipoprofessor, i.iusnaodesejosubstituirbolsa,
				   i.iussexo, i.eciid, i.nacid, i.iusnomeconjuge, i.iusagenciasugerida, i.iusagenciaend, i.iusformacaoinicialorientador,
				   i.iusemailprincipal, i.iusemailopcional, i.iustipoorientador, to_char(i.iusdatainclusao,'YYYY-mm-dd') as iusdatainclusao, i.iustermocompromisso,  
				   i.tvpid, i.funid, i.foeid, f.iufid, f.cufid, f.iufdatainiformacao, f.iufdatafimformacao, f.iufsituacaoformacao,
				   m.estuf as estuf_nascimento, m.muncod as muncod_nascimento, ma.estuf||' / '||ma.mundescricao as municipiodescricaoatuacao, ma.muncod as muncodatuacao, ma.estuf as estufatuacao,
				   d.itdid, d.tdoid, d.itdufdoc, d.itdnumdoc, d.itddataexp, d.itdnoorgaoexp,
				   e.ienid, mm.muncod as muncod_endereco, mm.estuf as estuf_endereco,
				   e.ientipo, e.iencep, e.iencomplemento, e.iennumero, e.ienlogradouro, e.ienbairro, cf.cufcodareageral, to_char(t.tpeatuacaoinicio,'YYYY-mm-dd') as tpeatuacaoinicio, to_char(t.tpeatuacaofim,'YYYY-mm-dd') as tpeatuacaofim, i.iusserieprofessor   
			FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			LEFT  JOIN territorios.municipio m ON m.muncod = i.muncod 
			LEFT  JOIN sispacto2.identiusucursoformacao f ON f.iusd = i.iusd 
			LEFT  JOIN sispacto2.identusutipodocumento d ON d.iusd = i.iusd 
			LEFT  JOIN sispacto2.identificaoendereco e ON e.iusd = i.iusd 
			LEFT  JOIN territorios.municipio mm ON mm.muncod = e.muncod 
			LEFT  JOIN territorios.municipio ma ON ma.muncod = i.muncodatuacao
			LEFT  JOIN sispacto2.cursoformacao cf ON cf.cufid = f.cufid 
			LEFT  JOIN sispacto2.orientadorturma ot ON ot.iusd = i.iusd 
			WHERE t.pflcod='".$dados['pflcod']."' ".(($dados['uncid'])?" AND i.uncid='".$dados['uncid']."'":"")." ".(($dados['picid'])?" AND i.picid='".$dados['picid']."'":"")." ".(($dados['turid'])?" AND ot.turid='".$dados['turid']."'":"")." ".(($dados['iustipoorientador'])?" AND i.iustipoorientador='".$dados['iustipoorientador']."'":"")." ".(($dados['tpejustificativaformadories'])?" AND t.tpejustificativaformadories IS NOT NULL":"")." ".(($dados['iusd'])?" AND i.iusd='".$dados['iusd']."'":"")." AND iusstatus='A' ORDER BY i.iusd";
	
	$identificacaousuario = $db->carregar($sql);
	
	if($identificacaousuario[0]) {

		foreach($identificacaousuario as $key => $iu) {
			
			$idusuarios[$key] = $iu;
			unset($telefones);
			$sql = "SELECT itetipo, itedddtel, itenumtel FROM sispacto2.identificacaotelefone WHERE iusd='".$iu['iusd']."'";
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
	
	$sql = "UPDATE seguranca.usuario_sistema SET suscod='A' WHERE usucpf='".$dados['usucpf']."' AND sisid='".SIS_SISPACTO."'";
	$db->executar($sql);
	
	$db->commit();
	
	$arrUsu = $db->pegaLinha("SELECT usunome, usuemail FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['usucpf'])."'");
	
	$remetente = array("nome" => "SIMEC - MÓDULO SISPACTO","email" => $arrUsu['usuemail']);
 	$destinatario = $arrUsu['usuemail'];
 	$usunome = $arrUsu['usunome'];
 	
 	$assunto = "Atualização de senha no SIMEC - MÓDULO SISPACTO";
 	$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
	$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo SISPACTO. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
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
	
	$al = array("alert"=>"Senha reiniciada com sucesso","location"=>"sispacto2.php?modulo=".$dados['modulo']."&acao=A&aba=".$dados['aba']);
	alertlocation($al);
	
	
}

function pegarDadosUsuarioPorCPF($dados) {
	global $db;
	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf='".$dados['cpf']."'";
	$usuemail = $db->pegaUm($sql);
	
	$sql = "SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf='".$dados['cpf']."' AND sisid='".SIS_SISPACTO."'";
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
	
	$iusagenciasugerida_atual = $db->pegaUm("SELECT iusagenciasugerida FROM sispacto2.identificacaousuario WHERE iusd='".$dados['iusd']."'");
	if($iusagenciasugerida_atual != substr($dados['iusagenciasugerida'],0,4)) {
		$sqlsgb = "cadastradosgb=FALSE,";
	}
	

	$sql = "UPDATE sispacto2.identificacaousuario SET
			iusdatanascimento = '".formata_data_sql($dados['iusdatanascimento'])."',
			iusnomemae		  = '".$dados['iusnomemae']."',
			iussexo 		  = '".$dados['iussexo']."',
			muncod		  	  = '".$dados['muncod_nascimento']."',
			eciid 		  	  = '".$dados['eciid']."',
			nacid		  	  = '".$dados['nacid']."',
			iusnomeconjuge	  = '".$dados['iusnomeconjuge']."',
			iusagenciasugerida = '".substr($dados['iusagenciasugerida'],0,4)."',
			iusagenciaend = '".substr(addslashes($dados['iusagenciaend']),0,250)."',
			tvpid = '".$dados['tvpid']."',
			funid = '".$dados['funid']."',
			foeid = '".$dados['foeid']."',
			{$sqlsgb}
			iusemailprincipal = '".$dados['iusemailprincipal']."',
			iusemailopcional=".(($dados['iusemailopcional'])?"'".$dados['iusemailopcional']."'":"NULL").",
			iusnaodesejosubstituirbolsa=".(($dados['iusnaodesejosubstituirbolsa']=='TRUE')?"TRUE":"FALSE").",
			muncodatuacao=".(($dados['muncod_abrangencia'])?"'".$dados['muncod_abrangencia']."'":"NULL").",
			iustermocompromisso=TRUE
			WHERE iusd='".$dados['iusd']."'";
	
	$db->executar($sql);
	
	$erros = validarFormacao($dados);
	
	if($erros) {
		$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erros),"location"=>$dados['goto']);
		alertlocation($al);
	}
	

	$iufid = $db->pegaUm("SELECT iufid FROM sispacto2.identiusucursoformacao WHERE iusd='".$dados['iusd']."'");
	
	// controlando formação
	if($iufid) {
		
		$sql = "UPDATE sispacto2.identiusucursoformacao SET
		            cufid=".(($dados['cufid'])?"'".$dados['cufid']."'":"NULL").", 
		            iufdatainiformacao='".formata_data_sql($dados['iufdatainiformacao'])."', 
		            iufdatafimformacao=".(($dados['iufdatafimformacao'])?"'".formata_data_sql($dados['iufdatafimformacao'])."'":"NULL").", 
		            iufsituacaoformacao='".$dados['iufsituacaoformacao']."'
		        WHERE iufid='".$iufid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sispacto2.identiusucursoformacao(
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
	
	
	$itdid = $db->pegaUm("SELECT itdid FROM sispacto2.identusutipodocumento WHERE iusd='".$dados['iusd']."'");
	
	// controlando documento
	if($itdid) {
		
		$sql = "UPDATE sispacto2.identusutipodocumento SET
            	tdoid='".$dados['tdoid']."', itdufdoc='".$dados['itdufdoc']."', 
            	itdnumdoc='".$dados['itdnumdoc']."', itddataexp='".formata_data_sql($dados['itddataexp'])."', 
            	itdnoorgaoexp='".$dados['itdnoorgaoexp']."'		
		        WHERE itdid='".$itdid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sispacto2.identusutipodocumento(
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
	
	$ienid = $db->pegaUm("SELECT ienid FROM sispacto2.identificaoendereco WHERE iusd='".$dados['iusd']."'");
	
	// controlando endereço
	if($ienid) {
		
		$sql = "UPDATE sispacto2.identificaoendereco SET
            	muncod='".substr($dados['muncod_endereco'],0,7)."', ientipo='".$dados['ientipo']."', 
            	iencep='".str_replace(array("-"),array(""),$dados['iencep'])."', iencomplemento=".(($dados['iencomplemento'])?"'".addslashes($dados['iencomplemento'])."'":"NULL").", 
            	iennumero=".((!is_null($dados['iennumero']) && is_numeric($dados['iennumero']))?"'".$dados['iennumero']."'":"NULL").", ienlogradouro='".substr(addslashes($dados['ienlogradouro']),0,100)."', 
            	ienbairro='".addslashes($dados['ienbairro'])."' 		
		        WHERE ienid='".$ienid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sispacto2.identificaoendereco(
            	muncod, iusd, ientipo, iencep, iencomplemento, iennumero, 
            	iensatatus, ienlogradouro, ienbairro)
    			VALUES ('".substr($dados['muncod_endereco'],0,7)."', '".$dados['iusd']."', '".$dados['ientipo']."', '".str_replace(array("-"),array(""),$dados['iencep'])."', 
    					".(($dados['iencomplemento'])?"'".addslashes($dados['iencomplemento'])."'":"NULL").", ".((!is_null($dados['iennumero']))?"'".str_replace(array(" "),array(""),$dados['iennumero'])."'":"NULL").", 'A', '".addslashes($dados['ienlogradouro'])."', '".substr(addslashes($dados['ienbairro']),0,60)."');";
		
		$db->executar($sql);
		
	}
	
	// controlando telefones
	$db->executar("DELETE FROM sispacto2.identificacaotelefone WHERE iusd='".$dados['iusd']."'");
	
	$tipos = array("R","T","C","F");
	
	foreach($tipos as $tipo) {
		
		$sql = "INSERT INTO sispacto2.identificacaotelefone(
            	iusd, itedddtel, itenumtel, itetipo, itestatus)
    			VALUES ('".$dados['iusd']."', ".(($dados['itedddtel'][$tipo])?"'".$dados['itedddtel'][$tipo]."'":"NULL").", ".(($dados['itenumtel'][$tipo])?"'".$dados['itenumtel'][$tipo]."'":"NULL").", '".$tipo."', 'A');";
		
		$db->executar($sql);
		
	}
	
	$sql = "INSERT INTO sispacto2.historicoidentificaousuario(
            iusd, hiudatainc, hiucpf, hiulog, hiustatus, hiutipo)
    		VALUES ('".$dados['iusd']."', NOW(), '".$_SESSION['usucpf']."', '".addslashes(str_replace(array("'"),array(""),simec_json_encode($dados)))."', 'A', 'atualizarDadosIdentificacaoUsuario');";
	$db->executar($sql);
	
	$sql = "UPDATE sispacto2.tipoperfil SET tpeatuacaoinicio=".(($dados['tpeatuacaoinicio_mes'] && $dados['tpeatuacaoinicio_ano'])?"'".$dados['tpeatuacaoinicio_ano']."-".$dados['tpeatuacaoinicio_mes']."-01'":"NULL").", 
										   tpeatuacaofim=".(($dados['tpeatuacaofim_mes'] && $dados['tpeatuacaofim_ano'])?"'".$dados['tpeatuacaofim_ano']."-".$dados['tpeatuacaofim_mes']."-01'":"NULL")." WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	$db->commit();
	
	sincronizarUsuariosSIMEC(array('cpf' => $dados['iuscpf']));
	
	$al = array("alert"=>$dados['mensagemalert'],"location"=>$dados['goto']);
	alertlocation($al);
	
}



function carregarOrientacao($endereco) {
	global $db;

	$sql = "SELECT a.abaid, o.oabdesc FROM sispacto2.abas a
			LEFT JOIN sispacto2.orientacaoaba o ON o.abaid = a.abaid
			WHERE a.abaendereco='".$endereco."'";

	$abas = $db->pegaLinha($sql);

	$orientacao = $abas['oabdesc'];
	$abaid      = $abas['abaid'];
	
	$perfis = pegaPerfilGeral();
	
	if(!$perfis) $perfis = array();
	
	if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis)) {
		$htmladm = "<br><img src=\"../imagens/page_attach.png\" style=\"cursor:pointer;\" onclick=\"mostrarOrientacaoAdm('".$abaid."');\">";
	}

	return (($orientacao)?nl2br($orientacao):"&nbsp;").$htmladm;
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

	$sql = "SELECT * FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd WHERE i.iusd='".$dados['iusdantigo']."'";
	$identificacaousuario_antigo = $db->pegaLinha($sql);
	
	if($identificacaousuario_antigo['pflcod']==PFL_PROFESSORALFABETIZADOR) {
		
		$docids = $db->carregarColuna("SELECT docid FROM sispacto2.pagamentobolsista WHERE tpeid='".$identificacaousuario_antigo['tpeid']."'");
		
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
				$db->executar("DELETE FROM sispacto2.pagamentobolsista WHERE docid IN('".implode("','",$docids)."')");				
			}
		}
		
	}
	
	if(!$identificacaousuario_antigo) {
		$al = array("alert"=>"Usuário a ser substituido não foi encontrado","location"=>$_SERVER['HTTP_REFERER']);
		alertlocation($al);
	}
	
	if($identificacaousuario_antigo['pflcod'] == PFL_ORIENTADORESTUDO) $having_orientador = " HAVING COUNT(*) > 1";
	
	$sql = "SELECT COUNT(*) as t FROM sispacto2.mensario m 
			INNER JOIN sispacto2.identificacaousuario i ON i.iusd = m.iusd 
			INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = m.menid
			INNER JOIN workflow.documento d ON d.docid = m.docid AND d.tpdid=".TPD_FLUXOMENSARIO."
			WHERE i.iusnaodesejosubstituirbolsa=false AND m.iusd='".$identificacaousuario_antigo['iusd']."' AND mavtotal>=7 AND d.esdid=".ESD_ENVIADO_MENSARIO." ".$having_orientador;
	
	$is_apto = $db->pegaUm($sql);

	if($is_apto) {
		$al = array("alert"=>"O usuário (".$identificacaousuario_antigo['iusnome'].") não pode ser substituido pois se encontra APTO A RECER BOLSA(Avaliações positivas) em alguns períodos. Solicite ao Coordenador GERAL/ADJUNTO que acesse a aba Aprovar Equipe, e aprove sua bolsa. Após este procedimento, este usuário estará disponível para troca.","location"=>$_SERVER['HTTP_REFERER']);
		alertlocation($al);
	}
	
	
	if(!$identificacaousuario_antigo['uncid']) $identificacaousuario_antigo['uncid'] = $dados['uncid'];
	
	$sql = "SELECT i.iusd, t.tpeid, i.iusnome FROM sispacto2.identificacaousuario i LEFT JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd WHERE i.iuscpf='".str_replace(array(".","-"),array("",""),$dados['iuscpf_'])."'";
	$identificacaousuario_novo = $db->pegaLinha($sql);
	
	if($identificacaousuario_novo['tpeid']) {
		if(!$dados['noredirect']) {
	 		$al = array("alert"=>"Novo Usuário (".$identificacaousuario_novo['iusnome'].") ja possui atribuções no SISPACTO, por isso não pode ser inserido","location"=>$_SERVER['HTTP_REFERER']);
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
     	$sql = "INSERT INTO sispacto2.identificacaousuario(
 	            picid, uncid, iuscpf, iusnome, iusemailprincipal, muncodatuacao,  
 	            iusdatainclusao, iusstatus, iusformacaoinicialorientador, iustipoprofessor, iustipoorientador)
 			    VALUES (".(($identificacaousuario_antigo['picid'])?"'".$identificacaousuario_antigo['picid']."'":"NULL").", ".(($identificacaousuario_antigo['uncid'])?"'".$identificacaousuario_antigo['uncid']."'":"NULL").", '".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."', '".$dados['iusnome_']."', '".$dados['iusemailprincipal_']."',".(($identificacaousuario_antigo['muncodatuacao'])?"'".$identificacaousuario_antigo['muncodatuacao']."'":"NULL").",  
 			            NOW(), 'A', ".(($identificacaousuario_antigo['iusformacaoinicialorientador'])?$identificacaousuario_antigo['iusformacaoinicialorientador']:"NULL").", 
 			            ".(($identificacaousuario_antigo['iustipoprofessor'])?"'".$identificacaousuario_antigo['iustipoprofessor']."'":"NULL").",
 			            ".(($identificacaousuario_antigo['iustipoorientador'])?"'".$identificacaousuario_antigo['iustipoorientador']."'":"NULL").") returning iusd;";
     	$identificacaousuario_novo['iusd'] = $db->pegaUm($sql);
	} else {
		$sql = "UPDATE sispacto2.identificacaousuario SET iusstatus='A', picid=".(($identificacaousuario_antigo['picid'])?"'".$identificacaousuario_antigo['picid']."'":"NULL").", uncid=".(($identificacaousuario_antigo['uncid'])?"'".$identificacaousuario_antigo['uncid']."'":"NULL").", 
														 iusformacaoinicialorientador=".(($identificacaousuario_antigo['iusformacaoinicialorientador'])?$identificacaousuario_antigo['iusformacaoinicialorientador']:"NULL").", 
														 iustipoprofessor=".(($identificacaousuario_antigo['iustipoprofessor'])?"'".$identificacaousuario_antigo['iustipoprofessor']."'":"NULL").",
														 iustipoorientador=".(($identificacaousuario_antigo['iustipoorientador'])?"'".$identificacaousuario_antigo['iustipoorientador']."'":"NULL")."
														 WHERE iusd='".$identificacaousuario_novo['iusd']."'";
		$db->executar($sql);
	}
	
	$sql = "DELETE FROM sispacto2.usuarioresponsabilidade WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."'";
	$db->executar($sql);
	
	$sql = "UPDATE sispacto2.usuarioresponsabilidade SET usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."', uncid=".(($identificacaousuario_antigo['uncid'])?"'".$identificacaousuario_antigo['uncid']."'":"NULL")." WHERE rpustatus='A' AND usucpf='".$identificacaousuario_antigo['usucpf']."' AND pflcod='".$identificacaousuario_antigo['pflcod']."'";
	$db->executar($sql);
	
	$sql = "UPDATE sispacto2.tipoperfil SET iusd='".$identificacaousuario_novo['iusd']."' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
	$db->executar($sql);
	
	$sql = "UPDATE sispacto2.turmas SET iusd='".$identificacaousuario_novo['iusd']."' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
	$db->executar($sql);
	
	$sql = "UPDATE sispacto2.orientadorturma SET iusd='".$identificacaousuario_novo['iusd']."' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
	$db->executar($sql);
	
	$otuid = $db->pegaUm("SELECT otuid FROM sispacto2.orientadorturmaoutros WHERE iusd='".$identificacaousuario_novo['iusd']."'");
	if(!$otuid) {
		
		$sql = "UPDATE sispacto2.orientadorturmaoutros SET iusd='".$identificacaousuario_novo['iusd']."' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
		$db->executar($sql);
	}
	
	$sql = "UPDATE sispacto2.identificacaousuario SET iusstatus='I' WHERE iusd='".$identificacaousuario_antigo['iusd']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$identificacaousuario_antigo['iuscpf']."' AND pflcod='".$dados['pflcod_']."'";
	$db->executar($sql);
	
	if($dados['pflcod_']==PFL_ORIENTADORESTUDO) {
		$existe_proletramento 	 = $db->pegaUm("SELECT cpf FROM sispacto2.tutoresproletramento WHERE cpf='".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."'");
		$existe_semproletramento = $db->pegaUm("SELECT cpf FROM sispacto2.tutoressemproletramento WHERE cpf='".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."'");
		if($existe_proletramento) $iustipoorientador = 'tutoresproletramento'; 
		elseif($existe_semproletramento) $iustipoorientador = 'tutoresredesemproletramento';
		else $iustipoorientador = 'profissionaismagisterio';
			
		$sql = "UPDATE sispacto2.identificacaousuario SET iustipoorientador='{$iustipoorientador}' WHERE iusd='".$identificacaousuario_novo['iusd']."'";
		$db->executar($sql);
	}
	
	// removendo avaliações não concluidas
	$sql = "SELECT m.menid FROM sispacto2.mensario m 
			INNER JOIN workflow.documento d ON d.docid = m.docid 
			WHERE iusd='".$identificacaousuario_antigo['iusd']."' AND d.esdid!='".ESD_APROVADO_MENSARIO."'";
	
	$menids = $db->carregarColuna($sql);
	
	if($menids) {
		
		$sql = "SELECT mavid FROM sispacto2.mensarioavaliacoes WHERE menid IN('".implode("','",$menids)."')";
		$mavids = $db->carregarColuna($sql);
		
		if($mavids) {
			$db->executar("DELETE FROM sispacto2.historicoreaberturanota WHERE mavid IN('".implode("','",$mavids)."')");
			$db->executar("DELETE FROM sispacto2.mensarioavaliacoes WHERE mavid IN('".implode("','",$mavids)."')");
		}
	}
	
	$sql = "INSERT INTO sispacto2.historicotrocausuario(iusdnovo, iusdantigo, pflcod, hstdata, usucpf, uncid)
    		VALUES ('".$identificacaousuario_novo['iusd']."', '".$identificacaousuario_antigo['iusd']."', '".$dados['pflcod_']."', NOW(), '".$_SESSION['usucpf']."', ".(($identificacaousuario_antigo['uncid'])?"'".$identificacaousuario_antigo['uncid']."'":"NULL").");";
	$db->executar($sql);
	
	$db->commit();
		
	if($identificacaousuario_antigo['uncid']) {
		gerarVersaoProjetoUniversidade(array('uncid' => $identificacaousuario_antigo['uncid']));
	}
	
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
				
				$sql = "SELECT * FROM sispacto2.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$cpf)."'";
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
			    
		 		$remetente = array("nome" => "SIMEC - MÓDULO SISPACTO","email" => $identificacaousuario['iusemailprincipal']);
 				$destinatario = $identificacaousuario['iusemailprincipal'];
 				$usunome = $identificacaousuario['iusnome'];
 				$assunto = "Cadastro no SIMEC - MÓDULO SISPACTO";
 				$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
		 		$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo SISPACTO. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
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
		 		
			    $existe_sis = $db->pegaUm("select usucpf from seguranca.usuario_sistema where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and sisid='".SIS_SISPACTO."'");
			    	
			    if(!$existe_sis) {
			    		
			    	$sql = "INSERT INTO seguranca.usuario_sistema(
			         	    usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
			     			VALUES ('".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', ".SIS_SISPACTO.", 'A', NULL, NOW(), 'A');";
				    	
			     	$db->executar($sql);
				    	
			    } else {
		 	    	$sql = "UPDATE seguranca.usuario_sistema SET susstatus='A', suscod='".$dados['suscod']."' WHERE usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' AND sisid='".SIS_SISPACTO."'";
		 	    	$db->executar($sql);
			    }
			    
			    $sql = "INSERT INTO seguranca.historicousuario(htudsc, htudata, usucpf, sisid, suscod, usucpfadm)
    					VALUES ('Mudança realizada pela ferramenta de gerencia do SISPACTO.', 
    							NOW(), 
    							'".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', 
    							'".$_SESSION['sisid']."', '".$dados['suscod']."', '".$_SESSION['usucpf']."');";
			    
	 	    	$db->executar($sql);
			    
			    $existe_pfl = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and pflcod='".$pflcod."'");
    	
			    if(!$existe_pfl) {
			    	$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', '".$pflcod."');";
			     	$db->executar($sql);
			    }

			    $rpuid = $db->pegaUm("select rpuid from sispacto2.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and pflcod='".$pflcod."' AND rpustatus='A'");
			    
			    if($dados['uncid']) {
	    
				    if(!$rpuid) {
				    	$sql = "INSERT INTO sispacto2.usuarioresponsabilidade(
				            		pflcod, usucpf, rpustatus, rpudata_inc, uncid)
				 			    VALUES ('".$pflcod."', '".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', 'A', NOW(), '".$dados['uncid']."');";
				    	
				    	$db->executar($sql);
				    } else {
				    	$sql = "UPDATE sispacto2.usuarioresponsabilidade SET uncid='".$dados['uncid']."' WHERE rpuid='".$rpuid."'";
				    	$db->executar($sql);
				    }
				    
			    }
			    
			    $rpuid = $db->pegaUm("select rpuid from sispacto2.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and pflcod='".$pflcod."' AND rpustatus='A'");
			    
			    if($identificacaousuario['picid']) {
			    	
			    	$sql = "SELECT * FROM sispacto2.pactoidadecerta WHERE picid='".$identificacaousuario['picid']."'";
			    	$pactoidadecerta = $db->pegaLinha($sql);
			    	
			    	if($pactoidadecerta['muncod']) {
			    		$cl  = "muncod='".$pactoidadecerta['muncod']."'";
			    		$ur  = "muncod";
			    		$ur2 = "'".$pactoidadecerta['muncod']."'";
			    	} elseif($pactoidadecerta['estuf'])  {
			    		$cl  = "estuf='".$pactoidadecerta['estuf']."'";
			    		$ur  = "estuf";
			    		$ur2 = "'".$pactoidadecerta['estuf']."'";
			    		
			    	}
	    
				    if(!$rpuid) {
				    	$sql = "INSERT INTO sispacto2.usuarioresponsabilidade(
				            		pflcod, usucpf, rpustatus, rpudata_inc, {$ur})
				 			    VALUES ('".$pflcod."', '".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', 'A', NOW(), {$ur2});";
				    	
				    	$db->executar($sql);
				    } else {
				    	$sql = "UPDATE sispacto2.usuarioresponsabilidade SET {$cl} WHERE rpuid='".$rpuid."'";
				    	$db->executar($sql);
				    }
				    
			    }
			    
    			$db->commit();
			}
			
		}
		
		
	}

    

    
 	$al = array("alert"=>"Gerenciamento executado com sucesso","location"=>$_SERVER['REQUEST_URI']);
 	alertlocation($al);
	
}

function verificarFormacaoCompleta($dados) {
	global $db;
	$sql = "SELECT foecompleto FROM sispacto2.formacaoescolaridade WHERE foeid='".$dados['foeid']."'";
	$foecompleto = $db->pegaUm($sql);
	echo (($foecompleto=="t")?"T":"");
	echo (($foecompleto=="f")?"F":"");
}

function listarCursosFormacao($dados) {
	global $db;
	$sql = "SELECT cufid as codigo, cufcursodesc as descricao FROM sispacto2.cursoformacao WHERE cufstatus='A' AND cufcodareageral='".$dados['cufcodareageral']."' ORDER BY cufcursodesc";
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
    $file = new FilesSimec( "identificacaousuarioanexo", $campos, "sispacto2" );
    $file->setUpload( NULL, "arquivo" );
    
	$al = array("alert"=>"Documento de Designação gravada com sucesso","location"=>$dados['goto']);
	alertlocation($al);
    
	
}

function downloadDocumentoDesignacao($dados) {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec( "identificacaousuarioanexo", NULL, "sispacto2" );
    $file->getDownloadArquivo( $dados['arqid'] );
}

function removerDocumentoDesignacao($dados) {
	global $db;
	$sql = "DELETE FROM sispacto2.identificacaousuarioanexo WHERE iuaid='".$dados['iuaid']."'";
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
    $file = new FilesSimec( "documentoatividade", NULL, "sispacto2" );
    $file->getDownloadArquivo( $dados['arqid'] );
}

function removerAnexoPortaria($dados) {
	global $db;
	$sql = "DELETE FROM sispacto2.portarianomeacao WHERE ponid='".$dados['ponid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Anexo excluído com sucesso","location"=>$_SERVER['HTTP_REFERER']);
	alertlocation($al);
	
	
}

function carregarDadosTurma($dados) {
	global $db;
	$sql = "SELECT * FROM sispacto2.turmas t
			LEFT JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd 
		 	LEFT JOIN sispacto2.tipoperfil tt ON tt.iusd = i.iusd   
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
		$sql = "SELECT '<center>".((!$dados['consulta'])?"<img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirAlunoTurma('||i.iusd||');\">":"")." ".(($dados['formacaoinicial'])?"'|| CASE WHEN SUBSTR(i.iuscpf,1,3)!='SIS' THEN '<input type=radio name=\"iusd['||i.iusd||']\" value=\"TRUE\" '||CASE WHEN i.iusformacaoinicialorientador=true THEN 'checked' ELSE '' END||'> Presente <input type=radio name=\"iusd['||i.iusd||']\" value=\"FALSE\" '||CASE WHEN i.iusformacaoinicialorientador=false THEN 'checked' ELSE '' END||'> Ausente' ELSE '' END ||'":"")."</center>' as acao, i.iuscpf, i.iusnome, i.iusemailprincipal, m.estuf || ' / ' || m.mundescricao as municipio, CASE WHEN pp.muncod IS NULL THEN 'Estadual' ELSE 'Municipal' END as esfera, tu.turdesc FROM sispacto2.orientadorturma ot 
				INNER JOIN sispacto2.identificacaousuario i ON i.iusd = ot.iusd 
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN sispacto2.turmas tu ON tu.turid = ot.turid 
				LEFT JOIN sispacto2.pactoidadecerta pp ON pp.picid = i.picid 
				LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
				WHERE ot.turid='".$dados['turid']."' ORDER BY i.iusnome";
		
		$cabecalho = array("&nbsp;","CPF","Nome","Email","UF/Município","Esfera","Turma");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, true);
	} else {
		echo "<p>Nenhuma turma foi selecionada</p>";
	}
}

function carregarAlunosTurmaOutros($dados) {
	global $db;
	if($dados['turid']) {
		$sql = "SELECT '<center>".((!$dados['consulta'])?"<img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirAlunoTurmaOutros('||i.iusd||');\">":"")." ".(($dados['formacaoinicial'])?"'|| CASE WHEN SUBSTR(i.iuscpf,1,3)!='SIS' THEN '<input type=radio name=\"iusd['||i.iusd||']\" value=\"TRUE\" '||CASE WHEN i.iusformacaoinicialorientador=true THEN 'checked' ELSE '' END||'> Presente <input type=radio name=\"iusd['||i.iusd||']\" value=\"FALSE\" '||CASE WHEN i.iusformacaoinicialorientador=false THEN 'checked' ELSE '' END||'> Ausente' ELSE '' END ||'":"")."</center>' as acao, i.iuscpf, i.iusnome, i.iusemailprincipal, m.estuf || ' / ' || m.mundescricao as municipio, CASE WHEN pp.muncod IS NOT NULL THEN 'Municipal' WHEN pp.estuf IS NOT NULL THEN 'Estadual' ELSE 'Equipe IES' END ||'( '||p.pfldsc||' )' as esfera, tu.turdesc FROM sispacto2.orientadorturmaoutros ot
				INNER JOIN sispacto2.identificacaousuario i ON i.iusd = ot.iusd
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			    INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				INNER JOIN sispacto2.turmas tu ON tu.turid = ot.turid
				LEFT JOIN sispacto2.pactoidadecerta pp ON pp.picid = i.picid
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
	$sql = "SELECT m.menid, d.docid, d.esdid FROM sispacto2.mensario m INNER JOIN workflow.documento d ON d.docid = m.docid WHERE m.iusd='".$dados['iusd']."' AND fpbid='".$dados['fpbid']."'";
	$mensario = $db->pegaLinha($sql);
	
	$menid = $mensario['menid'];
	$docid = $mensario['docid'];
	$esdid = $mensario['esdid'];
	
	if(!$menid) {
		
		$arrUs    = $db->pegaLinha("SELECT i.iusnome, p.pfldsc FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod WHERE i.iusd='".$dados['iusd']."'");
		$iusnome  = $arrUs['iusnome'];
		$pfldsc   = $arrUs['pfldsc'];
		
		$referencia = $db->pegaUm("SELECT fpbmesreferencia || ' / ' || fpbanoreferencia as descricao FROM sispacto2.folhapagamento WHERE fpbid='".$dados['fpbid']."'");
		
		$docid = wf_cadastrarDocumento( TPD_FLUXOMENSARIO, 'Mensário : '.$iusnome.' - '.$pfldsc.' Ref.'.$referencia );
		$esdid = ESD_EM_ABERTO_MENSARIO;
		
		$sql = "INSERT INTO sispacto2.mensario(
            	iusd, fpbid, docid, pflcod)
    			VALUES ('".$dados['iusd']."', '".$dados['fpbid']."', '".$docid."', (SELECT pflcod FROM sispacto2.tipoperfil WHERE iusd='".$dados['iusd']."')) RETURNING menid;";
		
		$menid = $db->pegaUm($sql);
		$db->commit();
	}
	
	return array("memid"=>$menid,"docid"=>$docid,"esdid"=>$esdid);
	
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
				
				$sql = "SELECT mavid, mavfrequencia, mavatividadesrealizadas FROM sispacto2.mensarioavaliacoes WHERE menid='".$dadosmensario['memid']."' AND (iusdavaliador='".$dados['iusdavaliador']."' OR pflcodavaliador IN(SELECT pflcod FROM sispacto2.tipoperfil WHERE iusd='".$dados['iusdavaliador']."'))";
				$ma = $db->pegaLinha($sql);
				$mavid = $ma['mavid'];
				
				if($mavid) {
					
					if(is_numeric($dados['frequencia'][$iusd]) || is_numeric($dados['atividadesrealizadas'][$iusd])) {
						
						if($dados['frequencia'][$iusd]!=$ma['mavfrequencia'] || $dados['atividadesrealizadas'][$iusd]!=$ma['mavatividadesrealizadas']) {
							$upt = "iusdavaliador='".$dados['iusdavaliador']."',";
						}
						
					
						$sql = "UPDATE sispacto2.mensarioavaliacoes SET mavfrequencia=".((is_numeric($dados['frequencia'][$iusd]))?"'".$dados['frequencia'][$iusd]."'":"NULL").", 
															 		    mavatividadesrealizadas=".((is_numeric($dados['atividadesrealizadas'][$iusd]))?"'".$dados['atividadesrealizadas'][$iusd]."'":"NULL").",
																	    {$upt}
															 		    mavmonitoramento=".((is_numeric($dados['monitoramento'][$iusd]))?"'".$dados['monitoramento'][$iusd]."'":"NULL").", 
															 		    mavtotal=".((is_numeric(($dados['total'][$iusd])))?"'".($dados['total'][$iusd])."'":"NULL")." WHERE mavid='".$mavid."'";
						$db->executar($sql);
					
					}
					
				} else {
					
					if(is_numeric($dados['frequencia'][$iusd]) || is_numeric($dados['atividadesrealizadas'][$iusd])) {
					
						$sql = "INSERT INTO sispacto2.mensarioavaliacoes(
		            			iusdavaliador, mavfrequencia, mavatividadesrealizadas, 
		            			mavmonitoramento, mavtotal, menid, pflcodavaliador)
			    					SELECT '".$dados['iusdavaliador']."', 
			    						    ".((is_numeric($dados['frequencia'][$iusd]))?"'".$dados['frequencia'][$iusd]."'":"NULL").", 
			    							".((is_numeric($dados['atividadesrealizadas'][$iusd]))?"'".$dados['atividadesrealizadas'][$iusd]."'":"NULL").", 
			            					".((is_numeric($dados['monitoramento'][$iusd]))?"'".$dados['monitoramento'][$iusd]."'":"NULL").", 
			            					".((is_numeric(($dados['total'][$iusd])))?"'".($dados['total'][$iusd])."'":"NULL").", 
										   '".$dadosmensario['memid']."',
											(SELECT pflcod FROM sispacto2.tipoperfil WHERE iusd='".$dados['iusdavaliador']."')
								 	WHERE (SELECT mavid FROM sispacto2.mensarioavaliacoes WHERE iusdavaliador='".$dados['iusdavaliador']."' AND menid='".$dadosmensario['memid']."') IS NULL
						
								RETURNING mavid;";
						
						$mavid = $db->pegaUm($sql);
					
					}
					
				}
				
				$sql = "UPDATE sispacto2.mensarioavaliacoes ma SET mavtotal=foo.total FROM (
						SELECT * FROM (
						SELECT 
						m.menid,
						mavid,
						mavtotal,
						(COALESCE((mavfrequencia*fatfrequencia),0) + COALESCE((mavatividadesrealizadas*fatatividadesrealizadas),0) + COALESCE(mavmonitoramento,0)) as total
						FROM sispacto2.mensarioavaliacoes ma 
						INNER JOIN sispacto2.mensario m ON m.menid = ma.menid 
						INNER JOIN sispacto2.identificacaousuario u ON u.iusd = m.iusd 
						INNER JOIN sispacto2.tipoperfil t ON t.iusd = u.iusd 
						INNER JOIN sispacto2.fatoresdeavaliacao f ON f.fatpflcodavaliado = t.pflcod 
						WHERE ma.menid='".$dadosmensario['memid']."' 
						) fee
						WHERE fee.mavtotal != total
						) foo 
						WHERE ma.menid = foo.menid";
				
				$db->executar($sql);
				
				if($mavid && $dados['cpfresponsavel'][$iusd] && $dados['mavdsc'][$iusd]) {
					$sql = "INSERT INTO sispacto2.historicoreaberturanota(
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
		
		if(!$_SESSION['sispacto2']['orientadorestudo']['uncid']) return 'Este usuário não esta vinculado a nenhuma IES';
		
		$funcaoavaliacao = $db->pegaUm("SELECT tpatipoavaliacao FROM sispacto2.tipoavaliacaoperfil WHERE pflcod='".$pflcod."' AND uncid='".$_SESSION['sispacto2']['orientadorestudo']['uncid']."' AND fpbid='".$fpbid."'");
		
		// tratando condição de tipos de monitoramento
		if($funcaoavaliacao=='monitoramentoTextual') return true;
		
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sispacto2.mensario me 
							INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid
							INNER JOIN sispacto2.identificacaousuario i ON i.iusd = me.iusd 
							INNER JOIN sispacto2.orientadorturma ot ON ot.iusd = me.iusd 
							INNER JOIN sispacto2.turmas tt ON tt.turid = ot.turid 		
							WHERE tt.iusd='".$_SESSION['sispacto2']['orientadorestudo']['iusd']."' AND me.fpbid='".$fpbid."'");
		if(!$tot) {
			return 'Nenhuma avaliação foi salva';
		} else {
			
			$sql = "SELECT i.iusnome FROM sispacto2.mensario me 
					INNER JOIN workflow.documento d ON d.docid = me.docid
					INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid 
					INNER JOIN sispacto2.identificacaousuario i ON i.iusd = me.iusd 
					INNER JOIN sispacto2.orientadorturma ot ON ot.iusd = me.iusd 
					INNER JOIN sispacto2.turmas tt ON tt.turid = ot.turid 		
					WHERE tt.iusd='".$_SESSION['sispacto2']['orientadorestudo']['iusd']."' AND ma.iusdavaliador='".$_SESSION['sispacto2']['orientadorestudo']['iusd']."' AND me.fpbid='".$fpbid."' AND ma.mavtotal IS NULL AND d.esdid != ".ESD_APROVADO_MENSARIO." 
					ORDER BY i.iusnome";
			
			$iusnome = $db->carregarColuna($sql);
			
			if($iusnome) {
				return 'Existem Professores Alfabetizadores sem avaliação: \n\n'.implode('\n<br>',$iusnome);
			}
			
		}
		
		$sql = "SELECT count(*) as tot FROM sispacto2.respostasavaliacaocomplementar WHERE iusdavaliador='".$_SESSION['sispacto2']['orientadorestudo']['iusd']."'";
		$existe_respostasavaliacaocomplementar = $db->pegaUm($sql);
		
		if(!$existe_respostasavaliacaocomplementar) {
			return 'É necessário preencher a Avaliação Complementar';
		}
		
		return true;
	
	}
	
	if($pflcod == PFL_FORMADORIES) {
		
		// tratando falha de segurança
		if(!$_SESSION['sispacto2']['formadories']['iusd']) return 'Formador IES não foi IDENTIFICADO, faça o LOGOUT, e acesse novamente o SISPACTO.';
		
		$funcaoavaliacao = $db->pegaUm("SELECT tpatipoavaliacao FROM sispacto2.tipoavaliacaoperfil WHERE pflcod='".$pflcod."' AND uncid='".$_SESSION['sispacto2']['formadories']['uncid']."' AND fpbid='".$fpbid."'");
		
		// tratando condição de tipos de monitoramento
		if($funcaoavaliacao=='monitoramentoTextual') return true;
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sispacto2.mensario me 
							INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid
							INNER JOIN sispacto2.identificacaousuario i ON i.iusd = me.iusd 
							INNER JOIN sispacto2.orientadorturma ot ON ot.iusd = me.iusd 
							INNER JOIN sispacto2.turmas tt ON tt.turid = ot.turid 		
							WHERE tt.iusd='".$_SESSION['sispacto2']['formadories']['iusd']."' AND me.fpbid='".$fpbid."' AND ma.iusdavaliador='".$_SESSION['sispacto2']['formadories']['iusd']."' AND ma.mavtotal IS NOT NULL");
		
		if(!$tot) {
			return 'Nenhuma avaliação foi salva';
		} else {
			
			$sql = "SELECT i.iusnome FROM sispacto2.mensario me 
					INNER JOIN workflow.documento d ON d.docid = me.docid 
					LEFT JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid AND ma.iusdavaliador='".$_SESSION['sispacto2']['formadories']['iusd']."'
					INNER JOIN sispacto2.identificacaousuario i ON i.iusd = me.iusd AND i.iusformacaoinicialorientador=TRUE 
					INNER JOIN sispacto2.orientadorturma ot ON ot.iusd = me.iusd 
					INNER JOIN sispacto2.turmas tt ON tt.turid = ot.turid 		
					WHERE tt.iusd='".$_SESSION['sispacto2']['formadories']['iusd']."' AND me.fpbid='".$fpbid."' AND ma.mavtotal IS NULL AND d.esdid != ".ESD_APROVADO_MENSARIO." 
					ORDER BY i.iusnome";
			
			$iusnome = $db->carregarColuna($sql);
			
			if($iusnome) {
				return 'Existem Orientadores de Estudo sem avaliação: \n\n'.implode('\n<br>',$iusnome);
			}
			
		}
		
		return true;
	
	}
	
	if($pflcod == PFL_SUPERVISORIES) {
		
		$sql_tot = sqlAvaliacaoSupervisor(array('uncid'=>$_SESSION['sispacto2']['supervisories']['uncid'],'iusd'=>$_SESSION['sispacto2']['supervisories']['iusd'],'fpbid'=>$fpbid));
		
		$sql = "SELECT COUNT(*) FROM ({$sql_tot}) ff";
		$navals = $db->pegaUm($sql);
		
		if(!$navals) {
			return true;
		}
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sispacto2.mensario me 
							INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid
							WHERE me.fpbid='".$fpbid."' AND ma.iusdavaliador='".$_SESSION['sispacto2']['supervisories']['iusd']."' AND ma.mavtotal IS NOT NULL");
		
		if(!$tot) {
			return 'É necessário avaliar um membro';
		}
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORADJUNTOIES) {
		
		$sql_tot = sqlAvaliacaoCoordenadorAdjuntoIES(array('uncid'=>$_SESSION['sispacto2']['coordenadoradjuntoies']['uncid'],'iusd'=>$_SESSION['sispacto2']['coordenadoradjuntoies']['iusd'],'fpbid'=>$fpbid));
		
		$sql = "SELECT COUNT(*) FROM ({$sql_tot}) ff";
		$navals = $db->pegaUm($sql);
		
		if(!$navals) {
			return true;
		}
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sispacto2.mensario me 
							INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid
							WHERE me.fpbid='".$fpbid."' AND ma.iusdavaliador='".$_SESSION['sispacto2']['coordenadoradjuntoies']['iusd']."' AND ma.mavtotal IS NOT NULL");
		
		if(!$tot) {
			return 'É necessário avaliar um membro';
		}
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORIES) {
		
		$sql_tot = sqlAvaliacaoCoordenadorIES(array('uncid'=>$_SESSION['sispacto2']['universidade']['uncid'],'iusd'=>$_SESSION['sispacto2']['universidade']['iusd'],'fpbid'=>$fpbid));
		
		$sql = "SELECT COUNT(*) FROM ({$sql_tot}) ff";
		$navals = $db->pegaUm($sql);
		
		if(!$navals) {
			return true;
		}
		
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sispacto2.mensario me 
							INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid
							WHERE me.fpbid='".$fpbid."' AND ma.iusdavaliador='".$_SESSION['sispacto2']['universidade']['iusd']."' AND ma.mavtotal IS NOT NULL");
		
		if(!$tot) {
			return 'É necessário avaliar um membro';
		}
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORLOCAL) {
		
		$funcaoavaliacao = $db->pegaUm("SELECT tpatipoavaliacao FROM sispacto2.tipoavaliacaoperfil WHERE pflcod='".$pflcod."' AND uncid='".$_SESSION['sispacto2']['coordenadorlocal']['uncid']."' AND fpbid='".$fpbid."'");
		
		// tratando condição de tipos de monitoramento
		if($funcaoavaliacao=='monitoramentoTextual') return true;
		
		$tot = $db->pegaUm("SELECT COUNT(*) as tot FROM sispacto2.mensario me 
							INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid
							WHERE me.fpbid='".$fpbid."' AND ma.iusdavaliador='".$_SESSION['sispacto2']['coordenadorlocal']['iusd']."' AND ma.mavtotal IS NOT NULL");
		
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
		
		$sql = "SELECT i.iusnome, me.docid, ma.mavtotal FROM sispacto2.mensario me 
				INNER JOIN workflow.documento dc ON dc.docid = me.docid AND dc.tpdid=".TPD_FLUXOMENSARIO." 
				INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sispacto2.identificacaousuario i ON i.iusd = me.iusd 
				INNER JOIN sispacto2.orientadorturma ot ON ot.iusd = me.iusd 
				INNER JOIN sispacto2.turmas tt ON tt.turid = ot.turid 		
				WHERE tt.iusd='".$_SESSION['sispacto2']['orientadorestudo']['iusd']."' AND dc.esdid='".ESD_EM_ABERTO_MENSARIO."' AND ma.iusdavaliador='".$_SESSION['sispacto2']['orientadorestudo']['iusd']."' AND me.fpbid='".$fpbid."'";
		
		
		$arrMensario = $db->carregar($sql);
		
		if($arrMensario[0]) {
			foreach($arrMensario as $mensario) {
				wf_alterarEstado( $mensario['docid'], AED_ENVIAR_MENSARIO, '', array('fpbid'=>$fpbid));
			}
		}
		
		
		$sql = "UPDATE sispacto2.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sispacto2.mensario me 
				INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sispacto2.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sispacto2']['orientadorestudo']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_FORMADORIES) {
		
		$sql = "UPDATE sispacto2.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sispacto2.mensario me 
				INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sispacto2.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sispacto2']['formadories']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_SUPERVISORIES) {
		
		$sql = "UPDATE sispacto2.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sispacto2.mensario me 
				INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sispacto2.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sispacto2']['supervisories']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORADJUNTOIES) {

		
		$sql = "UPDATE sispacto2.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sispacto2.mensario me 
				INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sispacto2.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sispacto2']['coordenadoradjuntoies']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORIES) {
		
		$sql = "UPDATE sispacto2.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sispacto2.mensario me 
				INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sispacto2.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sispacto2']['universidade']['iusd']."' AND me.fpbid='".$fpbid."'
				) foo 
				WHERE co.mavid=foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		return true;
	
	}
	
	if($pflcod == PFL_COORDENADORLOCAL) {
		
		$sql = "UPDATE sispacto2.mensarioavaliacoes co SET mavtotal=(mavtotal+foo.fatmonitoramento), mavmonitoramento=foo.fatmonitoramento FROM (
				SELECT ma.mavid, fa.fatmonitoramento FROM sispacto2.mensario me 
				INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = me.menid 
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = me.iusd 
				INNER JOIN sispacto2.fatoresdeavaliacao fa ON fa.fatpflcodavaliado = t.pflcod	
				WHERE me.iusd='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['iusd']."' AND me.fpbid='".$fpbid."'
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
	
	$sql = "SELECT * FROM sispacto2.fatoresdeavaliacao WHERE fatid='".$dados['fatid']."'";
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
				SELECT m.iusd, ma.mavfrequencia, ma.mavatividadesrealizadas, ma.mavmonitoramento, ma.mavtotal, ma.mavid, CASE WHEN ma.iusdavaliador!='".$dados['iusd']."' THEN 'Avaliado pelo '||i.iusnome ELSE '' END as obs 
									   FROM sispacto2.mensario m 
									   INNER JOIN sispacto2.identificacaousuario ius ON ius.iusd = m.iusd 
									   INNER JOIN sispacto2.mensarioavaliacoes ma ON ma.menid = m.menid 
									   INNER JOIN sispacto2.identificacaousuario i ON i.iusd = ma.iusdavaliador 
									   WHERE (ius.uncid='".$dados['uncid']."' OR i.uncid='".$dados['uncid']."') AND m.fpbid='".$dados['fpbid']."' AND (iusdavaliador='".$dados['iusd']."' OR pflcodavaliador IN(SELECT pflcod FROM sispacto2.tipoperfil WHERE iusd='".$dados['iusd']."'))
			)
			SELECT DISTINCT CASE WHEN foo.mais = '' THEN '' ELSE '<img align=\"absmiddle\" style=\"cursor:pointer\" src=\"../imagens/mais.gif\" title=\"mais\" onclick=\"exibirAvaliacaoSub(\''||foo.mais||'\', this)\"> ' END as expandir, 
							'<img align=\"absmiddle\" src=\"../imagens/'|| CASE WHEN (SELECT esdid FROM workflow.documento WHERE docid=foo.docid) IN(".ESD_APROVADO_MENSARIO.") THEN 'money.gif' WHEN (SELECT esdid FROM workflow.documento WHERE docid=foo.docid)='".ESD_INVALIDADO_MENSARIO."' THEN 'valida3.gif' ELSE CASE WHEN me.mavtotal IS NULL THEN 'valida5.gif' WHEN me.mavtotal < 7 THEN 'valida6.gif' ELSE 'valida4.gif' END END ||'\" id=\"img_'||foo.iusd||'\"> <img align=\"absmiddle\" src=\"../imagens/ajuda.png\" width=\"16\" height=\"16\" style=\"cursor:pointer;\" onclick=\"verAjuda(\''||fat.fatid||'\');\"> 
					'||CASE WHEN (SELECT esdid FROM workflow.documento WHERE docid=foo.docid) IN(".ESD_ENVIADO_MENSARIO.",".ESD_EM_ABERTO_MENSARIO.") OR foo.docid IS NULL THEN '{$imgexcluir} ".(($dados['consulta'] || $dados['esdid']==ESD_EM_ABERTO_MENSARIO)?"":"<img align=\"absmiddle\" src=\"../imagens/send.png\" onmouseover=\"return escape(\'Reavaliar usuário\');\" width=\"16\" height=\"16\" style=\"cursor:pointer;\" id=\"corrigir_'||foo.iusd||'\" onclick=\"mostrarCorrecaoAprovado(\''||foo.iusd||'\');\">")." ' 
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
			INNER JOIN sispacto2.fatoresdeavaliacao fat ON fat.fatpflcodavaliado = foo.pflcod
			LEFT JOIN tmp_avaliacao me ON me.iusd = foo.iusd 
			".(($where)?"WHERE ".implode(" AND ",$where):"")." 
			ORDER BY 4   
			";
			
	$arrAvaliacao = $db->carregar($sql);
				
			
	if($arrAvaliacao[0]) {					
	
		$cabecalho = array("&nbsp;","&nbsp;","CPF","Nome","E-mail","Perfil","Frequência","Atividades Realizadas","Monitoramento","Nota Final");
		$db->monta_lista_simples($arrAvaliacao,$cabecalho,5000,10,'N','100%',$par2);
	
	} else {
		
		
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
	carregarAvaliacaoEquipe(array("sql"=>$sql_avaliacao,"fpbid"=>$dados['fpbid'],"iusd"=>$dados['iusd'],"uncid"=>$dados['uncid'],"consulta"=>$dados['consulta']));

}

function inserirDadosLog($dados) {
	global $db;
	
	$sql = "INSERT INTO log_historico.logsgb_sispacto2(
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
		FROM sispacto2.identificacaousuario i 
		LEFT JOIN territorios.municipio m ON m.muncod = i.muncod 
		LEFT JOIN sispacto2.identificaoendereco ie ON ie.iusd = i.iusd 
		LEFT JOIN territorios.municipio m2 ON m2.muncod = ie.muncod 
		LEFT JOIN sispacto2.identusutipodocumento it ON it.iusd = i.iusd 
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
    	
    	if(!$dados['sincronizacao']) $lnscpf = $db->carregarColuna("SELECT lnscpf FROM sispacto2.listanegrasgb");
    	else $lnscpf = array();
    	
    	if(!in_array($dadosusuario['iuscpf'],$lnscpf)) {
    		inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcpf'=>$dadosusuario['iuscpf'],'logservico'=>'lerDadosBolsista'));
    	} else {
    		inserirDadosLog(array('logrequest'=>'Bolsista com problemas de characteres especiais no SGB. Adicionado a lista negra.','logresponse'=>'Bolsista com problemas de characteres especiais no SGB. Adicionado a lista negra.','logcpf'=>$dadosusuario['iuscpf'],'logservico'=>'lerDadosBolsista'));
    		$existecpf = $dadosusuario['iuscpf'];
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
		
   		inserirDadosLog(array('logerro'=>$logerro_gravarDadosBolsista,'logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcpf'=>$dadosusuario['iuscpf'],'logservico'=>'gravarDadosBolsista'));
    	
    	$sql = "UPDATE sispacto2.identificacaousuario SET cadastradosgb=".(($logerro_gravarDadosBolsista=='TRUE')?'FALSE':'TRUE')." WHERE iusd='".$dados['iusd']."'";
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
			FROM sispacto2.universidadecadastro u 
			INNER JOIN sispacto2.universidade un ON un.uniid = u.uniid  
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
    	
   	$sql = "UPDATE sispacto2.universidadecadastro SET cadastrosgb=".(($logerro_gravaDadosEntidade=='TRUE')?'FALSE':'TRUE')." WHERE uncid='".$dados['uncid']."'";
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
	
	$sql = "DELETE FROM sispacto2.respostasavaliacaocomplementar WHERE iusdavaliador='".$dados['iusdavaliador']."' AND fpbid='".$dados['fpbid']."'";
	$db->executar($sql);
	
	if($dados['icc']) {
		foreach($dados['icc'] as $iacid => $iccid) {
			if($iccid) {
				$racid = $db->pegaUm("SELECT racid FROM sispacto2.respostasavaliacaocomplementar WHERE iusdavaliador='".$dados['iusdavaliador']."' AND iacid='".$iacid."' AND fpbid='".$dados['fpbid']."'");
				
				if($racid) {
					
					$sql = "UPDATE sispacto2.respostasavaliacaocomplementar SET 
							iusdavaliador='".$dados['iusdavaliador']."', 
							iusdavaliado=".(($dados['iusavaliado'][$iacid])?"'".$dados['iusavaliado'][$iacid]."'":"NULL").", 
							iacid='".$iacid."', 
							iccid='".$iccid."', 
							fpbid='".$dados['fpbid']."' 
							WHERE racid='".$racid."'";
					
					$db->executar($sql);
					
				} else {
				
					$sql = "INSERT INTO sispacto2.respostasavaliacaocomplementar(
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
	// se for equipe do mec, não precisa verificar termo
	if($dados['pflcod'] == PFL_EQUIPEMEC) return true;
	
	// verificando se coordenador local aceitou o termo de compromisso
	$termo = carregarDadosIdentificacaoUsuario(array("iusd"=>$dados['iusd'],"pflcod"=>$dados['pflcod']));
	
	if($termo) {
		$termo = current($termo);
	}
	
	if($termo['iustermocompromisso']!="t") {
		$al = array("alert"=>"Por favor preencha todos os campos obrigatórios da tela “Dados”.","location"=>"sispacto2.php?modulo=principal/{$dados['sis']}/{$dados['sis']}&acao=A&aba=dados");
		alertlocation($al);
	}
}

function gerarVersaoProjetoUniversidade($dados) {
	global $db;
	include_once '_funcoes_universidade.php';
	ob_start();
	$versao_html = true;
	if($dados['uncid']) carregarCoordenadorIES(array('uncid'=>$dados['uncid']));
	include APPRAIZ.'sispacto2/modulos/principal/universidade/visualizacao_projeto.inc';
	$html = ob_get_contents();
	ob_clean();
		
	$sql = "INSERT INTO sispacto2.versoesprojetouniversidade(
            	uncid, usucpf, vpndata, vpnhtml)
    			VALUES ('".$dados['uncid']."', '".$_SESSION['usucpf']."', NOW(), '".addslashes($html)."');";
	$db->executar($sql);
	$db->commit();
	
	
}

function carregarMudancasTroca($dados) {
	global $db;
	$sql = "SELECT CASE WHEN h.hstacao='T' THEN '<span style=font-size:xx-small>Troca</span>'
						WHEN h.hstacao='R' THEN '<span style=font-size:xx-small>Remoção</span>' 
						WHEN h.hstacao='I' THEN '<span style=font-size:xx-small>Inserção</span>'
						WHEN h.hstacao='F' THEN '<span style=font-size:xx-small>Mudança de turma</span>'
						END as acao, 
			'<span style=font-size:xx-small>'||i2.iuscpf||' - '||i2.iusnome||'</span>' as nome_antigo, 
			'<span style=font-size:xx-small>'||i.iuscpf||' - '||i.iusnome||'</span>' as nome_novo,
			'<span style=font-size:xx-small>'||t1.turdesc ||' ( '||i3.iusnome||' )</span>' as turma_antigo,
			'<span style=font-size:xx-small>'||t2.turdesc ||' ( '||i4.iusnome||' )</span>' as turma_novo,
			'<span style=font-size:xx-small>'||p.pfldsc||'</span>', 
            '<span style=font-size:xx-small>'||u.usucpf||' - '||u.usunome||'</span>' as responsavel, 
			'<span style=font-size:xx-small>'||to_char(h.hstdata,'dd/mm/YYYY HH24:MI')||'</span>' as hstdata 
			FROM sispacto2.historicotrocausuario h 
			LEFT JOIN sispacto2.identificacaousuario i ON i.iusd = h.iusdnovo 
			LEFT JOIN sispacto2.identificacaousuario i2 ON i2.iusd = h.iusdantigo 
			LEFT JOIN seguranca.perfil p ON p.pflcod = h.pflcod 
			LEFT JOIN seguranca.usuario u ON u.usucpf = h.usucpf 
			LEFT JOIN sispacto2.turmas t1 ON t1.turid = h.turidantigo 
			LEFT JOIN sispacto2.identificacaousuario i3 ON i3.iusd = t1.iusd 
			LEFT JOIN sispacto2.turmas t2 ON t2.turid = h.turidnovo 
			LEFT JOIN sispacto2.identificacaousuario i4 ON i4.iusd = t2.iusd
			WHERE h.uncid='".$dados['uncid']."' ORDER BY h.hstdata";
	
	$mudancas = $db->carregar($sql);
	
	return $mudancas;

}



function verificarValidacaoVisualizacaoProjeto($dados) {}

function processarPagamentoBolsistaSGB($dados) {
	global $db;
	
	$sql = "SELECT * FROM sispacto2.pagamentobolsista WHERE pboid='".$dados->id."'";
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
			
				$novaparcela = $db->pegaUm("SELECT (rfuparcela+1) as novaparcela FROM sispacto2.folhapagamentouniversidade f 
							 				INNER JOIN sispacto2.universidadecadastro u ON u.uncid = f.uncid 
							 				WHERE u.uniid='".$pagamentobolsista['uniid']."' AND f.fpbid='".$pagamentobolsista['fpbid']."'");
			}
			
			$sql = "UPDATE sispacto2.pagamentobolsista SET remid=null, pboparcela='".$novaparcela."' WHERE pboid='".$pagamentobolsista['pboid']."'";
			$db->executar($sql);
			$db->commit();
			
		} else {
			echo wf_alterarEstado( $pagamentobolsista['docid'], AED_RECUSAR_PAGAMENTO, $cmddsc = 'Erro retornado pelo FNDE: '.$dados->situacao->codigo.' / '.$dados->situacao->descricao, array());
			$sql = "UPDATE sispacto2.pagamentobolsista SET remid=null WHERE pboid='".$pagamentobolsista['pboid']."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	
}

function consultarDetalhesAvaliacoes($dados) {
	global $db;
  
	$sql = "SELECT CASE WHEN ptc.muncod IS NOT NULL THEN 'Municipal ('|| mun.estuf ||' / '|| mun.mundescricao ||')'
						WHEN ptc.estuf IS NOT NULL THEN 'Estadual ('|| est.estuf || ' / ' || estdescricao ||')' 
						ELSE 'Equipe IES' END as esfera, 
					uni.unisigla || ' - ' || uni.uninome as universidade, 
					per.pfldsc, doc.docid, esd.esddsc, m.fpbid, m.iusd, m.menid, i.iusnome, me.mesdsc||'/'||fa.fpbanoreferencia as periodo, f.fatfrequencia, f.fatatividadesrealizadas, f.fatmonitoramento 
            FROM sispacto2.mensario m 
			INNER JOIN sispacto2.identificacaousuario i ON i.iusd = m.iusd 
			LEFT JOIN sispacto2.universidadecadastro unc ON unc.uncid = i.uncid 
			LEFT JOIN sispacto2.universidade uni ON uni.uniid = unc.uniid 
			LEFT JOIN sispacto2.pactoidadecerta ptc ON ptc.picid = i.picid 
			LEFT JOIN territorios.municipio mun ON mun.muncod = ptc.muncod 
			LEFT JOIN territorios.estado est ON est.estuf = ptc.estuf  
			INNER JOIN seguranca.perfil per ON per.pflcod = m.pflcod
			INNER JOIN sispacto2.fatoresdeavaliacao f ON f.fatpflcodavaliado = m.pflcod
			INNER JOIN sispacto2.folhapagamento fa ON fa.fpbid = m.fpbid 
			INNER JOIN public.meses me ON me.mescod::integer = fa.fpbmesreferencia
			INNER JOIN workflow.documento doc ON doc.docid = m.docid AND doc.tpdid=".TPD_FLUXOMENSARIO."
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE menid='".$dados['menid']."'";
	
	$mensario = $db->pegaLinha($sql);
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	echo "<tr><td class=SubTituloDireita width=15%>Avaliado:</td><td style=font-size:x-small;>".$mensario['iusnome']."</td><td class=SubTituloDireita width=15%>Perfil:</td><td style=font-size:x-small;>".$mensario['pfldsc']."</td></tr>";
	echo "<tr><td class=SubTituloDireita width=15%>Universidade:</td><td style=font-size:x-small;>".$mensario['universidade']."</td><td class=SubTituloDireita width=15%>Esfera:</td><td style=font-size:x-small;>".$mensario['esfera']."</td></tr>";
	echo "</table>";
	

	$sql = "SELECT '<span style=font-size:x-small;>'||foo.iusnome||'</span>' as iusnome,
				   '<span style=font-size:x-small;>'||foo.pfldsc||'</span>' as pfldsc,
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
		    FROM sispacto2.mensarioavaliacoes m
			INNER JOIN sispacto2.identificacaousuario i ON i.iusd = m.iusdavaliador 
			LEFT JOIN seguranca.perfil p ON p.pflcod = m.pflcodavaliador 
			WHERE m.menid='".$mensario['menid']."') foo ORDER BY foo.iusnome";
	
	echo "<p align=center><b>Nota Avaliação</b></p>";	
	if($mensario['menid']) {
		$cabecalho = array("<span style=font-size:x-small;>Avaliador</span>","<span style=font-size:x-small;>Perfil</span>","<span style=font-size:x-small;>Frequencia</span>","<span style=font-size:x-small;>Op. Frequencia</span>","<span style=font-size:x-small;>Atividades realizadas</span>","<span style=font-size:x-small;>Op. Atividades realizadas</span>","<span style=font-size:x-small;>Monitoramento</span>","<span style=font-size:x-small;>Nota Final</span>");
		$db->monta_lista_simples($sql,$cabecalho,5000,10,'N','95%','center');
	} else echo "<p align=center style=color:red;font-size:x-small;>Não existem avaliações</p>";
	
	echo "<br>";
	echo "<p align=center><b>Fluxo da avaliação</b></p>";	
	if($mensario['docid']) {
		fluxoWorkflowInterno(array('docid'=>$mensario['docid']));
	} else echo "<p align=center style=color:red;>Não existem avaliações</p>";
	

	if($mensario['iusd'] && $mensario['fpbid']) {
	
		$sql_atv_com = "SELECT i.iusnome, p.pfldsc, ia.iacdsc, ic.iccdsc, ic.iccvalor FROM sispacto2.respostasavaliacaocomplementar r 
				INNER JOIN sispacto2.identificacaousuario i ON i.iusd = r.iusdavaliador 
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				INNER JOIN sispacto2.itensavaliacaocomplementar ia ON ia.iacid = r.iacid 
				INNER JOIN sispacto2.itensavaliacaocomplementarcriterio ic ON ic.iccid = r.iccid 
				WHERE r.iusdavaliado='".$mensario['iusd']."' AND r.fpbid='".$mensario['fpbid']."' ORDER BY ia.iacdsc, i.iusnome";
		
		$existe = $db->pegaUm("SELECT count(*) FROM (".$sql_atv_com.") foo");
		
		$sql = "(
				{$sql_atv_com}
				) UNION ALL (
				SELECT '', '', '', CASE WHEN AVG(ic.iccvalor) > 0 THEN 'Média' ELSE '<span style=color:red;>Não existem avaliações complementares</span>' END as l, AVG(ic.iccvalor) as media FROM sispacto2.respostasavaliacaocomplementar r 
				INNER JOIN sispacto2.identificacaousuario i ON i.iusd = r.iusdavaliador 
				INNER JOIN sispacto2.itensavaliacaocomplementar ia ON ia.iacid = r.iacid 
				INNER JOIN sispacto2.itensavaliacaocomplementarcriterio ic ON ic.iccid = r.iccid 
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
			$case .= "WHEN foo.mav".$dados['criterio']."='".$reg['codigo']."' THEN '<span style=font-size:x-small;>".$reg['descricao']."</span>' ";
		}
		$case .= "ELSE '<span style=color:red;font-size:x-small;>Não se aplica</font>' ";
		$case .= "END";
	}
	
	return $case;

}

function aprovarEquipe($dados) {
	global $db;
	
	if($dados['menid']) {
		foreach($dados['menid'] as $menid) {
			
			$sql = "SELECT * FROM sispacto2.mensario m 
					INNER JOIN workflow.documento d ON d.docid = m.docid 
					WHERE menid='".$menid."'";
			
			$arrMensario = $db->pegaLinha($sql);
			
			if(($arrMensario['pflcod']==PFL_PROFESSORALFABETIZADOR || $arrMensario['pflcod']==PFL_FORMADORIESP) && $arrMensario['esdid']==ESD_EM_ABERTO_MENSARIO) {
				$result = wf_alterarEstado( $arrMensario['docid'], AED_APROVAR_EMABERTO_MENSARIO, $cmddsc = '', array('fpbid'=>$arrMensario['fpbid'],'pflcod'=>$arrMensario['pflcod'],'menid'=>$menid));
			} else {
				$result = wf_alterarEstado( $arrMensario['docid'], AED_APROVAR_MENSARIO, $cmddsc = '', array('menid'=>$menid));
			}
			
		}
	}

	$al = array("alert"=>"Equipe aprovada com sucesso","location"=>"sispacto2.php?modulo=".$dados['modulo']."&acao=A&aba=aprovarusuario&fpbid=".$dados['fpbid']."&pflcodaprovar=".$dados['pflcodaprovar']);
	alertlocation($al);
	
}

function posAprovarMensario($menid) {
	global $db;
	
	$sql = "SELECT	t.tpeid, m.iusd, m.fpbid, p.pflcod, p.pfldsc, i.iuscpf, i.iusnaodesejosubstituirbolsa, i.iusnome, f.fpbmesreferencia, f.fpbanoreferencia, pp.plpvalor, un.uniid FROM sispacto2.mensario m 
			INNER JOIN sispacto2.identificacaousuario i ON i.iusd = m.iusd
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			INNER JOIN sispacto2.folhapagamento f ON f.fpbid = m.fpbid 
			INNER JOIN sispacto2.pagamentoperfil pp ON pp.pflcod = t.pflcod 
			INNER JOIN sispacto2.universidadecadastro un ON un.uncid = i.uncid 
			WHERE m.menid='".$menid."'";
	
	$arrInfo = $db->pegaLinha($sql);
	
	if($arrInfo['iusnaodesejosubstituirbolsa']!='t') {
		
		$sql = "SELECT 'Não foi possível criar o registro de bolsa para ".str_replace(array("'"),array(" "),$arrInfo['iusnome']).", pois a bolsa ja foi paga para ' || i.iusnome || ' => ' || 'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao FROM sispacto2.pagamentobolsista p 
				INNER JOIN sispacto2.identificacaousuario i ON i.iusd = p.iusd 
				INNER JOIN sispacto2.folhapagamento f ON f.fpbid = p.fpbid 
				INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
				WHERE p.tpeid='".$arrInfo['tpeid']."' AND p.fpbid='".$arrInfo['fpbid']."'";
		
		$descricao = $db->pegaUm($sql);
		
		echo $descricao;
		
		if($descricao) {
			die("<script>alert('".$descricao."');window.close();</script>");
		} else {
			$docid = wf_cadastrarDocumento(TPD_PAGAMENTOBOLSA, "Pagamento - ".$arrInfo['pfldsc']." - (".$arrInfo['iuscpf'].")".$arrInfo['iusnome']." - ".$arrInfo['fpbmesreferencia']."/".$arrInfo['fpbanoreferencia']);
			
			$sql = "INSERT INTO sispacto2.pagamentobolsista(
		            iusd, fpbid, docid, cpfresponsavel, pbodataenvio, pbovlrpagamento, 
		            pflcod, uniid, tpeid)
		    VALUES ('".$arrInfo['iusd']."', '".$arrInfo['fpbid']."', '".$docid."', '".$_SESSION['usucpf']."', NOW(), '".$arrInfo['plpvalor']."', 
		            '".$arrInfo['pflcod']."', '".$arrInfo['uniid']."', '".$arrInfo['tpeid']."');";
			
			$db->executar($sql);
			$db->commit();
			
		}
	}
	
	return true;
	
	
}

function calculaPorcentagemUsuarioAtivos($dados) {
	global $db;
	
	if($_REQUEST['modulo']=='principal/universidade/universidadeexecucao') {
		$sql_equipe = sqlEquipeCoordenadorIES(array("uncid"=>$_SESSION['sispacto2']['universidade']['uncid']));
		$sis = 'universidade';
	}
	
	if($_REQUEST['modulo']=='principal/coordenadoradjuntoies/coordenadoradjuntoies') {
		$sql_equipe = sqlEquipeCoordenadorAdjunto(array("uncid"=>$_SESSION['sispacto2']['coordenadoradjuntoies']['uncid']));
		$sis = 'coordenadoradjuntoies';
	}
	
	if($_REQUEST['modulo']=='principal/supervisories/supervisories') {
		$sql_equipe = sqlEquipeSupervisor(array("uncid"=>$_SESSION['sispacto2']['supervisories']['uncid']));
		$sis = 'supervisories';
	}
	
	if($_REQUEST['modulo']=='principal/formadories/formadories') {
		$sql_equipe = sqlEquipeFormador(array("iusd"=>$_SESSION['sispacto2']['formadories']['iusd'],"uncid"=>$_SESSION['sispacto2']['formadories']['uncid']));
		$sis = 'formadories';
	}
	
	if($_REQUEST['modulo']=='principal/orientadorestudo/orientadorestudo') {
		$sql_equipe = sqlEquipeOrientador(array("iusd"=>$_SESSION['sispacto2']['orientadorestudo']['iusd'],"uncid"=>$_SESSION['sispacto2']['orientadorestudo']['uncid']));
		$sis = 'orientadorestudo';
	}
	
	if($_REQUEST['modulo']=='principal/coordenadorlocal/coordenadorlocalexecucao') {
		$sql_equipe_p = sqlEquipeCoordenadorLocal(array("picid"=>$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']));
		$sis = 'coordenadorlocal';
	}
	
	if($sql_equipe_p) {
		$sql_total = "SELECT COUNT(*) as tot FROM ({$sql_equipe_p}) foo WHERE foo.status='A' AND CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." OR foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN foo.iusd in( SELECT i.iusd FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.pactoidadecerta p ON p.picid = i.picid INNER JOIN workflow.documento d ON d.docid = p.docidturma WHERE d.esdid='".ESD_FECHADO_TURMA."') ELSE true END";
		$total = $db->pegaUm($sql_total);
		
		$sql_total_a = "SELECT COUNT(*) as tot FROM ({$sql_equipe_p}) foo WHERE foo.status='A' AND CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." OR foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN foo.iusd in( SELECT i.iusd FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.pactoidadecerta p ON p.picid = i.picid INNER JOIN workflow.documento d ON d.docid = p.docidturma WHERE d.esdid='".ESD_FECHADO_TURMA."') ELSE true END";
		$total_a = $db->pegaUm($sql_total_a);
		
		if($total) $apassituacao = round(($total_a/$total)*100);
		
		gerenciarAtividadePacto(array('iusd'=>$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['iusd'],'apadatainicio'=>$apadatainicio,'apadatafim'=>$apadatafim,'apassituacao'=>$apassituacao,'suaid'=>$dados['suaid'],'picid'=>$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']));
	}
	
	if($sql_equipe) {
		$sql_total = "SELECT COUNT(*) as tot FROM ({$sql_equipe}) foo WHERE CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." OR foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN foo.iusd in( SELECT i.iusd FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.pactoidadecerta p ON p.picid = i.picid INNER JOIN workflow.documento d ON d.docid = p.docidturma WHERE d.esdid='".ESD_FECHADO_TURMA."') ELSE true END";
		$total = $db->pegaUm($sql_total);
		
		$sql_total_a = "SELECT COUNT(*) as tot FROM ({$sql_equipe}) foo WHERE foo.status='A' AND CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." OR foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN foo.iusd in( SELECT i.iusd FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.pactoidadecerta p ON p.picid = i.picid INNER JOIN workflow.documento d ON d.docid = p.docidturma WHERE d.esdid='".ESD_FECHADO_TURMA."') ELSE true END";
		$total_a = $db->pegaUm($sql_total_a);
		
		$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sispacto2'][$sis]['uncid']));
		
		if($total) $aunsituacao = round(($total_a/$total)*100);
		gerenciarAtividadeUniversidade(array('iusd'=>$_SESSION['sispacto2'][$sis]['iusd'],'aundatafim'=>$aundatafim,'aundatainicio'=>$aundatainicio,'aunsituacao'=>$aunsituacao,'ecuid'=>$ecuid,'suaid'=>$dados['suaid']));
	}
	
	
}

function gerenciarAtividadeUniversidade($dados) {
	global $db;
	
	$sql = "SELECT aunid FROM sispacto2.atividadeuniversidade a 
			WHERE suaid='".$dados['suaid']."' AND ecuid='".$dados['ecuid']."'".(($dados['iusd'])?" AND iusd='".$dados['iusd']."'":"");
	
	$aunid = $db->pegaUm($sql);
	
	if($aunid) {
		
		$sql = "UPDATE sispacto2.atividadeuniversidade SET 
				aunsituacao=".(($dados['aunsituacao'])?"'".$dados['aunsituacao']."'":"0").", 
				aundatainicio=".(($dados['aundatainicio'])?"'".$dados['aundatainicio']."'":"NULL").", 
				aundatafim=".(($dados['aundatafim'])?"'".$dados['aundatafim']."'":"NULL")."
			    WHERE aunid='".$aunid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sispacto2.atividadeuniversidade(
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
	
	$sql = "SELECT apaid FROM sispacto2.atividadepacto WHERE suaid='".$dados['suaid']."' AND picid='".$dados['picid']."'".(($dados['iusd'])?" AND iusd='".$dados['iusd']."'":"");
	$apaid = $db->pegaUm($sql);
	
	if($apaid) {
		
		$sql = "UPDATE sispacto2.atividadepacto SET 
				apassituacao=".(($dados['apassituacao'])?"'".$dados['apassituacao']."'":"NULL").", 
				apadatainicio=".(($dados['apadatainicio'])?"'".$dados['apadatainicio']."'":"NULL").",
				apadatafim=".(($dados['apadatafim'])?"'".$dados['apadatafim']."'":"NULL")."
			    WHERE apaid='".$apaid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO sispacto2.atividadepacto(
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
											  FROM sispacto2.subatividades su
										  	  INNER JOIN sispacto2.atividadeuniversidade ap ON su.suaid = ap.suaid 
										  	  INNER JOIN sispacto2.estruturacurso es ON es.ecuid = ap.ecuid 
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
									  FROM sispacto2.atividadeuniversidade au 
									  INNER JOIN sispacto2.estruturacurso es ON es.ecuid = au.ecuid
									  WHERE suaid='".$dados['suaid']."' AND es.uncid='".$dados['uncid']."'".(($dados['iusd'])?" AND au.iusd='".$dados['iusd']."'":""));
	
	return $atividadeuni;
	
	
}

function pegarEstruturaCurso($dados) {
	global $db;
	$sql = "SELECT ecuid FROM sispacto2.estruturacurso WHERE uncid='".$dados['uncid']."'";
	$ecuid = $db->pegaUm($sql);
	
	if(!$ecuid) {
		
		$sql = "INSERT INTO sispacto2.estruturacurso(
        	    uncid, muncod, ecustatus)
    			VALUES ('".$dados['uncid']."', NULL, 'A') RETURNING ecuid;";
		
		$ecuid = $db->pegaUm($sql);
		$db->commit();
		
	}
	
	return $ecuid;
	
}

function carregarPeriodoReferencia($dados) {
	global $db;
	
	if($dados['fpbid'] && !is_numeric($dados['fpbid'])) {
		$al = array("alert"=>"Período de Referência não identificado. Tente novamente","location"=>"sismedio.php?modulo=".$_REQUEST['modulo']."&acao=".$_REQUEST['acao']."&aba=".$_REQUEST['aba']);
		alertlocation($al);
	}
	
	if($dados['pflcod_avaliador']) {
		$plpmaximobolsas = $db->pegaUm("SELECT plpmaximobolsas FROM sispacto2.pagamentoperfil WHERE pflcod='".$dados['pflcod_avaliador']."'");
		if($plpmaximobolsas) $limit = "LIMIT {$plpmaximobolsas}";
	}
	
	if($dados['pflcod_avaliador'] == PFL_EQUIPEMEC) $dados['pflcod_avaliador'] = null;
	
	$sql = "SELECT f.fpbid as codigo, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao 
			FROM sispacto2.folhapagamento f 
			INNER JOIN sispacto2.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE f.fpbstatus='A' AND rf.uncid='".$dados['uncid']."' ".(($dados['pflcod_avaliador'])?"AND rf.pflcod='".$dados['pflcod_avaliador']."'":"AND rf.pflcod IS NULL")." AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd') {$limit}";
	
	$sql_tot = "SELECT count(*) as tot FROM ($sql) as foo";
	$tot = $db->pegaUm($sql_tot);
	
	if(!$tot) {
		echo "<br><div style=\"width: 80%;padding: 10px;border: 5px solid gray;margin: 0px;\">Não existem períodos de referências cadastrados, isso ocorre porque a universidade não fechou o registro de Frequência e/ou o MEC não selecionou os períodos de referência.</div><br>";
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
	
	$wh[] = "d.esdid NOT IN('".ESD_INVALIDADO_MENSARIO."')";
	
	$sql = "SELECT {$acao} foo3.periodo, per.pflcod, per.pfldsc, SUM(napto) as na, SUM(apto) as ap, SUM(aprov) as ar FROM (
	SELECT foo2.fpbid, 'Ref.'||m.mesdsc||'/'||f.fpbanoreferencia as periodo, foo2.pflcod,  CASE WHEN foo2.resultado='Não Apto' THEN 1 ELSE 0 END as napto, CASE WHEN foo2.resultado='Apto' THEN 1 ELSE 0 END as apto, CASE WHEN foo2.resultado='Aprovado' THEN 1 ELSE 0 END as aprov
	FROM sispacto2.folhapagamento f 
	INNER JOIN public.meses m ON m.mescod::numeric = f.fpbmesreferencia 
	INNER JOIN (
	
	SELECT foo.pflcod,
			".criteriosAprovacao('restricao3').", foo.fpbid FROM (
	SELECT 
	COALESCE((SELECT AVG(mavtotal) FROM sispacto2.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
	(SELECT COUNT(mapid) FROM sispacto2.materiaisprofessores mp WHERE mp.iusd=m.iusd) as totalmateriaisprofessores,
	(SELECT COUNT(*) FROM sispacto2.turmasprofessoresalfabetizadores pa WHERE tpastatus='A' AND (coalesce(tpatotalmeninos,0)+coalesce(tpatotalmeninas,0))!=0 AND pa.iusd=m.iusd) as totalturmas,
	(SELECT COUNT(*) FROM sispacto2.gestaomobilizacaoperguntas gm WHERE gm.iusd=m.iusd) as rcoordenadorlocal,
 							  		
	(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens 
	FROM sispacto2.aprendizagemconhecimentoturma a 
 	INNER JOIN sispacto2.aprendizagemconhecimento c ON c.catid = a.catid
	INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
	WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='M' AND t.iusd=m.iusd) as aprendizagemMat,
 					
	(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens 
	FROM sispacto2.aprendizagemconhecimentoturma2 a 
 	INNER JOIN sispacto2.aprendizagemconhecimento c ON c.catid = a.catid
	INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
	WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='M' AND t.iusd=m.iusd) as aprendizagemMat2,
 							  		
	(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens 
	FROM sispacto2.aprendizagemconhecimentoturma a 
 	INNER JOIN sispacto2.aprendizagemconhecimento c ON c.catid = a.catid
	INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
	WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='P' AND t.iusd=m.iusd) as aprendizagemPor,
 					
	(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens 
	FROM sispacto2.aprendizagemconhecimentoturma2 a 
 	INNER JOIN sispacto2.aprendizagemconhecimento c ON c.catid = a.catid
	INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
	WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='P' AND t.iusd=m.iusd) as aprendizagemPor2,
	(SELECT count(*) as itens FROM sispacto2.usomateriaisdidaticos WHERE iusd=m.iusd) as aprendizagemUsoMateriaisDidaticos,
 	(SELECT count(*) as itens FROM sispacto2.relatoexperiencia WHERE iusd=m.iusd) as relatoexperiencia,
	(SELECT count(*) as itens FROM sispacto2.impressoesana i INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = i.tpaid AND i.iusd = t.iusd WHERE i.iusd=m.iusd) as impressoesana,
 	(SELECT count(*) as itens FROM sispacto2.questoesdiversasatv8 WHERE iusd=m.iusd) as questoesdiversasatv8,
 	(SELECT count(*) as itens FROM sispacto2.contribuicaopacto WHERE iusd=m.iusd) as contribuicaopacto,
 							  		
	i.iusdocumento,
 	i.reuid,
	i.iustermocompromisso,
	i.iusnaodesejosubstituirbolsa,
	m.fpbid,
	d.esdid,
	m.pflcod,
	i.iustipoprofessor,
	pp.plpmaximobolsas,
 	fpu.rfuparcela,
	(SELECT COUNT(DISTINCT pflcodavaliador) FROM sispacto2.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes,
	(SELECT COUNT(DISTINCT fpbid) FROM sispacto2.pagamentobolsista pg WHERE pg.iusd=m.iusd) as qtduspagamento,
	(SELECT COUNT(DISTINCT fpbid) FROM sispacto2.pagamentobolsista pg WHERE pg.tpeid=t.tpeid) as qtdtppagamento,
	(SELECT COUNT(mavid) FROM sispacto2.mensarioavaliacoes ma  WHERE ma.menid=m.menid AND ma.mavfrequencia=0) as numeroausencia,
	dorien.esdid as esdorien,
	dturpr.esdid as esdturpr
	FROM sispacto2.mensario m
	INNER JOIN sispacto2.identificacaousuario i ON i.iusd = m.iusd 
 	INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd  
	INNER JOIN sispacto2.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = m.pflcod AND fpu.fpbid = m.fpbid
	LEFT JOIN sispacto2.pagamentoperfil pp ON pp.pflcod = m.pflcod 
	LEFT JOIN sispacto2.pactoidadecerta pic ON pic.picid = i.picid 
	LEFT JOIN workflow.documento dorien ON dorien.docid = pic.docid 
	LEFT JOIN workflow.documento dturpr ON dturpr.docid = pic.docidturma 
			
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
	$db->monta_lista($sql,$cabecalho,100,10,'N','center','N','','','',21600,array('ordena'=>false));
	
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
			FROM sispacto2.identificacaousuario u 
			INNER JOIN sispacto2.tipoperfil t on t.iusd=u.iusd 
			INNER JOIN seguranca.perfil p on p.pflcod = t.pflcod 
			LEFT JOIN seguranca.usuario_sistema us on us.usucpf=u.iuscpf and us.sisid=".SIS_SISPACTO." 
			LEFT JOIN seguranca.usuario usu on usu.usucpf = u.iuscpf
			WHERE u.iusstatus='A' AND 
				  CASE WHEN t.pflcod=".PFL_ORIENTADORESTUDO." THEN u.iusformacaoinicialorientador=true ELSE true END AND 
				  t.pflcod in(
						".PFL_PROFESSORALFABETIZADOR.",
						".PFL_COORDENADORLOCAL.",
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
			INNER JOIN sispacto2.pagamentobolsista pb ON pb.pflcod = p.pflcod 
			INNER JOIN sispacto2.universidadecadastro un ON un.uniid = pb.uniid 
			INNER JOIN workflow.documento dc ON dc.docid = pb.docid AND dc.tpdid=".TPD_PAGAMENTOBOLSA." 
			WHERE p.pflcod IN(
			".PFL_PROFESSORALFABETIZADOR.",
			".PFL_COORDENADORLOCAL.",
			".PFL_ORIENTADORESTUDO.",
			".PFL_COORDENADORIES.",
			".PFL_COORDENADORADJUNTOIES.",
			".PFL_SUPERVISORIES.",
			".PFL_FORMADORIES.") ".(($wh)?" AND ".implode(" AND ",$wh):"")."

			) fee 

			GROUP BY fee.pflcod, fee.pfldsc
			
			) foo
			
			INNER JOIN sispacto2.pagamentoperfil pp ON pp.pflcod = foo.pflcod";
	
	
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
					(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_SISPACTO.") as status,
					(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod = t.pflcod) as perfil,
					(SELECT usucpf FROM sispacto2.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					CASE WHEN pic.picid IS NOT NULL THEN 
														CASE WHEN pic.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN pic.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
					
			FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sispacto2.pactoidadecerta pic ON pic.picid = i.picid 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = pic.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE t.pflcod IN('".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."','".PFL_COORDENADORLOCAL."','".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORIES."','".PFL_ORIENTADORESTUDO."') AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome";
	
	return $sql;
}

function cadastrarPeriodoReferencia($dados) {
	global $db;
	
	$uncids = array_keys($dados['smesini']);
	
	if($uncids) {
		foreach($uncids as $uncid) {
			
			$sql = "DELETE FROM sispacto2.folhapagamentouniversidade WHERE uncid='".$uncid."'";
			$db->executar($sql);
			
			$sql = "select foo.fpbid from (
					select fpbid, fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0') as dt from sispacto2.folhapagamento ) foo
					where foo.dt >= '".$dados['sanoinicio'][$uncid]."-".str_pad($dados['smesini'][$uncid],2,"0", STR_PAD_LEFT)."' AND foo.dt <= '".$dados['sanofim'][$uncid]."-".str_pad($dados['smesfim'][$uncid],2,"0", STR_PAD_LEFT)."'";
			
			$fpbids = $db->carregarColuna($sql);
			
			if($fpbids) {
				foreach($fpbids as $key => $fpbid) {
					$sql = "INSERT INTO sispacto2.folhapagamentouniversidade(
	            			uncid, fpbid, rfuparcela)
						    VALUES ('".$uncid."', '".$fpbid."', '".($key+1)."');";
					
					$db->executar($sql);
					
				}
			}

			$perfis = $db->carregarColuna("SELECT p.pflcod FROM seguranca.perfil p
												   INNER JOIN sispacto2.pagamentoperfil pp ON pp.pflcod = p.pflcod
												   ORDER BY p.pflnivel");
				
			foreach($perfis as $pflcod) {

				$sql = "select foo.fpbid from (
									select fpbid, fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0') as dt from sispacto2.folhapagamento ) foo
									where foo.dt >= '".$dados['sanoiniciop'][$uncid][$pflcod]."-".str_pad($dados['smesinip'][$uncid][$pflcod],2,"0", STR_PAD_LEFT)."' AND foo.dt <= '".$dados['sanofimp'][$uncid][$pflcod]."-".str_pad($dados['smesfimp'][$uncid][$pflcod],2,"0", STR_PAD_LEFT)."'";
					
				$fpbids = $db->carregarColuna($sql);
					
				if($fpbids) {
					foreach($fpbids as $key => $fpbid) {

						$sql = "INSERT INTO sispacto2.folhapagamentouniversidade(
													            			uncid, fpbid, rfuparcela, pflcod)
																		    VALUES ('".$uncid."', '".$fpbid."', '".($key+1)."', '".$pflcod."');";
							
						$db->executar($sql);
							
					}
				}
					
			}
				
			
		}
	}
	
	$db->commit();
	
	$al = array("alert"=>"Período de referência aprovado com sucesso","location"=>"sispacto2.php?modulo=principal/mec/mec&acao=A&aba=configuracoes");
	alertlocation($al);
	
	
}

function carregarLogCadastroSGB($dados) {
	global $db;
	
	$iusd = $db->pegaUm("SELECT iusd FROM sispacto2.identificacaousuario WHERE iuscpf='".$dados['usucpf']."'");
	
	if($iusd) echo "<input type=hidden name=iusd id=iusd_log value=\"".$iusd."\">";
	
	$sql = "SELECT u.iuscpf, u.iusnome, to_char(logdata,'dd/mm/YYYY HH24:MI') as data, logresponse FROM log_historico.logsgb_sispacto2 l 
			INNER JOIN sispacto2.identificacaousuario u ON u.iuscpf = l.logcpf 
			WHERE logcpf='".$dados['usucpf']."' AND logservico='gravarDadosBolsista' ORDER BY l.logdata DESC LIMIT 5";
	$cabecalho = array("CPF","Nome","Data","Erro");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true,false,false,true);
	
}

function criteriosAprovacao($cla) {
	global $db;
	
	$cl['restricao4'] = "CASE 
						 WHEN foo.mensarionota < 7 THEN '<span style=color:red;font-size:x-small;>Bolsista não possui avaliação positiva (maior/igual a 7)</span>'  
						 WHEN foo.iustermocompromisso=false THEN '<span style=color:red;font-size:x-small;>Bolsista não preencheu os dados cadastrais</span>'
						 WHEN foo.iusnaodesejosubstituirbolsa=true      THEN '<span style=color:red;font-size:x-small;>Bolsista do FNDE/MEC e não deseja substituir bolsa atual pela bolsa do PACTO</span>'
						 WHEN foo.fpbidini IS NOT NULL AND foo.fpbidfim IS NOT NULL AND (foo.fpbid < foo.fpbidini OR foo.fpbid > foo.fpbidfim) THEN '<span style=color:red;font-size:x-small;>Este período de referência não esta habilitado para pagamento</span>'
						 WHEN ((foo.qtduspagamento >= foo.plpmaximobolsas) OR (foo.qtdtppagamento >= foo.plpmaximobolsas)) THEN '<span style=color:red;font-size:x-small;>Número máximo de avaliações ('||foo.plpmaximobolsas||') foi atingido</span>' 
						 WHEN foo.reuid IS NOT NULL THEN '<div style=\"color:red;font-size:x-small;width:300px;height:40px;overflow:auto;\">'||(SELECT COALESCE(reurestricao,'') FROM sispacto2.restricaousuario WHERE reuid=foo.reuid)||'</div>'
						 WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." THEN
						                                                  CASE  WHEN foo.esdorien != ".ESD_VALIDADO_COORDENADOR_LOCAL." THEN '<center><span style=color:red;font-size:x-small;>Cadastro de Orientadores de Estudo não foi validado pela universidade.<br>Contate o Coordenador Local - '||foo.rede||'</span></center>'
																				WHEN foo.iusdocumento=false   THEN '<center><span style=color:red;font-size:x-small;>Possui problemas na documentação</span></center>'
						   													  	WHEN foo.numeroavaliacoes < 2 THEN '<span style=color:red;font-size:x-small;>As avaliações do Formador IES e Coordenador Local são obrigatórias</span>'
						   													  	WHEN foo.numeroausencia > 0 THEN '<span style=color:red;font-size:x-small;>Ausência na Universidade e/ou Município</span>'
						   													  	ELSE '<span style=color:blue;font-size:x-small;>Nenhuma restrição</span>'
						   												  END
				   		 WHEN foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN
					   														CASE WHEN (foo.esdorien != '".ESD_VALIDADO_COORDENADOR_LOCAL."' OR foo.esdturpr != '".ESD_FECHADO_TURMA."') THEN '<center><span style=color:red;font-size:x-small;>Cadastro dos professores e/ou Orientadores de Estudo não foi finalizado.<br>Contate o Coordenador Local('||foo.rede||')</span></center>'
																				 WHEN foo.totalturmas = 0 AND foo.rfuparcela > 0 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (1º Parcela).</span>'
																				 WHEN foo.aprendizagemMat != ".APRENDIZAGEM_MATEMATICA." AND foo.rfuparcela > 1 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (2º Parcela)</span>'
																				 WHEN foo.aprendizagemPor != ".APRENDIZAGEM_PORTUGUES." AND foo.rfuparcela > 2 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (3º Parcela)</span>'
																				 WHEN foo.totalmateriaisprofessores = 0 AND foo.rfuparcela > 3 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (4º Parcela)</span>' 
 	            																 WHEN foo.aprendizagemUsoMateriaisDidaticos != ".APRENDIZAGEM_MATERIALDIDATICO." AND foo.rfuparcela > 4 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (5º Parcela)</span>'
 			            														 WHEN foo.relatoexperiencia = 0 AND foo.rfuparcela > 5 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (6º Parcela)</span>'
     		 			            											 WHEN foo.impressoesana != foo.totalturmas AND foo.rfuparcela > 6 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (7º Parcela)</span>'
 			            														 WHEN foo.questoesdiversasatv8 = 0 AND foo.rfuparcela > 7 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (8º Parcela)</span>'
																				 WHEN foo.aprendizagemMat2 != ".APRENDIZAGEM_MATEMATICA." AND foo.rfuparcela > 8 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (9º Parcela) - Matemática</span>'
																				 WHEN foo.aprendizagemPor2 != ".APRENDIZAGEM_PORTUGUES." AND foo.rfuparcela > 8 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (9º Parcela) - Português</span>'
																				 WHEN foo.contribuicaopacto = 0 AND foo.rfuparcela > 9 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (10º Parcela)</span>'
					   															 WHEN foo.iustipoprofessor = 'censo' THEN '<span style=color:blue;font-size:x-small;>Nenhuma restrição</span>'
					   															 ELSE '<span style=color:red;font-size:x-small;>Professor Alfabetizador não cadastrado no censo 2013</span>' END
				   		 WHEN foo.pflcod=".PFL_COORDENADORLOCAL." THEN 
					   	 													CASE WHEN (foo.esdorien != '".ESD_VALIDADO_COORDENADOR_LOCAL."' OR foo.esdturpr != '".ESD_FECHADO_TURMA."') THEN '<center><span style=color:red;font-size:x-small;>Cadastro dos professores e/ou Orientadores de Estudo não foi finalizado.</span></center>'
 			    																-- WHEN foo.rcoordenadorlocal = 0 THEN '<span style=color:red;>Falta preencher as informações sobre \"Gestão e Mobilização\" no SisPacto</span>' 
					   	 													ELSE '<span style=color:blue;>Nenhuma restrição</span>' END
     			
				   		 ELSE '<span style=color:blue;font-size:x-small;>Nenhuma restrição</span>' END as restricao";
	
	
	$cl['restricao1'] = "CASE 
						 WHEN foo.iusnaodesejosubstituirbolsa=true      THEN '<span style=color:red;font-size:x-small;>Bolsista do FNDE/MEC e não deseja substituir bolsa atual pela bolsa do PACTO</span>' 
						 WHEN foo.fpbidini IS NOT NULL AND foo.fpbidfim IS NOT NULL AND (foo.fpbid < foo.fpbidini OR foo.fpbid > foo.fpbidfim) THEN '<span style=color:red;>Este período de referência não esta habilitado para pagamento</span>' 
						 WHEN ((foo.qtduspagamento >= foo.plpmaximobolsas) OR (foo.qtdtppagamento >= foo.plpmaximobolsas)) THEN '<span style=color:red;font-size:x-small;>Número máximo de avaliações ('||foo.plpmaximobolsas||') foi atingido</span>'
						 WHEN foo.reuid IS NOT NULL THEN '<div style=\"color:red;font-size:x-small;width:300px;height:40px;overflow:auto;\">'||(SELECT COALESCE(reurestricao,'') FROM sispacto2.restricaousuario WHERE reuid=foo.reuid)||'</div>'
						 WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." THEN 
						                                                  CASE  WHEN foo.esdorien != ".ESD_VALIDADO_COORDENADOR_LOCAL." THEN '<center><span style=color:red;font-size:x-small;>Cadastro de Orientadores de Estudo não foi validado pela universidade.<br>Contate o Coordenador Local - '||foo.rede||'</span></center>'
 			            														WHEN foo.iusdocumento=false   THEN '<center><span style=color:red;>Possui problemas na documentação</span></center>'
						   													  	WHEN foo.numeroavaliacoes < 2 THEN '<span style=color:red;>As avaliações do Formador IES e Coordenador Local são obrigatórias</span>' 
						   													  	WHEN foo.numeroausencia > 0 THEN '<span style=color:red;>Ausência na Universidade e/ou Município</span>'
						   													  	ELSE '<span style=color:blue;>Nenhuma restrição</span>' 
						   												  END
				   		 WHEN foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN 
					   														CASE WHEN (foo.esdorien != '".ESD_VALIDADO_COORDENADOR_LOCAL."' OR foo.esdturpr != '".ESD_FECHADO_TURMA."') THEN '<center><span style=color:red;font-size:x-small;>Cadastro dos professores e/ou Orientadores de Estudo não foi finalizado.<br>Contate o Coordenador Local - '||foo.rede||'</span></center>'
														 						 WHEN foo.totalturmas = 0 AND foo.rfuparcela > 0 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (1º Parcela)</span>'
 			    																 WHEN foo.aprendizagemMat != ".APRENDIZAGEM_MATEMATICA." AND foo.rfuparcela > 1 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (2º Parcela)</span>'
 			            														 WHEN foo.aprendizagemPor != ".APRENDIZAGEM_PORTUGUES." AND foo.rfuparcela > 2 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (3º Parcela)</span>'
 			            														 WHEN foo.totalmateriaisprofessores = 0 AND foo.rfuparcela > 3 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (4º Parcela)</span>' 
																				 WHEN foo.aprendizagemUsoMateriaisDidaticos != ".APRENDIZAGEM_MATERIALDIDATICO." AND foo.rfuparcela > 4 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (5º Parcela)</span>' 
																				 WHEN foo.relatoexperiencia = 0 AND foo.rfuparcela > 5 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (6º Parcela)</span>'
																				 WHEN foo.impressoesana != foo.totalturmas AND foo.rfuparcela > 6 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (7º Parcela)</span>'
    																			 WHEN foo.questoesdiversasatv8 = 0 AND foo.rfuparcela > 7 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (8º Parcela)</span>'
																				 WHEN foo.aprendizagemMat2 != ".APRENDIZAGEM_MATEMATICA." AND foo.rfuparcela > 8 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (9º Parcela) - Matemática</span>'
																				 WHEN foo.aprendizagemPor2 != ".APRENDIZAGEM_PORTUGUES." AND foo.rfuparcela > 8 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (9º Parcela) - Português</span>' 
					 															 WHEN foo.contribuicaopacto = 0 AND foo.rfuparcela > 9 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (10º Parcela) - Português</span>'
					   															 WHEN foo.iustipoprofessor = 'censo' THEN '<span style=color:blue;font-size:x-small;>Nenhuma restrição</span>' 
					   															 ELSE '<span style=color:red;>Professor Alfabetizador não cadastrado no censo 2013</span>' END 
				   		 WHEN foo.pflcod=".PFL_COORDENADORLOCAL." THEN 
					   	 													CASE WHEN (foo.esdorien != '".ESD_VALIDADO_COORDENADOR_LOCAL."' OR foo.esdturpr != '".ESD_FECHADO_TURMA."') THEN '<center><span style=color:red;font-size:x-small;>Cadastro dos professores e/ou Orientadores de Estudo não foi finalizado.</span></center>'
						 --														 WHEN foo.rcoordenadorlocal = 0 THEN '<span style=color:red;>Falta preencher as informações sobre \"Gestão e Mobilização\" no SisPacto</span>' 
					   	 													ELSE '<span style=color:blue;>Nenhuma restrição</span>' END
				   		 ELSE '<span style=color:blue;>Nenhuma restrição</span>' END as restricao";
	
	$cl['restricao2'] = "CASE WHEN foo.mensarionota >= 7  AND foo.iustermocompromisso=true AND 
						(CASE WHEN foo.fpbidini IS NOT NULL AND foo.fpbidfim IS NOT NULL AND (foo.fpbid < foo.fpbidini OR foo.fpbid > foo.fpbidfim) THEN false ELSE true END) AND
						(CASE WHEN ((foo.qtduspagamento >= foo.plpmaximobolsas) OR (foo.qtdtppagamento >= foo.plpmaximobolsas)) THEN false ELSE true END) AND 
						(CASE WHEN foo.reuid IS NOT NULL THEN false ELSE true END) AND  
						(CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." THEN 
																			CASE WHEN foo.esdorien != ".ESD_VALIDADO_COORDENADOR_LOCAL." THEN false
																				 WHEN foo.iusdocumento=false THEN false 
																				 WHEN foo.numeroausencia > 0 THEN false
																				 WHEN foo.numeroavaliacoes > 1 THEN true ELSE false 
																				 END
				   		 WHEN foo.pflcod=".PFL_COORDENADORLOCAL." THEN 
					   	 													CASE WHEN (foo.esdorien != '".ESD_VALIDADO_COORDENADOR_LOCAL."' OR foo.esdturpr != '".ESD_FECHADO_TURMA."') THEN false
	 																			 --WHEN foo.rcoordenadorlocal = 0 THEN false 
					   	 													ELSE true END
				
							WHEN foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN 
																				CASE WHEN (foo.esdorien != '".ESD_VALIDADO_COORDENADOR_LOCAL."' OR foo.esdturpr != '".ESD_FECHADO_TURMA."') THEN false
																					 WHEN foo.totalturmas = 0 AND foo.rfuparcela > 0 THEN false
																					 WHEN foo.aprendizagemMat != ".APRENDIZAGEM_MATEMATICA." AND foo.rfuparcela > 1 THEN false 
																					 WHEN foo.aprendizagemPor != ".APRENDIZAGEM_PORTUGUES." AND foo.rfuparcela > 2 THEN false
																					 WHEN foo.totalmateriaisprofessores = 0 AND foo.rfuparcela > 3 THEN false 
    																				 WHEN foo.aprendizagemUsoMateriaisDidaticos != ".APRENDIZAGEM_MATERIALDIDATICO." AND foo.rfuparcela > 4 THEN false 
 							  														 WHEN foo.relatoexperiencia = 0 AND foo.rfuparcela > 5 THEN false
																					 WHEN foo.impressoesana != foo.totalturmas AND foo.rfuparcela > 6 THEN false
			    																	 WHEN foo.questoesdiversasatv8 = 0 AND foo.rfuparcela > 7 THEN false 
																				 	 WHEN foo.aprendizagemMat2 != ".APRENDIZAGEM_MATEMATICA." AND foo.rfuparcela > 8 THEN false
																				 	 WHEN foo.aprendizagemPor2 != ".APRENDIZAGEM_PORTUGUES." AND foo.rfuparcela > 8 THEN false
			    																	 WHEN foo.contribuicaopacto = 0 AND foo.rfuparcela > 9 THEN false
																					 WHEN foo.iustipoprofessor = 'censo' THEN true 
																				ELSE false END
							ELSE true END) AND foo.iusnaodesejosubstituirbolsa=false  THEN CASE WHEN foo.notacomplementar >= 7 THEN 'checked' ELSE '' END
																								
						ELSE 'disabled' END";
	
	$cl['restricao3'] = "CASE WHEN foo.iusnaodesejosubstituirbolsa=true THEN 'Não Apto' 
 			    			  WHEN ((foo.qtduspagamento >= foo.plpmaximobolsas) OR (foo.qtdtppagamento >= foo.plpmaximobolsas)) THEN 'Não Apto'
 							  WHEN foo.reuid IS NOT NULL THEN 'Não Apto' 
							  WHEN foo.esdid=".ESD_APROVADO_MENSARIO." THEN 'Aprovado'
						 	  WHEN foo.mensarionota >= 7  AND foo.iustermocompromisso=true AND (CASE WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." THEN 
																																					CASE WHEN foo.esdorien != ".ESD_VALIDADO_COORDENADOR_LOCAL." THEN false
																																						 WHEN foo.iusdocumento=false THEN false 
																																						 WHEN foo.numeroausencia > 0 THEN false
																																						 WHEN foo.numeroavaliacoes > 1 THEN true ELSE false END 
																							   		 WHEN foo.pflcod=".PFL_COORDENADORLOCAL." THEN 
																			   						 								CASE WHEN (foo.esdorien != '".ESD_VALIDADO_COORDENADOR_LOCAL."' OR foo.esdturpr != '".ESD_FECHADO_TURMA."') THEN false
 																																		 --WHEN foo.rcoordenadorlocal = 0 THEN false 
																			   						 								ELSE true END
																			
																									WHEN foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN 
																																						CASE WHEN (foo.esdorien != '".ESD_VALIDADO_COORDENADOR_LOCAL."' OR foo.esdturpr != '".ESD_FECHADO_TURMA."') THEN false
			     																																			 WHEN foo.totalturmas = 0 AND foo.rfuparcela > 0 THEN false
																																							 WHEN foo.aprendizagemMat != ".APRENDIZAGEM_MATEMATICA." AND foo.rfuparcela > 1 THEN false 
																																							 WHEN foo.aprendizagemPor != ".APRENDIZAGEM_PORTUGUES." AND foo.rfuparcela > 2 THEN false
																																							 WHEN foo.totalmateriaisprofessores = 0 AND foo.rfuparcela > 3 THEN false 
			     																																			 WHEN foo.aprendizagemUsoMateriaisDidaticos != ".APRENDIZAGEM_MATERIALDIDATICO." AND foo.rfuparcela > 4 THEN false
			    																																			 WHEN foo.relatoexperiencia = 0 AND foo.rfuparcela > 5 THEN false 
				 			    																															 WHEN foo.impressoesana != foo.totalturmas AND foo.rfuparcela > 6 THEN false
			    																																			 WHEN foo.questoesdiversasatv8 = 0 AND foo.rfuparcela > 7 THEN false 
																																						 	 WHEN foo.aprendizagemMat2 != ".APRENDIZAGEM_MATEMATICA." AND foo.rfuparcela > 8 THEN false
																																						 	 WHEN foo.aprendizagemPor2 != ".APRENDIZAGEM_PORTUGUES." AND foo.rfuparcela > 8 THEN false
																																							 WHEN foo.contribuicaopacto = 0 AND foo.rfuparcela > 9 THEN false
																																							 WHEN foo.iustipoprofessor = 'censo' THEN true 
																																						ELSE false END
																									ELSE true END) THEN 'Apto' 
		    ELSE 'Não Apto' END resultado";
	
	
	return $cl[$cla];
	
}

function verificarEmailUnico($dados) {
	global $db;
	
	$sql = "SELECT '- '||i.iusnome||' ( '||p.pfldsc||' );' as descricao FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
			WHERE i.iusemailprincipal='".$dados['iusemailprincipal']."' AND i.iusd!='".$dados['iusd']."' AND i.iusstatus='A'";
	
	$nomes = $db->carregarColuna($sql);
	
	echo implode('\n', $nomes);
	
	
}

function consultarDetalhesPagamento($dados) {
	global $db;
	$sql = "SELECT i.iusnome, me.mesdsc||'/'||fa.fpbanoreferencia as periodo, e.esddsc, p.pbovlrpagamento, pp.pfldsc, uni.uninome, uni.unicnpj, p.docid FROM sispacto2.pagamentobolsista p 
			INNER JOIN sispacto2.identificacaousuario i ON i.iusd = p.iusd 
			INNER JOIN sispacto2.folhapagamento fa ON fa.fpbid = p.fpbid 
			INNER JOIN public.meses me ON me.mescod::integer = fa.fpbmesreferencia 
			INNER JOIN workflow.documento d ON d.docid = p.docid  AND d.tpdid=".TPD_PAGAMENTOBOLSA."
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			INNER JOIN seguranca.perfil pp ON pp.pflcod = p.pflcod 
			INNER JOIN sispacto2.universidade uni ON uni.uniid = p.uniid  
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
				SELECT count(p.picid) as tot, ".$dados['group']." as grouper FROM sispacto2.materiais m 
				INNER JOIN sispacto2.pactoidadecerta p ON p.picid = m.picid 
				INNER JOIN sispacto2.abrangencia a ON a.muncod = p.muncod 
				INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='M' and e.uncid='".$dados['uncid']."' GROUP BY ".$dados['group']."
				) UNION ALL (
				SELECT count(p.picid) as tot, ".$dados['group']." as grouper FROM sispacto2.materiais m 
				INNER JOIN sispacto2.pactoidadecerta p ON p.picid = m.picid 
				INNER JOIN territorios.municipio mu ON mu.estuf = p.estuf  
				INNER JOIN sispacto2.abrangencia a ON a.muncod = mu.muncod 
				INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='E' and e.uncid='".$dados['uncid']."' GROUP BY ".$dados['group']."
				)
				) foo";
		
		$materiais = $db->carregar($sql);
		
	} else {
		
		$materiais = $db->carregar("SELECT count(picid) as tot, ".$dados['group']." as grouper FROM sispacto2.materiais GROUP BY ".$dados['group']);
				
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
	$materiais = $db->carregar("SELECT count(iusd) as tot, ".$dados['group']." as grouper FROM sispacto2.materiaisprofessores GROUP BY ".$dados['group']);

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
	
	$npagamentos = $db->pegaUm("SELECT COUNT(*) FROM sispacto2.pagamentobolsista WHERE iusd='".$dados['iusd']."'");
	
	if($npagamentos > 0) {
		
		$identificacaousuario = $db->pegaLinha("SELECT i.iuscpf, t.pflcod, p.pfldsc FROM sispacto2.identificacaousuario i 
												INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
												INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod  
												WHERE i.iusd='".$dados['iusd']."'");
		
		
		$sql = "INSERT INTO sispacto2.identificacaousuario(
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
				  FROM sispacto2.identificacaousuario where iusd='".$dados['iusd']."'
				RETURNING iusd;";
		
		$iusd_novo = $db->pegaUm($sql);
		
		
		$sql = "DELETE FROM sispacto2.usuarioresponsabilidade  WHERE rpustatus='A' AND usucpf='".$identificacaousuario['iuscpf']."' AND pflcod='".$identificacaousuario['pflcod']."'";
		$db->executar($sql);
	
		$sql = "UPDATE sispacto2.tipoperfil SET iusd='".$iusd_novo."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	
		$sql = "UPDATE sispacto2.turmas SET iusd='".$iusd_novo."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	
		$sql = "UPDATE sispacto2.orientadorturma SET iusd='".$iusd_novo."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
		
		$sql = "UPDATE sispacto2.orientadorturmaoutros SET iusd='".$iusd_novo."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	
		$sql = "UPDATE sispacto2.identificacaousuario SET iusstatus='I' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	
		$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$identificacaousuario['iuscpf']."' AND pflcod='".$identificacaousuario['pflcod']."'";
		$db->executar($sql);
	
		// removendo avaliações não concluidas
		$sql = "SELECT m.menid FROM sispacto2.mensario m 
				INNER JOIN workflow.documento d ON d.docid = m.docid 
				WHERE iusd='".$dados['iusd']."' AND d.esdid!='".ESD_APROVADO_MENSARIO."'";
	
		$menids = $db->carregarColuna($sql);
		
		if($menids) {
			
			$sql = "SELECT mavid FROM sispacto2.mensarioavaliacoes WHERE menid IN('".implode("','",$menids)."')";
			$mavids = $db->carregarColuna($sql);
			
			if($mavids) {
				$db->executar("DELETE FROM sispacto2.historicoreaberturanota WHERE mavid IN('".implode("','",$mavids)."')");
				$db->executar("DELETE FROM sispacto2.mensarioavaliacoes WHERE mavid IN('".implode("','",$mavids)."')");
			}
		}
		
		
	} else {
	
		removerTipoPerfil(array('iusd'=>$dados['iusd'],'pflcod'=>$dados['pflcod'],'naoredirecionar'=>true));
	
	}
	
	if(!$dados['uncid']) $dados['uncid'] = $db->pegaUm("SELECT uncid FROM sispacto2.identificacaousuario WHERE iusd='".$dados['iusd']."'");

	$sql = "INSERT INTO sispacto2.historicotrocausuario(
            iusdantigo, pflcod, hstdata, usucpf, uncid, 
            hstacao)
    		VALUES ('".$dados['iusd']."', '".$dados['pflcod']."', NOW(), '".$_SESSION['usucpf']."', '".$dados['uncid']."', 'R');";
	
	$db->executar($sql);
	$db->commit();
	
	gerarVersaoProjetoUniversidade(array('uncid'=>$dados['uncid']));
	
	$al = array("alert"=>"Exclusão ocorrida com sucesso","location"=>"sispacto2.php?modulo=".$dados['modulo']."&acao=".$dados['acao']."&aba=gerenciarusuario&uncid=".$dados['uncid']);
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
	
	$_SESSION['imgparams']['filtro'] = "1=1";
	
	if($dados['uncid']) {
		$sql = "SELECT m.arqid, m.matid, m.mafdsc FROM sispacto2.materiaisfotos m
				INNER JOIN sispacto2.materiais ma ON ma.matid = m.matid 
				INNER JOIN sispacto2.pactoidadecerta p ON p.picid = ma.picid 
				INNER JOIN sispacto2.abrangencia a ON a.muncod = p.muncod 
				INNER JOIN territorios.municipio mu ON mu.muncod = a.muncod
				INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='M' and e.uncid='".$dados['uncid']."' 
				ORDER BY random() LIMIT 6";
	} else {
		$sql = "SELECT m.arqid, m.matid, m.mafdsc FROM sispacto2.materiaisfotos m 
				INNER JOIN sispacto2.materiais ma ON ma.matid = m.matid 
				INNER JOIN sispacto2.pactoidadecerta p ON p.picid = ma.picid 
				ORDER BY random() LIMIT 6";
	}
	$fotos = $db->carregar($sql);
	if($fotos) {
		foreach($fotos as $ft) {
			echo "<td><img id=".$ft['arqid']." onmouseover=\"return escape('".$ft['mafdsc']."');\" src=\"../slideshow/slideshow/verimagem.php?arqid=".$ft['arqid']."&newwidth=70&newheight=70\" class=\"imageBox_theImage\" onclick=\"javascript:window.open('../slideshow/slideshow/index.php?pagina=&amp;arqid=".$ft['arqid']."&amp;_sisarquivo=sispacto&amp;tabelacontrole=sispacto2.materiaisfotos&amp;getFiltro=true&amp;matid=".$ft['matid']."','imagem','width=850,height=600,resizable=yes');\"></td>";
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
	<td><b>(<?=(($recebeumaterialpacto['total'])?$recebeumaterialpacto['1']." / ".round(($recebeumaterialpacto['1']/$recebeumaterialpacto['total'])*100,1):"-") ?>%)</b> Sim, receberam o material fornecido pelo MEC</td>
	<td><b>(<?=(($recebeumaterialpacto['total'])?$recebeumaterialpacto['2']." / ".round(($recebeumaterialpacto['2']/$recebeumaterialpacto['total'])*100,1):"-") ?>%)</b> Sim, receberam uma cópia do material providenciada pelo município</td>
	<td nowrap><b>(<?=(($recebeumaterialpacto['total'])?$recebeumaterialpacto['3']." / ".round(($recebeumaterialpacto['3']/$recebeumaterialpacto['total'])*100,1):"-") ?>%)</b> Não</td>
</tr>

	<tr>
	<td><font size=1>A sua escola recebeu o material referente ao Programa Nacional do Livro Didático (PNLD)</font></td>
	<td><b>(<?=(($recebeumaterialpnld['total'])?$recebeumaterialpnld['1']." / ".round(($recebeumaterialpnld['1']/$recebeumaterialpnld['total'])*100,1):"-") ?>%)</b> Sim, recebemos o material integralmente</td>
	<td><b>(<?=(($recebeumaterialpnld['total'])?$recebeumaterialpnld['2']." / ".round(($recebeumaterialpnld['2']/$recebeumaterialpnld['total'])*100,1):"-") ?>%)</b> Sim, recebemos parte do material</td>
	<td nowrap><b>(<?=(($recebeumaterialpnld['total'])?$recebeumaterialpnld['3']." / ".round(($recebeumaterialpnld['3']/$recebeumaterialpnld['total'])*100,1):"-") ?>%)</b> Não</td>
	</tr>
	<tr>
	<td><font size=1>A sua escola recebeu os livros do PNLD - Obras Complementares específico para cada sala de aula de alfabetização?</font></td>
	<td><b>(<?=(($recebeulivrospnld['total'])?$recebeulivrospnld['1']." / ".round(($recebeulivrospnld['1']/$recebeulivrospnld['total'])*100,1):"-") ?>%)</b> Sim, recebemos o material integralmente</td>
	<td><b>(<?=(($recebeulivrospnld['total'])?$recebeulivrospnld['2']." / ".round(($recebeulivrospnld['2']/$recebeulivrospnld['total'])*100,1):"-") ?>%)</b> Sim, recebemos parte do material</td>
	<td nowrap><b>(<?=(($recebeulivrospnld['total'])?$recebeulivrospnld['3']." / ".round(($recebeulivrospnld['3']/$recebeulivrospnld['total'])*100,1):"-") ?>%)</b> Não</td>
	</tr>
	<tr>
	<td><font size=1>A turma da qual você é regente recebeu os livros do Programa Nacional Biblioteca da Escola (PNBE), específico para cada sala de aula de alfabetização?</font></td>
	<td><b>(<?=(($recebeumaterialpnbe['total'])?$recebeumaterialpnbe['1']." / ".round(($recebeumaterialpnbe['1']/$recebeumaterialpnbe['total'])*100,1):"-") ?>%)</b> Sim, recebemos o material integralmente</td>
	<td><b>(<?=(($recebeumaterialpnbe['total'])?$recebeumaterialpnbe['2']." / ".round(($recebeumaterialpnbe['2']/$recebeumaterialpnbe['total'])*100,1):"-") ?>%)</b> Sim, recebemos parte do material</td>
	<td nowrap><b>(<?=(($recebeumaterialpnbe['total'])?$recebeumaterialpnbe['3']." / ".round(($recebeumaterialpnbe['3']/$recebeumaterialpnbe['total'])*100,1):"-") ?>%)</b> Não</td>
	</tr>
	<tr>
	<td><font size=1>Na turma da qual você é regente, foi criado um cantinho de leitura em cada sala de aula de alfabetização com o material do PNBE?</font></td>
	<td><b>(<?=(($criadocantinholeitura['total'])?$criadocantinholeitura['1']." / ".round(($criadocantinholeitura['1']/$criadocantinholeitura['total'])*100,1):"-") ?>%)</b> Sim, criamos o cantinho de leitura</td>
	<td>&nbsp;</td>
	<td nowrap><b>(<?=(($criadocantinholeitura['total'])?$criadocantinholeitura['3']." / ".round(($criadocantinholeitura['3']/$criadocantinholeitura['total'])*100,1):"-") ?>%)</b> Não</td>
	</tr>
	<tr>
	<td colspan="4">
	<fieldset><legend>Fotos cantinho de leitura</legend>
	<div style="overflow:auto;width:500px;height:100px;">
	<?
	echo "<table>";
	echo "<tr>";
	
	$_SESSION['imgparams']['tabela'] = "sispacto2.materiaisprofessoresfotos";
	$_SESSION['imgparams']['filtro'] = "1=1";
	
	$sql = "SELECT m.arqid, m.mapid, m.mpfdsc FROM sispacto2.materiaisprofessoresfotos m 
			INNER JOIN sispacto2.materiaisprofessores ma ON ma.mapid = m.mapid 
			ORDER BY random() LIMIT 6";

	$fotos = $db->carregar($sql);
	if($fotos) {
		foreach($fotos as $ft) {
			echo "<td><img id=".$ft['arqid']." onmouseover=\"return escape('".$ft['mafdsc']."');\" src=\"../slideshow/slideshow/verimagem.php?arqid=".$ft['arqid']."&newwidth=70&newheight=70\" class=\"imageBox_theImage\" onclick=\"javascript:window.open('../slideshow/slideshow/index.php?pagina=&amp;arqid=".$ft['arqid']."&amp;_sisarquivo=sispacto&amp;getFiltro=true&amp;mapid=".$ft['mapid']."','imagem','width=850,height=600,resizable=yes');\"></td>";
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
				COALESCE(array_to_string(array(SELECT i.iusnome FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as coordenadorlocal,
				COALESCE(array_to_string(array(SELECT i.iusemailprincipal FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as emailcoordenadorlocal,
				COALESCE(array_to_string(array(SELECT '('||itedddtel||') '||itenumtel as tel FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd INNER JOIN sispacto2.identificacaotelefone it ON it.iusd = i.iusd AND it.itetipo='C' WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'-') as telefonecoordenadorlocal
				FROM sispacto2.materiais m 
				INNER JOIN sispacto2.pactoidadecerta p ON p.picid = m.picid 
				INNER JOIN territorios.municipio mu ON mu.estuf = p.estuf 
				INNER JOIN territorios.estado es ON es.estuf = p.estuf 
				INNER JOIN sispacto2.abrangencia a ON a.muncod = mu.muncod 
				INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='E' and e.uncid='".$dados['uncid']."' and {$dados['campo']}='{$dados['opcao']}'
				ORDER BY 1,2
				) UNION ALL (
				SELECT 
				'Municipal' as esfera,
				mu.estuf || ' / ' || mu.mundescricao as descricao,
				COALESCE(array_to_string(array(SELECT i.iusnome FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as coordenadorlocal,
				COALESCE(array_to_string(array(SELECT i.iusemailprincipal FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as emailcoordenadorlocal,
				COALESCE(array_to_string(array(SELECT '('||itedddtel||') '||itenumtel as tel FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd INNER JOIN sispacto2.identificacaotelefone it ON it.iusd = i.iusd AND it.itetipo='C' WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'-') as telefonecoordenadorlocal
				 
				
				FROM sispacto2.materiais m 
				INNER JOIN sispacto2.pactoidadecerta p ON p.picid = m.picid 
				INNER JOIN sispacto2.abrangencia a ON a.muncod = p.muncod 
				INNER JOIN territorios.municipio mu ON mu.muncod = a.muncod
				INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.esfera='M' and e.uncid='".$dados['uncid']."' and {$dados['campo']}='{$dados['opcao']}'
				ORDER BY 1,2
				)
				) foo";
		
		
	} else {
	
		$sql = "SELECT 
				CASE WHEN p.muncod IS NOT NULL THEN 'Municipal' ELSE 'Estadual' END as esfera,
				CASE WHEN p.muncod IS NOT NULL THEN mu.estuf || ' / ' || mu.mundescricao ELSE es.estuf || ' / ' || es.estdescricao END as descricao,
				COALESCE(array_to_string(array(SELECT i.iusnome FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as coordenadorlocal,
				COALESCE(array_to_string(array(SELECT i.iusemailprincipal FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'Coordenador Local não cadastrado') as emailcoordenadorlocal,
				COALESCE(array_to_string(array(SELECT '('||itedddtel||') '||itenumtel as tel FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd INNER JOIN sispacto2.identificacaotelefone it ON it.iusd = i.iusd AND it.itetipo='C' WHERE i.picid=p.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ', '),'-') as telefonecoordenadorlocal
				FROM sispacto2.materiais m 
				INNER JOIN sispacto2.pactoidadecerta p ON p.picid = m.picid 
				LEFT JOIN territorios.municipio mu ON mu.muncod = p.muncod 
				LEFT JOIN territorios.estado es ON es.estuf = p.estuf 
				WHERE {$dados['campo']}='{$dados['opcao']}' ORDER BY 1,2";
	}
	
	$cabecalho = array("Esfera","Descrição","Coordenador Local","Email","Telefone");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true);
	
}

function carregarInformes($dados) {
	global $db;
	
	$perfis = pegaPerfilGeral();
	if(!$perfis) $perfis = array();
	
	$editavel = false;
	if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis)) {
		$editavel = true;
	}
	
	echo '<p><b>Informes</b></p>';
	
	echo '<div style="background-color:white;height:150px;overflow:auto;">';
	
	$informes = $db->carregar("SELECT inpid, inpdescricao, to_char(inpdatainserida,'dd/mm/YYYY HH24:MI') as inpdatainserida FROM sispacto2.informespacto WHERE pflcoddestino='".$dados['pflcoddestino']."' AND inpstatus='A' ORDER BY inpdatainserida DESC");
	
	if($informes[0]) {
		foreach($informes as $inf) {
			echo " - ".$inf['inpdescricao']." ( <b>Inserida em ".$inf['inpdatainserida']."</b> )".(($editavel)?"<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirInforme('".$inf['inpid']."');\">":"")."<br>";
		}
	} else {
		echo " - Não existem informes cadastrados";
	}
	
	echo '</div>';
	
	if($editavel) {
		echo "<img src=\"../imagens/gif_inclui.gif\" style=\"cursor:pointer;\" onclick=\"inserirInforme('".$dados['pflcoddestino']."');\"> Inserir novo informe";
	}
	
	
}

function inserirInforme($dados) {
	global $db;
	
	$sql = "INSERT INTO sispacto2.informespacto(
            inpdescricao, inpdatainserida, usucpf, pflcoddestino, 
            inpstatus)
    		VALUES ('".utf8_decode($dados['inpdescricao'])."', 
					NOW(),
					'".$_SESSION['usucpf']."',
					'".$dados['pflcoddestino']."',
					'A');";
	
	$db->executar($sql);
	$db->commit();

}

function excluirInforme($dados) {
	global $db;
	
	$sql = "DELETE FROM sispacto2.informespacto WHERE inpid='".$dados['inpid']."'";
	$db->executar($sql);
	$db->commit();

}

function carregarHistoricoUsuario($dados) {
	global $db;
	
	$sql = "SELECT '<span style=font-size:x-small;>'||us.usunome||'</span>' as usunome, 
				   '<span style=font-size:x-small;>'||to_char(htudata,'dd/mm/YYYY HH24:MI')||'</span>' as data, 
				   '<span style=font-size:x-small;>'||hu.htudsc||'</span>' as motivo, 
				   '<span style=font-size:x-small;>'||
				   CASE WHEN hu.suscod='A' THEN 'Ativado'
						WHEN hu.suscod='B' THEN 'Bloqueado'
						WHEN hu.suscod='I' THEN 'Inativado'
						WHEN hu.suscod='P' THEN 'Pendente'
						ELSE 'Desconhecido' END||'</span>' as situacao, 
				   '<span style=font-size:x-small;>'||us2.usunome||'</span>' as resp 
			FROM seguranca.historicousuario hu 
			INNER JOIN seguranca.usuario us ON us.usucpf = hu.usucpf 
			LEFT JOIN seguranca.usuario us2 ON us2.usucpf = hu.usucpfadm
			WHERE hu.usucpf='".$dados['usucpf']."' AND hu.sisid='".SIS_SISPACTO."' ORDER BY htudata DESC";
	
	$cabecalho = array("Nome","Data","Justificativa","Situação","Responsável");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true);
	
}


function trocarTurmas($dados) {
	global $db;
	
	if($dados['troca']) {
		foreach($dados['troca'] as $iusd => $turid) {

			if($turid) {
				
				$existe = $db->pegaUm("SELECT otuid FROM {$dados['tabelacontrole']} WHERE iusd='".$iusd."'");
				
				if($existe) {

					$db->executar("UPDATE {$dados['tabelacontrole']} SET turid='".$turid."' WHERE iusd='".$iusd."'");
					
					$db->executar("INSERT INTO sispacto2.historicotrocausuario(
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

					$db->executar("INSERT INTO {$dados['tabelacontrole']}(turid, iusd, otustatus, otudata)
    							   VALUES ('".$turid."', '".$iusd."', 'A', NOW());");

					$db->executar("INSERT INTO sispacto2.historicotrocausuario(
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
	
	$sql = "UPDATE sispacto2.identificacaousuario SET iusemailprincipal='".$dados['iusemailprincipal']."' WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	$db->commit();
	
}

function exibirPorcentagemPagamentoPerfil($dados) {
	global $db;
	
	if($dados['uncid']) $wh[] = "i.uncid='".$dados['uncid']."'";
	
	$sql = "SELECT p.pflcod, p.pfldsc FROM sispacto2.pagamentoperfil pp 
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
			
			$uncids = $db->carregarColuna("SELECT DISTINCT uncid FROM sispacto2.tipoavaliacaoperfil WHERE fpbid <='".$dados['fpbid']."'");
				
			unset($f);
				
			if($uncids) {
				$f = "WHERE CASE WHEN foo.uncid IN('".implode("','",$uncids)."') THEN foo.numerobolsas < foo.plpmaximobolsas ELSE true END";
			}
			
			$sql = "SELECT count(*) as tot FROM (
					
					SELECT i.iusd, i.uncid,  t.pflcod, p.plpmaximobolsas, (SELECT count(*) FROM sispacto2.mensario WHERE iusd=i.iusd) as numerobolsas
					FROM sispacto2.identificacaousuario i 
					INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
					INNER JOIN sispacto2.pagamentoperfil p ON p.pflcod = t.pflcod 
					INNER JOIN sispacto2.folhapagamentouniversidade rf ON rf.uncid = i.uncid AND rf.pflcod = t.pflcod AND rf.fpbid='".$dados['fpbid']."'
					LEFT JOIN sispacto2.pactoidadecerta pa ON pa.picid = i.picid 
					LEFT JOIN workflow.documento dc ON dc.docid = pa.docidturma
					WHERE i.iusstatus='A' AND t.pflcod='".$p['pflcod']."' AND CASE WHEN t.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN i.iustipoprofessor='censo' AND dc.esdid='".ESD_FECHADO_TURMA."' ELSE true END AND CASE WHEN t.pflcod=".PFL_ORIENTADORESTUDO." THEN i.iusformacaoinicialorientador=true ELSE true END AND i.uncid IN( SELECT rf.uncid FROM sispacto2.folhapagamentouniversidade rf WHERE rf.fpbid='".$dados['fpbid']."' ) ".(($wh)?" AND ".implode(" AND ", $wh):"")."
				   		
					) foo 
				   	{$f}";
			
			$totalus = $db->pegaUm($sql);
			
			$sql = "SELECT count(*) as tot FROM sispacto2.pagamentobolsista p 
					INNER JOIN sispacto2.universidadecadastro i ON i.uniid = p.uniid 
					WHERE p.pflcod='".$p['pflcod']."' AND p.fpbid='".$dados['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sispacto2.folhapagamentouniversidade rf WHERE rf.fpbid='".$dados['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");
			
			$totalpag = $db->pegaUm($sql);
			
			
			$sql = "SELECT count(*) as tot FROM sispacto2.pagamentobolsista p 
					INNER JOIN sispacto2.universidadecadastro i ON i.uniid = p.uniid 
					INNER JOIN workflow.documento d ON d.docid = p.docid 
					WHERE d.esdid IN('".ESD_PAGAMENTO_EFETIVADO."','".ESD_PAGAMENTO_NAO_AUTORIZADO."') AND p.pflcod='".$p['pflcod']."' AND p.fpbid='".$dados['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sispacto2.folhapagamentouniversidade rf WHERE rf.fpbid='".$dados['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");
			
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
	
	$sql = "SELECT f.fpbid, 'Ref.'||m.mesdsc||'/'||f.fpbanoreferencia as periodo FROM sispacto2.folhapagamento f
			INNER JOIN public.meses m ON m.mescod::numeric = f.fpbmesreferencia 
			".(($dados['uncid'])?"INNER JOIN sispacto2.folhapagamentouniversidade fp ON fp.fpbid = f.fpbid AND fp.uncid='".$dados['uncid']."' AND fp.pflcod IS NULL":"")."
			".(($wh1)?" WHERE ".implode(" AND ", $wh1):"");
	
	$folhapagamento = $db->carregar($sql);
	
	echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
	if($folhapagamento[0]) {
		
		echo '<tr>';
		echo '<td class="SubTituloCentro">&nbsp;</td>';
		echo '<td class="SubTituloCentro">Período de Referência</td>';
		echo '<td class="SubTituloCentro">Total bolsistas</td>';
		echo '<td class="SubTituloCentro" colspan=2>Total em pagamento</td>';
		echo '<td class="SubTituloCentro" colspan=2>Total concluído</td>';
		echo '<td class="SubTituloCentro">Restante</td>';
		echo '</tr>';
		
		foreach($folhapagamento as $fp) {
			echo '<tr>';
			
			$sql = "SELECT count(*) as tot, sum(foo.plpvalor) as vlr FROM (
 			
 					SELECT i.iusd, i.uncid,  t.pflcod, p.plpmaximobolsas, (SELECT count(*) FROM sispacto2.mensario WHERE iusd=i.iusd) as numerobolsas, p.plpvalor 
 					FROM sispacto2.identificacaousuario i 
					INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
					INNER JOIN sispacto2.pagamentoperfil p ON p.pflcod = t.pflcod 
					INNER JOIN sispacto2.folhapagamentouniversidade rf ON rf.uncid = i.uncid AND rf.pflcod = t.pflcod AND rf.fpbid='".$fp['fpbid']."'
					LEFT JOIN sispacto2.pactoidadecerta pa ON pa.picid = i.picid 
					LEFT JOIN workflow.documento dc ON dc.docid = pa.docidturma
					WHERE i.iusstatus='A' AND CASE WHEN t.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN i.iustipoprofessor='censo' AND dc.esdid='".ESD_FECHADO_TURMA."' ELSE true END AND CASE WHEN t.pflcod=".PFL_ORIENTADORESTUDO." THEN i.iusformacaoinicialorientador=true ELSE true END ".(($wh)?" AND ".implode(" AND ", $wh):"")."
 					
 					) foo 
					{$f}";
			$totalinsc = $db->pegaLinha($sql);
			$totalus = $totalinsc['tot'];
					
			
			$sql = "SELECT count(*) as tot FROM sispacto2.pagamentobolsista p 
					INNER JOIN sispacto2.universidadecadastro i ON i.uniid = p.uniid 
					WHERE p.fpbid='".$fp['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sispacto2.folhapagamentouniversidade rf WHERE rf.fpbid='".$fp['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");
			
			$totalpag = $db->pegaUm($sql);
			
			$sql = "SELECT count(*) as tot, sum(p.pbovlrpagamento) as vlr FROM sispacto2.pagamentobolsista p 
					INNER JOIN sispacto2.universidadecadastro i ON i.uniid = p.uniid 
					INNER JOIN workflow.documento d ON d.docid = p.docid 
					WHERE d.esdid IN('".ESD_PAGAMENTO_EFETIVADO."','".ESD_PAGAMENTO_NAO_AUTORIZADO."') AND p.fpbid='".$fp['fpbid']."' AND i.uncid IN( SELECT rf.uncid FROM sispacto2.folhapagamentouniversidade rf WHERE rf.fpbid='".$fp['fpbid']."' )".(($wh)?" AND ".implode(" AND ", $wh):"");
			
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
		echo '<td class="SubTituloDireita">Esfera:</td>';
		echo '<td>';
		$sql = "(
				SELECT p.picid as codigo,
					   'Municipal : '|| m.estuf || ' / ' || m.mundescricao AS descricao
				FROM sispacto2.pactoidadecerta p 
				INNER JOIN sispacto2.abrangencia a ON a.muncod = p.muncod
				INNER JOIN sispacto2.estruturacurso es ON es.ecuid = a.ecuid 
				INNER JOIN territorios.municipio m ON m.muncod = p.muncod 
				WHERE es.uncid='".$dados['uncid']."'
				ORDER BY 2
				) UNION ALL (
				SELECT  DISTINCT p.picid as codigo,
					   'Estadual : '|| e.estuf || ' / ' || e.estdescricao AS descricao
				FROM sispacto2.pactoidadecerta p 
				INNER JOIN territorios.municipio m ON m.estuf = p.estuf
				INNER JOIN sispacto2.abrangencia a ON a.muncod = m.muncod
				INNER JOIN sispacto2.estruturacurso es ON es.ecuid = a.ecuid 
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
				FROM sispacto2.turmas p 
				INNER JOIN sispacto2.identificacaousuario i ON i.iusd = p.iusd 
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_FORMADORIES."
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
		echo '<td class="SubTituloDireita">Esfera:</td>';
		echo '<td>';
		$sql = "(
				SELECT p.picid as codigo,
					   'Municipal : '|| m.estuf || ' / ' || m.mundescricao AS descricao
				FROM sispacto2.pactoidadecerta p 
				INNER JOIN sispacto2.abrangencia a ON a.muncod = p.muncod
				INNER JOIN sispacto2.estruturacurso es ON es.ecuid = a.ecuid 
				INNER JOIN territorios.municipio m ON m.muncod = p.muncod 
				WHERE es.uncid='".$dados['uncid']."'
				ORDER BY 2
				) UNION ALL (
				SELECT  DISTINCT p.picid as codigo,
					   'Estadual : '|| e.estuf || ' / ' || e.estdescricao AS descricao
				FROM sispacto2.pactoidadecerta p 
				INNER JOIN territorios.municipio m ON m.estuf = p.estuf
				INNER JOIN sispacto2.abrangencia a ON a.muncod = m.muncod
				INNER JOIN sispacto2.estruturacurso es ON es.ecuid = a.ecuid 
				INNER JOIN territorios.estado e ON e.estuf = p.estuf 
				WHERE es.uncid='".$dados['uncid']."'
				ORDER BY 2
				
				)";
		$db->monta_combo('picid__', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'picid__','', $dados['picid__']);
		echo '</td>';
	
		echo '</tr>';
		
		echo '<tr>';
		echo '<td class="SubTituloDireita">Turma:</td>';
		echo '<td>';
		$sql = "SELECT turid as codigo,
					   i.iusnome ||' ( '||turdesc||' )' AS descricao
				FROM sispacto2.turmas p 
				INNER JOIN sispacto2.identificacaousuario i ON i.iusd = p.iusd 
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_ORIENTADORESTUDO."
				WHERE i.uncid='".$dados['uncid']."'
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
		echo '<td class="SubTituloDireita">Esfera:</td>';
		echo '<td>';
		$sql = "(
				SELECT p.picid as codigo,
					   'Municipal : '|| m.estuf || ' / ' || m.mundescricao AS descricao
				FROM sispacto2.pactoidadecerta p 
				INNER JOIN sispacto2.abrangencia a ON a.muncod = p.muncod AND a.esfera='M'
				INNER JOIN sispacto2.estruturacurso es ON es.ecuid = a.ecuid 
				INNER JOIN territorios.municipio m ON m.muncod = p.muncod 
				WHERE es.uncid='".$dados['uncid']."'
				ORDER BY 2
				) UNION ALL (
				SELECT  DISTINCT p.picid as codigo,
					   'Estadual : '|| e.estuf || ' / ' || e.estdescricao AS descricao
				FROM sispacto2.pactoidadecerta p 
				INNER JOIN territorios.municipio m ON m.estuf = p.estuf
				INNER JOIN sispacto2.abrangencia a ON a.muncod = m.muncod AND a.esfera='E'
				INNER JOIN sispacto2.estruturacurso es ON es.ecuid = a.ecuid 
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
				FROM sispacto2.folhapagamento f 
				INNER JOIN sispacto2.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
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
				FROM sispacto2.folhapagamento f 
				INNER JOIN sispacto2.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
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
				FROM sispacto2.folhapagamento f 
				INNER JOIN sispacto2.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
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
			    COALESCE(array_to_string(array(SELECT iusnome FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=pic.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'Coordenador Local não cadastrado') as coordenadorlocal, 
			    COALESCE(array_to_string(array(SELECT iusemailprincipal FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=pic.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'Coordenador Local não cadastrado') as emailcoordenador
			FROM sispacto2.abrangencia a 
			INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sispacto2.pactoidadecerta pic ON pic.muncod = a.muncod 
			LEFT JOIN workflow.documento d ON d.docid = pic.docidturma 
			WHERE e.uncid='".$dados['uncid']."' AND a.esfera='M' AND (d.esdid!='".ESD_FECHADO_TURMA."' OR d.esdid IS NULL) ORDER BY 1,2";
	
	$cabecalho = array("UF","Município","Coordenador Local","E-mail");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true,false,false,true);
	
	
}



function efetuarInsercaoUsuarioPerfil($dados) {
	global $db;
	if($dados['picid__']) $dados['muncodatuacao__'] = $db->pegaUm("SELECT muncod FROM sispacto2.pactoidadecerta WHERE picid='".$dados['picid__']."'");
	
	
	$iusd = $db->pegaUm("SELECT iusd FROM sispacto2.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array("",""),$dados['iuscpf__'])."'");
	
	if($iusd) {
		
		$sql = "UPDATE sispacto2.identificacaousuario SET 
				picid   =".(($dados['picid__'])?"'".$dados['picid__']."'":"NULL").", 
	    		iuscpf  =".(($dados['iuscpf__'])?"'".str_replace(array(".","-"),array("",""),$dados['iuscpf__'])."'":"NULL").", 
	    		iusnome =".(($dados['iusnome__'])?"'".$dados['iusnome__']."'":"NULL").", 
	    		iusemailprincipal =".(($dados['iusemailprincipal__'])?"'".$dados['iusemailprincipal__']."'":"NULL").", 
	    		iusstatus ='A', 
	    		iustipoorientador =".(($dados['iustipoorientador__'])?"'".$dados['iustipoorientador__']."'":"NULL").",
	    		muncodatuacao =".(($dados['muncodatuacao__'])?"'".$dados['muncodatuacao__']."'":"NULL").", 
	            uncid =".(($dados['uncid'])?"'".$dados['uncid']."'":"NULL").", 
	            iusformacaoinicialorientador =".(($dados['iusformacaoinicialorientador__'])?$dados['iusformacaoinicialorientador__']:"NULL").", 
	            iustipoprofessor =".(($dados['iustipoprofessor__'])?"'".$dados['iustipoprofessor__']."'":"NULL")."
				WHERE iusd='".$iusd."'";
		
		$db->executar($sql);
		
	} else {
	
		$sql = "INSERT INTO sispacto2.identificacaousuario(
	            picid, iuscpf, iusnome, 
	            iusemailprincipal, iusdatainclusao,  
	            iusstatus, iustipoorientador, 
	            muncodatuacao, uncid,  
	            iusformacaoinicialorientador, iustipoprofessor)
	    VALUES (".(($dados['picid__'])?"'".$dados['picid__']."'":"NULL").", 
	    		".(($dados['iuscpf__'])?"'".str_replace(array(".","-"),array("",""),$dados['iuscpf__'])."'":"NULL").", 
	    		".(($dados['iusnome__'])?"'".$dados['iusnome__']."'":"NULL").", 
	    		".(($dados['iusemailprincipal__'])?"'".$dados['iusemailprincipal__']."'":"NULL").", 
	    		NOW(), 
	    		'A', 
	    		".(($dados['iustipoorientador__'])?"'".$dados['iustipoorientador__']."'":"NULL").",
	    		".(($dados['muncodatuacao__'])?"'".$dados['muncodatuacao__']."'":"NULL").", 
	            ".(($dados['uncid'])?"'".$dados['uncid']."'":"NULL").", 
	            ".(($dados['iusformacaoinicialorientador__'])?$dados['iusformacaoinicialorientador__']:"NULL").", 
	            ".(($dados['iustipoprofessor__'])?"'".$dados['iustipoprofessor__']."'":"NULL").") RETURNING iusd";
		
		$iusd = $db->pegaUm($sql);
	
	}
	
	$sql = "SELECT p.pfldsc, p.pflcod FROM sispacto2.tipoperfil t INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod WHERE t.iusd='".$iusd."'";
	$arrPf = $db->pegaLinha($sql);
	
	if($arrPf['pfldsc'] && $arrPf['pflcod']!=$dados['pflcod__']) {
		$al = array("alert"=>"Inserção não efetivada com sucesso. O usuário ja esta cadastrado com o perfil : ".$arrPf['pfldsc'],"location"=>$_SERVER['REQUEST_URI']);
		alertlocation($al);
	}
	
	$tpeid = $db->pegaUm("SELECT tpeid FROM sispacto2.tipoperfil WHERE iusd='".$iusd."'");
	
	if(!$tpeid) {
		$sql = "INSERT INTO sispacto2.tipoperfil(
	            iusd, pflcod, fpbidini, fpbidfim)
	    		VALUES ('".$iusd."', '".$dados['pflcod__']."', ".(($dados['fpbidini'])?"'".$dados['fpbidini']."'":"NULL").", ".(($dados['fpbidfim'])?"'".$dados['fpbidfim']."'":"NULL").");";
		
		$db->executar($sql);
	}
	
	if($dados['turid__']) {
		$otuid = $db->pegaUm("SELECT otuid FROM sispacto2.orientadorturma WHERE iusd='".$iusd."'");
		
		if(!$otuid) {
			$sql = "INSERT INTO sispacto2.orientadorturma(
	            	turid, iusd)
	    			VALUES ('".$dados['turid__']."', '".$iusd."');";
	
			$db->executar($sql);
		} else {
			
			$sql = "UPDATE sispacto2.orientadorturma SET turid='".$dados['turid__']."' WHERE iusd='".$iusd."';";
			$db->executar($sql);
			
		}
	}
	
	$db->executar("INSERT INTO sispacto2.historicotrocausuario(
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
			(SELECT itedddtel FROM sispacto2.identificacaotelefone WHERE iusd=i.iusd AND itetipo='T') as dddtel,
			u.usufoneddd,
			(SELECT itenumtel FROM sispacto2.identificacaotelefone WHERE iusd=i.iusd AND itetipo='T') as tel,
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
			p.pfldsc || ' - SISPACTO' as funcao_pacto,
			u.ususexo,
			i.iussexo,
			split_part(i.iusnome, ' ', 1) as apelido_pacto,
			u.usunomeguerra as apelido_segur
			FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.usuario u ON u.usucpf = i.iuscpf 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
			INNER JOIN territorios.municipio m ON m.muncod = i.muncodatuacao 
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
												   FROM sispacto2.pactoidadecerta p 
						    		  		   	   INNER JOIN workflow.documento d ON d.docid = p.docid 
						    		  		   	   LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
						    		  		   	   LEFT JOIN territorios.estado e ON e.estuf = p.estuf 
						    		  		   	   WHERE p.picid='".$dados['picid']."'");
	
	if($p) {
		
		$db->executar("UPDATE sispacto2.pactoidadecerta SET picselecaopublica=true, picincluirprofessorrede=false WHERE picid='".$p['picid']."'");
		$db->commit();
			
		$ar = array("estuf" 	  => $p['estuf'],
					"muncod" 	  => $p['muncod'],
					"dependencia" => (($p['muncod'])?'municipal':'estadual'));
		
		$totalalfabetizadores = carregarTotalAlfabetizadores($ar);
		
		$orientadoresestudo = carregarDadosIdentificacaoUsuario(array("picid"=>$p['picid'],"pflcod"=>PFL_ORIENTADORESTUDO));
		
		if($totalalfabetizadores['total_orientadores_a_serem_cadastrados'] > count($orientadoresestudo)) {
			$restantes = ($totalalfabetizadores['total_orientadores_a_serem_cadastrados']-count($orientadoresestudo));
			for($i = 0;$i < $restantes;$i++) {
				
				$num_ius = $db->pegaUm("SELECT substr(iuscpf, 8) as num FROM sispacto2.identificacaousuario WHERE picid='".$p['picid']."' AND iuscpf ilike 'SIS%' ORDER BY iusd DESC");
				if($num_ius) $num_ius++;
				else $num_ius=1;
				
				$iuscpf  		   = "SIS".str_pad($p['picid'], 4, "0", STR_PAD_LEFT).str_pad($num_ius, 4, "0", STR_PAD_LEFT);
				$iusnome 		   = "Orientador de Estudo - ".str_replace("'"," ",$p['descricao'])." - ".str_pad($num_ius, 4, "0", STR_PAD_LEFT);
				$iusemailprincipal = "noemail@noemail.com";
				
				if($p['muncod']) {
					$uncid = $db->pegaUm("SELECT e.uncid FROM sispacto2.abrangencia a 
										  INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
										  WHERE a.muncod='".$p['muncod']."' AND esfera='M'");
				} elseif($p['estuf']) {
					$uncid = $db->pegaUm("SELECT e.uncid FROM sispacto2.abrangencia a 
										  INNER JOIN territorios.municipio m ON m.muncod = a.muncod
										  INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
										  WHERE m.estuf='".$p['estuf']."' AND esfera='E'");
					
				}
				
				$sql = "INSERT INTO sispacto2.identificacaousuario(picid, 
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
				
				$sql = "INSERT INTO sispacto2.tipoperfil( iusd, pflcod, tpestatus)
    					VALUES ( '".$iusd."', '".PFL_ORIENTADORESTUDO."', 'A');";
				
				$db->executar($sql);
				
				if($uncid) {
					$turid = $db->pegaUm("SELECT t.turid FROM sispacto2.turmas t 
										  INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd 
										  INNER JOIN sispacto2.tipoperfil tt ON tt.iusd = i.iusd 
										  WHERE tt.pflcod='".PFL_FORMADORIES."' AND i.uncid='".$uncid."' LIMIT 1");
					
					if($turid) {
						$db->executar("INSERT INTO sispacto2.orientadorturma(
									            turid, iusd, otustatus, otudata)
									    VALUES ('".$turid."', '".$iusd."', 'A', NOW());");
					}
				}
				
				
			}
			
			$db->commit();
		} else {
			$al = array("alert"=>"O município selecionado não possui vagas para Orientadores de Estudo.","location"=>"sispacto2.php?modulo=principal/mec/mec&acao=A");
			alertlocation($al);
		}
	
	}
	
	$al = array("alert"=>"Foram inseridos {$restantes} Orientadores de Estudo SIS.","location"=>"sispacto2.php?modulo=principal/mec/mec&acao=A");
	alertlocation($al);
	
	
}

function invalidarMensario($dados) {
	global $db;
	$sql = "SELECT d.esdid FROM sispacto2.mensario m 
			INNER JOIN workflow.documento d ON d.docid = m.docid 
			WHERE m.docid='".$dados['docidmensario']."'";
	$esdidorigem = $db->pegaUm($sql);
	
	$sql = "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem='".$esdidorigem."' AND esdiddestino='".ESD_INVALIDADO_MENSARIO."' AND aedstatus='A'";
	$aedid = $db->pegaUm($sql);
	
	if($aedid) {
		wf_alterarEstado( $dados['docidmensario'], $aedid, $dados['cmddsc'], array());
	}

	$al = array("alert"=>"Mensário invalidado com sucesso","location"=>"sispacto2.php?modulo={$dados['modulo']}&acao=A&aba=aprovarusuario&fpbid=".$dados['fpbid']."&pflcodaprovar=".$dados['pflcodaprovar']);
	alertlocation($al);
	
}

function corrigirAcessoUniversidade($dados) {
	global $db;
	$sql = "SELECT i.uncid, i.iuscpf, i.picid, t.pflcod, i.muncodatuacao, i.iusd FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = t.iusd
			WHERE iuscpf='".$_SESSION['usucpf']."'";
	
	$identificacaousuario = $db->pegaLinha($sql);
	
	if($identificacaousuario['uncid']) {
		
		$sql = "UPDATE sispacto2.usuarioresponsabilidade SET uncid='".$identificacaousuario['uncid']."' WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".$identificacaousuario['pflcod']."' AND rpustatus='A'";
		$db->executar($sql);
		$db->commit();

		if($dados['sis']) $_SESSION['sispacto2'][$dados['sis']]['uncid'] = $identificacaousuario['uncid'];
		
	} elseif($identificacaousuario['picid']) {
		
		$sql = "SELECT * FROM sispacto2.pactoidadecerta WHERE picid=".$identificacaousuario['picid'];
		$pactoidadecerta = $db->pegaLinha($sql);
		
		if($pactoidadecerta['estuf'] && $identificacaousuario['muncodatuacao']) {
			$esfera = "E";
			$muncod = $identificacaousuario['muncodatuacao'];
		}
		
		if($pactoidadecerta['muncod']) {
			$esfera = "M";
			$muncod = $pactoidadecerta['muncod'];
		}
		
		$sql = "SELECT uncid FROM sispacto2.abrangencia a 
				INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
				WHERE a.muncod='".$muncod."' AND a.esfera='".$esfera."'";
		
		$uncid = $db->pegaUm($sql);
		
		if($uncid) {
			$db->executar("UPDATE sispacto2.identificacaousuario SET uncid='".$uncid."' WHERE iusd='".$identificacaousuario['iusd']."'");
			$db->executar("UPDATE sispacto2.usuarioresponsabilidade SET uncid='".$uncid."' WHERE usucpf='".$_SESSION['usucpf']."' AND pflcod='".$identificacaousuario['pflcod']."' AND rpustatus='A'");
			$db->commit();
		}
		
		if($dados['sis']) $_SESSION['sispacto2'][$dados['sis']]['uncid'] = $uncid;
		
	}
}

function criarDocumentosPagamentos($dados) {
	global $db;
	
	$pagamentos = $db->carregar("SELECT p.pboid, pf.pfldsc, i.iuscpf, i.iusnome, f.fpbmesreferencia, f.fpbanoreferencia FROM sispacto2.pagamentobolsista p 
								 INNER JOIN seguranca.perfil pf ON pf.pflcod = p.pflcod 
								 INNER JOIN sispacto2.identificacaousuario i ON i.iusd = p.iusd 
								 INNER JOIN sispacto2.folhapagamento f ON f.fpbid = p.fpbid 
								 WHERE docid IS NULL");
	
	if($pagamentos[0]) {
		foreach($pagamentos as $arrInfo) {
			$docid = wf_cadastrarDocumento(TPD_PAGAMENTOBOLSA, "Pagamento - ".$arrInfo['pfldsc']." - (".$arrInfo['iuscpf'].")".$arrInfo['iusnome']." - ".$arrInfo['fpbmesreferencia']."/".$arrInfo['fpbanoreferencia']);
			$db->executar("UPDATE sispacto2.pagamentobolsista SET docid='".$docid."' WHERE pboid='".$arrInfo['pboid']."'");
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
		$db->executar("UPDATE sispacto2.identificacaousuario SET iusnome='".$obj['PESSOA']->no_pessoa_rf."' WHERE iuscpf='".$dados['iuscpf']."'");
		$db->executar("UPDATE seguranca.usuario SET usunome='".$obj['PESSOA']->no_pessoa_rf."' WHERE usucpf='".$dados['iuscpf']."'");
		$db->commit();
	}
	
	$al = array("alert"=>"Nome atualizado com sucesso","location"=>"sispacto2.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=dados");
	alertlocation($al);
	
	
	
}

function aprovarTrocaNomesSGB($dados) {
	global $db;
	
	if($dados['cpf']) {
		foreach($dados['cpf'] as $cpf) {
			if($dados['nome_receita'][$cpf]) {
				$sql = "UPDATE sispacto2.identificacaousuario SET iusnome='".$dados['nome_receita'][$cpf]."' WHERE iuscpf='".str_replace(array(".","-"),array("",""),$cpf)."'";
				$db->executar($sql);
			}	
		}
		$db->commit();
	}
	
	$al = array("alert"=>"Trocas realizadas com sucesso","location"=>"sispacto2.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=aprovarnomes");
	alertlocation($al);
	
}

function pegarRestricaoPagamento($dados) {
	global $db;
	
	$sql = "SELECT 			   CASE WHEN foo.mensarionota < 7		       THEN '<span style=color:red;font-size:x-small;>A avaliação do usuário não atingiu a nota mínima de 7(sete)</span>'
			WHEN foo.iustermocompromisso        =false     THEN '<span style=color:redfont-size:x-small;;>Bolsista não preencheu o termo de compromisso</span>'
			WHEN foo.iusnaodesejosubstituirbolsa=true      THEN '<span style=color:red;font-size:x-small;>Bolsista do FNDE/MEC e não deseja substituir bolsa atual pela bolsa do PACTO</span>' 
			WHEN foo.fpbidini IS NOT NULL AND foo.fpbidfim IS NOT NULL AND (foo.fpbid < foo.fpbidini OR foo.fpbid > foo.fpbidfim) THEN '<span style=color:red;font-size:x-small;>Este período de referência não esta habilitado para pagamento</span>'
		    WHEN foo.pflcod=".PFL_ORIENTADORESTUDO." THEN 
			                                             CASE  WHEN foo.iusdocumento=false   THEN '<center><span style=color:red;font-size:x-small;>Possui problemas na documentação</span></center>'
						   									   WHEN foo.numeroavaliacoes < 2 THEN '<span style=color:red;font-size:x-small;>As avaliações do Formador IES e Coordenador Local são obrigatórias</span>' 
						   									   WHEN foo.numeroausencia > 0 THEN '<span style=color:red;font-size:x-small;>Ausência na Universidade e/ou Município</span>'
						   								ELSE '<span style=color:blue;font-size:x-small;>Nenhuma restrição - Aguardando aprovação do Coordenador Geral/Adjunto</span>' 
						   								END
			WHEN foo.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN 
			   														CASE WHEN foo.totalturmas = 0 AND foo.rfuparcela > 0 THEN '<span style=color:red;font-size:x-small;font-size:x-small;>Preencher a aba Atividades Obrigatórias (1º Parcela)</span>'
	    																 WHEN foo.aprendizagemMat != ".APRENDIZAGEM_MATEMATICA." AND foo.rfuparcela > 1 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (2º Parcela)</span>'
	            														 WHEN foo.aprendizagemPor != ".APRENDIZAGEM_PORTUGUES." AND foo.rfuparcela > 2 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (3º Parcela)</span>'
    		 			            									 WHEN foo.totalmateriaisprofessores = 0 AND foo.rfuparcela > 3 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (4º Parcela)</span>' 
																		 WHEN foo.aprendizagemUsoMateriaisDidaticos != ".APRENDIZAGEM_MATERIALDIDATICO." AND foo.rfuparcela > 4 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (5º Parcela)</span>'
																		 WHEN foo.relatoexperiencia = 0 AND foo.rfuparcela > 5 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (6º Parcela)</span>'
																		 WHEN foo.impressoesana != foo.totalturmas AND foo.rfuparcela > 6 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (7º Parcela)</span>'
																		 WHEN foo.questoesdiversasatv8 = 0 AND foo.rfuparcela > 7 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (8º Parcela)</span>' 
	    																 WHEN foo.aprendizagemMat2 != ".APRENDIZAGEM_MATEMATICA." AND foo.rfuparcela > 8 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (9º Parcela) - Matemática</span>'
	            														 WHEN foo.aprendizagemPor2 != ".APRENDIZAGEM_PORTUGUES." AND foo.rfuparcela > 8 THEN '<span style=color:red;font-size:x-small;>Preencher a aba Atividades Obrigatórias (9º Parcela) - Português</span>'
						   												 WHEN foo.iustipoprofessor = 'censo' THEN '<span style=color:blue;font-size:x-small;>Nenhuma restrição</span>' 
					   												 ELSE '<span style=color:red;font-size:x-small;>Professor Alfabetizador não cadastrado no censo 2013</span>' END 
			--WHEN foo.pflcod=".PFL_COORDENADORLOCAL." THEN 
			--		   										CASE WHEN foo.rcoordenadorlocal = 0 THEN '<span style=color:red;font-size:x-small;>Falta preencher as informações sobre \"Gestão e Mobilização\" no SisPacto</span>' 
			--		   										ELSE '<span style=color:blue;font-size:x-small;>Nenhuma restrição - Aguardando aprovação do Coordenador Geral/Adjunto</span>' END
			ELSE '<span style=color:blue;font-size:x-small;>Nenhuma restrição - Aguardando aprovação do Coordenador Geral/Adjunto</span>' END as restricao
		FROM (
		SELECT  m.menid,
				i.iustermocompromisso, 
				m.fpbid,
				p.pflcod,
				i.iusdocumento,
				i.iustipoprofessor,
				i.reuid,
				i.iusnaodesejosubstituirbolsa,
				(SELECT COUNT(DISTINCT pflcodavaliador) FROM sispacto2.mensarioavaliacoes ma  WHERE ma.menid=m.menid) as numeroavaliacoes,
				COALESCE((SELECT AVG(mavtotal) FROM sispacto2.mensarioavaliacoes ma  WHERE ma.menid=m.menid),0.00) as mensarionota,
				(SELECT COUNT(mapid) FROM sispacto2.materiaisprofessores mp WHERE mp.iusd=m.iusd) as totalmateriaisprofessores,
				(SELECT COUNT(*) FROM sispacto2.turmasprofessoresalfabetizadores pa WHERE tpastatus='A' AND (coalesce(tpatotalmeninos,0)+coalesce(tpatotalmeninas,0))!=0 AND pa.iusd=m.iusd) as totalturmas,
				(SELECT COUNT(mavid) FROM sispacto2.mensarioavaliacoes ma  WHERE ma.menid=m.menid AND ma.mavfrequencia=0) as numeroausencia,
				(SELECT COUNT(*) FROM sispacto2.gestaomobilizacaoperguntas gm WHERE gm.iusd=m.iusd) as rcoordenadorlocal,
					
				(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens 
				 FROM sispacto2.aprendizagemconhecimentoturma a 
				 INNER JOIN sispacto2.aprendizagemconhecimento c ON c.catid = a.catid
				 INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
				 WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='M' AND t.iusd=m.iusd) as aprendizagemMat,
					
				(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens 
				 FROM sispacto2.aprendizagemconhecimentoturma2 a 
				 INNER JOIN sispacto2.aprendizagemconhecimento c ON c.catid = a.catid
				 INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
				 WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='M' AND t.iusd=m.iusd) as aprendizagemMat2,
					
				(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens 
				 FROM sispacto2.aprendizagemconhecimentoturma a 
				 INNER JOIN sispacto2.aprendizagemconhecimento c ON c.catid = a.catid
				 INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
				 WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='P' AND t.iusd=m.iusd) as aprendizagemPor,

				(SELECT CASE WHEN count(DISTINCT a.tpaid) > 0 THEN count(*)/count(DISTINCT a.tpaid) ELSE 0 END as itens 
				 FROM sispacto2.aprendizagemconhecimentoturma2 a 
				 INNER JOIN sispacto2.aprendizagemconhecimento c ON c.catid = a.catid
				 INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = a.tpaid 
				 WHERE t.tpastatus='A' AND tpaconfirmaregencia=true AND c.cattipo='P' AND t.iusd=m.iusd) as aprendizagemPor2, 
					
					
				(SELECT count(*) as itens FROM sispacto2.usomateriaisdidaticos WHERE iusd=m.iusd) as aprendizagemUsoMateriaisDidaticos,
				(SELECT count(*) as itens FROM sispacto2.relatoexperiencia WHERE iusd=m.iusd) as relatoexperiencia,
					
				(SELECT count(*) as itens FROM sispacto2.impressoesana i INNER JOIN sispacto2.turmasprofessoresalfabetizadores t ON t.tpaid = i.tpaid AND i.iusd = t.iusd WHERE i.iusd=m.iusd) as impressoesana,
				(SELECT count(*) as itens FROM sispacto2.questoesdiversasatv8 WHERE iusd=m.iusd) as questoesdiversasatv8,
				(SELECT count(*) as itens FROM sispacto2.contribuicaopacto WHERE iusd=m.iusd) as contribuicaopacto,
					
    			(SELECT COUNT(DISTINCT fpbid) FROM sispacto2.pagamentobolsista pg WHERE pg.iusd=m.iusd) as qtduspagamento,
				(SELECT COUNT(DISTINCT fpbid) FROM sispacto2.pagamentobolsista pg WHERE pg.tpeid=t.tpeid) as qtdtppagamento,
				
				t.fpbidini,
				t.fpbidfim,
    			pp.plpmaximobolsas,
				fpu.rfuparcela,
				dorien.esdid as esdorien,
				dturpr.esdid as esdturpr
		FROM sispacto2.mensario m 
		INNER JOIN sispacto2.identificacaousuario i ON i.iusd = m.iusd 
		INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
		INNER JOIN sispacto2.folhapagamentouniversidade fpu ON fpu.uncid = i.uncid AND fpu.pflcod = t.pflcod AND fpu.fpbid = m.fpbid
		INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
    	LEFT JOIN sispacto2.pagamentoperfil pp ON pp.pflcod = p.pflcod 
		LEFT JOIN sispacto2.pactoidadecerta pic ON pic.picid = i.picid 
		LEFT JOIN workflow.documento dorien ON dorien.docid = pic.docid 
		LEFT JOIN workflow.documento dturpr ON dturpr.docid = pic.docidturma 
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
				$db->executar("UPDATE sispacto2.pagamentobolsista SET remid=null WHERE docid='".$docid."'");
				$db->commit();
			}
			
		}
	}
	
	$al = array("alert"=>"Reenvio agendado com sucesso","location"=>"sispacto2.php?modulo=principal/mec/mec&acao=A&aba=reenviarpagamentos");
	alertlocation($al);
	
	
}

function excluirAvaliacoesMensario($dados) {
	global $db;
	
	$db->executar("DELETE FROM sispacto2.historicoreaberturanota WHERE mavid='".$dados['mavid']."'");
	$db->executar("DELETE FROM sispacto2.mensarioavaliacoes WHERE mavid='".$dados['mavid']."'");
	
	$db->commit();
	
	$al = array("alert"=>"Avaliação apagada","location"=>"sispacto2.php?modulo=".$dados['modulo']."&acao=A&aba=".$dados['aba']."&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
	
	
}

function exibirMunicipiosAtuacao($dados) {
	global $db;
	
	$identificacaousuario = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf, i.muncodatuacao, p.estuf 
											FROM sispacto2.identificacaousuario i 
											INNER JOIN sispacto2.pactoidadecerta p ON p.picid = i.picid
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
			INNER JOIN sispacto2.pactoidadecerta p ON p.estuf = m.estuf 
			INNER JOIN sispacto2.identificacaousuario i ON i.picid = p.picid 
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
	
	$db->executar("UPDATE sispacto2.identificacaousuario SET muncodatuacao='".$dados['muncod']."' WHERE iusd='".$dados['iusd']."'");
	$db->commit();
	
}

function carregarTurmasUniversidade($dados) {
	global $db;
	
	if($dados['pflcod']) {

	    $sql = "SELECT turid as codigo, i.iusnome || ' ( '||tu.turdesc||' )' as descricao 
	    		FROM sispacto2.turmas tu 
	    		INNER JOIN sispacto2.identificacaousuario i ON i.iusd = tu.iusd 
	    		INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
	    		INNER JOIN sispacto2.universidadecadastro unc ON unc.uncid = i.uncid 
	    		INNER JOIN sispacto2.universidade uni ON uni.uniid = unc.uniid 
	    		WHERE t.pflcod='".$dados['pflcod']."' AND unc.uncid='".$dados['uncid']."' ORDER BY descricao";
	    
	    $db->monta_combo('turid_destino', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'turid_destino','', $_REQUEST['turid']);
	    
    } 
	
}

function trocarUniversidade($dados) {
	global $db;
	
	$sql = "UPDATE sispacto2.identificacaousuario SET uncid='".$dados['uncid']."' WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	$sql = "UPDATE sispacto2.usuarioresponsabilidade SET uncid='".$dados['uncid']."' WHERE usucpf=(SELECT iuscpf FROM sispacto2.identificacaousuario WHERE iusd='".$dados['iusd']."') AND rpustatus='A'";
	$db->executar($sql);
	$sql = "UPDATE sispacto2.turmas SET uncid='".$dados['uncid']."' WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	if($dados['turid_destino']) {

		$otuid = $db->pegaUm("SELECT otuid FROM sispacto2.orientadorturma WHERE iusd='".$dados['iusd']."'");
		
		if($otuid) {

			$sql = "UPDATE sispacto2.orientadorturma SET turid='".$dados['turid_destino']."' WHERE iusd='".$dados['iusd']."'";
			$db->executar($sql);
			
		} else {

			$sql = "INSERT INTO sispacto2.orientadorturma(
		            turid, iusd, otustatus, otudata)
		    		VALUES ('".$dados['turid_destino']."', '".$dados['iusd']."', 'A', NOW());";
			
			$db->executar($sql);
		
		}
	}
	
	$db->commit();
	
	$sql = "SELECT turid FROM sispacto2.turmas WHERE iusd='".$dados['iusd']."'";
	$turid = $db->pegaUm($sql);
	
	if($turid) {
		$sql = "SELECT * FROM sispacto2.orientadorturma WHERE turid='".$turid."'";
		$mt = $db->carregar($sql);
	}
	
	if($mt[0]) {
		foreach($mt as $m) {
			$msg .= trocarUniversidade(array('iusd' => $m['iusd'],'uncid' => $dados['uncid'],'return' => true));
		}
	}
	
	$iu = $db->pegaLinha("SELECT iusnome, pfldsc FROM sispacto2.identificacaousuario i 
					INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
					INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod  
					WHERE i.iusd='".$dados['iusd']."'");

	$msg = $iu['iusnome']."( ".$iu['pfldsc']." ) foi atualizado com sucesso;";

	if($dados['return']) {
		return $msg;
	} else {
		$al = array("alert"=>str_replace(";",'\n',$msg),"location"=>"sispacto2.php?modulo=principal/mec/mec&acao=A&aba=trocaruniversidade");
		alertlocation($al);

	}
	
	
}

function carregarCertificacaoEquipe($dados) {
	global $db;
	
	if($dados['sis']=='orientadorestudo') $pflcod = PFL_PROFESSORALFABETIZADOR;
	elseif($dados['sis']=='formadories') $pflcod = PFL_ORIENTADORESTUDO;
	elseif($dados['sis']=='coordenadorlocal') $pflcod = PFL_ORIENTADORESTUDO;
	else die("Perfil não permissão para recomendar certificação.");
	
	
	$limit = $db->pegaUm("SELECT plpmaximobolsas FROM sispacto2.pagamentoperfil WHERE pflcod='".$pflcod."'");
	
	$sql = "SELECT f.fpbid,  m.mesdsc || '/' || fpbanoreferencia as referencia FROM sispacto2.folhapagamentouniversidade u 
			INNER JOIN sispacto2.folhapagamento f ON f.fpbid = u.fpbid 
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
		    WHERE u.uncid='".$dados['uncid']."' ORDER BY f.fpbid LIMIT {$limit}";
	
	$folhapagamento = $db->carregar($sql);
	
	$l[] = "&nbsp;";
	$l[] = "<font style=font-size:xx-small;>CPF</font>";
	$l[] = "<font style=font-size:xx-small;>Nome</font>";
	
	if($folhapagamento) {
		foreach($folhapagamento as $key => $fpb) {

			$c[] = "COALESCE((SELECT mavtotal FROM sispacto2.mensarioavaliacoes ma 
		    		 INNER JOIN sispacto2.mensario me ON me.menid = ma.menid 
					 WHERE me.iusd = i.iusd AND me.fpbid='{$fpb['fpbid']}'),0.00) as total_{$key}";
			
			$c[] = "COALESCE((SELECT mavfrequencia FROM sispacto2.mensarioavaliacoes ma
					 INNER JOIN sispacto2.mensario me ON me.menid = ma.menid
					 WHERE me.iusd = i.iusd AND me.fpbid='{$fpb['fpbid']}'),0.0) as freq_{$key}";
			
			$a[] = "'<font style=font-size:xx-small;>Nota : '||foo.total_{$key}||'<br>Frequência : '||foo.freq_{$key}||'</font>' as alias_{$key}";
			$l[] = "<font style=font-size:xx-small;>".$fpb['referencia']."</font>";
			$to[] = "fee.total_{$key}";
			$fq[] = "fee.freq_{$key}";

		}
	}
	
	$l[] = "<font style=font-size:xx-small;>Avl.Final</font>";
	$l[] = "<font style=font-size:xx-small;>Recomendações</font>";
	
	$dados['sql'] = str_replace(array("pp.pfldsc"),array(implode(",",$c)),$dados['sql']);
	
	$sql = "SELECT '<img align=\"absmiddle\" id=\"img_'||foo.iuscpf||'\" src=\"../imagens/'||CASE WHEN foo.mavrecomendadocertificacao='1' THEN 'valida4.gif'
																								  WHEN foo.mavrecomendadocertificacao='2' THEN 'valida6.gif'
																								  ELSE CASE WHEN avg_total >= 7 AND tot_freq >= 75 THEN 'valida4.gif' ELSE 'valida6.gif' END END||'\"> <img src=\"../imagens/page_attach.png\" id=\"imgc_'||foo.iuscpf||'\" align=absmiddle onclick=\"exibirJustificativa(\''||foo.iuscpf||'\');\" style=\"cursor:pointer;'||CASE WHEN foo.mavrecomendadocertificacaojustificativa IS NULL THEN 'display:none;' ELSE '' END||'\"> ' as acao, 
				   '<font style=font-size:xx-small;>'||replace(to_char(foo.iuscpf::numeric, '000:000:000-00'), ':', '.')||'</font>' as iuscpf,
				   '<font style=font-size:xx-small;>'||foo.iusnome||'</font>',
				   ".implode(",",$a).",
				   '<font style=font-size:xx-small;><b>Nota : '||foo.avg_total||'<br>Frequência : '||foo.tot_freq||'%</b></font> <input type=hidden id=\"recomendacao_'||foo.iuscpf||'\" value=\"'||CASE WHEN avg_total >= 7 AND tot_freq >= 7.5 THEN '1' ELSE '2' END||'\" > <input type=hidden name=\"mavrecomendadocertificacaojustificativa['||foo.iuscpf||']\" id=\"mavrecomendadocertificacaojustificativa_'||foo.iuscpf||'\" value=\"'||COALESCE(foo.mavrecomendadocertificacaojustificativa,'')||'\" >' as avl_final,
				   '<font style=font-size:xx-small;><input type=radio name=\"certificacao['||foo.iuscpf||']\" id=\"certificacao_'||foo.iuscpf||'_1\" value=\"1\" onclick=\"recomendarCertificacao(this,\''||foo.iuscpf||'\');\" '||CASE WHEN foo.mavrecomendadocertificacao='1' THEN 'checked'
					            																																																		WHEN foo.mavrecomendadocertificacao='2' THEN ''
																								  																								  										ELSE CASE WHEN avg_total >= 7 AND tot_freq >= 75 THEN 'checked' ELSE '' END END||'> Recomendo para certificação<br><input type=radio name=\"certificacao['||foo.iuscpf||']\" id=\"certificacao_'||foo.iuscpf||'_2\" value=\"2\"  onclick=\"recomendarCertificacao(this,\''||foo.iuscpf||'\');\" '||CASE WHEN foo.mavrecomendadocertificacao='2' THEN 'checked' 
					            																																																																																																																												  WHEN foo.mavrecomendadocertificacao='1' THEN ''
																								  																								  										 																																																																										  ELSE CASE WHEN avg_total < 7 OR tot_freq < 75 THEN 'checked' ELSE '' END END||'> Não recomendo para certificação' as recomendacao
			
			FROM (
					    				
			SELECT fee.iuscpf,
				   fee.iusnome,
				   ".implode(", ",$to).",
				   ".implode(", ",$fq).", 
				   round((".implode("+",$to).")/".count($to).",2) as avg_total,
				   round(((".implode("+",$fq).")*100)/".count($fq).",0) as tot_freq,
				   (SELECT mavrecomendadocertificacao FROM sispacto2.mensarioavaliacoes ma INNER JOIN sispacto2.mensario m ON m.menid = ma.menid INNER JOIN sispacto2.identificacaousuario i ON i.iusd = m.iusd WHERE i.iuscpf = fee.iuscpf AND ma.iusdavaliador='".$dados['iusd']."' AND m.fpbid='".$dados['fpbid']."' AND mavrecomendadocertificacao is not null) as mavrecomendadocertificacao,
				   (SELECT mavrecomendadocertificacaojustificativa FROM sispacto2.mensarioavaliacoes ma INNER JOIN sispacto2.mensario m ON m.menid = ma.menid INNER JOIN sispacto2.identificacaousuario i ON i.iusd = m.iusd WHERE i.iuscpf = fee.iuscpf AND ma.iusdavaliador='".$dados['iusd']."' AND m.fpbid='".$dados['fpbid']."' AND mavrecomendadocertificacao is not null) as mavrecomendadocertificacaojustificativa
					    	
		    FROM (
		    
		    ".$dados['sql']."
		    							 		
			) fee
			
			) foo";

	$db->monta_lista_simples($sql,$l,5000,10,'N','100%',$par2);

}


function certificarEquipe($dados) {
	global $db;

	if($dados['certificacao']) {
		foreach($dados['certificacao'] as $iuscpf => $recomendacao) {

			$iusd = $db->pegaUm("SELECT iusd FROM sispacto2.identificacaousuario WHERE iuscpf='".$iuscpf."'");

			$dadosmensario = criarMensario(array("iusd"=>$iusd,"fpbid"=>$dados['fpbid']));
				
			if($dadosmensario['esdid']!=ESD_APROVADO_MENSARIO) {
	
				$sql = "SELECT mavid FROM sispacto2.mensarioavaliacoes WHERE menid='".$dadosmensario['memid']."' AND iusdavaliador='".$dados['iusdavaliador']."'";
				$mavid = $db->pegaUm($sql);
	
				if($mavid) {
							
					$sql = "UPDATE sispacto2.mensarioavaliacoes SET mavrecomendadocertificacao='".$recomendacao."', mavtotal='0', mavrecomendadocertificacaojustificativa=".(($dados['mavrecomendadocertificacaojustificativa'][$iuscpf])?"'".$dados['mavrecomendadocertificacaojustificativa'][$iuscpf]."'":"NULL")." WHERE mavid='".$mavid."'";
					$db->executar($sql);
							
						
				} else {
							
					$sql = "INSERT INTO sispacto2.mensarioavaliacoes(
	            			iusdavaliador, mavtotal, mavrecomendadocertificacao, mavrecomendadocertificacaojustificativa, menid)
	    					VALUES ('".$dados['iusdavaliador']."', '0',
	    							'".$recomendacao."', ".(($dados['mavrecomendadocertificacaojustificativa'][$iuscpf])?"'".$dados['mavrecomendadocertificacaojustificativa'][$iuscpf]."'":"NULL").", '".$dadosmensario['memid']."') RETURNING mavid;";

					$mavid = $db->pegaUm($sql);
						
				}
					
			}
		}
	}
	
	$db->commit();
	
	$al = array("alert"=>"Recomendações gravadas com sucesso","location"=>$dados['goto']);
	alertlocation($al);

}

function abaSomenteSuper($dados) {

	if($db->testa_superuser()) return true;
	else return false;

}

function salvarOrientacaoAdm($dados) {
	global $db;

	$oabid = $db->pegaUm("SELECT oabid FROM sispacto2.orientacaoaba WHERE abaid='".$dados['abaid']."'");

	if($oabid) {

		$sql = "UPDATE sispacto2.orientacaoaba SET oabdesc='".$dados['oabdesc']."' WHERE oabid='".$oabid."'";
		$db->executar($sql);

	} else {

		$sql = "INSERT INTO sispacto2.orientacaoaba(
	            abaid, oabdesc, oabstatus)
	    		VALUES ('".$dados['abaid']."', '".$dados['oabdesc']."', 'A');";
		$db->executar($sql);

	}

	$db->commit();

	$al = array("alert"=>"Orientação gravada com sucesso.","location"=>$_SERVER['REQUEST_URI']);
	alertlocation($al);

}

function escondeAba($dados) {
	return false;
}


function criarBotoesNavegacao($dados) {
	global $db;
	
	if($dados['url']) {

		$aba = $db->pegaLinha("SELECT abaendereco, abaordem, abapai, abafuncaomostrar FROM sispacto2.abas WHERE abaendereco='".$dados['url']."'");
		
		$abaanterior = $db->pegaLinha("SELECT abaendereco, abafuncaomostrar FROM sispacto2.abas WHERE abaordem='".($aba['abaordem']-1)."' AND abapai='".$aba['abapai']."'");
		$abaproxima = $db->pegaLinha("SELECT abaendereco, abafuncaomostrar FROM sispacto2.abas WHERE abaordem='".($aba['abaordem']+1)."' AND abapai='".$aba['abapai']."'");
		
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


function carregarTurmasCoordenadorLocal($dados) {
	global $db;
	
	$sql = "INSERT INTO sispacto2.turmas(
            uncid, iusd, turdesc, turstatus, picid, muncod, pflcod)
			SELECT i.uncid, i.iusd, 'TURMA SUP #'||i.iusd as turdesc, 'A', null, null, t.pflcod FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=1130
			LEFT JOIN sispacto2.turmas tu ON tu.iusd = i.iusd 
			WHERE i.iusstatus='A' AND tu.turid IS NULL 
			AND i.uncid IN(

			SELECT u.uncid FROM sispacto2.universidadecadastro u 
						INNER JOIN workflow.documento d ON d.docid = u.docid 
						INNER JOIN workflow.documento d2 ON d2.docid = u.docidformacaoinicial 
						WHERE d.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND d2.esdid='".ESD_FECHADO_FORMACAOINICIAL."'
			
			)";
	
	$db->executar($sql);
	
	$sql = "INSERT INTO sispacto2.turmas(
            uncid, iusd, turdesc, turstatus, picid, muncod, pflcod)
			SELECT i.uncid, i.iusd, 'TURMA CL #'||i.iusd as turdesc, 'A', i.picid, null, t.pflcod FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL." 
			LEFT JOIN sispacto2.turmas tu ON tu.iusd = i.iusd  AND tu.pflcod=".PFL_COORDENADORLOCAL." 
			WHERE i.iusstatus='A' AND tu.turid IS NULL AND i.picid IN(
			
			SELECT i.picid FROM sispacto2.universidadecadastro u 
			INNER JOIN workflow.documento d ON d.docid = u.docid 
			INNER JOIN workflow.documento d2 ON d2.docid = u.docidformacaoinicial 
			INNER JOIN sispacto2.estruturacurso e ON e.uncid = u.uncid 
			INNER JOIN sispacto2.abrangencia a ON a.ecuid = e.ecuid AND a.esfera='M' 
			INNER JOIN sispacto2.pactoidadecerta p ON p.muncod = a.muncod 
			INNER JOIN sispacto2.identificacaousuario i ON i.picid = p.picid 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
			WHERE d.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND d2.esdid='".ESD_FECHADO_FORMACAOINICIAL."' AND i.iusstatus='A'
			GROUP BY i.picid
						 
			)";
	
	$db->executar($sql);
	
	$sql = "INSERT INTO sispacto2.orientadorturmaoutros(
            turid, iusd, otustatus, otudata)
			SELECT (SELECT max(turid) FROM sispacto2.turmas WHERE pflcod=".PFL_COORDENADORLOCAL." AND picid=i.picid) as turid, i.iusd, 'A', NOW() FROM sispacto2.identificacaousuario i 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_ORIENTADORESTUDO." 
			LEFT JOIN sispacto2.orientadorturmaoutros ot ON ot.iusd = i.iusd  
			WHERE i.iusstatus='A' AND ot.otuid IS NULL AND i.picid IN(
			
			SELECT i.picid FROM sispacto2.universidadecadastro u 
			INNER JOIN workflow.documento d ON d.docid = u.docid 
			INNER JOIN workflow.documento d2 ON d2.docid = u.docidformacaoinicial 
			INNER JOIN sispacto2.estruturacurso e ON e.uncid = u.uncid 
			INNER JOIN sispacto2.abrangencia a ON a.ecuid = e.ecuid AND a.esfera='M' 
			INNER JOIN sispacto2.pactoidadecerta p ON p.muncod = a.muncod 
			INNER JOIN sispacto2.identificacaousuario i ON i.picid = p.picid 
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
			WHERE d.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND d2.esdid='".ESD_FECHADO_FORMACAOINICIAL."' AND i.iusstatus='A'
			GROUP BY i.picid 
			HAVING count(i.iusd)=1
			
			)";
	
	$db->executar($sql);
	
	$db->commit();
	
	
	$sql = "INSERT INTO sispacto2.turmas(
            uncid, iusd, turdesc, turstatus, picid, muncod, pflcod)
			SELECT i.uncid, i.iusd, 'TURMA CL #'||i.iusd as turdesc, 'A', i.picid, null, t.pflcod FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL." 
			INNER JOIN sispacto2.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			LEFT JOIN sispacto2.turmas tu ON tu.iusd = i.iusd
			WHERE i.iusstatus='A' AND tu.turid IS NULL AND d.esdid='".ESD_VALIDADO_COORDENADOR_LOCAL."'";
	
	$db->executar($sql);
	
	$sql = "INSERT INTO sispacto2.orientadorturmaoutros(
            turid, iusd, otustatus, otudata)
			SELECT (SELECT max(turid) FROM sispacto2.turmas WHERE pflcod=".PFL_COORDENADORLOCAL." AND picid=i.picid) as turid, i.iusd, 'A', NOW() FROM sispacto2.identificacaousuario i
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_ORIENTADORESTUDO."
			LEFT JOIN sispacto2.orientadorturmaoutros ot ON ot.iusd = i.iusd
			WHERE i.iusstatus='A' AND ot.otuid IS NULL AND i.picid IN(
		
			SELECT i.picid FROM sispacto2.universidadecadastro u
			INNER JOIN workflow.documento d ON d.docid = u.docid
			INNER JOIN workflow.documento d2 ON d2.docid = u.docidformacaoinicial
			INNER JOIN sispacto2.estruturacurso e ON e.uncid = u.uncid
			INNER JOIN sispacto2.abrangencia a ON a.ecuid = e.ecuid AND a.esfera='E' 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sispacto2.pactoidadecerta p ON p.estuf = m.estuf 
			INNER JOIN sispacto2.identificacaousuario i ON i.picid = p.picid
			INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
			WHERE d.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND d2.esdid='".ESD_FECHADO_FORMACAOINICIAL."' AND i.iusstatus='A'
			GROUP BY i.picid
			HAVING count(distinct i.iusd)=1
		
			)";
	
	$db->executar($sql);
	
	$db->commit();
}


function condicaoComposicaoTurma( $picid, $uncid ) {
	global $db;
	
	if($picid) {
		$erros = validarEnvioAnaliseIES();
	
		$sql = "SELECT COUNT(*) FROM sispacto2.identificacaousuario i
		 		 INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd AND t.pflcod='".PFL_PROFESSORALFABETIZADOR."'
		 		 WHERE i.picid='".$picid."'";
	
		$qtd_professores = $db->pegaUm($sql);
	
		if($qtd_professores == 0) {
			$erros .= '- É necessário cadastrar Professores Alfabetizadores';
		}
	}
	
	if($uncid) {

		$sql = "SELECT i.iusnome, tu.turid, CASE WHEN tu.turid IS NOT NULL THEN (SELECT count(*) FROM sispacto2.orientadorturmaoutros WHERE turid=tu.turid) ELSE 0 END ntur  FROM sispacto2.identificacaousuario i 
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd ANd t.pflcod='".PFL_SUPERVISORIES."' 
				LEFT JOIN sispacto2.turmas tu ON tu.iusd = i.iusd 
				WHERE i.uncid='".$uncid."'";
		
		$lista = $db->carregar($sql);
		
		if($lista[0]) {
			foreach($lista as $l) {
				if(!$l['turid']) $erros .= $l['iusnome'].' não possui turma cadastrada\n<br>';
				elseif(!$l['ntur']) $erros .= $l['iusnome'].' não possui equipe cadastrada na turma\n<br>';
			}
		}
		
		$sql = "SELECT i.iusnome, p.pfldsc FROM sispacto2.identificacaousuario i 
				INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd ANd t.pflcod IN('".PFL_FORMADORIESP."','".PFL_FORMADORIES."','".PFL_COORDENADORLOCAL."') 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				LEFT JOIN sispacto2.orientadorturmaoutros ot ON ot.iusd = i.iusd  
				WHERE i.iusstatus='A' ANd i.uncid='".$uncid."' AND ot.otuid IS NULL";
		
		$lista = $db->carregar($sql);
		
		if($lista[0]) {
			foreach($lista as $l) {
				$erros .= $l['iusnome'].'('.$l['pfldsc'].') não esta alocado em nenhuma turma\n<br>';
			}
		}
		
	}

	return (($erros)?$erros:true);
}

function carregarTurmasOutros($dados) {
	global $db;

	$sql = "SELECT
				'<img src=\"../imagens/mais.gif\" title=\"mais\" id=\"btn_turma_'||t.turid||'\" style=\"cursor:pointer;\" onclick=\"abrirTurmaOutros('||t.turid||',this)\"> ".((!$dados['consulta'])?"<img src=../imagens/salvar.png style=\"cursor:pointer;\" onclick=\"comporTurma(\''||turid||'\')\">":"")."' as acao,
				t.turdesc,
				CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END as iuscpf,
				i.iusnome||' ( '||p.pfldsc||' )' as iusnome,
				i.iusemailprincipal,
				(SELECT '(' || itedddtel || ') '|| itenumtel FROM sispacto2.identificacaotelefone WHERE iusd=t.iusd AND itetipo='T') as telefone,
				(SELECT COUNT(*) FROM sispacto2.orientadorturmaoutros ot INNER JOIN sispacto2.tipoperfil tt ON tt.iusd = ot.iusd WHERE turid=t.turid) as nalunos
			FROM sispacto2.turmas t
			INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd
			LEFT JOIN sispacto2.tipoperfil t1 ON t1.iusd = i.iusd
			LEFT JOIN seguranca.perfil p ON p.pflcod = t1.pflcod
			WHERE ".(($dados['picid'])?"t.picid='".$dados['picid']."'":"")." ".(($dados['uncid'])?"t.uncid='".$dados['uncid']."'":"")." AND t1.pflcod='".$dados['pflcod']."' ORDER BY turid DESC";

	$cabecalho = array("&nbsp;","Turma","CPF","Nome","Email","Telefone","Número de orientadores");

	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);

}

function abaTurmaCLs($dados) {
	global $db;
	
	$qtd_cl = $db->pegaUm("SELECT count(*) FROM sispacto2.identificacaousuario i 
						   INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
						   WHERE t.pflcod='".PFL_COORDENADORLOCAL."' AND i.picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."' AND i.iusstatus='A'");

	$estado = wf_pegarEstadoAtual( $_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['docid'] );
	
	if($estado['esdid'] == ESD_VALIDADO_COORDENADOR_LOCAL && $qtd_cl > 1) {
		return true;
	} else {
		return false;
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
<p>f) avaliar os professores alfabetizadores cursistas quanto à frequência, à participação e ao acompanhamento dos estudantes, registrando as informações no SisPacto;</p> 
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
<p>b) cadastrar os orientadores de estudo e os professores alfabetizadores no SisPacto e no SGB;</p> 
<p>c) monitorar a realização dos encontros presenciais ministrados pelos orientadores de estudo junto aos professores alfabetizadores;</p> 
<p>d) apoiar as IES na organização do calendário acadêmico, na definição dos polos de formação e na adequação das instalações físicas para a realização dos encontros presenciais;</p> 
<p>e) assegurar, junto à respectiva secretaria de Educação, as condições de deslocamento e hospedagem para participação nos encontros presenciais dos orientadores de estudo e dos professores alfabetizadores, sempre que necessário;</p> 
<p>f) articular-se com os gestores escolares e coordenadores pedagógicos visando ao fortalecimento da Formação Continuada de Professores Alfabetizadores;</p>
<p>g) organizar e coordenar o seminário de socialização de experiências em seu âmbito de atuação (municipal, estadual ou distrital);</p>
<p>h) monitorar o recebimento e devida utilização dos materiais pedagógicos previstos nas ações do Pacto;</p>
<p>i) acompanhar as ações da secretaria de Educação na aplicação das avaliações diagnósticas, e assegurar que os professores alfabetizadores registrem os resultados obtidos pelos alunos no SisPacto;</p> 
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
<p>d) monitorar a frequência, a participação e as avaliações dos orientadores de estudo no SisPacto;</p>
<p>e) acompanhar as atividades dos orientadores de estudo junto aos professores alfabetizadores;</p>
<p>f) organizar os seminários ou encontros com os orientadores de estudo para acompanhamento e avaliação da Formação;</p> 
<p>g) analisar e discutir os relatórios de formação com os orientadores de estudo;</p>
<p>h) elaborar e encaminhar ao supervisor da Formação os relatórios dos encontros presenciais;</p>
<p>i) analisar, em conjunto com os orientadores de estudo, os relatórios das turmas de professores alfabetizadores e orientar os encaminhamentos;</p> 
<p>j) encaminhar a documentação necessária para a certificação dos orientadores de estudo e dos professores alfabetizadores; e</p>
<p>k) acompanhar, no SisPacto, o desempenho das atividades de formação previstas para os orientadores de estudo sob sua responsabilidade, informando ao supervisor sobre eventuais ocorrências que interfiram no pagamento da bolsa no período.</p> 
<p><input type=checkbox id=declaro name=declaro onclick=\"declaracaoatribuicoes(this);\" > Declaro ter ciência das minhas atribuições.</p>
			";
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

function verificarInstrumentosPendentes($dados) {
	global $db;
	
	$arr = $db->pegaLinha("SELECT it.intdsc, it.intid, it.inttitulo, it.inttextoperguntas, it.inttextoopcoes FROM sispacto2.instrumentostitulo it
						   INNER JOIN sispacto2.instrumentosperguntas i ON i.intid = it.intid
						   LEFT JOIN sispacto2.instrumentosrespostas ir ON ir.inpid = i.inpid AND ir.iusd='".$dados['iusd']."'
 						   WHERE ir.inrid IS NULL AND it.intstatus='A'
						   LIMIT 1");

	if($dados['contagem']) {
		echo count($arr);
	} else {
		return $arr;
	}

	

}

function carregarInstrumentos($dados) {
	global $db;
	
	$intline = verificarInstrumentosPendentes(array('iusd'=>$dados['iusd']));
	
	if($intline) {

		echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		echo '<tr><td class="SubTituloCentro">'.$intline['intdsc'].'</td></tr>';
		echo '<tr><td>'.$intline['inttitulo'].'</td></tr>';

		$inpline = $db->carregar("SELECT i.inpid, i.inpdsc, i.inogrupo, ir.inoid FROM sispacto2.instrumentosperguntas i 
								  LEFT JOIN sispacto2.instrumentosrespostas ir ON ir.inpid = i.inpid AND ir.iusd='".$dados['iusd']."' 
								  WHERE intid='".$intline['intid']."' ORDER BY inpordem");
		
		echo '<tr><td>';
		
		if($inpline[0]) {

			echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">';

			echo '<tr>';
			echo '<td class="SubTituloCentro">'.$intline['inttextoperguntas'].'</td>';
			echo '<td class="SubTituloCentro">'.$intline['inttextoopcoes'].'</td>';
			echo '</tr>';
			
			foreach($inpline as $inp) {
				echo '<tr>';
				echo '<td class="SubTituloDireita"><span style=font-size:x-small;>'.$inp['inpdsc'].'</span></td>';
				echo '<td>';
				
				$sql = "(
              			SELECT '&inpid={$inp['inpid']}' as codigo, 'Selecione' as descricao
              			) UNION ALL (
              			SELECT '&inoid='||inoid||'&inpid={$inp['inpid']}' as codigo, inodsc as descricao FROM sispacto2.instrumentosopcoes WHERE inogrupo='{$inp['inogrupo']}' ORDER BY inoid
						)";
				$db->monta_combo('inoid', $sql, 'S', '', 'gravarRespostaInstrumento', '', '', '', 'N', 'inoid_'.$inp['inpid'],'', "&inoid=".$inp['inoid']."&inpid=".$inp['inpid']);
				
				echo '</td>';
				echo '</tr>';

			}
			
			echo '<tr>';
			echo '<td class="SubTituloCentro" colspan="2"><input type="button" name="concluir" value="Concluir" onclick="avancarInstrumento();"></td>';
			echo '</tr>';
			
			echo '</table>';
			
		}
		
		echo '</td></tr>';
        
        echo '</table>';
		
	}
}

function gravarRespostaInstrumento($dados) {
	global $db;
	
	if($dados['inpid']) {

		$inrid = $db->pegaUm("SELECT inrid FROM sispacto2.instrumentosrespostas WHERE iusd='".$dados['iusd']."' AND inpid='".$dados['inpid']."'");
		
		if($inrid) {
	
			if($dados['inoid']) $sql = "UPDATE sispacto2.instrumentosrespostas SET inoid='".$dados['inoid']."' WHERE inrid='{$inrid}'";
			else $sql = "DELETE FROM sispacto2.instrumentosrespostas WHERE inrid='{$inrid}'";
			
		} else {
			
			if($dados['inoid']) {
				$sql = "INSERT INTO sispacto2.instrumentosrespostas(
				            iusd, inpid, inoid)
				    	VALUES ('".$dados['iusd']."', '".$dados['inpid']."', '".$dados['inoid']."');";
			
			}
		
		}
		
	}
	
	if($sql) {
		$db->executar($sql, false);
		$db->commit();
	}
	
}


function montarGraficoPagamentos() {

	include_once APPRAIZ . "includes/library/simec/Grafico.php";
	
	$grafico = new Grafico(Grafico::K_TIPO_PIZZA);
	
	echo '<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">';
	
	echo '<tr>';
	
	echo '<td>';

	$sql = "select pe.pfldsc as descricao, sum(p.pbovlrpagamento) as valor, '' as categoria from sispacto2.pagamentobolsista p
			inner join seguranca.perfil pe on pe.pflcod = p.pflcod
			inner join workflow.documento d on d.docid = p.docid
			where d.esdid=".ESD_PAGAMENTO_EFETIVADO."
			group by pe.pfldsc";
	
	$grafico->setTitulo('Bolsas efetivadas (R$) por perfil')->setFormatoTooltip("function() { return '<span><span style=\"color: ' + this.series.color + '\">' + this.series.name + '</span>: <b>' + number_format(this.y, 2, ',', '.') + '</b>'; }")->montarGraficoLinha($sql);
	
	echo '</td>';
	
	echo '<td>';
	
	$sql = "select e.esddsc as descricao, count(*) as valor, '' as categoria
			from sispacto2.pagamentobolsista p
			inner join workflow.documento d on d.docid = p.docid
			inner join workflow.estadodocumento e on e.esdid = d.esdid
			where d.esdid!=".ESD_PAGAMENTO_EFETIVADO."
			group by e.esddsc";
	
	$grafico->setTitulo('Situação das bolsas em trâmite (exceto pagamento efetivado)')->setFormatoTooltip("function() { return '<span><span style=\"color: ' + this.series.color + '\">' + this.series.name + '</span>: <b>' + number_format(this.y, 0, ',', '.') + '</b>'; }")->montarGraficoLinha($sql);
	
	echo '</td>';
	
	
	echo '</tr>';
	
	echo '</table>';


}

function montarGraficoPerfilTurmas() {

	global $db;
	
	include_once APPRAIZ . "includes/library/simec/Grafico.php";
	
	$sql = "SELECT  
		                SUM(tpatotalmeninos) as totalmeninos, 
		                SUM(tpatotalmeninas) as totalmeninas,
		                COALESCE(SUM(tpafaixaetariaabaixo6anos),0) as tpafaixaetariaabaixo6anos,
		                COALESCE(SUM(tpafaixaetaria6anos),0) as faixaetaria6anos,
		                COALESCE(SUM(tpafaixaetaria7anos),0) as tpafaixaetaria7anos,
		                COALESCE(SUM(tpafaixaetaria8anos),0) as tpafaixaetaria8anos,
		                COALESCE(SUM(tpafaixaetaria9anos),0) as tpafaixaetaria9anos,
		                COALESCE(SUM(tpafaixaetaria10anos),0) as tpafaixaetaria10anos,
		                COALESCE(SUM(tpafaixaetaria11anos),0) as tpafaixaetaria11anos,
		                COALESCE(SUM(tpafaixaetariaacima11anos),0) as tpafaixaetariaacima11anos
				FROM sispacto2.turmasprofessoresalfabetizadores t
				INNER JOIN territorios.municipio m on m.muncod = t.tpamuncodescola 
				INNER JOIN sispacto2.identificacaousuario i ON i.iusd = t.iusd  
				WHERE tpastatus='A' AND tpaconfirmaregencia=true AND (tpatotalmeninos is not null or tpatotalmeninas is not null)";
	
	$perfilturmas = $db->pegaLinha($sql);
	
	$dadosgenero[] = array('valor' => $perfilturmas['totalmeninos'], 'descricao' => 'Meninos', 'categoria' => '');
	$dadosgenero[] = array('valor' => $perfilturmas['totalmeninas'], 'descricao' => 'Meninas', 'categoria' => '');
	
	$dadosfaixaetaria[] = array('valor' => $perfilturmas['tpafaixaetariaabaixo6anos'], 'descricao' => 'Abaixo de 6 anos', 'categoria' => '');
	$dadosfaixaetaria[] = array('valor' => $perfilturmas['faixaetaria6anos'], 'descricao' => '6 anos', 'categoria' => '');
	$dadosfaixaetaria[] = array('valor' => $perfilturmas['tpafaixaetaria7anos'], 'descricao' => '7 anos', 'categoria' => '');
	$dadosfaixaetaria[] = array('valor' => $perfilturmas['tpafaixaetaria8anos'], 'descricao' => '8 anos', 'categoria' => '');
	$dadosfaixaetaria[] = array('valor' => $perfilturmas['tpafaixaetaria9anos'], 'descricao' => '9 anos', 'categoria' => '');
	$dadosfaixaetaria[] = array('valor' => $perfilturmas['tpafaixaetaria10anos'], 'descricao' => '10 anos', 'categoria' => '');
	$dadosfaixaetaria[] = array('valor' => $perfilturmas['tpafaixaetaria11anos'], 'descricao' => '11 anos', 'categoria' => '');
	$dadosfaixaetaria[] = array('valor' => $perfilturmas['tpafaixaetariaacima11anos'], 'descricao' => 'Acima de 11 anos', 'categoria' => '');
	
	$grafico = new Grafico(Grafico::K_TIPO_PIZZA);
	
	echo '<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">';
	
	echo '<tr>';
	
	echo '<td>';
	
	$grafico->setTitulo('Gênero')->montarGraficoLinha($dadosgenero);
	
	echo '</td>';
	
	echo '<td>';
	
	$grafico->setTitulo('Faixa Etária')->montarGraficoLinha($dadosfaixaetaria);
	
	echo '</td>';
	
	
	echo '</tr>';
	
	echo '</table>';


}

function inserirComprovanteGRU($dados) {
	global $db;
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	$campos = array("cogcpf" => "'".str_replace(array(".","-"),array("",""),$dados['cogcpf'])."'",
					"fpbid" => $dados['fpbid'],
					"pflcod" => $dados['pflcod'],
					"cogdtpagamento" => "'".formata_data_sql($dados['cogdtpagamento'])."'",
					"cogdtinsercao" => "NOW()",
					"cogcpfinsercao" => "'".$_SESSION['usucpf']."'");

	$file = new FilesSimec( "comprovantegru", $campos, "sispacto2" );
	$file->setUpload( NULL, "arquivo" );
	
	$al = array("alert"=>"Comprovante GRU inserido com sucesso","location"=>"sispacto2.php?modulo=principal/comprovantegru&acao=A");
	alertlocation($al);
	
}

function excluirComprovanteGRU($dados) {
	global $db;
	
	$sql = "UPDATE sispacto2.comprovantegru SET cogstatus='I' WHERE cogid='".$dados['cogid']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Comprovante GRU removido com sucesso","location"=>"sispacto2.php?modulo=principal/comprovantegru&acao=A");
	alertlocation($al);
	
}

function salvarInstrumentosAbertos($dados) {
	global $db;
	
	$iraid = $db->pegaUm("SELECT iraid FROM sispacto2.instrumentosrespostasabertas WHERE iusd='".$dados['iusd']."'");
	
	if($iraid) {

		$sql = "UPDATE sispacto2.instrumentosrespostasabertas SET irapergunta1=".(($dados['irapergunta1'])?"'".$dados['irapergunta1']."'":"NULL").", 
																  irapergunta2=".(($dados['irapergunta2'])?"'".$dados['irapergunta2']."'":"NULL").",
																  irapergunta3=".(($dados['irapergunta3'])?"'".$dados['irapergunta3']."'":"NULL").",
																  irapergunta4=".(($dados['irapergunta4'])?"'".$dados['irapergunta4']."'":"NULL").",
																  irapergunta5=".(($dados['irapergunta1'])?"'".$dados['irapergunta1']."'":"NULL")." WHERE iraid={$iraid}";

	} else { 

		$sql = "INSERT INTO sispacto2.instrumentosrespostasabertas(
		            iusd, irapergunta1, irapergunta2, irapergunta3, irapergunta4, irapergunta5)
		    VALUES ('".$dados['iusd']."', '".$dados['irapergunta1']."', '".$dados['irapergunta2']."', '".$dados['irapergunta3']."', '".$dados['irapergunta4']."', '".$dados['irapergunta5']."');";

	}
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Pesquisa gravada com sucesso","location"=>"sispacto2.php?modulo=".$dados['modulo']."&acao=A&aba=principal");
	alertlocation($al);
	
}

function visualizarDesabilitado($dados) {

	$_SESSION['sispacto2'][$dados['vis']]['iusdesativado'] = false;

	$al = array("location" => "sispacto2.php?modulo=principal/{$dados['vis']}/{$dados['vis']}&acao=A");
	alertlocation($al);

}

function analisarRelatorioFinal($dados) {
	global $db;
	
	include "_funcoes_universidade.php";
	
	if($dados['iusd'] && $dados['uncid']) {
		$_SESSION['sispacto2']['universidade']['iusd'] = $dados['iusd'];
		$_SESSION['sispacto2']['universidade']['uncid'] = $dados['uncid'];
		
		echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';
		echo '<link href="../includes/Estilo.css" rel="stylesheet" type="text/css"/>';
		echo '<link href="../includes/listagem.css" rel="stylesheet" type="text/css"/>';
		
		$consulta = true;
		
		include APPRAIZ_SISPACTO.'universidade/avaliacaofinal.inc';
	}
}


?>