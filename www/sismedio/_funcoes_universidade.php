<?

function excluirAbrangencia($dados) {
	global $db;
	
	$sql = "SELECT * FROM sismedio.abrangencia WHERE abrid='".$dados['abrid']."'";
	$abr = $db->pegaLinha($sql);
	
	if($abr['esfera']=='M') {
		
		$sql = "DELETE FROM sismedio.orientadorturma WHERE iusd IN(
				select i.iusd from sismedio.identificacaousuario i 
				inner join sismedio.pactoidadecerta p on p.picid = i.picid
				inner join sismedio.tipoperfil t on t.iusd = i.iusd 
				where pflcod=".PFL_ORIENTADORESTUDO." and p.muncod='".$abr['muncod']."')";
		
		$db->executar($sql);
	}
	
	if($abr['esfera']=='E') {
		
		$sql = "DELETE FROM sismedio.orientadorturma WHERE iusd IN(
				select i.iusd from sismedio.identificacaousuario i 
				inner join territorios.municipio mm on mm.muncod = i.muncodatuacao
				inner join sismedio.pactoidadecerta p on p.estuf = mm.estuf and p.picid = i.picid 
				inner join sismedio.tipoperfil t on t.iusd = i.iusd 
				where pflcod=".PFL_ORIENTADORESTUDO." and mm.muncod='".$abr['muncod']."')";
		
		$db->executar($sql);
		
	}
	
	$sql = "DELETE FROM sismedio.abrangencia WHERE abrid='".$dados['abrid']."'";
	$db->executar($sql);
	$db->commit();
}


function definirAbrangencia($dados) {
	global $db;
	
	$total   = totalAlfabetizadoresAbrangencia(array("uncid"=>$dados['uncid']));
	$nturmas = totalTurmasAbrangencia(array("total"=>$total)); 
	$pp = carregarNumeroOrientadoresPendencia(array("ecuid" => $dados['ecuid']));

	
	echo "<p><b>Escolas atendidas</b></p>";

	
	$sql = "SELECT
			".((!$dados['consulta'])?"'<center><img src=\"../imagens/excluir.gif\" border=\"0\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirAbrangencia('||a.abrid||');\"></center>'":"''")." as acao2,
			m.lemuf||' / '||m.lemmundsc||' - '||m.lemnomeescola as descricao,
			replace(to_char(m.lemcpfgestor::numeric, '000:000:000-00'), ':', '.')||' - '||m.lemnomegestor as diretor,
			COALESCE(e.esddsc,'Não iniciado') as situacao,
			(SELECT count(*) FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd LEFT JOIN sismedio.definicaoorientadoresestudo doe ON doe.iusd = i.iusd AND doe.doecodigoinep = i.iuscodigoinep WHERE t.pflcod IN(".PFL_PROFESSORALFABETIZADOR.",".PFL_COORDENADORPEDAGOGICO.") AND doe.doeid IS NULL AND i.iuscodigoinep=m.lemcodigoinep::bigint) as qtdprof,
			(SELECT count(*) FROM sismedio.identificacaousuario i INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd INNER JOIN sismedio.definicaoorientadoresestudo doe ON doe.iusd = i.iusd AND doe.doecodigoinep = i.iuscodigoinep WHERE i.iuscodigoinep=m.lemcodigoinep::bigint) as qtdorientador
			FROM sismedio.abrangencia a
			INNER JOIN sismedio.listaescolasensinomedio m ON m.lemcodigoinep = a.lemcodigoinep 
			LEFT JOIN workflow.documento d ON d.docid = m.docid 
			LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			WHERE a.ecuid='".$dados['ecuid']."'
			ORDER BY 2";
	
	$cabecalho = array("&nbsp;","UF/ Município - Escola", "CPF - Diretor","Situação cadastramento","Qtd professores / coordenadores pedagógicos","Qtd orientador de estudo");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%','N',true, false, false, true);
	
	echo "<br>";
	echo "<p><b>Quantitativo por perfil</b></p>";
	
	$sql = "SELECT foo.perfil, count(distinct foo.iusd) as qtd FROM (
				SELECT
							CASE WHEN doe.doeid IS NOT NULL         THEN 'Orientador de Estudo'
							     WHEN i.iustipoprofessor='cpflivre' AND t.pflcod=".PFL_PROFESSORALFABETIZADOR." THEN 'Professor (Não bolsista)'
							     ELSE p.pfldsc END as perfil,
							i.iusd
							FROM sismedio.abrangencia a
							INNER JOIN sismedio.listaescolasensinomedio m ON m.lemcodigoinep = a.lemcodigoinep 
							INNER JOIN sismedio.identificacaousuario i ON i.iuscodigoinep = m.lemcodigoinep::bigint 
							INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
							LEFT JOIN sismedio.definicaoorientadoresestudo doe ON doe.iusd = i.iusd AND doe.doecodigoinep = i.iuscodigoinep 
							INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
							LEFT JOIN workflow.documento d ON d.docid = m.docid 
							LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
							WHERE a.ecuid='".$dados['ecuid']."' AND t.pflcod IN(".PFL_PROFESSORALFABETIZADOR.",".PFL_COORDENADORPEDAGOGICO.")
				) foo 
			GROUP BY foo.perfil 
			ORDER BY foo.perfil";
	
	$cabecalho = array("Perfil","Quantidade");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
	
	
	
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
	
	$sql = "SELECT COUNT(*) FROM sismedio.abrangencia a 
			LEFT JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid
			LEFT JOIN sismedio.pactoidadecerta p ON p.muncod=a.muncod
			LEFT JOIN workflow.documento d ON d.docid = p.docid 
			LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
			WHERE ".(($dados['uncid'])?"e.uncid='".$dados['uncid']."' AND":"")." ".(($dados['ecuid'])?"a.ecuid='".$dados['ecuid']."' AND":"")." a.esfera='M' AND (d.esdid IN('".ESD_ELABORACAO_COORDENADOR_LOCAL."','".ESD_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL."','".ESD_ANALISE_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL."') OR d.esdid IS NULL)";
	
	$nummunpendencias = $db->pegaUm($sql);
	
	$sql = "SELECT COUNT(DISTINCT mm.estuf) FROM sismedio.pactoidadecerta p 
			INNER JOIN territorios.municipio mm ON mm.estuf = p.estuf  
			LEFT JOIN workflow.documento d ON d.docid = p.docid 
			LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
			INNER JOIN sismedio.abrangencia a ON mm.muncod=a.muncod 
			LEFT JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
			WHERE ".(($dados['uncid'])?"e.uncid='".$dados['uncid']."' AND":"")." ".(($dados['ecuid'])?"a.ecuid='".$dados['ecuid']."' AND":"")." (d.esdid IN('".ESD_ELABORACAO_COORDENADOR_LOCAL."','".ESD_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL."','".ESD_ANALISE_TROCANDO_ORIENTADORES_COORDENADOR_LOCAL."') OR d.esdid IS NULL) AND a.esfera='E'
			GROUP BY p.estuf";
	
	$numestpendencias = $db->pegaUm($sql);
	
	return array("nummunpendencias" => $nummunpendencias, "numestpendencias" => $numestpendencias);	
	
}

function totalAlfabetizadoresAbrangencia($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*) FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.pactoidadecerta p ON p.picid = i.picid
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sismedio.abrangencia a ON p.muncod = a.muncod 
			INNER JOIN sismedio.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid = '".$dados['uncid']."' AND a.esfera='M'";
	
	$totalmunicipio = $db->pegaUm($sql);
	
	$sql = "SELECT COUNT(*) FROM sismedio.identificacaousuario i 
			INNER JOIN  territorios.municipio mm ON mm.muncod = i.muncodatuacao
			INNER JOIN sismedio.pactoidadecerta p ON p.estuf = mm.estuf AND p.picid = i.picid
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sismedio.abrangencia a ON mm.muncod = a.muncod 
			INNER JOIN sismedio.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid = '".$dados['uncid']."' AND a.esfera='E'";
	
	$totalestado = $db->pegaUm($sql);
	
	$total = $totalmunicipio + $totalestado;
	
	return $total;
}

