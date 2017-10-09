<?

function alertlocation($dados) {
	
	die("<script>
		".(($dados['alert'])?"alert('".$dados['alert']."');":"")."
		".(($dados['location'])?"window.location='".$dados['location']."';":"")."
		".(($dados['javascript'])?$dados['javascript']:"")."
		 </script>");
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



function pegarDadosUsuarioPorCPF($dados) {
	global $db;
	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf='".$dados['cpf']."'";
	$usuemail = $db->pegaUm($sql);
	
	$sql = "SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf='".$dados['cpf']."' AND sisid='".SIS_ESCOLATERRA."'";
	$suscod = $db->pegaUm($sql);
	
	
	echo $usuemail."||".(($suscod)?$suscod:"NC");
}

function excluirUsuarioPerfil($dados) {
	global $db;

	$npagamentos = $db->pegaUm("SELECT COUNT(*) FROM escolaterra.pagamentobolsista WHERE iusid='".$dados['iusid']."'");

	if($npagamentos > 0) {

		$identificacaousuario = $db->pegaLinha("SELECT i.iuscpf, t.pflcod, p.pfldsc FROM escolaterra.identificacaousuario i
												INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid
												INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
												WHERE i.iusid='".$dados['iusid']."'");


		$sql = "INSERT INTO escolaterra.identificacaousuario(
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
		FROM escolaterra.identificacaousuario where iusid='".$dados['iusid']."'
		RETURNING iusid;";

		$iusid_novo = $db->pegaUm($sql);


		$sql = "DELETE FROM escolaterra.usuarioresponsabilidade  WHERE rpustatus='A' AND usucpf='".$identificacaousuario['iuscpf']."' AND pflcod='".$identificacaousuario['pflcod']."'";
		$db->executar($sql);

		$sql = "UPDATE escolaterra.tipoperfil SET iusid='".$iusid_novo."' WHERE iusid='".$dados['iusid']."'";
		$db->executar($sql);

		$sql = "UPDATE escolaterra.turmas SET iusid='".$iusid_novo."' WHERE iusid='".$dados['iusid']."'";
		$db->executar($sql);

		$sql = "UPDATE escolaterra.turmaidusuario SET iusid='".$iusid_novo."' WHERE iusid='".$dados['iusid']."'";
		$db->executar($sql);

		$sql = "UPDATE escolaterra.identificacaousuario SET iusstatus='I' WHERE iusid='".$dados['iusid']."'";
		$db->executar($sql);

		$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$identificacaousuario['iuscpf']."' AND pflcod='".$identificacaousuario['pflcod']."'";
		$db->executar($sql);

		// removendo avaliações não concluidas
		$sql = "SELECT m.menid FROM escolaterra.mensario m
				INNER JOIN workflow.documento d ON d.docid = m.docid
				WHERE iusid='".$dados['iusid']."' AND d.esdid!='".ESD_APROVADO_MENSARIO."'";

		$menids = $db->carregarColuna($sql);

		if($menids) {
		
			$sql = "SELECT mavid FROM escolaterra.mensarioavaliacoes WHERE menid IN('".implode("','",$menids)."')";
			$mavids = $db->carregarColuna($sql);
				
			if($mavids) {
				$db->executar("DELETE FROM escolaterra.historicoreaberturanota WHERE mavid IN('".implode("','",$mavids)."')");
				$db->executar("DELETE FROM escolaterra.mensarioavaliacoes WHERE mavid IN('".implode("','",$mavids)."')");
			}
		}


	} else {

		$sql = "DELETE FROM escolaterra.tipoperfil WHERE iusid='".$dados['iusid']."' AND pflcod='".$dados['pflcod']."'";
		$db->executar($sql);
		
		$sql = "UPDATE escolaterra.identificacaousuario SET iusstatus='I' WHERE iusid='".$dados['iusid']."'";
		$db->executar($sql);
	
		$usucpf = $db->pegaUm("SELECT iuscpf FROM escolaterra.identificacaousuario WHERE iusid='".$dados['iusid']."'");
	
		if($usucpf) {
			$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$usucpf."' AND pflcod='".$dados['pflcod']."'";
			$db->executar($sql);
			$sql = "DELETE FROM escolaterra.usuarioresponsabilidade WHERE usucpf='".$usucpf."' AND pflcod='".$dados['pflcod']."'";
			$db->executar($sql);
		}
	
		$sql = "DELETE FROM escolaterra.turmaidusuario WHERE iusid='".$dados['iusid']."'";
		$db->executar($sql);

	}

	$db->commit();

	$al = array("alert"=>"Exclusão ocorrida com sucesso","location"=>"escolaterra.php?modulo=".$dados['modulo']."&acao=".$dados['acao']."&aba=gerenciarusuario&uncid=".$dados['uncid']);
	alertlocation($al);


}

function carregarDadosIdentificacaoUsuario($dados) {
	global $db;
	
	if(!$dados['pflcod']) {
		$al = array("alert"=>"Problemas para carregar os dados usuário","location"=>"escolaterra.php?modulo=inicio&acao=C");
		alertlocation($al);
	}
	
	if($dados['iusid']) {
		$wh[] = "i.iusid='".$dados['iusid']."'";
	}
	
	if($dados['ufpid']) {
		$wh[] = "i.ufpid='".$dados['ufpid']."'";
	}
	
	$sql = "SELECT i.cadastradosgb, i.iusid, i.iuscpf, i.iusnome, i.iusdatanascimento, i.iusnomemae, i.iusnaodesejosubstituirbolsa,
				   i.iussexo, i.eciid, i.nacid, i.iusnomeconjuge, i.iusagenciasugerida, i.iusagenciaend, 
				   i.iusemailprincipal, i.iusemailopcional, to_char(i.iusdatainclusao,'YYYY-mm-dd') as iusdatainclusao, i.iustermocompromisso,  
				   i.tvpid, i.funid, i.foeid, f.iufid, f.cufid, f.iufdatainiformacao, f.iufdatafimformacao, f.iufsituacaoformacao,
				   m.estuf as estuf_nascimento, m.muncod as muncod_nascimento, ma.estuf||' / '||ma.mundescricao as municipiodescricaoatuacao, ma.muncod as muncodatuacao, 
				   d.itdid, d.tdoid, d.itdufdoc, d.itdnumdoc, d.itddataexp, d.itdnoorgaoexp,
				   e.ienid, mm.muncod as muncod_endereco, mm.estuf as estuf_endereco,
				   e.ientipo, e.iencep, e.iencomplemento, e.iennumero, e.ienlogradouro, e.ienbairro, cf.cufcodareageral
			FROM escolaterra.identificacaousuario i 
			INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid 
			LEFT  JOIN territorios.municipio m ON m.muncod = i.muncod 
			LEFT  JOIN escolaterra.identiusucursoformacao f ON f.iusid = i.iusid 
			LEFT  JOIN escolaterra.identusutipodocumento d ON d.iusid = i.iusid 
			LEFT  JOIN escolaterra.identificaoendereco e ON e.iusid = i.iusid 
			LEFT  JOIN territorios.municipio mm ON mm.muncod = e.muncod 
			LEFT  JOIN territorios.municipio ma ON ma.muncod = i.muncodatuacao
			LEFT  JOIN escolaterra.cursoformacao cf ON cf.cufid = f.cufid 
			LEFT  JOIN escolaterra.turmaidusuario ot ON ot.iusid = i.iusid
			WHERE t.pflcod='".$dados['pflcod']."' AND iusstatus='A' ".(($wh)?"AND ".implode(" AND ",$wh):"")." ORDER BY i.iusid";
	
	$identificacaousuario = $db->carregar($sql);
	
	if($identificacaousuario[0]) {

		foreach($identificacaousuario as $key => $iu) {
			
			$idusuarios[$key] = $iu;
			unset($telefones);
			$sql = "SELECT itetipo, itedddtel, itenumtel FROM escolaterra.identificacaotelefone WHERE iusid='".$iu['iusid']."'";
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

function efetuarTrocaUsuarioPerfil($dados) {
	global $db;
	
	if(!$dados['iuscpf_']) $erro[] = "CPF em branco";
	if(!$dados['iusnome_']) $erro[] = "Nome em branco";
	if(!$dados['iusemailprincipal_']) $erro[] = "Email em branco";
	
	if($erro) {
		$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erro),"location"=>$_SERVER['HTTP_REFERER']);
		alertlocation($al);
	}

	$sql = "SELECT * 
			FROM escolaterra.identificacaousuario i 
			INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid 
			WHERE i.iusid='".$dados['iusidantigo']."'";
	
	$identificacaousuario_antigo = $db->pegaLinha($sql);
	
	if(!$identificacaousuario_antigo) {
		$al = array("alert"=>"Usuário a ser substituido não foi encontrado","location"=>$_SERVER['HTTP_REFERER']);
		alertlocation($al);
	}
	
	$sql = "SELECT i.iusid, t.tpeid, i.iusnome 
			FROM escolaterra.identificacaousuario i 
			LEFT JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid 
			WHERE i.iuscpf='".str_replace(array(".","-"),array("",""),$dados['iuscpf_'])."'";
	
	$identificacaousuario_novo = $db->pegaLinha($sql);
	
	if($identificacaousuario_novo['tpeid']) {
		if(!$dados['noredirect']) {
	 		$al = array("alert"=>"Novo Usuário (".$identificacaousuario_novo['iusnome'].") ja possui atribuções no ESCOLA DA TERRA, por isso não pode ser inserido","location"=>$_SERVER['HTTP_REFERER']);
	 		alertlocation($al);
		} else {
			return false;
		}
	}
	
	if(!$identificacaousuario_novo['iusd']) {
		
     	$sql = "INSERT INTO escolaterra.identificacaousuario(
 	            ufpid, 
 	            iuscpf, 
 	            iusnome, 
 	            iusemailprincipal, 
 	            muncodatuacao,  
 	            iusdatainclusao, iusstatus)
 			    VALUES (".(($identificacaousuario_antigo['ufpid'])?"'".$identificacaousuario_antigo['ufpid']."'":"NULL").", 
 			    		'".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."', 
 			    		'".$dados['iusnome_']."', 
 			    		'".$dados['iusemailprincipal_']."',
 			    		".(($identificacaousuario_antigo['muncodatuacao'])?"'".$identificacaousuario_antigo['muncodatuacao']."'":"NULL").",  
 			            NOW(), 
 			            'A') returning iusid;";
     	
     	$identificacaousuario_novo['iusid'] = $db->pegaUm($sql);
     	
	} else {
		
		$sql = "UPDATE escolaterra.identificacaousuario SET iusstatus='A', 
															ufpid=".(($identificacaousuario_antigo['ufpid'])?"'".$identificacaousuario_antigo['ufpid']."'":"NULL").", 
			    WHERE iusid='".$identificacaousuario_novo['iusid']."'";
		
		$db->executar($sql);
	}
	
	$sql = "DELETE FROM escolaterra.usuarioresponsabilidade WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."'";
	$db->executar($sql);
	
	$sql = "UPDATE escolaterra.usuarioresponsabilidade SET usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."', ufpid=".(($identificacaousuario_antigo['ufpid'])?"'".$identificacaousuario_antigo['ufpid']."'":"NULL")." WHERE rpustatus='A' AND usucpf='".$identificacaousuario_antigo['usucpf']."' AND pflcod='".$identificacaousuario_antigo['pflcod']."'";
	$db->executar($sql);
	
	$sql = "UPDATE escolaterra.tipoperfil SET iusid='".$identificacaousuario_novo['iusid']."' WHERE iusid='".$identificacaousuario_antigo['iusid']."'";
	$db->executar($sql);
	
	$sql = "UPDATE escolaterra.turmas SET iusid='".$identificacaousuario_novo['iusid']."' WHERE iusid='".$identificacaousuario_antigo['iusid']."'";
	$db->executar($sql);
	
	$sql = "UPDATE escolaterra.turmaidusuario SET iusid='".$identificacaousuario_novo['iusid']."' WHERE iusid='".$identificacaousuario_antigo['iusid']."'";
	$db->executar($sql);
	
	$sql = "UPDATE escolaterra.identificacaousuario SET iusstatus='I' WHERE iusid='".$identificacaousuario_antigo['iusid']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$identificacaousuario_antigo['iuscpf']."' AND pflcod='".$dados['pflcod_']."'";
	$db->executar($sql);
	
	$sql = "INSERT INTO escolaterra.historicotrocausuario(iusidnovo, iusidantigo, pflcod, hstdata, usucpf)
    		VALUES ('".$identificacaousuario_novo['iusid']."', '".$identificacaousuario_antigo['iusid']."', '".$dados['pflcod_']."', NOW(), '".$_SESSION['usucpf']."');";
	$db->executar($sql);
	
	if($_FILES['arquivo_']['error']==0) {
		 
		$db->executar("DELETE FROM escolaterra.identificacaousuariodocumentos WHERE iusid='".$identificacaousuario_novo['iusid']."' AND iudtipo='".$dados['tipodocumentoselecao_']."'");
		 
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$campos = array("iusid" => "'".$identificacaousuario_novo['iusid']."'","iudtipo" => "'C'");
		$file = new FilesSimec( "identificacaousuariodocumentos", $campos, "escolaterra" );
		$file->setUpload( NULL, "arquivo_" );
		 
	}
	
	
	$db->commit();
	
	if(!$dados['noredirect']) {
	 	$al = array("alert"=>"Troca efetuada com sucesso.","location"=>$_SERVER['HTTP_REFERER']);
	 	alertlocation($al);
	} else {
		return true;
	}
}

function montaAbasEscolaTerra($abapai, $abaativa) {
	global $db;
	
	$sql = "SELECT abaordem, abadsc, abaendereco, abafuncaomostrar, abapai FROM escolaterra.abas WHERE abapai='".$abapai."' ORDER BY abaordem";
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

function verificaPermissao() {
	global $db;
	$perfis = pegaPerfilGeral();
	$sql = "SELECT ur.pflcod, i.ufpid FROM escolaterra.usuarioresponsabilidade ur INNER JOIN escolaterra.identificacaousuario i ON i.iuscpf = ur.usucpf WHERE usucpf='".$_SESSION['usucpf']."' AND rpustatus='A'";
	$ur = $db->carregar($sql);
	
	if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis)) {
		return false;
	}
	
	if(in_array(PFL_COORDENADORESTADUAL,$perfis)) {
		if($ur[0]) {
			foreach($ur as $urr) {
				if($urr['pflcod']==PFL_COORDENADORESTADUAL && $urr['ufpid']==$_SESSION['escolaterra']['coordenadorestadual']['ufpid']) {
					return false;
				}
			}
		}
	}
	
	if(in_array(PFL_TUTOR,$perfis)) {
		if($_SESSION['escolaterra']['tutor']['ufpid']) {
			return false;
		}
	}
	
	return true;
	
}

function carregarOrientacaoPorFiltro($dados) {
	global $db;

	$sql = "SELECT oabdesc FROM escolaterra.orientacaoaba WHERE abaid='".$dados['abaid']."'";
	$oabdesc = $db->pegaUm($sql);

	echo $oabdesc;
}


function carregarOrientacao($endereco) {
	global $db;
	
	$sql = "SELECT a.abaid, o.oabdesc FROM escolaterra.abas a 
			LEFT JOIN escolaterra.orientacaoaba o ON o.abaid = a.abaid 
			WHERE a.abaendereco='".$endereco."'";

	$abas = $db->pegaLinha($sql);
	
	$orientacao = $abas['oabdesc'];
	$abaid      = $abas['abaid'];
	
	if($db->testa_superuser()) {
		$htmladm = "<br><img src=\"../imagens/page_attach.png\" style=\"cursor:pointer;\" onclick=\"mostrarOrientacaoAdm('".$abaid."');\">";
	}
	
	return (($orientacao)?nl2br($orientacao):"Orientação não foi cadastrada").$htmladm;
	
}

function salvarOrientacaoAdm($dados) {
	global $db;

	$oabid = $db->pegaUm("SELECT oabid FROM escolaterra.orientacaoaba WHERE abaid='".$dados['abaid']."'");

	if($oabid) {

		$sql = "UPDATE escolaterra.orientacaoaba SET oabdesc='".$dados['oabdesc']."' WHERE oabid='".$oabid."'";
		$db->executar($sql);

	} else {

		$sql = "INSERT INTO escolaterra.orientacaoaba(
	            abaid, oabdesc, oabstatus)
	    		VALUES ('".$dados['abaid']."', '".$dados['oabdesc']."', 'A');";
		$db->executar($sql);

	}

	$db->commit();

	$al = array("alert"=>"Orientação gravada com sucesso.","location"=>$_SERVER['REQUEST_URI']);
	alertlocation($al);

}

function verificarEmailUnico($dados) {
	global $db;
	
	$sql = "SELECT '- '||i.iusnome||' ( '||p.pfldsc||' );' as descricao FROM escolaterra.identificacaousuario i 
			INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
			WHERE i.iusemailprincipal='".$dados['iusemailprincipal']."' AND i.iusid!='".$dados['iusid']."'";
	
	$nomes = $db->carregarColuna($sql);
	
	echo implode('\n', $nomes);
	
	
}

function verificarFormacaoCompleta($dados) {
	global $db;
	$sql = "SELECT foecompleto FROM escolaterra.formacaoescolaridade WHERE foeid='".$dados['foeid']."'";
	$foecompleto = $db->pegaUm($sql);
	echo (($foecompleto=="t")?"T":"");
	echo (($foecompleto=="f")?"F":"");
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

function listarCursosFormacao($dados) {
	global $db;
	$sql = "SELECT cufid as codigo, cufcursodesc as descricao FROM escolaterra.cursoformacao WHERE cufstatus='A' AND cufcodareageral='".$dados['cufcodareageral']."' ORDER BY cufcursodesc";
	$db->monta_combo('cufid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'cufid', '');
	
}

function atualizarNomeUsuario($dados) {
	global $db;
	
	include_once '../includes/webservice/cpf.php';
	
	$objPessoaFisica = new PessoaFisicaClient("http://ws.mec.gov.br/PessoaFisica/wsdl");
	$xml 			 = $objPessoaFisica->solicitarDadosPessoaFisicaPorCpf($dados['iuscpf']);
		
	$obj = (array) simplexml_load_string($xml);
	
	if($obj['PESSOA']->no_pessoa_rf) {
		$db->executar("UPDATE escolaterra.identificacaousuario SET iusnome='".$obj['PESSOA']->no_pessoa_rf."' WHERE iuscpf='".$dados['iuscpf']."'");
		$db->commit();
	}
	
	$al = array("alert"=>"Nome atualizado com sucesso","location"=>"escolaterra.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=dados");
	alertlocation($al);
	
}

function atualizarDadosIdentificacaoUsuario($dados) {
	global $db;
	$erros = validarIdentificacaoUsuario($dados);
	
	if($erros) {
		$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erros),"location"=>$dados['goto']);
		alertlocation($al);
	}

	$sql = "UPDATE escolaterra.identificacaousuario SET
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
			iustermocompromisso=TRUE
			WHERE iusid='".$dados['iusid']."'";
	
	$db->executar($sql);
	
	$erros = validarFormacao($dados);
	
	if($erros) {
		$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erros),"location"=>$dados['goto']);
		alertlocation($al);
	}
	

	$iufid = $db->pegaUm("SELECT iufid FROM escolaterra.identiusucursoformacao WHERE iusid='".$dados['iusid']."'");
	
	// controlando formação
	if($iufid) {
		
		$sql = "UPDATE escolaterra.identiusucursoformacao SET
		            cufid=".(($dados['cufid'])?"'".$dados['cufid']."'":"NULL").", 
		            iufdatainiformacao='".formata_data_sql($dados['iufdatainiformacao'])."', 
		            iufdatafimformacao=".(($dados['iufdatafimformacao'])?"'".formata_data_sql($dados['iufdatafimformacao'])."'":"NULL").", 
		            iufsituacaoformacao='".$dados['iufsituacaoformacao']."'
		        WHERE iufid='".$iufid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO escolaterra.identiusucursoformacao(
		            iusid, cufid, iufdatainiformacao, iufdatafimformacao, iufsituacaoformacao, 
		            iufstatus)
		    VALUES ('".$dados['iusid']."', 
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
	
	
	$itdid = $db->pegaUm("SELECT itdid FROM escolaterra.identusutipodocumento WHERE iusid='".$dados['iusid']."'");
	
	// controlando documento
	if($itdid) {
		
		$sql = "UPDATE escolaterra.identusutipodocumento SET
            	tdoid='".$dados['tdoid']."', itdufdoc='".$dados['itdufdoc']."', 
            	itdnumdoc='".$dados['itdnumdoc']."', itddataexp='".formata_data_sql($dados['itddataexp'])."', 
            	itdnoorgaoexp='".$dados['itdnoorgaoexp']."'		
		        WHERE itdid='".$itdid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO escolaterra.identusutipodocumento(
            	iusid, tdoid, itdufdoc, itdnumdoc, itddataexp, itdnoorgaoexp, itdstatus)
    			VALUES ('".$dados['iusid']."', '".$dados['tdoid']."', '".$dados['itdufdoc']."', '".$dados['itdnumdoc']."', 
    			'".formata_data_sql($dados['itddataexp'])."', '".$dados['itdnoorgaoexp']."', 'A');";
		
		$db->executar($sql);
		
	}
	
	$erros = validarEndereco($dados);
	
	if($erros) {
		$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erros),"location"=>$dados['goto']);
		alertlocation($al);
	}
	
	$ienid = $db->pegaUm("SELECT ienid FROM escolaterra.identificaoendereco WHERE iusid='".$dados['iusid']."'");
	
	// controlando endereço
	if($ienid) {
		
		$sql = "UPDATE escolaterra.identificaoendereco SET
            	muncod='".substr($dados['muncod_endereco'],0,7)."', ientipo='".$dados['ientipo']."', 
            	iencep='".str_replace(array("-"),array(""),$dados['iencep'])."', iencomplemento=".(($dados['iencomplemento'])?"'".$dados['iencomplemento']."'":"NULL").", 
            	iennumero=".((!is_null($dados['iennumero']) && is_numeric($dados['iennumero']))?"'".$dados['iennumero']."'":"NULL").", ienlogradouro='".$dados['ienlogradouro']."', 
            	ienbairro='".$dados['ienbairro']."' 		
		        WHERE ienid='".$ienid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO escolaterra.identificaoendereco(
            	muncod, iusid, ientipo, iencep, iencomplemento, iennumero, 
            	iensatatus, ienlogradouro, ienbairro)
    			VALUES ('".substr($dados['muncod_endereco'],0,7)."', '".$dados['iusid']."', '".$dados['ientipo']."', '".str_replace(array("-"),array(""),$dados['iencep'])."', 
    					".(($dados['iencomplemento'])?"'".$dados['iencomplemento']."'":"NULL").", ".((!is_null($dados['iennumero']))?"'".str_replace(array(" "),array(""),$dados['iennumero'])."'":"NULL").", 'A', '".$dados['ienlogradouro']."', '".substr($dados['ienbairro'],0,60)."');";
		
		$db->executar($sql);
		
	}
	
	// controlando telefones
	$db->executar("DELETE FROM escolaterra.identificacaotelefone WHERE iusid='".$dados['iusid']."'");
	
	$tipos = array("R","T","C","F");
	
	foreach($tipos as $tipo) {
		
		$sql = "INSERT INTO escolaterra.identificacaotelefone(
            	iusid, itedddtel, itenumtel, itetipo, itestatus)
    			VALUES ('".$dados['iusid']."', ".(($dados['itedddtel'][$tipo])?"'".$dados['itedddtel'][$tipo]."'":"NULL").", ".(($dados['itenumtel'][$tipo])?"'".$dados['itenumtel'][$tipo]."'":"NULL").", '".$tipo."', 'A');";
		
		$db->executar($sql);
		
	}
	
	$sql = "INSERT INTO escolaterra.historicoidentificaousuario(
            iusid, hiudatainc, hiucpf, hiulog, hiustatus, hiutipo)
    		VALUES ('".$dados['iusid']."', NOW(), '".$_SESSION['usucpf']."', '".str_replace(array("'"),array(""),simec_json_encode($dados))."', 'A', 'atualizarDadosIdentificacaoUsuario');";
	$db->executar($sql);
	
	$db->commit();
	
	sincronizarUsuariosSIMEC(array('cpf' => $dados['iuscpf']));
	
	$al = array("alert"=>$dados['mensagemalert'],"location"=>$dados['goto']);
	alertlocation($al);
	
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
			(SELECT itedddtel FROM escolaterra.identificacaotelefone WHERE iusid=i.iusid AND itetipo='T') as dddtel,
			u.usufoneddd,
			(SELECT itenumtel FROM escolaterra.identificacaotelefone WHERE iusid=i.iusid AND itetipo='T') as tel,
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
			p.pfldsc || ' - ESCOLA DA TERRA' as funcao_pacto,
			u.ususexo,
			i.iussexo,
			split_part(i.iusnome, ' ', 1) as apelido_pacto,
			u.usunomeguerra as apelido_segur
			FROM escolaterra.identificacaousuario i 
			INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid 
			INNER JOIN seguranca.usuario u ON u.usucpf = i.iuscpf 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
			LEFT JOIN territorios.municipio m ON m.muncod = i.muncod 
			WHERE i.iuscpf='".str_replace(array(".","-"),array("",""),$dados['cpf'])."'
			)foo WHERE foo.iuscpf = u.usucpf";
	
	$db->executar($sql);
	$db->commit();
	
}

