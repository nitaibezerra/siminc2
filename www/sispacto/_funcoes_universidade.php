<?

function excluirAbrangencia($dados) {
	global $db;
	
	$sql = "SELECT * FROM sispacto.abrangencia WHERE abrid='".$dados['abrid']."'";
	$abr = $db->pegaLinha($sql);
	
	if($abr['esfera']=='M') {
		
		$sql = "DELETE FROM sispacto.orientadorturma WHERE iusd IN(
				select i.iusd from sispacto.identificacaousuario i 
				inner join sispacto.pactoidadecerta p on p.picid = i.picid
				inner join sispacto.tipoperfil t on t.iusd = i.iusd 
				where pflcod=".PFL_ORIENTADORESTUDO." and p.muncod='".$abr['muncod']."')";
		
		$db->executar($sql);
	}
	
	if($abr['esfera']=='E') {
		
		$sql = "DELETE FROM sispacto.orientadorturma WHERE iusd IN(
				select i.iusd from sispacto.identificacaousuario i 
				inner join territorios.municipio mm on mm.muncod = i.muncodatuacao
				inner join sispacto.pactoidadecerta p on p.estuf = mm.estuf and p.picid = i.picid 
				inner join sispacto.tipoperfil t on t.iusd = i.iusd 
				where pflcod=".PFL_ORIENTADORESTUDO." and mm.muncod='".$abr['muncod']."')";
		
		$db->executar($sql);
		
	}
	
	$sql = "DELETE FROM sispacto.abrangencia WHERE abrid='".$dados['abrid']."'";
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
			(select count(*) from sispacto.identificacaousuario i 
			inner join sispacto.pactoidadecerta p on p.picid = i.picid
			inner join sispacto.tipoperfil t on t.iusd = i.iusd 
			where pflcod=".PFL_ORIENTADORESTUDO." and p.muncod=a.muncod) as redemunicipal,
			COALESCE(esd.esddsc,'N�o iniciado') as situacao,
			COALESCE(array_to_string(array(SELECT iusnome FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=pp.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'Coordenador Local n�o cadastrado') as coordenadorlocal,
			COALESCE(array_to_string(array(SELECT '('||itedddtel||') '||itenumtel as tel FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd INNER JOIN sispacto.identificacaotelefone it ON it.iusd = i.iusd AND it.itetipo='C' WHERE i.picid=pp.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'-') as telefonecoordenadorlocal,
			COALESCE(array_to_string(array(SELECT iusemailprincipal FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=pp.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'-') as emailcoordenadorlocal
			FROM sispacto.abrangencia a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			LEFT JOIN sispacto.pactoidadecerta pp ON pp.muncod = m.muncod 
			LEFT JOIN workflow.documento dd ON dd.docid = pp.docid 
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = dd.esdid 
			WHERE a.ecuid='".$dados['ecuid']."' AND a.esfera='M'
			ORDER BY 3";
	
	$cabecalho = array("&nbsp;","&nbsp;","UF/ Munic�pio","Orientadores","Situa��o","Coordenador Local","Telefone","Email");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	
	if($pp['nummunpendencias']>0) echo "<p style=\"color:red\"><img src=\"../imagens/atencao.png\" border=\"0\" align=\"absmiddle\"> H� ".$pp['nummunpendencias']." munic�pio(s) que n�o concluiu/ conclu�ram o cadastramento do(s) seu(s) Orientador(es) de Estudo.</p>";
	
	echo "<p><b>Rede Estadual</b></p>";
	
	$sql = "SELECT
			'<center><img src=\"../imagens/mais.gif\" id=\"abran_est_'||m.muncod||'\" title=\"mais\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"abrirDetalhamentoAbrangencia(\''||a.muncod||'\',\'E\',this);\">' as acao1,
			".((!$dados['consulta'])?"'<center><img src=\"../imagens/excluir.gif\" border=\"0\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirAbrangencia('||a.abrid||');\"></center>'":"''")." as acao2,
			m.estuf||' - '||m.mundescricao as descricao,
			(select count(*) from sispacto.identificacaousuario i 
			inner join territorios.municipio mm on mm.muncod = i.muncodatuacao
			inner join sispacto.pactoidadecerta p on p.estuf = mm.estuf and p.picid = i.picid 
			inner join sispacto.tipoperfil t on t.iusd = i.iusd 
			where pflcod=".PFL_ORIENTADORESTUDO." and mm.muncod=a.muncod) as redeestadual,
			COALESCE(esd.esddsc,'N�o iniciado') as situacao,
			COALESCE(array_to_string(array(SELECT iusnome FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=pp.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'Coordenador Local n�o cadastrado') as coordenadorlocal,
			COALESCE(array_to_string(array(SELECT '('||itedddtel||') '||itenumtel as tel FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd INNER JOIN sispacto.identificacaotelefone it ON it.iusd = i.iusd AND it.itetipo='C' WHERE i.picid=pp.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'-') as telefonecoordenadorlocal,
			COALESCE(array_to_string(array(SELECT iusemailprincipal FROM sispacto.identificacaousuario i INNER JOIN sispacto.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=pp.picid AND t.pflcod=".PFL_COORDENADORLOCAL."), ','),'-') as emailcoordenadorlocal
			FROM sispacto.abrangencia a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sispacto.pactoidadecerta pp ON pp.estuf = m.estuf 
			INNER JOIN workflow.documento dd ON dd.docid = pp.docid 
			INNER JOIN workflow.estadodocumento esd ON esd.esdid = dd.esdid  
			WHERE a.ecuid='".$dados['ecuid']."' AND a.esfera='E'
			ORDER BY 3";
	
	$cabecalho = array("&nbsp;","&nbsp;","UF/ Munic�pio","Orientadores","Situa��o","Coordenador Local","Telefone","Email");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	if($pp['numestpendencias']>0) echo "<p style=\"color:red\"><img src=\"../imagens/atencao.png\" border=\"0\" align=\"absmiddle\"> H� ".$pp['numestpendencias']." estado(s) que n�o concluiu/ conclu�ram o cadastramento do(s) seu(s) Orientador(es) de Estudo.</p>";	
	
	echo "<br>";
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	echo "<tr><td class=\"SubTituloDireita\" width=\"20%\"><b>Total de Orientadores de Estudo:</b></td><td>".$total."</td></tr>";
	echo "<tr><td class=\"SubTituloDireita\" width=\"20%\"><b>N�mero de Turmas Estimado:</b></td><td>".$nturmas."</td></tr>";
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
	
	$sql = "SELECT COUNT(*) FROM sispacto.abrangencia a 
			LEFT JOIN sispacto.estruturacurso e ON e.ecuid = a.ecuid
			LEFT JOIN sispacto.pactoidadecerta p ON p.muncod=a.muncod
			LEFT JOIN workflow.documento d ON d.docid = p.docid 
			LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
			WHERE ".(($dados['uncid'])?"e.uncid='".$dados['uncid']."' AND":"")." ".(($dados['ecuid'])?"a.ecuid='".$dados['ecuid']."' AND":"")." a.esfera='M' AND (d.esdid IN('".ESD_ELABORACAO_COORDENADOR_LOCAL."','".ESD_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL."','".ESD_ANALISE_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL."') OR d.esdid IS NULL)";
	
	$nummunpendencias = $db->pegaUm($sql);
	
	$sql = "SELECT COUNT(DISTINCT mm.estuf) FROM sispacto.pactoidadecerta p 
			INNER JOIN territorios.municipio mm ON mm.estuf = p.estuf  
			LEFT JOIN workflow.documento d ON d.docid = p.docid 
			LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
			INNER JOIN sispacto.abrangencia a ON mm.muncod=a.muncod 
			LEFT JOIN sispacto.estruturacurso e ON e.ecuid = a.ecuid 
			WHERE ".(($dados['uncid'])?"e.uncid='".$dados['uncid']."' AND":"")." ".(($dados['ecuid'])?"a.ecuid='".$dados['ecuid']."' AND":"")." (d.esdid IN('".ESD_ELABORACAO_COORDENADOR_LOCAL."','".ESD_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL."','".ESD_ANALISE_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL."') OR d.esdid IS NULL) AND a.esfera='E'
			GROUP BY p.estuf";
	
	$numestpendencias = $db->pegaUm($sql);
	
	return array("nummunpendencias" => $nummunpendencias, "numestpendencias" => $numestpendencias);	
	
}

function totalAlfabetizadoresAbrangencia($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*) FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.pactoidadecerta p ON p.picid = i.picid
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sispacto.abrangencia a ON p.muncod = a.muncod 
			INNER JOIN sispacto.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid = '".$dados['uncid']."' AND a.esfera='M'";
	
	$totalmunicipio = $db->pegaUm($sql);
	
	$sql = "SELECT COUNT(*) FROM sispacto.identificacaousuario i 
			INNER JOIN  territorios.municipio mm ON mm.muncod = i.muncodatuacao
			INNER JOIN sispacto.pactoidadecerta p ON p.estuf = mm.estuf AND p.picid = i.picid
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sispacto.abrangencia a ON mm.muncod = a.muncod 
			INNER JOIN sispacto.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid = '".$dados['uncid']."' AND a.esfera='E'";
	
	$totalestado = $db->pegaUm($sql);
	
	$total = $totalmunicipio + $totalestado;
	
	return $total;
}

function cadastrarMunicipioAbrangencia($dados) {
	global $db;
	
	$municipio_abrangencia = $db->carregarColuna("SELECT 'N�o foi poss�vel vincular o munic�pio - '||m.mundescricao||' esta cadastrado na Universidade: '||un.uninome as descricao FROM sispacto.abrangencia a 
						 						  INNER JOIN sispacto.estruturacurso e ON e.ecuid = a.ecuid 
						 						  INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
						 						  INNER JOIN sispacto.universidadecadastro u ON u.uncid = e.uncid 
						 						  INNER JOIN sispacto.universidade un ON un.uniid = u.uniid  
						 						  WHERE uncstatus='A' AND a.muncod IN('".implode("','",$dados['muncod_abrangencia'])."') AND a.esfera='".$dados['esfera']."' LIMIT 10");
	
	if(!$municipio_abrangencia) {
		
		foreach($dados['muncod_abrangencia'] as $muncod) {
			if($muncod) {
				
				$sql = "INSERT INTO sispacto.abrangencia(
			            muncod, ecuid, abrstatus, esfera)
			    		VALUES ('".$muncod."', '".$dados['ecuid']."', 'A', '".$dados['esfera']."');";
				
				$db->executar($sql);
				
			}
		}
	
		$db->commit();
		
 		$al = array("alert"=>"Munic�pios gravado com sucesso","javascript"=>"window.opener.definirAbrangencia();window.close();");
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
		FROM sispacto.universidadecadastro un 
		INNER JOIN sispacto.universidade su ON su.uniid = un.uniid 
		LEFT JOIN territorios.municipio mu ON mu.muncod = su.muncod 
		INNER JOIN sispacto.reitor re on re.uniid = su.uniid 
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
	
	$sql = "UPDATE sispacto.universidadecadastro SET uncdatainicioprojeto='".formata_data_sql($dados['uncdatainicioprojeto'])."',
													 uncdatafimprojeto='".formata_data_sql($dados['uncdatafimprojeto'])."',
													 unctipo='".$dados['unctipo']."',
													 unctipocertificacao='".$dados['unctipocertificacao']."' 
		    WHERE uncid='".$dados['uncid']."'";
	
	$db->executar($sql);

	$sql = "UPDATE sispacto.universidade
   			SET unisigla='".$dados['unisigla']."', uninome='".$dados['uninome']."', unicnpj='".str_replace(array(".","-","/"),array("","",""),$dados['unicnpj'])."', unicep='".str_replace(array("-"),array(""),$dados['unicep'])."', 
		        unilogradouro='".$dados['unilogradouro']."', unibairro='".$dados['unibairro']."', unicomplemento=".(($dados['unicomplemento'])?"'".$dados['unicomplemento']."'":"NULL").", unidddcomercial='".$dados['unidddcomercial']."', 
       			uninumcomercial='".$dados['uninumcomercial']."', uniemail='".$dados['uniemail']."', uniuf='".$dados['uniuf']."', uninumero='".$dados['uninumero']."', 
       			unisite='".$dados['unisite']."', muncod='".$dados['muncod_endereco']."'
 			WHERE uniid IN(SELECT uniid FROM sispacto.universidadecadastro WHERE uncid='".$dados['uncid']."')";
	
	$db->executar($sql);
	
	$sql = "UPDATE sispacto.reitor
   			SET reinome='".$dados['reinome']."', reicpf='".str_replace(array(".","-"),array("",""),$dados['reicpf'])."', reidddcomercial='".$dados['reidddcomercial']."', reinumcomercial='".$dados['reinumcomercial']."', 
       			reiemail='".$dados['reiemail']."'
 			WHERE uniid IN(SELECT uniid FROM sispacto.universidadecadastro WHERE uncid='".$dados['uncid']."')";
	
	$db->executar($sql);
	
	$db->commit();
	
 	$al = array("alert"=>"Dados Gerais do Projeto inseridos com sucesso.","location"=>$dados['goto']);
 	alertlocation($al);
	
}



function inserirCoordenadorIESGerenciamento($dados) {
	global $db;
	
	$sql = "SELECT iusd FROM sispacto.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'";
 	$iusd = $db->pegaUm($sql);
	
 	if($iusd) {
 		$sql = "UPDATE sispacto.identificacaousuario SET iusstatus='A', iusemailprincipal='".$dados['iusemailprincipal']."' WHERE iusd='".$iusd."'";
 		$db->executar($sql);
 	} else {
     	$sql = "INSERT INTO sispacto.identificacaousuario(
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
 		
 		$remetente = array("nome" => "SIMEC - M�DULO SISPACTO","email" => $dados['iusemailprincipal']);
 		$destinatario = $dados['iusemailprincipal'];
 		$usunome = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
 		$assunto = "Cadastro no SIMEC - M�DULO SISPACTO";
 		$conteudo = "<br/><span style='background-color: red;'><b>Esta � uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, n�o responda. Pois, neste caso, a mesma ser� descartada.</b></span><br/><br/>";
 		$conteudo .= sprintf("%s %s, <p>Voc� foi cadastrado no SIMEC, m�dulo SISPACTO. Sua conta est� ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
 							  <p>Se for o seu primeiro acesso, o sistema solicitar� que voc� crie uma nova senha. Se voc� j� tiver cadastro no SIMEC, insira o seu CPF e senha. Caso tenha esquecido a sua senha de acesso ao SIMEC, clique em \"Esqueceu a senha?\" e insira o seu CPF. O sistema enviar� a sua nova senha para o e-mail que voc� cadastrou. Em caso de d�vida, entre em contato com a sua Secretaria de Educa��o.</p>
 							  <p>Sua Senha de acesso �: %s</p>
 							  <br><br>* Caso voc� j� alterou a senha acima, favor desconsiderar este e-mail.",
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
   	
    $existe_usr = $db->pegaUm("select usucpf from sispacto.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and pflcod='".PFL_COORDENADORIES."' AND rpustatus='A' AND uncid='".$dados['uncid']."'");
    
     if(!$existe_usr) {
    		$sql = "INSERT INTO sispacto.usuarioresponsabilidade(
            		pflcod, usucpf, rpustatus, rpudata_inc, uncid)
 			    VALUES ('".PFL_COORDENADORIES."', '".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', 'A', NOW(), '".$dados['uncid']."');";
    		$db->executar($sql);
     }
    
     $arrTp = $db->pegaLinha("SELECT
     							  t.pflcod, 
     							  p.pfldsc, 
     							  uni.unisigla || '/' || uni.uninome as descricao 
     						FROM sispacto.tipoperfil t 
     						INNER JOIN seguranca.perfil p ON p.pflcod=t.pflcod 
     						INNER JOIN sispacto.identificacaousuario i ON i.iusd=t.iusd 
     						INNER JOIN sispacto.universidadecadastro unc ON unc.uncid=i.uncid 
     						LEFT JOIN sispacto.universidade uni ON uni.uniid=unc.uniid 
     						WHERE t.iusd='".$iusd."'");
     
     $existe_tpf = $arrTp['pflcod'];
    
     if(!$existe_tpf) {
 		$sql = "INSERT INTO sispacto.tipoperfil(
 		            iusd, pflcod, tpestatus)
 		    	VALUES ('".$iusd."', '".PFL_COORDENADORIES."', 'A');";
     	$db->executar($sql);
     } else {
    	
     	if($existe_tpf!=PFL_COORDENADORIES) {
 			$al = array("alert"=>"Este CPF ja possui um perfil (".$arrTp['pfldsc'].",".$arrTp['descricao'].") no sistema e n�o pode ser cadastrado","location"=>"sispacto.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=".$dados['picid']);
 			alertlocation($al);
     	}
    	
     }
    
     $db->commit();
			
 	$al = array("alert"=>"Coordenador-Geral da IES inserido com sucesso","location"=>"sispacto.php?modulo=principal/universidade/gerenciarcoordenadories&acao=A&uncid=".$dados['uncid']);
 	alertlocation($al);
	
 }





function carregarCoordenadorIES($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT d.esdid, u.uncid, re.reiid, su.uniuf, u.curid, u.docid, su.unisigla||' - '||su.uninome as descricao, u.docidformacaoinicial FROM sispacto.universidadecadastro u 
					 	   INNER JOIN sispacto.universidade su ON su.uniid = u.uniid
						   INNER JOIN sispacto.reitor re on re.uniid = su.uniid 
						   LEFT JOIN workflow.documento d ON d.docid = u.docid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$docid = $arr['docid'];
	
	if(!$docid) {
		$docid = wf_cadastrarDocumento(TPD_ORIENTADORIES,"Sispacto_CoordenadorIES_uncid".$dados['uncid']);
		$db->executar("UPDATE sispacto.universidadecadastro SET docid='".$docid."' WHERE uncid='".$dados['uncid']."'");
		$db->commit();
	}
	
	$docidformacaoinicial = $arr['docidformacaoinicial'];
	
	if(!$docidformacaoinicial) {
		$docidformacaoinicial = wf_cadastrarDocumento(TPD_FORMACAOINICIAL,"Sispacto_CoordenadorIES_uncid".$dados['uncid']."_Formacaoinicial");
		$db->executar("UPDATE sispacto.universidadecadastro SET docidformacaoinicial='".$docidformacaoinicial."' WHERE uncid='".$dados['uncid']."'");
		$db->commit();
	}
	
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sispacto.identificacaousuario i 
							   INNER JOIN sispacto.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.uncid='".$dados['uncid']."' AND t.pflcod='".PFL_COORDENADORIES."'");
	
	
	$_SESSION['sispacto']['universidade'] = array("descricao" => $arr['descricao']."( ".$infprof['iusnome']." )",
												  "curid" => $arr['curid'], 
												  "uncid" => $arr['uncid'], 
												  "reiid" => $arr['reiid'], 
												  "estuf" => $arr['uniuf'], 
												  "docid" => $docid,
												  "docidformacaoinicial" => $docidformacaoinicial,
												  "iusd" => $infprof['iusd'],
												  "iuscpf" => $infprof['iuscpf']);
	
	if($dados['direcionar']) {
		if($arr['esdid']==ESD_VALIDADO_COORDENADOR_IES) $al = array("location"=>"sispacto.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=principal");
		else $al = array("location"=>"sispacto.php?modulo=principal/universidade/universidade&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function carregarSubAtividades($dados) {
	global $db;
	$sql = "SELECT suaid as codigo, suadesc as descricao FROM sispacto.subatividades WHERE atiid='".$dados['atiid']."'";
	$db->monta_combo('suaid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'suaid', '');
	
}

function carregarUniversidadesPorUF($dados) {
	global $db;
	$sql = "SELECT u.uncid as codigo, su.uninome as descricao FROM sispacto.universidadecadastro u 
	 	    INNER JOIN sispacto.universidade su ON su.uniid = u.uniid
			WHERE su.uniuf='".$dados['estuf']."'";
	
	$combo = $db->monta_combo($dados['name'], $sql, 'S', 'Selecione', (($dados['onclick'])?$dados['onclick']:''), '', '', '', 'S', $dados['id'], true, $dados['valuecombo']);
	
	if($dados['returncombo']) return $combo;
	else echo $combo;
}

function carregarEstruturaCurso($dados) {
	global $db;
	$sql = "SELECT * FROM sispacto.estruturacurso e
			LEFT JOIN territorios.municipio m ON m.muncod = e.muncod 
			WHERE e.ecuid='".$dados['ecuid']."'";
	$estruturacurso = $db->pegaLinha($sql);
	return $estruturacurso;
}

function atualizarEstruturaCurso($dados) {
	global $db;
	$sql = "UPDATE sispacto.estruturacurso SET muncod='".$dados['muncod_endereco']."', ecuobsplanoatividades=".(($dados['ecuobsplanoatividades'])?"'".$dados['ecuobsplanoatividades']."'":"NULL")." WHERE ecuid='".$dados['ecuid']."'";
	$db->executar($sql);
	
	$suaids = array_keys($dados['aundatainicioprev']);
	
	if($suaids) {
		foreach($suaids as $suaid) {
			$aunid = $db->pegaUm("SELECT aunid FROM sispacto.atividadeuniversidade au WHERE au.suaid = '".$suaid."' AND au.ecuid = '".$dados['ecuid']."'");
			
			if($aunid) {
				$sql = "UPDATE sispacto.atividadeuniversidade SET aundatainicioprev=".(($dados['aundatainicioprev'][$suaid])?"'".formata_data_sql($dados['aundatainicioprev'][$suaid])."'":"NULL").", aundatafimprev=".(($dados['aundatafimprev'][$suaid])?"'".formata_data_sql($dados['aundatafimprev'][$suaid])."'":"NULL")." WHERE aunid='".$aunid."'";
				$db->executar($sql);
			} else {
				$sql = "INSERT INTO sispacto.atividadeuniversidade(
            			suaid, aundatainicioprev, aundatafimprev, aunstatus, ecuid)
    					VALUES ('".$suaid."', ".(($dados['aundatainicioprev'][$suaid])?"'".formata_data_sql($dados['aundatainicioprev'][$suaid])."'":"NULL").", ".(($dados['aundatafimprev'][$suaid])?"'".formata_data_sql($dados['aundatafimprev'][$suaid])."'":"NULL").", 'A', '".$dados['ecuid']."');";
				$db->executar($sql);
			}
		}
	}
	
	$ainid = $db->pegaUm("SELECT ainid FROM sispacto.articulacaoinstitucional WHERE ecuid='".$dados['ecuid']."'");
	
	if($ainid) {
		$sql = "UPDATE sispacto.articulacaoinstitucional
   				SET ainseduc=".$dados['ainseduc'].", 
   					ainseducjustificativa='".$dados['ainseducjustificativa']."', 
   					ainundime=".$dados['ainundime'].", 
				    ainundimejustificativa='".$dados['ainundimejustificativa']."', 
				    ainuncme=".$dados['ainuncme'].", 
				    ainuncmejustificativa='".$dados['ainuncmejustificativa']."'
				 WHERE ainid='".$ainid."';";
		
		$db->executar($sql);
		
	} else {
		$sql = "INSERT INTO sispacto.articulacaoinstitucional(
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
	$sql = "SELECT * FROM sispacto.articulacaoinstitucional WHERE ecuid='".$dados['ecuid']."'";
	$articulacaoinstitucional = $db->pegaLinha($sql);
	return $articulacaoinstitucional;
	
}


function carregarPlanoAtividades($dados) {
	global $db;
	
	$sql = "SELECT a.atiid, a.atidesc, s.suaid, s.suadesc, 
			".(($dados['ecuid'])?"(SELECT au.aundatainicioprev FROM sispacto.atividadeuniversidade au WHERE au.suaid = s.suaid AND au.ecuid='".$dados['ecuid']."')":"''")." as aundatainicioprev, 
			".(($dados['ecuid'])?"(SELECT au.aundatafimprev FROM sispacto.atividadeuniversidade au WHERE au.suaid = s.suaid AND au.ecuid='".$dados['ecuid']."')":"''")." as aundatafimprev 
			FROM sispacto.subatividades s 
			INNER JOIN sispacto.atividades a ON a.atiid = s.atiid 
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
	echo "<td width=\"50%\" class=\"SubTituloCentro\">ATIVIDADES / SUBATIVIDADES</td><td class=\"SubTituloCentro\" colspan=\"2\">PER�ODO DE EXECU��O</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td width=\"50%\" align=\"center\">&nbsp;</td><td align=\"center\">In�cio</td><td align=\"center\">T�rmino</td>";
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
					else echo "<td>".campo_data2('aundatainicioprev['.$su['suaid'].']','S', 'S', 'Inic�o', 'S', '', '', $su['aundatainicioprev'], '', '', 'aundatainicioprev_'.$su['suaid'])."</td>";
					if($dados['consulta']) echo "<td align=\"center\">".formata_data($su['aundatafimprev'])."</td>";
					else echo "<td>".campo_data2('aundatafimprev['.$su['suaid'].']','S', 'S', 'T�rmino', 'S', '', '', $su['aundatafimprev'], '', '', 'aundatafimprev_'.$su['suaid'])."</td>";
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
					   '<input type=\"text\" style=\"text-align:;\" name=\"saldo_'||o.orcid||'\" size=\"16\" maxlength=\"14\" value=\"'||trim(to_char(o.orcvlrunitario-coalesce(orcvlrexecutado,0),'999g999g999d99'))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"saldo_'||o.orcid||'\" title=\"Saldo\" readonly=\"readonly\" class=\" disabled\">' as saldo,
					   '<input type=\"text\" style=\"text-align:;\" name=\"orcvaloratualizado['||o.orcid||']\" size=\"16\" maxlength=\"14\" value=\"'||trim(coalesce(to_char(o.orcvlratualizado,'999g999g999d99'),''))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calcularOrcamentoExecucao();\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"valoratualizado_'||o.orcid||'\" title=\"Valor atualizado\" ".(($dados['consulta'])?"readonly=\"readonly\" class=\" disabled\"":" class=\" normal\"").">' as valoratualizado
				FROM sispacto.orcamento o 
				INNER JOIN sispacto.grupodespesa g ON g.gdeid = o.gdeid 
				WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A' 
				ORDER BY g.gdedesc
				) UNION ALL (
				
				SELECT '<b>TOTAIS</b>' as tot, 
					   '&nbsp;' as tot2, 
					   '<input type=\"text\" style=\"text-align:;\" name=\"totalvalorprevisto\" size=\"16\" maxlength=\"14\" value=\"'||trim(to_char(SUM(o.orcvlrunitario),'999g999g999d99'))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"totalvalorprevisto\" title=\"Total Valor Previsto\" readonly=\"readonly\" class=\" disabled\">' as totalvalorprevisto, 
					   '<input type=\"text\" style=\"text-align:;\" name=\"totalvalorexecutado\" size=\"16\" maxlength=\"14\" value=\"'||trim(coalesce(to_char(SUM(o.orcvlrexecutado),'999g999g999d99'),''))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"totalvalorexecutado\" title=\"Total Valor executado\" readonly=\"readonly\" class=\" disabled\">' as totalvalorexecutado,
					   '<input type=\"text\" style=\"text-align:;\" name=\"totalsaldo\" size=\"16\" maxlength=\"14\" value=\"'||trim(to_char(SUM(o.orcvlrunitario)-coalesce(SUM(o.orcvlrexecutado),0),'999g999g999d99'))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"totalsaldo\" title=\"Saldo\" readonly=\"readonly\" class=\" disabled\">' as totalsaldo,
					   '<input type=\"text\" style=\"text-align:;\" name=\"totalvaloratualizado\" size=\"16\" maxlength=\"14\" value=\"'||trim(coalesce(to_char(SUM(o.orcvlratualizado),'999g999g999d99'),''))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"totalvaloratualizado\" title=\"Total Valor atualizado\" readonly=\"readonly\" class=\" disabled\">' as valoratualizado
				FROM sispacto.orcamento o 
				INNER JOIN sispacto.grupodespesa g ON g.gdeid = o.gdeid 
				WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A' 
				
				) UNION ALL (
				
				SELECT '<b>DIFEREN�A</b>' as dif, 
					   '&nbsp;' as dif2, 
					   '&nbsp;' as dif3, 
					   '&nbsp;' as dif4,
					   '&nbsp;' as dif5,
					   '<input type=\"text\" style=\"text-align:;\" name=\"diferenca\" size=\"16\" maxlength=\"14\" value=\"'||trim(to_char((SUM(o.orcvlrunitario)-coalesce(SUM(o.orcvlrexecutado),0)) - coalesce(SUM(o.orcvlratualizado),0),'999g999g999d99'))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"diferenca\" title=\"Valor atualizado\" readonly=\"readonly\" class=\" disabled\">' as diferenca
				FROM sispacto.orcamento o 
				INNER JOIN sispacto.grupodespesa g ON g.gdeid = o.gdeid 
				WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A' 
				
				
				)";
		
		$cabecalho = array("Grupo de despesa","Unidade de medida","Valor previsto (R$)","Valor executado (R$)","Saldo (R$)","Valor atualizado (R$)");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','S');
		
		
	} elseif($dados['relatoriofinal']) { 
		
		$sql = "SELECT g.gdedesc, o.orcvlrunitario, ".(($dados['consulta'])?"coalesce(o.orcdescricao,'')":"'<textarea disabled cols=\"15\" rows=\"3\" onmouseover=\"MouseOver( this );\" onfocus=\"MouseClick( this );\" onmouseout=\"MouseOut( this );\" onblur=\"MouseBlur( this );\" style=\"width:50ex;\" class=\"txareanormal\">'||coalesce(o.orcdescricao,'')||'</textarea>'")." as x, '<input type=\"text\" style=\"text-align:;\" name=\"orcvlrfinal['||o.orcid||']\" size=\"16\" maxlength=\"14\" value=\"'||coalesce(trim(to_char(o.orcvlrfinal,'999g999g999d99')),'')||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"orcvlrfinal_'||o.orcid||'\" title=\"Valor final\" class=\" normal\">' as valorfinal, ".(($dados['consulta'])?"coalesce(o.orcdescricaofinal,'')":"'<textarea id=\"orcdescricao_'||o.orcid||'\" name=\"orcdescricao['||o.orcid||']\" cols=\"15\" rows=\"3\" onmouseover=\"MouseOver( this );\" onfocus=\"MouseClick( this );\" onmouseout=\"MouseOut( this );\" onblur=\"MouseBlur( this );\" style=\"width:50ex;\" class=\"txareanormal\">'||coalesce(o.orcdescricaofinal,'')||'</textarea>'")." as detalhamentofinal
				FROM sispacto.orcamento o
				INNER JOIN sispacto.grupodespesa g ON g.gdeid = o.gdeid
				WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A'
				ORDER BY g.gdedesc";
		
		$cabecalho = array("Grupo de Despesa","Valor total (R$)","Detalhamento","Valor final (R$)","Detalhamento final");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'S','100%','S');
		
	} else {
	
		$sql = "SELECT ".(($dados['consulta'])?"''":"'<center><img src=../imagens/alterar.gif style=\"cursor:pointer;\" onclick=\"inserirCustos(\''||o.orcid||'\');\"> <img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirCustos(\''||o.orcid||'\');\"></center>'")." as acao, g.gdedesc, 'Verba', o.orcvlrunitario, o.orcdescricao 
				FROM sispacto.orcamento o 
				INNER JOIN sispacto.grupodespesa g ON g.gdeid = o.gdeid 
				WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A' 
				ORDER BY g.gdedesc";
		$cabecalho = array("&nbsp;","Grupo de Despesa","Unidade de Medida","Valor total (R$)","Detalhamento");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'S','100%','S');
	
	}
	
	
}

function carregarNaturezaDespesasCustos($dados) {
	global $db;
	$sql = "SELECT n.ndecodigo, n.ndedesc, SUM(o.orcvlrtotal) as total 
			FROM sispacto.orcamento o 
			INNER JOIN sispacto.itemdespesa i ON i.ideid = o.ideid 
			INNER JOIN sispacto.grupodespesa g ON g.gdeid = i.gdeid 
			INNER JOIN sispacto.naturezadespesa n ON n.ndeid = g.ndeid  
			WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A' 
			GROUP BY n.ndecodigo, n.ndedesc";
	
	$cabecalho = array("C�digo","Descri��o","Valor(R$)");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}

function carregarOrcamento($dados) {
	global $db;
	$sql = "SELECT * FROM sispacto.orcamento o 
			INNER JOIN sispacto.grupodespesa g ON o.gdeid = g.gdeid 
			WHERE orcid='".$dados['orcid']."'";
	
	$orcamento = $db->pegaLinha($sql);
	
	return $orcamento;
	
}

function atualizarOrcamentoExecucao($dados) {
	global $db;
	
	if($dados['orcvalorexecutado']) {
		foreach(array_keys($dados['orcvalorexecutado']) as $orcid) {
			$sql = "UPDATE sispacto.orcamento SET orcvlrexecutado=".(($dados['orcvalorexecutado'][$orcid])?"'".str_replace(array(".",","),array("","."),$dados['orcvalorexecutado'][$orcid])."'":"NULL").",
												  orcvlratualizado=".(($dados['orcvaloratualizado'][$orcid])?"'".str_replace(array(".",","),array("","."),$dados['orcvaloratualizado'][$orcid])."'":"NULL")."
					WHERE orcid='".$orcid."'";

			$db->executar($sql);
			$db->commit();
		}
	}
	
	$al = array("alert"=>"Or�amento atualizado com sucesso.","location"=>"sispacto.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=orcamentoexecucao");
	alertlocation($al);
	
}

function atualizarCusto($dados) {
	global $db;
	$sql = "UPDATE sispacto.orcamento SET gdeid='".$dados['gdeid']."', orcvlrunitario='".str_replace(array(".",","),array("","."),$dados['orcvlrunitario'])."', orcdescricao='".$dados['orcdescricao']."'
			WHERE orcid='".$dados['orcid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Custo inserido com sucesso","javascript"=>"window.opener.carregarListaCustos();window.close();");
	alertlocation($al);
	
}

function excluirCustos($dados) {
	global $db;
	$sql = "DELETE FROM sispacto.orcamento WHERE orcid='".$dados['orcid']."'";
	$db->executar($sql);
	$db->commit();
	
	
}

function inserirCusto($dados) {
	global $db;
	$sql = "INSERT INTO sispacto.orcamento(
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
		
		$sql = "SELECT i.iuscpf, i.iusnome, i.iusemailprincipal, es.esddsc  FROM sispacto.identificacaousuario i 
				INNER JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
				LEFT JOIN workflow.documento d ON d.docid = p.docid 
				LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
				INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN sispacto.abrangencia a ON p.muncod=a.muncod
				WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND a.ecuid='".$dados['ecuid']."' AND a.muncod='".$dados['muncod']."' AND a.esfera='M'";
		
		$cabecalho = array("CPF","Nome","Email","Situa��o cadastro");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	
	}
	
	if($dados['esfera']=='E') {
		
		$sql = "SELECT i.iuscpf, i.iusnome, i.iusemailprincipal, es.esddsc FROM sispacto.identificacaousuario i 
				INNER JOIN territorios.municipio mm ON mm.muncod = i.muncodatuacao
				INNER JOIN sispacto.pactoidadecerta p ON p.estuf = mm.estuf AND p.picid = i.picid 
				LEFT JOIN workflow.documento d ON d.docid = p.docid 
				LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
				INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN sispacto.abrangencia a ON mm.muncod=a.muncod
				WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND a.ecuid='".$dados['ecuid']."' AND a.muncod='".$dados['muncod']."' AND a.esfera='E'";
		
		$cabecalho = array("CPF","Nome","Email","Situa��o cadastro");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	}
	
}

function carregarEquipeRecursosHumanos($dados) {
	global $db;
	$sql = "(SELECT '&nbsp' as acao, '<center>'||iuscpf||'</center>' as iuscpf, iusnome, iusemailprincipal, p.pfldsc, to_char(t.tpeatuacaoinicio,'mm/YYYY')||' a '||to_char(t.tpeatuacaofim,'mm/YYYY') as periodo  
			FROM sispacto.identificacaousuario i 
			LEFT JOIN sispacto.portarianomeacao po ON po.iusd = i.iusd 
			LEFT JOIN public.arquivo ar ON ar.arqid = po.arqid
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sispacto.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			WHERE t.pflcod IN('".PFL_COORDENADORIES."') AND i.uncid='".$dados['uncid']."')
			UNION ALL (
			SELECT ".((!$dados['consulta'])?"'<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"inserirEquipe(\''||i.iusd||'\');\" > <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirEquipeRecursosHumanos(\''||i.iusd||'\');\">' || CASE WHEN t.tpejustificativaformadories IS NULL THEN '' ELSE ' <img src=\"../imagens/valida2.gif\" border=\"0\" style=\"cursor:pointer;\" onclick=\"jAlert(\''||t.tpejustificativaformadories||'\', \'Justificativa\');\">' END||'</center>'":"'&nbsp;'")." as acao, '<center>'||iuscpf||'</center>', iusnome, iusemailprincipal, p.pfldsc, to_char(t.tpeatuacaoinicio,'mm/YYYY')||' a '||to_char(t.tpeatuacaofim,'mm/YYYY') as periodo 
			FROM sispacto.identificacaousuario i
			LEFT JOIN sispacto.portarianomeacao po ON po.iusd = i.iusd 
			LEFT JOIN public.arquivo ar ON ar.arqid = po.arqid
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sispacto.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			WHERE t.pflcod IN('".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."') AND i.uncid='".$dados['uncid']."' ORDER BY p.pflcod, i.iusnome)";
	
	$equiperh = $db->carregar($sql);
	
	
	$cabecalho = array("&nbsp;","CPF","Nome","E-mail","Perfil","Per�odo de atua��o");
	$db->monta_lista_simples($equiperh,$cabecalho,1000,5,'N','100%','N');

}

function numeroMaximoCoordenadorAjuntoIES($dados) {
	global $db;
	$sql = "SELECT m.estuf FROM sispacto.estruturacurso e 
	 		INNER JOIN sispacto.abrangencia a ON a.ecuid = e.ecuid 
	 		INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
	 		WHERE e.uncid='".$dados['uncid']."' 
	 		GROUP BY m.estuf";
	
	$maxCoordenadorAjunto = $db->carregarColuna($sql);
	return count($maxCoordenadorAjunto);
	
}

function numeroCoordenadorAdjuntoIES($dados) {
	global $db;
	$sql = "SELECT COUNT(*) FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			WHERE i.uncid='".$dados['uncid']."' AND t.pflcod='".PFL_COORDENADORADJUNTOIES."'";
	
	$numCoordenadorAjunto = $db->pegaUm($sql);
	
	return $numCoordenadorAjunto;
	
}


function validarNumeroCoordenadorAdjuntoIES($dados) {
	global $db;
	
	$maxCoordenadorAjunto = numeroMaximoCoordenadorAjuntoIES($dados);
	
	$numCoordenadorAjunto = numeroCoordenadorAdjuntoIES($dados);
	
	if($maxCoordenadorAjunto>$numCoordenadorAjunto) return true;
	else return false;
	
}

function numeroFormadorIES($dados) {
	global $db;
	$sql = "SELECT COUNT(*) FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
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
	$sql = "SELECT tpejustificativaformadories FROM sispacto.tipoperfil WHERE iusd='".$dados['iusd']."'";
	$tpejustificativaformadories = $db->pegaUm($sql);
	return $tpejustificativaformadories;
}


function inserirEquipeRecursosHumanos($dados) {
	global $db;
	
	if($dados['iusd']) {
		$sql = "DELETE FROM sispacto.tipoperfil WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
		
		if($dados['pflcod']!=PFL_FORMADORIES) {
			$sql = "SELECT turid FROM sispacto.turmas WHERE iusd='".$dados['iusd']."'";
			$existe = $db->pegaUm($sql);
			if($existe) {
				$al = array("alert"=>"N�o � poss�vel trocar o perfil. Este usu�rio possui turma associada. Remova a turma para altera o perfil","location"=>"sispacto.php?modulo=principal/universidade/inserirequipe&acao=A&iusd=".$dados['iusd']);
				alertlocation($al);
			}
		}
		
	}
	
	if($dados['pflcod']==PFL_COORDENADORADJUNTOIES) {
		if(!validarNumeroCoordenadorAdjuntoIES(array("uncid"=>$dados['uncid']))) {
			$al = array("alert"=>"N�mero de Coordenador Adjunto da IES est� no m�ximo. N�o � poss�vel inserir o CPF","location"=>"sispacto.php?modulo=principal/universidade/inserirequipe&acao=A");
			alertlocation($al);
		}
	}
	
	$iusd = $db->pegaUm("SELECT iusd FROM sispacto.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
	
	if(!$iusd) {
		
		$sql = "INSERT INTO sispacto.identificacaousuario(
	            iuscpf, iusnome, iusemailprincipal, foeid, uncid, iusdatainclusao)
	    		VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', 
	    				'".$dados['iusnome']."', '".$dados['iusemailprincipal']."', 
	    				'".$dados['foeid']."', '".$dados['uncid']."', NOW()) RETURNING iusd;";
		
		
		$iusd = $db->pegaUm($sql);
		
	} else {
		
		$sql = "SELECT 'Nome : '||i.iusnome||', Perfil : '||p.pfldsc||', CPF n�o pode ser cadastrado. � necess�rio remove-lo do perfil indicado.' as msg FROM sispacto.identificacaousuario i 
				INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
				WHERE i.iusd='".$iusd."'";
		
		$msg = $db->pegaUm($sql);
		
		if($msg) {
			$al = array("alert"=>$msg,"location"=>"sispacto.php?modulo=principal/universidade/inserirequipe&acao=A");
			alertlocation($al);
		}
		
		$sql = "UPDATE sispacto.identificacaousuario SET iusemailprincipal='".$dados['iusemailprincipal']."', 
														 foeid='".$dados['foeid']."', 
														 uncid='".$dados['uncid']."',
														 iusstatus='A'
				WHERE iusd='".$iusd."'";
		
		$db->executar($sql);
		
	}
	
	// controlando telefones
	$db->executar("DELETE FROM sispacto.identificacaotelefone WHERE iusd='".$iusd."' AND itetipo='T'");
	
	$sql = "INSERT INTO sispacto.identificacaotelefone(
           	iusd, itedddtel, itenumtel, itetipo, itestatus)
   			VALUES ('".$iusd."','".$dados['itedddtel']['T']."', '".$dados['itenumtel']['T']."', 'T', 'A');";
		
	$db->executar($sql);

	$sql = "INSERT INTO sispacto.tipoperfil(
            iusd, pflcod, tpestatus, tpeatuacaoinicio, tpeatuacaofim, tpejustificativaformadories)
    		VALUES ('".$iusd."', '".$dados['pflcod']."', 'A', '".$dados['tpeatuacaoinicio_ano']."-".$dados['tpeatuacaoinicio_mes']."-01', '".$dados['tpeatuacaofim_ano']."-".$dados['tpeatuacaofim_mes']."-01',".(($dados['tpejustificativaformadories'])?"'".$dados['tpejustificativaformadories']."'":"NULL").");";
	
	$db->executar($sql);
	
	$db->commit();
	
	$al = array("alert"=>"Equipe gravada com sucesso","javascript"=>"window.opener.carregarEquipeRecursosHumanos();window.close();");
	alertlocation($al);
	
}

function excluirEquipeRecursosHumanos($dados) {
	global $db;
	
	$sql = "DELETE FROM sispacto.tipoperfil WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM sispacto.identificacaotelefone WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);

	$db->commit();
	
	$sql = "SELECT turid FROM sispacto.turmas WHERE iusd='".$dados['iusd']."'";
	$turids = $db->carregarColuna($sql);
	
	if($turids) {
		foreach($turids as $turid) {
			excluirTurma(array("turid" => $turid));			
		}
	}
	
	$al = array("alert"=>"Equipe removida com sucesso","location"=>"sispacto.php?modulo=principal/universidade/universidade&acao=A&aba=recursos_humanos");
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
	$total_turmas = $db->pegaUm("SELECT COUNT(*) FROM sispacto.turmas WHERE uncid='".$dados['uncid']."' AND turstatus='A'");
	return $total_turmas;
}


function atualizarTurma($dados) {
	global $db;
	
	$sql = "UPDATE sispacto.turmas SET iusd='".$dados['iusd']."', turdesc='".$dados['turdesc']."', muncod='".$dados['muncod_endereco']."' WHERE turid='".$dados['turid']."'";
	
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
	
	
	if(validarCadastroTurma($dados)) {
	
		$sql = "INSERT INTO sispacto.turmas(
		        uncid, iusd, turdesc, turstatus, muncod)
		    	VALUES ('".$dados['uncid']."', '".$dados['iusd']."', '".$dados['turdesc']."', 'A', '".$dados['muncod_endereco']."');";
		
		$db->executar($sql);
		$db->commit();
		
		echo "Turma inserida com sucesso";
	
	} else {
		
		echo "N�o foi poss�vel cadastrar a turma. N�mero de turmas est� no limite.";
		
	}
}

function carregarTurmas($dados) {
	global $db;
	$sql = "SELECT 
				'<img src=\"../imagens/mais.gif\" title=\"mais\" id=\"btn_turma_'||t.turid||'\" style=\"cursor:pointer;\" onclick=\"abrirTurma('||t.turid||',this)\"> ".((!$dados['consulta'])?"<img src=../imagens/excluir.gif style=\"cursor:pointer\" onclick=\"excluirTurma('||t.turid||');\"> <img src=../imagens/alterar.gif style=\"cursor:pointer;\"  onclick=\"abrirCadastroTurma(\''||turid||'\')\"> <img src=../imagens/salvar.png style=\"cursor:pointer;\" onclick=\"comporTurma(\''||turid||'\')\">":"")."' as acao,
				m.estuf|| ' / ' || m.mundescricao as polo, 
				t.turdesc,
				'<center>'||i.iuscpf||'</center>' as iuscpf,
				i.iusnome,
				i.iusemailprincipal,
				(SELECT '(' || itedddtel || ') '|| itenumtel FROM sispacto.identificacaotelefone WHERE iusd=t.iusd AND itetipo='T') as telefone,
				(SELECT COUNT(*) FROM sispacto.orientadorturma ot INNER JOIN sispacto.tipoperfil tt ON tt.iusd = ot.iusd and tt.pflcod=".PFL_ORIENTADORESTUDO." WHERE turid=t.turid) as nalunos
			FROM sispacto.turmas t 
			INNER JOIN sispacto.identificacaousuario i ON i.iusd = t.iusd 
			LEFT JOIN territorios.municipio m ON m.muncod = t.muncod
			WHERE t.uncid='".$dados['uncid']."'";
	
	if($dados['formacaoinicial']) {
		echo "<p><b>Marcar Todos : </b><input type=\"radio\" name=\"marcartodos\" onclick=\"marcarTodos(this);\" value=\"TRUE\"> Presente <input type=\"radio\" name=\"marcartodos\" onclick=\"marcarTodos(this);\" value=\"FALSE\"> Ausente</p>";
	}

	
	$cabecalho = array("&nbsp;","Polo","Turma","CPF","Nome","Email","Telefone","N�mero de orientadores");
	
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
}

function validarFormadoresTurmas($dados) {
	global $db;
	$sql = "SELECT 'Formador ('||i.iusnome||') n�o foi vinculado a nenhuma turma.' as t FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_FORMADORIES."
			LEFT JOIN sispacto.turmas tt ON tt.iusd = i.iusd 
			WHERE i.uncid='".$dados['uncid']."' AND tt.turid IS NULL";
	$mensagens = $db->carregarColuna($sql);
	
	return $mensagens;
	
}


function inserirAlunoTurma($dados) {
	global $db;
	$sql = "INSERT INTO sispacto.orientadorturma(
            turid, iusd, otustatus)
    		VALUES ('".$dados['turid']."', '".$dados['iusd']."', 'A');";
	$db->executar($sql);
	
	if($dados['uncid']) {
		$sql = "UPDATE sispacto.identificacaousuario SET uncid='".$dados['uncid']."' WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	} else {
		$sql = "UPDATE sispacto.identificacaousuario SET uncid=NULL WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
	}
	
	$db->commit();
}

function excluirAlunoTurma($dados) {
	global $db;
	if($dados['iusd']) $wh = "iusd='".$dados['iusd']."'";
	elseif($dados['turid']) $wh = "turid='".$dados['turid']."'";
	
	$sql = "DELETE FROM sispacto.orientadorturma WHERE ".$wh;
	$db->executar($sql);
	$db->commit();
}

function excluirTurma($dados) {
	global $db;
	
	excluirAlunoTurma($dados);
	
	$sql = "DELETE FROM sispacto.turmas WHERE turid='".$dados['turid']."'";
	$db->executar($sql);
	$db->commit();
	
}

function carregarFiltrosMunicipios($dados) {
	global $db;
	if($dados['estuf']) {
		$sql = "SELECT m.muncod as codigo, REPLACE(m.mundescricao,'\'',' ') as descricao 
				FROM territorios.municipio m 
				".(($dados['esfera']=="M")?"INNER JOIN sispacto.pactoidadecerta p ON p.muncod = m.muncod":"")." 
				WHERE m.estuf='".$dados['estuf']."' AND m.muncod NOT IN(SELECT muncod FROM sispacto.abrangencia WHERE esfera='".$dados['esfera']."') ORDER BY mundescricao";
	} else {
		$sql = "SELECT muncod as codigo, muncod as descricao FROM sispacto.abrangencia WHERE 1=2";
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
		$al = array("alert"=>"Por favor preencha todos os campos obrigat�rios da tela �Dados Coordenador IES�.","location"=>"sispacto.php?modulo=principal/universidade/universidade&acao=A&aba=dados");
		alertlocation($al);
	}
}

function numeroAlunosTurma($dados) {
	global $db;
	$sql = "SELECT COUNT(*) FROM sispacto.orientadorturma WHERE turid='".$dados['turid']."'";
	$numturmas = $db->pegaUm($sql);
	
	echo $numturmas;
	
}

function validarEnvioAnaliseMEC() {
	global $db;
	
	$mensagens = validarFormadoresTurmas(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	
	if($mensagens) $erro .= implode('<br>\n',$mensagens).'<br><br>\n\n H� '.count($mensagens).' Orientadores de Estudo que n�o foram vinculados a nenhuma turma. Retorne para a tela Turmas e vincule todos os nomes.<br><br>\n\n';
	
	$pp = carregarNumeroOrientadoresPendencia(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	if($pp['nummunpendencias']>0) $erro .= 'H� '.$pp['nummunpendencias'].' munic�pios que n�o conclu�ram o cadastramento dos seus Orientadores de Estudo<br>\n';
	if($pp['numestpendencias']>0) $erro .= 'H� '.$pp['numestpendencias'].' estados que n�o conclu�ram o cadastramento dos seus Orientadores de Estudo<br>\n';
	
	$numeroMaximoTurmas = numeroMaximoTurmas(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	$total_turmas = numeroTurmas(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	if($total_turmas>$numeroMaximoTurmas) {
		$erro .= '<br>\nN�mero m�ximo de turmas (na tela Turmas) deve ser igual ao N�mero m�ximo de Formadores (na tela Equipe IES) + Formadores Justificados<br><br>\n\n';
	}
	
	$tso = validarTurmasSemOrientadores(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	
	if($tso) {
		$erro .= '<br>\nExistem '.count($tso).' turma(s) sem orientadores cadastrados.';
	}
	
	if($erro) return $erro;
	else return true;

}

function validarTurmasSemOrientadores($dados) {
	global $db;
	
	$sql = "SELECT foo.turdesc FROM (
			SELECT t.turdesc, (SELECT COUNT(*) FROM sispacto.orientadorturma WHERE turid=t.turid) as nalunos FROM sispacto.turmas t 
			WHERE t.uncid='".$dados['uncid']."') foo WHERE foo.nalunos=0";
	
	$turmassemorientadores = $db->carregar($sql);
	
	return $turmassemorientadores;
	
}

function validarOrientadoresCadastradosTurma($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*)			
			FROM sispacto.turmas t 
			INNER JOIN sispacto.orientadorturma ot ON ot.turid = t.turid
			INNER JOIN sispacto.tipoperfil tt ON tt.iusd = ot.iusd and tt.pflcod=".PFL_ORIENTADORESTUDO." 
			INNER JOIN sispacto.identificacaousuario i ON i.iusd = tt.iusd 
			WHERE t.uncid='".$dados['uncid']."' AND i.iusstatus='A'";
	
	$numOrientadoresTurmas = $db->pegaUm($sql);
	
	$sql = "SELECT SUM(foo.t) FROM (
			(SELECT COUNT(*) as t  FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
			LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sispacto.abrangencia a ON p.muncod=a.muncod 
			INNER JOIN sispacto.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid='".$dados['uncid']."' AND i.uncid='".$dados['uncid']."' AND a.esfera='M' AND i.iusstatus='A') 
			UNION ALL (
			SELECT COUNT(*) as t FROM sispacto.identificacaousuario i 
			INNER JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
			INNER JOIN sispacto.pactoidadecerta p ON p.estuf = m.estuf AND p.picid = i.picid 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sispacto.abrangencia a ON m.muncod=a.muncod 
			INNER JOIN sispacto.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid='".$dados['uncid']."' AND i.uncid='".$dados['uncid']."' AND a.esfera='E' AND i.iusstatus='A')
			) foo";
	
	$numTotalOrientadores = $db->pegaUm($sql);
	
	if($numOrientadoresTurmas==$numTotalOrientadores) return true;
	else return ($numTotalOrientadores-$numOrientadoresTurmas);
	
}



function calculaPorcentagemTramitacaoUniversidade($dados) {
	global $db;
	
	
	$sql = "SELECT d.docid, d.esdid, to_char(h.htddata,'YYYY-mm-dd') as htddata 
			FROM sispacto.universidadecadastro u 
			INNER JOIN workflow.documento d ON d.docid = u.docid 
			LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
			WHERE uncid='".$_SESSION['sispacto']['universidade']['uncid']."'";
	
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
	
	$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	
	gerenciarAtividadeUniversidade(array('aundatafim'=>$aundatafim,'aundatainicio'=>$aundatainicio,'aunsituacao'=>$aunsituacao,'ecuid'=>$ecuid,'suaid'=>$dados['suaid']));
	
}

function calculaPorcentagemFormacaoInicial($dados) {
	global $db;
	
	
	$sql = "SELECT d.docid, d.esdid, to_char(h.htddata,'YYYY-mm-dd') as htddata 
			FROM sispacto.universidadecadastro u 
			INNER JOIN workflow.documento d ON d.docid = u.docidformacaoinicial 
			LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
			WHERE uncid='".$_SESSION['sispacto']['universidade']['uncid']."'";
	
	$documento = $db->pegaLinha($sql);
	
	$sql = "SELECT to_char(htddata,'YYYY-mm-dd') as htddata FROM workflow.historicodocumento WHERE docid='".$documento['docid']."' ORDER BY htddata ASC LIMIT 1";
	$aundatainicio = $db->pegaUm($sql);
	
	if($documento['esdid']==ESD_FECHADO_FORMACAOINICIAL) {
		$aunsituacao = '100';
		$aundatafim = $documento['htddata'];;
	}
	
	$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	
	gerenciarAtividadeUniversidade(array('aundatafim'=>$aundatafim,'aundatainicio'=>$aundatainicio,'aunsituacao'=>$aunsituacao,'ecuid'=>$ecuid,'suaid'=>$dados['suaid']));
	
}

function calculaPorcentagemDefinicaoEquipeIES($dados) {
	global $db;
	
	$sql = "SELECT to_char(MIN(iusdatainclusao),'YYYY-mm-dd') as inicio, to_char(MAX(iusdatainclusao),'YYYY-mm-dd') as fim, COUNT(*) as numequipe 
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod IN('".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."','".PFL_COORDENADORIES."') AND i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' AND i.iusstatus='A'";
	
	$info = $db->pegaLinha($sql);
	$aundatainicio = $info['inicio'];
	$aundatafim = $info['fim'];
	
	// CoordenadorIES + Coordenador Adjunto+ Supervisor IES + Formador IES
	$maximoFormadorIES = numeroMaximoTurmas(array("uncid"=>$_SESSION['sispacto']['universidade']['uncid']));
	$totalMaxEquipe = 1+numeroMaximoCoordenadorAjuntoIES(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']))+1+$maximoFormadorIES;
	
	$aunsituacao = round(($info['numequipe']/$totalMaxEquipe)*100);
	
	$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	
	gerenciarAtividadeUniversidade(array('aundatafim'=>$aundatafim,'aundatainicio'=>$aundatainicio,'aunsituacao'=>$aunsituacao,'ecuid'=>$ecuid,'suaid'=>$dados['suaid']));
}

function calculaPorcentagemFormacaoTurmas($dados) {
	global $db;
	
	$total   = totalAlfabetizadoresAbrangencia(array("uncid"=>$_SESSION['sispacto']['universidade']['uncid']));
	$nturmas = totalTurmasAbrangencia(array("total"=>$total));
	$formadoressolicitados = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sispacto']['universidade']['uncid'],"pflcod"=>PFL_FORMADORIES,"tpejustificativaformadories"=>true));
	
	$sql = "SELECT MAX(otudata) as fim, MIN(otudata) as inicio, COUNT(DISTINCT t.turid) as nturmas FROM sispacto.turmas t INNER JOIN sispacto.orientadorturma ot ON ot.turid = t.turid WHERE t.uncid='".$_SESSION['sispacto']['universidade']['uncid']."'";
	$dturmas = $db->pegaLinha($sql);
	
	$aundatainicio = $dturmas['inicio'];
	$aundatafim = $dturmas['fim'];
	$numeroturmas = $dturmas['nturmas'];
	$maximoTurmas = $nturmas+count($formadoressolicitados);
	$aunsituacao = (($maximoTurmas)?round(($numeroturmas/$maximoTurmas)*100):0);
	
	
	
	$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sispacto']['universidade']['uncid']));
	
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
					CASE WHEN pic.picid IS NOT NULL THEN 
														CASE WHEN pic.muncod IS NOT NULL THEN m1.estuf||' / '||m1.mundescricao||' ( Municipal )' 
															 WHEN pic.estuf IS NOT NULL THEN m2.estuf||' / '||m2.mundescricao||' ( Estadual )' 
														END 
					ELSE 'Equipe IES' END as rede
					
			FROM sispacto.identificacaousuario i
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sispacto.pactoidadecerta pic ON pic.picid = i.picid 
			LEFT JOIN territorios.municipio m1 ON m1.muncod = pic.muncod 
			LEFT JOIN territorios.municipio m2 ON m2.muncod = i.muncodatuacao 
			WHERE (t.pflcod IN('".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."','".PFL_COORDENADORLOCAL."','".PFL_PROFESSORALFABETIZADOR."') OR (t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.iusformacaoinicialorientador=true)) AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome";
	
	return $sql;
}


function mostrarAbaFormacaoInicial($dados) {
	global $db;
	$estado = wf_pegarEstadoAtual( $_SESSION['sispacto']['universidade']['docid'] );
	if($estado['esdid'] == ESD_VALIDADO_COORDENADOR_IES) {
		return true;
	} else {
		return false;
	}
	
}

function salvarFormacaoInicial($dados) {
	global $db;
	if($dados['iusd']) {
		foreach($dados['iusd'] as $iusd => $fi) {
			$sql = "UPDATE sispacto.identificacaousuario SET iusformacaoinicialorientador=".$fi." WHERE iusd='".$iusd."'";
			$db->executar($sql);
		}
		$db->commit();
	}
	
	$al = array("alert"=>"Forma��o Inicial dos Orientadores foram salvas com sucesso. N�o esque�a de preencher os alunos Presentes na Forma��o Inicial que n�o constam na lista, ao final do processo CLICAR no bot�o de Concluir os Registros de Frequ�ncia","location"=>"sispacto.php?modulo=principal/universidade/universidade&acao=A&aba=formacaoinicial");
	alertlocation($al);

}

function condicaoFormacaoInicial() {
	global $db;
	
	$sql = "SELECT COUNT(i.iusd) as total FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			INNER join sispacto.pactoidadecerta p on i.picid = p.picid 
			INNER JOIN sispacto.abrangencia a ON p.muncod=a.muncod 
			INNER JOIN sispacto.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE SUBSTR(i.iuscpf,1,3)!='SIS' AND i.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' AND c.uncid='".$_SESSION['sispacto']['universidade']['uncid']."' AND t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.iusstatus='A' AND iusformacaoinicialorientador IS NULL";
	
	$total_n_gravados = $db->pegaUm($sql);
	
	if($total_n_gravados) return "� necess�rio gravar os registros selecionando os orientadores que obtiveram a Forma��o Inicial";
	else return true;
	
}

function verificarValidacaoEstruturaFormacao($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*) as total FROM sispacto.abrangencia a 
			INNER JOIN sispacto.estruturacurso e ON e.ecuid = a.ecuid 
			WHERE e.uncid='".$dados['uncid']."'";
	
	$tot_abrangencia = $db->pegaUm($sql);
	
	$sql = "SELECT muncod FROM sispacto.estruturacurso e WHERE e.uncid='".$dados['uncid']."'";
	$existe_sede = $db->pegaUm($sql);
	
	if($tot_abrangencia == 0 || !$existe_sede) {
		$al = array("alert"=>"� necess�rio cadastrar as informa��es na aba Estrutura da Forma��o","location"=>"sispacto.php?modulo=principal/universidade/universidade&acao=A&aba=estrutura_curso");
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
	
	if($aed_devolver) $aedid=AED_REPROVAR_CADASTRO_ORIENTADORES;
	else $aedid=AED_APROVAR_CADASTRO_ORIENTADORES;
	
	$sql = "SELECT DISTINCT pp.docid
			FROM sispacto.abrangencia a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sispacto.estruturacurso e ON e.ecuid = a.ecuid 
			LEFT JOIN sispacto.pactoidadecerta pp ON pp.muncod = m.muncod 
			LEFT JOIN workflow.documento dd ON dd.docid = pp.docid 
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = dd.esdid 
			WHERE e.uncid='".$uncid."' AND a.esfera='M'";
	
	$docids = $db->carregarColuna($sql);
	
	if($docids) {
		foreach($docids as $docid) {
			wf_alterarEstado( $docid, $aedid, $cmddsc = '', array());
		}
	}
	
	$sql = "SELECT pp.docid
			FROM sispacto.abrangencia a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sispacto.estruturacurso e ON e.ecuid = a.ecuid
			LEFT JOIN sispacto.pactoidadecerta pp ON pp.estuf = m.estuf 
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
		
		$sql = "UPDATE sispacto.identificacaousuario id SET uncid=foo.uncid FROM (
				SELECT 
				u.uncid,
				i.iusd
				FROM sispacto.universidadecadastro u 
				INNER JOIN sispacto.estruturacurso e ON e.uncid = u.uncid 
				INNER JOIN sispacto.abrangencia a ON a.ecuid = e.ecuid 
				INNER JOIN sispacto.pactoidadecerta p ON p.muncod = a.muncod 
				INNER JOIN sispacto.identificacaousuario i ON i.picid = p.picid 
				INNER JOIN sispacto.tipoperfil t ON i.iusd = t.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
				WHERE u.uncid='".$uncid."' AND esfera='M' AND i.uncid IS NULL ) foo WHERE foo.iusd = id.iusd";
		
		$db->executar($sql);
		
		$sql = "UPDATE sispacto.identificacaousuario id SET uncid=foo.uncid FROM (
				SELECT 
				distinct 
				u.uncid,
				i.iusd
				FROM sispacto.universidadecadastro u 
				INNER JOIN sispacto.estruturacurso e ON e.uncid = u.uncid 
				INNER JOIN sispacto.abrangencia a ON a.ecuid = e.ecuid 
				INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
				INNER JOIN sispacto.pactoidadecerta p ON p.estuf = m.estuf 
				INNER JOIN sispacto.identificacaousuario i ON i.picid = p.picid 
				INNER JOIN sispacto.tipoperfil t ON i.iusd = t.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
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
			FROM sispacto.versoesprojetouniversidade v 
			INNER JOIN seguranca.usuario u ON u.usucpf = v.usucpf
			WHERE uncid='".$dados['uncid']."' ORDER BY v.vpndata DESC";
	$cabecalho = array("&nbsp","Usu�rio que inseriu vers�o","Data da inser��o");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
}

function visualizarVersao($dados) {
	global $db;
	$sql = "SELECT vpnhtml FROM sispacto.versoesprojetouniversidade WHERE vpnid='".$dados['vpnid']."'";
	$html = $db->pegaUm($sql);
	
	echo $html;
	
}

function carregarOuvintesFormacaoInicial($dados) {
	global $db;
	
	$sql = "SELECT '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"inserirOuvinte(\''||f.fioid||'\');\" > <img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirOuvinte(\''||fioid||'\');\">' as acao, fiocpf, fionome, fioemail, m.estuf||' / '||m.mundescricao as municipio,
				   CASE WHEN f.fioesfera='M' THEN 'Municipal'
				   		WHEN f.fioesfera='E' THEN 'Estadual' END as esfera,
				   	i.iusnome as substituido 
			FROM sispacto.formacaoinicialouvintes f 
			LEFT JOIN territorios.municipio m ON m.muncod = f.muncod 
			LEFT JOIN sispacto.identificacaousuario i ON i.iusd = f.iusd
			WHERE f.uncid='".$dados['uncid']."' AND f.fiostatus='A'";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email","UF / Munic�pio","Esfera","Ir� substituir");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
}

function carregarUsuariosHabiliatadosTroca($dados) {
	global $db;
	$wh[] = "i.uncid='".$dados['uncid']."'";
	$wh[] = "(i.iusformacaoinicialorientador=false OR i.iusformacaoinicialorientador IS NULL)";
	if($dados['esfera']=='M') $wh[] = "i.picid IN(SELECT picid FROM sispacto.pactoidadecerta WHERE muncod='".$dados['muncod']."')";
	elseif($dados['esfera']=='E') $wh[] = "i.picid IN(SELECT picid FROM sispacto.pactoidadecerta WHERE estuf='".$dados['estuf']."')";
	
	$sql = "SELECT i.iusd as codigo, i.iusnome as descricao FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd AND t.pflcod='".PFL_ORIENTADORESTUDO."' 
			INNER JOIN sispacto.orientadorturma o ON o.iusd = i.iusd 
			WHERE ".(($wh)?implode(" AND ",$wh):"")." ORDER BY i.iusnome";
	
	$db->monta_combo('iusd', $sql, 'S', 'N�O � SUBSTITUTO, SOMENTE OUVINTE', '', '', '', '', 'S', 'iusd', '', $dados['iusd']);
}


function atualizarOuvinte($dados) {
	global $db;
	$sql = "UPDATE sispacto.formacaoinicialouvintes SET  fiocpf='".str_replace(array(".","-"),array("",""),$dados['fiocpf'])."', 
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

	$sql = "INSERT INTO sispacto.formacaoinicialouvintes(
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
	$sql = "UPDATE sispacto.formacaoinicialouvintes SET fiostatus='I' WHERE fioid='".$dados['fioid']."'";
	
	$db->executar($sql);
	$db->commit();
	
}

function posDevolverCoordenadorIES() {
	global $db;
	
	if(!strstr($_SERVER['HTTP_HOST'],"simec-local")) {
	
		$sql = "SELECT c.cmddsc FROM workflow.documento d 
				INNER JOIN workflow.comentariodocumento c ON c.hstid = d.hstid 
				WHERE d.docid='".$_SESSION['sispacto']['universidade']['docid']."'";
		
		$cmddsc = $db->pegaUm($sql);
		
		$sql = "SELECT iusnome, iusemailprincipal FROM sispacto.identificacaousuario  
				WHERE iusd='".$_SESSION['sispacto']['universidade']['iusd']."'";
		
		$identificacaousuario = $db->pegaLinha($sql);
		
		require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
		require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= "SIMEC";
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= "SIMEC - SISPACTO - Devolu��o do Projeto para altera��es";
		
		$mensagem->AddAddress( $identificacaousuario['iusemailprincipal'], $identificacaousuario['iusnome'] );
		$mensagem->AddAddress( "alexandre.dourado@mec.gov.br" );
		
			
		$mensagem->Body = "<p>Prezado(a) ".$identificacaousuario['iusnome']." (Coordenador IES)</p>
						   <p>Seu projeto no SISPACTO foi analisado e foi devolvido para altera��es.</p>
						   <p>As observa��es do avaliador foram:</p>".$cmddsc."<br/><p>Secretaria de Educa��o B�sica<br/>Minist�rio da Educa��o</p>";
		
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
				$sql = "UPDATE sispacto.atividadeuniversidade SET aundatainicio='".formata_data_sql($aundatainicio)."' WHERE suaid='".$suaid."' AND ecuid IN(SELECT ecuid FROM sispacto.estruturacurso WHERE uncid='".$_SESSION['sispacto']['universidade']['uncid']."')";
				$db->executar($sql);
			}		
		}
	}
	
	if($dados['aundatafim']) {
		foreach($dados['aundatafim'] as $suaid => $aundatafim) {
			if($aundatafim) {
				$sql = "UPDATE sispacto.atividadeuniversidade SET aundatafim='".formata_data_sql($aundatafim)."' WHERE suaid='".$suaid."' AND ecuid IN(SELECT ecuid FROM sispacto.estruturacurso WHERE uncid='".$_SESSION['sispacto']['universidade']['uncid']."')";
				$db->executar($sql);
			}		
		}
	}
	
	$db->commit();
	
	$al = array("alert"=>"Plano de atividade atualizado com sucesso","location"=>"sispacto.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=planoatividade");
	alertlocation($al);
	
}

function condicaoEnviarOrcamentoMec($uncid) {
	global $db;
	
	$sql = "SELECT ((coalesce(SUM(o.orcvlrexecutado),0) - coalesce(SUM(o.orcvlratualizado),0))) as preenchimento
			FROM sispacto.orcamento o 
			INNER JOIN sispacto.grupodespesa g ON g.gdeid = o.gdeid 
			WHERE o.uncid='".$uncid."' AND o.orcstatus='A'";
	
	$preenchimento = $db->pegaUm($sql);
	
	if(!$preenchimento) return "Preencha os dados referentes ao Valor Executado(R$) e Valor Atualizado(R$)";

	
	$sql = "SELECT ((SUM(o.orcvlrunitario)-coalesce(SUM(o.orcvlrexecutado),0) - coalesce(SUM(o.orcvlratualizado),0))) as diferenca
			FROM sispacto.orcamento o 
			INNER JOIN sispacto.grupodespesa g ON g.gdeid = o.gdeid 
			WHERE o.uncid='".$uncid."' AND o.orcstatus='A'";
	
	$diferenca = $db->pegaUm($sql);
	
	if($diferenca < 0) return "IES esta demandando mais recursos do que o valor total do projeto aprovado";
	return true;
	
}

function atualizarRelatorioFinal($dados) {
	global $db;
	
	$sql = "UPDATE sispacto.relatoriofinal
			   SET rlfperexecinicio=".(($dados['rlfperexecinicio'])?"'".formata_data_sql($dados['rlfperexecinicio'])."'":"NULL").", 
			   	   rlfperexecfim=".(($dados['rlfperexecfim'])?"'".formata_data_sql($dados['rlfperexecfim'])."'":"NULL").", 
			   	   rlfestruturafisica=".(($dados['rlfestruturafisica'])?"'".implode(";", $dados['rlfestruturafisica']).";'":"NULL").", 
			       rlfcolegiadocurso=".(($dados['rlfcolegiadocurso'])?"'".substr($dados['rlfcolegiadocurso'],0,2000)."'":"NULL").", 
			       rlfarticulacaoinstitucional=".(($dados['rlfarticulacaoinstitucional'])?"'".substr($dados['rlfarticulacaoinstitucional'],0,3000)."'":"NULL").", 
			       rlfcomentariosavaliacaocoordenadoreslocais=".(($dados['rlfcomentariosavaliacaocoordenadoreslocais'])?"'".substr($dados['rlfcomentariosavaliacaocoordenadoreslocais'],0,3000)."'":"NULL").", 
			       rlfplanejamentopedagogicocurso=".(($dados['rlfplanejamentopedagogicocurso'])?"'".substr($dados['rlfplanejamentopedagogicocurso'],0,2000)."'":"NULL").", 
			       rlforganizacaopedagogicacurso=".(($dados['rlforganizacaopedagogicacurso'])?"'".substr($dados['rlforganizacaopedagogicacurso'],0,2000)."'":"NULL").", 
			       rlfacompanhamentocursistas=".(($dados['rlfacompanhamentocursistas'])?"'".$dados['rlfacompanhamentocursistas']."'":"NULL").", 
			       rlfconteudocurso=".(($dados['rlfconteudocurso'])?"'".substr($dados['rlfconteudocurso'],0,2000)."'":"NULL").", 
			       rlfmetodologia=".(($dados['rlfmetodologia'])?"'".$dados['rlfmetodologia']."'":"NULL").", 
			       rlfcriteriosavaliacoes=".(($dados['rlfcriteriosavaliacoes'])?"'".substr($dados['rlfcriteriosavaliacoes'],0,2000)."'":"NULL").", 
			       rlfequipepedagogica=".(($dados['rlfequipepedagogica'])?"'".substr($dados['rlfequipepedagogica'],0,2000)."'":"NULL").", 
			       rlfarticulacaomec=".(($dados['rlfarticulacaomec'])?"'".$dados['rlfarticulacaomec']."'":"NULL").", 
			       rlflicoesaprendidas=".(($dados['rlflicoesaprendidas'])?"'".$dados['rlflicoesaprendidas']."'":"NULL").", 
			       rlfsugestoes=".(($dados['rlfsugestoes'])?"'".$dados['rlfsugestoes']."'":"NULL").", 
			       rlfoutroscomentarios=".(($dados['rlfoutroscomentarios'])?"'".$dados['rlfoutroscomentarios']."'":"NULL")."
			 WHERE uncid='".$_SESSION['sispacto']['universidade']['uncid']."'";
	
	$db->executar($sql);
	
	if($dados['orcvlrfinal']) {
		foreach($dados['orcvlrfinal'] as $orcid => $vlr) {
			$sql = "UPDATE sispacto.orcamento SET orcvlrfinal=".(($vlr)?"'".str_replace(array(".",","),array("","."),$vlr)."'":"NULL")." 
					WHERE orcid='".$orcid."'";
			
			$db->executar($sql);
		}
	}
	
	if($dados['orcdescricao']) {
		foreach($dados['orcdescricao'] as $orcid => $dsc) {
			$sql = "UPDATE sispacto.orcamento SET orcdescricaofinal=".(($dsc)?"'".$dsc."'":"NULL")."
					WHERE orcid='".$orcid."'";
				
			$db->executar($sql);
		}
	}
	
	
	
	$db->commit();
	
	$al = array("alert"=>"Relat�rio Final atualizado com sucesso","location"=>"sispacto.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=relatoriofinal");
	alertlocation($al);
	
}



function exibirCursistasRelatorioFinal($dados) {
	global $db;
	
	if($dados['pflcod']==PFL_PROFESSORALFABETIZADOR) {
		$pflcod_avaliador   = PFL_ORIENTADORESTUDO;
	}
	elseif($dados['pflcod']==PFL_ORIENTADORESTUDO) {
		$pflcod_avaliador   = PFL_FORMADORIES;
	}
	else die("Perfil n�o permiss�o para recomendar certifica��o.");

	$limit = $db->pegaUm("SELECT plpmaximobolsas FROM sispacto.pagamentoperfil WHERE pflcod='".$dados['pflcod']."'");

	$sql = "SELECT f.fpbid,  m.mesdsc || '/' || fpbanoreferencia as referencia FROM sispacto.folhapagamentouniversidade u
		INNER JOIN sispacto.folhapagamento f ON f.fpbid = u.fpbid
		INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
	    WHERE u.uncid='".$dados['uncid']."' ORDER BY f.fpbid LIMIT {$limit}";

	$folhapagamento = $db->carregar($sql);

	unset($l, $c, $a, $to, $fq);

	$l[] = "CPF";
	$l[] = "Nome";
	$l[] = "E-mail";
	$l[] = "UF";
	$l[] = "Munic�pio";
	$l[] = "Rede";

	if($folhapagamento) {
		foreach($folhapagamento as $key => $fpb) {

			$c[] = "COALESCE((SELECT AVG(mavtotal) as mavtotal FROM sispacto.mensarioavaliacoes ma
			INNER JOIN sispacto.mensario me ON me.menid = ma.menid
			WHERE me.iusd = i.iusd AND me.fpbid='{$fpb['fpbid']}'),0.00) as total_{$key}";

			$c[] = "COALESCE((SELECT AVG(mavfrequencia) as mavfrequencia FROM sispacto.mensarioavaliacoes ma
			INNER JOIN sispacto.mensario me ON me.menid = ma.menid
			WHERE me.iusd = i.iusd AND me.fpbid='{$fpb['fpbid']}'),0.0) as freq_{$key}";
			
			if($dados['xls']) {
				$a[] = "foo.total_{$key} as alias_nota";
				$a[] = "foo.freq_{$key} as alias_freq";
			} else {
				$a[] = "'<font style=font-size:xx-small;>'||foo.total_{$key}||'</font>' as alias_nota";
				$a[] = "'<font style=font-size:xx-small;>'||foo.freq_{$key}||'</font>' as alias_freq";
			}


			$to[] = "fee.total_{$key}";
			$fq[] = "fee.freq_{$key}";

			}
	}

	$l[] = "Nota M�dia";
	$l[] = "Frequ�ncia";
	$l[] = "Situa��o Final";
	
	if($dados['xls']) {
		
		$cls = "replace(to_char(foo.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf,
			   foo.iusnome as iusnome,
			   foo.iusemailprincipal as iusemailprincipal,
			   foo.estuf as estuf,
			   foo.mundescricao as mundescricao,
			   foo.rede as rede,
			   foo.avg_total as avl_nota,
			   foo.tot_freq||'%' as avl_freq,
			   CASE WHEN foo.mavrecomendadocertificacao='1' THEN 'Recomendado'
					WHEN foo.mavrecomendadocertificacao='2' THEN 'N�o recomendado'
					ELSE 'N�o avaliado ' END as acao";
		
	} else {
		$cls = "'<font style=font-size:xx-small;>'||replace(to_char(foo.iuscpf::numeric, '000:000:000-00'), ':', '.')||'</font>' as iuscpf,
			   '<font style=font-size:xx-small;>'||foo.iusnome||'</font>' as iusnome,
			   '<font style=font-size:xx-small;>'||foo.iusemailprincipal||'</font>' as iusemailprincipal,
			   '<font style=font-size:xx-small;>'||foo.estuf||'</font>' as estuf,
			   '<font style=font-size:xx-small;>'||foo.mundescricao||'</font>' as mundescricao,
			   '<font style=font-size:xx-small;>'||foo.rede||'</font>' as rede,
			   '<font style=font-size:xx-small;float:right;>'||foo.avg_total||'</font>' as avl_nota,
			   '<font style=font-size:xx-small;float:right;>'||foo.tot_freq||'%</font>' as avl_freq,
			   CASE WHEN foo.mavrecomendadocertificacao='1' THEN '<font style=font-size:xx-small;color:blue;>Recomendado</font>'
					WHEN foo.mavrecomendadocertificacao='2' THEN '<font style=font-size:xx-small;color:red;>N�o recomendado</font>'
					ELSE '<font style=font-size:xx-small;>N�o avaliado</font> ' END || CASE WHEN foo.mavrecomendadocertificacaojustificativa IS NOT NULL THEN ' <img src=\"../imagens/page_attach.png\" align=absmiddle onclick=\"alert(\''||foo.mavrecomendadocertificacaojustificativa||'\');\" style=\"cursor:pointer;\">' ELSE '' END as acao";
	}
	

		$sql = "SELECT

		{$cls}

		FROM (

		SELECT fee.iuscpf,
			   fee.iusnome,
			   fee.iusemailprincipal,
			   fee.estuf,
			   fee.mundescricao,
			   fee.rede,
			   round((".implode("+",$to).")/".count($to).",2) as avg_total,
			   round(((".implode("+",$fq).")*100)/".count($fq).",0) as tot_freq,
			   (SELECT mavrecomendadocertificacao FROM sispacto.mensarioavaliacoes ma INNER JOIN sispacto.mensario m ON m.menid = ma.menid INNER JOIN sispacto.identificacaousuario i ON i.iusd = m.iusd INNER JOIN sispacto.tipoperfil t ON t.iusd = ma.iusdavaliador AND t.pflcod=".$pflcod_avaliador." WHERE i.iuscpf = fee.iuscpf AND mavrecomendadocertificacao is not null) as mavrecomendadocertificacao,
			   (SELECT mavrecomendadocertificacaojustificativa FROM sispacto.mensarioavaliacoes ma INNER JOIN sispacto.mensario m ON m.menid = ma.menid INNER JOIN sispacto.identificacaousuario i ON i.iusd = m.iusd INNER JOIN sispacto.tipoperfil t ON t.iusd = ma.iusdavaliador AND t.pflcod=".$pflcod_avaliador." WHERE i.iuscpf = fee.iuscpf AND mavrecomendadocertificacao is not null) as mavrecomendadocertificacaojustificativa

	    FROM (

	    SELECT i.iuscpf, i.iusnome, i.iusemailprincipal, m.estuf, m.mundescricao, CASE WHEN p.muncod IS NOT NULL THEN 'Municipal' WHEN p.estuf IS NOT NULL THEN 'Estadual' END as rede, ".implode(",", $c)."
		FROM sispacto.identificacaousuario i
		INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
	    LEFT JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
	    LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao  
		WHERE i.iusstatus='A' AND t.pflcod='".$dados['pflcod']."' AND i.uncid='".$dados['uncid']."' AND i.iuscpf NOT ILIKE 'SIS%'
			   		ORDER BY i.iusnome

		) fee

		) foo";

	if($dados['xls']) {
		
		ob_clean();
		
		header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
		header("Pragma: no-cache");
		header("Content-type: application/xls; name=SIMEC_Recomendados_".date("Ymdhis").".xls");
		header("Content-Disposition: attachment; filename=SIMEC_Recomendados_".date("Ymdhis").".xls");
		header("Content-Description: MID Gera excel");
		
		$db->monta_lista_tabulado($sql, $l, 10000000, 5, 'N', '100%', '');
		
		
	} else {
		$db->monta_lista_simples($sql,$l,10000000,10,'N','95%','center');
	}
	
	$db->executar("DELETE FROM sispacto.certificacao WHERE uncid='".$dados['uncid']."'");
	$db->executar("INSERT INTO sispacto.certificacao(
            	   iusd, pflcod, cerfrequencia, cercertificou, uncid)
			
					SELECT
			
					foo.iusd, 
			   		".$dados['pflcod'].",
					foo.tot_freq as avl_freq,
				    CASE WHEN foo.mavrecomendadocertificacao='1' THEN TRUE
						 WHEN foo.mavrecomendadocertificacao='2' THEN FALSE
						 ELSE NULL END as cercertificou,
					foo.uncid
			
					FROM (
			
					SELECT fee.iusd,
						   fee.uncid,
						   round((".implode("+",$to).")/".count($to).",2) as avg_total,
						   round(((".implode("+",$fq).")*100)/".count($fq).",0) as tot_freq,
						   (SELECT mavrecomendadocertificacao FROM sispacto.mensarioavaliacoes ma INNER JOIN sispacto.mensario m ON m.menid = ma.menid INNER JOIN sispacto.identificacaousuario i ON i.iusd = m.iusd INNER JOIN sispacto.tipoperfil t ON t.iusd = ma.iusdavaliador AND t.pflcod=".$pflcod_avaliador." WHERE i.iuscpf = fee.iuscpf AND mavrecomendadocertificacao is not null) as mavrecomendadocertificacao,
						   (SELECT mavrecomendadocertificacaojustificativa FROM sispacto.mensarioavaliacoes ma INNER JOIN sispacto.mensario m ON m.menid = ma.menid INNER JOIN sispacto.identificacaousuario i ON i.iusd = m.iusd INNER JOIN sispacto.tipoperfil t ON t.iusd = ma.iusdavaliador AND t.pflcod=".$pflcod_avaliador." WHERE i.iuscpf = fee.iuscpf AND mavrecomendadocertificacao is not null) as mavrecomendadocertificacaojustificativa
			
				    FROM (
			
				    SELECT i.iuscpf, i.uncid, i.iusd, ".implode(",", $c)."
					FROM sispacto.identificacaousuario i
					INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
				    LEFT JOIN sispacto.pactoidadecerta p ON p.picid = i.picid 
				    LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao  
					WHERE i.iusstatus='A' AND t.pflcod='".$dados['pflcod']."' AND i.uncid='".$dados['uncid']."' AND i.iuscpf NOT ILIKE 'SIS%'
					ORDER BY i.iusnome
			
					) fee
			
					) foo");
	
	$db->commit();
	

}

