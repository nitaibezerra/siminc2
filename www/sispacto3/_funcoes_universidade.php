<?

function excluirAbrangencia($dados) {
	global $db;
	
	$sql = "SELECT * FROM sispacto3.abrangencia WHERE abrid='".$dados['abrid']."'";
	$abr = $db->pegaLinha($sql);
	
	if($abr['esfera']=='M') {
		
		$sql = "DELETE FROM sispacto3.orientadorestudoturma WHERE iusd IN(
				select i.iusd from sispacto3.identificacaousuario i 
				inner join sispacto3.pactoidadecerta p on p.picid = i.picid
				inner join sispacto3.tipoperfil t on t.iusd = i.iusd 
				where pflcod=".PFL_ORIENTADORESTUDO." and p.muncod='".$abr['muncod']."')";
		
		$db->executar($sql);
	}
	
	if($abr['esfera']=='E') {
		
		$sql = "DELETE FROM sispacto3.orientadorestudoturma WHERE iusd IN(
				select i.iusd from sispacto3.identificacaousuario i 
				inner join territorios.municipio mm on mm.muncod = i.muncodatuacao
				inner join sispacto3.pactoidadecerta p on p.estuf = mm.estuf and p.picid = i.picid 
				inner join sispacto3.tipoperfil t on t.iusd = i.iusd 
				where pflcod=".PFL_ORIENTADORESTUDO." and mm.muncod='".$abr['muncod']."')";
		
		$db->executar($sql);
		
	}
	
	$sql = "DELETE FROM sispacto3.abrangencia WHERE abrid='".$dados['abrid']."'";
	$db->executar($sql);
	$db->commit();
}