function carregarMunicipiosPorUF($dados) {
	global $db;
	$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$dados['estuf']."' ORDER BY mundescricao";
	$combo = $db->monta_combo($dados['name'], $sql, 'S', 'Selecione', (($dados['onclick'])?$dados['onclick']:''), '', '', '200', 'S', $dados['id'], true, $dados['valuecombo']);
	
	if($dados['returncombo']) return $combo;
	else echo $combo;
}

function carregarRede($dados) {
	global $db;
	
	$arr[] = array('codigo'=>'E','descricao'=>'Estadual');
	$arr[] = array('codigo'=>'M','descricao'=>'Municipal');
	
	$combo = $db->monta_combo($dados['name'], $arr, 'S', 'Selecione', (($dados['onclick'])?$dados['onclick']:''), '', '', '200', 'S', $dados['id'], true, $dados['valuecombo']);

	if($dados['returncombo']) return $combo;
	else echo $combo;
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

function carregarDadosTurma($dados) {
	global $db;
	
	$turma = $db->pegaLinha("SELECT * FROM escolaterra.turmas WHERE iusid='".$_SESSION['escolaterra'][$dados['perfil']]['iusid']."'");
	
	if(!$turma) {
		
		$docid = wf_cadastrarDocumento( TPD_CADASTRAMENTO, 'TURMA : '.$_SESSION['escolaterra'][$dados['perfil']]['descricao'].' - #'.$_SESSION['escolaterra'][$dados['perfil']]['iusid'] );
		
		$sql = "INSERT INTO escolaterra.turmas(
	            iusid, turdesc, turstatus, docid)
	    		VALUES ('".$_SESSION['escolaterra'][$dados['perfil']]['iusid']."', 
	    				'TURMA ".$_SESSION['escolaterra'][$dados['perfil']]['descricao']." / #".$_SESSION['escolaterra'][$dados['perfil']]['iusid']."', 
	    				'A',
	    				'".$docid."') RETURNING turid;";
		
		$turma['turid']     = $db->pegaUm($sql);
		$turma['turdesc']   = "TURMA ".$_SESSION['escolaterra'][$dados['perfil']]['descricao']." / #".$_SESSION['escolaterra'][$dados['perfil']]['iusid'];
		$turma['turstatus'] = "A"; 
		$turma['docid']     = $docid;
		
		$db->commit();
		
	}
	
	return $turma;
	
}