function inserirEncontroPresencial($dados) {
	global $db;
	
	$sql = "INSERT INTO sispacto.relatoriofinalencontrospresenciais(
            rlfid, repnome, repdata, repcargahoraria)
    		VALUES ((SELECT rlfid FROM sispacto.relatoriofinal WHERE uncid='".$dados['uncid']."'), '".utf8_decode($dados['repnome'])."', '".formata_data_sql($dados['repdata'])."', ".(($dados['repcargahoraria'])?"'".$dados['repcargahoraria']."'":"NULL").");";
	
	$db->executar($sql);
	$db->commit();
	
}

function listarEncontrosPresenciais($dados) {
	global $db;
	
	$sql = "SELECT '<center><img id=\"encontrop_'||repid||'\" src=../imagens/excluir.gif align=absmiddle style=cursor:pointer; onclick=excluirEncontroPresencial('||repid||');></center>' as acao, repnome, to_char(repdata,'dd/mm/YYYY') as repdata, repcargahoraria FROM sispacto.relatoriofinalencontrospresenciais WHERE rlfid IN( SELECT rlfid FROM sispacto.relatoriofinal WHERE uncid='".$dados['uncid']."' )";
	$cabecalho = array("&nbsp;","Evento","Data","Carga Hor�ria");
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','center');

}

function excluirEncontroPresencial($dados) {
	global $db;
	
	$sql = "DELETE FROM sispacto.relatoriofinalencontrospresenciais WHERE repid='".$dados['repid']."'";
	$db->executar($sql);
	$db->commit();
}





?>