function cadastrarEscolaAbrangencia($dados) {
	global $db;
	
	$lemcodigoinep_abrangencia = $db->carregarColuna("SELECT 'Não foi possível vincular a escola - '||lemnomeescola||' esta cadastrado na Universidade: '||un.uninome as descricao FROM sismedio.abrangencia a 
						 						  INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
						 						  INNER JOIN sismedio.listaescolasensinomedio l ON l.lemcodigoinep = a.lemcodigoinep 
						 						  INNER JOIN sismedio.universidadecadastro u ON u.uncid = e.uncid 
						 						  INNER JOIN sismedio.universidade un ON un.uniid = u.uniid  
						 						  WHERE uncstatus='A' AND a.muncod IN('".implode("','",$dados['lemcodigoinep_abrangencia'])."') LIMIT 10");
	
	if(!$lemcodigoinep_abrangencia) {
		
		foreach($dados['lemcodigoinep_abrangencia'] as $inep) {
			if($inep) {
				
				$sql = "INSERT INTO sismedio.abrangencia(
			            lemcodigoinep, ecuid, abrstatus)
			    		VALUES ('".$inep."', '".$dados['ecuid']."', 'A');";
				
				$db->executar($sql);
				
			}
		}
	
		$db->commit();
		
 		$al = array("alert"=>"Escolas gravado com sucesso","javascript"=>"window.opener.definirAbrangencia();window.close();");
 		alertlocation($al);
	
	} else {
		
 		$al = array("alert"=>implode('\n',$lemcodigoinep_abrangencia),"javascript"=>"window.opener.definirAbrangencia();window.close();");
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
		FROM sismedio.universidadecadastro un 
		INNER JOIN sismedio.universidade su ON su.uniid = un.uniid 
		LEFT JOIN territorios.municipio mu ON mu.muncod = su.muncod 
		LEFT JOIN sismedio.reitor re on re.uniid = su.uniid 
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
	
	$sql = "UPDATE sismedio.universidadecadastro SET uncdatainicioprojeto='".formata_data_sql($dados['uncdatainicioprojeto'])."',
													 uncdatafimprojeto='".formata_data_sql($dados['uncdatafimprojeto'])."',
													 unctipo='".$dados['unctipo']."',
													 unctipocertificacao='".$dados['unctipocertificacao']."' 
		    WHERE uncid='".$dados['uncid']."'";
	
	$db->executar($sql);

	$sql = "UPDATE sismedio.universidade
   			SET unisigla='".$dados['unisigla']."', uninome='".$dados['uninome']."', unicnpj='".str_replace(array(".","-","/"),array("","",""),$dados['unicnpj'])."', unicep='".str_replace(array("-"),array(""),$dados['unicep'])."', 
		        unilogradouro='".$dados['unilogradouro']."', unibairro='".$dados['unibairro']."', unicomplemento=".(($dados['unicomplemento'])?"'".$dados['unicomplemento']."'":"NULL").", unidddcomercial='".$dados['unidddcomercial']."', 
       			uninumcomercial='".$dados['uninumcomercial']."', uniemail='".$dados['uniemail']."', uniuf='".$dados['uniuf']."', uninumero='".$dados['uninumero']."', 
       			unisite='".$dados['unisite']."', muncod='".$dados['muncod_endereco']."'
 			WHERE uniid IN(SELECT uniid FROM sismedio.universidadecadastro WHERE uncid='".$dados['uncid']."')";
	
	$db->executar($sql);
	
	$sql = "UPDATE sismedio.reitor
   			SET reinome='".$dados['reinome']."', reicpf='".str_replace(array(".","-"),array("",""),$dados['reicpf'])."', reidddcomercial='".$dados['reidddcomercial']."', reinumcomercial='".$dados['reinumcomercial']."', 
       			reiemail='".$dados['reiemail']."'
 			WHERE uniid IN(SELECT uniid FROM sismedio.universidadecadastro WHERE uncid='".$dados['uncid']."')";
	
	$db->executar($sql);
	
	$db->commit();
	
 	$al = array("alert"=>"Dados Gerais do Projeto inseridos com sucesso.","location"=>$dados['goto']);
 	alertlocation($al);
	
}



function inserirCoordenadorIESGerenciamento($dados) {
	global $db;
	
	$sql = "SELECT iusd FROM sismedio.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'";
 	$iusd = $db->pegaUm($sql);
	
 	if($iusd) {
 		$sql = "UPDATE sismedio.identificacaousuario SET iusstatus='A', uncid='".$dados['uncid']."', iusemailprincipal='".$dados['iusemailprincipal']."' WHERE iusd='".$iusd."'";
 		$db->executar($sql);
 	} else {
     	$sql = "INSERT INTO sismedio.identificacaousuario(
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
 		
 		$remetente = array("nome" => "SIMEC - MÓDULO SISMÉDIO","email" => $dados['iusemailprincipal']);
 		$destinatario = $dados['iusemailprincipal'];
 		$usunome = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
 		$assunto = "Cadastro no SIMEC - MÓDULO SISMÉDIO";
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
 	}	
    	
    $existe_sis = $db->pegaUm("select usucpf from seguranca.usuario_sistema where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and sisid='".SIS_MEDIO."'");
    	
    if(!$existe_sis) {
    		
    	$sql = "INSERT INTO seguranca.usuario_sistema(
         	    usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
     			VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', ".SIS_MEDIO.", 'A', NULL, NOW(), 'A');";
	    	
     	$db->executar($sql);
	    	
    } else {
    	if($dados['suscod']=="A") {
 	    	$sql = "UPDATE seguranca.usuario_sistema SET suscod='A' WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' AND sisid='".SIS_MEDIO."'";
 	    	$db->executar($sql);
    	}
    }
    	
    $existe_pfl = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and pflcod='".PFL_COORDENADORIES."'");
    	
    if(!$existe_pfl) {
    	$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', '".PFL_COORDENADORIES."');";
     	$db->executar($sql);
    }
   	
    $existe_usr = $db->pegaUm("select usucpf from sismedio.usuarioresponsabilidade where usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."' and pflcod='".PFL_COORDENADORIES."' AND rpustatus='A' AND uncid='".$dados['uncid']."'");
    
     if(!$existe_usr) {
    		$sql = "INSERT INTO sismedio.usuarioresponsabilidade(
            		pflcod, usucpf, rpustatus, rpudata_inc, uncid)
 			    VALUES ('".PFL_COORDENADORIES."', '".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', 'A', NOW(), '".$dados['uncid']."');";
    		$db->executar($sql);
     }
    
     $arrTp = $db->pegaLinha("SELECT
     							  t.pflcod, 
     							  p.pfldsc, 
     							  uni.unisigla || '/' || uni.uninome as descricao 
     						FROM sismedio.tipoperfil t 
     						INNER JOIN seguranca.perfil p ON p.pflcod=t.pflcod 
     						INNER JOIN sismedio.identificacaousuario i ON i.iusd=t.iusd 
     						INNER JOIN sismedio.universidadecadastro unc ON unc.uncid=i.uncid 
     						LEFT JOIN sismedio.universidade uni ON uni.uniid=unc.uniid 
     						WHERE t.iusd='".$iusd."'");
     
     $existe_tpf = $arrTp['pflcod'];
    
     if(!$existe_tpf) {
 		$sql = "INSERT INTO sismedio.tipoperfil(
 		            iusd, pflcod, tpestatus)
 		    	VALUES ('".$iusd."', '".PFL_COORDENADORIES."', 'A');";
     	$db->executar($sql);
     } else {
    	
     	if($existe_tpf!=PFL_COORDENADORIES) {
 			$al = array("alert"=>"Este CPF ja possui um perfil (".$arrTp['pfldsc'].",".$arrTp['descricao'].") no sistema e não pode ser cadastrado","location"=>"sismedio.php?modulo=principal/coordenadorlocal/gerenciarcoordenadorlocal&acao=A&picid=".$dados['picid']);
 			alertlocation($al);
     	}
    	
     }
    
     $db->commit();
			
 	$al = array("alert"=>"Coordenador-Geral da IES inserido com sucesso","javascript"=>"window.opener.location=window.opener.location","location"=>"sismedio.php?modulo=principal/universidade/gerenciarcoordenadories&acao=A&uncid=".$dados['uncid']);
 	alertlocation($al);
	
 }





function carregarCoordenadorIES($dados) {
	global $db;
	$arr = $db->pegaLinha("SELECT d.esdid, u.uncid, su.uniuf, u.curid, u.docid, su.unisigla||' - '||su.uninome as descricao, u.docidformacaoinicial FROM sismedio.universidadecadastro u 
					 	   INNER JOIN sismedio.universidade su ON su.uniid = u.uniid
						   LEFT JOIN workflow.documento d ON d.docid = u.docid 
						   WHERE u.uncid='".$dados['uncid']."'");
	
	$docid = $arr['docid'];
	
	if(!$docid) {
		$docid = wf_cadastrarDocumento(TPD_COORDENADORIES,"SIS Médio Coordenador Geral IES : ".$dados['uncid']);
		$db->executar("UPDATE sismedio.universidadecadastro SET docid='".$docid."' WHERE uncid='".$dados['uncid']."'");
		$db->commit();
	}
	
	$docidformacaoinicial = $arr['docidformacaoinicial'];
	
	if(!$docidformacaoinicial) {
		$docidformacaoinicial = wf_cadastrarDocumento(TPD_FORMACAOINICIAL,"SisMédio Formação Inicial : ".$dados['uncid']);
		$db->executar("UPDATE sismedio.universidadecadastro SET docidformacaoinicial='".$docidformacaoinicial."' WHERE uncid='".$dados['uncid']."'");
		$db->commit();
	}
	
	
	$infprof = $db->pegaLinha("SELECT i.iusd, i.iusnome, i.iuscpf 
							   FROM sismedio.identificacaousuario i 
							   INNER JOIN sismedio.tipoperfil t ON t.iusd=i.iusd 
							   WHERE i.uncid='".$dados['uncid']."' AND t.pflcod='".PFL_COORDENADORIES."'");
	
	
	$_SESSION['sismedio']['universidade'] = array("descricao" => $arr['descricao']."( ".$infprof['iusnome']." )",
												  "curid" => $arr['curid'], 
												  "uncid" => $arr['uncid'], 
												  "reiid" => $arr['reiid'], 
												  "estuf" => $arr['uniuf'], 
												  "docid" => $docid,
												  "docidformacaoinicial" => $docidformacaoinicial,
												  "iusd" => $infprof['iusd'],
												  "iuscpf" => $infprof['iuscpf']);
	
	$_SESSION['sismedio']['universidadeexecucao'] = array("descricao" => $arr['descricao']."( ".$infprof['iusnome']." )",
														  "curid" => $arr['curid'],
														  "uncid" => $arr['uncid'],
														  "reiid" => $arr['reiid'],
														  "estuf" => $arr['uniuf'],
														  "docid" => $docid,
														  "docidformacaoinicial" => $docidformacaoinicial,
														  "iusd" => $infprof['iusd'],
														  "iuscpf" => $infprof['iuscpf']);
	
	
	if($dados['direcionar']) {
		if($arr['esdid']==ESD_VALIDADO_COORDENADOR_IES) $al = array("location"=>"sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=principal");
		else $al = array("location"=>"sismedio.php?modulo=principal/universidade/universidade&acao=A&aba=principal");
		alertlocation($al);
	}
	
}

function carregarSubAtividades($dados) {
	global $db;
	$sql = "SELECT suaid as codigo, suadesc as descricao FROM sismedio.subatividades WHERE atiid='".$dados['atiid']."'";
	$db->monta_combo('suaid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'suaid', '');
	
}

function carregarUniversidadesPorUF($dados) {
	global $db;
	$sql = "SELECT u.uncid as codigo, su.uninome as descricao FROM sismedio.universidadecadastro u 
	 	    INNER JOIN sismedio.universidade su ON su.uniid = u.uniid
			WHERE su.uniuf='".$dados['estuf']."'";
	
	$combo = $db->monta_combo($dados['name'], $sql, 'S', 'Selecione', (($dados['onclick'])?$dados['onclick']:''), '', '', '', 'S', $dados['id'], true, $dados['valuecombo']);
	
	if($dados['returncombo']) return $combo;
	else echo $combo;
}

function carregarEstruturaCurso($dados) {
	global $db;
	$sql = "SELECT * FROM sismedio.estruturacurso e
			LEFT JOIN territorios.municipio m ON m.muncod = e.muncod 
			WHERE e.ecuid='".$dados['ecuid']."'";
	$estruturacurso = $db->pegaLinha($sql);
	return $estruturacurso;
}

function atualizarEstruturaCurso($dados) {
	global $db;
	$sql = "UPDATE sismedio.estruturacurso SET muncod='".$dados['muncod_endereco']."', ecuobsplanoatividades=".(($dados['ecuobsplanoatividades'])?"'".substr($dados['ecuobsplanoatividades'],0,5000)."'":"NULL")." WHERE ecuid='".$dados['ecuid']."'";
	$db->executar($sql);
	
	if($dados['aundatainicioprev']) $suaids = array_keys($dados['aundatainicioprev']);
	
	if($suaids) {
		foreach($suaids as $suaid) {
			$aunid = $db->pegaUm("SELECT aunid FROM sismedio.atividadeuniversidade au WHERE au.suaid = '".$suaid."' AND au.ecuid = '".$dados['ecuid']."'");
			
			if($aunid) {
				$sql = "UPDATE sismedio.atividadeuniversidade SET aundatainicioprev=".(($dados['aundatainicioprev'][$suaid])?"'".formata_data_sql($dados['aundatainicioprev'][$suaid])."'":"NULL").", aundatafimprev=".(($dados['aundatafimprev'][$suaid])?"'".formata_data_sql($dados['aundatafimprev'][$suaid])."'":"NULL")." WHERE aunid='".$aunid."'";
				$db->executar($sql);
			} else {
				$sql = "INSERT INTO sismedio.atividadeuniversidade(
            			suaid, aundatainicioprev, aundatafimprev, aunstatus, ecuid)
    					VALUES ('".$suaid."', ".(($dados['aundatainicioprev'][$suaid])?"'".formata_data_sql($dados['aundatainicioprev'][$suaid])."'":"NULL").", ".(($dados['aundatafimprev'][$suaid])?"'".formata_data_sql($dados['aundatafimprev'][$suaid])."'":"NULL").", 'A', '".$dados['ecuid']."');";
				$db->executar($sql);
			}
		}
	}
	
	$ainid = $db->pegaUm("SELECT ainid FROM sismedio.articulacaoinstitucional WHERE ecuid='".$dados['ecuid']."'");
	
	if($ainid) {
		$sql = "UPDATE sismedio.articulacaoinstitucional
   				SET ainseduc=".$dados['ainseduc'].", 
   					ainseducjustificativa='".$dados['ainseducjustificativa']."' 
				 WHERE ainid='".$ainid."';";
		
		$db->executar($sql);
		
	} else {
		$sql = "INSERT INTO sismedio.articulacaoinstitucional(
	            ecuid, ainseduc, ainseducjustificativa, ainstatus)
	    		VALUES ('".$dados['ecuid']."', ".$dados['ainseduc'].", '".$dados['ainseducjustificativa']."', 'A');";
		
		$db->executar($sql);
		
	}
	
	$db->commit();
	$al = array("alert"=>"Estrutura do curso atualizada com sucesso","location"=>$dados['goto']);
	alertlocation($al);
	
}

function carregarArticulacaoInstitucional($dados) {
	global $db;
	$sql = "SELECT * FROM sismedio.articulacaoinstitucional WHERE ecuid='".$dados['ecuid']."'";
	$articulacaoinstitucional = $db->pegaLinha($sql);
	return $articulacaoinstitucional;
	
}


function carregarPlanoAtividades($dados) {
	global $db;
	
	$sql = "SELECT a.atiid, a.atidesc, s.suaid, s.suadesc, 
			".(($dados['ecuid'])?"(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au WHERE au.suaid = s.suaid AND au.ecuid='".$dados['ecuid']."')":"''")." as aundatainicioprev, 
			".(($dados['ecuid'])?"(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au WHERE au.suaid = s.suaid AND au.ecuid='".$dados['ecuid']."')":"''")." as aundatafimprev 
			FROM sismedio.subatividades s 
			INNER JOIN sismedio.atividades a ON a.atiid = s.atiid 
			WHERE a.attitipo IN('".$dados['attitipo']."') AND s.suavisivel=true ORDER BY a.atidesc, s.suadesc";
	
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
		echo "<td class=\"SubTituloEsquerda\" colspan=\"3\">Nenhuma Subatividade foi cadastrada</td>";
		echo "</tr>";
	}
	
	echo "</table>";
	
}

function carregarPlanoAtividadesExecucao($dados) {
	global $db;

	$sql = "SELECT a.atiid, a.atidesc, s.suaid, s.suadesc,
			".(($dados['ecuid'])?"(SELECT au.aundatainicioprev FROM sismedio.atividadeuniversidade au WHERE au.suaid = s.suaid AND au.ecuid='".$dados['ecuid']."')":"''")." as aundatainicioprev_,
			".(($dados['ecuid'])?"(SELECT au.aundatafimprev FROM sismedio.atividadeuniversidade au WHERE au.suaid = s.suaid AND au.ecuid='".$dados['ecuid']."')":"''")." as aundatafimprev_,
			
			".(($dados['ecuid'])?"(SELECT au.aundatainicioprev2 FROM sismedio.atividadeuniversidade au WHERE au.suaid = s.suaid AND au.ecuid='".$dados['ecuid']."')":"''")." as aundatainicioprev,
			".(($dados['ecuid'])?"(SELECT au.aundatafimprev2 FROM sismedio.atividadeuniversidade au WHERE au.suaid = s.suaid AND au.ecuid='".$dados['ecuid']."')":"''")." as aundatafimprev
			FROM sismedio.subatividades s
			INNER JOIN sismedio.atividades a ON a.atiid = s.atiid
			WHERE a.attitipo IN('".$dados['attitipo']."') AND s.suavisivel=true ORDER BY a.atidesc, s.suadesc";

	$subatividades = $db->carregar($sql);

	if($subatividades[0]) {
		foreach($subatividades as $sub) {
			$arrRsAgrupado[$sub['atidesc']]['atidesc'] = $sub['atidesc'];
			$arrRsAgrupado[$sub['atidesc']]['subatividades'][] = array("suaid"=>$sub['suaid'],
																		"suadesc"=>$sub['suadesc'],
																		"aundatainicioprev_"=>$sub['aundatainicioprev_'],
																		"aundatafimprev_"=>$sub['aundatafimprev_'],
																		"aundatainicioprev"=>$sub['aundatainicioprev'],
																		"aundatafimprev"=>$sub['aundatafimprev']);
		}
	}

	echo "<table class=\"listagem\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" width=\"100%\">";
	echo "<tr>";
	echo "<td width=\"40%\" class=\"SubTituloCentro\">ATIVIDADES / SUBATIVIDADES</td><td class=\"SubTituloCentro\" colspan=\"4\">PERÍODO DE EXECUÇÃO</td>";
	echo "</tr>";

	echo "<tr>";
	echo "<td width=\"40%\" align=\"center\">&nbsp;</td><td align=\"center\">Início (previsto)</td><td align=\"center\">Término (previsto)</td><td align=\"center\">Início</td><td align=\"center\">Término</td>";
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
					echo "<td align=center>".formata_data($su['aundatainicioprev_'])."</td>";
					echo "<td align=center>".formata_data($su['aundatafimprev_'])."</td>";
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
		echo "<td class=\"SubTituloEsquerda\" colspan=\"3\">Nenhuma Subatividade foi cadastrada</td>";
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
				FROM sismedio.orcamento o 
				INNER JOIN sismedio.grupodespesa g ON g.gdeid = o.gdeid 
				WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A' 
				ORDER BY g.gdedesc
				) UNION ALL (
				
				SELECT '<b>TOTAIS</b>' as tot, 
					   '&nbsp;' as tot2, 
					   '<input type=\"text\" style=\"text-align:;\" name=\"totalvalorprevisto\" size=\"16\" maxlength=\"14\" value=\"'||trim(to_char(SUM(o.orcvlrunitario),'999g999g999d99'))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"totalvalorprevisto\" title=\"Total Valor Previsto\" readonly=\"readonly\" class=\" disabled\">' as totalvalorprevisto, 
					   '<input type=\"text\" style=\"text-align:;\" name=\"totalvalorexecutado\" size=\"16\" maxlength=\"14\" value=\"'||trim(coalesce(to_char(SUM(o.orcvlrexecutado),'999g999g999d99'),''))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"totalvalorexecutado\" title=\"Total Valor executado\" readonly=\"readonly\" class=\" disabled\">' as totalvalorexecutado,
					   '<input type=\"text\" style=\"text-align:;\" name=\"totalsaldo\" size=\"16\" maxlength=\"14\" value=\"'||trim(to_char(SUM(o.orcvlrunitario)-coalesce(SUM(o.orcvlrexecutado),0),'999g999g999d99'))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"totalsaldo\" title=\"Saldo\" readonly=\"readonly\" class=\" disabled\">' as totalsaldo 
				FROM sismedio.orcamento o 
				INNER JOIN sismedio.grupodespesa g ON g.gdeid = o.gdeid 
				WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A' 
				
				)";
		
		$cabecalho = array("Elementos de Despesa","Unidade de medida","Valor previsto (R$)","Valor executado (R$)","Saldo (R$)");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','S');
		
		
	} else {
	
		$sql = "SELECT ".(($dados['consulta'])?"''":"'<center><img src=../imagens/alterar.gif style=\"cursor:pointer;\" onclick=\"inserirCustos(\''||o.orcid||'\');\"> <img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirCustos(\''||o.orcid||'\');\"></center>'")." as acao, g.gdedesc, 'Verba', o.orcvlrunitario, o.orcdescricao 
				FROM sismedio.orcamento o 
				INNER JOIN sismedio.grupodespesa g ON g.gdeid = o.gdeid 
				WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A' 
				ORDER BY g.gdedesc";
		$cabecalho = array("&nbsp;","Elementos de Despesa","Unidade de Medida","Valor total (R$)","Detalhamento");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'S','100%','S');
	
	}
	
	
}