function carregarCamposPerfil($dados) {
	global $db;
	
	$perfil = $db->pegaLinha("SELECT * FROM seguranca.perfil WHERE pflcod='".$dados['pflcod']."'");
	
	echo $perfil['pfldsc']."<input type=hidden name=\"pflcod_\" id=\"pflcod_\" value=\"".$perfil['pflcod']."\">";
}

function efetuarInsercaoUsuarioPerfil($dados) {
	global $db;
	
	$iusid = $db->pegaUm("SELECT iusid FROM escolaterra.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array("",""),$dados['iuscpf_'])."'");
	
	if($iusid) {
		
		$sql = "UPDATE escolaterra.identificacaousuario SET 
				ufpid   =".(($dados['ufpid_'])?"'".$dados['ufpid_']."'":"NULL").", 
	    		iuscpf  =".(($dados['iuscpf_'])?"'".str_replace(array(".","-"),array("",""),$dados['iuscpf_'])."'":"NULL").", 
	    		iusnome =".(($dados['iusnome_'])?"'".$dados['iusnome_']."'":"NULL").", 
	    		iusemailprincipal =".(($dados['iusemailprincipal_'])?"'".$dados['iusemailprincipal_']."'":"NULL").", 
            	muncodatuacao =".(($dados['muncod_endereco'])?"'".$dados['muncod_endereco']."'":"NULL").",
	    		iusstatus ='A' 
				WHERE iusid='".$iusid."'";
		
		$db->executar($sql);
		
	} else {
	
		$sql = "INSERT INTO escolaterra.identificacaousuario(
	            ufpid, iuscpf, iusnome, 
	            iusemailprincipal, iusdatainclusao,  
	            iusstatus, muncodatuacao)
	    VALUES (".(($dados['ufpid_'])?"'".$dados['ufpid_']."'":"NULL").", 
	    		".(($dados['iuscpf_'])?"'".str_replace(array(".","-"),array("",""),$dados['iuscpf_'])."'":"NULL").", 
	    		".(($dados['iusnome_'])?"'".$dados['iusnome_']."'":"NULL").", 
	    		".(($dados['iusemailprincipal_'])?"'".$dados['iusemailprincipal_']."'":"NULL").", 
	    		NOW(), 
	    		'A', ".(($dados['muncod_endereco'])?"'".$dados['muncod_endereco']."'":"NULL").") RETURNING iusid";
		
		$iusid = $db->pegaUm($sql);
	
	}
	
	$sql = "INSERT INTO escolaterra.identificacaousuarioescola(
            iusid, iuecodigoinep)
			SELECT i.iusid, e.fk_cod_entidade FROM escolaterra.identificacaousuario i 
			INNER JOIN educacenso_2013.tab_docente d ON d.num_cpf = i.iuscpf
			INNER JOIN educacenso_2013.tab_docente_entidade e ON e.fk_cod_docente = d.pk_cod_docente 
			WHERE i.iusid='".$iusid."'";
	
	$db->executar($sql);
	
	$sql = "SELECT p.pfldsc, 
				   p.pflcod 
			FROM escolaterra.tipoperfil t 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.iusid='".$iusid."'";
	
	$arrPf = $db->pegaLinha($sql);
	
	if($arrPf['pfldsc'] && $arrPf['pflcod']!=$dados['pflcod_']) {
		
		$al = array("alert"=>"Inserção não efetivada com sucesso. O usuário cadastrado com o perfil : ".$arrPf['pfldsc'],"location"=>$_SERVER['REQUEST_URI']);
		alertlocation($al);
		
	}
	
	$tpeid = $db->pegaUm("SELECT tpeid FROM escolaterra.tipoperfil WHERE iusid='".$iusid."'");
	
	if(!$tpeid) {
		
		if(!$dados['pflcod_']) {
			$db->rollback();
			$al = array("alert"=>"Perfil não foi selecionado, tente novamente.","location"=>$_SERVER['REQUEST_URI']);
			alertlocation($al);
		}
		
		$sql = "INSERT INTO escolaterra.tipoperfil(iusid, pflcod)
	    		VALUES ('".$iusid."', '".$dados['pflcod_']."');";
		
		$db->executar($sql);
		
	}
	
	if($dados['turid_']) {
		
		$tiuid = $db->pegaUm("SELECT tiuid FROM escolaterra.turmaidusuario WHERE iusid='".$iusid."'");
		
		if(!$tiuid) {
			
			$sql = "INSERT INTO escolaterra.turmaidusuario(
	            	turid, iusid)
	    			VALUES ('".$dados['turid_']."', '".$iusid."');";
	
			$db->executar($sql);
			
		} else {
			
			$sql = "UPDATE escolaterra.turmaidusuario SET turid='".$dados['turid_']."' WHERE iusid='".$iusid."';";
			$db->executar($sql);
			
		}
	}
	
    if($_FILES['arquivo']['error']==0) {
    	
    	$db->executar("DELETE FROM escolaterra.identificacaousuariodocumentos WHERE iusid='".$iusid."' AND iudtipo='".$dados['tipodocumentoselecao']."'");
    	
	   	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	    $campos = array("iusid" => "'".$iusid."'","iudtipo" => "'C'");
	    $file = new FilesSimec( "identificacaousuariodocumentos", $campos, "escolaterra" );
	    $file->setUpload( NULL, "arquivo" );
	    
    }

	
	$db->commit();
	
	
	$al = array("alert"=>"Inserção efetivada com sucesso","location"=>$_SERVER['REQUEST_URI']);
	alertlocation($al);
	
	
}

