<?

function alertlocation($dados) {
	
	die("<script>
		".(($dados['alert'])?"alert('".$dados['alert']."');":"")."
		".(($dados['location'])?"window.location='".$dados['location']."';":"")."
		".(($dados['javascript'])?$dados['javascript']:"")."
		 </script>");
}


function montaAbasSmfc($abapai, $abaativa) {
	global $db;
	
	$sql = "SELECT abaordem, abadsc, abaendereco, abafuncaomostrar, abapai FROM smfc.abas WHERE abapai='".$abapai."' ORDER BY abaordem";
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

function carregarOrientacao($endereco) {
	global $db;
	
	$sql = "SELECT oabdesc FROM sispacto.abas a 
			INNER JOIN sispacto.orientacaoaba o ON o.abaid = a.abaid 
			WHERE a.abaendereco='".$endereco."'";
	
	$orientacao = $db->pegaUm($sql);
	
	return (($orientacao)?$orientacao:"Orientação não foi cadastrada");
}

function carregarDetalhesPerfil($dados) {
	global $db;
	
	if($dados['pflcod_']==PFL_COORDENADOR_INSTITUCIONAL) {
		
		echo '<p align=center><b>Informações Coordenador Institucional</b></p>';
		
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">';
		echo '<tr>';
		
		echo '<td class="SubTituloDireita">Instituição:</td>';
		
		echo '<td>';
		
		$sql = "SELECT insid as codigo, inssigla||' / '||insnome as descricao FROM smfc.instituicoes WHERE insstatus='A'";
		$db->monta_combo('insid__', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'insid__','', $dados['insid__']);
		
		echo '</td>';
	
		echo '</tr>';
		
		echo '</table>';
	}

}


function efetuarInsercaoUsuarioPerfil($dados) {
	global $db;
	
	$iusid = $db->pegaUm("SELECT iusid FROM smfc.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array("",""),$dados['iuscpf__'])."'");
	
	if($iusid) {
		
		$sql = "UPDATE smfc.identificacaousuario SET 
				insid   =".(($dados['insid__'])?"'".$dados['insid__']."'":"NULL").", 
	    		iuscpf  =".(($dados['iuscpf__'])?"'".str_replace(array(".","-"),array("",""),$dados['iuscpf__'])."'":"NULL").", 
	    		iusnome =".(($dados['iusnome__'])?"'".$dados['iusnome__']."'":"NULL").", 
	    		iusemailprincipal =".(($dados['iusemailprincipal__'])?"'".$dados['iusemailprincipal__']."'":"NULL").", 
	    		iusstatus ='A' 
				WHERE iusid='".$iusid."'";
		
		$db->executar($sql);
		
	} else {
	
		$sql = "INSERT INTO smfc.identificacaousuario(
	            iuscpf, iusnome, 
	            iusemailprincipal, iusdatainclusao,  
	            iusstatus, insid)
	    VALUES (".(($dados['iuscpf__'])?"'".str_replace(array(".","-"),array("",""),$dados['iuscpf__'])."'":"NULL").", 
	    		".(($dados['iusnome__'])?"'".$dados['iusnome__']."'":"NULL").", 
	    		".(($dados['iusemailprincipal__'])?"'".$dados['iusemailprincipal__']."'":"NULL").", 
	    		NOW(), 
	    		'A', 
	    		".(($dados['insid__'])?"'".$dados['insid__']."'":"NULL")."
	    		) RETURNING iusid";
		
		$iusid = $db->pegaUm($sql);
	
	}
	
	$sql = "SELECT p.pfldsc, 
				   p.pflcod 
			FROM smfc.tipoperfil t 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.iusid='".$iusid."'";
	
	$arrPf = $db->pegaLinha($sql);
	
	if($arrPf['pfldsc'] && $arrPf['pflcod']!=$dados['pflcod__']) {
		
		$al = array("alert"=>"Inserção não efetivada com sucesso. O usuário ja esta cadastrado com o perfil : ".$arrPf['pfldsc'],
					"location"=>$_SERVER['REQUEST_URI']);
		alertlocation($al);
	}
	
	$tpeid = $db->pegaUm("SELECT tpeid FROM smfc.tipoperfil WHERE iusid='".$iusid."'");
	
	if(!$tpeid) {
		$sql = "INSERT INTO smfc.tipoperfil(
	            iusid, pflcod)
	    		VALUES ('".$iusid."', '".$dados['pflcod__']."');";
		
		$db->executar($sql);
	}
	
	$db->commit();
	
	$al = array("alert"=>"Inserção efetivada com sucesso",
				"location"=>$_SERVER['REQUEST_URI']);
	alertlocation($al);
	
	
}

function pegarDadosUsuarioPorCPF($dados) {
	global $db;
	$sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf='".$dados['cpf']."'";
	$usuemail = $db->pegaUm($sql);
	
	$sql = "SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf='".$dados['cpf']."' AND sisid='".SIS_SMFC."'";
	$suscod = $db->pegaUm($sql);
	
	
	echo $usuemail."||".(($suscod)?$suscod:"NC");
}

function carregarDadosIdentificacaoUsuario($dados) {
	global $db;
	
	if(!$dados['pflcod']) {
		$al = array("alert"=>"Problemas para carregar os dados usuário","location"=>"smfc.php?modulo=inicio&acao=C");
		alertlocation($al);
	}
	
	$sql = "SELECT i.cadastradosgb, i.uncid, i.iusid, i.iuscpf, i.iusnome, i.iusdatanascimento, i.iusnomemae, i.iusnaodesejosubstituirbolsa,
				   i.iussexo, i.eciid, i.nacid, i.iusnomeconjuge, i.iusagenciasugerida, i.iusagenciaend, 
				   i.iusemailprincipal, i.iusemailopcional, to_char(i.iusdatainclusao,'YYYY-mm-dd') as iusdatainclusao, i.iustermocompromisso,  
				   i.tvpid, i.funid, i.foeid, f.iufid, f.cufid, f.iufdatainiformacao, f.iufdatafimformacao, f.iufsituacaoformacao,
				   m.estuf as estuf_nascimento, m.muncod as muncod_nascimento, ma.estuf||' / '||ma.mundescricao as municipiodescricaoatuacao, ma.muncod as muncodatuacao, 
				   d.itdid, d.tdoid, d.itdufdoc, d.itdnumdoc, d.itddataexp, d.itdnoorgaoexp,
				   e.ienid, mm.muncod as muncod_endereco, mm.estuf as estuf_endereco,
				   e.ientipo, e.iencep, e.iencomplemento, e.iennumero, e.ienlogradouro, e.ienbairro, cf.cufcodareageral   
			FROM smfc.identificacaousuario i 
			INNER JOIN smfc.tipoperfil t ON t.iusid = i.iusid 
			LEFT  JOIN territorios.municipio m ON m.muncod = i.muncod 
			LEFT  JOIN smfc.identiusucursoformacao f ON f.iusid = i.iusid 
			LEFT  JOIN smfc.identusutipodocumento d ON d.iusid = i.iusid 
			LEFT  JOIN smfc.identificaoendereco e ON e.iusid = i.iusid 
			LEFT  JOIN territorios.municipio mm ON mm.muncod = e.muncod 
			LEFT  JOIN territorios.municipio ma ON ma.muncod = i.muncodatuacao
			LEFT  JOIN smfc.cursoformacao cf ON cf.cufid = f.cufid 
			WHERE t.pflcod='".$dados['pflcod']."' ".(($dados['iusid'])?" AND i.iusid='".$dados['iusid']."'":"")." AND iusstatus='A' ORDER BY i.iusid";
	
	$identificacaousuario = $db->carregar($sql);
	
	if($identificacaousuario[0]) {

		foreach($identificacaousuario as $key => $iu) {
			
			$idusuarios[$key] = $iu;
			unset($telefones);
			$sql = "SELECT itetipo, itedddtel, itenumtel FROM smfc.identificacaotelefone WHERE iusid='".$iu['iusid']."'";
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

function carregarMunicipiosPorUF($dados) {
	global $db;
	$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$dados['estuf']."' ORDER BY mundescricao";
	$combo = $db->monta_combo($dados['name'], $sql, 'S', 'Selecione', (($dados['onclick'])?$dados['onclick']:''), '', '', '200', 'S', $dados['id'], true, $dados['valuecombo']);
	
	if($dados['returncombo']) return $combo;
	else echo $combo;
}

function verificarFormacaoCompleta($dados) {
	global $db;
	$sql = "SELECT foecompleto FROM smfc.formacaoescolaridade WHERE foeid='".$dados['foeid']."'";
	$foecompleto = $db->pegaUm($sql);
	echo (($foecompleto=="t")?"T":"");
	echo (($foecompleto=="f")?"F":"");
}

function listarCursosFormacao($dados) {
	global $db;
	$sql = "SELECT cufid as codigo, cufcursodesc as descricao FROM smfc.cursoformacao WHERE cufstatus='A' AND cufcodareageral='".$dados['cufcodareageral']."' ORDER BY cufcursodesc";
	$db->monta_combo('cufid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'cufid', '');
	
}

function listarAgencias($dados) {
	global $db;
	if($dados['muncod']) {
		$codIbge 	= $dados['muncod'];
		$nuRaioKm 	= $db->pegaUm("SELECT munmedraio FROM territorios.municipio WHERE muncod='".$dados['muncod']."'");
		
		$cliente = new SoapClient( "http://ws.mec.gov.br/AgenciasBb/wsdl") ;
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

function verificarEmailUnico($dados) {
	global $db;
	
	$sql = "SELECT '- '||i.iusnome||' ( '||p.pfldsc||' );' as descricao FROM smfc.identificacaousuario i 
			INNER JOIN smfc.tipoperfil t ON t.iusid = i.iusid 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
			WHERE i.iusemailprincipal='".$dados['iusemailprincipal']."' AND i.iusid!='".$dados['iusid']."'";
	
	$nomes = $db->carregarColuna($sql);
	
	echo implode('\n', $nomes);
	
	
}

function atualizarNomeUsuario($dados) {
	global $db;
	
	include_once '../includes/webservice/cpf.php';
	
	$objPessoaFisica = new PessoaFisicaClient("http://ws.mec.gov.br/PessoaFisica/wsdl");
	$xml 			 = $objPessoaFisica->solicitarDadosPessoaFisicaPorCpf($dados['iuscpf']);
		
	$obj = (array) simplexml_load_string($xml);
	
	if($obj['PESSOA']->no_pessoa_rf) {
		$db->executar("UPDATE smfc.identificacaousuario SET iusnome='".$obj['PESSOA']->no_pessoa_rf."' WHERE iuscpf='".$dados['iuscpf']."'");
		$db->executar("UPDATE seguranca.usuario SET usunome='".$obj['PESSOA']->no_pessoa_rf."' WHERE usucpf='".$dados['iuscpf']."'");
		$db->commit();
	}
	
	$al = array("alert"=>"Nome atualizado com sucesso","location"=>"smfc.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=dados");
	alertlocation($al);
	
}

function atualizarDadosIdentificacaoUsuario($dados) {
	global $db;
	
	$erros = validarIdentificacaoUsuario($dados);
	
	if($erros) {
		$al = array("alert"=>"Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :".'\n\n'.implode('\n',$erros),"location"=>$dados['goto']);
		alertlocation($al);
	}

	$sql = "UPDATE smfc.identificacaousuario SET
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
	

	$iufid = $db->pegaUm("SELECT iufid FROM smfc.identiusucursoformacao WHERE iusid='".$dados['iusid']."'");
	
	// controlando formação
	if($iufid) {
		
		$sql = "UPDATE smfc.identiusucursoformacao SET
		            cufid=".(($dados['cufid'])?"'".$dados['cufid']."'":"NULL").", 
		            iufdatainiformacao='".formata_data_sql($dados['iufdatainiformacao'])."', 
		            iufdatafimformacao=".(($dados['iufdatafimformacao'])?"'".formata_data_sql($dados['iufdatafimformacao'])."'":"NULL").", 
		            iufsituacaoformacao='".$dados['iufsituacaoformacao']."'
		        WHERE iufid='".$iufid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO smfc.identiusucursoformacao(
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
	
	
	$itdid = $db->pegaUm("SELECT itdid FROM smfc.identusutipodocumento WHERE iusid='".$dados['iusid']."'");
	
	// controlando documento
	if($itdid) {
		
		$sql = "UPDATE smfc.identusutipodocumento SET
            	tdoid='".$dados['tdoid']."', itdufdoc='".$dados['itdufdoc']."', 
            	itdnumdoc='".$dados['itdnumdoc']."', itddataexp='".formata_data_sql($dados['itddataexp'])."', 
            	itdnoorgaoexp='".$dados['itdnoorgaoexp']."'		
		        WHERE itdid='".$itdid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO smfc.identusutipodocumento(
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
	
	$ienid = $db->pegaUm("SELECT ienid FROM smfc.identificaoendereco WHERE iusid='".$dados['iusid']."'");
	
	// controlando endereço
	if($ienid) {
		
		$sql = "UPDATE smfc.identificaoendereco SET
            	muncod='".substr($dados['muncod_endereco'],0,7)."', ientipo='".$dados['ientipo']."', 
            	iencep='".str_replace(array("-"),array(""),$dados['iencep'])."', iencomplemento=".(($dados['iencomplemento'])?"'".$dados['iencomplemento']."'":"NULL").", 
            	iennumero=".((!is_null($dados['iennumero']) && is_numeric($dados['iennumero']))?"'".$dados['iennumero']."'":"NULL").", ienlogradouro='".$dados['ienlogradouro']."', 
            	ienbairro='".$dados['ienbairro']."' 		
		        WHERE ienid='".$ienid."'";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "INSERT INTO smfc.identificaoendereco(
            	muncod, iusid, ientipo, iencep, iencomplemento, iennumero, 
            	iensatatus, ienlogradouro, ienbairro)
    			VALUES ('".substr($dados['muncod_endereco'],0,7)."', '".$dados['iusid']."', '".$dados['ientipo']."', '".str_replace(array("-"),array(""),$dados['iencep'])."', 
    					".(($dados['iencomplemento'])?"'".$dados['iencomplemento']."'":"NULL").", ".((!is_null($dados['iennumero']))?"'".str_replace(array(" "),array(""),$dados['iennumero'])."'":"NULL").", 'A', '".$dados['ienlogradouro']."', '".substr($dados['ienbairro'],0,60)."');";
		
		$db->executar($sql);
		
	}
	
	// controlando telefones
	$db->executar("DELETE FROM smfc.identificacaotelefone WHERE iusid='".$dados['iusid']."'");
	
	$tipos = array("R","T","C","F");
	
	foreach($tipos as $tipo) {
		
		$sql = "INSERT INTO smfc.identificacaotelefone(
            	iusid, itedddtel, itenumtel, itetipo, itestatus)
    			VALUES ('".$dados['iusid']."', ".(($dados['itedddtel'][$tipo])?"'".$dados['itedddtel'][$tipo]."'":"NULL").", ".(($dados['itenumtel'][$tipo])?"'".$dados['itenumtel'][$tipo]."'":"NULL").", '".$tipo."', 'A');";
		
		$db->executar($sql);
		
	}

	$db->commit();
	
	//sincronizarUsuariosSIMEC(array('cpf' => $dados['iuscpf']));
	
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

function listaCursosPlanejamento($dados) {
	global $db;
	
	
	
	switch($dados['sis']) {
		case 'mec':
			$btns .= '<img src=\"../imagens/excluir.gif\">';
			$clas .= 'class=\" normal\"';
			$clas2 .= 'class=\" disabled\" readonly=\"readonly\"';
			break;
		case 'coordenadorinstitucional':
			$btns .= '<img src=\"../imagens/icone_br.png\" style=\"cursor:pointer;\" onclick=\"exibirAbrangenciaCursos(\'||p.picid||\');\">';
			$clas .= 'class=\" disabled\" readonly=\"readonly\"';
			$clas2 .= 'class=\" normal\"';
			break;
	}
	
	$sql = "SELECT '{$btns} <input type=\"hidden\" name=\"picid[]\" value=\"'||p.picid||'\">' as acao,
				   c.curdesc,
				   '<center><input type=\"text\" style=\"text-align:;\" name=\"picvagasestimadas['||p.picid||']\" size=\"11\" maxlength=\"9\" value=\"'||COALESCE(p.picvagasestimadas::text,'')||'\" onkeyup=\"this.value=mascaraglobal(\'#######\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"picvagasestimadas_'||p.picid||'\" title=\"Vagas estimadas\" {$clas}></center>' as vagasestimadas,
				   '<center><input type=\"text\" style=\"text-align:;\" name=\"picvalorprevisto['||p.picid||']\" size=\"16\" maxlength=\"14\" value=\"'||COALESCE(trim(to_char(picvalorprevisto,'999g999g999d99')),'')||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"picvalorprevisto_'||p.picid||'\" title=\"Valor Previsto\" {$clas}></center>' as valorprevisto,
				   '<table width=100%><tr><td><font size=1><input type=\"radio\" name=\"picsituacao['||p.picid||']\" value=\"A\" '||CASE WHEN picsituacao='A' THEN 'checked' ELSE '' END||' onclick=\"analiseSituacao('||p.picid||',this);\"> Aceitar <br><input type=\"radio\" name=\"picsituacao['||p.picid||']\" value=\"R\" '||CASE WHEN picsituacao='R' THEN 'checked' ELSE '' END||' onclick=\"analiseSituacao('||p.picid||',this);\"> Recusar <br><input type=\"radio\" name=\"picsituacao['||p.picid||']\" value=\"P\" '||CASE WHEN picsituacao='P' THEN 'checked' ELSE '' END||' onclick=\"analiseSituacao('||p.picid||',this);\"> Repactuar</font></td><td valign=bottom '||CASE WHEN picsituacao='P' THEN '' ELSE 'style=\"display:none;\"' END||' id=\"dadosrepactua_'||p.picid||'\"><table width=\"100%\"><tr><td class=\"SubTituloDireita\"><b><font size=1>Vagas:</font></b></td><td><input type=\"text\" style=\"text-align:;\" name=\"picrepactuavagas['||p.picid||']\" size=\"11\" maxlength=\"9\" value=\"'||COALESCE(p.picrepactuavagas::text,'')||'\" onkeyup=\"this.value=mascaraglobal(\'#######\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"picrepactuavagas_'||p.picid||'\" title=\"Vagas repactuadas\" {$clas2}></td></tr><tr><td class=\"SubTituloDireita\"><b><font size=1>Valor:</font></b></td><td><input type=\"text\" style=\"text-align:;\" name=\"picrepactuavalor['||p.picid||']\" size=\"16\" maxlength=\"14\" value=\"'||COALESCE(trim(to_char(picrepactuavalor,'999g999g999d99')),'')||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"picrepactuavalor_'||p.picid||'\" title=\"Valor Repactuado\" {$clas2}></td></tr></table></td></tr></table>' as situacao,
			   	   '<font size=1>'|| us.usunome || ' ( ' || to_char(picdatainsercao,'dd/mm/YYYY HH24:MI') ||' )</font>' as usuario 
			FROM smfc.planejamentoinstituicaocurso p 
			INNER JOIN catalogocurso.curso c ON c.curid = p.curid 
			LEFT JOIN seguranca.usuario us ON us.usucpf = p.usucpf 
			WHERE pieid='".$dados['pieid']."'";
	
	$cabecalho = array("&nbsp;","Curso","Vagas Estimadas","Valor Previsto(R$)","Situação","Inserido por");
	$db->monta_lista($sql,$cabecalho,100,10,'N','center','N','formulario','','',null,array('ordena'=>false));
	
}

function inserirCursoPlanejamento($dados) {
	global $db;
	
	$sql = "INSERT INTO smfc.planejamentoinstituicaocurso(
            pieid, curid, usucpf,  
            picstatus, picdatainsercao)
    		VALUES ('".$dados['pieid']."', '".$dados['curid']."', '".$_SESSION['usucpf']."', 'A', NOW());";
    		
	$db->executar($sql);
	$db->commit();
	
}

function salvarPlanejamentoInstituicaoCurso($dados) {
	global $db;
	
	if($dados['picid']) {
		foreach($dados['picid'] as $picid) {
			
			$sql = "UPDATE smfc.planejamentoinstituicaocurso
   					SET 
   					picvagasestimadas=".(($dados['picvagasestimadas'][$picid])?"'".$dados['picvagasestimadas'][$picid]."'":"NULL").", 
   					picvalorprevisto=".(($dados['picvalorprevisto'][$picid])?"'".str_replace(array(".",","),array("","."),$dados['picvalorprevisto'][$picid])."'":"NULL").",
   					picsituacao=".(($dados['picsituacao'][$picid])?"'".$dados['picsituacao'][$picid]."'":"NULL").",
   					picrepactuavagas=".(($dados['picrepactuavagas'][$picid])?"'".$dados['picrepactuavagas'][$picid]."'":"NULL").", 
   					picrepactuavalor=".(($dados['picrepactuavalor'][$picid])?"'".str_replace(array(".",","),array("","."),$dados['picrepactuavalor'][$picid])."'":"NULL")."
					WHERE picid='".$picid."'";
			
			$db->executar($sql);
			
		}
		
		$db->commit();
	}
	
	$al = array("alert"=>"Planejamento salvo com sucesso",
				"location"=>"smfc.php?modulo=".$dados['modulo']."&acao=A&aba=planejamento&pieid=".$dados['pieid']);
	alertlocation($al);
	
}

?>