function definirAbrangencia($dados) {
	global $db;
	
	$total   = totalAlfabetizadoresAbrangencia(array("uncid"=>$dados['uncid']));
	$nturmas = totalTurmasAbrangencia(array("total"=>$total)); 
	$pp = carregarNumeroOrientadoresPendencia(array("ecuid" => $dados['ecuid']));
		
	echo "<p><b>Rede Municipal</b></p>";
	
	$sql = "SELECT
			'<center><img src=\"../imagens/mais.gif\" id=\"abran_mun_'||m.muncod||'\" title=\"mais\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"abrirDetalhamentoAbrangencia(\''||a.muncod||'\',\'M\',this);\">' as acao1,
			".((!$dados['consulta'])?"'<center><img src=\"../imagens/excluir.gif\" border=\"0\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirAbrangencia('||a.abrid||');\"></center>'":"''")." as acao2,
			m.estuf||' - '||m.mundescricao as descricao,
			(select count(*) from sispacto3.identificacaousuario i 
			inner join sispacto3.pactoidadecerta p on p.picid = i.picid
			inner join sispacto3.tipoperfil t on t.iusd = i.iusd 
			where pflcod=".PFL_ORIENTADORESTUDO." and p.muncod=a.muncod) as redemunicipal,
			COALESCE(esd.esddsc,'Não iniciado') as situacao,
			COALESCE(array_to_string(array(SELECT iusnome FROM sispacto3.identificacaousuario i INNER JOIN sispacto3.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=pp.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'Coordenador Local não cadastrado') as coordenadorlocal,
			COALESCE(array_to_string(array(SELECT '('||itedddtel||') '||itenumtel as tel FROM sispacto3.identificacaousuario i INNER JOIN sispacto3.tipoperfil t ON i.iusd=t.iusd INNER JOIN sispacto3.identificacaotelefone it ON it.iusd = i.iusd AND it.itetipo='C' WHERE i.picid=pp.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'-') as telefonecoordenadorlocal,
			COALESCE(array_to_string(array(SELECT iusemailprincipal FROM sispacto3.identificacaousuario i INNER JOIN sispacto3.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=pp.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'-') as emailcoordenadorlocal
			FROM sispacto3.abrangencia a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			LEFT JOIN sispacto3.pactoidadecerta pp ON pp.muncod = m.muncod 
			LEFT JOIN workflow.documento dd ON dd.docid = pp.docid 
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = dd.esdid 
			WHERE a.ecuid='".$dados['ecuid']."' AND a.esfera='M'
			ORDER BY 3";
	
	$cabecalho = array("&nbsp;","&nbsp;","UF/ Município","Orientadores","Situação","Coordenador Local","Telefone","Email");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	
	if($pp['nummunpendencias']>0) echo "<p style=\"color:red\"><img src=\"../imagens/atencao.png\" border=\"0\" align=\"absmiddle\"> Há ".$pp['nummunpendencias']." município(s) que não concluiu/ concluíram o cadastramento do(s) seu(s) Orientador(es) de Estudo.</p>";
	
	echo "<p><b>Rede Estadual</b></p>";
	
	$sql = "SELECT
			'<center><img src=\"../imagens/mais.gif\" id=\"abran_est_'||m.muncod||'\" title=\"mais\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"abrirDetalhamentoAbrangencia(\''||a.muncod||'\',\'E\',this);\">' as acao1,
			".((!$dados['consulta'])?"'<center><img src=\"../imagens/excluir.gif\" border=\"0\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirAbrangencia('||a.abrid||');\"></center>'":"''")." as acao2,
			m.estuf||' - '||m.mundescricao as descricao,
			(select count(*) from sispacto3.identificacaousuario i 
			inner join territorios.municipio mm on mm.muncod = i.muncodatuacao
			inner join sispacto3.pactoidadecerta p on p.estuf = mm.estuf and p.picid = i.picid 
			inner join sispacto3.tipoperfil t on t.iusd = i.iusd 
			where pflcod=".PFL_ORIENTADORESTUDO." and mm.muncod=a.muncod) as redeestadual,
			COALESCE(esd.esddsc,'Não iniciado') as situacao
			FROM sispacto3.abrangencia a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sispacto3.pactoidadecerta pp ON pp.estuf = m.estuf 
			INNER JOIN workflow.documento dd ON dd.docid = pp.docid 
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = dd.esdid  
			WHERE a.ecuid='".$dados['ecuid']."' AND a.esfera='E'
			ORDER BY 3";
	
	
	$cabecalho = array("&nbsp;","&nbsp;","UF/ Município","Orientadores","Situação");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	if($pp['numestpendencias']>0) echo "<p style=\"color:red\"><img src=\"../imagens/atencao.png\" border=\"0\" align=\"absmiddle\"> Há ".$pp['numestpendencias']." estado(s) que não concluiu/ concluíram o cadastramento do(s) seu(s) Orientador(es) de Estudo.</p>";

	
	$sql = "SELECT SUM(foo.x) from (
			
			(
			SELECT count(distinct i.iusd) as x
						FROM sispacto3.abrangencia a 
						INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
						INNER JOIN sispacto3.pactoidadecerta pp ON pp.estuf = m.estuf 
						INNER JOIN workflow.documento d ON d.docid = pp.docid
						INNER JOIN sispacto3.identificacaousuario i ON i.picid = pp.picid 
						INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
						WHERE a.ecuid='".$dados['ecuid']."' AND d.esdid IN(".ESD_ANALISE_COORDENADOR_LOCAL.",".ESD_VALIDADO_COORDENADOR_LOCAL.") AND a.esfera='E'
			) UNION ALL (
			
			SELECT count(distinct i.iusd) as x
						FROM sispacto3.abrangencia a 
						INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
						INNER JOIN sispacto3.pactoidadecerta pp ON pp.muncod = m.muncod 
						INNER JOIN workflow.documento d ON d.docid = pp.docid 
						INNER JOIN sispacto3.identificacaousuario i ON i.picid = pp.picid 
						INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
						WHERE a.ecuid='".$dados['ecuid']."' AND d.esdid IN(".ESD_ANALISE_COORDENADOR_LOCAL.",".ESD_VALIDADO_COORDENADOR_LOCAL.") AND a.esfera='M'
			
			)
			
			) foo";
	
	$totalcoordenadorlocal = $db->pegaUm($sql);
	
	echo "<br>";
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	echo "<tr><td class=\"SubTituloDireita\" width=\"20%\"><b>Total de Coordenador Local:</b></td><td>".$totalcoordenadorlocal."</td></tr>";
	echo "<tr><td class=\"SubTituloDireita\" width=\"20%\"><b>Total de Orientadores de Estudo:</b></td><td>".$total."</td></tr>";
	echo "<tr><td class=\"SubTituloDireita\" width=\"20%\"><b>Número de Turmas Estimado:</b></td><td>".$nturmas."</td></tr>";
	echo "</table>";
	
}

function totalTurmasAbrangencia($dados) {
	global $db;
	if($dados['total']>=10) {
		$resto = $dados['total']%25;
		$nturmas = floor($dados['total']/25);
		if($resto>=10) $nturmas++;
	} elseif($dados['total']>0) {
		$nturmas = 1;
	} else {
		$nturmas = 0;
	}
	return $nturmas;
}

function carregarNumeroOrientadoresPendencia($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*) FROM sispacto3.abrangencia a 
			LEFT JOIN sispacto3.estruturacurso e ON e.ecuid = a.ecuid
			LEFT JOIN sispacto3.pactoidadecerta p ON p.muncod=a.muncod
			LEFT JOIN workflow.documento d ON d.docid = p.docid 
			LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
			WHERE ".(($dados['uncid'])?"e.uncid='".$dados['uncid']."' AND":"")." ".(($dados['ecuid'])?"a.ecuid='".$dados['ecuid']."' AND":"")." a.esfera='M' AND (d.esdid IN('".ESD_ELABORACAO_COORDENADOR_LOCAL."','".ESD_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL."','".ESD_ANALISE_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL."') OR d.esdid IS NULL)";
	
	$nummunpendencias = $db->pegaUm($sql);
	
	$sql = "SELECT COUNT(DISTINCT mm.estuf) FROM sispacto3.pactoidadecerta p 
			INNER JOIN territorios.municipio mm ON mm.estuf = p.estuf  
			LEFT JOIN workflow.documento d ON d.docid = p.docid 
			LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
			INNER JOIN sispacto3.abrangencia a ON mm.muncod=a.muncod 
			LEFT JOIN sispacto3.estruturacurso e ON e.ecuid = a.ecuid 
			WHERE ".(($dados['uncid'])?"e.uncid='".$dados['uncid']."' AND":"")." ".(($dados['ecuid'])?"a.ecuid='".$dados['ecuid']."' AND":"")." (d.esdid IN('".ESD_ELABORACAO_COORDENADOR_LOCAL."','".ESD_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL."','".ESD_ANALISE_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL."') OR d.esdid IS NULL) AND a.esfera='E'
			GROUP BY p.estuf";
	
	$numestpendencias = $db->pegaUm($sql);
	
	return array("nummunpendencias" => $nummunpendencias, "numestpendencias" => $numestpendencias);	
	
}

function totalAlfabetizadoresAbrangencia($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*) FROM sispacto3.identificacaousuario i 
			INNER JOIN sispacto3.pactoidadecerta p ON p.picid = i.picid 
			INNER JOIN workflow.documento d ON d.docid = p.docid
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sispacto3.abrangencia a ON p.muncod = a.muncod 
			INNER JOIN sispacto3.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid = '".$dados['uncid']."' AND a.esfera='M' AND d.esdid IN(".ESD_ANALISE_COORDENADOR_LOCAL.",".ESD_VALIDADO_COORDENADOR_LOCAL.")";
	
	$totalmunicipio = $db->pegaUm($sql);
	
	$sql = "SELECT COUNT(*) FROM sispacto3.identificacaousuario i 
			INNER JOIN  territorios.municipio mm ON mm.muncod = i.muncodatuacao
			INNER JOIN sispacto3.pactoidadecerta p ON p.estuf = mm.estuf AND p.picid = i.picid 
			INNER JOIN workflow.documento d ON d.docid = p.docid 
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sispacto3.abrangencia a ON mm.muncod = a.muncod 
			INNER JOIN sispacto3.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid = '".$dados['uncid']."' AND a.esfera='E' AND d.esdid IN(".ESD_ANALISE_COORDENADOR_LOCAL.",".ESD_VALIDADO_COORDENADOR_LOCAL.")";
	
	$totalestado = $db->pegaUm($sql);
	
	$total = $totalmunicipio + $totalestado;
	
	return $total;
}

function cadastrarMunicipioAbrangencia($dados) {
	global $db;
	
	$municipio_abrangencia = $db->carregarColuna("SELECT 'Não foi possível vincular o município - '||m.mundescricao||' esta cadastrado na Universidade: '||un.uninome as descricao FROM sispacto3.abrangencia a 
						 						  INNER JOIN sispacto3.estruturacurso e ON e.ecuid = a.ecuid 
						 						  INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
						 						  INNER JOIN sispacto3.universidadecadastro u ON u.uncid = e.uncid 
						 						  INNER JOIN sispacto3.universidade un ON un.uniid = u.uniid  
						 						  WHERE uncstatus='A' AND a.muncod IN('".implode("','",$dados['muncod_abrangencia'])."') AND a.esfera='".$dados['esfera']."' LIMIT 10");
	
	if(!$municipio_abrangencia) {
		
		foreach($dados['muncod_abrangencia'] as $muncod) {
			if($muncod) {
				
				$sql = "INSERT INTO sispacto3.abrangencia(
			            muncod, ecuid, abrstatus, esfera)
			    		VALUES ('".$muncod."', '".$dados['ecuid']."', 'A', '".$dados['esfera']."');";
				
				$db->executar($sql);
				
			}
		}
	
		$db->commit();
		
 		$al = array("alert"=>"Municípios gravado com sucesso","javascript"=>"window.opener.definirAbrangencia();window.close();");
 		alertlocation($al);
	
	} else {
		
 		$al = array("alert"=>implode('\n',$municipio_abrangencia),"javascript"=>"window.opener.definirAbrangencia();window.close();");
 		alertlocation($al);
		
	}
}

function carregarCadastroIESProjeto($dados) {
	global $db;
	
	$sql = "SELECT un.uncid, 
				   un.curid, 
				   trim(to_char(un.uncvalortotalprojeto,'999g999g999d99')) as uncvalortotalprojeto, 
				   un.uncdatainicioprojeto, 
				   un.uncdatafimprojeto, 
				   un.unctipo, 
				   un.unctipocertificacao,
				   su.unisigla,
  				   su.uninome,
  				   su.unicnpj,
  				   su.unicep,
  				   mu.muncod,
  				   mu.mundescricao,
  				   su.unilogradouro,
  				   su.unibairro,
  				   su.unicomplemento,
  				   su.unidddcomercial,
  				   su.uninumcomercial,
  				   su.uniemail,
				   su.uniuf,
  				   su.uninumero,
  				   su.unisite,
				   re.reinome,
				   re.reicpf,
				   re.reidddcomercial,
				   re.reinumcomercial,
				   re.reiemail
		FROM sispacto3.universidadecadastro un 
		INNER JOIN sispacto3.universidade su ON su.uniid = un.uniid 
		LEFT JOIN territorios.municipio mu ON mu.muncod = su.muncod 
		INNER JOIN sispacto3.reitor re on re.uniid = su.uniid 
		WHERE un.uncid='".$dados['uncid']."'";

	$arr['universidade'] = $db->pegaLinha($sql);

	$arr['curso'] = carregarCurso(array("curid"=>$arr['universidade']['curid']));
	
	return $arr;
	
}

function carregarCurso($dados) {
	global $db;
	if($dados['curid']) {
		$sql = "select cur.curid, cur.curdesc, cur.curobjetivo, cur.curementa, cur.curcertificado, cur.curchmim, cur.curchmax from catalogocurso.curso cur where cur.curid = ".$dados['curid']." and cur.curstatus = 'A'";
		$arr = $db->pegaLinha($sql);
	}
	
	return $arr;
	
}


function atualizarDadosIES($dados) {
	global $db;
	
	if(!$dados['somentereitor']) {
	
		$sql = "UPDATE sispacto3.universidadecadastro SET uncdatainicioprojeto='".formata_data_sql($dados['uncdatainicioprojeto'])."',
														 uncdatafimprojeto='".formata_data_sql($dados['uncdatafimprojeto'])."',
														 unctipo='".$dados['unctipo']."',
														 unctipocertificacao='".$dados['unctipocertificacao']."' 
			    WHERE uncid='".$dados['uncid']."'";
		
		$db->executar($sql);
	
		$sql = "UPDATE sispacto3.universidade
	   			SET unisigla='".$dados['unisigla']."', uninome='".$dados['uninome']."', unicnpj='".str_replace(array(".","-","/"),array("","",""),$dados['unicnpj'])."', unicep='".str_replace(array("-"),array(""),$dados['unicep'])."', 
			        unilogradouro='".$dados['unilogradouro']."', unibairro='".$dados['unibairro']."', unicomplemento=".(($dados['unicomplemento'])?"'".$dados['unicomplemento']."'":"NULL").", unidddcomercial='".$dados['unidddcomercial']."', 
	       			uninumcomercial='".$dados['uninumcomercial']."', uniemail='".$dados['uniemail']."', uniuf='".$dados['uniuf']."', uninumero='".$dados['uninumero']."', 
	       			unisite='".$dados['unisite']."', muncod='".$dados['muncod_endereco']."'
	 			WHERE uniid IN(SELECT uniid FROM sispacto3.universidadecadastro WHERE uncid='".$dados['uncid']."')";
		
		$db->executar($sql);
	
	}
	
	$sql = "UPDATE sispacto3.reitor
   			SET reinome='".$dados['reinome']."', reicpf='".str_replace(array(".","-"),array("",""),$dados['reicpf'])."', reidddcomercial='".$dados['reidddcomercial']."', reinumcomercial='".$dados['reinumcomercial']."', 
       			reiemail='".$dados['reiemail']."'
 			WHERE uniid IN(SELECT uniid FROM sispacto3.universidadecadastro WHERE uncid='".$dados['uncid']."')";
	
	$db->executar($sql);
	
	$db->commit();
	
 	$al = array("alert"=>"Dados Gerais do Projeto inseridos com sucesso.","location"=>$dados['goto']);
 	alertlocation($al);
	
}



function inserirCoordenadorIESGerenciamento($dados) {
	global $db;
	
	$sql = "SELECT iusd FROM sispacto3.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'";
 	$iusd = $db->pegaUm($sql);
	
 	if($iusd) {
 		$sql = "UPDATE sispacto3.identificacaousuario SET iusstatus='A', iusemailprincipal='".$dados['iusemailprincipal']."' WHERE iusd='".$iusd."'";
 		$db->executar($sql);
 	} else {
     	$sql = "INSERT INTO sispacto3.identificacaousuario(
 	            uncid, iuscpf, iusnome, iusemailprincipal,  
 	            iusdatainclusao, iusstatus)
 			    VALUES ('".$dados['uncid']."', '".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".$dados['iusnome']."', '".$dados['iusemailprincipal']."',  
 			            NOW(), 'A') returning iusd;";
     	$iusd = $db->pegaUm($sql);
 	}
    	
    $existe_usu = $db->pegaUm("select usucpf from seguranca.usuario where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
    	
   	if(!$existe_usu) {
    	
	   	$sql = "INSERT INTO seguranca.usuario(
             	usucpf, usunome, usuemail, usustatus, ususenha, suscod)
     			VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".$dados['iusnome']."', '".$dados['iusemailprincipal']."', 'A', '".md5_encrypt_senha("simecdti", '')."', 'A');";
     	$db->executar($sql);
    	
    } else {
    	
    	if($dados['reenviarsenha']=="S") {
    		$cl_senha = ", ususenha='".md5_encrypt_senha( "simecdti", '' )."', usuchaveativacao=false";
    	}
    	$sql = "UPDATE seguranca.usuario SET usustatus='A', suscod='A', usuemail='".$dados['iusemailprincipal']."' {$cl_senha} WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'";
    	$db->executar($sql);
    }
   	
 	if($dados['reenviarsenha']=="S") {
 		
 		$remetente = array("nome" => "SIMEC - MÓDULO SISPACTO","email" => $dados['iusemailprincipal']);
 		$destinatario = $dados['iusemailprincipal'];
 		$usunome = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
 		$assunto = "Cadastro no SIMEC - MÓDULO SISPACTO";
 		$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
 		$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo sispacto3. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
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
 	}	
    	
    $existe_sis = $db->pegaUm("select usucpf from seguranca.usuario_sistema where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and sisid='".SIS_SISPACTO."'");
    	
    if(!$existe_sis) {
    		
    	$sql = "INSERT INTO seguranca.usuario_sistema(
         	    usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
     			VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', ".SIS_SISPACTO.", 'A', NULL, NOW(), 'A');";
	    	
     	$db->executar($sql);
	    	
    } else {
    	if($dados['suscod']=="A") {
 	    	$sql = "UPDATE seguranca.usuario_sistema SET suscod='A' WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' AND sisid='".SIS_SISPACTO."'";
 	    	$db->executar($sql);
    	}
    }
    	
    $existe_pfl = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and pflcod='".PFL_COORDENADORIES."'");
    	
    if(!$existe_pfl) {
    	$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".PFL_COORDENADORIES."');";
     	$db->executar($sql);
    }
   	
    $existe_usr = $db->pegaUm("select usucpf from sispacto3.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and pflcod='".PFL_COORDENADORIES."' AND rpustatus='A' AND uncid='".$dados['uncid']."'");
    
     if(!$existe_usr) {
    		$sql = "INSERT INTO sispacto3.usuarioresponsabilidade(
            		pflcod, usucpf, rpustatus, rpudata_inc, uncid)
 			    VALUES ('".PFL_COORDENADORIES."', '".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', 'A', NOW(), '".$dados['uncid']."');";
    		$db->executar($sql);
     }
    
     $arrTp = $db->pegaLinha("SELECT
     							  t.pflcod, 
     							  p.pfldsc, 
     							  uni.unisigla || '/' || uni.uninome as descricao 
     						FROM sispacto3.tipoperfil t 
     						INNER JOIN seguranca.perfil p ON p.pflcod=t.pflcod 
     						INNER JOIN sispacto3.identificacaousuario i ON i.iusd=t.iusd 
     						INNER JOIN sispacto3.universidadecadastro unc ON unc.uncid=i.uncid 
     						LEFT JOIN sispacto3.universidade uni ON uni.uniid=unc.uniid 
     						WHERE t.iusd='".$iusd."'");
     
     $existe_tpf = $arrTp['pflcod'];
    
     if(!$existe_tpf) {
 		$sql = "INSERT INTO sispacto3.tipoperfil(
 		            iusd, pflcod, tpestatus)
 		    	VALUES ('".$iusd."', '".PFL_COORDENADORIES."', 'A');";
     	$db->executar($sql);
     } else {
    	
     	if($existe_tpf!=PFL_COORDENADORIES) {
 			$al = array("alert"=>"Este CPF ja possui um perfil (".$arrTp['pfldsc'].",".$arrTp['descricao'].") no sistema e não pode ser cadastrado","location"=>"sispacto3.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=".$dados['picid']);
 			alertlocation($al);
     	}
    	
     }
    
     $db->commit();
			
 	$al = array("alert"=>"Coordenador-Geral da IES inserido com sucesso","location"=>"sispacto3.php?modulo=principal/universidade/gerenciarcoordenadories&acao=A&uncid=".$dados['uncid']);
 	alertlocation($al);
	
 }





function carregarCoordenadorIES($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT u.docidformacaoinicial, d8.docid as docidturmas, d7.docid as docidequipeies, d6.docid as docidorcamento, d5.docid as docidestruturaformacao, d4.docid as dociddadosprojeto, d3.esdid as esdidturma, u.docidturma, d2.esdid as esdidformacaoincial, d.esdid, u.uncid, re.reiid, su.uniuf, u.curid, u.docid, su.unisigla||' - '||su.uninome as descricao, u.docidformacaoinicial 
						   FROM sispacto3.universidadecadastro u 
					 	   INNER JOIN sispacto3.universidade su ON su.uniid = u.uniid
						   INNER JOIN sispacto3.reitor re on re.uniid = su.uniid 
						   LEFT JOIN workflow.documento d ON d.docid = u.docid 
						   LEFT JOIN workflow.documento d2 ON d2.docid = u.docidformacaoinicial 
						   LEFT JOIN workflow.documento d3 ON d3.docid = u.docidturma 
						   LEFT JOIN workflow.documento d4 ON d4.docid = u.dociddadosprojeto 
						   LEFT JOIN workflow.documento d5 ON d5.docid = u.docidestruturaformacao 
						   LEFT JOIN workflow.documento d6 ON d6.docid = u.docidorcamento
						   LEFT JOIN workflow.documento d7 ON d7.docid = u.docidequipeies 
						   LEFT JOIN workflow.documento d8 ON d8.docid = u.docidturmas
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$docidformacaoinicial = $arr['docidformacaoinicial'];
	
	if(!$docidformacaoinicial) {
		$docidformacaoinicial = wf_cadastrarDocumento(TPD_FORMACAOINICIAL,"SISPACTO 2015 - FORMACAO INICIAL - ".$_SESSION['usunome']);
		$db->executar("UPDATE sispacto3.universidadecadastro SET docidformacaoinicial='".$docidformacaoinicial."' WHERE uncid='".$dados['uncid']."'");
		$db->commit();
	}
	
	
	$dociddadosprojeto = $arr['dociddadosprojeto'];
	
	if(!$dociddadosprojeto) {
		$dociddadosprojeto = wf_cadastrarDocumento(TPD_PROJETOIES,"SISPACTO 2015 - DADOS GERAIS DO PROJETO - ".$_SESSION['usunome']);
		$db->executar("UPDATE sispacto3.universidadecadastro SET dociddadosprojeto='".$dociddadosprojeto."' WHERE uncid='".$dados['uncid']."'");
		$db->commit();
	}
	
	$docidestruturaformacao = $arr['docidestruturaformacao'];
	
	if(!$docidestruturaformacao) {
		$docidestruturaformacao = wf_cadastrarDocumento(TPD_PROJETOIES,"SISPACTO 2015 - ESTRUTURA DA FORMAÇÃO - ".$_SESSION['usunome']);
		$db->executar("UPDATE sispacto3.universidadecadastro SET docidestruturaformacao='".$docidestruturaformacao."' WHERE uncid='".$dados['uncid']."'");
		$db->commit();
	}
	
	$docidorcamento = $arr['docidorcamento'];
	
	if(!$docidorcamento) {
		$docidorcamento = wf_cadastrarDocumento(TPD_PROJETOIES,"SISPACTO 2015 - ESTRUTURA DA FORMAÇÃO - ".$_SESSION['usunome']);
		$db->executar("UPDATE sispacto3.universidadecadastro SET docidorcamento='".$docidorcamento."' WHERE uncid='".$dados['uncid']."'");
		$db->commit();
	}
	
	$docidequipeies = $arr['docidequipeies'];
	
	if(!$docidequipeies) {
		$docidequipeies = wf_cadastrarDocumento(TPD_PROJETOIES,"SISPACTO 2015 - EQUIPE IES - ".$_SESSION['usunome']);
		$db->executar("UPDATE sispacto3.universidadecadastro SET docidequipeies='".$docidequipeies."' WHERE uncid='".$dados['uncid']."'");
		$db->commit();
	}
	
	$docidturmas = $arr['docidturmas'];
	
	if(!$docidturmas) {
		$docidturmas = wf_cadastrarDocumento(TPD_PROJETOIES,"SISPACTO 2015 - TURMAS - ".$_SESSION['usunome']);
		$db->executar("UPDATE sispacto3.universidadecadastro SET docidturmas='".$docidturmas."' WHERE uncid='".$dados['uncid']."'");
		$db->commit();
	}
	
	
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sispacto3.identificacaousuario i 
							   INNER JOIN sispacto3.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.uncid='".$dados['uncid']."' AND t.pflcod='".PFL_COORDENADORIES."'");
	
	$_SESSION['sispacto3']['universidade'] = array("descricao" => $arr['descricao']."( ".$infprof['iusnome']." )",
												  "curid" => $arr['curid'], 
												  "uncid" => $arr['uncid'], 
												  "reiid" => $arr['reiid'], 
												  "estuf" => $arr['uniuf'], 
												  "dociddadosprojeto" => $dociddadosprojeto,
												  "docidestruturaformacao" => $docidestruturaformacao,
												  "docidorcamento" => $docidorcamento,
												  "docidequipeies" => $docidequipeies,
												  "docidturmas" => $docidturmas,
												  "docidformacaoinicial" => $docidformacaoinicial,
												  "iusd" => $infprof['iusd'],
												  "iuscpf" => $infprof['iuscpf']);
	
	if($dados['direcionar']) {
		if($arr['esdid']==ESD_VALIDADO_COORDENADOR_IES && $arr['esdidformacaoincial']==ESD_FECHADO_FORMACAOINICIAL)
			if($arr['esdidturma']!=ESD_FECHADO_TURMA) $al = array("location"=>"sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=turmasoutros");
			else $al = array("location"=>"sispacto3.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=principal");
		else $al = array("location"=>"sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function carregarSubAtividades($dados) {
	global $db;
	$sql = "SELECT suaid as codigo, suadesc as descricao FROM sispacto3.subatividades WHERE atiid='".$dados['atiid']."'";
	$db->monta_combo('suaid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'suaid', '');
	
}

function carregarUniversidadesPorUF($dados) {
	global $db;
	$sql = "SELECT u.uncid as codigo, su.uninome as descricao FROM sispacto3.universidadecadastro u 
	 	    INNER JOIN sispacto3.universidade su ON su.uniid = u.uniid
			WHERE su.uniuf='".$dados['estuf']."'";
	
	$combo = $db->monta_combo($dados['name'], $sql, 'S', 'Selecione', (($dados['onclick'])?$dados['onclick']:''), '', '', '', 'S', $dados['id'], true, $dados['valuecombo']);
	
	if($dados['returncombo']) return $combo;
	else echo $combo;
}

function carregarEstruturaCurso($dados) {
	global $db;
	$sql = "SELECT * FROM sispacto3.estruturacurso e
			LEFT JOIN territorios.municipio m ON m.muncod = e.muncod 
			WHERE e.ecuid='".$dados['ecuid']."'";
	$estruturacurso = $db->pegaLinha($sql);
	return $estruturacurso;
}

function atualizarEstruturaCurso($dados) {
	global $db;
	$sql = "UPDATE sispacto3.estruturacurso SET muncod='".$dados['muncod_endereco']."', ecuobsplanoatividades=".(($dados['ecuobsplanoatividades'])?"'".$dados['ecuobsplanoatividades']."'":"NULL")." WHERE ecuid='".$dados['ecuid']."'";
	$db->executar($sql);
	
	$suaids = array_keys($dados['aundatainicioprev']);
	
	if($suaids) {
		foreach($suaids as $suaid) {
			$aunid = $db->pegaUm("SELECT aunid FROM sispacto3.atividadeuniversidade au WHERE au.suaid = '".$suaid."' AND au.ecuid = '".$dados['ecuid']."'");
			
			if($aunid) {
				$sql = "UPDATE sispacto3.atividadeuniversidade SET aundatainicioprev=".(($dados['aundatainicioprev'][$suaid])?"'".formata_data_sql($dados['aundatainicioprev'][$suaid])."'":"NULL").", aundatafimprev=".(($dados['aundatafimprev'][$suaid])?"'".formata_data_sql($dados['aundatafimprev'][$suaid])."'":"NULL")." WHERE aunid='".$aunid."'";
				$db->executar($sql);
			} else {
				$sql = "INSERT INTO sispacto3.atividadeuniversidade(
            			suaid, aundatainicioprev, aundatafimprev, aunstatus, ecuid)
    					VALUES ('".$suaid."', ".(($dados['aundatainicioprev'][$suaid])?"'".formata_data_sql($dados['aundatainicioprev'][$suaid])."'":"NULL").", ".(($dados['aundatafimprev'][$suaid])?"'".formata_data_sql($dados['aundatafimprev'][$suaid])."'":"NULL").", 'A', '".$dados['ecuid']."');";
				$db->executar($sql);
			}
		}
	}
	
	$ainid = $db->pegaUm("SELECT ainid FROM sispacto3.articulacaoinstitucional WHERE ecuid='".$dados['ecuid']."'");
	
	if($ainid) {
		$sql = "UPDATE sispacto3.articulacaoinstitucional
   				SET ainseduc=".$dados['ainseduc'].", 
   					ainseducjustificativa='".$dados['ainseducjustificativa']."', 
   					ainundime=".$dados['ainundime'].", 
				    ainundimejustificativa='".$dados['ainundimejustificativa']."', 
				    ainuncme=".$dados['ainuncme'].", 
				    ainuncmejustificativa='".$dados['ainuncmejustificativa']."'
				 WHERE ainid='".$ainid."';";
		
		$db->executar($sql);
		
	} else {
		$sql = "INSERT INTO sispacto3.articulacaoinstitucional(
	            ecuid, ainseduc, ainseducjustificativa, ainundime, ainundimejustificativa, 
	            ainuncme, ainuncmejustificativa, ainstatus)
	    		VALUES ('".$dados['ecuid']."', ".$dados['ainseduc'].", '".$dados['ainseducjustificativa']."', ".$dados['ainundime'].", '".$dados['ainundimejustificativa']."', 
	            ".$dados['ainuncme'].", '".$dados['ainuncmejustificativa']."', 'A');";
		
		$db->executar($sql);
		
	}
	
	$db->commit();
	$al = array("alert"=>"Estrutura do curso atualizada com sucesso","location"=>$dados['goto']);
	alertlocation($al);
	
}

function carregarArticulacaoInstitucional($dados) {
	global $db;
	$sql = "SELECT * FROM sispacto3.articulacaoinstitucional WHERE ecuid='".$dados['ecuid']."'";
	$articulacaoinstitucional = $db->pegaLinha($sql);
	return $articulacaoinstitucional;
	
}


function carregarPlanoAtividades($dados) {
	global $db;
	
	$sql = "SELECT a.atiid, a.atidesc, s.suaid, s.suadesc, 
			".(($dados['ecuid'])?"(SELECT au.aundatainicioprev FROM sispacto3.atividadeuniversidade au WHERE au.suaid = s.suaid AND au.ecuid='".$dados['ecuid']."')":"''")." as aundatainicioprev, 
			".(($dados['ecuid'])?"(SELECT au.aundatafimprev FROM sispacto3.atividadeuniversidade au WHERE au.suaid = s.suaid AND au.ecuid='".$dados['ecuid']."')":"''")." as aundatafimprev 
			FROM sispacto3.subatividades s 
			INNER JOIN sispacto3.atividades a ON a.atiid = s.atiid 
			WHERE a.attitipo IN('U','E') AND s.suavisivel=true ORDER BY a.atidesc, s.suadesc";
	
	$subatividades = $db->carregar($sql);
	
	if($subatividades[0]) {
		foreach($subatividades as $sub) {
			$arrRsAgrupado[$sub['atidesc']]['atidesc'] = $sub['atidesc'];
			$arrRsAgrupado[$sub['atidesc']]['subatividades'][] = array("suaid"=>$sub['suaid'],
																	 "suadesc"=>$sub['suadesc'],
																	 "aundatainicioprev"=>$sub['aundatainicioprev'],
																	 "aundatafimprev"=>$sub['aundatafimprev']);
		}
	}
	
	echo "<table class=\"listagem\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" width=\"100%\">";
	echo "<tr>";
	echo "<td width=\"50%\" class=\"SubTituloCentro\">ATIVIDADES / SUBATIVIDADES</td><td class=\"SubTituloCentro\" colspan=\"2\">PERÍODO DE EXECUÇÃO</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td width=\"50%\" align=\"center\">&nbsp;</td><td align=\"center\">Início</td><td align=\"center\">Término</td>";
	echo "</tr>";
	
	
	if($arrRsAgrupado) {
		foreach($arrRsAgrupado as $atidesc => $at) {
			
			echo "<tr>";
			echo "<td width=\"50%\"><b>".$at['atidesc']."</b></td><td>&nbsp;</td><td>&nbsp;</td>";
			echo "</tr>";
			
			if($at['subatividades']) {
				
				foreach($at['subatividades'] as $su) {
					echo "<tr>";
					echo "<td width=\"50%\">".$su['suadesc']."</td>";
					if($dados['consulta']) echo "<td align=\"center\">".formata_data($su['aundatainicioprev'])."</td>";
					else echo "<td>".campo_data2('aundatainicioprev['.$su['suaid'].']','S', 'S', 'Inicío', 'S', '', '', $su['aundatainicioprev'], '', '', 'aundatainicioprev_'.$su['suaid'])."</td>";
					if($dados['consulta']) echo "<td align=\"center\">".formata_data($su['aundatafimprev'])."</td>";
					else echo "<td>".campo_data2('aundatafimprev['.$su['suaid'].']','S', 'S', 'Término', 'S', '', '', $su['aundatafimprev'], '', '', 'aundatafimprev_'.$su['suaid'])."</td>";
					echo "</tr>";
				}
			}
		}
	} else {
		echo "<tr>";
		echo "<td class=\"SubTituloEsquerda\">Nenhuma Subatividade foi cadastrada</td>";
		echo "</tr>";
	}
	
	echo "</table>";
	
}


function carregarListaCustos($dados) {
	global $db;
	
	if($dados['execucao']) {
		
				$sql = "(
				SELECT g.gdedesc, 
					   'Verba', 
					   '<input type=\"text\" style=\"text-align:;\" name=\"valorprevisto_'||o.orcid||'\" size=\"16\" maxlength=\"14\" value=\"'||trim(to_char(o.orcvlrunitario,'999g999g999d99'))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"valorprevisto_'||o.orcid||'\" title=\"Valor previsto\" readonly=\"readonly\" class=\" disabled\">' as valorprevisto,
					   '<input type=\"text\" style=\"text-align:;\" name=\"orcvalorexecutado['||o.orcid||']\" size=\"16\" maxlength=\"14\" value=\"'||trim(coalesce(to_char(o.orcvlrexecutado,'999g999g999d99'),''))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calcularOrcamentoExecucao();\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"valorexecutado_'||o.orcid||'\" title=\"Valor executado\" ".(($dados['consulta'])?"readonly=\"readonly\" class=\" disabled\"":" class=\" normal\"").">' as valorexecutado,
					   '<input type=\"text\" style=\"text-align:;\" name=\"saldo_'||o.orcid||'\" size=\"16\" maxlength=\"14\" value=\"'||trim(to_char(o.orcvlrunitario-coalesce(orcvlrexecutado,0),'999g999g999d99'))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"saldo_'||o.orcid||'\" title=\"Saldo\" readonly=\"readonly\" class=\" disabled\">' as saldo
				FROM sispacto3.orcamento o 
				INNER JOIN sispacto3.grupodespesa g ON g.gdeid = o.gdeid 
				WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A' 
				ORDER BY g.gdedesc
				) UNION ALL (
				
				SELECT '<b>TOTAIS</b>' as tot, 
					   '&nbsp;' as tot2, 
					   '<input type=\"text\" style=\"text-align:;\" name=\"totalvalorprevisto\" size=\"16\" maxlength=\"14\" value=\"'||trim(to_char(SUM(o.orcvlrunitario),'999g999g999d99'))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"totalvalorprevisto\" title=\"Total Valor Previsto\" readonly=\"readonly\" class=\" disabled\">' as totalvalorprevisto, 
					   '<input type=\"text\" style=\"text-align:;\" name=\"totalvalorexecutado\" size=\"16\" maxlength=\"14\" value=\"'||trim(coalesce(to_char(SUM(o.orcvlrexecutado),'999g999g999d99'),''))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"totalvalorexecutado\" title=\"Total Valor executado\" readonly=\"readonly\" class=\" disabled\">' as totalvalorexecutado,
					   '<input type=\"text\" style=\"text-align:;\" name=\"totalsaldo\" size=\"16\" maxlength=\"14\" value=\"'||trim(to_char(SUM(o.orcvlrunitario)-coalesce(SUM(o.orcvlrexecutado),0),'999g999g999d99'))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"totalsaldo\" title=\"Saldo\" readonly=\"readonly\" class=\" disabled\">' as totalsaldo 
				FROM sispacto3.orcamento o 
				INNER JOIN sispacto3.grupodespesa g ON g.gdeid = o.gdeid 
				WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A' 
				
				)";
		
		$cabecalho = array("Elementos de Despesa","Unidade de medida","Valor previsto (R$)","Valor executado (R$)","Saldo (R$)");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','S');
		
		
		
	} elseif($dados['relatoriofinal']) { 
		
		$sql = "SELECT g.gdedesc, o.orcvlrunitario, '<input type=\"text\" style=\"text-align:;\" name=\"orcvlrfinal['||o.orcid||']\" size=\"16\" maxlength=\"14\" value=\"'||coalesce(trim(to_char(o.orcvlrfinal,'999g999g999d99')),'')||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"orcvlrfinal_'||o.orcid||'\" title=\"Valor final\" class=\" normal\">' as valorfinal, '<textarea id=\"orcdescricao_'||o.orcid||'\" name=\"orcdescricao['||o.orcid||']\" cols=\"20\" rows=\"3\" onmouseover=\"MouseOver( this );\" onfocus=\"MouseClick( this );\" onmouseout=\"MouseOut( this );\" onblur=\"MouseBlur( this );\" style=\"width:80ex;\" class=\"txareanormal\">'||coalesce(o.orcdescricaofinal,'')||'</textarea>' as detalhamentofinal
				FROM sispacto3.orcamento o
				INNER JOIN sispacto3.grupodespesa g ON g.gdeid = o.gdeid
				WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A'
				ORDER BY g.gdedesc";
		
		$cabecalho = array("Grupo de Despesa","Valor total (R$)","Valor final (R$)","Detalhamento final");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'S','100%','S');
		
	} else {
	
		$sql = "SELECT ".(($dados['consulta'])?"''":"'<center><img src=../imagens/alterar.gif style=\"cursor:pointer;\" onclick=\"inserirCustos(\''||o.orcid||'\');\"> <img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirCustos(\''||o.orcid||'\');\"></center>'")." as acao, g.gdedesc, 'Verba', o.orcvlrunitario, o.orcdescricao 
				FROM sispacto3.orcamento o 
				INNER JOIN sispacto3.grupodespesa g ON g.gdeid = o.gdeid 
				WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A' 
				ORDER BY g.gdedesc";
		
		$cabecalho = array("&nbsp;","Grupo de Despesa","Unidade de Medida","Valor total (R$)","Memória de cálculo");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'S','100%','S');
	
	}
	
	
}