function carregarNaturezaDespesasCustos($dados) {
	global $db;
	$sql = "SELECT n.ndecodigo, n.ndedesc, SUM(o.orcvlrtotal) as total 
			FROM sismedio.orcamento o 
			INNER JOIN sismedio.itemdespesa i ON i.ideid = o.ideid 
			INNER JOIN sismedio.grupodespesa g ON g.gdeid = i.gdeid 
			INNER JOIN sismedio.naturezadespesa n ON n.ndeid = g.ndeid  
			WHERE o.uncid='".$dados['uncid']."' AND o.orcstatus='A' 
			GROUP BY n.ndecodigo, n.ndedesc";
	
	$cabecalho = array("Código","Descrição","Valor(R$)");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}

function carregarOrcamento($dados) {
	global $db;
	$sql = "SELECT * FROM sismedio.orcamento o 
			INNER JOIN sismedio.grupodespesa g ON o.gdeid = g.gdeid 
			WHERE orcid='".$dados['orcid']."'";
	
	$orcamento = $db->pegaLinha($sql);
	
	return $orcamento;
	
}

function atualizarOrcamentoExecucao($dados) {
	global $db;
	
	if($dados['orcvalorexecutado']) {
		foreach(array_keys($dados['orcvalorexecutado']) as $orcid) {
			$sql = "UPDATE sismedio.orcamento SET orcvlrexecutado=".(($dados['orcvalorexecutado'][$orcid])?"'".str_replace(array(".",","),array("","."),$dados['orcvalorexecutado'][$orcid])."'":"NULL").",
												  orcvlratualizado=".(($dados['orcvaloratualizado'][$orcid])?"'".str_replace(array(".",","),array("","."),$dados['orcvaloratualizado'][$orcid])."'":"NULL")."
					WHERE orcid='".$orcid."'";

			$db->executar($sql);
			$db->commit();
		}
	}
	
	$al = array("alert"=>"Orçamento atualizado com sucesso.","location"=>"sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=orcamentoexecucao");
	alertlocation($al);
	
}