function atualizarEmail($dados) {
	global $db;
	
	$sql = "UPDATE escolaterra.identificacaousuario SET iusemailprincipal='".$dados['iusemailprincipal']."' WHERE iusid='".$dados['iusid']."'";
	$db->executar($sql);
	$db->commit();
	
}

function reiniciarSenha($dados) {
	global $db;
	
	$sql = "UPDATE seguranca.usuario SET ususenha='".md5_encrypt_senha("simecdti","")."' WHERE usucpf='".$dados['usucpf']."'";
	$db->executar($sql);
	
	$sql = "UPDATE seguranca.usuario_sistema SET suscod='A' WHERE usucpf='".$dados['usucpf']."' AND sisid='".SIS_ESCOLATERRA."'";
	$db->executar($sql);
	
	$db->commit();
	
	$arrUsu = $db->pegaLinha("SELECT usunome, usuemail FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['usucpf'])."'");
	
	$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO ESCOLA DA TERRA","email" => $arrUsu['usuemail']);
 	$destinatario = $arrUsu['usuemail'];
 	$usunome = $arrUsu['usunome'];
 	
 	$assunto = "Atualização de senha no SIMEC - MÓDULO ESCOLA DA TERRA";
 	$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
	$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo ESCOLA DA TERRA. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
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
	
	$al = array("alert"=>"Senha reiniciada com sucesso","location"=>"escolaterra.php?modulo=".$dados['modulo']."&acao=A&aba=".$dados['aba']);
	alertlocation($al);
	
	
}


function carregarHistoricoUsuario($dados) {
	global $db;
	
	$sql = "SELECT us.usunome, to_char(htudata,'dd/mm/YYYY HH24:MI') as data, hu.htudsc, hu.suscod, us2.usunome as resp FROM seguranca.historicousuario hu 
			INNER JOIN seguranca.usuario us ON us.usucpf = hu.usucpf 
			LEFT JOIN seguranca.usuario us2 ON us2.usucpf = hu.usucpfadm
			WHERE hu.usucpf='".$dados['usucpf']."' AND hu.sisid='".SIS_ESCOLATERRA."' ORDER BY htudata DESC";
	
	$cabecalho = array("Nome","Data","Justificativa","Situação","Responsável");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','N',true);
	
}

function ativarEquipe($dados) {
	global $db;
	
	if($dados['chk']) {
		
		foreach($dados['chk'] as $pflcod => $cpfs) {
			
			foreach($cpfs as $cpf) {
				
				$sql = "SELECT * FROM escolaterra.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$cpf)."'";
 				$identificacaousuario = $db->pegaLinha($sql);

			    $existe_usu = $db->pegaUm("SELECT usucpf FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."'");
    	
   				if(!$existe_usu) {
    	
				   	$sql = "INSERT INTO seguranca.usuario(
			             	usucpf, usunome, usuemail, usustatus, ususenha, suscod)
			     			VALUES ('".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', '".addslashes($identificacaousuario['iusnome'])."', '".$identificacaousuario['iusemailprincipal']."', 'A', '".md5_encrypt_senha("simecdti","")."', 'A');";
			     	$db->executar($sql);
    	
			    } else {
    	
			    	$sql = "UPDATE seguranca.usuario SET usustatus='A', suscod='A', usuemail='".$identificacaousuario['iusemailprincipal']."' WHERE usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."'";
    				$db->executar($sql);
			    }
			    
		 		$remetente = array("nome" => SIGLA_SISTEMA. " - MÓDULO ESCOLA DA TERRA","email" => $identificacaousuario['iusemailprincipal']);
 				$destinatario = $identificacaousuario['iusemailprincipal'];
 				$usunome = $identificacaousuario['iusnome'];
 				$assunto = "Cadastro no SIMEC - MÓDULO ESCOLA DA TERRA";
 				$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
		 		$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo ESCOLA DA TERRA. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
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
		 		
			    $existe_sis = $db->pegaUm("SELECT usucpf FROM seguranca.usuario_sistema WHERE usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and sisid='".SIS_ESCOLATERRA."'");
			    	
			    if(!$existe_sis) {
			    		
			    	$sql = "INSERT INTO seguranca.usuario_sistema(
			         	    usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
			     			VALUES ('".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', ".SIS_ESCOLATERRA.", 'A', NULL, NOW(), 'A');";
				    	
			     	$db->executar($sql);
				    	
			    } else {
		 	    	$sql = "UPDATE seguranca.usuario_sistema SET susstatus='A', suscod='".$dados['suscod']."' WHERE usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' AND sisid='".SIS_ESCOLATERRA."'";
		 	    	$db->executar($sql);
			    }
			    
			    $sql = "INSERT INTO seguranca.historicousuario(htudsc, htudata, usucpf, sisid, suscod, usucpfadm)
    					VALUES ('Mudança realizada pela ferramenta de gerencia do ESCOLA DA TERRA.', 
    							NOW(), 
    							'".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', 
    							'".$_SESSION['sisid']."', '".$dados['suscod']."', '".$_SESSION['usucpf']."');";
			    
	 	    	$db->executar($sql);
			    
			    $existe_pfl = $db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' AND pflcod='".$pflcod."'");
    	
			    if(!$existe_pfl) {
			    	$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', '".$pflcod."');";
			     	$db->executar($sql);
			    }

			    $rpuid = $db->pegaUm("SELECT rpuid FROM escolaterra.usuarioresponsabilidade WHERE usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' AND pflcod='".$pflcod."' AND rpustatus='A'");
			    
			    if($dados['ufpid']) {
	    
				    if(!$rpuid) {
				    	$sql = "INSERT INTO escolaterra.usuarioresponsabilidade(
				            		pflcod, usucpf, rpustatus, rpudata_inc, ufpid)
				 			    VALUES ('".$pflcod."', '".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', 'A', NOW(), '".$dados['ufpid']."');";
				    	
				    	$db->executar($sql);
				    } else {
				    	$sql = "UPDATE escolaterra.usuarioresponsabilidade SET ufpid='".$dados['ufpid']."' WHERE rpuid='".$rpuid."'";
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

function removerIdentificacaoPerfil($dados) {
	global $db;
	
	$sql = "DELETE FROM escolaterra.tipoperfil WHERE iusid='".$dados['iusid']."' AND pflcod='".$dados['pflcod']."'";
	$db->executar($sql);
	$sql = "UPDATE escolaterra.identificacaousuario SET iusstatus='I' WHERE iusid='".$dados['iusid']."'";
	$db->executar($sql);
	$db->commit();
	
 	$al = array("alert"=>"Gerenciamento executado com sucesso","location"=>"/escolaterra/escolaterra.php?modulo=".$dados['modulo']."&acao=A&aba=".$dados['aba']);
 	alertlocation($al);
	
}

function verificarValidacaoCadastramento($dados) {
	global $db;
	
	if(!$dados['iusid']) {
		$al = array("alert"=>"Bolsista não identificado. Tente novamente.","location"=>"escolaterra.php?modulo=inicio&acao=C");
		alertlocation($al);
	}
	
	$esdid = $db->pegaUm("SELECT d.esdid 
						 FROM escolaterra.turmas t 
						 INNER JOIN workflow.documento d ON d.docid = t.docid 
						 WHERE t.iusid='".$dados['iusid']."'");
	
	if($esdid == ESD_VALIDADO) return true;
	else return false;
	
}

function verificarAceitacaoTermoCompromisso($dados) {
	global $db;
	
	$iustermocompromisso = $db->pegaUm("SELECT iustermocompromisso FROM escolaterra.identificacaousuario WHERE iusid='".$dados['iusid']."'");
	
	if($iustermocompromisso=='t') return true;
	else return false;
	
}

function validarCadastramentoTurmas($dados) {
	global $db;
	
	if($dados['tur']) {
		foreach($dados['tur'] as $docid => $aedid) {
			if($aedid) wf_alterarEstado( $docid, $aedid, $dados['cmddsc'][$docid], $dados);
		}
	}
	
	$al = array("alert"=>"Validação efetuada com sucesso","location"=>"escolaterra.php?modulo=principal/coordenadorestadual/coordenadorestadual&acao=A&aba=validacaoprofessores");
	alertlocation($al);
	
}

function validarRelatorioTutores($dados) {
	global $db;
	if($dados['rel']) {
		foreach($dados['rel'] as $docid => $aedid) {
			$_SESSION['escolaterra']['tutor']['iusid'] = $db->pegaUm("SELECT iusid FROM escolaterra.relatorioacompanhamento WHERE docid='{$docid}'");
			if($aedid) wf_alterarEstado( $docid, $aedid, $dados['cmddsc'][$docid], $dados);
		}
	}
	
	$al = array("alert"=>"Validação efetuada com sucesso","location"=>"escolaterra.php?modulo=principal/coordenadorestadual/coordenadorestadual&acao=A&aba=analisetutores&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}


function carregarPeriodoReferencia($dados) {
	global $db;
	$sql = "SELECT f.fpbid as codigo, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao 
			FROM escolaterra.periodoreferencia f 
			INNER JOIN escolaterra.periodoreferenciauf rf ON rf.fpbid = f.fpbid 
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE f.fpbstatus='A' AND rf.ufpid='".$dados['ufpid']."' AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd') 
 			ORDER BY (fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date";
	
	
	if(!$dados['somentecombo']) echo "Selecione período de referência : ";
	$db->monta_combo('fpbid', $sql, 'S', 'Selecione', 'selecionarPeriodoReferencia', '', '', '', 'S', 'fpbid','', $dados['fpbid']);
	
}

function gravarAcompanhamentoProfessor($dados) {
	global $db;
	
	if(isset($dados['acoexistetempouniversidade'])) {
		$campos[] = 'acoexistetempouniversidade';
		$values[] = $dados['acoexistetempouniversidade'];
	}
	
	if(isset($dados['acorecebeuformacao'])) {
		$campos[] = 'acorecebeuformacao';
		$values[] = $dados['acorecebeuformacao'];
	}
	
	if(isset($dados['acorecebeuformacaojustificativa'])) {
		$campos[] = 'acorecebeuformacaojustificativa';
		$values[] = (($dados['acorecebeuformacaojustificativa'])?"'".$dados['acorecebeuformacaojustificativa']."'":"NULL");
	}
	
	if(isset($dados['acocargahorariauniversidade'])) {
		$campos[] = 'acocargahorariauniversidade';
		$values[] = (($dados['acocargahorariauniversidade'])?"'".$dados['acocargahorariauniversidade']."'":"NULL");
	}
	
	if(isset($dados['acoconteudosdesenvolvidosformacao'])) {
		$campos[] = 'acoconteudosdesenvolvidosformacao';
		$values[] = (($dados['acoconteudosdesenvolvidosformacao'])?"'".substr($dados['acoconteudosdesenvolvidosformacao'],0,1000)."'":"NULL");
	}
	
	if(isset($dados['acotrabalhandoefetivamente'])) {
		$campos[] = 'acotrabalhandoefetivamente';
		$values[] = "'".$dados['acotrabalhandoefetivamente']."'";
	}
	
	if(isset($dados['acotrabalhandoefetivamenteobservacao'])) {
		$campos[] = 'acotrabalhandoefetivamenteobservacao';
		$values[] = (($dados['acotrabalhandoefetivamenteobservacao'])?"'".substr(addslashes($dados['acotrabalhandoefetivamenteobservacao']),0,1000)."'":"NULL");
	}
	
	if(isset($dados['acoevolucaoaprendizagem'])) {
		$campos[] = 'acoevolucaoaprendizagem';
		$values[] = "'".$dados['acoevolucaoaprendizagem']."'";
	}
	
	if(isset($dados['acoevolucaoaprendizagemobservacao'])) {
		$campos[] = 'acoevolucaoaprendizagemobservacao';
		$values[] = (($dados['acoevolucaoaprendizagemobservacao'])?"'".substr($dados['acoevolucaoaprendizagemobservacao'],0,1000)."'":"NULL");
	}
	
	if(isset($dados['acousodosmateriais'])) {
		$campos[] = 'acousodosmateriais';
		$values[] = "'".$dados['acousodosmateriais']."'";
	}
	
	if(isset($dados['acousodosmateriaisobservacao'])) {
		$campos[] = 'acousodosmateriaisobservacao';
		$values[] = (($dados['acousodosmateriaisobservacao'])?"'".substr($dados['acousodosmateriaisobservacao'],0,1000)."'":"NULL");
	}
	
	$acoid = $db->pegaUm("SELECT acoid FROM escolaterra.acompanhamentoprofessores WHERE iusidprofessor='".$dados['iusidprofessor']."' AND fpbid='".$dados['fpbid']."'");
	
	if($acoid) {
		
		if(isset($_SESSION['escolaterra']['tutor']['iusid'])) {
			$campos[] = 'iusidtutor';
			$values[] = (($_SESSION['escolaterra']['tutor']['iusid'])?"'".$_SESSION['escolaterra']['tutor']['iusid']."'":"NULL");
		}
		
		if($campos) {
			foreach($campos as $key => $campo) {
				$set[] = $campo."=".$values[$key];
			}
		}
		
		$sql = "UPDATE escolaterra.acompanhamentoprofessores SET ".implode(",",$set)." WHERE acoid='".$acoid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO escolaterra.acompanhamentoprofessores(
			            fpbid, 
			            iusidtutor, 
			            iusidprofessor,
			            ".(($campos)?implode(",",$campos):"")." 
			            )
			    VALUES ('".$dados['fpbid']."', 
			    		'".$_SESSION['escolaterra']['tutor']['iusid']."', 
			    		'".$dados['iusidprofessor']."', 
			    		".(($values)?implode(",",$values):"")."
			    		);";
		
		$db->executar($sql);
	}
	
	$db->commit();
	
	$al = array("alert"=>"Acompanhamento gravado com sucesso","location"=>"escolaterra.php?modulo=principal/tutor/tutorexecucao&acao=A&aba=acompanharprofessores&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}


function condicaoEnviarAnalise($tipo) {

	$funcao = 'condicao_'.$tipo;
	return $funcao();
	
}

function condicao_coordenadorestadual() {
	global $db;
	
	$total = $db->pegaUm("SELECT count(*) as total
							FROM escolaterra.identificacaousuario i 
							INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid AND t.pflcod=".PFL_TUTOR." 
							INNER JOIN escolaterra.turmaidusuario u ON u.iusid = i.iusid 
							INNER JOIN escolaterra.turmas tu ON tu.turid = u.turid  
							WHERE tu.iusid='".$_SESSION['escolaterra']['coordenadorestadual']['iusid']."'");
	
	if($total==0) {
		return 'É obrigatório cadastrar pelo menos 1 TUTOR';
	}
	
	return true;
	
}

function condicao_tutor() {
	global $db;
	
	$total = $db->pegaUm("SELECT count(*) as total
							FROM escolaterra.identificacaousuario i 
							INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid AND t.pflcod=".PFL_PROFESSOR." 
							INNER JOIN escolaterra.turmaidusuario u ON u.iusid = i.iusid 
							INNER JOIN escolaterra.turmas tu ON tu.turid = u.turid  
							WHERE tu.iusid='".$_SESSION['escolaterra']['tutor']['iusid']."'");
	
	if($total==0) {
		return 'É obrigatório cadastrar pelo menos 1 PROFESSOR';
	}
	
	return true;
	
}

function carregarAcompanhamentoProfessores($dados) {
	global $db;
	
	$acompanhamentoprofessor = $db->pegaLinha("SELECT a.* FROM escolaterra.acompanhamentoprofessores a WHERE iusidprofessor='".$dados['iusidprofessor']."' AND fpbid='".$dados['fpbid']."'");
	$acompanhamentoprofessor['iusnome'] = $db->pegaUm("SELECT iusnome FROM escolaterra.identificacaousuario WHERE iusid='".$dados['iusidprofessor']."'");
	
	if($acompanhamentoprofessor) 
		echo simec_json_encode($acompanhamentoprofessor);
	
}

function efetuarDownload($dados) {
	global $db;
	
	$arqid = $db->pegaUm("SELECT arqid FROM escolaterra.identificacaousuariodocumentos WHERE iusid='".$dados['iusid']."' AND iudtipo='C'");
	
	if($arqid) {
	
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$file = new FilesSimec( "identificacaousuariodocumentos", NULL, "escolaterra" );
		$file->getDownloadArquivo( $arqid );
	
	} else {
		
		$al = array("alert"=>"Arquivo não foi inserido corretamente","location"=>"escolaterra.php?modulo=".$dados['modulo']."&acao=A&aba=cadastro");
		alertlocation($al);
		
		
	}
	
}

function salvarMunicipioAtuacao($dados) {
	global $db;
	
	$db->executar("UPDATE escolaterra.identificacaousuario SET muncodatuacao='".$dados['muncodatuacao']."' WHERE iusid='".$dados['iusid_atuacao']."'");
	$db->commit();
	
	$al = array("alert"=>"Município de atuação foi atualizado com sucesso","location"=>"escolaterra.php?modulo=".$dados['modulo']."&acao=A&aba=cadastro");
	alertlocation($al);
	

}

function salvarRedeAtuacao($dados) {
	global $db;

	$db->executar("UPDATE escolaterra.identificacaousuario SET iusrede='".$dados['iusrede']."' WHERE iusid='".$dados['iusid_atuacao']."'");
	$db->commit();

	$al = array("alert"=>"Rede de atuação foi atualizado com sucesso","location"=>"escolaterra.php?modulo=".$dados['modulo']."&acao=A&aba=cadastro");
	alertlocation($al);

}

function enviarRelatorioAnalise($fpbid) {
	global $db;

	$sql = "SELECT i.iusnome FROM escolaterra.identificacaousuario i 
    		INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid 
    		INNER JOIN escolaterra.turmaidusuario ti ON ti.iusid = i.iusid 
    		INNER JOIN escolaterra.turmas tu ON tu.turid = ti.turid 
    		LEFT JOIN escolaterra.acompanhamentoprofessores a ON a.iusidprofessor = i.iusid AND a.fpbid={$fpbid}
    		WHERE t.pflcod='".PFL_PROFESSOR."' AND tu.iusid='".$_SESSION['escolaterra']['tutor']['iusid']."' AND (a.acoexistetempouniversidade IS NULL OR a.acotrabalhandoefetivamente IS NULL)";
	
	$professoressemaval = $db->carregarColuna($sql);
	
	if($professoressemaval) {
		return 'Existem Professores sem avaliação: \n\n'.implode('\n<br>',$professoressemaval);
	}
	
	$sql = "SELECT racid FROM escolaterra.relatorioacompanhamento 
			WHERE iusid='".$_SESSION['escolaterra']['tutor']['iusid']."' AND 
			      fpbid='".$fpbid."' AND 
			   	  (ractutacoesacompanhamento IS NULL OR ractutdificuldadesencontradas IS NULL OR ractutobservacoes IS NULL OR ractutavancospraticasobservadas IS NULL)";
	
	$relatorioacompanhamento = $db->pegaUm($sql);
	
	if($relatorioacompanhamento) {
		return 'Relatório de acompanhamento não foi preenchido';
	}
	
	return true;

}

function enviarRelatorioAnaliseCoordenadorEstadual($fpbid) {
	global $db;


	$sql = "SELECT raeid FROM escolaterra.relatorioacompanhamentocoordenadorestadual
			WHERE iusid='".$_SESSION['escolaterra']['coordenadorestadual']['iusid']."' AND
					fpbid='".$fpbid."' AND
			   	  (raerealizouacaoescolaterra IS NULL OR raetempoformacao IS NULL OR raeprincipaisatividades IS NULL OR raeparticipouencontro IS NULL)";

	$relatorioacompanhamento = $db->pegaUm($sql);

	if($relatorioacompanhamento) {
	return 'Relatório de acompanhamento não foi preenchido';
	}

	return true;

}

function carregarRelatorioTutor($dados) {
	global $db;
	
	echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
	
	
	$tutornome = $db->pegaUm("SELECT iusnome FROM escolaterra.identificacaousuario WHERE iusid='".$dados['iusid']."'");
	
	$sql = "SELECT * FROM escolaterra.relatorioacompanhamento WHERE iusid='".$dados['iusid']."' AND fpbid='".$dados['fpbid']."'";
	$relatorioacompanhamento = $db->pegaLinha($sql);
	
	echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="20%">Nome do tutor:</td>';
	echo '<td colspan=3><span style=font-size:large;>'.$tutornome.'</span></td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td colspan="4" class="SubTituloCentro">Descrição das Atividades Realizadas de Acompanhamento Pedagógico</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="20%">1. Ações de Acompanhamento:</td>';
	echo '<td>';
	echo (($relatorioacompanhamento['ractutacoesacompanhamento'])?$relatorioacompanhamento['ractutacoesacompanhamento']:'<span style=color:red;>Não preenchido</span>');
	echo '<td>';
	echo '<td rowspan="5" valign="top" width="5%">';
	
	$_SESSION['escolaterra']['tutor']['iusid'] = $dados['iusid'];
	/* Barra de estado atual e ações e Historico */
	wf_desenhaBarraNavegacao( $relatorioacompanhamento['docid'], array('fpbid' => $dados['fpbid'],'iusid' => $dados['iusid']) );
	
	echo '</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="20%">2. Dificuldades enfrentadas:</td>';
	echo '<td>';
	echo (($relatorioacompanhamento['ractutdificuldadesencontradas'])?$relatorioacompanhamento['ractutdificuldadesencontradas']:'<span style=color:red;>Não preenchido</span>');
	echo '</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="20%">3. Observações:</td>';
	echo '<td>';
	echo (($relatorioacompanhamento['ractutobservacoes'])?$relatorioacompanhamento['ractutobservacoes']:'<span style=color:red;>Não preenchido</span>');
	echo '</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="20%">4. Avanços nas Práticas Observadas:</td>';
	echo '<td>';
	echo (($relatorioacompanhamento['ractutavancospraticasobservadas'])?$relatorioacompanhamento['ractutavancospraticasobservadas']:'<span style=color:red;>Não preenchido</span>');
	echo '</td>';
	echo '</tr>';
	echo '</table>';
	
	$sql = "SELECT replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')||' / '||i.iusnome as professor, a.acoevolucaoaprendizagemobservacao, a.acoevolucaoaprendizagem, a.acousodosmateriais, a.acousodosmateriaisobservacao, a.acotrabalhandoefetivamenteobservacao, a.acotrabalhandoefetivamente, a.acoexistetempouniversidade, a.acoconteudosdesenvolvidosformacao, a.acocargahorariauniversidade, a.acorecebeuformacao, a.acorecebeuformacaojustificativa FROM escolaterra.acompanhamentoprofessores a 
			INNER JOIN escolaterra.identificacaousuario i ON i.iusid = a.iusidprofessor 
			WHERE iusidtutor='".$dados['iusid']."' AND fpbid='".$dados['fpbid']."'";

	$acompanhamentoprofessores = $db->carregar($sql);
	
	echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
	if($acompanhamentoprofessores[0]) {

		foreach($acompanhamentoprofessores as $acp) {
			echo '<tr>';
			echo '<td style=font-size:large; colspan=2>'.$acp['professor'].'</td>';
			echo '</tr>';
			echo '<tr>';
			echo '<tr>';
			echo '<td valign=top width=50%>';
					
			echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
			echo '<tr>';
			echo '<td class="SubTituloCentro" colspan=2>Tempo Universidade</td>';
			echo '</tr>';
			
			if($acp['acoexistetempouniversidade']=='t') $acoexistetempouniversidade = 'Sim';
			if($acp['acoexistetempouniversidade']=='f') $acoexistetempouniversidade = 'Não';
			if(!$acp['acoexistetempouniversidade']) $acoexistetempouniversidade = 'Não respondida';
			
			echo '<tr>
					<td class="SubTituloDireita" width=50%><span style=font-size:x-small;>Neste período ocorreu atividades na Universidade?</span></td>
					<td><span style=font-size:x-small;>'.$acoexistetempouniversidade.'</span></td>
				  </tr>';
			
			if($acp['acoexistetempouniversidade']=='t') {

				if($acp['acorecebeuformacao']=='t') $acorecebeuformacao = 'Sim';
				if($acp['acorecebeuformacao']=='f') $acorecebeuformacao = 'Não';

				echo '<tr><td colspan="2"><table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
				echo '<tr><td class="SubTituloDireita" width="40%"><span style=font-size:x-small;>Esse Professor recebeu formação do Escola da Terra na Universidade?</span></td><td><span style=font-size:x-small;>'.$acorecebeuformacao.'</span></td></tr>';
				
				if($acp['acorecebeuformacao']=='f') {
					echo '<tr><td class="SubTituloDireita"><span style=font-size:x-small;>Justifique:</td><td><span style=font-size:x-small;>'.$acp['acorecebeuformacaojustificativa'].'</span></td></tr>';
				}
				
				echo '<tr><td class="SubTituloDireita"><span style=font-size:x-small;>Carga Horária Trabalhada na Universidade:</span></td><td><span style=font-size:x-small;>'.$acp['acocargahorariauniversidade'].'</span></td></tr>';
				echo '<tr><td class="SubTituloDireita"><span style=font-size:x-small;>Conteúdos desenvolvidos na Formação:</span></td><td><span style=font-size:x-small;>'.$acp['acoconteudosdesenvolvidosformacao'].'</span></td></tr>';
				
				echo '</table></td></tr>';
			}
			echo '</table>';
			
			echo '</td>';
			
			
			echo '<td valign=top width=50%>';
			
			echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
			echo '<tr>';
			echo '<td class="SubTituloCentro" colspan=2>Tempo Escola Comunidade</td>';
			echo '</tr>';
			echo '<tr>';
			echo '<td class="SubTituloDireita" width="40%"><span style=font-size:x-small;>O professor trabalhou adequadamente com as sugestões e orientações recebidas na formação?</td>';
			
			switch($acp['acotrabalhandoefetivamente']) {
				case 'S':$acotrabalhandoefetivamente="Sim";break;
				case 'N':$acotrabalhandoefetivamente="Não";break;
				case 'E':$acotrabalhandoefetivamente="Em parte";break;
			}
			
			echo '<td><span style=font-size:x-small;>'.(($acotrabalhandoefetivamente)?$acotrabalhandoefetivamente:'<span style=color:red;>Não preenchido</span>').'</td>';
			echo '</tr>';
			echo '<tr>';
			echo '<td class="SubTituloDireita"><span style=font-size:x-small;>Observações:</td>';
			echo '<td><span style=font-size:x-small;>';
			echo (($acp['acotrabalhandoefetivamenteobservacao'])?$acp['acotrabalhandoefetivamenteobservacao']:'<span style=color:red;>Não preenchido</span>');
			echo '</td>';
			echo '</tr>';
			echo '<tr>';
			echo '<td class="SubTituloDireita"><span style=font-size:x-small;>Uso dos materiais (Kits)?</td>';
			
			switch($acp['acousodosmateriais']) {
				case 'S':$acousodosmateriais="Sim";break;
				case 'N':$acousodosmateriais="Não";break;
				case 'E':$acousodosmateriais="Em parte";break;
			}
			
			echo '<td><span style=font-size:x-small;>'.(($acousodosmateriais)?$acousodosmateriais:'<span style=color:red;>Não preenchido</span>').'</td>';
			echo '</tr>';
			echo '<tr>';
			echo '<td class="SubTituloDireita"><span style=font-size:x-small;>Observações:</td>';
			echo '<td><span style=font-size:x-small;>';
			echo (($acp['acousodosmateriaisobservacao'])?$acp['acousodosmateriaisobservacao']:'<span style=color:red;>Não preenchido</span>');
			echo '</td>';
			echo '</tr>';
			echo '<tr>';
			echo '<td class="SubTituloDireita"><span style=font-size:x-small;>Há perspectiva de evolução da aprendizagem dos estudantes?</td>';
			switch($acp['acoevolucaoaprendizagem']) {
				case 'S':$acoevolucaoaprendizagem="Sim";break;
				case 'N':$acoevolucaoaprendizagem="Não";break;
				case 'E':$acoevolucaoaprendizagem="Em parte";break;
			}
			echo '<td><span style=font-size:x-small;>'.(($acoevolucaoaprendizagem)?$acoevolucaoaprendizagem:'<span style=color:red;>Não preenchido</span>').'</td>';
			echo '</tr>';
			
			echo '<tr>';
			echo '<td class="SubTituloDireita"><span style=font-size:x-small;>Observações:</td>';
			echo '<td><span style=font-size:x-small;>';
			echo (($acp['acoevolucaoaprendizagemobservacao'])?$acp['acoevolucaoaprendizagemobservacao']:'<span style=color:red;>Não preenchido</span>');
			echo '</td>';
			echo '</tr>';

			echo '</table>';
			
			echo '</td>';
			
			echo '</tr>';
		}
		
	} else {
		echo '<tr>';
		echo '<td class="SubTituloCentro">Não existem acompanhamentos dos professores</td>';
		echo '</tr>';
	}
	echo '</table>';

}


function carregarRelatorioCoordenadorEstadual($dados) {
	global $db;

	echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';


	$coordenadorestadualnome = $db->pegaUm("SELECT iusnome FROM escolaterra.identificacaousuario WHERE iusid='".$dados['iusid']."'");

	$sql = "SELECT * FROM escolaterra.relatorioacompanhamentocoordenadorestadual WHERE iusid='".$dados['iusid']."' AND fpbid='".$dados['fpbid']."'";
	$relatorioacompanhamento = $db->pegaLinha($sql);

	echo '<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="20%">Nome do Coordenador Estadual:</td>';
	echo '<td colspan=3><span style=font-size:large;>'.$coordenadorestadualnome.'</span></td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="30%">1. Você realizou ação para a Escola da Terra esse mês?</td>';
	
	switch($relatorioacompanhamento['raerealizouacaoescolaterra']) {
		case 'S':$raerealizouacaoescolaterra="Sim";break;
		case 'N':$raerealizouacaoescolaterra="Não";break;
	}
	
	echo '<td>'.$raerealizouacaoescolaterra.'</td>';
	echo '<td rowspan="3" valign="top" width="5%">';
	/* Barra de estado atual e ações e Historico */
	wf_desenhaBarraNavegacao( $relatorioacompanhamento['docid'], array('iusid' => $dados['iusid'], 'fpbid' => $_REQUEST['fpbid']) );
	echo '</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="30%">2. Em que tempo de formação</td>';
	
	switch($relatorioacompanhamento['raetempoformacao']) {
		case 'U':$raetempoformacao="Universidade";break;
		case 'C':$raetempoformacao="Comunidade";break;
		case 'A':$raetempoformacao="Ambos";break;
	}
	
	echo '<td>'.$raetempoformacao.'</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="30%">3. Descreva, sinteticamente,  as principais atividades realizadas</td>';
	echo '<td>'.$relatorioacompanhamento['raeprincipaisatividades'].'</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="30%">4. Participou de encontro presencial na/com a Universidade?</td>';
	
	switch($relatorioacompanhamento['raeparticipouencontro']) {
		case 'S':$raeparticipouencontro="Sim";break;
		case 'N':$raeparticipouencontro="Não";break;
	}
	
	echo '<td>'.$raeparticipouencontro.'</td>';
	echo '</tr>';
		
	echo '<tr id="td_encontropresencial">';
	echo '<td colspan="2">';
	echo '<br>';
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="30%">Em que data ocorreu o encontro presencial?</td>';
	echo '<td>'.$relatorioacompanhamento['raedataencontro'].'</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="30%">Nesse encontro presencial teve participação de:</td>';
	
	switch($relatorioacompanhamento['raeparticipacaoencontro']) {
		case 'T':$raeparticipacaoencontro="Tutores";break;
		case 'C':$raeparticipacaoencontro="Cursistas";break;
		case 'A':$raeparticipacaoencontro="Ambos";break;
	}
	
	echo '<td>'.$raeparticipacaoencontro.'</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="30%">Descreva, resumidamente, algumas das atividades ocorridas nesse encontro presencial</td>';
	echo '<td>'.$relatorioacompanhamento['raeatividadesocorridasencontro'].'</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="30%">De modo geral,  como você avalia  a satisfação/entusiasmo  dos tutores e/ou cursistas  nesse encontro presencial ?</td>';
	
	switch($relatorioacompanhamento['raesatisfacaoencontro']) {
		case 'P':$raesatisfacaoencontro="Pouco satisfeitos";break;
		case 'S':$raesatisfacaoencontro="Satisfeitos";break;
		case 'M':$raesatisfacaoencontro="Muito satisfeitos";break;
	}
	
	
	echo '<td>'.$raesatisfacaoencontro.'</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="30%">Quais destaques positivos  do referido do encontro presencial?</td>';
	echo '<td>'.$relatorioacompanhamento['raedestaquespositivosencontro'].'</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="30%">Quais destaques negativos - inibidores?</td>';
	echo '<td>'.$relatorioacompanhamento['raedestaquesnegativosencontro'].'</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="30%">O que você sugere para aperfeiçoar o evento?</td>';
	echo '<td>'.$relatorioacompanhamento['raeaperfeicoarencontro'].'</td>';
	echo '</tr>';
	echo '</table>';
			
	echo '</td>';
	echo '</tr>';
		
	echo '</table>';
	echo '</td>';
	echo '</tr>';

	echo '</table>';

}


function exibirSituacaoPagamento($dados) {
	global $db;

	$acao = "<img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"detalharDetalhesPagamentosUsuarios('||foo.pflcod||', this);\">";

	if($dados['ufpid']) {
		$wh[] = "uf.ufpid='".$dados['ufpid']."'";
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
			INNER JOIN escolaterra.pagamentobolsista pb ON pb.pflcod = p.pflcod 
			INNER JOIN escolaterra.ufparticipantes uf ON uf.ufpid = pb.ufpid
			INNER JOIN workflow.documento dc ON dc.docid = pb.docid AND dc.tpdid=".TPD_PAGAMENTOBOLSA."
			".(($wh)?" WHERE ".implode(" AND ",$wh):"")."

					) fee

					GROUP BY fee.pflcod, fee.pfldsc
						
							) foo
								
							INNER JOIN escolaterra.pagamentoperfil pp ON pp.pflcod = foo.pflcod";
	
	

	$cabecalho = array("&nbsp;","Perfil","Aguardando autorização MEC","R$","Autorizado MEC","R$","Aguardando autorização SGB","R$","Aguardando pagamento","R$","Enviado ao Banco","R$","Pagamento efetivado","R$","Pagamento recusado","R$","Pagamento não autorizado FNDE","R$");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);

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

function inserirDadosLog($dados) {
	global $db;

	$sql = "INSERT INTO log_historico.logsgb_escolaterra(
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
		FROM escolaterra.identificacaousuario i
		LEFT JOIN territorios.municipio m ON m.muncod = i.muncod
		LEFT JOIN escolaterra.identificaoendereco ie ON ie.iusid = i.iusid
		LEFT JOIN territorios.municipio m2 ON m2.muncod = ie.muncod
		LEFT JOIN escolaterra.identusutipodocumento it ON it.iusid = i.iusid
		WHERE i.iusid='".$dados['iusid']."'";

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
		 
		if(!$dados['sincronizacao']) $lnscpf = $db->carregarColuna("SELECT lnscpf FROM escolaterra.listanegrasgb");
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
		 
		$sql = "UPDATE escolaterra.identificacaousuario SET cadastradosgb=".(($logerro_gravarDadosBolsista=='TRUE')?'FALSE':'TRUE')." WHERE iusid='".$dados['iusid']."'";
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

	$sql = "SELECT ufpcnpj, ufpnome, muncod, estuf FROM escolaterra.ufparticipantes	WHERE ufpid='".$dados['ufpid']."'";
	$dadosentidade = $db->pegaLinha($sql);

	$xmlRetornoEntidade = $soapClient->lerDadosEntidade( array('sistema'           => SISTEMA_SGB,
			'login'            => USUARIO_SGB,
			'senha'            => SENHA_SGB,
			'nu_cnpj_entidade' => $dadosentidade['ufpcnpj']
	) );
	 
	inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcnpj'=>$dadosentidade['ufpcnpj'],'logservico'=>'lerDadosEntidade'));

	preg_match("/<nu_cnpj_entidade>(.*)<\\/nu_cnpj_entidade>/si", $xmlRetornoEntidade, $match);

	$existecnpj = (string) $match[1];

	$dadosEntidade = array( 'sistema'          => SISTEMA_SGB,
			'login'            => USUARIO_SGB,
			'senha'            => SENHA_SGB,
			'nu_cnpj_entidade' => $dadosentidade['ufpcnpj'],
			'co_tipo_entidade' => '1',
			'no_entidade'      => $dadosentidade['ufpnome'],
			'sg_entidade'      => '',
			'co_municipio'     => $dadosentidade['muncod'],
			'sg_uf'            => $dadosentidade['estuf']
	);

	$xmlRetorno_gravaDadosEntidade   = $soapClient->gravaDadosEntidade( $dadosEntidade );

	$logerro_gravaDadosEntidade = analisaCodXML($xmlRetorno_gravaDadosEntidade,'10001');

	inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcnpj'=>$dadosentidade['ufpcnpj'],'logservico'=>'gravaDadosEntidade','logerro' => $logerro_gravaDadosEntidade));

	if($existecnpj) $logerro_gravaDadosEntidade = 'FALSE';
	if(analisaCodXML($xmlRetorno_gravaDadosEntidade,'00036') == 'FALSE') $logerro_gravaDadosEntidade = 'FALSE';
	 
	$sql = "UPDATE escolaterra.ufparticipantes SET cadastrosgb=".(($logerro_gravaDadosEntidade=='TRUE')?'FALSE':'TRUE')." WHERE ufpid='".$dados['ufpid']."'";
	$db->executar($sql);
	$db->commit();

}


function verServicoEntidade($dados) {
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
	
	$sql = "SELECT ufpcnpj FROM escolaterra.ufparticipantes WHERE (cadastrosgb=false OR cadastrosgb IS NULL) AND ufpcnpj IS NOT NULL";
	$ufpcnpjs = $db->carregarColuna($sql);
	
	libxml_use_internal_errors( true );
	
	if($ufpcnpjs) {
		foreach($ufpcnpjs as $ufpcnpj) {
			$xmlRetornoEntidade = $soapClient->lerDadosEntidade( array('sistema'           => SISTEMA_SGB,
					'login'            => USUARIO_SGB,
					'senha'            => SENHA_SGB,
					'nu_cnpj_entidade' => $ufpcnpj
			) );

			echo "<pre>";
			print_r($xmlRetornoEntidade);
		}
	}
	
}

function condicaoRetornarCE($fpbid, $iusid) {
	global $db;

	$esdid = $db->pegaUm("SELECT esdid FROM escolaterra.pagamentobolsista p INNER JOIN workflow.documento d ON d.docid = p.docid WHERE p.iusid='".$iusid."' AND p.fpbid='".$fpbid."'");
	if(!$esdid) return true;
	
	if($esdid==ESD_PAGAMENTO_APTO) return true;
	else return 'O pagamento ja foi autorizado e não pode ser mais retornado';

}

function posacaoRetornarCE($fpbid, $iusid) {
	global $db;
	
	$db->executar("DELETE FROM escolaterra.pagamentobolsista WHERE iusid='".$iusid."' AND fpbid='".$fpbid."'");
	$db->commit();
	
	return true;
}

function processarPagamentoBolsistaSGB($dados) {
	global $db;

	$sql = "SELECT * FROM escolaterra.pagamentobolsista WHERE pboid='".$dados->id."'";
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
					
				$novaparcela = $db->pegaUm("SELECT (rfuparcela+1) as novaparcela FROM escolaterra.periodoreferenciauf f
							 				INNER JOIN escolaterra.ufparticipantes u ON u.ufpid = f.ufpid
							 				WHERE u.ufpid='".$pagamentobolsista['ufpid']."' AND f.fpbid='".$pagamentobolsista['fpbid']."'");
			}
				
			$sql = "UPDATE escolaterra.pagamentobolsista SET remid=null, pboparcela='".$novaparcela."' WHERE pboid='".$pagamentobolsista['pboid']."'";
			$db->executar($sql);
			$db->commit();
				
		} else {
			echo wf_alterarEstado( $pagamentobolsista['docid'], AED_RECUSAR_PAGAMENTO, $cmddsc = 'Erro retornado pelo FNDE: '.$dados->situacao->codigo.' / '.$dados->situacao->descricao, array());
			$sql = "UPDATE escolaterra.pagamentobolsista SET remid=null WHERE pboid='".$pagamentobolsista['pboid']."'";
			$db->executar($sql);
			$db->commit();
		}
	}

}

function carregarLogCadastroSGB($dados) {
	global $db;

	$iusd = $db->pegaUm("SELECT iusid FROM escolaterra.identificacaousuario WHERE iuscpf='".$dados['usucpf']."'");

	if($iusd) echo "<input type=hidden name=iusd id=iusd_log value=\"".$iusd."\">";

	$sql = "SELECT u.iuscpf, u.iusnome, to_char(logdata,'dd/mm/YYYY HH24:MI') as data, logresponse FROM log_historico.logsgb_escolaterra l
			INNER JOIN escolaterra.identificacaousuario u ON u.iuscpf = l.logcpf
			WHERE logcpf='".$dados['usucpf']."' AND logservico='gravarDadosBolsista' ORDER BY l.logdata DESC LIMIT 5";
	$cabecalho = array("CPF","Nome","Data","Erro");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true,false,false,true);

}


?>