function carregarNaturezaDespesasCustos($dados) {
	global $db;
	$sql = "SELECT n.ndecodigo, n.ndedesc, SUM(o.orcvlrtotal) as total 
			FROM sispacto3.orcamento o 
			INNER JOIN sispacto3.itemdespesa i ON i.ideid = o.ideid 
			INNER JOIN sispacto3.grupodespesa g ON g.gdeid = i.gdeid 
			INNER JOIN sispacto3.naturezadespesa n ON n.ndeid = g.ndeid  
			WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A' 
			GROUP BY n.ndecodigo, n.ndedesc";
	
	$cabecalho = array("Código","Descrição","Valor(R$)");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}

function carregarOrcamento($dados) {
	global $db;
	$sql = "SELECT * FROM sispacto3.orcamento o 
			INNER JOIN sispacto3.grupodespesa g ON o.gdeid = g.gdeid 
			WHERE orcid='".$dados['orcid']."'";
	
	$orcamento = $db->pegaLinha($sql);
	
	return $orcamento;
	
}

function atualizarOrcamentoExecucao($dados) {
	global $db;
	
	if($dados['orcvalorexecutado']) {
		foreach(array_keys($dados['orcvalorexecutado']) as $orcid) {
			$sql = "UPDATE sispacto3.orcamento SET orcvlrexecutado=".(($dados['orcvalorexecutado'][$orcid])?"'".str_replace(array(".",","),array("","."),$dados['orcvalorexecutado'][$orcid])."'":"NULL").",
												  orcvlratualizado=".(($dados['orcvaloratualizado'][$orcid])?"'".str_replace(array(".",","),array("","."),$dados['orcvaloratualizado'][$orcid])."'":"NULL")."
					WHERE orcid='".$orcid."'";

			$db->executar($sql);
			$db->commit();
		}
	}
	
	$al = array("alert"=>"Orçamento atualizado com sucesso.","location"=>"sispacto3.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=orcamentoexecucao");
	alertlocation($al);
	
}

function atualizarCusto($dados) {
	global $db;
	$sql = "UPDATE sispacto3.orcamento SET gdeid='".$dados['gdeid']."', orcvlrunitario='".str_replace(array(".",","),array("","."),$dados['orcvlrunitario'])."', orcdescricao='".$dados['orcdescricao']."'
			WHERE orcid='".$dados['orcid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Custo inserido com sucesso","javascript"=>"window.opener.carregarListaCustos();window.close();");
	alertlocation($al);
	
}

function excluirCustos($dados) {
	global $db;
	$sql = "DELETE FROM sispacto3.orcamento WHERE orcid='".$dados['orcid']."'";
	$db->executar($sql);
	$db->commit();
	
	
}

function inserirCusto($dados) {
	global $db;
	$sql = "INSERT INTO sispacto3.orcamento(
            uncid, gdeid, orcvlrunitario, 
            orcstatus, orcdescricao)
    		VALUES ('".$dados['uncid']."', '".$dados['gdeid']."', '".str_replace(array(".",","),array("","."),$dados['orcvlrunitario'])."', 'A', '".$dados['orcdescricao']."');";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Item inserido com sucesso.","javascript"=>"window.opener.carregarListaCustos();window.close();");
	alertlocation($al);
	
}

function carregarDetalhamentoAbrangencia($dados) {
	global $db;
	
	if($dados['esfera']=='M') {
		
		$sql = "SELECT i.iuscpf, i.iusnome, i.iusemailprincipal, es.esddsc  FROM sispacto3.identificacaousuario i 
				INNER JOIN sispacto3.pactoidadecerta p ON p.picid = i.picid 
				LEFT JOIN workflow.documento d ON d.docid = p.docid 
				LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
				INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN sispacto3.abrangencia a ON p.muncod=a.muncod
				WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND a.ecuid='".$dados['ecuid']."' AND a.muncod='".$dados['muncod']."' AND a.esfera='M'";
		
		$cabecalho = array("CPF","Nome","Email","Situação cadastro");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	
	}
	
	if($dados['esfera']=='E') {
		
		$sql = "SELECT i.iuscpf, i.iusnome, i.iusemailprincipal, es.esddsc FROM sispacto3.identificacaousuario i 
				INNER JOIN territorios.municipio mm ON mm.muncod = i.muncodatuacao
				INNER JOIN sispacto3.pactoidadecerta p ON p.estuf = mm.estuf AND p.picid = i.picid 
				LEFT JOIN workflow.documento d ON d.docid = p.docid 
				LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
				INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN sispacto3.abrangencia a ON mm.muncod=a.muncod
				WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND a.ecuid='".$dados['ecuid']."' AND a.muncod='".$dados['muncod']."' AND a.esfera='E'";
		
		$cabecalho = array("CPF","Nome","Email","Situação cadastro");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	}
	
}