function atualizarCusto($dados) {
	global $db;
	$sql = "UPDATE sismedio.orcamento SET gdeid='".$dados['gdeid']."', orcvlrunitario='".str_replace(array(".",","),array("","."),$dados['orcvlrunitario'])."', orcdescricao='".$dados['orcdescricao']."'
			WHERE orcid='".$dados['orcid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Custo inserido com sucesso","javascript"=>"window.opener.carregarListaCustos();window.close();");
	alertlocation($al);
	
}

function excluirCustos($dados) {
	global $db;
	$sql = "DELETE FROM sismedio.orcamento WHERE orcid='".$dados['orcid']."'";
	$db->executar($sql);
	$db->commit();
	
	
}

function inserirCusto($dados) {
	global $db;
	$sql = "INSERT INTO sismedio.orcamento(
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
		
		$sql = "SELECT i.iuscpf, i.iusnome, i.iusemailprincipal, es.esddsc  FROM sismedio.identificacaousuario i 
				INNER JOIN sismedio.pactoidadecerta p ON p.picid = i.picid 
				LEFT JOIN workflow.documento d ON d.docid = p.docid 
				LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN sismedio.abrangencia a ON p.muncod=a.muncod
				WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND a.ecuid='".$dados['ecuid']."' AND a.muncod='".$dados['muncod']."' AND a.esfera='M'";
		
		$cabecalho = array("CPF","Nome","Email","Situação cadastro");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	
	}
	
	if($dados['esfera']=='E') {
		
		$sql = "SELECT i.iuscpf, i.iusnome, i.iusemailprincipal, es.esddsc FROM sismedio.identificacaousuario i 
				INNER JOIN territorios.municipio mm ON mm.muncod = i.muncodatuacao
				INNER JOIN sismedio.pactoidadecerta p ON p.estuf = mm.estuf AND p.picid = i.picid 
				LEFT JOIN workflow.documento d ON d.docid = p.docid 
				LEFT JOIN workflow.estadodocumento es ON es.esdid = d.esdid 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN sismedio.abrangencia a ON mm.muncod=a.muncod
				WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND a.ecuid='".$dados['ecuid']."' AND a.muncod='".$dados['muncod']."' AND a.esfera='E'";
		
		$cabecalho = array("CPF","Nome","Email","Situação cadastro");
		$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
	}
	
}

function carregarEquipeRecursosHumanos($dados) {
	global $db;
	$sql = "(SELECT '&nbsp' as acao, '<center>'||replace(to_char(iuscpf::numeric, '000:000:000-00'), ':', '.')||'</center>' as iuscpf, iusnome, iusemailprincipal, p.pfldsc, to_char(t.tpeatuacaoinicio,'mm/YYYY')||' a '||to_char(t.tpeatuacaofim,'mm/YYYY') as periodo  
			FROM sismedio.identificacaousuario i 
			LEFT JOIN sismedio.portarianomeacao po ON po.iusd = i.iusd 
			LEFT JOIN public.arquivo ar ON ar.arqid = po.arqid
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sismedio.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			WHERE t.pflcod IN('".PFL_COORDENADORIES."') AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A')
			UNION ALL (
			SELECT ".((!$dados['consulta'])?"'<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"inserirEquipe(\''||i.iusd||'\');\" > <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirEquipeRecursosHumanos(\''||i.iusd||'\');\">' || CASE WHEN t.tpejustificativaformadories IS NULL THEN '' ELSE ' <img src=\"../imagens/valida2.gif\" border=\"0\" style=\"cursor:pointer;\" onclick=\"jAlert(\''||t.tpejustificativaformadories||'\', \'Justificativa\');\">' END||'</center>'":"'&nbsp;'")." as acao, '<center>'||replace(to_char(iuscpf::numeric, '000:000:000-00'), ':', '.')||'</center>', iusnome, iusemailprincipal, p.pfldsc, to_char(t.tpeatuacaoinicio,'mm/YYYY')||' a '||to_char(t.tpeatuacaofim,'mm/YYYY') as periodo 
			FROM sismedio.identificacaousuario i
			LEFT JOIN sismedio.portarianomeacao po ON po.iusd = i.iusd 
			LEFT JOIN public.arquivo ar ON ar.arqid = po.arqid
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN sismedio.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			WHERE t.pflcod IN('".PFL_FORMADORREGIONAL."','".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."') AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome)";
	
	$equiperh = $db->carregar($sql);
	
	
	$cabecalho = array("&nbsp;","CPF","Nome","E-mail","Perfil","Período de atuação");
	$db->monta_lista_simples($equiperh,$cabecalho,1000,5,'N','100%','N');
	
	echo "<br>";
	echo "<p><b>Quantitativo por perfil</b></p>";
	
	$sql = "SELECT foo.perfil, count(distinct foo.iusd) as qtd FROM (
				(SELECT p.pfldsc as perfil, i.iusd    
				FROM sismedio.identificacaousuario i 
				LEFT JOIN sismedio.portarianomeacao po ON po.iusd = i.iusd 
				LEFT JOIN public.arquivo ar ON ar.arqid = po.arqid
				INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				LEFT JOIN sismedio.pagamentoperfil pp ON pp.pflcod = p.pflcod 
				WHERE t.pflcod IN('".PFL_COORDENADORIES."') AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A')
				UNION ALL (
				SELECT p.pfldsc as perfil, i.iusd 
				FROM sismedio.identificacaousuario i
				LEFT JOIN sismedio.portarianomeacao po ON po.iusd = i.iusd 
				LEFT JOIN public.arquivo ar ON ar.arqid = po.arqid
				INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				LEFT JOIN sismedio.pagamentoperfil pp ON pp.pflcod = p.pflcod 
				WHERE t.pflcod IN('".PFL_FORMADORREGIONAL."','".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."') AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome)
			) foo
			GROUP BY foo.perfil
			ORDER BY foo.perfil";
	
	$cabecalho = array("Perfil","Quantidade");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);

}

function numeroMaximoCoordenadorAjuntoIES($dados) {
	global $db;
	$sql = "SELECT m.estuf FROM sismedio.estruturacurso e 
	 		INNER JOIN sismedio.abrangencia a ON a.ecuid = e.ecuid 
	 		INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
	 		WHERE e.uncid='".$dados['uncid']."' 
	 		GROUP BY m.estuf";
	
	$maxCoordenadorAjunto = $db->carregarColuna($sql);
	return count($maxCoordenadorAjunto);
	
}

function numeroCoordenadorAdjuntoIES($dados) {
	global $db;
	$sql = "SELECT COUNT(*) FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
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
	$sql = "SELECT COUNT(*) FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
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
	$sql = "SELECT tpejustificativaformadories FROM sismedio.tipoperfil WHERE iusd='".$dados['iusd']."'";
	$tpejustificativaformadories = $db->pegaUm($sql);
	return $tpejustificativaformadories;
}