function carregarEquipeRecursosHumanos($dados) {
	global $db;
	$sql = "(SELECT '&nbsp' as acao, '<center>'||CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END||'</center>' as iuscpf, iusnome, iusemailprincipal, p.pfldsc  
			FROM sispacto3.identificacaousuario i 
			LEFT JOIN sispacto3.portarianomeacao po ON po.iusd = i.iusd 
			LEFT JOIN public.arquivo ar ON ar.arqid = po.arqid
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sispacto3.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			WHERE t.pflcod IN('".PFL_COORDENADORIES."') AND i.uncid='".$dados['uncid']."')
			UNION ALL (
			SELECT ".((!$dados['consulta'])?"'<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"inserirEquipe(\''||i.iusd||'\');\" > <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirEquipeRecursosHumanos(\''||i.iusd||'\');\">' || CASE WHEN t.tpejustificativaformadories IS NULL THEN '' ELSE ' <img src=\"../imagens/valida2.gif\" border=\"0\" style=\"cursor:pointer;\" onclick=\"jAlert(\''||t.tpejustificativaformadories||'\', \'Justificativa\');\">' END||'</center>'":"'&nbsp;'")." as acao, '<center>'||CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END||'</center>', iusnome, iusemailprincipal, p.pfldsc 
			FROM sispacto3.identificacaousuario i
			LEFT JOIN sispacto3.portarianomeacao po ON po.iusd = i.iusd 
			LEFT JOIN public.arquivo ar ON ar.arqid = po.arqid
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sispacto3.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			WHERE t.pflcod IN('".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."') AND i.uncid='".$dados['uncid']."' ORDER BY p.pflcod, i.iusnome)";
	
	$equiperh = $db->carregar($sql);
	
	
	$cabecalho = array("&nbsp;","CPF","Nome","E-mail","Perfil");
	$db->monta_lista_simples($equiperh,$cabecalho,1000,5,'N','100%','N');

}

function numeroMaximoCoordenadorAjuntoIES($dados) {
	global $db;
	$sql = "SELECT m.estuf FROM sispacto3.estruturacurso e 
	 		INNER JOIN sispacto3.abrangencia a ON a.ecuid = e.ecuid 
	 		INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
	 		WHERE e.uncid='".$dados['uncid']."' 
	 		GROUP BY m.estuf";
	
	$maxCoordenadorAjunto = $db->carregarColuna($sql);
	return count($maxCoordenadorAjunto);
	
}

function numeroCoordenadorAdjuntoIES($dados) {
	global $db;
	$sql = "SELECT COUNT(*) FROM sispacto3.identificacaousuario i 
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			WHERE i.uncid='".$dados['uncid']."' AND t.pflcod='".PFL_COORDENADORADJUNTOIES."'";
	
	$numCoordenadorAjunto = $db->pegaUm($sql);
	
	return $numCoordenadorAjunto;
	
}


function numeroFormadorIES($dados) {
	global $db;
	$sql = "SELECT COUNT(*) FROM sispacto3.identificacaousuario i 
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			WHERE i.uncid='".$dados['uncid']."' AND t.pflcod='".PFL_FORMADORIES."'";
	
	$numFormadorIes = $db->pegaUm($sql);
	
	return $numFormadorIes;
}


function validarNumeroFormadorIES($dados) {
	global $db;
	$total   = totalAlfabetizadoresAbrangencia(array("uncid"=>$dados['uncid']));
	$nturmas = totalTurmasAbrangencia(array("total"=>$total)); 

	$numFormadorIes = numeroFormadorIES($dados);
	
	if($nturmas>$numFormadorIes) return true;
	else return false;
	
}

function pegarJustificativaFormadorIES($dados) {
	global $db;
	$sql = "SELECT tpejustificativaformadories FROM sispacto3.tipoperfil WHERE iusd='".$dados['iusd']."'";
	$tpejustificativaformadories = $db->pegaUm($sql);
	return $tpejustificativaformadories;
}


function inserirEquipeRecursosHumanos($dados) {
	global $db;
	
	if($dados['iusd']) {
		
		$pagamento = $db->pegaUm("SELECT pboid FROM sispacto3.pagamentobolsista WHERE iusd='".$dados['iusd']."'");
		
		if(!$pagamento) {
			$sql = "DELETE FROM sispacto3.tipoperfil WHERE iusd='".$dados['iusd']."'";
			$db->executar($sql);
		}
		
		
		if($dados['pflcod']!=PFL_FORMADORIES) {
			$sql = "SELECT turid FROM sispacto3.turmas WHERE iusd='".$dados['iusd']."'";
			$existe = $db->pegaUm($sql);
			if($existe) {
				$al = array("alert"=>"Não é possível trocar o perfil. Este usuário possui turma associada. Remova a turma para altera o perfil","location"=>"sispacto3.php?modulo=principal/universidade/inserirequipe&acao=A&iusd=".$dados['iusd']);
				alertlocation($al);
			}
		}
		
	}
	
	$iusd = $db->pegaUm("SELECT iusd FROM sispacto3.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
	
	if(!$iusd) {
		
		$sql = "INSERT INTO sispacto3.identificacaousuario(
	            iuscpf, iusnome, iusemailprincipal, foeid, uncid, iusdatainclusao)
	    		VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', 
	    				'".$dados['iusnome']."', '".$dados['iusemailprincipal']."', 
	    				'".$dados['foeid']."', '".$dados['uncid']."', NOW()) RETURNING iusd;";
		
		
		$iusd = $db->pegaUm($sql);
		
	} else {
		
		$sql = "SELECT 'Nome : '||i.iusnome||', Perfil : '||p.pfldsc||', '||CASE WHEN u.uncid IS NOT NULL THEN uu.unisigla || ' / ' || uu.uninome || ',' ELSE '' END||' '||CASE WHEN pc.estuf IS NOT NULL THEN e.estuf || ' / ' || e.estdescricao || ',' ELSE '' END||' '||CASE WHEN pc.muncod IS NOT NULL THEN m.estuf || ' / ' || m.mundescricao || ',' ELSE '' END||' CPF não pode ser cadastrado. É necessário remove-lo do perfil indicado.' as msg 
				FROM sispacto3.identificacaousuario i 
				INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				LEFT JOIN sispacto3.pactoidadecerta pc ON pc.picid = i.picid 
				LEFT JOIN territorios.municipio m ON m.muncod = pc.muncod 
				LEFT JOIN territorios.estado e ON e.estuf = pc.estuf
				LEFT JOIN sispacto3.universidadecadastro u ON u.uncid = i.uncid 
				LEFT JOIN sispacto3.universidade uu ON uu.uniid = u.uniid 
				WHERE i.iusd='".$iusd."' AND CASE WHEN t.pflcod IN(".PFL_FORMADORIES.",".PFL_SUPERVISORIES.",".PFL_COORDENADORADJUNTOIES.") THEN i.uncid IS NOT NULL ELSE true END";
		
		$msg = $db->pegaUm($sql);
		
		if($msg) {
			$al = array("alert"=>$msg,"location"=>"sispacto3.php?modulo=principal/universidade/inserirequipe&acao=A");
			alertlocation($al);
		}
		
		$sql = "UPDATE sispacto3.identificacaousuario SET iusemailprincipal='".$dados['iusemailprincipal']."', 
														 foeid='".$dados['foeid']."', 
														 uncid='".$dados['uncid']."',
														 iusstatus='A'
				WHERE iusd='".$iusd."'";
		
		$db->executar($sql);
		
	}
	
	// controlando telefones
	$db->executar("DELETE FROM sispacto3.identificacaotelefone WHERE iusd='".$iusd."' AND itetipo='T'");
	
	$sql = "INSERT INTO sispacto3.identificacaotelefone(
           	iusd, itedddtel, itenumtel, itetipo, itestatus)
   			VALUES ('".$iusd."','".$dados['itedddtel']['T']."', '".$dados['itenumtel']['T']."', 'T', 'A');";
		
	$db->executar($sql);

	$existe_tipoperfil  = $db->pegaUm("SELECT tpeid FROM sispacto3.tipoperfil WHERE iusd='".$iusd."'");
	
	if(!$existe_tipoperfil) {
		
		$sql = "INSERT INTO sispacto3.tipoperfil(
	            iusd, pflcod, tpestatus, tpejustificativaformadories)
	    		VALUES ('".$iusd."', '".$dados['pflcod']."', 'A',".(($dados['tpejustificativaformadories'])?"'".$dados['tpejustificativaformadories']."'":"NULL").");";
		
		$db->executar($sql);
		
	}
	
	$db->commit();
	
	$al = array("alert"=>"Equipe gravada com sucesso","javascript"=>"window.opener.carregarEquipeRecursosHumanos();window.close();");
	alertlocation($al);
	
}

function excluirEquipeRecursosHumanos($dados) {
	global $db;
	
	$existe_pg = $db->pegaUm("SELECT pboid FROM sispacto3.pagamentobolsista WHERE tpeid IN( SELECT tpeid FROM sispacto3.tipoperfil WHERE iusd='".$dados['iusd']."' )");
	
	if($existe_pg) {
		$al = array("alert"=>"Membro da equipe ja possui pagamento e não pode ser excluído do projeto. Acesse a Execução => Gerenciar Equipe, e faça a exclusão.","location"=>"sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=recursos_humanos");
		alertlocation($al);
	}
	
	$sql = "DELETE FROM sispacto3.tipoperfil WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM sispacto3.identificacaotelefone WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);

	$db->commit();
	
	$sql = "SELECT turid, pflcod FROM sispacto3.turmas WHERE iusd='".$dados['iusd']."'";
	$turids = $db->carregar($sql);
	
	if($turids[0]) {
		foreach($turids as $tur) {
			if($tur['pflcod']) $plpabreviacao = $db->pegaUm("SELECT plpabreviacao FROM sispacto3.pagamentoperfil WHERE pflcod='".$tur['pflcod']."'");
			excluirTurma(array("turid" => $tur['turid'], "plpabreviacao" => $plpabreviacao));			
		}
	}
	
	$al = array("alert"=>"Membro da equipe removido com sucesso","location"=>"sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=recursos_humanos");
	alertlocation($al);
	
}

function numeroMaximoTurmas($dados) {
	global $db;
	$total   = totalAlfabetizadoresAbrangencia(array("uncid"=>$dados['uncid']));
	$nturmas = totalTurmasAbrangencia(array("total"=>$total));
	$formadoressolicitados = carregarDadosIdentificacaoUsuario(array("uncid"=>$dados['uncid'],"pflcod"=>PFL_FORMADORIES,"tpejustificativaformadories"=>true));
	$numeroMaximoTurmas = $nturmas + count($formadoressolicitados);
	return $numeroMaximoTurmas;
}

function numeroTurmas($dados) {
	global $db;
	$total_turmas = $db->pegaUm("SELECT COUNT(*) FROM sispacto3.turmas WHERE uncid='".$dados['uncid']."' AND turstatus='A'");
	return $total_turmas;
}


function atualizarTurma($dados) {
	global $db;
	
	$sql = "UPDATE sispacto3.turmas SET iusd='".$dados['iusd']."', turdesc='".$dados['turdesc']."', muncod='".$dados['muncod_endereco']."' WHERE turid='".$dados['turid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	echo "Turma atualizada com sucesso";
}

function validarCadastroTurma($dados) {
	global $db;
	
	$numeroMaximoTurmas = numeroMaximoTurmas($dados);
	$total_turmas = numeroTurmas($dados);
	if($numeroMaximoTurmas>$total_turmas) return true;
	else return false;
	
}

function inserirTurma($dados) {
	global $db;
	
	$sql = "INSERT INTO sispacto3.turmas(
	        uncid, iusd, turdesc, turstatus, muncod)
	    	VALUES ('".$dados['uncid']."', '".$dados['iusd']."', '".$dados['turdesc']."', 'A', '".$dados['muncod_endereco']."');";
	
	$db->executar($sql);
	$db->commit();
	
	echo "Turma inserida com sucesso";

}

function inserirTurmaOutros($dados) {
	global $db;
	
	$pflcod = $db->pegaUm("SELECT pflcod FROM sispacto3.tipoperfil WHERE iusd='".$dados['iusd']."'");

	$sql = "INSERT INTO sispacto3.turmas(
	        uncid, iusd, turdesc, turstatus, pflcod)
	    	VALUES ('".$dados['uncid']."', '".$dados['iusd']."', '".$dados['turdesc']."', 'A', '".$pflcod."');";

	$db->executar($sql);
	$db->commit();

	echo "Turma inserida com sucesso";


}

function carregarTurmas($dados) {
	global $db, $_HIERARQUIA_PFL;
	
	$pflcods = $_HIERARQUIA_PFL[$dados['pflcodturma']];
	
	if($pflcods) {
		foreach($pflcods as $pflcod) {
			
			$sql = "INSERT INTO sispacto3.turmas(
		            uncid, iusd, turdesc, turstatus, pflcod)
					SELECT i.uncid, i.iusd, 'TURMA #{$pflcod} - '||i.iusd as turma, 'A', {$pflcod} as pflcod FROM sispacto3.identificacaousuario i
					INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd
					LEFT JOIN sispacto3.turmas tu ON tu.iusd = i.iusd AND tu.pflcod=".$pflcod."
					WHERE i.iusstatus='A' AND t.pflcod=".$dados['pflcodturma']."  AND i.uncid='".$dados['uncid']."' AND tu.turid IS NULL";
			
			$db->executar($sql);
			$db->commit();
				
		}
	}
	
	if($dados['formacaoinicial']) {
		echo "<p><b>Marcar Todos : </b><input type=\"radio\" name=\"marcartodos\" onclick=\"marcarTodos(this);\" value=\"TRUE\"> Presente <input type=\"radio\" name=\"marcartodos\" onclick=\"marcarTodos(this);\" value=\"FALSE\"> Ausente</p>";
	}

	if($pflcods) {
		foreach($pflcods as $pflcod) {
				
			$plpabreviacaosub = $db->pegaUm("SELECT plpabreviacao FROM sispacto3.pagamentoperfil WHERE pflcod='{$pflcod}'");
			$pfldscsub = $db->pegaUm("SELECT pfldsc FROM seguranca.perfil WHERE pflcod='{$pflcod}'");
			
			
			$per = $db->pegaUm("SELECT pfldsc FROM seguranca.perfil WHERE pflcod='".$pflcod."'");
			echo "<p align=center><b>Turma de ".$per."</b></p>";
			
			$sql = "SELECT 
						'<img src=\"../imagens/mais.gif\" title=\"mais\" id=\"btn_turma_'||t.turid||'\" style=\"cursor:pointer;\" onclick=\"abrirTurma('||t.turid||',\'{$plpabreviacaosub}\',this)\"> ".((!$dados['consulta'])?"<img src=../imagens/salvar.png style=\"cursor:pointer;\" onclick=\"comporTurma(\''||turid||'\')\">":"")."' as acao,
						t.turdesc,
						'<center>'||replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')||'</center>' as iuscpf,
						i.iusnome,
						i.iusemailprincipal,
						(SELECT '(' || itedddtel || ') '|| itenumtel FROM sispacto3.identificacaotelefone WHERE iusd=t.iusd AND itetipo='T') as telefone,
						(SELECT count(*) FROM sispacto3.".$plpabreviacaosub."turma tu INNER JOIN sispacto3.tipoperfil tt ON tt.iusd = tu.iusd WHERE turid=t.turid) as qtd
					FROM sispacto3.turmas t 
					INNER JOIN sispacto3.identificacaousuario i ON i.iusd = t.iusd 
					INNER JOIN sispacto3.tipoperfil tt ON tt.iusd = i.iusd AND tt.pflcod='".$dados['pflcodturma']."'
					LEFT JOIN territorios.municipio m ON m.muncod = t.muncod
					WHERE t.uncid='".$dados['uncid']."' AND t.pflcod={$pflcod}";
			
			$cabecalho = array("&nbsp;","Turma","CPF","Nome","Email","Telefone","Qtd. ".$pfldscsub);
			$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	
		}
	}
	
	
}