function inserirEquipeRecursosHumanos($dados) {
	global $db;
	
	if($dados['iusd']) {
		if($dados['pflcod']!=PFL_FORMADORIES) {
			$sql = "SELECT turid FROM sismedio.turmas WHERE iusd='".$dados['iusd']."'";
			$existe = $db->pegaUm($sql);
			if($existe) {
				$al = array("alert"=>"Não é possível trocar o perfil. Este usuário possui turma associada. Remova a turma para alterar o perfil","location"=>"sismedio.php?modulo=principal/universidade/inserirequipe&acao=A&iusd=".$dados['iusd']);
				alertlocation($al);
			}
		}
		
	}
	
	/*
	if($dados['pflcod']==PFL_COORDENADORADJUNTOIES) {
		if(!validarNumeroCoordenadorAdjuntoIES(array("uncid"=>$dados['uncid']))) {
			$al = array("alert"=>"Número de Coordenador Adjunto da IES está no máximo. Não é possível inserir o CPF","location"=>"sismedio.php?modulo=principal/universidade/inserirequipe&acao=A");
			alertlocation($al);
		}
	}
	*/
	
	$iusd = $db->pegaUm("SELECT iusd FROM sismedio.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$dados['iuscpf'])."'");
	
	if(!$iusd) {
		
		$sql = "INSERT INTO sismedio.identificacaousuario(
	            iuscpf, iusnome, iusemailprincipal, foeid, uncid, iusdatainclusao, iustiposupervisor, iustiposupervisories, iussupervisoranalista)
	    		VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf'])."', 
	    				'".$dados['iusnome']."', '".$dados['iusemailprincipal']."', 
	    				'".$dados['foeid']."', '".$dados['uncid']."', NOW(), ".(($dados['iustiposupervisor'])?"'".$dados['iustiposupervisor']."'":"NULL").", ".(($dados['iustiposupervisories'])?"'".$dados['iustiposupervisories']."'":"NULL").", ".(($dados['iussupervisoranalista'])?$dados['iussupervisoranalista']:"NULL").") RETURNING iusd;";
		
		
		$iusd = $db->pegaUm($sql);
		
		$sql = "INSERT INTO sismedio.tipoperfil(
            iusd, pflcod, tpestatus, tpeatuacaoinicio, tpeatuacaofim, tpejustificativaformadories)
    		VALUES ('".$iusd."', '".$dados['pflcod']."', 'A', '".$dados['tpeatuacaoinicio_ano']."-".$dados['tpeatuacaoinicio_mes']."-01', '".$dados['tpeatuacaofim_ano']."-".$dados['tpeatuacaofim_mes']."-01',".(($dados['tpejustificativaformadories'])?"'".$dados['tpejustificativaformadories']."'":"NULL").");";
		
		$db->executar($sql);
		
	} else {
		
		$sql = "SELECT 'Nome : '||i.iusnome||' Perfil : '||p.pfldsc||' Universidade: '||uu.unisigla||' - '||uu.uninome||' Escola: '||l.lemcodigoinep||' - '||l.lemnomeescola||'. CPF não pode ser cadastrado. É necessário remove-lo do perfil indicado.' as msg 
				FROM sismedio.identificacaousuario i 
				INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
				INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
				LEFT JOIN sismedio.universidadecadastro u ON u.uncid = i.uncid 
				LEFT JOIN sismedio.universidade uu ON uu.uniid = u.uniid 
				LEFT JOIN sismedio.listaescolasensinomedio l ON i.iuscodigoinep = l.lemcodigoinep::bigint 
				WHERE i.iusd='".$iusd."' AND t.pflcod!='".$dados['pflcod']."'";
		
		$msg = $db->pegaUm($sql);
		
		if($msg) {
			$al = array("alert"=>$msg,"location"=>"sismedio.php?modulo=principal/universidade/inserirequipe&acao=A");
			alertlocation($al);
		}
		
		$sql = "UPDATE sismedio.identificacaousuario SET iusemailprincipal='".$dados['iusemailprincipal']."', 
														 foeid='".$dados['foeid']."', 
														 uncid='".$dados['uncid']."',
														 iustiposupervisor=".(($dados['iustiposupervisor'])?"'".$dados['iustiposupervisor']."'":"NULL").",
														 iustiposupervisories=".(($dados['iustiposupervisories'])?"'".$dados['iustiposupervisories']."'":"NULL").",
														 iussupervisoranalista=".(($dados['iussupervisoranalista'])?$dados['iussupervisoranalista']:"NULL").",
														 iusstatus='A'
				WHERE iusd='".$iusd."'";
		
		$db->executar($sql);
		
		$sql = "SELECT tpeid FROM sismedio.tipoperfil WHERE iusd='".$iusd."'";
		$tpeid = $db->pegaUm($sql);
		
		if($tpeid) {
		
			$sql = "UPDATE sismedio.tipoperfil SET tpeatuacaoinicio='".$dados['tpeatuacaoinicio_ano']."-".$dados['tpeatuacaoinicio_mes']."-01', tpeatuacaofim='".$dados['tpeatuacaofim_ano']."-".$dados['tpeatuacaofim_mes']."-01' WHERE iusd='".$iusd."'";
			$db->executar($sql);
		
		} else {
			
			$sql = "INSERT INTO sismedio.tipoperfil(
            iusd, pflcod, tpestatus, tpeatuacaoinicio, tpeatuacaofim, tpejustificativaformadories)
    		VALUES ('".$iusd."', '".$dados['pflcod']."', 'A', '".$dados['tpeatuacaoinicio_ano']."-".$dados['tpeatuacaoinicio_mes']."-01', '".$dados['tpeatuacaofim_ano']."-".$dados['tpeatuacaofim_mes']."-01',".(($dados['tpejustificativaformadories'])?"'".$dados['tpejustificativaformadories']."'":"NULL").");";
			
			$db->executar($sql);
			
		}
		
	}
	
	// controlando telefones
	$db->executar("DELETE FROM sismedio.identificacaotelefone WHERE iusd='".$iusd."' AND itetipo='T'");
	
	$sql = "INSERT INTO sismedio.identificacaotelefone(
           	iusd, itedddtel, itenumtel, itetipo, itestatus)
   			VALUES ('".$iusd."','".$dados['itedddtel']['T']."', '".$dados['itenumtel']['T']."', 'T', 'A');";
		
	$db->executar($sql);

	
	$db->commit();
	
	$al = array("alert"=>"Equipe gravada com sucesso","javascript"=>"window.opener.carregarEquipeRecursosHumanos();window.close();");
	alertlocation($al);
	
}

function excluirEquipeRecursosHumanos($dados) {
	global $db;
	
	$sql = "DELETE FROM sismedio.tipoperfil WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	
	$sql = "DELETE FROM sismedio.identificacaotelefone WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);

	$db->commit();
	
	$sql = "SELECT turid FROM sismedio.turmas WHERE iusd='".$dados['iusd']."'";
	$turids = $db->carregarColuna($sql);
	
	if($turids) {
		foreach($turids as $turid) {
			excluirTurma(array("turid" => $turid));			
		}
	}
	
	$al = array("alert"=>"Equipe removida com sucesso","location"=>"sismedio.php?modulo=principal/universidade/universidade&acao=A&aba=recursos_humanos");
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
	$total_turmas = $db->pegaUm("SELECT COUNT(*) FROM sismedio.turmas WHERE uncid='".$dados['uncid']."' AND turstatus='A'");
	return $total_turmas;
}


function atualizarTurma($dados) {
	global $db;
	
	$sql = "UPDATE sismedio.turmas SET iusd='".$dados['iusd']."', turdesc='".$dados['turdesc']."', muncod='".$dados['muncod_endereco']."' WHERE turid='".$dados['turid']."'";
	
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
	
		$sql = "INSERT INTO sismedio.turmas(
		        uncid, iusd, turdesc, turstatus, muncod)
		    	VALUES ('".$dados['uncid']."', '".$dados['iusd']."', '".$dados['turdesc']."', 'A', '".$dados['muncod_endereco']."');";
		
		$db->executar($sql);
		$db->commit();
		
		echo "Turma inserida com sucesso";
	
	} else {
		
		echo "Não foi possível cadastrar a turma. Número de turmas está no limite.";
		
	}
}

function carregarTurmas($dados) {
	global $db;
	$sql = "SELECT 
				'<img src=\"../imagens/mais.gif\" title=\"mais\" id=\"btn_turma_'||t.turid||'\" style=\"cursor:pointer;\" onclick=\"abrirTurma('||t.turid||',this)\"> ".((!$dados['consulta'])?"<img src=../imagens/salvar.png style=\"cursor:pointer;\" onclick=\"comporTurma(\''||turid||'\')\">":"")."' as acao,
				'<span style=font-size:xx-small;>'||t.turdesc||'</span>' as turdesc,
				'<center>'||i.iuscpf||'</center>' as iuscpf,
				'<span style=font-size:xx-small;>'||i.iusnome||'</span>' as iusnome,
				i.iusemailprincipal,
				(SELECT '(' || itedddtel || ') '|| itenumtel FROM sismedio.identificacaotelefone WHERE iusd=t.iusd AND itetipo='T') as telefone,
				(SELECT COUNT(*) FROM sismedio.orientadorturma ot INNER JOIN sismedio.tipoperfil tt ON tt.iusd = ot.iusd WHERE turid=t.turid) as nalunos
			FROM sismedio.turmas t 
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = t.iusd 
			INNER JOIN sismedio.tipoperfil tp ON tp.iusd = i.iusd 
			WHERE t.uncid='".$dados['uncid']."' AND tp.pflcod='".$dados['pflcod']."' ".(($dados['r_turma'])?"AND t.iusd IN('".implode("','",$dados['r_turma'])."')":"")." ORDER BY t.turdesc";
	
	if($dados['formacaoinicial']) {
		echo "<p><b>Marcar Todos : </b><input type=\"radio\" name=\"marcartodos\" onclick=\"marcarTodos(this);\" value=\"TRUE\"> Presente <input type=\"radio\" name=\"marcartodos\" onclick=\"marcarTodos(this);\" value=\"FALSE\"> Ausente</p>";
	}

	
	$cabecalho = array("&nbsp;","Turma","CPF","Nome","Email","Telefone","Qtd de participantes na turma");
	
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
}

function validarFormadoresTurmas($dados) {
	global $db;
	$sql = "SELECT 'Formador ('||i.iusnome||') não foi vinculado a nenhuma turma.' as t FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_FORMADORIES."
			LEFT JOIN sismedio.turmas tt ON tt.iusd = i.iusd 
			WHERE i.uncid='".$dados['uncid']."' AND tt.turid IS NULL";
	$mensagens = $db->carregarColuna($sql);
	
	return $mensagens;
	
}


function inserirAlunoTurma($dados) {
	global $db;
	
	$otuid = $db->pegaUm("SELECT otuid FROM sismedio.orientadorturma WHERE iusd='".$dados['iusd']."'");
	
	if($otuid) {
		
		$sql = "UPDATE sismedio.orientadorturma SET turid='".$dados['turid']."' WHERE otuid='".$otuid."'";
		
	} else {
		
		$sql = "INSERT INTO sismedio.orientadorturma(turid, iusd, otustatus)
	    		VALUES ('".$dados['turid']."', '".$dados['iusd']."', 'A');";

	}
	
	$db->executar($sql);
	$db->commit();
}

function excluirAlunoTurma($dados) {
	global $db;
	if($dados['iusd']) $wh = "iusd='".$dados['iusd']."'";
	elseif($dados['turid']) $wh = "turid='".$dados['turid']."'";
	
	$sql = "DELETE FROM sismedio.orientadorturma WHERE ".$wh;
	$db->executar($sql);
	$db->commit();
}

function excluirTurma($dados) {
	global $db;
	
	excluirAlunoTurma($dados);
	
	$sql = "DELETE FROM sismedio.turmas WHERE turid='".$dados['turid']."'";
	$db->executar($sql);
	$db->commit();
	
}