function validarFormadoresTurmas($dados) {
	global $db;
	$sql = "SELECT 'Formador ('||i.iusnome||') não foi vinculado a nenhuma turma.' as t FROM sispacto3.identificacaousuario i 
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_FORMADORIES."
			LEFT JOIN sispacto3.turmas tt ON tt.iusd = i.iusd 
			WHERE i.uncid='".$dados['uncid']."' AND tt.turid IS NULL";
	$mensagens = $db->carregarColuna($sql);
	
	return $mensagens;
	
}


function inserirAlunoTurma($dados) {
	global $db;
	$sql = "INSERT INTO sispacto3.".$dados['plpabreviacao']."turma(
            turid, iusd, otustatus)
    		VALUES ('".$dados['turid']."', '".$dados['iusd']."', 'A');";
	$db->executar($sql);
	
	if($dados['uncid']) {
		$sql = "UPDATE sispacto3.identificacaousuario SET uncid='".$dados['uncid']."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	} else {
		$sql = "UPDATE sispacto3.identificacaousuario SET uncid=NULL WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	}
	
	$db->commit();
}


function excluirAlunoTurma($dados) {
	global $db;
	if($dados['iusd']) $wh = "iusd='".$dados['iusd']."'";
	elseif($dados['turid']) $wh = "turid='".$dados['turid']."'";
	
	$sql = "DELETE FROM sispacto3.".$dados['plpabreviacao']."turma WHERE ".$wh;
	$db->executar($sql);
	$db->commit();
}

function excluirTurma($dados) {
	global $db;
	
	excluirAlunoTurma($dados);
	
	$sql = "DELETE FROM sispacto3.turmas WHERE turid='".$dados['turid']."'";
	$db->executar($sql);
	$db->commit();
	
}


function carregarFiltrosMunicipios($dados) {
	global $db;
	if($dados['estuf']) {
		$sql = "SELECT m.muncod as codigo, REPLACE(m.mundescricao,'\'',' ') as descricao 
				FROM territorios.municipio m 
				".(($dados['esfera']=="M")?"INNER JOIN sispacto3.pactoidadecerta p ON p.muncod = m.muncod":"")." 
				WHERE m.estuf='".$dados['estuf']."' AND m.muncod NOT IN(SELECT muncod FROM sispacto3.abrangencia WHERE esfera='".$dados['esfera']."') ORDER BY mundescricao";
	} else {
		$sql = "SELECT muncod as codigo, muncod as descricao FROM sispacto3.abrangencia WHERE 1=2";
	}
	
	$_SESSION['indice_sessao_combo_popup']['muncod_abrangencia']['sql'] = $sql;
}

function verificarCoordenadorIESTermoCompromisso($dados) {
	global $db;
	// verificando se coordenador local aceitou o termo de compromisso
	$coordies = carregarDadosIdentificacaoUsuario(array("uncid"=>$dados['uncid'],"pflcod"=>PFL_COORDENADORIES));
	
	if($coordies) {
		$coordies = current($coordies);
	}
	
	if($coordies['iustermocompromisso']!="t") {
		$al = array("alert"=>"Por favor preencha todos os campos obrigatórios da tela “Dados Coordenador IES”.","location"=>"sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=dados");
		alertlocation($al);
	}
}

function numeroAlunosTurma($dados) {
	global $db;
	$sql = "SELECT COUNT(*) FROM sispacto3.orientadorturma WHERE turid='".$dados['turid']."'";
	$numturmas = $db->pegaUm($sql);
	
	echo $numturmas;
	
}

function validarEnvioAnaliseMEC($aba) {
	global $db;
	
	$perfis = pegaPerfilGeral();
	if(!$perfis) $perfis = array();
	
	if(!$db->testa_superuser() && !in_array(PFL_ADMINISTRADOR,$perfis)) {
		switch($aba) {
			case 'turmas':
				
				$sql = "SELECT replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.')||' - '||i.iusnome||' - '||pp.pfldsc as nome FROM sispacto3.identificacaousuario i 
						INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod IN(".PFL_ORIENTADORESTUDO.",".PFL_FORMADORIES.",".PFL_SUPERVISORIES.",".PFL_COORDENADORLOCAL.")
						INNER JOIN seguranca.perfil pp ON pp.pflcod = t.pflcod 
						LEFT JOIN sispacto3.orientadorestudoturma ot ON ot.iusd = i.iusd 
						LEFT JOIN sispacto3.formadoriesturma      ot2 ON ot2.iusd = i.iusd 
						LEFT JOIN sispacto3.supervisoriesturma    ot3 ON ot3.iusd = i.iusd 
						LEFT JOIN sispacto3.coordenadorlocalturma ot4 ON ot4.iusd = i.iusd 
						WHERE i.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."' AND ot.otuid IS NULL AND ot2.otuid IS NULL AND ot3.otuid IS NULL AND ot4.otuid IS NULL";
				
				$nomesemturma = $db->carregarColuna($sql);
				
				if($nomesemturma) {
					$erro = 'Existem alguns nomes que não foram alocados em turmas:\n';
					
					$erro .= implode('<br>\n',$nomesemturma);
				}
				
				
				
				break;
			case 'recursos_humanos':
				
				$total   = totalAlfabetizadoresAbrangencia(array("uncid"=>$_SESSION['sispacto3']['universidade']['uncid']));
				$nturmas = totalTurmasAbrangencia(array("total"=>$total));
				
				$qtdformadores = $db->pegaUm("SELECT count(*) FROM sispacto3.identificacaousuario i
											  INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
											  WHERE i.iusstatus='A' AND i.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."' AND t.pflcod='".PFL_FORMADORIES."'");
				
				if($qtdformadores > $nturmas) {
					$erro .= '- Número máximo de formadores IES é '.$nturmas.'. Atualmente existem '.$qtdformadores.' formadores cadastrados.<br>\n';
				}
				
				break;
			case 'estrutura_curso':
				
				$muncod = $db->pegaUm("SELECT muncod FROM sispacto3.estruturacurso WHERE uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'");
				
				$qtdmunicipio = $db->pegaUm("SELECT count(*) as qtdmunicipio FROM sispacto3.abrangencia a 
											 INNER JOIN sispacto3.estruturacurso e ON e.ecuid = a.ecuid 
											 WHERE e.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'");
				
				if(!$muncod) {
					$erro .= '- Sede do curso em branco<br>\n';
				}
					
				if(!$qtdmunicipio) {
					$erro .= '- Nenhum município de abrangência selecionado<br>\n';
				}
				
				$cronograma = $db->pegaUm("SELECT count(*) as total
											FROM sispacto3.subatividades s 
											INNER JOIN sispacto3.atividades a ON a.atiid = s.atiid 
											LEFT JOIN sispacto3.atividadeuniversidade au ON au.suaid = s.suaid AND au.ecuid='".$_SESSION['sispacto3']['universidade']['ecuid']."'
											WHERE a.attitipo IN('U','E') AND s.suavisivel=true AND aunid IS NULL");
				
				if($cronograma) {
					$erro .= '- Plano de Atividades em branco<br>\n';
				}
				
				$articulacaoinstitucional = $db->pegaUm("SELECT count(*) FROM sispacto3.articulacaoinstitucional WHERE ecuid='".$_SESSION['sispacto3']['universidade']['ecuid']."' AND (
																																							ainseduc IS NULL OR ainseducjustificativa IS NULL OR
																																							ainundime IS NULL OR ainundimejustificativa IS NULL OR 
																																							ainuncme IS NULL OR ainuncmejustificativa IS NULL
																																						   )");
				
				if($articulacaoinstitucional) {
					$erro .= '- Articulação Institucional em branco<br>\n';
				}
				
				break;
			
			case 'dados_projeto':
			
				$dadosprojeto = $db->pegaLinha("SELECT uncdatainicioprojeto, uncdatafimprojeto, unctipo, unctipocertificacao FROM sispacto3.universidadecadastro WHERE uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'");
				
				if(!$dadosprojeto['uncdatainicioprojeto'] || !$dadosprojeto['uncdatafimprojeto']) {
					$erro .= '- Vigência do projeto em branco<br>\n';
				}
				
				if(!$dadosprojeto['unctipo']) {
					$erro .= '- Origem dos recursos em branco<br>\n';
				}
				
				if(!$dadosprojeto['unctipocertificacao']) {
					$erro .= '- Tipo de Certificação em branco<br>\n';
				}
				
				break;
		}
	}
	
	


	if($erro) return $erro;
	else return true;
	
	
	
	//$mensagens = validarFormadoresTurmas(array("uncid" => $_SESSION['sispacto3']['universidade']['uncid']));
	
	//if($mensagens) $erro .= implode('<br>\n',$mensagens).'<br><br>\n\n Há '.count($mensagens).' Orientadores de Estudo que não foram vinculados a nenhuma turma. Retorne para a tela Turmas e vincule todos os nomes.<br><br>\n\n';
	
	/*
	$pp = carregarNumeroOrientadoresPendencia(array("uncid" => $_SESSION['sispacto3']['universidade']['uncid']));
	if($pp['nummunpendencias']>0) $erro .= 'Há '.$pp['nummunpendencias'].' municípios que não concluíram o cadastramento dos seus Orientadores de Estudo<br>\n';
	if($pp['numestpendencias']>0) $erro .= 'Há '.$pp['numestpendencias'].' estados que não concluíram o cadastramento dos seus Orientadores de Estudo<br>\n';
	*/
	/*
	$numeroMaximoTurmas = numeroMaximoTurmas(array("uncid" => $_SESSION['sispacto3']['universidade']['uncid']));
	$total_turmas = numeroTurmas(array("uncid" => $_SESSION['sispacto3']['universidade']['uncid']));
	if($total_turmas>$numeroMaximoTurmas) {
		$erro .= '<br>\nNúmero máximo de turmas (na tela Turmas) deve ser igual ao Número máximo de Formadores (na tela Equipe IES) + Formadores Justificados<br><br>\n\n';
	}
	*/
	/*
	$tso = validarTurmasSemOrientadores(array("uncid" => $_SESSION['sispacto3']['universidade']['uncid']));
	
	if($tso) {
		$erro .= '<br>\nExistem '.count($tso).' turma(s) sem orientadores cadastrados.';
	}
	
	*/

}

function validarTurmasSemOrientadores($dados) {
	global $db;
	
	$sql = "SELECT foo.turdesc FROM (
			
			SELECT t.turdesc, (SELECT COUNT(*) FROM sispacto3.orientadorturma WHERE turid=t.turid) as nalunos FROM sispacto3.turmas t 
			INNER JOIN sispacto3.tipoperfil tp ON tp.iusd = t.iusd AND t.pflcod = '".PFL_FORMADORIES."' 
			WHERE t.uncid='".$dados['uncid']."'
					
			) foo WHERE foo.nalunos=0";
	
	$turmassemorientadores = $db->carregar($sql);
	
	return $turmassemorientadores;
	
}

function validarOrientadoresCadastradosTurma($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*)			
			FROM sispacto3.turmas t 
			INNER JOIN sispacto3.orientadorturma ot ON ot.turid = t.turid
			INNER JOIN sispacto3.tipoperfil tt ON tt.iusd = ot.iusd and tt.pflcod=".PFL_ORIENTADORESTUDO." 
			INNER JOIN sispacto3.identificacaousuario i ON i.iusd = tt.iusd 
			WHERE t.uncid='".$dados['uncid']."' AND i.iusstatus='A'";
	
	$numOrientadoresTurmas = $db->pegaUm($sql);
	
	$sql = "SELECT SUM(foo.t) FROM (
			(SELECT COUNT(*) as t  FROM sispacto3.identificacaousuario i 
			INNER JOIN sispacto3.pactoidadecerta p ON p.picid = i.picid 
			LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sispacto3.abrangencia a ON p.muncod=a.muncod 
			INNER JOIN sispacto3.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid='".$dados['uncid']."' AND i.uncid='".$dados['uncid']."' AND a.esfera='M' AND i.iusstatus='A') 
			UNION ALL (
			SELECT COUNT(*) as t FROM sispacto3.identificacaousuario i 
			INNER JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
			INNER JOIN sispacto3.pactoidadecerta p ON p.estuf = m.estuf AND p.picid = i.picid 
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sispacto3.abrangencia a ON m.muncod=a.muncod 
			INNER JOIN sispacto3.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid='".$dados['uncid']."' AND i.uncid='".$dados['uncid']."' AND a.esfera='E' AND i.iusstatus='A')
			) foo";
	
	$numTotalOrientadores = $db->pegaUm($sql);
	
	if($numOrientadoresTurmas==$numTotalOrientadores) return true;
	else return ($numTotalOrientadores-$numOrientadoresTurmas);
	
}



function calculaPorcentagemTramitacaoUniversidade($dados) {
	global $db;
	
	
	$sql = "SELECT d.docid, d.esdid, to_char(h.htddata,'YYYY-mm-dd') as htddata 
			FROM sispacto3.universidadecadastro u 
			INNER JOIN workflow.documento d ON d.docid = u.docid 
			LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
			WHERE uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'";
	
	$documento = $db->pegaLinha($sql);
	
	$sql = "SELECT to_char(htddata,'YYYY-mm-dd') as htddata FROM workflow.historicodocumento WHERE docid='".$documento['docid']."' ORDER BY htddata ASC LIMIT 1";
	$aundatainicio = $db->pegaUm($sql);
	
	if($documento['esdid']==ESD_ANALISE_COORDENADOR_IES) {
		$aunsituacao = '50';
	}
	
	if($documento['esdid']==ESD_VALIDADO_COORDENADOR_IES) {
		$aunsituacao = '100';
		$aundatafim = $documento['htddata'];;
	}
	
	$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sispacto3']['universidade']['uncid']));
	
	gerenciarAtividadeUniversidade(array('aundatafim'=>$aundatafim,'aundatainicio'=>$aundatainicio,'aunsituacao'=>$aunsituacao,'ecuid'=>$ecuid,'suaid'=>$dados['suaid']));
	
}


function sqlEquipeCoordenadorIES($dados) {
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
					(SELECT usucpf FROM sispacto3.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					CASE WHEN pic.picid IS NOT NULL THEN 
														CASE WHEN pic.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN pic.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
					
			FROM sispacto3.identificacaousuario i
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sispacto3.pactoidadecerta pic ON pic.picid = i.picid 
			LEFT JOIN workflow.documento d ON d.docid = pic.docid 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = pic.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE (t.pflcod IN('".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."','".PFL_COORDENADORLOCAL."','".PFL_PROFESSORALFABETIZADOR."') OR (t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.iusformacaoinicialorientador=true)) AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' AND CASE WHEN pic.picid IS NOT NULL THEN d.esdid=".ESD_VALIDADO_COORDENADOR_LOCAL." ELSE true END ORDER BY p.pflcod, i.iusnome";
	
	return $sql;
}


function mostrarAbaFormacaoInicial($dados) {
	global $db;
	$sql = "SELECT u.uncid FROM sispacto3.universidadecadastro u 
			INNER JOIN workflow.documento d ON d.docid = u.docidorcamento
			INNER JOIN workflow.documento d2 ON d2.docid = u.docidestruturaformacao
			INNER JOIN workflow.documento d3 ON d3.docid = u.docidequipeies 
			INNER JOIN workflow.documento d4 ON d4.docid = u.dociddadosprojeto 
			INNER JOIN workflow.documento d5 ON d5.docid = u.docidturmas
			WHERE d.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND 
				  d2.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND 
				  d3.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND 
				  d4.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND 
				  d5.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND
				  u.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."' AND 
				  u.usucpfparecer IS NOT NULL";
		
	$uncid = $db->pegaUm($sql);
	
	if($uncid) {
		return true;
	} else {
		return false;
	}
	
}

function mostrarAbaTurmasOutras($dados) {
	global $db;
	$estado = wf_pegarEstadoAtual( $_SESSION['sispacto3']['universidade']['docid'] );
	$estado2 = wf_pegarEstadoAtual( $_SESSION['sispacto3']['universidade']['docidformacaoinicial'] );
	if($estado['esdid'] == ESD_VALIDADO_COORDENADOR_IES && $estado2['esdid'] == ESD_FECHADO_FORMACAOINICIAL) {
		return true;
	} else {
		return false;
	}

}

function salvarFormacaoInicial($dados) {
	global $db;
	if($dados['iusd']) {
		foreach($dados['iusd'] as $iusd => $fi) {
			$sql = "UPDATE sispacto3.identificacaousuario SET iusformacaoinicialorientador=".$fi." WHERE iusd='".$iusd."'";
			$db->executar($sql);
		}
		$db->commit();
	}
	
	$al = array("alert"=>"Formação Inicial dos Orientadores foram salvas com sucesso. Não esqueça de preencher os alunos Presentes na Formação Inicial que não constam na lista, ao final do processo CLICAR no botão de Concluir os Registros de Frequência","location"=>"sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=formacaoinicial");
	alertlocation($al);

}

function condicaoFormacaoInicial() {
	global $db;
	
	$sql = "SELECT DISTINCT i.iusnome ||' - '||m.estuf||'/'||m.mundescricao as d FROM sispacto3.identificacaousuario i 
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			INNER join sispacto3.pactoidadecerta p on i.picid = p.picid 
			INNER JOIN sispacto3.abrangencia a ON p.muncod=a.muncod 
			INNER JOIN sispacto3.estruturacurso c ON c.ecuid = a.ecuid 
			INNER JOIN territorios.municipio m ON m.muncod = p.muncod 
			WHERE SUBSTR(i.iuscpf,1,3)!='SIS' AND i.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."' AND c.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."' AND t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.iusstatus='A' AND iusformacaoinicialorientador IS NULL";
	
	$iusnomes = $db->carregarColuna($sql);
	
	if($iusnomes) return 'É necessário gravar os registros selecionando os orientadores que obtiveram a Formação Inicial<br><br>\n\n'.implode('\n<br>',$iusnomes);
	else return true;
	
}

function verificarValidacaoEstruturaFormacao($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*) as total FROM sispacto3.abrangencia a 
			INNER JOIN sispacto3.estruturacurso e ON e.ecuid = a.ecuid 
			WHERE e.uncid='".$dados['uncid']."'";
	
	$tot_abrangencia = $db->pegaUm($sql);
	
	$sql = "SELECT muncod FROM sispacto3.estruturacurso e WHERE e.uncid='".$dados['uncid']."'";
	$existe_sede = $db->pegaUm($sql);
	
	if($tot_abrangencia == 0 || !$existe_sede) {
		$al = array("alert"=>"É necessário cadastrar as informações na aba Estrutura da Formação","location"=>"sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=estrutura_curso");
		alertlocation($al);
	}
	
}


function devolverProjetoUniversidade($uncid) {
	global $aed_devolver;
	$aed_devolver=true;
	return aprovarProjetoUniversidade($uncid);
}

function aprovarProjetoUniversidade($uncid) {
	global $db, $aed_devolver;
	
	if($aed_devolver) {
		$aedid=AED_REPROVAR_CADASTRO_ORIENTADORES;
		$esdid=ESD_VALIDADO_COORDENADOR_LOCAL;
	} else {
		$aedid=AED_APROVAR_CADASTRO_ORIENTADORES;
		$esdid=ESD_ANALISE_COORDENADOR_LOCAL;
	}
	
	$sql = "SELECT DISTINCT pp.docid
			FROM sispacto3.abrangencia a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sispacto3.estruturacurso e ON e.ecuid = a.ecuid 
			LEFT JOIN sispacto3.pactoidadecerta pp ON pp.muncod = m.muncod 
			LEFT JOIN workflow.documento dd ON dd.docid = pp.docid 
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = dd.esdid 
			WHERE e.uncid='".$uncid."' AND a.esfera='M' AND esd.esdid='".$esdid."'";
	
	$docids = $db->carregarColuna($sql);
	
	if($docids) {
		foreach($docids as $docid) {
			wf_alterarEstado( $docid, $aedid, $cmddsc = '', array());
		}
	}
	
	$sql = "SELECT pp.docid
			FROM sispacto3.abrangencia a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sispacto3.estruturacurso e ON e.ecuid = a.ecuid
			LEFT JOIN sispacto3.pactoidadecerta pp ON pp.estuf = m.estuf 
			LEFT JOIN workflow.documento dd ON dd.docid = pp.docid 
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = dd.esdid  
			WHERE e.uncid='".$uncid."' AND a.esfera='E'";
	
	$docids = $db->carregarColuna($sql);
	
	if($docids) {
		foreach($docids as $docid) {
			wf_alterarEstado( $docid, $aedid, $cmddsc = '', array());
		}
	}
	
	if(!$aed_devolver) {
		
		$sql = "UPDATE sispacto3.identificacaousuario id SET uncid=foo.uncid FROM (
				SELECT 
				u.uncid,
				i.iusd
				FROM sispacto3.universidadecadastro u 
				INNER JOIN sispacto3.estruturacurso e ON e.uncid = u.uncid 
				INNER JOIN sispacto3.abrangencia a ON a.ecuid = e.ecuid 
				INNER JOIN sispacto3.pactoidadecerta p ON p.muncod = a.muncod 
				INNER JOIN sispacto3.identificacaousuario i ON i.picid = p.picid 
				INNER JOIN sispacto3.tipoperfil t ON i.iusd = t.iusd 
				WHERE u.uncid='".$uncid."' AND esfera='M' AND i.uncid IS NULL ) foo WHERE foo.iusd = id.iusd";
		
		$db->executar($sql);
		
		$sql = "UPDATE sispacto3.identificacaousuario id SET uncid=foo.uncid FROM (
				SELECT 
				distinct 
				u.uncid,
				i.iusd
				FROM sispacto3.universidadecadastro u 
				INNER JOIN sispacto3.estruturacurso e ON e.uncid = u.uncid 
				INNER JOIN sispacto3.abrangencia a ON a.ecuid = e.ecuid 
				INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
				INNER JOIN sispacto3.pactoidadecerta p ON p.estuf = m.estuf 
				INNER JOIN sispacto3.identificacaousuario i ON i.picid = p.picid 
				INNER JOIN sispacto3.tipoperfil t ON i.iusd = t.iusd 
				WHERE u.uncid='".$uncid."' AND esfera='E' AND i.uncid IS NULL ) foo WHERE foo.iusd = id.iusd";
		
		$db->executar($sql);
		
		$db->commit();
		
		gerarVersaoProjetoUniversidade(array('uncid'=>$uncid));
	}
	
	return true;
}

function carregarVersoes($dados) {
	global $db;
	$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"visualizarVersao(\''||v.vpnid||'\');\">' as acao,
				   u.usunome,
				   to_char(v.vpndata,'dd/mm/YYYY HH24:MI') as data
			FROM sispacto3.versoesprojetouniversidade v 
			INNER JOIN seguranca.usuario u ON u.usucpf = v.usucpf
			WHERE uncid='".$dados['uncid']."' ORDER BY v.vpndata DESC";
	$cabecalho = array("&nbsp","Usuário que inseriu versão","Data da inserção");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
}

function visualizarVersao($dados) {
	global $db;
	$sql = "SELECT vpnhtml FROM sispacto3.versoesprojetouniversidade WHERE vpnid='".$dados['vpnid']."'";
	$html = $db->pegaUm($sql);
	
	echo $html;
	
}

function carregarOuvintesFormacaoInicial($dados) {
	global $db;
	
	$sql = "SELECT '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"inserirOuvinte(\''||f.fioid||'\');\" > <img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirOuvinte(\''||fioid||'\');\">' as acao, fiocpf, fionome, fioemail, m.estuf||' / '||m.mundescricao as municipio,
				   CASE WHEN f.fioesfera='M' THEN 'Municipal'
				   		WHEN f.fioesfera='E' THEN 'Estadual' END as esfera,
				   	i.iusnome as substituido 
			FROM sispacto3.formacaoinicialouvintes f 
			LEFT JOIN territorios.municipio m ON m.muncod = f.muncod 
			LEFT JOIN sispacto3.identificacaousuario i ON i.iusd = f.iusd
			WHERE f.uncid='".$dados['uncid']."' AND f.fiostatus='A'";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email","UF / Município","Esfera","Irá substituir");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
}

function carregarUsuariosHabiliatadosTroca($dados) {
	global $db;
	$wh[] = "i.uncid='".$dados['uncid']."'";
	$wh[] = "(i.iusformacaoinicialorientador=false OR i.iusformacaoinicialorientador IS NULL)";
	if($dados['esfera']=='M') $wh[] = "i.picid IN(SELECT picid FROM sispacto3.pactoidadecerta WHERE muncod='".$dados['muncod']."')";
	elseif($dados['esfera']=='E') $wh[] = "i.picid IN(SELECT picid FROM sispacto3.pactoidadecerta WHERE estuf='".$dados['estuf']."')";
	
	$sql = "SELECT i.iusd as codigo, i.iusnome as descricao FROM sispacto3.identificacaousuario i 
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod='".PFL_ORIENTADORESTUDO."' 
			INNER JOIN sispacto3.orientadorturma o ON o.iusd = i.iusd 
			WHERE ".(($wh)?implode(" AND ",$wh):"")." ORDER BY i.iusnome";
	
	$db->monta_combo('iusd', $sql, 'S', 'NÃO É SUBSTITUTO, SOMENTE OUVINTE', '', '', '', '', 'S', 'iusd', '', $dados['iusd']);
}


function atualizarOuvinte($dados) {
	global $db;
	$sql = "UPDATE sispacto3.formacaoinicialouvintes SET  fiocpf='".str_replace(array(".","-"),array("",""),$dados['fiocpf'])."', 
            fionome='".$dados['fionome']."', 
            fioemail='".$dados['fioemail']."', 
            fioesfera='".$dados['fioesfera']."', 
            estuf='".$dados['estuf_endereco']."', 
            muncod='".$dados['muncod_endereco']."', 
            iusd=".(($dados['iusd'])?"'".$dados['iusd']."'":"NULL")." 
            WHERE fioid='".$dados['fioid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Ouvinte atualizado com sucesso.","javascript"=>"window.opener.carregarOuvintes();window.close();");
	alertlocation($al);
	
}


function inserirOuvinte($dados) {
	global $db;

	$sql = "INSERT INTO sispacto3.formacaoinicialouvintes(
            fiocpf, 
            fionome, 
            fioemail, 
            fioesfera, 
            estuf, 
            muncod, 
            iusd, 
            uncid)
    VALUES ('".str_replace(array(".","-"),array("",""),$dados['fiocpf'])."', 
    		'".$dados['fionome']."', 
    		'".$dados['fioemail']."', 
    		'".$dados['fioesfera']."', 
    		'".$dados['estuf_endereco']."', 
    		'".$dados['muncod_endereco']."', 
    		".(($dados['iusd'])?"'".$dados['iusd']."'":"NULL").", 
            '".$dados['uncid']."');";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Ouvinte inserido com sucesso.","javascript"=>"window.opener.carregarOuvintes();window.close();");
	alertlocation($al);
	
}

function excluirOuvinte($dados) {
	global $db;
	$sql = "UPDATE sispacto3.formacaoinicialouvintes SET fiostatus='I' WHERE fioid='".$dados['fioid']."'";
	
	$db->executar($sql);
	$db->commit();
	
}

function posDevolverCoordenadorIES() {
	global $db;
	
	if(!strstr($_SERVER['HTTP_HOST'],"simec-local")) {
	
		$sql = "SELECT c.cmddsc FROM workflow.documento d 
				INNER JOIN workflow.comentariodocumento c ON c.hstid = d.hstid 
				WHERE d.docid='".$_SESSION['sispacto3']['universidade']['docid']."'";
		
		$cmddsc = $db->pegaUm($sql);
		
		$sql = "SELECT iusnome, iusemailprincipal FROM sispacto3.identificacaousuario  
				WHERE iusd='".$_SESSION['sispacto3']['universidade']['iusd']."'";
		
		$identificacaousuario = $db->pegaLinha($sql);
		
		require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
		require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= "SIMEC";
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= "SIMEC - SISPACTO - Devolução do Projeto para alterações";
		
		$mensagem->AddAddress( $identificacaousuario['iusemailprincipal'], $identificacaousuario['iusnome'] );
		$mensagem->AddAddress( $_SESSION['email_sistema'] );
		
			
		$mensagem->Body = "<p>Prezado(a) ".$identificacaousuario['iusnome']." (Coordenador IES)</p>
						   <p>Seu projeto no SISPACTO foi analisado e foi devolvido para alterações.</p>
						   <p>As observações do avaliador foram:</p>".$cmddsc."<br/><p>Secretaria de Educação Básica<br/>Ministério da Educação</p>";
		
		$mensagem->IsHTML( true );
		$mensagem->Send();
	
	}
	
	return true;
	
}

function atualizarPlanoAtividade($dados) {
	global $db;
	if($dados['aundatainicio']) {
		foreach($dados['aundatainicio'] as $suaid => $aundatainicio) {
			if($aundatainicio) {
				$sql = "UPDATE sispacto3.atividadeuniversidade SET aundatainicio='".formata_data_sql($aundatainicio)."' WHERE suaid='".$suaid."' AND ecuid IN(SELECT ecuid FROM sispacto3.estruturacurso WHERE uncid='".$_SESSION['sispacto3']['universidade']['uncid']."')";
				$db->executar($sql);
			}		
		}
	}
	
	if($dados['aundatafim']) {
		foreach($dados['aundatafim'] as $suaid => $aundatafim) {
			if($aundatafim) {
				$sql = "UPDATE sispacto3.atividadeuniversidade SET aundatafim='".formata_data_sql($aundatafim)."' WHERE suaid='".$suaid."' AND ecuid IN(SELECT ecuid FROM sispacto3.estruturacurso WHERE uncid='".$_SESSION['sispacto3']['universidade']['uncid']."')";
				$db->executar($sql);
			}		
		}
	}
	
	$db->commit();
	
	$al = array("alert"=>"Plano de atividade atualizado com sucesso","location"=>"sispacto3.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=planoatividade");
	alertlocation($al);
	
}