function carregarFiltrosEscolas($dados) {
	global $db;
	if($dados['estuf']) {
		
		$sql = "SELECT lemcodigoinep as codigo, lemuf||' / '||lemmundsc||' - '||lemnomeescola as descricao 
				FROM sismedio.listaescolasensinomedio WHERE lemuf='".$dados['estuf']."' AND lemcodigoinep NOT IN(SELECT lemcodigoinep FROM sismedio.abrangencia) 
				ORDER BY lemmundsc ASC";
	} else {
		$sql = "SELECT lemcodigoinep as codigo, lemcodigoinep as descricao FROM sismedio.abrangencia WHERE 1=2";
	}
	
	$_SESSION['indice_sessao_combo_popup']['lemcodigoinep_abrangencia']['sql'] = $sql;
}

function verificarCoordenadorIESTermoCompromisso($dados) {
	global $db;
	// verificando se coordenador local aceitou o termo de compromisso
	$coordies = carregarDadosIdentificacaoUsuario(array("uncid"=>$dados['uncid'],"pflcod"=>PFL_COORDENADORIES));
	
	if($coordies) {
		$coordies = current($coordies);
	}
	
	if($coordies['iustermocompromisso']!="t") {
		$al = array("alert"=>"Por favor preencha todos os campos obrigatórios da tela “Dados Coordenador IES”.","location"=>"sismedio.php?modulo=principal/universidade/universidade&acao=A&aba=dados");
		alertlocation($al);
	}
}

function numeroAlunosTurma($dados) {
	global $db;
	$sql = "SELECT COUNT(*) FROM sismedio.orientadorturma WHERE turid='".$dados['turid']."'";
	$numturmas = $db->pegaUm($sql);
	
	echo $numturmas;
	
}

function validarEnvioAnaliseMEC() {
	global $db;
	
	if($db->testa_superuser()) return true;
	
	$sql = "SELECT count(*) as n FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			WHERE t.pflcod='".PFL_SUPERVISORIES."' AND i.iusstatus='A' AND iustiposupervisor IS NULL AND i.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'";
	
	$num = $db->pegaUm($sql);
	
	if($num) {
		$erro .= 'Existem supervisores que não tiveram seu tipo definido. Clique em editar e atualize esses supervisores<br>\n'; 
	}
	
	$sql = "SELECT count(*) as n FROM sismedio.abrangencia a 
			INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
			WHERE e.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'";
	
	$numescolas = $db->pegaUm($sql);
	
	if(!$numescolas) {
		$erro .= 'Não foi selecionado nenhuma escola na abrangência da universidade. Clique em Estrutura da Formação e atualize os dados<br>\n';
	}
	
	$sql = "SELECT count(*) as n FROM sismedio.abrangencia a
			INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
			INNER JOIN sismedio.listaescolasensinomedio l ON l.lemcodigoinep = a.lemcodigoinep 
			INNER JOIN workflow.documento d ON d.docid = l.docid 
			WHERE e.uncid='".$_SESSION['sismedio']['universidade']['uncid']."' AND d.esdid IN('".ESD_ESCOLA_VALIDADO."','".ESD_ESCOLA_SEM_INTERESSE."')";
	
	$numescolas_emanalise = $db->pegaUm($sql);
	
	if($numescolas!=$numescolas_emanalise) {
		$erro .= 'Existem escolas que não tiveram o cadastramento validado pelo Supervisor. Aguarde a validação do cadastramento dessas escolas.<br>\n';
	}
	
	/*
	$mensagens = validarFormadoresTurmas(array("uncid" => $_SESSION['sismedio']['universidade']['uncid']));
	
	if($mensagens) $erro .= implode('<br>\n',$mensagens).'<br><br>\n\n Há '.count($mensagens).' Orientadores de Estudo que não foram vinculados a nenhuma turma. Retorne para a tela Turmas e vincule todos os nomes.<br><br>\n\n';
	
	$pp = carregarNumeroOrientadoresPendencia(array("uncid" => $_SESSION['sismedio']['universidade']['uncid']));
	if($pp['nummunpendencias']>0) $erro .= 'Há '.$pp['nummunpendencias'].' municípios que não concluíram o cadastramento dos seus Orientadores de Estudo<br>\n';
	if($pp['numestpendencias']>0) $erro .= 'Há '.$pp['numestpendencias'].' estados que não concluíram o cadastramento dos seus Orientadores de Estudo<br>\n';
	
	$numeroMaximoTurmas = numeroMaximoTurmas(array("uncid" => $_SESSION['sismedio']['universidade']['uncid']));
	$total_turmas = numeroTurmas(array("uncid" => $_SESSION['sismedio']['universidade']['uncid']));
	if($total_turmas>$numeroMaximoTurmas) {
		$erro .= '<br>\nNúmero máximo de turmas (na tela Turmas) deve ser igual ao Número máximo de Formadores (na tela Equipe IES) + Formadores Justificados<br><br>\n\n';
	}
	
	$tso = validarTurmasSemOrientadores(array("uncid" => $_SESSION['sismedio']['universidade']['uncid']));
	
	if($tso) {
		$erro .= '<br>\nExistem '.count($tso).' turma(s) sem orientadores cadastrados.';
	}
	
	*/
	
	if($erro) return $erro;
	else return true;
	

}

function validarTurmasSemOrientadores($dados) {
	global $db;
	
	$sql = "SELECT foo.turdesc FROM (
			SELECT t.turdesc, (SELECT COUNT(*) FROM sismedio.orientadorturma WHERE turid=t.turid) as nalunos FROM sismedio.turmas t 
			WHERE t.uncid='".$dados['uncid']."') foo WHERE foo.nalunos=0";
	
	$turmassemorientadores = $db->carregar($sql);
	
	return $turmassemorientadores;
	
}

function validarOrientadoresCadastradosTurma($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*)			
			FROM sismedio.turmas t 
			INNER JOIN sismedio.orientadorturma ot ON ot.turid = t.turid
			INNER JOIN sismedio.tipoperfil tt ON tt.iusd = ot.iusd and tt.pflcod=".PFL_ORIENTADORESTUDO." 
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = tt.iusd 
			WHERE t.uncid='".$dados['uncid']."' AND i.iusstatus='A'";
	
	$numOrientadoresTurmas = $db->pegaUm($sql);
	
	$sql = "SELECT SUM(foo.t) FROM (
			(SELECT COUNT(*) as t  FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.pactoidadecerta p ON p.picid = i.picid 
			LEFT JOIN territorios.municipio m ON m.muncod = p.muncod 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sismedio.abrangencia a ON p.muncod=a.muncod 
			INNER JOIN sismedio.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid='".$dados['uncid']."' AND i.uncid='".$dados['uncid']."' AND a.esfera='M' AND i.iusstatus='A') 
			UNION ALL (
			SELECT COUNT(*) as t FROM sismedio.identificacaousuario i 
			INNER JOIN territorios.municipio m ON m.muncod = i.muncodatuacao
			INNER JOIN sismedio.pactoidadecerta p ON p.estuf = m.estuf AND p.picid = i.picid 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sismedio.abrangencia a ON m.muncod=a.muncod 
			INNER JOIN sismedio.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE t.pflcod=".PFL_ORIENTADORESTUDO." AND c.uncid='".$dados['uncid']."' AND i.uncid='".$dados['uncid']."' AND a.esfera='E' AND i.iusstatus='A')
			) foo";
	
	$numTotalOrientadores = $db->pegaUm($sql);
	
	if($numOrientadoresTurmas==$numTotalOrientadores) return true;
	else return ($numTotalOrientadores-$numOrientadoresTurmas);
	
}



function calculaPorcentagemTramitacaoUniversidade($dados) {
	global $db;
	
	
	$sql = "SELECT d.docid, d.esdid, to_char(h.htddata,'YYYY-mm-dd') as htddata 
			FROM sismedio.universidadecadastro u 
			INNER JOIN workflow.documento d ON d.docid = u.docid 
			LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
			WHERE uncid='".$_SESSION['sismedio']['universidade']['uncid']."'";
	
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
	
	$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sismedio']['universidade']['uncid']));
	
	gerenciarAtividadeUniversidade(array('aundatafim'=>$aundatafim,'aundatainicio'=>$aundatainicio,'aunsituacao'=>$aunsituacao,'ecuid'=>$ecuid,'suaid'=>$dados['suaid']));
	
}

function calculaPorcentagemFormacaoInicial($dados) {
	global $db;
	
	
	$sql = "SELECT d.docid, d.esdid, to_char(h.htddata,'YYYY-mm-dd') as htddata 
			FROM sismedio.universidadecadastro u 
			INNER JOIN workflow.documento d ON d.docid = u.docidformacaoinicial 
			LEFT JOIN workflow.historicodocumento h ON h.hstid = d.hstid 
			WHERE uncid='".$_SESSION['sismedio']['universidade']['uncid']."'";
	
	$documento = $db->pegaLinha($sql);
	
	$sql = "SELECT to_char(htddata,'YYYY-mm-dd') as htddata FROM workflow.historicodocumento WHERE docid='".$documento['docid']."' ORDER BY htddata ASC LIMIT 1";
	$aundatainicio = $db->pegaUm($sql);
	
	if($documento['esdid']==ESD_FECHADO_FORMACAOINICIAL) {
		$aunsituacao = '100';
		$aundatafim = $documento['htddata'];;
	}
	
	$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sismedio']['universidade']['uncid']));
	
	gerenciarAtividadeUniversidade(array('aundatafim'=>$aundatafim,'aundatainicio'=>$aundatainicio,'aunsituacao'=>$aunsituacao,'ecuid'=>$ecuid,'suaid'=>$dados['suaid']));
	
}