function condicaoEnviarOrcamentoMec($uncid) {
	global $db;
	
	$sql = "SELECT ((coalesce(SUM(o.orcvlrexecutado),0) - coalesce(SUM(o.orcvlratualizado),0))) as preenchimento
			FROM sispacto3.orcamento o 
			INNER JOIN sispacto3.grupodespesa g ON g.gdeid = o.gdeid 
			WHERE o.uncid='".$uncid."' AND o.orcstatus='A'";
	
	$preenchimento = $db->pegaUm($sql);
	
	if(!$preenchimento) return "Preencha os dados referentes ao Valor Executado(R$) e Valor Atualizado(R$)";

	
	$sql = "SELECT ((SUM(o.orcvlrunitario)-coalesce(SUM(o.orcvlrexecutado),0) - coalesce(SUM(o.orcvlratualizado),0))) as diferenca
			FROM sispacto3.orcamento o 
			INNER JOIN sispacto3.grupodespesa g ON g.gdeid = o.gdeid 
			WHERE o.uncid='".$uncid."' AND o.orcstatus='A'";
	
	$diferenca = $db->pegaUm($sql);
	
	if($diferenca < 0) return "IES esta demandando mais recursos do que o valor total do projeto aprovado";
	return true;
	
}

function atualizarRelatorioFinal($dados) {
	global $db;
	
	$sql = "UPDATE sispacto3.relatoriofinal
			   SET rlfperexecinicio=".(($dados['rlfperexecinicio'])?"'".formata_data_sql($dados['rlfperexecinicio'])."'":"NULL").", 
			   	   rlfperexecfim=".(($dados['rlfperexecfim'])?"'".formata_data_sql($dados['rlfperexecfim'])."'":"").", 
			   	   rlfestruturafisica=".(($dados['rlfestruturafisica'])?"'".implode(";", $dados['rlfestruturafisica']).";'":"NULL").", 
			       rlfcolegiadocurso=".(($dados['rlfcolegiadocurso'])?"'".$dados['rlfcolegiadocurso']."'":"NULL").", 
			       rlfarticulacaoinstitucional=".(($dados['rlfarticulacaoinstitucional'])?"'".$dados['rlfarticulacaoinstitucional']."'":"NULL").", 
			       rlfcomentariosavaliacaocoordenadoreslocais=".(($dados['rlfarticulacaoinstitucional'])?"'".$dados['rlfarticulacaoinstitucional']."'":"NULL").", 
			       rlfplanejamentopedagogicocurso=".(($dados['rlfplanejamentopedagogicocurso'])?"'".$dados['rlfplanejamentopedagogicocurso']."'":"NULL").", 
			       rlforganizacaopedagogicacurso=".(($dados['rlforganizacaopedagogicacurso'])?"'".$dados['rlforganizacaopedagogicacurso']."'":"NULL").", 
			       rlfacompanhamentocursistas=".(($dados['rlfacompanhamentocursistas'])?"'".$dados['rlfacompanhamentocursistas']."'":"NULL").", 
			       rlfconteudocurso=".(($dados['rlfconteudocurso'])?"'".$dados['rlfconteudocurso']."'":"NULL").", 
			       rlfmetodologia=".(($dados['rlfmetodologia'])?"'".$dados['rlfmetodologia']."'":"NULL").", 
			       rlfcriteriosavaliacoes=".(($dados['rlfcriteriosavaliacoes'])?"'".$dados['rlfcriteriosavaliacoes']."'":"NULL").", 
			       rlfequipepedagogica=".(($dados['rlfequipepedagogica'])?"'".$dados['rlfequipepedagogica']."'":"NULL").", 
			       rlfarticulacaomec=".(($dados['rlfarticulacaomec'])?"'".$dados['rlfarticulacaomec']."'":"NULL").", 
			       rlflicoesaprendidas=".(($dados['rlflicoesaprendidas'])?"'".$dados['rlflicoesaprendidas']."'":"NULL").", 
			       rlfsugestoes=".(($dados['rlfsugestoes'])?"'".$dados['rlfsugestoes']."'":"NULL").", 
			       rlfoutroscomentarios=".(($dados['rlfoutroscomentarios'])?"'".$dados['rlfoutroscomentarios']."'":"NULL")."
			 WHERE uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'";
	
	$db->executar($sql);
	
	if($dados['orcvlrfinal']) {
		foreach($dados['orcvlrfinal'] as $orcid => $vlr) {
			$sql = "UPDATE sispacto3.orcamento SET orcvlrfinal=".(($vlr)?"'".str_replace(array(".",","),array("","."),$vlr)."'":"NULL")." 
					WHERE orcid='".$orcid."'";
			
			$db->executar($sql);
		}
	}
	
	if($dados['orcdescricao']) {
		foreach($dados['orcdescricao'] as $orcid => $dsc) {
			$sql = "UPDATE sispacto3.orcamento SET orcdescricaofinal=".(($dsc)?"'".$dsc."'":"NULL")."
					WHERE orcid='".$orcid."'";
				
			$db->executar($sql);
		}
	}
	
	
	
	$db->commit();
	
	$al = array("alert"=>"Relatório Final atualizado com sucesso","location"=>"sispacto3.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=relatoriofinal");
	alertlocation($al);
	
}



function exibirCursistasRelatorioFinal($dados) {
	global $db;
	
	$sql = "SELECT foo.acao, SUM(foo.resultados) FROM (
			SELECT CASE WHEN c.cerfrequencia>=75 THEN '<font style=font-size:xx-small;color:blue;>Recomendado</font>'
						ELSE '<font style=font-size:xx-small;color:red;>Não recomendado</font>' END as acao,
					1 as resultados
			FROM sispacto3.certificacao c 
			INNER JOIN sispacto3.identificacaousuario i ON i.iusd = c.iusd 
			WHERE i.uncid='".$dados['uncid']."' AND c.pflcod='".$dados['pflcod']."'
			) foo 
			GROUP BY foo.acao";
	
	echo '<table width=30% align=center>';
	echo '<tr><td>';
	$db->monta_lista_simples($sql,array(),10000000,10,'S','100%','center');
	echo '</td></tr>';
	echo '</table>';
	
	
	$sql = "SELECT
			   '<font style=font-size:xx-small;>'||CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END||'</font>' as iuscpf,
			   '<font style=font-size:xx-small;>'||i.iusnome||'</font>' as iusnome,
			   '<font style=font-size:xx-small;>'||i.iusemailprincipal||'</font>' as iusemailprincipal,
			   '<font style=font-size:xx-small;float:right;>'||c.cerfrequencia||'%</font>' as avl_freq,
			   CASE WHEN c.cerfrequencia>=75 THEN '<font style=font-size:xx-small;color:blue;>Recomendado</font>'
					ELSE '<font style=font-size:xx-small;color:red;>Não recomendado</font>' END as acao
			
			FROM sispacto3.certificacao c 
			INNER JOIN sispacto3.identificacaousuario i ON i.iusd = c.iusd
			
			WHERE i.uncid='".$dados['uncid']."' AND c.pflcod='".$dados['pflcod']."'";

	if($dados['xls']) {
		
		ob_clean();
		
		header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
		header("Pragma: no-cache");
		header("Content-type: application/xls; name=SIMEC_Certificacao_".date("Ymdhis").".xls");
		header("Content-Disposition: attachment; filename=SIMEC_Certificacao_".date("Ymdhis").".xls");
		header("Content-Description: MID Gera excel");
		
		$sql = "SELECT
			   CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END as iuscpf,
			   i.iusnome as iusnome,
			   i.iusemailprincipal as iusemailprincipal,
			   c.cerfrequencia||'%' as avl_freq,
			   CASE WHEN c.cerfrequencia>=75 THEN 'Recomendado'
					ELSE 'Não recomendado' END as acao
		
			FROM sispacto3.certificacao c
			INNER JOIN sispacto3.identificacaousuario i ON i.iusd = c.iusd
		
			WHERE i.uncid='".$dados['uncid']."' AND c.pflcod='".$dados['pflcod']."'";
		
		
		
		$db->monta_lista_tabulado($sql, $l, 10000000, 5, 'N', '100%', '');
		
		exit;
		
	} else {
		
		$sql = "SELECT
			   '<font style=font-size:xx-small;>'||CASE WHEN i.iuscpf ~ '^[0-9]*.?[0-9]*$' THEN replace(to_char(i.iuscpf::numeric, '000:000:000-00'), ':', '.') ELSE i.iuscpf END||'</font>' as iuscpf,
			   '<font style=font-size:xx-small;>'||i.iusnome||'</font>' as iusnome,
			   '<font style=font-size:xx-small;>'||i.iusemailprincipal||'</font>' as iusemailprincipal,
			   '<font style=font-size:xx-small;float:right;>'||c.cerfrequencia||'%</font>' as avl_freq,
			   CASE WHEN c.cerfrequencia>=75 THEN '<font style=font-size:xx-small;color:blue;>Recomendado</font>'
					ELSE '<font style=font-size:xx-small;color:red;>Não recomendado</font>' END as acao
		
			FROM sispacto3.certificacao c
			INNER JOIN sispacto3.identificacaousuario i ON i.iusd = c.iusd
		
			WHERE i.uncid='".$dados['uncid']."' AND c.pflcod='".$dados['pflcod']."'";
		
		
		$db->monta_lista_simples($sql,$l,10000000,10,'N','95%','center');
		
	}

}

function inserirEncontroPresencial($dados) {
	global $db;
	
	$sql = "INSERT INTO sispacto3.relatoriofinalencontrospresenciais(
            rlfid, repnome, repdata, repcargahoraria)
    		VALUES ((SELECT rlfid FROM sispacto3.relatoriofinal WHERE uncid='".$dados['uncid']."'), '".$dados['repnome']."', '".formata_data_sql($dados['repdata'])."', ".(($dados['repcargahoraria'])?"'".$dados['repcargahoraria']."'":"NULL").");";
	
	$db->executar($sql);
	$db->commit();
	
}

function listarEncontrosPresenciais($dados) {
	global $db;
	
	$sql = "SELECT '<center><img id=\"encontrop_'||repid||'\" src=../imagens/excluir.gif align=absmiddle style=cursor:pointer; onclick=excluirEncontroPresencial('||repid||');></center>' as acao, repnome, to_char(repdata,'dd/mm/YYYY') as repdata, repcargahoraria FROM sispacto3.relatoriofinalencontrospresenciais WHERE rlfid IN( SELECT rlfid FROM sispacto3.relatoriofinal WHERE uncid='".$dados['uncid']."' )";
	$cabecalho = array("&nbsp;","Evento","Data","Carga Horária");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','center');

}

function excluirEncontroPresencial($dados) {
	global $db;
	
	$sql = "DELETE FROM sispacto3.relatoriofinalencontrospresenciais WHERE repid='".$dados['repid']."'";
	$db->executar($sql);
	$db->commit();
}

function importarInformacoesSispacto($dados) {
	global $db;
	
	switch($dados['tela']) {
		case 'dados_projeto':
			
			$sql = "UPDATE sispacto3.universidadecadastro x SET uncdatainicioprojeto=foo.uncdatainicioprojeto, uncdatafimprojeto=foo.uncdatafimprojeto, unctipo=foo.unctipo, unctipocertificacao=foo.unctipocertificacao 
					FROM (
						SELECT * FROM sispacto2.universidadecadastro
					) foo WHERE x.uncid = foo.uncid AND x.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'";
			
			$db->executar($sql);
			$db->commit();
			
			break;
		case 'estrutura_curso':
			
			
			$sql = "UPDATE sispacto3.estruturacurso x SET muncod=foo.muncod, ecuobsplanoatividades=foo.ecuobsplanoatividades FROM (
					SELECT * FROM sispacto2.estruturacurso 
					) foo WHERE x.uncid = foo.uncid AND x.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'";
			
			$db->executar($sql);
			
			$db->executar("DELETE FROM sispacto3.articulacaoinstitucional WHERE ecuid IN(SELECT ecuid FROM sispacto3.estruturacurso WHERE uncid='".$_SESSION['sispacto3']['universidade']['uncid']."')");
			
			$sql = "INSERT INTO sispacto3.articulacaoinstitucional(
		            ecuid, ainseduc, ainseducjustificativa, ainundime, ainundimejustificativa,
		            ainuncme, ainuncmejustificativa, ainstatus)
					SELECT (SELECT ecuid FROM sispacto3.estruturacurso WHERE uncid=e.uncid) as ecuid, ainseduc, ainseducjustificativa, ainundime, ainundimejustificativa,
		            ainuncme, ainuncmejustificativa, ainstatus 
					FROM sispacto2.articulacaoinstitucional a 
					INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
					WHERE uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'";
		
			$db->executar($sql);
			
			$db->executar("DELETE FROM sispacto3.abrangencia WHERE ecuid IN(SELECT ecuid FROM sispacto3.estruturacurso WHERE uncid='".$_SESSION['sispacto3']['universidade']['uncid']."')");
			
			$sql = "INSERT INTO sispacto3.abrangencia(
            		muncod, ecuid, abrstatus, esfera)
    				SELECT a.muncod, (SELECT ecuid FROM sispacto3.estruturacurso WHERE uncid=e.uncid) as ecuid, 'A', a.esfera 
					FROM sispacto2.abrangencia a 
					INNER JOIN sispacto2.estruturacurso e ON e.ecuid = a.ecuid 
					LEFT JOIN sispacto3.abrangencia a2 ON a2.muncod = a.muncod  AND a2.esfera = a.esfera
					WHERE uncid='".$_SESSION['sispacto3']['universidade']['uncid']."' AND a2.abrid IS NULL";
			
			$db->executar($sql);
			
			$db->commit();
			
			break;
			
		case 'recursos_humanos':
			
			$imp_pfls = array(PFL_COORDENADORADJUNTOIES2014 => PFL_COORDENADORADJUNTOIES, PFL_SUPERVISORIES2014 => PFL_SUPERVISORIES, PFL_FORMADORIES2014 => PFL_FORMADORIES);
			
			foreach($imp_pfls as $pflcod2014 => $pflcod) {
				
				$sql = "SELECT i.iuscpf FROM sispacto2.identificacaousuario i 
						INNER JOIN sispacto2.tipoperfil t ON t.iusd = i.iusd 
						WHERE t.pflcod='".$pflcod2014."' AND i.iusstatus='A' AND i.uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'";
				
				$iuscpfs = $db->carregarColuna($sql);
				
				if($iuscpfs) {
					
					foreach($iuscpfs as $cpf) {
						
						$sql = "INSERT INTO sispacto3.identificacaousuario(
					            picid, muncod, eciid, nacid, fk_cod_docente, iuscpf, iusnome,
					            iussexo, iusdatanascimento, iusnomemae, iusnomeconjuge, iusagenciasugerida,
					            iusemailprincipal, iusemailopcional, iusdatainclusao, iuscadastrovalidadoies,
					            iussituacao, iusstatus, funid, iusagenciaend, iustipoorientador,
					            foeid, iustermocompromisso, tvpid, muncodatuacao, uncid, iusserieprofessor,
					            iusformacaoinicialorientador, cadastradosgb, iustipoprofessor,
					            iusdocumento, iusnaodesejosubstituirbolsa)
								SELECT i.picid, i.muncod, i.eciid, i.nacid, i.fk_cod_docente, i.iuscpf, i.iusnome,
					            i.iussexo, i.iusdatanascimento, i.iusnomemae, i.iusnomeconjuge, i.iusagenciasugerida,
					            i.iusemailprincipal, i.iusemailopcional, i.iusdatainclusao, i.iuscadastrovalidadoies,
					            i.iussituacao, i.iusstatus, i.funid, i.iusagenciaend, i.iustipoorientador,
					            i.foeid, i.iustermocompromisso, i.tvpid, i.muncodatuacao, i.uncid, i.iusserieprofessor,
					            false, false, i.iustipoprofessor,
					            i.iusdocumento, i.iusnaodesejosubstituirbolsa FROM sispacto2.identificacaousuario i 
								LEFT JOIN sispacto3.identificacaousuario i2 ON i.iuscpf = i2.iuscpf 
								LEFT JOIN sispacto3.tipoperfil t2 ON t2.iusd = i2.iusd 
								WHERE i.iuscpf='".$cpf."' AND i2.iusd IS NULL AND t2.tpeid IS NULL
					            RETURNING iusd";
						
						$iusd = $db->pegaUm($sql);
						
						if(!$iusd) {
							$iusd = $db->pegaUm("SELECT iusd FROM sispacto3.identificacaousuario WHERE iuscpf='".$cpf."'");
						}
						
						$db->executar("DELETE FROM sispacto3.portarianomeacao WHERE iusd='".$iusd."'");
						$db->executar("DELETE FROM sispacto3.identificacaotelefone WHERE iusd='".$iusd."'");
						$db->executar("DELETE FROM sispacto3.identificaoendereco WHERE iusd='".$iusd."'");
						$db->executar("DELETE FROM sispacto3.identiusucursoformacao WHERE iusd='".$iusd."'");
						$db->executar("DELETE FROM sispacto3.identusutipodocumento WHERE iusd='".$iusd."'");
						
						$sql = "INSERT INTO sispacto3.tipoperfil(
					            iusd, pflcod, tpestatus)
							    VALUES (
								'".$iusd."', '".$pflcod."', 'A');";
						
						$db->executar($sql);
						
						$sql = "INSERT INTO sispacto3.portarianomeacao(
		            			arqid, iusd, ponstatus)
		    					SELECT arqid, '".$iusd."', ponstatus FROM sispacto2.portarianomeacao WHERE iusd IN( SELECT iusd FROM sispacto.identificacaousuario WHERE iuscpf='".$cpf."' )";
						
						$db->executar($sql);
						
						$sql = "INSERT INTO sispacto3.identificacaotelefone(
			            		iusd, itedddtel, itenumtel, itetipo, itestatus)
			    				SELECT '".$iusd."', itedddtel, itenumtel, itetipo, itestatus FROM sispacto2.identificacaotelefone WHERE iusd IN( SELECT iusd FROM sispacto.identificacaousuario WHERE iuscpf='".$cpf."' )";
						
						$db->executar($sql);
						
						$sql = "INSERT INTO sispacto3.identificaoendereco(
					            muncod, iusd, ientipo, iencep, iencomplemento, iennumero, 
					            iensatatus, ienlogradouro, ienbairro)
								SELECT muncod, '".$iusd."', ientipo, iencep, iencomplemento, iennumero, 
					            iensatatus, ienlogradouro, ienbairro FROM sispacto2.identificaoendereco WHERE iusd IN( SELECT iusd FROM sispacto.identificacaousuario WHERE iuscpf='".$cpf."' )";
						
						$db->executar($sql);
						
						$sql = "INSERT INTO sispacto3.identiusucursoformacao(
					            iusd, cufid, iufdatainiformacao, iufdatafimformacao, iufsituacaoformacao, 
					            iufstatus)
								SELECT '".$iusd."', cufid, iufdatainiformacao, iufdatafimformacao, iufsituacaoformacao, 
					            iufstatus FROM sispacto2.identiusucursoformacao WHERE iusd IN( SELECT iusd FROM sispacto2.identificacaousuario WHERE iuscpf='".$cpf."' )";
			
						$db->executar($sql);
						
						$sql = "INSERT INTO sispacto3.identusutipodocumento(
					            iusd, tdoid, itdufdoc, itdnumdoc, itddataexp, itdnoorgaoexp, 
					            itdstatus)
								SELECT '".$iusd."', tdoid, itdufdoc, itdnumdoc, itddataexp, itdnoorgaoexp, 
					            itdstatus FROM sispacto2.identusutipodocumento WHERE iusd IN( SELECT iusd FROM sispacto2.identificacaousuario WHERE iuscpf='".$cpf."' )";
						
						$db->executar($sql);
						
						$db->commit();
						
						
						
						
					}
				}
				
				
			}
			
			break;
		case 'orcamento':
			
			$sql = "INSERT INTO sispacto3.orcamento(
				            uncid, orcstatus, orcquantidade, orcvlrunitario, orcvlrtotal, 
				            gdeid, orcdescricao, orcvlrexecutado, orcvlratualizado, orcvlrfinal, 
				            orcdescricaofinal)
				    SELECT uncid, orcstatus, orcquantidade, orcvlrunitario, orcvlrtotal, 
				            gdeid, orcdescricao, orcvlrexecutado, orcvlratualizado, orcvlrfinal, 
				            orcdescricaofinal FROM sispacto2.orcamento WHERE uncid='".$_SESSION['sispacto3']['universidade']['uncid']."'";

			$db->executar($sql);
			
			$db->commit();
				
			break;
		
	}
	
	$al = array("alert"=>"Importação efetuada com sucesso","location"=>"sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=".$dados['tela']);
	alertlocation($al);

}