function calculaPorcentagemDefinicaoEquipeIES($dados) {
	global $db;
	
	$sql = "SELECT to_char(MIN(iusdatainclusao),'YYYY-mm-dd') as inicio, to_char(MAX(iusdatainclusao),'YYYY-mm-dd') as fim, COUNT(*) as numequipe 
			FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE t.pflcod IN('".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."','".PFL_COORDENADORIES."') AND i.uncid='".$_SESSION['sismedio']['universidade']['uncid']."' AND i.iusstatus='A'";
	
	$info = $db->pegaLinha($sql);
	$aundatainicio = $info['inicio'];
	$aundatafim = $info['fim'];
	
	// CoordenadorIES + Coordenador Adjunto+ Supervisor IES + Formador IES
	$maximoFormadorIES = numeroMaximoTurmas(array("uncid"=>$_SESSION['sismedio']['universidade']['uncid']));
	$totalMaxEquipe = 1+numeroMaximoCoordenadorAjuntoIES(array("uncid" => $_SESSION['sismedio']['universidade']['uncid']))+1+$maximoFormadorIES;
	
	$aunsituacao = round(($info['numequipe']/$totalMaxEquipe)*100);
	
	$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sismedio']['universidade']['uncid']));
	
	gerenciarAtividadeUniversidade(array('aundatafim'=>$aundatafim,'aundatainicio'=>$aundatainicio,'aunsituacao'=>$aunsituacao,'ecuid'=>$ecuid,'suaid'=>$dados['suaid']));
}

function calculaPorcentagemFormacaoTurmas($dados) {
	global $db;
	
	$total   = totalAlfabetizadoresAbrangencia(array("uncid"=>$_SESSION['sismedio']['universidade']['uncid']));
	$nturmas = totalTurmasAbrangencia(array("total"=>$total));
	$formadoressolicitados = carregarDadosIdentificacaoUsuario(array("uncid"=>$_SESSION['sismedio']['universidade']['uncid'],"pflcod"=>PFL_FORMADORIES,"tpejustificativaformadories"=>true));
	
	$sql = "SELECT MAX(otudata) as fim, MIN(otudata) as inicio, COUNT(DISTINCT t.turid) as nturmas FROM sismedio.turmas t INNER JOIN sismedio.orientadorturma ot ON ot.turid = t.turid WHERE t.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'";
	$dturmas = $db->pegaLinha($sql);
	
	$aundatainicio = $dturmas['inicio'];
	$aundatafim = $dturmas['fim'];
	$numeroturmas = $dturmas['nturmas'];
	$maximoTurmas = $nturmas+count($formadoressolicitados);
	$aunsituacao = (($maximoTurmas)?round(($numeroturmas/$maximoTurmas)*100):0);
	
	
	
	$ecuid = pegarEstruturaCurso(array("uncid" => $_SESSION['sismedio']['universidade']['uncid']));
	
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
					(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_MEDIO.") as status,
					(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod = t.pflcod) as perfil,
					(SELECT usucpf FROM sismedio.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid=i.uncid AND rpustatus='A') as resp,
					CASE WHEN m.muncod IS NOT NULL THEN m.estuf||' / '||m.mundescricao 
					ELSE 'Equipe IES' END as rede
					
			FROM sismedio.identificacaousuario i
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			LEFT JOIN territorios.municipio m ON m.muncod = i.muncodatuacao 
			WHERE t.pflcod IN('".PFL_FORMADORREGIONAL."','".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."','".PFL_PROFESSORALFABETIZADOR."','".PFL_ORIENTADORESTUDO."','".PFL_COORDENADORPEDAGOGICO."') AND i.uncid='".$dados['uncid']."' AND i.iusstatus='A' ORDER BY p.pflcod, i.iusnome";
	
	return $sql;
}


function mostrarAbaFormacaoInicial($dados) {
	global $db;
	$estado = wf_pegarEstadoAtual( $_SESSION['sismedio']['universidade']['docid'] );
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
			$sql = "UPDATE sismedio.identificacaousuario SET iusformacaoinicialorientador=".$fi." WHERE iusd='".$iusd."'";
			$db->executar($sql);
		}
		$db->commit();
	}
	
	$al = array("alert"=>"Formação Inicial dos Orientadores foram salvas com sucesso. Não esqueça de preencher os alunos Presentes na Formação Inicial que não constam na lista, ao final do processo CLICAR no botão de Concluir os Registros de Frequência","location"=>"sismedio.php?modulo=principal/universidade/universidade&acao=A&aba=formacaoinicial");
	alertlocation($al);

}

function condicaoFormacaoInicial() {
	global $db;
	
	$sql = "SELECT COUNT(i.iusd) as total FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd 
			INNER join sismedio.pactoidadecerta p on i.picid = p.picid 
			INNER JOIN sismedio.abrangencia a ON p.muncod=a.muncod 
			INNER JOIN sismedio.estruturacurso c ON c.ecuid = a.ecuid 
			WHERE SUBSTR(i.iuscpf,1,3)!='SIS' AND i.uncid='".$_SESSION['sismedio']['universidade']['uncid']."' AND c.uncid='".$_SESSION['sismedio']['universidade']['uncid']."' AND t.pflcod='".PFL_ORIENTADORESTUDO."' AND i.iusstatus='A' AND iusformacaoinicialorientador IS NULL";
	
	$total_n_gravados = $db->pegaUm($sql);
	
	if($total_n_gravados) return "É necessário gravar os registros selecionando os orientadores que obtiveram a Formação Inicial";
	else return true;
	
}

function verificarValidacaoEstruturaFormacao($dados) {
	global $db;
	
	$sql = "SELECT COUNT(*) as total FROM sismedio.abrangencia a 
			INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
			WHERE e.uncid='".$dados['uncid']."'";
	
	$tot_abrangencia = $db->pegaUm($sql);
	
	$sql = "SELECT muncod FROM sismedio.estruturacurso e WHERE e.uncid='".$dados['uncid']."'";
	$existe_sede = $db->pegaUm($sql);
	
	if($tot_abrangencia == 0 || !$existe_sede) {
		$al = array("alert"=>"É necessário cadastrar as informações na aba Estrutura da Formação","location"=>"sismedio.php?modulo=principal/universidade/universidade&acao=A&aba=estrutura_curso");
		alertlocation($al);
	}
	
}


function devolverProjetoUniversidade($uncid) {
	
	return true;
	
	//global $aed_devolver;
	/*
	$aed_devolver=true;
	return aprovarProjetoUniversidade($uncid);
	*/
}

function aprovarProjetoUniversidade($uncid) {
	global $db, $aed_devolver;
	
	/*
	if($aed_devolver) $aedid=AED_REPROVAR_CADASTRO_ORIENTADORES;
	else $aedid=AED_APROVAR_CADASTRO_ORIENTADORES;
	
	$sql = "SELECT DISTINCT pp.docid
			FROM sismedio.abrangencia a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid 
			LEFT JOIN sismedio.pactoidadecerta pp ON pp.muncod = m.muncod 
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
			FROM sismedio.abrangencia a 
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
			INNER JOIN sismedio.estruturacurso e ON e.ecuid = a.ecuid
			LEFT JOIN sismedio.pactoidadecerta pp ON pp.estuf = m.estuf 
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
		
		$sql = "UPDATE sismedio.identificacaousuario id SET uncid=foo.uncid FROM (
				SELECT 
				u.uncid,
				i.iusd
				FROM sismedio.universidadecadastro u 
				INNER JOIN sismedio.estruturacurso e ON e.uncid = u.uncid 
				INNER JOIN sismedio.abrangencia a ON a.ecuid = e.ecuid 
				INNER JOIN sismedio.pactoidadecerta p ON p.muncod = a.muncod 
				INNER JOIN sismedio.identificacaousuario i ON i.picid = p.picid 
				INNER JOIN sismedio.tipoperfil t ON i.iusd = t.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
				WHERE u.uncid='".$uncid."' AND esfera='M' ) foo WHERE foo.iusd = id.iusd";
		
		$db->executar($sql);
		
		$sql = "UPDATE sismedio.identificacaousuario id SET uncid=foo.uncid FROM (
				SELECT 
				distinct 
				u.uncid,
				i.iusd
				FROM sismedio.universidadecadastro u 
				INNER JOIN sismedio.estruturacurso e ON e.uncid = u.uncid 
				INNER JOIN sismedio.abrangencia a ON a.ecuid = e.ecuid 
				INNER JOIN territorios.municipio m ON m.muncod = a.muncod 
				INNER JOIN sismedio.pactoidadecerta p ON p.estuf = m.estuf 
				INNER JOIN sismedio.identificacaousuario i ON i.picid = p.picid 
				INNER JOIN sismedio.tipoperfil t ON i.iusd = t.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
				WHERE u.uncid='".$uncid."' AND esfera='E' ) foo WHERE foo.iusd = id.iusd";
		
		$db->executar($sql);
		
		$db->commit();
		
		gerarVersaoProjetoUniversidade(array('uncid'=>$uncid));
	}
	*/

	gerarVersaoProjetoUniversidade(array('uncid'=>$uncid));
	
	return true;
}

function carregarVersoes($dados) {
	global $db;
	$sql = "SELECT '<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"visualizarVersao(\''||v.vpnid||'\');\">' as acao,
				   u.usunome,
				   to_char(v.vpndata,'dd/mm/YYYY HH24:MI') as data
			FROM sismedio.versoesprojetouniversidade v 
			INNER JOIN seguranca.usuario u ON u.usucpf = v.usucpf
			WHERE uncid='".$dados['uncid']."' ORDER BY v.vpndata DESC";
	$cabecalho = array("&nbsp","Usuário que inseriu versão","Data da inserção");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'S','100%',$par2);
}

function visualizarVersao($dados) {
	global $db;
	$sql = "SELECT vpnhtml FROM sismedio.versoesprojetouniversidade WHERE vpnid='".$dados['vpnid']."'";
	$html = $db->pegaUm($sql);
	
	echo $html;
	
}

function carregarOuvintesFormacaoInicial($dados) {
	global $db;
	
	$sql = "SELECT '<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"inserirOuvinte(\''||f.fioid||'\');\" > <img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirOuvinte(\''||fioid||'\');\">' as acao, fiocpf, fionome, fioemail, m.estuf||' / '||m.mundescricao as municipio,
				   CASE WHEN f.fioesfera='M' THEN 'Municipal'
				   		WHEN f.fioesfera='E' THEN 'Estadual' END as esfera,
				   	i.iusnome as substituido 
			FROM sismedio.formacaoinicialouvintes f 
			LEFT JOIN territorios.municipio m ON m.muncod = f.muncod 
			LEFT JOIN sismedio.identificacaousuario i ON i.iusd = f.iusd
			WHERE f.uncid='".$dados['uncid']."' AND f.fiostatus='A'";
	
	$cabecalho = array("&nbsp;","CPF","Nome","Email","UF / Município","Esfera","Irá substituir");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%',$par2);
}

function carregarUsuariosHabiliatadosTroca($dados) {
	global $db;
	$wh[] = "i.uncid='".$dados['uncid']."'";
	$wh[] = "(i.iusformacaoinicialorientador=false OR i.iusformacaoinicialorientador IS NULL)";
	if($dados['esfera']=='M') $wh[] = "i.picid IN(SELECT picid FROM sismedio.pactoidadecerta WHERE muncod='".$dados['muncod']."')";
	elseif($dados['esfera']=='E') $wh[] = "i.picid IN(SELECT picid FROM sismedio.pactoidadecerta WHERE estuf='".$dados['estuf']."')";
	
	$sql = "SELECT i.iusd as codigo, i.iusnome as descricao FROM sismedio.identificacaousuario i 
			INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd AND t.pflcod='".PFL_ORIENTADORESTUDO."' 
			INNER JOIN sismedio.orientadorturma o ON o.iusd = i.iusd 
			WHERE ".(($wh)?implode(" AND ",$wh):"")." ORDER BY i.iusnome";
	
	$db->monta_combo('iusd', $sql, 'S', 'NÃO É SUBSTITUTO, SOMENTE OUVINTE', '', '', '', '', 'S', 'iusd', '', $dados['iusd']);
}


function atualizarOuvinte($dados) {
	global $db;
	$sql = "UPDATE sismedio.formacaoinicialouvintes SET  fiocpf='".str_replace(array(".","-"),array("",""),$dados['fiocpf'])."', 
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

	$sql = "INSERT INTO sismedio.formacaoinicialouvintes(
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
	$sql = "UPDATE sismedio.formacaoinicialouvintes SET fiostatus='I' WHERE fioid='".$dados['fioid']."'";
	
	$db->executar($sql);
	$db->commit();
	
}

function posDevolverCoordenadorIES() {
	global $db;
	
	if(!strstr($_SERVER['HTTP_HOST'],"simec-local")) {
	
		$sql = "SELECT c.cmddsc FROM workflow.documento d 
				INNER JOIN workflow.comentariodocumento c ON c.hstid = d.hstid 
				WHERE d.docid='".$_SESSION['sismedio']['universidade']['docid']."'";
		
		$cmddsc = $db->pegaUm($sql);
		
		$sql = "SELECT iusnome, iusemailprincipal FROM sismedio.identificacaousuario  
				WHERE iusd='".$_SESSION['sismedio']['universidade']['iusd']."'";
		
		$identificacaousuario = $db->pegaLinha($sql);
		
		require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
		require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= "SIMEC";
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= "SIMEC - SISMÉDIO - Devolução do Projeto para alterações";
		
		$mensagem->AddAddress( $identificacaousuario['iusemailprincipal'], $identificacaousuario['iusnome'] );
		$mensagem->AddAddress( $_SESSION['email_sistema'] );
		
			
		$mensagem->Body = "<p>Prezado(a) ".$identificacaousuario['iusnome']." (Coordenador IES)</p>
						   <p>Seu projeto no SISMÉDIO foi analisado e foi devolvido para alterações.</p>
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
				$sql = "UPDATE sismedio.atividadeuniversidade SET aundatainicio='".formata_data_sql($aundatainicio)."' WHERE suaid='".$suaid."' AND ecuid IN(SELECT ecuid FROM sismedio.estruturacurso WHERE uncid='".$_SESSION['sismedio']['universidade']['uncid']."')";
				$db->executar($sql);
			}		
		}
	}
	
	if($dados['aundatafim']) {
		foreach($dados['aundatafim'] as $suaid => $aundatafim) {
			if($aundatafim) {
				$sql = "UPDATE sismedio.atividadeuniversidade SET aundatafim='".formata_data_sql($aundatafim)."' WHERE suaid='".$suaid."' AND ecuid IN(SELECT ecuid FROM sismedio.estruturacurso WHERE uncid='".$_SESSION['sismedio']['universidade']['uncid']."')";
				$db->executar($sql);
			}		
		}
	}
	
	$db->commit();
	
	$al = array("alert"=>"Plano de atividade atualizado com sucesso","location"=>"sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=planoatividade");
	alertlocation($al);
	
}

function atualizarPlanoAtividadeExecucao($dados) {
	global $db;
	
	if($dados['aundatainicioprev']) {
		foreach($dados['aundatainicioprev'] as $suaid => $aundatainicioprev) {
			if($aundatainicioprev) {
				$sql = "UPDATE sismedio.atividadeuniversidade SET aundatainicioprev2='".formata_data_sql($aundatainicioprev)."' WHERE suaid='".$suaid."' AND ecuid IN(SELECT ecuid FROM sismedio.estruturacurso WHERE uncid='".$_SESSION['sismedio']['universidade']['uncid']."')";
				$db->executar($sql);
			}
		}
	}

	if($dados['aundatafimprev']) {
		foreach($dados['aundatafimprev'] as $suaid => $aundatafimprev) {
			if($aundatafimprev) {
				$sql = "UPDATE sismedio.atividadeuniversidade SET aundatafimprev2='".formata_data_sql($aundatafimprev)."' WHERE suaid='".$suaid."' AND ecuid IN(SELECT ecuid FROM sismedio.estruturacurso WHERE uncid='".$_SESSION['sismedio']['universidade']['uncid']."')";
				$db->executar($sql);
			}
		}
	}
	
	$sql = "UPDATE sismedio.estruturacurso SET ecuobsplanoatividadesexecucao=".(($dados['ecuobsplanoatividadesexecucao'])?"'".$dados['ecuobsplanoatividadesexecucao']."'":"NULL")." WHERE ecuid IN(SELECT ecuid FROM sismedio.estruturacurso WHERE uncid='".$_SESSION['sismedio']['universidade']['uncid']."')";
	$db->executar($sql);

	$db->commit();

	$al = array("alert"=>"Plano de atividade atualizado com sucesso","location"=>"sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=planoatividade");
	alertlocation($al);

}

function condicaoEnviarOrcamentoMec($uncid) {
	global $db;
	
	$sql = "SELECT ((coalesce(SUM(o.orcvlrexecutado),0) - coalesce(SUM(o.orcvlratualizado),0))) as preenchimento
			FROM sismedio.orcamento o 
			INNER JOIN sismedio.grupodespesa g ON g.gdeid = o.gdeid 
			WHERE o.uncid='".$uncid."' AND o.orcstatus='A'";
	
	$preenchimento = $db->pegaUm($sql);
	
	if(!$preenchimento) return "Preencha os dados referentes ao Valor Executado(R$) e Valor Atualizado(R$)";

	
	$sql = "SELECT ((SUM(o.orcvlrunitario)-coalesce(SUM(o.orcvlrexecutado),0) - coalesce(SUM(o.orcvlratualizado),0))) as diferenca
			FROM sismedio.orcamento o 
			INNER JOIN sismedio.grupodespesa g ON g.gdeid = o.gdeid 
			WHERE o.uncid='".$uncid."' AND o.orcstatus='A'";
	
	$diferenca = $db->pegaUm($sql);
	
	if($diferenca < 0) return "IES esta demandando mais recursos do que o valor total do projeto aprovado";
	return true;
	
}

function inserirDocumento($dados) {
	global $db;
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$campos = array("uncid" => "'".$dados['uncid']."'","usucpf" => $_SESSION['usucpf'],"douobservacoes" => "'".$dados['douobservacoes']."'");
	$file = new FilesSimec( "documentosuniversidade", $campos, "sismedio" );
	$file->setUpload( NULL, "arquivo" );
	
	$al = array("alert"    => "Documento inserido com sucesso",
				"location" => "sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=documentosuniversidade");
	alertlocation($al);
	
}

function excluirDocumentoUniversidade($dados) {
	global $db;
	
	$sql = "UPDATE sismedio.documentosuniversidade SET doustatus='I' WHERE douid='".$dados['douid']."'";
	$db->executar($sql);
	$db->commit();
	
	
	$al = array("alert"    => "Documento removido com sucesso",
			"location" => "sismedio.php?modulo=principal/universidade/universidadeexecucao&acao=A&aba=documentosuniversidade");
	alertlocation($al);
	
}

function esconderAbaPeriodo($dados) {
	global $db;
	
	$sql = "SELECT esdid FROM workflow.documento WHERE docid='".$_SESSION['sismedio']['universidade']['docid']."'";
	$esdid_unc = $db->pegaUm($sql);
	
	if($esdid_unc != ESD_VALIDADO_COORDENADOR_IES) {
		return false;	
	}
	
	return true;

	
}

function exibirCursistasRelatorioFinal($dados) {
	global $db;

	$sql = "SELECT foo.acao, SUM(foo.resultados) FROM (
			SELECT CASE WHEN c.cerfrequencia>=75 THEN '<font style=font-size:xx-small;color:blue;>Recomendado</font>'
						ELSE '<font style=font-size:xx-small;color:red;>Não recomendado</font>' END as acao,
					1 as resultados
			FROM sismedio.certificacao c
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = c.iusd
			WHERE ".(($dados['uncid']!='9999')?"i.uncid='".$dados['uncid']."' AND ":"")." c.pflcod='".$dados['pflcod']."'
			) foo
			GROUP BY foo.acao";

	echo '<table width=30% align=center>';
	echo '<tr><td>';
	$db->monta_lista_simples($sql,array(),10000000,10,'S','100%','center');
	echo '</td></tr>';
	echo '</table>';

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

			FROM sismedio.certificacao c
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = c.iusd

			WHERE ".(($dados['uncid']!='9999')?"i.uncid='".$dados['uncid']."' AND ":"")." c.pflcod='".$dados['pflcod']."'";



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

			FROM sismedio.certificacao c
			INNER JOIN sismedio.identificacaousuario i ON i.iusd = c.iusd

			WHERE ".(($dados['uncid']!='9999')?"i.uncid='".$dados['uncid']."' AND ":"")." c.pflcod='".$dados['pflcod']."'";


		$db->monta_lista_simples($sql,$l,10000000,10,'N','95%','center');

	}

}


?>