function esconderAba($dados) {
	return false;
}

function carregarMunicipiosPorUFAbrangenciaIES($dados) {
	global $db;
	$sql = "SELECT DISTINCT m.muncod as codigo, m.mundescricao as descricao FROM territorios.municipio m 
			INNER JOIN sispacto3.abrangencia a ON a.muncod = m.muncod 
			INNER JOIN sispacto3.estruturacurso e ON e.ecuid = a.ecuid 
			WHERE m.estuf='".$dados['estuf']."' AND e.uncid='".$dados['uncid']."' ORDER BY m.mundescricao";
	$combo = $db->monta_combo($dados['name'], $sql, 'S', 'Selecione', (($dados['onclick'])?$dados['onclick']:''), '', '', '200', 'S', $dados['id'], true, $dados['valuecombo']);

	if($dados['returncombo']) return $combo;
	else echo $combo;
}

function carregarEncontroPresencialCoordenadorIES($dados) {
	global $db;

	if($_SESSION['sispacto3']['universidade']['iusd']) {

		$sql = "SELECT '".(($dados['consulta'])?"":"<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"removerEncontroPresencial('||aofid||');\">")."' as acao, aofnome, to_char(aofdata,'dd/mm/YYYY') as aofdata, aofcargahoraria FROM sispacto3.avaliacaofinalcgencontropresencial WHERE iusd='".$_SESSION['sispacto3']['universidade']['iusd']."'";
		$cabecalho = array("&nbsp;","Nome do evento","Data do evento","Carga horária");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);

	} else {
		echo '<p>PROBLEMAS PARA CARREGAR OS ENCONTROS PRESENCIAIS. TENTE NOVAMENTE.</p>';
	}
}


function gravarAvaliacaoFinalCG($dados) {
	global $db;

	if($dados['orcvalorexecutado']) {
		foreach($dados['orcvalorexecutado'] as $orcid => $vl) {
			$sql = "UPDATE sispacto3.orcamento SET orcvlrexecutado=".(($vl)?"'".str_replace(array(".",","),array("","."),$vl)."'":"NULL")." WHERE orcid='".$orcid."'";
			$db->executar($sql);
		}
	}


	$afcid = $db->pegaUm("SELECT afcid FROM sispacto3.avaliacaofinalcg WHERE iusd='".$_SESSION['sispacto3']['universidade']['iusd']."'");

	if($afcid) {

		$sql = "UPDATE sispacto3.avaliacaofinalcg SET
					afccertificacaoini=".(($dados['afccertificacaoini'])?"'".formata_data_sql($dados['afccertificacaoini'])."'":"NULL").",
					afccertificacaofim=".(($dados['afccertificacaofim'])?"'".formata_data_sql($dados['afccertificacaofim'])."'":"NULL").",
					afclogistico=".(($dados['afclogistico'])?"'".trim(implode(";",$dados['afclogistico']))."'":"NULL").",
            		afcpedagogico_4_a=".(($dados['afcpedagogico_4_a'])?"'".$dados['afcpedagogico_4_a']."'":"NULL").",
					afclogistico_5_a=".(($dados['afclogistico_5_a'])?"'".$dados['afclogistico_5_a']."'":"NULL").",
					afcconteudo_1=".(($dados['afcconteudo_1'])?"'".$dados['afcconteudo_1']."'":"NULL").",
					afcconteudo_2=".(($dados['afcconteudo_2'])?"'".$dados['afcconteudo_2']."'":"NULL").",
					afcconteudo_1_a=".(($dados['afcconteudo_1_a'])?"'".$dados['afcconteudo_1_a']."'":"NULL").",
					afcconteudo_2_a=".(($dados['afcconteudo_2_a'])?"'".$dados['afcconteudo_2_a']."'":"NULL").",
					afcconteudo_3=".(($dados['afcconteudo_3'])?"'".$dados['afcconteudo_3']."'":"NULL").",
					afcpedagogico_1=".(($dados['afcpedagogico_1'])?"'".addslashes($dados['afcpedagogico_1'])."'":"NULL").",
					afcpedagogico_2=".(($dados['afcpedagogico_2'])?"'".addslashes($dados['afcpedagogico_2'])."'":"NULL").",
					afcpedagogico_3=".(($dados['afcpedagogico_3'])?"'".addslashes($dados['afcpedagogico_3'])."'":"NULL").",
					afcpedagogico_4=".(($dados['afcpedagogico_4'])?"'".addslashes($dados['afcpedagogico_4'])."'":"NULL").",
					afcconsideracoesfinais_4=".(($dados['afcconsideracoesfinais_4'])?"'".addslashes($dados['afcconsideracoesfinais_4'])."'":"NULL")."
					WHERE afcid={$afcid}";

	} else {
		
		$docid = wf_cadastrarDocumento(TPD_FLUXORELATORIOFINAL,"Relatório Final ".$_SESSION['sispacto3']['universidade']['iusd']);

		$sql = "INSERT INTO sispacto3.avaliacaofinalcg(
		            iusd,
					afccertificacaoini,
					afccertificacaofim,
					afclogistico,
		            afcpedagogico_4_a,
					afclogistico_5_a,
					afcconteudo_1,
					afcconteudo_2,
					afcconteudo_1_a,
					afcconteudo_2_a,
					afcconteudo_3,
					afcpedagogico_1,
					afcpedagogico_2,
					afcpedagogico_3,
					afcpedagogico_4,
					afcconsideracoesfinais_4,
					docid)
				    VALUES ('".$_SESSION['sispacto3']['universidade']['iusd']."',
							".(($dados['afccertificacaoini'])?"'".formata_data_sql($dados['afccertificacaoini'])."'":"NULL").",
							".(($dados['afccertificacaofim'])?"'".formata_data_sql($dados['afccertificacaofim'])."'":"NULL").",
							".(($dados['afclogistico'])?"'".implode(";",$dados['afclogistico'])."'":"NULL").",
				            ".(($dados['afcpedagogico_4_a'])?"'".$dados['afcpedagogico_4_a']."'":"NULL").",
							".(($dados['afclogistico_5_a'])?"'".$dados['afclogistico_5_a']."'":"NULL").",
							".(($dados['afcconteudo_1'])?"'".$dados['afcconteudo_1']."'":"NULL").",
							".(($dados['afcconteudo_2'])?"'".$dados['afcconteudo_2']."'":"NULL").",
							".(($dados['afcconteudo_1_a'])?"'".$dados['afcconteudo_1_a']."'":"NULL").",
						    ".(($dados['afcconteudo_2_a'])?"'".$dados['afcconteudo_2_a']."'":"NULL").",
							".(($dados['afcconteudo_3'])?"'".$dados['afcconteudo_3']."'":"NULL").",
							".(($dados['afcpedagogico_1'])?"'".addslashes($dados['afcpedagogico_1'])."'":"NULL").",
							".(($dados['afcpedagogico_2'])?"'".addslashes($dados['afcpedagogico_2'])."'":"NULL").",
							".(($dados['afcpedagogico_3'])?"'".addslashes($dados['afcpedagogico_3'])."'":"NULL").",
							".(($dados['afcpedagogico_4'])?"'".addslashes($dados['afcpedagogico_4'])."'":"NULL").",
							".(($dados['afcconsideracoesfinais_4'])?"'".addslashes($dados['afcconsideracoesfinais_4'])."'":"NULL").",
							{$docid}
		
					);";

	}

	$db->executar($sql);
	$db->commit();

	$al = array("alert"=>"Avaliação final gravada com sucesso","location"=>"sispacto3.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=avaliacaofinal");
	alertlocation($al);


}

function gravarEncontroPresencialCG($dados) {
	global $db;

	$sql = "INSERT INTO sispacto3.avaliacaofinalcgencontropresencial(
            iusd, aofnome, aofdata, aofcargahoraria)
    		VALUES ('".$_SESSION['sispacto3']['universidade']['iusd']."', '".utf8_decode($dados['aofnome'])."', '".formata_data_sql($dados['aofdata'])."', '".$dados['aofcargahoraria']."');";

	$db->executar($sql);
	$db->commit();
}

function removerEncontroPresencialCoordenadorIES($dados) {
	global $db;

	$sql = "DELETE FROM sispacto3.avaliacaofinalcgencontropresencial WHERE aofid='".$dados['aofid']."'";
	$db->executar($sql);
	$db->commit();

}

function atualizarParecer($dados) {
	global $db;
	
	$sql = "UPDATE sispacto3.universidadecadastro SET uncparecer=".(($dados['uncparecer'])?"'".$dados['uncparecer']."'":"NULL").", usucpfparecer='".$_SESSION['usucpf']."', uncparecerdata=NOW() WHERE uncid='".$dados['uncid']."'";
	$db->executar($sql);
	$db->commit();
	
	gerarVersaoProjetoUniversidade(array('uncid' => $dados['uncid']));
	
	$sql = "UPDATE sispacto3.identificacaousuario x set uncid=xx.ecu FROM (
			select i.uncid as ius, e.uncid as ecu, i.iusd FROM sispacto3.identificacaousuario i
			inner join sispacto3.tipoperfil t on t.iusd = i.iusd
			inner join sispacto3.pactoidadecerta p on p.picid = i.picid
			inner join sispacto3.abrangencia a on a.muncod = p.muncod and esfera='M'
					inner join sispacto3.estruturacurso e on e.ecuid = a.ecuid
					inner join sispacto3.universidadecadastro u on u.uncid = e.uncid
					inner join workflow.documento d on d.docid = u.docidestruturaformacao
					where (i.uncid is null or i.uncid != e.uncid) and d.esdid=".ESD_VALIDADO_COORDENADOR_IES." and e.uncid='".$dados['uncid']."'
			) xx where xx.iusd = x.iusd";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "UPDATE sispacto3.identificacaousuario x set uncid=xx.ecu FROM (
			select i.uncid as ius, e.uncid as ecu, i.iusd FROM sispacto3.identificacaousuario i
			inner join sispacto3.tipoperfil t on t.iusd = i.iusd
			inner join sispacto3.pactoidadecerta p on p.picid = i.picid and p.estuf is not null
			inner join territorios.municipio m on m.muncod = i.muncodatuacao
			inner join sispacto3.abrangencia a on a.muncod = m.muncod and esfera='E'
					inner join sispacto3.estruturacurso e on e.ecuid = a.ecuid
					inner join sispacto3.universidadecadastro u on u.uncid = e.uncid
					inner join workflow.documento d on d.docid = u.docidestruturaformacao
					where (i.uncid is null or i.uncid != e.uncid) and d.esdid=".ESD_VALIDADO_COORDENADOR_IES." and e.uncid='".$dados['uncid']."'
				
			) xx where xx.iusd = x.iusd";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Parecer atualizado com sucesso","location"=>"sispacto3.php?modulo=principal/universidade/universidade&acao=A&aba=visualizacao_projeto");
	alertlocation($al);
	
	
}




?>