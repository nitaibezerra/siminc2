<?php

function reenviarSenha($post, $pflcod, $senha) {
    global $db;

    extract($post);

    $iuscpf = corrige_cpf($iuscpf);
    $ususenha = md5_encrypt_senha("simecdti", "");

    if ($senha == "S") {
        $msg = "Senha reiniciada com sucesso!";
    } else {
        $msg = "Usuário ativado com sucesso!";
    }

    $sql = "UPDATE seguranca.usuario SET suscod = 'A', usustatus = 'A', ususenha = '{$ususenha}' WHERE usucpf = '{$iuscpf}'";
    $db->executar($sql);

    $sql = "UPDATE seguranca.usuario_sistema SET suscod = 'A' WHERE usucpf = '{$iuscpf}' AND sisid = '" . SIS_SISFOR . "'";
    $db->executar($sql);
    $db->commit();
    
    $e_perfil = $db->pegaUm("SELECT count(*) as n FROM seguranca.perfilusuario WHERE usucpf = '{$iuscpf}' AND pflcod = '".$pflcod."'");
    
    if(!$e_perfil) {
    	
    	$sql = "INSERT INTO seguranca.perfilusuario(
			            usucpf, pflcod)
			    VALUES ('{$iuscpf}', '{$pflcod}');";
    	
    	$db->executar($sql);
    	$db->commit();
    	
    }

    $arrUsu = $db->pegaLinha("SELECT usunome, usuemail FROM seguranca.usuario WHERE usucpf = '{$iuscpf}'");

    $remetente = array("nome" => "SIMEC - MÓDULO SISFOR", "email" => $arrUsu['usuemail']);
    $destinatario = $arrUsu['usuemail'];
    $usunome = $arrUsu['usunome'];

    $remetente = array("nome" => "SIMEC - MÓDULO SISFOR", "email" => $iusemailprincipal);
    $destinatario = $iusemailprincipal;
    $usunome = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf = '{$iuscpf}'");
    $assunto = "Cadastro no SIMEC - MÓDULO SISFOR";
    $conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
    $conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo SISFOR. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
 							  <p>Se for o seu primeiro acesso, o sistema solicitará que você crie uma nova senha. Se você já tiver cadastro no SIMEC, insira o seu CPF e senha. Caso tenha esquecido a sua senha de acesso ao SIMEC, clique em \"Esqueceu a senha?\" e insira o seu CPF. O sistema enviará a sua nova senha para o e-mail que você cadastrou. Em caso de dúvida, entre em contato com a sua Secretaria de Educação.</p>
 							  <p>Sua Senha de acesso é: %s</p>
 							  <br><br>* Caso você já alterou a senha acima, favor desconsiderar este e-mail.", 'Prezado(a)', $usunome, "simecdti");

    if (!strstr($_SERVER['HTTP_HOST'], "simec-local")) {
        enviar_email($remetente, $destinatario, $assunto, $conteudo);
    }

    if ($pflcod == PFL_COORDENADOR_CURSO) {
		$al = array("alert" => $msg, "javascript" => "window.opener.location.reload();window.close();");
    	alertlocation($al);
    } else {
        $db->sucesso('principal/listauniversidade', '', $msg, 'S', 'S');
    }
}

function inserirCoordenador($post, $pflcod) {
    global $db;

    extract($post);

    $iuscpf = corrige_cpf($iuscpf);
    $unitpocod = selecionarUnidade($unicod);

    $sql = "SELECT	 		uni.uniabrev || '/' || uni.unidsc AS descricao, tpe.pflcod, sie.unicod
    		FROM 			sisfor.sisfories sie
    		INNER JOIN		sisfor.tipoperfil tpe ON tpe.tpeid = sie.tpeid
    		INNER JOIN		public.unidade uni ON uni.unicod = sie.unicod
    		WHERE 			sie.usucpf = '{$iuscpf}' AND tpe.pflcod = " . PFL_COORDENADOR_INST . "";

    $usuies = $db->pegaLinha($sql);

    $sql = "SELECT	 		uni.uniabrev || '/' || uni.unidsc AS descricao, tpe.pflcod, sif.unicod
	   		FROM 			sisfor.sisfor sif
	   		INNER JOIN		sisfor.tipoperfil tpe ON tpe.tpeid = sif.tpeid
	   		INNER JOIN		public.unidade uni ON uni.unicod = sif.unicod
	   		WHERE 			sif.usucpf = '{$iuscpf}' AND tpe.pflcod = " . PFL_COORDENADOR_CURSO . "";

    $usucurso = $db->pegaLinha($sql);

    if ((($usuies && $usuies['unicod'] <> $unicod) || ($usucurso && $usucurso['unicod'] <> $unicod))) {
        if ($ieoid) {
            $locationcurso = "sisfor.php?modulo=principal/coordenador_curso/gerenciarcoordenador_curso&acao=A&ieoid=" . $ieoid . "&unicod=" . $unicod;
            $locationies = "sisfor.php?modulo=principal/coordenador/gerenciarcoordenador_ies&acao=A&ieoid=" . $ieoid . "&unicod=" . $unicod;
        } else if ($cnvid) {
            $locationcurso = "sisfor.php?modulo=principal/coordenador_curso/gerenciarcoordenador_curso&acao=A&cnvid=" . $cnvid . "&unicod=" . $unicod;
            $locationies = "sisfor.php?modulo=principal/coordenador/gerenciarcoordenador_ies&acao=A&cnvid=" . $cnvid . "&unicod=" . $unicod;
        } else if ($ocuid) {
            $locationcurso = "sisfor.php?modulo=principal/coordenador_curso/gerenciarcoordenador_curso&acao=A&ocuid=" . $ocuid . "&unicod=" . $unicod;
            $locationies = "sisfor.php?modulo=principal/coordenador/gerenciarcoordenador_ies&acao=A&ocuid=" . $ocuid . "&unicod=" . $unicod;
        } else if ($oatid) {
            $locationcurso = "sisfor.php?modulo=principal/coordenador_curso/gerenciarcoordenador_curso&acao=A&oatid=" . $oatid . "&unicod=" . $unicod;
            $locationies = "sisfor.php?modulo=principal/coordenador/gerenciarcoordenador_ies&acao=A&oatid=" . $oatid . "&unicod=" . $unicod;
        } else if ($unicod) {
            $locationcurso = "sisfor.php?modulo=principal/coordenador_curso/gerenciarcoordenador_curso&acao=A&unicod=" . $unicod;
            $locationies = "sisfor.php?modulo=principal/coordenador/gerenciarcoordenador_ies&acao=A&unicod=" . $unicod;
        }

        if ($ieoid || $cnvid || $ocuid || $oatid) {
            if ($usucurso['pflcod'] == PFL_COORDENADOR_CURSO) {
                $al = array("alert" => "Este CPF é Coordenador Curso na universidade (" . $usucurso['descricao'] . ") e não pode ser cadastrado!", "location" => $locationcurso);
            } else {
                $al = array("alert" => "Este CPF é Coordenador Institucional da universidade (" . $usuies['descricao'] . ") e não pode ser cadastrado!", "location" => $locationcurso);
            }
            alertlocation($al);
        } else {
            if ($usucurso['pflcod'] == PFL_COORDENADOR_CURSO) {
                $al = array("alert" => "Este CPF é Coordenador Curso na universidade (" . $usucurso['descricao'] . ") e não pode ser cadastrado!", "location" => $locationies);
            } else {
                $al = array("alert" => "Este CPF é Coordenador Institucional da universidade (" . $usuies['descricao'] . ") e não pode ser cadastrado!", "location" => $locationies);
            }
            alertlocation($al);
        }
    } else {
        $sql = "SELECT iusd FROM sisfor.identificacaousuario WHERE iuscpf = '{$iuscpf}'";
        $iusd = $db->pegaUm($sql);

        if ($iusd) {
            $sql = "UPDATE 		sisfor.identificacaousuario
	 				SET 		iusstatus = 'A', 
	 							iusemailprincipal = '{$iusemailprincipal}' 
	 				WHERE 		iusd = {$iusd}";
            $db->executar($sql);
        } else {
            $sql = "INSERT INTO sisfor.identificacaousuario (iuscpf,iusnome, iusemailprincipal,iusdatainclusao,iusstatus)
	 			    VALUES ('{$iuscpf}', '{$iusnome}','{$iusemailprincipal}',NOW(),'A') RETURNING iusd;";
            $iusd = $db->pegaUm($sql);
        }

        $existe_usu = $db->pegaUm("SELECT usucpf FROM seguranca.usuario WHERE usucpf = '{$iuscpf}'");

        if (!$existe_usu) {
            $ususenha = md5_encrypt_senha("simecdti", '');
            $sql = "INSERT INTO seguranca.usuario (usucpf, usunome, usuemail, usustatus, ususenha, suscod)
	     			VALUES 	   ('{$iuscpf}','{$iusnome}','{$iusemailprincipal}','A','{$ususenha}','A')";
            $db->executar($sql);
        } else {
            $sql = "UPDATE 		seguranca.usuario 
					SET 		usucpf = '{$iuscpf}', 
								usunome = '{$iusnome}', 
								usuemail = '{$iusemailprincipal}', 
								usustatus = 'A',
								suscod = 'A'
					WHERE		usucpf = '{$iuscpf}'";
            $db->executar($sql);
        }

        $existe_sis = $db->pegaUm("SELECT usucpf FROM seguranca.usuario_sistema WHERE usucpf = '{$iuscpf}' AND sisid='" . SIS_SISFOR . "'");

        if (!$existe_sis) {
            $sql = "INSERT INTO seguranca.usuario_sistema (usucpf, sisid, susstatus,pflcod,susdataultacesso,suscod)
	     			VALUES 	  ('{$iuscpf}'," . SIS_SISFOR . ", 'A', NULL, NOW(),'A');";

            $db->executar($sql);
        } else {
            if ($dados['suscod'] == "A") {
                $sql = "UPDATE 		seguranca.usuario_sistema
	 	    			SET 		suscod = 'A' 
	 	    			WHERE 		usucpf = '{$iuscpf}' 
	 	    			AND 		sisid = '" . SIS_SISFOR . "'";
                $db->executar($sql);
            }
        }

        $existe_pfl = $db->pegaUm("SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf = '{$iuscpf}' AND pflcod = '{$pflcod}'");

        if (!$existe_pfl) {
            $sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('{$iuscpf}', '{$pflcod}');";
            $db->executar($sql);
        }

        if ($pflcod == PFL_COORDENADOR_INST) {
            $tipo = "'inserirCoordenadorIES'";

            $sieid = $db->pegaUm("SELECT sieid FROM sisfor.sisfories WHERE unicod = '{$unicod}'");

            $sql = "INSERT INTO sisfor.tipoperfil (iusd, pflcod,tpestatus,tpeatuacaoinicio)
	 		    	VALUES 		  ({$iusd},'{$pflcod}','A',now()) RETURNING tpeid";
            $tpeid = $db->pegaUm($sql);

            if ($sieid) {
                $sql = "UPDATE sisfor.sisfories SET usucpf = '{$iuscpf}', tpeid = {$tpeid} WHERE sieid = {$sieid}";
                $db->executar($sql);
            } else {
                $sql = "INSERT INTO sisfor.sisfories (unicod, unitpocod, siestatus, usucpf, tpeid) VALUES ({$unicod}, '{$unitpocod}', 'A', '{$iuscpf}', {$tpeid})";
                $db->executar($sql);
            }
        } else {
            $tipo = "'inserirCoordenadorCurso'";

            $aryWhere[] = "sifstatus = 'A'";

            $values = "";
            $registro = "";

            if ($ieoid) {
                $aryWhere[] = "ieoid = {$ieoid}";
                $values = ", ieoid";
                $registro = "," . $ieoid;
                $registro = ",'{$ieoid}'";
            }

            if ($cnvid) {
                $aryWhere[] = "cnvid = {$cnvid}";
                $values = ", cnvid";
                $registro = ",'{$cnvid}'";
            }

            if ($ocuid) {
                $aryWhere[] = "ocuid = {$ocuid}";
                $values = ", ocuid";
                $registro = ",'{$ocuid}'";
            }

            if ($oatid) {
                $aryWhere[] = "oatid = {$oatid}";
                $values = ", oatid";
                $registro = ",'{$oatid}'";
            }

            $sql = "SELECT sifid FROM sisfor.sisfor " . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";
            $sifid = $db->pegaUm($sql);

            $sql = "INSERT INTO sisfor.tipoperfil (iusd, pflcod,tpestatus,tpeatuacaoinicio)
	 		    	VALUES 		  ({$iusd},{$pflcod},'A',now()) RETURNING tpeid";
            $tpeid = $db->pegaUm($sql);

            if ($sifid) {
                $sql = "UPDATE 	sisfor.sisfor SET usucpf = '{$iuscpf}', tpeid = {$tpeid} WHERE	sifid = {$sifid}";
                $db->executar($sql);
            } else {
                $sql = "INSERT INTO sisfor.sisfor (unicod, unitpocod, sifstatus, tpeid, usucpf $values)
		 		    	VALUES 	  ('{$unicod}','{$unitpocod}', 'A', {$tpeid}, '{$iuscpf}' $registro)";
                $db->executar($sql);
            }
        }

        $hiulog = str_replace(array("'"), array(""), simec_json_encode($post));

        $sql = "INSERT INTO sisfor.historicoidentificaousuario(
	            iusd, hiudatainc, hiucpf, hiulog, hiustatus, hiutipo)
	    		VALUES ({$iusd}, now(), '{$_SESSION['usucpf']}', '{$hiulog}', 'A', $tipo);";
        $db->executar($sql);

        $sql = "INSERT INTO sisfor.usuarioresponsabilidade (pflcod, usucpf, rpustatus, rpudata_inc, unicod, unitpocod)
 		    	VALUES 	  ('{$pflcod}', '{$iuscpf}', 'A', NOW(), '{$unicod}','{$unitpocod}')";
        $db->executar($sql);
        $db->commit();

        $remetente = array("nome" => "SIMEC - MÓDULO SISFOR", "email" => $iusemailprincipal);
        $destinatario = $iusemailprincipal;
        $usunome = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf = '{$iuscpf}'");
        $assunto = "Cadastro no SIMEC - MÓDULO SISFOR";
        $conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
        $conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo SISFOR. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
 							  <p>Se for o seu primeiro acesso, o sistema solicitará que você crie uma nova senha. Se você já tiver cadastro no SIMEC, insira o seu CPF e senha. Caso tenha esquecido a sua senha de acesso ao SIMEC, clique em \"Esqueceu a senha?\" e insira o seu CPF. O sistema enviará a sua nova senha para o e-mail que você cadastrou. Em caso de dúvida, entre em contato com a sua Secretaria de Educação.</p>
 							  <p>Sua Senha de acesso é: %s</p>
 							  <br><br>* Caso você já alterou a senha acima, favor desconsiderar este e-mail.", 'Prezado(a)', $usunome, "simecdti");

        if (!strstr($_SERVER['HTTP_HOST'], "simec-local")) {
            enviar_email($remetente, $destinatario, $assunto, $conteudo);
        }

        if ($ieoid || $cnvid || $ocuid || $oatid) {
        	if($siftipoplanejamento == FASE02){
            	$db->sucesso('principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2', '', 'Coordenador Curso inserido com sucesso!', 'S', 'S');
        	} elseif($siftipoplanejamento == FASE01) {
        		$db->sucesso('principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies', '', 'Coordenador Curso inserido com sucesso!', 'S', 'S');
        	} else {
        		$db->sucesso('principal/coordenador/coordenador_ies&acao=A&aba=programacao2015', '', 'Coordenador Curso inserido com sucesso!', 'S', 'S');
        	}
        } else {
            $db->sucesso('principal/listauniversidade&acao=A', '', 'Coordenador Institucional inserido com sucesso!', 'S', 'S');
        }
    }
}

function pesquisarUniversidade($perfil, $post = null) {
    global $db;

    if ($post) {
        extract($post);
    }

    $aryWhere[] = "ent.entstatus = 'A'";

    if (!empty($unicod)) {
        $aryWhere[] = "ent.entunicod = '{$unicod}'";
    }

    if (!empty($uniabrev)) {
        $uniabrev = utf8_decode($uniabrev);
        $aryWhere[] = "ent.entsig ILIKE '%{$uniabrev}%'";
    }

    if (!empty($iusnome)) {
        $iusnome = utf8_decode($iusnome);
        $aryWhere[] = "iud.iusnome ILIKE '%{$iusnome}%' AND tpe.pflcod = " . PFL_COORDENADOR_INST . "";
    }

    if (!empty($iuscpf)) {
        $iuscpf = corrige_cpf($iuscpf);
        $aryWhere[] = "iud.iuscpf = '{$iuscpf}' AND tpe.pflcod = " . PFL_COORDENADOR_INST . "";
    }

    if (in_array(PFL_COORDENADOR_INST, $perfil) && in_array(PFL_COORDENADOR_CURSO, $perfil)) {
        $aryWhere[] = "sie.usucpf = '{$_SESSION['usucpf']}'";
    } else {
        if (in_array(PFL_COORDENADOR_CURSO, $perfil)) {
            $aryWhere[] = "ent.entunicod = (SELECT DISTINCT unicod FROM sisfor.sisfor WHERE usucpf = '{$_SESSION['usucpf']}')";
        }

        if (in_array(PFL_COORDENADOR_INST, $perfil)) {
            $aryWhere[] = "sie.usucpf = '{$_SESSION['usucpf']}'";
        }
    }

   
    if (in_array(PFL_SUPER_USUARIO, $perfil) || in_array(PFL_ADMINISTRADOR, $perfil)) {
        $acao = "CASE 	WHEN (iud.iusd IS NULL) THEN '<img border=\"0\" title=\"COORDENADOR IES\" src=\"../imagens/usuario.gif\" id=\"'|| ent.entunicod ||'\" onclick=\"gerenciarCoordenadorIES('|| ent.entunicod ||');\" style=\"cursor:pointer;\"/>'
						WHEN (iud.iusd IS NOT NULL) OR (COALESCE(pla.planejamento,0) <> 0) THEN '<img border=\"0\" title=\"DADOS IES\" src=\"../imagens/alterar.gif\" id=\"'|| iud.iusd ||'\" onclick=\"alterarCoordenadorIES('|| iud.iusd ||');\" style=\"cursor:pointer;\"/> 
																								 <img border=\"0\" title=\"COORDENADOR IES\" src=\"../imagens/usuario.gif\" id=\"'|| ent.entunicod ||'\" onclick=\"gerenciarCoordenadorIES('|| ent.entunicod ||');\" style=\"cursor:pointer;\"/>' ELSE '' END AS acao,";
        $cabecalho = array('Ação', 'Código da Unidade Orçamentária', 'Sigla da Instituição', 'Unidade Orçamentária', 'CPF', 'Coordenador Institucional', 'Planejamento', 'Valor LOA', 'Valor Prev. Projetos Validados', 'Saldo Fase 2', 'Não aplicado Fase 2', 'Situação Fase 2' );
    } elseif (in_array(PFL_COORDENADOR_INST, $perfil) || in_array(PFL_EQUIPE_MEC, $perfil)) {
        $acao = "CASE 	WHEN (iud.iusd IS NULL) THEN '<img border=\"0\" title=\"COORDENADOR IES\" src=\"../imagens/usuario.gif\" id=\"'|| ent.entunicod ||'\" onclick=\"gerenciarCoordenadorIES('|| ent.entunicod ||');\" style=\"cursor:pointer;\"/>'
						WHEN (iud.iusd IS NOT NULL) OR (COALESCE(pla.planejamento,0) <> 0) THEN '<img border=\"0\" title=\"DADOS IES\" src=\"../imagens/alterar.gif\" id=\"'|| iud.iusd ||'\" onclick=\"alterarCoordenadorIES('|| iud.iusd ||');\" style=\"cursor:pointer;\"/> ' ELSE '' END AS acao,";
        $cabecalho = array('Ação', 'Código da Unidade Orçamentária', 'Sigla da Instituição', 'Unidade Orçamentária', 'CPF', 'Coordenador Institucional', 'Planejamento', 'Valor LOA', 'Valor Prev. Projetos Validados', 'Saldo Fase 2', 'Não aplicado Fase 2', 'Situação Fase 2' );
     } elseif (in_array(PFL_COORDENADOR_CURSO, $perfil) || in_array(PFL_CONSULTAGERAL, $perfil)) {
        $acao = "CASE WHEN (iud.iusd IS NOT NULL) OR (COALESCE(pla.planejamento,0) <> 0) THEN '<img border=\"0\" title=\"DADOS IES\" src=\"../imagens/alterar.gif\" id=\"'|| iud.iusd ||'\" onclick=\"alterarCoordenadorIES('|| iud.iusd ||');\" style=\"cursor:pointer;\"/> ' ELSE '' END AS acao,";
        $cabecalho = array('Ação', 'Código da Unidade Orçamentária', 'Sigla da Instituição', 'Unidade Orçamentária', 'CPF', 'Coordenador Institucional', 'Planejamento', 'Valor LOA', 'Valor Prev. Projetos Validados', 'Saldo Fase 2', 'Não aplicado Fase 2', 'Situação Fase 2' );
   }
    
    
    $sql = "
    		select acao, '<span style=font-size:x-small;>'||uo||'</span>' as uo, '<span style=font-size:x-small;>'||abreviacao||'</span>' as abreviacao, '<span style=font-size:x-small;>'||unidade||'</span>' as unidade, '<span style=font-size:x-small;>'||iuscpf||'</span>' as iuscpf, '<span style=font-size:x-small;>'||iusnome||'</span>' as iusnome, '<span style=font-size:x-small;>'||planejamento||'</span>' as planejamento, saldoloa, vlr_projeto,(saldoloa - vlr_projeto) as vlr_fase_2,
  						COALESCE( ( (saldoloa - vlr_projeto) - coalesce( vlr_comp , 0) ), 0.00) as saldo_disp, '<span style=font-size:x-small;>'||situacao_fase_2||'</span>' as situacao_fase_2  from (
    		SELECT 		
    		 distinct  $acao
                		ent.entunicod AS UO,
                		ent.entsig AS abreviacao,
            		    ent.entnome AS unidade,
            		    substr(iud.iuscpf,1,3) || '.' || substr(iud.iuscpf,4,3)|| '.' || substr(iud.iuscpf,7,3) || '-' || substr(iud.iuscpf,10,2) AS iuscpf,
            		    iud.iusnome, 				
            		    	
  						CASE WHEN est.esdid = " . WF_PLAN_ANALISE_MEC . " THEN 'Finalizado' 
  							 WHEN est.esdid = " . WF_PLAN_FECHADO . " THEN 'Fechado'
  							 ELSE CASE WHEN COALESCE(pla.planejamento,0.00) = 0 THEN 'Não Iniciado' ELSE 'Em elaboração' END END AS planejamento,
  							 		
  						( SELECT coalesce( SUM(VPPVALOR), 0.00) AS loa FROM sisfor.valorprevistoploa WHERE unicod = ent.entunicod ) as saldoloa,
  						
  						(  						
				  			SELECT 		COALESCE(SUM(orcvlrunitario),0.00) AS projeto
							FROM 		sisfor.orcamento o
							INNER JOIN 	sisfor.sisfor s  ON o.sifid = s.sifid
							INNER JOIN 	workflow.documento d on d.docid = s.docidprojeto
							INNER JOIN 	workflow.estadodocumento e on e.esdid = d.esdid		
							WHERE 		s.unicod =ent.entunicod  AND e.esdid = ".ESD_PROJETO_VALIDADO." and s.sifstatus = 'A' and o.orcstatus = 'A' and s.siftipoplanejamento  = 1
  						) as vlr_projeto,
  						(
  						 coalesce(( SELECT			sum( (COALESCE(sif.sifvalorloa,0.00) )) AS sifvalortotal
									FROM  			sisfor.sisfor sif
			
									LEFT JOIN       (SELECT t2.pflcod, i2.iuscpf, t2.sifid FROM sisfor.tipoperfil t2 INNER JOIN sisfor.identificacaousuario i2 ON t2.iusd = i2.iusd AND sifid is not null ) as adj ON adj.sifid = sif.sifid and adj.iuscpf=''
									LEFT JOIN		sisfor.identificacaousuario ius ON ius.iuscpf = sif.usucpf AND ius.iusstatus = 'A'
									LEFT JOIN		workflow.documento doc ON doc.docid = sif.docid
									LEFT JOIN		workflow.estadodocumento est ON est.esdid = doc.esdid
									LEFT JOIN 		sisfor.tipoperfil tpe ON tpe.tpeid = sif.tpeid AND tpe.pflcod = 1105
												
							 		WHERE (sif.cnvid IS NOT NULL OR sif.ocuid IS NOT NULL) AND sif.sifstatus = 'A' AND sif.siftipoplanejamento = 2 AND sif.unicod = ent.entunicod ) ,0.00 ) + 
							 coalesce(( SELECT 		
										sum( COALESCE(sifvalorloa,0.00)::numeric(20,2) ) AS sifvalorloa
										FROM       	 	sisfor.sisfor sif
										LEFT JOIN		sisfor.outraatividade oat ON sif.oatid = oat.oatid
										LEFT JOIN 		sisfor.tipoperfil tpe ON tpe.tpeid = sif.tpeid 
										LEFT JOIN		sisfor.identificacaousuario ius ON ius.iuscpf = sif.usucpf AND ius.iusstatus = 'A'
										LEFT JOIN		workflow.documento doc ON doc.docid = sif.docid
										LEFT JOIN		workflow.estadodocumento est ON est.esdid = doc.esdid
										LEFT JOIN 		catalogocurso2014.coordenacao coo ON coo.coordid = oat.coordid
														 WHERE sif.sifstatus = 'A'  and oat.oatstatus = 'A'  AND sif.unicod = ent.entunicod AND sif.siftipoplanejamento = 2 ) , 0.00)) as vlr_comp,
  						CASE WHEN est2.esdid is null then 'Não Iniciado'
									else
								est2.esddsc	
						end as situacao_fase_2
									
			FROM 		entidade.entidade ent
			INNER JOIN 	entidade.funcaoentidade fun ON fun.entid = ent.entid AND funid IN (12,11,44,102)
			LEFT JOIN	catalogocurso2014.iesofertante ieo ON ieo.unicod = ent.entunicod AND ieo.ieostatus = 'A'
			INNER JOIN	sisfor.sisfories sie ON sie.unicod = ent.entunicod
			LEFT JOIN 	sisfor.sisfories plan2 on plan2.unicod = ent.entunicod
			LEFT JOIN	workflow.documento doc2 ON doc2.docid = plan2.docidplan2
			LEFT JOIN	workflow.estadodocumento est2 ON est2.esdid = doc2.esdid
			LEFT JOIN 	sisfor.tipoperfil tpe ON tpe.tpeid = sie.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_INST . "
			LEFT JOIN	sisfor.identificacaousuario iud ON iud.iuscpf = sie.usucpf
			LEFT JOIN  (SELECT SUM(vppvalor) AS valseb, unicod  FROM sisfor.valorprevistoploa WHERE vppstatus = 'A' AND vppsecretaria = '1' GROUP BY unicod) AS seb ON seb.unicod = ent.entunicod		
			LEFT JOIN  (SELECT SUM(vppvalor) AS valsecadi, unicod  FROM sisfor.valorprevistoploa WHERE vppstatus = 'A' AND vppsecretaria = '2' GROUP BY unicod) AS sec ON sec.unicod = ent.entunicod		
			LEFT JOIN   (SELECT COUNT(sifid) AS planejamento, unicod FROM sisfor.sisfor GROUP BY unicod) AS pla ON pla.unicod = ent.entunicod
			LEFT JOIN	workflow.documento doc ON doc.docid = sie.docid
			LEFT JOIN	workflow.estadodocumento est ON est.esdid = doc.esdid
						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "
			 ORDER BY	ent.entunicod, ent.entnome ) as sub";
    
    $tamanho = array('5%', '4%', '5%', '20%', '8%', '15%', '6%', '6%','6%','6%', '6%','13%');
    $alinhamento = array('center', 'center', 'left', 'left', 'center', 'left', 'center', 'center', 'center', 'center', 'center', 'center', 'center');

    $db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '', $tamanho, $alinhamento, null,array('ordena'=>false));
}

function listarCursosFormacao($post) {
    global $db;

    extract($post);

    $aryWhere[] = "cufstatus = 'A'";

    if ($cufcodareageral) {
        $aryWhere[] = "cufcodareageral = {$cufcodareageral}";
    }

    $sql = "SELECT 		cufid AS codigo,
						cufcursodesc AS descricao 
			FROM 		sisfor.cursoformacao
						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "
			ORDER BY 	cufcursodesc";


    $db->monta_combo('cufid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'cufid', '');
}

function atualizarDadosIdentificacaoUsuario($post) {
    global $db;
    extract($post);

    $iusdatanascimento = formata_data_sql($iusdatanascimento);
    $iusagenciasugerida = substr($iusagenciasugerida, 0, 4);
    $iusagenciaend = substr($iusagenciaend, 0, 250);
    $iusemailopcional = ($iusemailopcional) ? "'{$iusemailopcional}'" : 'NULL';
    if($post['tpebolsa']) $iusnaodesejosubstituirbolsa = (in_array('iusnaodesejosubstituirbolsa',$post['tpebolsa'])) ? 'TRUE' : 'FALSE';
    else $iusnaodesejosubstituirbolsa = 'FALSE';

    $sql = "UPDATE 		sisfor.identificacaousuario
			SET			iusdatanascimento = '{$iusdatanascimento}',
						iusnomemae = '{$iusnomemae}',
						iussexo = '{$iussexo}',
						muncod = '{$muncod_nascimento}',
						eciid = '{$eciid}',
						nacid = '{$nacid}',
						iusnomeconjuge = '{$iusnomeconjuge}',
						iusagenciasugerida = '{$iusagenciasugerida}',
						iusagenciaend = '{$iusagenciaend}',
						tvpid = '{$tvpid}',
						funid = '{$funid}',
						foeid = '{$foeid}',
						iusemailprincipal = '{$iusemailprincipal}',
						iusemailopcional = {$iusemailopcional},
						iusnaodesejosubstituirbolsa = {$iusnaodesejosubstituirbolsa},
						iustermocompromisso = TRUE
			WHERE 		iusd = {$iusd}";

    $db->executar($sql);

    $erros = validarFormacao($post);

    if ($erros) {
        $al = array("alert" => "Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :" . '\n\n' . implode('\n', $erros), "location" => $location);
        alertlocation($al);
    }

    $iufid = $db->pegaUm("SELECT iufid FROM sisfor.identiusucursoformacao WHERE iusd = {$iusd}");

    //Controlando Formação
    if ($iufid) {
        $cufid = ($cufid) ? "'{$cufid}'" : 'NULL';
        $iufdatainiformacao = formata_data_sql($iufdatainiformacao);
        $iufdatafimformacao = ($iufdatafimformacao) ? "'" . formata_data_sql($iufdatafimformacao) . "'" : 'NULL';

        $sql = "UPDATE 	sisfor.identiusucursoformacao
				SET		cufid = $cufid, 
		            	iufdatainiformacao = '{$iufdatainiformacao}', 
		            	iufdatafimformacao = $iufdatafimformacao, 
		            	iufsituacaoformacao = '{$iufsituacaoformacao}'
		        WHERE 	iufid = {$iufid}";
        $db->executar($sql);
    } else {
        $cufid = ($cufid) ? "'{$cufid}'" : 'NULL';
        $iufdatainiformacao = formata_data_sql($iufdatainiformacao);
        $iufdatafimformacao = ($iufdatafimformacao) ? "'" . formata_data_sql($iufdatafimformacao) . "'" : 'NULL';

        $sql = "INSERT INTO sisfor.identiusucursoformacao(
		            		iusd, 
		            		cufid, 
		            		iufdatainiformacao, 
		            		iufdatafimformacao, 
		            		iufsituacaoformacao, 
		            		iufstatus)
		    	VALUES 	({$iusd}, 
		    	{$cufid},
		    			'{$iufdatainiformacao}', 
		    			$iufdatafimformacao,
		    			'{$iufsituacaoformacao}', 
		            	'A')";
        $db->executar($sql);
    }

    $erros = validarDocumento($post);

    if ($erros) {
        $al = array("alert" => "Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :" . '\n\n' . implode('\n', $erros), "location" => $location);
        alertlocation($al);
    }

    $itdid = $db->pegaUm("SELECT itdid FROM sisfor.identusutipodocumento WHERE iusd = {$iusd}");

    //Controlando Documento
    if ($itdid) {
        $itddataexp = formata_data_sql($itddataexp);

        $sql = "UPDATE 		sisfor.identusutipodocumento
				SET			tdoid = '{$tdoid}', 
							itdufdoc = '{$itdufdoc}', 
							itdnumdoc = '{$itdnumdoc}', 
							itddataexp = '{$itddataexp}', 
            				itdnoorgaoexp = '{$itdnoorgaoexp}'		
		        WHERE 		itdid = {$itdid}";

        $db->executar($sql);
    } else {
        $itddataexp = formata_data_sql($itddataexp);

        $sql = "INSERT INTO sisfor.identusutipodocumento(
            			iusd, 
            			tdoid, 
            			itdufdoc, 
            			itdnumdoc, 
            			itddataexp, 
            			itdnoorgaoexp, 
            			itdstatus)
    			VALUES ({$iusd}, 
    					'{$tdoid}', 
    					'{$itdufdoc}', 
    					'{$itdnumdoc}', 
    					'{$itddataexp}', 
    					'{$itdnoorgaoexp}', 
    					'A')";
        $db->executar($sql);
    }

    $erros = validarEndereco($post);

    if ($erros) {
        $al = array("alert" => "Não foi possível concluir o cadastro. Foram identificados ausência de informações no formulario, tente novamente mais tarde, e caso o erro persista, entre em contato com o responsável. As informações que faltam :" . '\n\n' . implode('\n', $erros), "location" => $location);
        alertlocation($al);
    }

    $ienid = $db->pegaUm("SELECT ienid FROM sisfor.identificaoendereco WHERE iusd = {$iusd}");

    //Controlando Endereço
    if ($ienid) {
        $muncod_endereco = substr($muncod_endereco, 0, 7);
        $iencep = str_replace(array("-"), array(""), $iencep);
        $iencomplemento = (($iencomplemento) ? "'{$iencomplemento}'" : 'NULL');
        $iennumero = ((!is_null($iennumero) && is_numeric($iennumero)) ? "'{$iennumero}'" : 'NULL');
        $ienbairro = substr($ienbairro, 0, 60);

        $sql = "UPDATE 		sisfor.identificaoendereco
				SET			muncod = '{$muncod_endereco}', 
							ientipo = '{$ientipo}', 
            				iencep = '{$iencep}', 
            				iencomplemento = $iencomplemento, 
            				iennumero = $iennumero, 
            				ienlogradouro = '{$ienlogradouro}', 
            				ienbairro = '".addslashes($ienbairro)."' 		
		        WHERE		ienid = {$ienid}";

        $db->executar($sql);
    } else {
        $muncod_endereco = substr($muncod_endereco, 0, 7);
        $iencep = str_replace(array("-"), array(""), $iencep);
        $iencomplemento = (($iencomplemento) ? "'{$iencomplemento}'" : 'NULL');
        $iennumero = ((!is_null($iennumero) && is_numeric($iennumero)) ? "'{$iennumero}'" : 'NULL');
        $ienbairro = substr($ienbairro, 0, 60);

        $sql = "INSERT INTO sisfor.identificaoendereco(
            			muncod, 
            			iusd, 
            			ientipo, 
            			iencep, 
            			iencomplemento, 
            			iennumero, 
            			iensatatus, 
            			ienlogradouro, 
            			ienbairro)
    			VALUES ('{$muncod_endereco}', 
    			{$iusd},
    					'{$ientipo}', 
    					'{$iencep}', 
    					$iencomplemento,
    					$iennumero,
    					'A', 
    					'{$ienlogradouro}', 
    					'".addslashes($ienbairro)."')";
        $db->executar($sql);
    }

    //Controlando Telefones
    $db->executar("DELETE FROM sisfor.identificacaotelefone WHERE iusd = {$iusd}");

    $tipos = array("R", "T", "C", "F");

    foreach ($tipos as $tipo) {
        $itedddtel[$tipo] = (($itedddtel[$tipo]) ? $itedddtel[$tipo] : 'NULL');
        $itenumtel[$tipo] = (($itenumtel[$tipo]) ? "'{$itenumtel[$tipo]}'" : 'NULL');

        $sql = "INSERT INTO sisfor.identificacaotelefone(
            			iusd,
            			itedddtel,
            			itenumtel,
            			itetipo,
            			itestatus)
    			VALUES ({$iusd},
    			{$itedddtel[$tipo]},
    			$itenumtel[$tipo],
    					'{$tipo}', 
    					'A')";
        $db->executar($sql);
    }

    $hiulog = str_replace(array("'"), array(""), simec_json_encode($post));

    $sql = "INSERT INTO sisfor.historicoidentificaousuario(
            iusd, hiudatainc, hiucpf, hiulog, hiustatus, hiutipo)
    		VALUES ({$iusd}, now(), '{$_SESSION['usucpf']}', '{$hiulog}', 'A', 'atualizarDadosIdentificacaoUsuario');";
    $db->executar($sql);

    $tpeatuacaoinicio = (($tpeatuacaoinicio_mes && $tpeatuacaoinicio_ano) ? "'" . $tpeatuacaoinicio_ano . "-" . $tpeatuacaoinicio_mes . "-01'" : 'NULL');
    $tpeatuacaofim = (($tpeatuacaofim_mes && $tpeatuacaofim_ano) ? "'" . $tpeatuacaofim_ano . "-" . $tpeatuacaofim_mes . "-01'" : 'NULL');

    $sql = "UPDATE 		sisfor.tipoperfil
			SET 		tpeatuacaoinicio = $tpeatuacaoinicio, 
						tpeatuacaofim = $tpeatuacaofim 
			WHERE 		iusd = {$iusd}";
    $db->executar($sql);
    
    $sql = "UPDATE sisfor.tipoperfil SET tpebolsa=FALSE WHERE iusd='".$iusd."'";
    $db->executar($sql);
    
    if($post['tpebolsa']) {
    	foreach($post['tpebolsa'] as $tpeid) {
    		if(is_numeric($tpeid)) {
    			$sql = "UPDATE sisfor.tipoperfil SET tpebolsa=TRUE WHERE tpeid='".$tpeid."'";
    			$db->executar($sql);
    		}
    	}
    }
    
    $db->commit();

    sincronizarUsuariosSIMEC(array('cpf' => $iuscpf));

    $al = array("alert" => $msgalert, "location" => $location);
    alertlocation($al);
}

function carregarMunicipiosPorUF($post) {
    global $db;

    extract($post);

    $sql = "SELECT 		muncod AS codigo,
						mundescricao AS descricao 
			FROM 		territorios.municipio 
			WHERE 		estuf = '{$estuf}' 
			ORDER BY 	mundescricao";

    $combo = $db->monta_combo($name, $sql, 'S', 'Selecione', (($onclick) ? $onclick : ''), '', '', '200', 'S', $id, true, $valuecombo);

    if ($returncombo) {
        return $combo;
    } else {
        echo $combo;
    }
}

function validarFormacao($dados) {
    extract($dados);

    if (!$iufdatainiformacao) {
        $erro[] = "Informe o 'Início - Formação'!";
    }
    if (!$iufsituacaoformacao) {
        $erro[] = "Informe a 'Situação - Formação'!";
    }
    return $erro;
}

function mascaraglobal($value, $mask) {
    $casasdec = explode(",", $mask);
    //Se possui casas decimais
    if ($casasdec[1]) {
        $value = sprintf("%01." . strlen($casasdec[1]) . "f", $value);
    }

    $value = str_replace(array("."), array(""), $value);
    if (strlen($mask) > 0) {
        $masklen = -1;
        $valuelen = -1;
        while ($masklen >= -strlen($mask)) {
            if (-strlen($value) <= $valuelen) {
                if (substr($mask, $masklen, 1) == "#") {
                    $valueformatado = trim(substr($value, $valuelen, 1)) . $valueformatado;
                    $valuelen--;
                } else {
                    if (trim(substr($value, $valuelen, 1)) != "") {
                        $valueformatado = trim(substr($mask, $masklen, 1)) . $valueformatado;
                    }
                }
            }
            $masklen--;
        }
    }
    return $valueformatado;
}

function carregarDadosIdentificacao($post = null, $pflcod) {
    global $db;

    if ($post) {
        extract($post);
    }

    $aryWhere[] = "i.iusstatus='A'";

    if ($pflcod) {
        $aryWhere[] = "t.pflcod = {$pflcod}";
    }
    if ($pflcod == PFL_TUTOR) {
        
        $join = " INNER JOIN 		sisfor.tipoperfil t ON i.iusd = t.iusd";

        
        if ($iusd) {
            $aryWhere[] = "i.iusd = {$iusd}";
        }
        
    } elseif ($pflcod == PFL_COORDENADOR_ADJUNTO_IES) {
        
        $join = " INNER JOIN 		sisfor.tipoperfil t ON i.iusd = t.iusd";
        
        if ($iusd) {
            $aryWhere[] = "i.iusd = {$iusd}";
        }
        
    }elseif ($pflcod == PFL_SUPERVISOR_IES) {
        
        $join = " INNER JOIN 		sisfor.tipoperfil t ON i.iusd = t.iusd";
        
        if ($iusd) {
            $aryWhere[] = "i.iusd = {$iusd}";
        }
        
    }
    elseif ($pflcod == PFL_FORMADOR_IES) {
        
        $join = " INNER JOIN 		sisfor.tipoperfil t ON i.iusd = t.iusd";
        
        if ($iusd) {
            $aryWhere[] = "i.iusd = {$iusd}";
        }
        
    }
    elseif ($pflcod == PFL_PROFESSOR_PESQUISADOR) {
        
        $join = " INNER JOIN 		sisfor.tipoperfil t ON i.iusd = t.iusd";
        
        if ($iusd) {
            $aryWhere[] = "i.iusd = {$iusd}";
        }
        
    }
    
    
    
    elseif ($pflcod == PFL_COORDENADOR_CURSO) {

        $join = " INNER JOIN 		sisfor.tipoperfil t ON i.iusd = t.iusd 
        		  INNER JOIN sisfor.sisfor s ON (t.sifid = s.sifid OR s.tpeid = t.tpeid) 
        		  LEFT JOIN catalogocurso2014.iesofertante ieo ON s.ieoid = ieo.ieoid";

        if ($sifid) {

            $aryWhere[] = "s.sifid = {$sifid}";
            
        } else {

            if ($curid) {
                $aryWhere[] = "ieo.curid = {$curid}";
            }

            if ($unicod) {
                $aryWhere[] = "s.unicod = '{$unicod}'";
            }

            if ($ieoid) {
                $aryWhere[] = "s.ieoid = {$ieoid}";
            }
        }
        
        if ($iusd) {
        	$aryWhere[] = "i.iusd = {$iusd}";
        }
        

        if ($cnvid) {
            $aryWhere[] = "s.cnvid = {$cnvid}";
        }

        if ($ocuid) {
            $aryWhere[] = "s.ocuid = {$ocuid}";
        }
        if ($oatid) {
            $aryWhere[] = "s.oatid = {$oatid}";
        }
    } else {

        $join = " INNER JOIN sisfor.sisfories s ON s.usucpf = i.iuscpf "
                . " INNER JOIN 		sisfor.tipoperfil t ON t.tpeid = s.tpeid";

        if ($iusd) {
            $aryWhere[] = "i.iusd = {$iusd}";
        }

        if ($unicod) {
            $aryWhere[] = "s.unicod = '{$unicod}'";
        }
    }

    $sql = "SELECT 			i.cadastradosgb,
							i.iusd, 
							i.iuscpf, 
							i.iusnome, 
							i.iusdatanascimento, 
							i.iusnomemae, 
							i.iustipoprofessor, 
							i.iusnaodesejosubstituirbolsa,
				   			i.iussexo, 
				   			i.eciid,
				   			i.nacid, 
				   			i.iusnomeconjuge, 
				   			i.iusagenciasugerida, 
				   			i.iusagenciaend, 
				   			i.iusformacaoinicialorientador,
				   			i.iusemailprincipal, 
				   			i.iusemailopcional, 
				   			i.iustipoorientador, 
				   			TO_CHAR(i.iusdatainclusao,'YYYY-mm-dd') AS iusdatainclusao, 
				   			i.iustermocompromisso,  
				   			i.tvpid, 
				   			i.funid, 
				   			i.foeid, 
				   			f.iufid, 
				   			f.cufid, 
				   			f.iufdatainiformacao, 
				   			f.iufdatafimformacao, 
				   			f.iufsituacaoformacao,
				   			m.estuf AS estuf_nascimento, 
				   			m.muncod AS muncod_nascimento, 
				   			ma.estuf ||' / '|| ma.mundescricao AS municipiodescricaoatuacao, 
				   			ma.muncod AS muncodatuacao, 
				   			d.itdid, 
				   			d.tdoid, 
				   			d.itdufdoc, 
				   			d.itdnumdoc, 
				   			d.itddataexp, 
				   			d.itdnoorgaoexp,
				   			e.ienid, 
				   			mm.muncod AS muncod_endereco, 
				   			mm.estuf AS estuf_endereco,
				   			e.ientipo, 
				   			e.iencep, 
				   			e.iencomplemento, 
				   			e.iennumero, 
				   			e.ienlogradouro, 
				   			e.ienbairro, 
				   			cf.cufcodareageral, 
				   			TO_CHAR(t.tpeatuacaoinicio,'YYYY-mm-dd') AS tpeatuacaoinicio, 
				   			TO_CHAR(t.tpeatuacaofim,'YYYY-mm-dd') AS tpeatuacaofim, 
				   			i.iusserieprofessor,
				   			t.tpeid,
				   			t.tpebolsa
			FROM 			sisfor.identificacaousuario i
							$join
			 			
			LEFT  JOIN 		territorios.municipio m ON m.muncod = i.muncod 
			LEFT  JOIN 		sisfor.identiusucursoformacao f ON f.iusd = i.iusd 
			LEFT  JOIN 		sisfor.identusutipodocumento d ON d.iusd = i.iusd 
			LEFT  JOIN 		sisfor.identificaoendereco e ON e.iusd = i.iusd
			LEFT  JOIN 		territorios.municipio mm ON mm.muncod = e.muncod
			LEFT  JOIN 		territorios.municipio ma ON ma.muncod = i.muncodatuacao
			LEFT  JOIN 		sisfor.cursoformacao cf ON cf.cufid = f.cufid 
							" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "
			ORDER BY 		i.iusd";

    $identificacaousuario = $db->carregar($sql);

    if ($identificacaousuario[0]) {
        foreach ($identificacaousuario as $key => $iu) {
            $idusuarios[$key] = $iu;
            unset($telefones);
            $sql = "SELECT itetipo, itedddtel, itenumtel FROM sisfor.identificacaotelefone WHERE iusd = '{$iu['iusd']}'";
            $tels = $db->carregar($sql);
            if ($tels[0]) {
                foreach ($tels as $tel) {
                    $telefones[$tel['itetipo']] = array("itedddtel" => $tel['itedddtel'], "itenumtel" => $tel['itenumtel']);
                }
                $idusuarios[$key]['telefones'] = $telefones;
            }
        }
    }
    return $idusuarios;
}

function listarAgencias($dados) {
    global $db;

    if ($dados['muncod']) {
        $codIbge = $dados['muncod'];
        $nuRaioKm = $db->pegaUm("SELECT munmedraio FROM territorios.municipio WHERE muncod='" . $dados['muncod'] . "'");
		$cliente = new SoapClient( "http://ws.mec.gov.br/AgenciasBb/wsdl",array(
																					'exceptions'	=> 0,
																					'trace'			=> true,
																					'encoding'		=> 'ISO-8859-1',
																					'cache_wsdl'    => WSDL_CACHE_NONE
		)) ;
        
        $xmlDeRespostaDoServidor = $cliente->getMunicipio($codIbge, $nuRaioKm);
        $agencias = new SimpleXMLElement($xmlDeRespostaDoServidor);
        if ($agencias->NODELIST) {
            foreach ($agencias->NODELIST as $agencia) {
                $agnum = (string) $agencia->co_agencia;
                $agcep = (string) $agencia->nu_cep_agencia;
                $agnom = (string) $agencia->no_agencia;
                $l_agencias[$agnum] = array("codigo" => $agnum . '_' . $agcep, "descricao" => $agnum . ' - ' . $agnom);
            }
            ksort($l_agencias);
            echo '<select id="dados_agencia" onchange="" style="width: auto" class="CampoEstilo obrigatorio" name="dados_agencia">';
            echo '<option value="">SELECIONE</option>';
            foreach ($l_agencias as $agencia) {
                echo '<option value="' . $agencia['codigo'] . '">' . utf8_encode($agencia['descricao'] . '') . '</option>';
            }
            echo '</select>';
        } else {
            echo "Não há agências do BB cadastradas no município escolhido. Escolha um município próximo.";
        }
    }
}

function pegarDadosUsuarioPorCPF($post) {
    global $db;

    extract($post);

    $sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '{$cpf}'";
    $usuemail = $db->pegaUm($sql);

    $sql = "SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf = '{$cpf}' AND sisid = '" . SIS_SISFOR . "'";
    $suscod = $db->pegaUm($sql);

    echo $usuemail . "||" . (($suscod) ? $suscod : 'NC');
}

function removerCoordenador($post, $pflcod) {
    global $db;

    extract($post);

    $iuscpf = corrige_cpf($iuscpf);
    
    $total = $db->pegaUm("SELECT count(*) as total FROM sisfor.pagamentobolsista WHERE tpeid='{$tpeid}'");
    
    if($total) {
    	$al = array("alert" => "Não é possível remover o Coordenador de Curso, pois este ja recebeu bolsa(s). Acesse Gerenciar Equipe e faça a substituição.", "javascript" => "window.close()");
    	alertlocation($al);
    }

    if ($pflcod == PFL_COORDENADOR_CURSO) {
        $sql = "UPDATE sisfor.sisfor SET usucpf = NULL, tpeid = NULL WHERE tpeid = {$tpeid}";
        $db->executar($sql);
        $msg = "Coordenador Curso removido com sucesso!";
        $url = "principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies";
        $tipo = "'removerCoordenadorCurso'";
    } else {
        $sql = "UPDATE sisfor.sisfories SET usucpf = NULL, tpeid = NULL WHERE usucpf = '{$iuscpf}' AND tpeid = {$tpeid}";
        $db->executar($sql);
        $msg = "Coordenador Institucional removido com sucesso!";
        $url = "principal/listauniversidade&acao=A";
        $tipo = "'removerCoordenadorIES'";
    }

    $hiulog = str_replace(array("'"), array(""), simec_json_encode($post));

    $sql = "INSERT INTO sisfor.historicoidentificaousuario(
            iusd, hiudatainc, hiucpf, hiulog, hiustatus, hiutipo)
    		VALUES ({$iusd}, now(), '{$_SESSION['usucpf']}', '{$hiulog}', 'A', $tipo);";
    $db->executar($sql);

    $sql = "SELECT COUNT(tpeid) AS perfil FROM sisfor.tipoperfil WHERE iusd = {$iusd}";
    $existe_perfil = $db->pegaUm($sql);

    if ($existe_perfil == 1) {
        $sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf = '{$iuscpf}' AND pflcod = {$pflcod}";
        $db->executar($sql);
    }
	
    $qtd_pagamentos = $db->pegaUm("SELECT COUNT(*) FROM sisfor.pagamentobolsista WHERE tpeid={$tpeid}");
    
    if(!$qtd_pagamentos) {
	    $sql = "DELETE FROM sisfor.tipoperfil WHERE tpeid = {$tpeid} AND pflcod = {$pflcod}";
	    $db->executar($sql);
    }

    if ($db->commit()) {
        $db->sucesso($url, '', $msg, 'S', 'S');
    }
}

function alertlocation($dados) {
    die("<script>
			" . (($dados['alert']) ? "alert('" . $dados['alert'] . "');" : "") . "
			" . (($dados['location']) ? "window.location='" . $dados['location'] . "';" : "") . "
			" . (($dados['javascript']) ? $dados['javascript'] : "") . "
		 </script>");
}

function atualizarMunicipio($post) {
    global $db;
    extract($post);

    $sql = "SELECT  	muncod AS codigo,
                    	mundescricao AS descricao 
           FROM 		territorios.municipio
           WHERE 		estuf = '{$estuf}' 
           ORDER BY 	mundescricao";

    $db->monta_combo('muncod', $sql, 'S', 'Selecione...', '', '', '', '250', 'S', 'muncod', '', $muncod);
    exit();
}

function pegarPerfil($usucpf) {
    global $db;

    $sql = "SELECT          pu.pflcod
            FROM            seguranca.perfilusuario pu 
            INNER JOIN      seguranca.perfil p ON p.pflcod = pu.pflcod
            AND             pu.usucpf = '{$usucpf}' 
            AND             p.sisid = {$_SESSION['sisid']}
            AND             pflstatus = 'A'";

    $arrPflcod = $db->carregar($sql);
    !$arrPflcod ? $arrPflcod = array() : $arrPflcod = $arrPflcod;
    $arrPerfil = array();
    foreach ($arrPflcod as $pflcod) {
        $arrPerfil[] = $pflcod['pflcod'];
    }
    return $arrPerfil;
}

function verificarEmailUnico($dados) {
    global $db;
    extract($dados);

    $sql = "SELECT '- '||i.iusnome||' ( '||p.pfldsc||' );' as descricao FROM sisfor.identificacaousuario i
			INNER JOIN sisfor.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
			WHERE i.iusemailprincipal = '{$iusemailprincipal}' AND i.iusd != {$iusd}";

    $nomes = $db->carregarColuna($sql);

    echo implode('\n', $nomes);
}

function verificarSituacaoCursoMEC($ieoid) {
    global $db;

    if ($ieoid) {
        $aryWhere[] = "ieoid = {$ieoid}";
    }

    $sql = "SELECT sifopcao FROM sisfor.sisfor " . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";
    $sifopcao = $db->pegaUm($sql);
    return $sifopcao;
}

function selecionarUnidade($unicod) {
    global $db;

    $sql = "SELECT unitpocod FROM public.unidade WHERE unicod = '{$unicod}'";

    $unitpocod = $db->pegaUm($sql);
    return $unitpocod;
}

function recuperarCodIES($iusd) {
    global $db;

    $sql = "SELECT 			unicod
			FROM 			sisfor.identificacaousuario ius 
			INNER JOIN		sisfor.sisfories sie ON ius.iuscpf = sie.usucpf
			LEFT JOIN 		sisfor.tipoperfil tpe ON tpe.tpeid = sie.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_INST . "			
			WHERE 			ius.iusd = {$iusd}";

    $unicod = $db->pegaUm($sql);
    return $unicod;
}

function validarDocumento($dados) {
    extract($dados);

    if (!$tdoid) {
        $erro[] = "Informe o 'Tipo de Documento'!";
    }
    if (!$itdufdoc) {
        $erro[] = "Informe o 'Estado do Documento'!";
    }
    if (!$itdnumdoc) {
        $erro[] = "Informe o 'Número do Documento'!";
    }
    if (!$itddataexp) {
        $erro[] = "Informe a 'Data Expedição'!";
    }
    if (!$itdnoorgaoexp) {
        $erro[] = "Informe o 'Orgão Expedidor'!";
    }

    return $erro;
}

function validarEndereco($dados) {
    extract($dados);

    if (!substr($muncod_endereco, 0, 7)) {
        $erro[] = "Informe o 'Município - Endereço'!";
    }
    if (!$ientipo) {
        $erro[] = "Informe o 'Tipo de Endereço'!";
    }
    if (!str_replace(array("-"), array(""), $iencep)) {
        $erro[] = "Informe o 'CEP'!";
    }
    if (!$ienlogradouro) {
        $erro[] = "Informe o 'Logradouro'!";
    }
    if (!$ienbairro) {
        $erro[] = "Informe o 'Bairro'!";
    }
    return $erro;
}

function sincronizarUsuariosSIMEC($dados) {
    global $db;
    extract($dados);

    $cpf = corrige_cpf($cpf);

    $sql = "UPDATE 		seguranca.usuario u SET
						usufoneddd = CASE WHEN foo.usufoneddd IS NULL THEN foo.dddtel::character(2) ELSE foo.usufoneddd END,
						usufonenum = CASE WHEN foo.usufonenum IS NULL THEN foo.tel ELSE foo.usufonenum END,
						muncod = CASE WHEN foo.muncod_segur IS NULL THEN foo.muncod_sisfor ELSE foo.muncod_segur END,
						regcod = CASE WHEN foo.estuf_segu IS NULL THEN foo.estuf_sisfor ELSE foo.estuf_segu END,
						tpocod = CASE WHEN foo.tpocod IS NULL THEN '1' ELSE foo.tpocod END,
						entid = CASE WHEN foo.entid IS NULL AND foo.orgcod IS NULL THEN 390402 ELSE foo.entid END,
						usudatanascimento = CASE WHEN foo.usudatanascimento IS NULL THEN foo.iusdatanascimento ELSE foo.usudatanascimento END,
						carid = CASE WHEN foo.carid IS NULL THEN 9 ELSE foo.carid END,
						usufuncao = CASE WHEN foo.funcao_segur IS NULL THEN foo.funcao_sisfor ELSE foo.funcao_segur END,
						ususexo = CASE WHEN foo.ususexo IS NULL THEN foo.iussexo ELSE foo.ususexo END,
						usunomeguerra = CASE WHEN foo.apelido_segur IS NULL THEN foo.apelido_sisfor ELSE foo.apelido_segur END
			FROM		(SELECT 		i.iuscpf,
										(SELECT itedddtel FROM sisfor.identificacaotelefone WHERE iusd=i.iusd AND itetipo='T') as dddtel,
										u.usufoneddd,
										(SELECT itenumtel FROM sisfor.identificacaotelefone WHERE iusd=i.iusd AND itetipo='T') as tel,
										u.usufonenum,
										i.muncod as muncod_sisfor,
										u.muncod as muncod_segur,
										m.estuf as estuf_sisfor,
										u.regcod as estuf_segu,
										u.tpocod,
										u.entid,
										u.orgcod,
										i.iusdatanascimento,
										u.usudatanascimento,
										u.carid,
										u.usufuncao as funcao_segur,
										p.pfldsc || ' - SISFOR' as funcao_sisfor,
										u.ususexo,
										i.iussexo,
										split_part(i.iusnome, ' ', 1) as apelido_sisfor,
										u.usunomeguerra as apelido_segur
						FROM 			sisfor.identificacaousuario i 
						INNER JOIN 		sisfor.tipoperfil t ON t.iusd = i.iusd 
						INNER JOIN 		seguranca.usuario u ON u.usucpf = i.iuscpf 
						INNER JOIN 		seguranca.perfil p ON p.pflcod = t.pflcod
						LEFT JOIN 		territorios.municipio m ON m.muncod = i.muncod 
						WHERE			 i.iuscpf = '{$cpf}'
			)foo WHERE foo.iuscpf = u.usucpf";

    $db->executar($sql);
    $db->commit();
}

function listarDocumentoDesignacao($iusd) {
    global $db;

    $perfil = pegarPerfil($_SESSION['usucpf']);

    $aryWhere[] = "iuastatus='A'";

    if ($iusd) {
        $aryWhere[] = "iusd = {$iusd}";
    }

    if (in_array(PFL_SUPER_USUARIO, $perfil)) {
        $acao = "'<center><img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"window.location=window.location+\'&requisicao=downloadDocumentoDesignacao&arqid='||a.arqid||'\'\">
				  &nbsp;&nbsp;<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"removerDocumentoDesignacao('||i.iuaid||')\"></center>' AS acao,";
    } else {
        $acao = "'<center><img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"window.location=window.location+\'&requisicao=downloadDocumentoDesignacao&arqid='||a.arqid||'\'\"></center>' AS acao,";
    }

    $sql = "SELECT 			$acao
							a.arqnome||'.'||a.arqextensao AS arquivo
			FROM 			sisfor.identificacaousuarioanexo i 
			INNER JOIN 		public.arquivo a ON a.arqid = i.arqid  
							" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";

    $cabecalho = array("Ação", "Arquivo");
    $db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%');
}

function anexarDocumentoDesignacao($dados) {
    global $db;
    extract($dados);

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $campos = array("iusd" => $iusd);
    $file = new FilesSimec("identificacaousuarioanexo", $campos, "sisfor");
    $file->setUpload(NULL, "arquivo");

    $al = array("alert" => "Documento de Designação gravada com sucesso!", "location" => $location);
    alertlocation($al);
}

function removerDocumentoDesignacao($iuaid) {
    global $db;

    $sql = "DELETE FROM sisfor.identificacaousuarioanexo WHERE iuaid = '{$iuaid}'";

    if ($db->executar($sql)) {
        $db->commit();
    }
}

function removerAnexoProjetoCurso($dados) {
    global $db;

    $sql = "DELETE FROM sisfor.anexoprojetocurso WHERE apcid = '{$dados['apcid']}'";

    $db->executar($sql);
    $db->commit();

    $al = array("alert" => "Documento removido com sucesso!",
        "location" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=dados_projeto");

    alertlocation($al);
}

function downloadDocumentoDesignacao($dados) {
    extract($dados);

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec("identificacaousuarioanexo", NULL, "sisfor");
    $file->getDownloadArquivo($arqid);
}

function carregarDadosSEB() {
    global $db;

    $aryWhereSEB[] = "vpl.unicod = '{$_SESSION['sisfor']['unicod']}'";
    $aryWhereSEB[] = "vpl.vppstatus = 'A' AND vpl.vppsecretaria = '1'";

    $sqlseb = "SELECT 		TO_CHAR(COALESCE(vpl.vppvalor,0),'999G999G990D99') AS valorloa,
							TO_CHAR(COALESCE(mec.valorestimado,0),'999G999G990D99') AS valorestimado,
							TO_CHAR(COALESCE(vpl.vppvalor,0) - COALESCE(mec.valorestimado,0),'999G999G990D99') AS saldomec,
							TO_CHAR(COALESCE(sis.sifvalorloa,0),'999G999G990D99') AS valormec, 
							TO_CHAR(COALESCE(vpl.vppvalor,0) - COALESCE(sis.sifvalorloa,0),'999G999G990D99') AS saldoies,
							TO_CHAR((COALESCE(ocu.sifvalorloa,0) + COALESCE(cnv.sifvalorloa,0) + COALESCE(oat.sifvalorloa,0)),'999G999G990D99') AS valoratividade,
							TO_CHAR((COALESCE(ocu.sifvaloroutras,0) + COALESCE(cnv.sifvaloroutras,0) + COALESCE(oat.sifvaloroutras,0)),'999G999G990D99') AS valoroutrocurso,
							TO_CHAR((COALESCE(ocu.valortotal,0) + COALESCE(cnv.valortotal,0) + COALESCE(oat.valortotal,0)),'999G999G990D99') AS valortotal
			   FROM 		sisfor.valorprevistoploa vpl 
			   LEFT JOIN 	(SELECT 		SUM(sifvalorloa) AS sifvalorloa, sif.unicod FROM sisfor.sisfor sif
							INNER JOIN 		catalogocurso2014.iesofertante ieo ON sif.ieoid = ieo.ieoid AND ieo.ieostatus = 'A'
							INNER JOIN 		catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
							LEFT JOIN  		catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
							WHERE			ieo.ieoid IS NOT NULL AND sifstatus = 'A' AND (coo.coordsigla ILIKE '%SEB%') GROUP BY sif.unicod) AS sis ON sis.unicod = vpl.unicod 
			   LEFT JOIN	(SELECT 		SUM(ieo.valor_estimado) AS valorestimado, ieo.unicod 
			   				 FROM  			catalogocurso2014.iesofertante ieo
							 INNER JOIN 	catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
				  			 LEFT JOIN   	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
				  			 WHERE 			ieo.ieostatus = 'A' AND (coo.coordsigla ILIKE '%SEB%') AND ieo.ieoid IS NOT NULL AND ieo.ieostatus = 'A'
				  			 GROUP BY		ieo.unicod) AS mec ON mec.unicod = vpl.unicod
			   LEFT JOIN    (SELECT 		SUM(sifvalorloa) AS sifvalorloa, SUM(sifvaloroutras) AS sifvaloroutras, SUM(COALESCE(sifvalorloa,0)+COALESCE(sifvaloroutras,0)) AS valortotal, unicod
							 FROM 			sisfor.sisfor sif
							 INNER JOIN		sisfor.outrocurso ocu ON sif.ocuid = ocu.ocuid
							 INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = ocu.coordid
							 WHERE			ocu.ocustatus = 'A' AND (coo.coordsigla ILIKE '%SEB%') GROUP BY unicod) AS ocu ON ocu.unicod = vpl.unicod
			   LEFT JOIN	(SELECT 		SUM(sifvalorloa) AS sifvalorloa, SUM(sifvaloroutras) AS sifvaloroutras, SUM(COALESCE(sifvalorloa,0)+COALESCE(sifvaloroutras,0)) AS valortotal, unicod
							 FROM 			sisfor.sisfor sif
							 INNER JOIN		sisfor.outraatividade oat ON sif.oatid = oat.oatid
							 INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = oat.coordid
							 WHERE			oat.oatstatus = 'A' AND (coo.coordsigla ILIKE '%SEB%') GROUP BY unicod) AS oat ON oat.unicod = vpl.unicod
			   LEFT JOIN 	(SELECT 		SUM(sifvalorloa) AS sifvalorloa, SUM(sifvaloroutras) AS sifvaloroutras, SUM(COALESCE(sifvalorloa,0)+COALESCE(sifvaloroutras,0)) AS valortotal, unicod
							 FROM 			sisfor.sisfor sif
							 INNER JOIN 	sisfor.cursonaovinculado cnv ON sif.cnvid = cnv.cnvid
							 INNER JOIN 	catalogocurso2014.curso cur ON cnv.curid = cur.curid
							 INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
							 WHERE			cnv.cnvstatus = 'A' AND (coo.coordsigla ILIKE '%SEB%') GROUP BY unicod) AS cnv ON cnv.unicod = vpl.unicod						  			 
							" . (is_array($aryWhereSEB) ? ' WHERE ' . implode(' AND ', $aryWhereSEB) : '') . "";

    $seb = $db->pegaLinha($sqlseb);
    return $seb;
}

function carregarDadosSECADI() {
    global $db;

    $aryWhereSECADI[] = "vpl.vppstatus = 'A' AND vpl.vppsecretaria = '2'";
    $aryWhereSECADI[] = "vpl.unicod = '{$_SESSION['sisfor']['unicod']}'";

    $sqlsecadi = "SELECT 	TO_CHAR(COALESCE(vpl.vppvalor,0),'999G999G990D99') AS valorloa,
							TO_CHAR(COALESCE(mec.valorestimado,0),'999G999G990D99') AS valorestimado,
							TO_CHAR(COALESCE(vpl.vppvalor,0) - COALESCE(mec.valorestimado,0),'999G999G990D99') AS saldomec,
							TO_CHAR(COALESCE(sis.sifvalorloa,0),'999G999G990D99') AS valormec, 
							TO_CHAR(COALESCE(vpl.vppvalor,0) - COALESCE(sis.sifvalorloa,0),'999G999G990D99') AS saldoies,
							TO_CHAR((COALESCE(ocu.sifvalorloa,0) + COALESCE(cnv.sifvalorloa,0) + COALESCE(oat.sifvalorloa,0)),'999G999G990D99') AS valoratividade,
							TO_CHAR((COALESCE(ocu.sifvaloroutras,0) + COALESCE(cnv.sifvaloroutras,0) + COALESCE(oat.sifvaloroutras,0)),'999G999G990D99') AS valoroutrocurso,
							TO_CHAR((COALESCE(ocu.valortotal,0) + COALESCE(cnv.valortotal,0) + COALESCE(oat.valortotal,0)),'999G999G990D99') AS valortotal							
			   FROM 		sisfor.valorprevistoploa vpl 
			   LEFT JOIN 	(SELECT 		SUM(sifvalorloa) AS sifvalorloa, sif.unicod FROM sisfor.sisfor sif
							INNER JOIN 		catalogocurso2014.iesofertante ieo ON sif.ieoid = ieo.ieoid AND ieo.ieostatus = 'A'
							INNER JOIN 		catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
							LEFT JOIN  		catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
							WHERE			ieo.ieoid IS NOT NULL AND sifstatus = 'A' AND (coo.coordsigla ILIKE '%SECADI%') GROUP BY sif.unicod) AS sis ON sis.unicod = vpl.unicod 
			   LEFT JOIN	(SELECT 		SUM(ieo.valor_estimado) AS valorestimado, ieo.unicod 
			   				 FROM  			catalogocurso2014.iesofertante ieo
							 INNER JOIN 	catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
				  			 LEFT JOIN   	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
				  			 WHERE 			ieo.ieostatus = 'A' AND (coo.coordsigla ILIKE '%SECADI%') AND ieo.ieoid IS NOT NULL AND ieo.ieostatus = 'A'
				  			 GROUP BY		ieo.unicod) AS mec ON mec.unicod = vpl.unicod 
			   LEFT JOIN    (SELECT 		SUM(sifvalorloa) AS sifvalorloa, SUM(sifvaloroutras) AS sifvaloroutras, SUM(COALESCE(sifvalorloa,0)+COALESCE(sifvaloroutras,0)) AS valortotal, unicod
							 FROM 			sisfor.sisfor sif
							 INNER JOIN		sisfor.outrocurso ocu ON sif.ocuid = ocu.ocuid
							 INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = ocu.coordid
							 WHERE			ocu.ocustatus = 'A' AND (coo.coordsigla ILIKE '%SECADI%') GROUP BY unicod) AS ocu ON ocu.unicod = vpl.unicod
			   LEFT JOIN	(SELECT 		SUM(sifvalorloa) AS sifvalorloa, SUM(sifvaloroutras) AS sifvaloroutras, SUM(COALESCE(sifvalorloa,0)+COALESCE(sifvaloroutras,0)) AS valortotal, unicod
							 FROM 			sisfor.sisfor sif
							 INNER JOIN		sisfor.outraatividade oat ON sif.oatid = oat.oatid
							 INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = oat.coordid
							 WHERE			oat.oatstatus = 'A' AND (coo.coordsigla ILIKE '%SECADI%') GROUP BY unicod) AS oat ON oat.unicod = vpl.unicod
			   LEFT JOIN 	(SELECT 		SUM(sifvalorloa) AS sifvalorloa, SUM(sifvaloroutras) AS sifvaloroutras, SUM(COALESCE(sifvalorloa,0)+COALESCE(sifvaloroutras,0)) AS valortotal, unicod
							 FROM 			sisfor.sisfor sif
							 INNER JOIN 	sisfor.cursonaovinculado cnv ON sif.cnvid = cnv.cnvid
							 INNER JOIN 	catalogocurso2014.curso cur ON cnv.curid = cur.curid
							 INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
							 WHERE			cnv.cnvstatus = 'A' AND (coo.coordsigla ILIKE '%SECADI%') GROUP BY unicod) AS cnv ON cnv.unicod = vpl.unicod				  			 								
				  			 " . (is_array($aryWhereSECADI) ? ' WHERE ' . implode(' AND ', $aryWhereSECADI) : '') . "";

    $secadi = $db->pegaLinha($sqlsecadi);
    return $secadi;
}

function salvarOutrosCursos($files, $post) {
    global $db;

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    extract($post);
    $unitpocod = selecionarUnidade($unicod);
    $sifvalorloa = desformata_valor($sifvalorloa);
    $sifvaloroutras = desformata_valor($sifvaloroutras);
    
    if($siftipoplanejamento == FASE02){
    	$url_sucesso = "principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2";
    	$url_insucesso = "principal/coordenador/cadastrooutroscursos2&acao=A";
    } else {
    	$url_sucesso = "principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies";
    	$url_insucesso = "principal/coordenador/cadastrooutroscursos&acao=A";
    }

    if ($files['arquivo']['tmp_name']) {
        $aryCampos = array("coordid" => $coordid, "ocunome" => "'" . $ocunome . "'", "ocustatus" => "'A'", "ocudesc" => "'" . $ocudesc . "'");
        $file = new FilesSimec("outrocurso", $aryCampos, "sisfor");
        $file->setUpload($ocunome, "arquivo", true, 'ocuid');
        $ocuid = $file->getCampoRetorno();

        if ($ocuid) {
            $sql = "INSERT INTO sisfor.sisfor
	 	    			  (unicod,unitpocod,ocuid,sifstatus,sifvalorloa,sifvaloroutras,sifqtdvagas,siftipoplanejamento)
		 			VALUES ({$unicod},'{$unitpocod}',{$ocuid},'A','{$sifvalorloa}','{$sifvaloroutras}',{$sifqtdvagas},{$siftipoplanejamento}) RETURNING sifid";

            $sifid = $db->pegaUm($sql);
            $db->commit();
        }

        $db->sucesso($url_sucesso, '', 'Curso cadastrado com sucesso!', 'S', 'S');
    } else {
    
        $sql = "INSERT INTO sisfor.outrocurso
	        (coordid,ocunome,ocustatus,ocudesc)
	        VALUES ({$coordid},'{$ocunome}','A','{$ocudesc}') RETURNING ocuid";
        
        $ocuid = $db->pegaUm($sql);
        $db->commit();
        
        if ($ocuid) {
            $sql = "INSERT INTO sisfor.sisfor
	 	    			  (unicod,unitpocod,ocuid,sifstatus,sifvalorloa,sifvaloroutras,sifqtdvagas,siftipoplanejamento)
		 			VALUES ({$unicod},'{$unitpocod}',{$ocuid},'A','{$sifvalorloa}','{$sifvaloroutras}',{$sifqtdvagas},{$siftipoplanejamento}) RETURNING sifid";

            $sifid = $db->pegaUm($sql);
            $db->commit();
        }

        $db->sucesso($url_sucesso, '', 'Curso cadastrado com sucesso!', 'S', 'S');
    }
}

function alterarOutrosCursos($files, $post) {
    global $db;

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    extract($post);
    $unitpocod = selecionarUnidade($unicod);
    $sifvalorloa = desformata_valor($sifvalorloa);
    $sifvaloroutras = desformata_valor($sifvaloroutras);

    if (empty($files)) {
        $sql = "UPDATE	 sisfor.sisfor
	    		SET		 sifvalorloa = '{$sifvalorloa}',
	    				 sifvaloroutras = '{$sifvaloroutras}',
	    				 sifqtdvagas = {$sifqtdvagas}
	    		WHERE	 ocuid = {$ocuid}";

        $db->executar($sql);

        $sql = "UPDATE	 sisfor.outrocurso
	    		SET		 coordid = {$coordid},
	    				 ocunome = '{$ocunome}',
	    				 ocudesc = '{$ocudesc}'
	    		WHERE	 ocuid = {$ocuid}";

        $db->executar($sql);
        $db->commit();
        $db->sucesso('principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies', '', 'Cadastro Curso alterado com sucesso!', 'S', 'S');
    } else {
       // if ($files['arquivo']['tmp_name']) {
            $arrCampos = array();
            $extensao = str_replace('.', '', strrchr($files['arquivo']['name'], '.'));
            $file = new FilesSimec("outrocurso", $arrCampos, "sisfor");
            $file->setMover($files['arquivo']['tmp_name'], $extensao, false);
            $arqid = $file->getIdArquivo();

            if ($arqid) {
                $sql = "UPDATE	 sisfor.sisfor
			    		SET		 sifvalorloa = '{$sifvalorloa}',
			    				 sifvaloroutras = '{$sifvaloroutras}',
			    				 sifqtdvagas = {$sifqtdvagas}
			    		WHERE	 ocuid = {$ocuid}";

                $db->executar($sql);

                $sql = "UPDATE	 sisfor.outrocurso
			    		SET		 coordid = {$coordid},
			    				 ocunome = '{$ocunome}',
			    				 ocudesc = '{$ocudesc}',
			    				 arqid = {$arqid}		 
			    		WHERE	 ocuid = {$ocuid}";

                $db->executar($sql);
                $db->commit();
            }
            $db->sucesso('principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies', '', 'Cadastro Curso alterado com sucesso!', 'S', 'S');
        //} else {
         //   $db->insucesso('Não foi possível realizar a operação.', '', 'principal/coordenador/cadastrooutroscursos&acao=A');
        //    exit();
       // }
    }
}

function listarOutrosCursos($ocuid = NULL, $siftipoplanejamento = NULL) {
    global $db;

    $aryWhere[] = "out.ocustatus = 'A'";
    
    if ($siftipoplanejamento) {
        $aryWhere[] = "sis.siftipoplanejamento = {$siftipoplanejamento}";
    }        

    if ($_SESSION['sisfor']['unicod']) {
        $aryWhere[] = "sis.unicod = '{$_SESSION['sisfor']['unicod']}'";
    }

    if ($ocuid) {
        $aryWhere[] = "out.ocuid = {$ocuid}";
    }

    $cabecalho = array('Ação', 'Secretaria', 'Nome', 'Descrição', 'Qtd. Vagas', 'Valor LOA', 'Valor Outras', 'Valor Total');
    
    $tamanho = array('7%', '7%', '', '', '7%', '8%','8%','8%');
    $alinhamento = array('center', 'center', '', '', 'center', 'right','right','right');
    	
    $acao = "CASE WHEN out.arqid IS NOT NULL THEN '<a href=\"sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A&download=S&arqid='|| out.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>&nbsp;&nbsp;' ELSE '' END ||
			 '<img border=\"0\" src=\"../imagens/alterar.gif\" id=\"'|| out.ocuid ||'\" onclick=\"visualizarCurso('|| out.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
			 <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| out.ocuid ||'\" onclick=\"excluirCursoOutros('|| out.ocuid ||');\" style=\"cursor:pointer;\"/>' AS acao,";

    $sql = "SELECT 		$acao
						cor.coordsigla,
						out.ocunome, 
						out.ocudesc,
						sis.sifqtdvagas,
						TO_CHAR(COALESCE(sis.sifvalorloa,0),'999G999G990D99') AS sifvalorloa,
						TO_CHAR(COALESCE(sis.sifvaloroutras,0),'999G999G990D99') AS sifvaloroutras,
						TO_CHAR(COALESCE(sis.sifvalorloa,0) + COALESCE(sis.sifvaloroutras,0),'999G999G990D99') AS valortotal
  			FROM 		sisfor.outrocurso out
  			LEFT JOIN   public.arquivo arq ON arq.arqid = out.arqid
  			LEFT JOIN	sisfor.sisfor sis ON sis.ocuid = out.ocuid
  			LEFT JOIN 	catalogocurso2014.coordenacao cor ON cor.coordid = out.coordid
  						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";

    if ($ocuid) {
        $outrocursos = $db->pegaLinha($sql);
        return $outrocursos;
    } else {
		$db->monta_lista($sql, $cabecalho, '50', '10', 'N', 'center', 'N', '', $tamanho, $alinhamento);
    }
}

function visualizarOutrosCursos($ocuid, $tipo = NULL, $siftipoplanejamento = NULL) {
    global $db;
 
    if ($_SESSION['sisfor']['unicod']) {
        $docid = criarDocumentoPlanejamento($_SESSION['sisfor']['unicod']);
        $esdid = pegarEstadoDocumento($docid);
        
        $docidplan2 = pegarDocidPlanejamento2($_SESSION['sisfor']['unicod']);
        $esdidplan2 = pegarEstadoDocumento($docidplan2);          
    }

    if($siftipoplanejamento == FASE02){
	    if ($esdidplan2 == WF_PLAN_ANALISE_MEC2) {
	        $arqid = "CASE WHEN out.arqid IS NOT NULL THEN '<a href=\"sisfor.php?modulo=principal/coordenador/cadastrooutroscursos2&acao=A&download=S&arqid='|| out.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>&nbsp;&nbsp;' || arq.arqnome||'.'||arq.arqextensao ELSE '' END AS arqid,";
	    } else {
	        $arqid = "CASE WHEN out.arqid IS NOT NULL THEN '<a href=\"sisfor.php?modulo=principal/coordenador/cadastrooutroscursos2&acao=A&download=S&arqid='|| out.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>&nbsp;&nbsp;' ELSE '' END ||
					  '<img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| out.arqid ||'\" onclick=\"excluirArquivo('|| out.arqid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'|| arq.arqnome||'.'||arq.arqextensao AS arqid,";
	    }
    } else {
	    if ($esdid == WF_PLAN_ANALISE_MEC) {
	        $arqid = "CASE WHEN out.arqid IS NOT NULL THEN '<a href=\"sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A&download=S&arqid='|| out.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>&nbsp;&nbsp;' || arq.arqnome||'.'||arq.arqextensao ELSE '' END AS arqid,";
	    } else {
	        $arqid = "CASE WHEN out.arqid IS NOT NULL THEN '<a href=\"sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A&download=S&arqid='|| out.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>&nbsp;&nbsp;' ELSE '' END ||
					  '<img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| out.arqid ||'\" onclick=\"excluirArquivo('|| out.arqid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'|| arq.arqnome||'.'||arq.arqextensao AS arqid,";
	    }
    }    

    $aryWhere[] = "out.ocustatus = 'A'";

    if ($ocuid) {
        $aryWhere[] = "out.ocuid = {$ocuid}";
    }

    $sql = "SELECT 		out.ocuid,
						out.coordid,
						out.ocunome, 
						out.ocudesc,
						$arqid
						sis.sifqtdvagas,
						TRIM(TO_CHAR(COALESCE(sis.sifvalorloa,0),'999G999G990D99')) AS sifvalorloa,
						TRIM(TO_CHAR(COALESCE(sis.sifvaloroutras,0),'999G999G990D99')) AS sifvaloroutras,
						TO_CHAR(COALESCE(sis.sifvalorloa,0) + COALESCE(sis.sifvaloroutras,0),'999G999G990D99') AS valortotal
  			FROM 		sisfor.outrocurso out
  			LEFT JOIN   public.arquivo arq ON arq.arqid = out.arqid
  			LEFT JOIN	sisfor.sisfor sis ON sis.ocuid = out.ocuid
  			LEFT JOIN 	catalogocurso2014.coordenacao cor ON cor.coordid = out.coordid
  						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";

    $cursos = $db->pegaLinha($sql);
    
    if ($tipo == 'html') {
        return $cursos;
    } else {
        $cursos["ocunome"] = iconv("ISO-8859-1", "UTF-8", $cursos["ocunome"]);
        $cursos["ocudesc"] = iconv("ISO-8859-1", "UTF-8", $cursos["ocudesc"]);
        $cursos["sifqtdvagas"] = iconv("ISO-8859-1", "UTF-8", $cursos["sifqtdvagas"]);
        echo simec_json_encode($cursos);
    }
}

function excluirOutrosCursos($ocuid) {
    global $db;

    if ($ocuid != '') {
        $sql = "UPDATE sisfor.outrocurso SET ocustatus = 'I' WHERE ocuid = {$ocuid};
				UPDATE sisfor.sisfor SET sifstatus = 'I' WHERE ocuid = {$ocuid};";
    }

    if ($db->executar($sql)) {
        $db->commit();
    }
}


function salvarOutrasAtividades($post, $files = NULL) {
    global $db;

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    extract($post);

    $unitpocod = selecionarUnidade($unicod);
    $sifvalorloa = desformata_valor($sifvalorloa);
    $sifvaloroutras = desformata_valor($sifvaloroutras);
    $ateid = $ateid ? "{$ateid}" : "NULL";

    if($siftipoplanejamento == FASE02){
   	$oateducaoinfantil = $oateducaoinfantil ? "'{$oateducaoinfantil}'" : "'f'";
    	$oatfundamentalinicial = $oatfundamentalinicial ? "'{$oatfundamentalinicial}'" : "'f'";
    	$oatfundamentalfinal = $oatfundamentalfinal ? "'{$oatfundamentalfinal}'" : "'f'";
    	$oatensinomedio = $oatensinomedio ? "'{$oatensinomedio}'" : "'f'";
    	$oatnaoseaplica = $oatnaoseaplica ? "'{$oatnaoseaplica }'" : "'f'";

    	$url_sucesso = "principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2";
    	$url_insucesso = "principal/coordenador/cadastroatividades2&acao=A";
    } else {
   	$oateducaoinfantil = "NULL";
    	$oatfundamentalinicial = "NULL";
    	$oatfundamentalfinal = "NULL";
    	$oatensinomedio = "NULL";
    	$oatnaoseaplica = "NULL";

    	$url_sucesso = "principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies";
    	$url_insucesso = "principal/coordenador/cadastroatividades&acao=A";
    }    
    
    if ($files['arquivo']['tmp_name']) {
        $aryCampos = array("coordid" => $coordid, "oatnome" => "'{$oatnome}'", "oatdesc" => "'{$oatdesc}'", "oatanoproposta" => "{$oatanoproposta}", "oatstatus" => "'A'", "oatatividade" => "'{$oatatividade}'","ateid" => $ateid, "oateducaoinfantil" => $oateducaoinfantil, "oatfundamentalinicial" => $oatfundamentalinicial, "oatfundamentalfinal" => $oatfundamentalfinal, "oatensinomedio" => $oatensinomedio, "oatnaoseaplica" => $oatnaoseaplica);
        $file = new FilesSimec("outraatividade", $aryCampos, "sisfor");
        $file->setUpload($oatnome, "arquivo", true, 'oatid');
        $oatid = $file->getCampoRetorno();    
    } else {
	    $sql = "INSERT INTO sisfor.outraatividade
	 	   			  (coordid,oatnome,oatdesc,oatanoproposta,oatstatus,oatatividade,ateid, oateducaoinfantil, oatfundamentalinicial, oatfundamentalfinal, oatensinomedio, oatnaoseaplica)
				VALUES ({$coordid},'{$oatnome}','{$oatdesc}',{$oatanoproposta},'A','{$oatatividade}',$ateid,$oateducaoinfantil,{$oatfundamentalinicial},$oatfundamentalfinal,$oatensinomedio,$oatnaoseaplica) RETURNING oatid";
	
	    $oatid = $db->pegaUm($sql);    	
    }

    if ($oatid) {
        $sql = "INSERT INTO sisfor.sisfor
	 	   			  (unicod,unitpocod,oatid,sifstatus,sifvalorloa,sifvaloroutras,siftipoplanejamento)
				VALUES ({$unicod},'{$unitpocod}',{$oatid},'A','{$sifvalorloa}','{$sifvaloroutras}',{$siftipoplanejamento}) RETURNING sifid";

        $sifid = $db->pegaUm($sql);
    }

    if ($sifid) {
        $db->commit();
        $db->sucesso($url_sucesso, '', 'Atividade cadastrada com sucesso!', 'S', 'S');
    } else {
        $db->insucesso('Não foi possível realizar a operação.', '', $url_insucesso);
    }
}

function alterarOutrasAtividades($post) {
    global $db;

    extract($post);
    $unitpocod = selecionarUnidade($unicod);
    $sifvalorloa = desformata_valor($sifvalorloa);
    $sifvaloroutras = desformata_valor($sifvaloroutras);
    $ateid = $ateid ? "{$ateid}" : "NULL";
    
    if($siftipoplanejamento == FASE02){
   		$oateducaoinfantil = $oateducaoinfantil ? "'{$oateducaoinfantil}'" : "'f'";
    	$oatfundamentalinicial = $oatfundamentalinicial ? "'{$oatfundamentalinicial}'" : "'f'";
    	$oatfundamentalfinal = $oatfundamentalfinal ? "'{$oatfundamentalfinal}'" : "'f'";
    	$oatensinomedio = $oatensinomedio ? "'{$oatensinomedio}'" : "'f'";
    	$oatnaoseaplica = $oatnaoseaplica ? "'{$oatnaoseaplica }'" : "'f'";

    	$url_sucesso = "principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2";
    	$url_insucesso = "principal/coordenador/cadastroatividades2&acao=A";
    } else {
   	$oateducaoinfantil = "NULL";
    	$oatfundamentalinicial = "NULL";
    	$oatfundamentalfinal = "NULL";
    	$oatensinomedio = "NULL";
    	$oatnaoseaplica = "NULL";

    	$url_sucesso = "principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies";
    	$url_insucesso = "principal/coordenador/cadastroatividades&acao=A";
    }    
    
    $sql = "UPDATE	 sisfor.sisfor
    		SET		 sifvalorloa = '{$sifvalorloa}',
    				 sifvaloroutras = '{$sifvaloroutras}'
    		WHERE	 oatid = {$oatid}";

    $db->executar($sql);

    $sql = "UPDATE	 sisfor.outraatividade
    		SET		 coordid = {$coordid},
    				 oatnome = '{$oatnome}',
    				 oatdesc = '{$oatdesc}',
    				 oatanoproposta = {$oatanoproposta},
    				 oatatividade = '{$oatatividade}',
    				 ateid = $ateid,
				 	oateducaoinfantil = $oateducaoinfantil, 
				 	oatfundamentalinicial = $oatfundamentalinicial, 
				 	oatfundamentalfinal = $oatfundamentalfinal, 
				 	oatensinomedio = $oatensinomedio, 
				 	oatnaoseaplica = $oatnaoseaplica
    		WHERE	oatid = {$oatid}";

    $db->executar($sql);

    if ($db->commit()) {
        $db->sucesso($url_sucesso, '', 'Atividade alterada com sucesso!', 'S', 'S');
    } else {
        $db->insucesso('Não foi possível realizar a operação.', '', $url_insucesso);
    }
}

function listarOutrasAtividades($oatid = NULL, $siftipoplanejamento = NULL) {
    global $db;

    $aryWhere[] = "oat.oatstatus = 'A'";
    
    if ($siftipoplanejamento) {
        $aryWhere[] = "sis.siftipoplanejamento = {$siftipoplanejamento}";
    }        

    if ($_SESSION['sisfor']['unicod']) {
        $aryWhere[] = "sis.unicod = '{$_SESSION['sisfor']['unicod']}'";
    }

    if ($oatid) {
        $aryWhere[] = "oat.oatid = {$oatid}";
    }
    
    if($siftipoplanejamento == FASE02){
        $acao = "'<img id=\"img_dimensao_' || sis.sifid  || '\" src=\"/imagens/mais.gif\" style=\"cursor: pointer\" onclick=\"carregarListaCustoAtividade(this.id,' || sis.sifid  || ');\"/>&nbsp;&nbsp;'";
    	$detalhe = ", TO_CHAR(COALESCE(sis.sifvaloroutras,0),'999G999G990D99') || '</td></tr><tr style=\"display:none\" id=\"listaCusto_' || sis.sifid || '\"><td id=\"trA_' || sis.sifid || '\" colspan=\"7\"></td></tr>' AS sifvalorloa";
    	$alinhamento = array('left', 'center', '', '', '', 'right');
    	$cabecalho = array('Ação', 'Secretaria', 'Nome', 'Descrição', 'Ano Proposta', 'Valor LOA','Valor Outras');
    }  else {
    	$detalhe = "AS sifvalorloa";
    	$tamanho = array('8%', '10%', '', '', '', '10%');
    	$alinhamento = array('center', 'center', '', '', 'center', 'right');
    	$cabecalho = array('Ação', 'Secretaria', 'Nome', 'Descrição', 'Ano Proposta', 'Valor LOA');
    }    

    $acao .= "'<img border=\"0\" src=\"../imagens/alterar.gif\" id=\"'|| oat.oatid ||'\" onclick=\"visualizarAtividade('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
			  <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| oat.oatid ||'\" onclick=\"excluirAtividade('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>' AS acao,";

    $sql = "SELECT 		$acao
						cor.coordsigla,
						oat.oatnome, 
						oat.oatdesc,
						oat.oatanoproposta,
						TO_CHAR(COALESCE(sis.sifvalorloa,0),'999G999G990D99') 
						$detalhe
  			FROM 		sisfor.outraatividade oat
  			LEFT JOIN	sisfor.sisfor sis ON sis.oatid = oat.oatid
  			LEFT JOIN 	catalogocurso2014.coordenacao cor ON cor.coordid = oat.coordid
  						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";

    if ($oatid) {
        $atividades = $db->pegaLinha($sql);
        return $atividades;
    } else {
    	$db->monta_lista($sql, $cabecalho, '50', '10', 'N', 'center', 'N', '', $tamanho, $alinhamento);
    }
}

function visualizarOutrasAtividades($oatid, $tipo = NULL, $siftipoplanejamento = NULL) {
    global $db;
    
    if ($_SESSION['sisfor']['unicod']) {
        $docidplan2 = pegarDocidPlanejamento2($_SESSION['sisfor']['unicod']);
        $esdidplan2 = pegarEstadoDocumento($docidplan2);              
    }    

    $aryWhere[] = "oat.oatstatus = 'A'";

    if ($oatid) {
        $aryWhere[] = "oat.oatid = {$oatid}";
    }
    
    if($siftipoplanejamento == FASE02){
	    if($esdidplan2 == WF_PLAN_ANALISE_MEC2) {
	        $arqid = "CASE WHEN oat.arqid IS NOT NULL THEN '<a href=\"sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A&download=S&arqid='|| oat.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>&nbsp;&nbsp;' || arq.arqnome ||'.'|| arq.arqextensao ELSE '' END AS arqid,";
	    } else {
	        $arqid = "CASE WHEN oat.arqid IS NOT NULL THEN '<a href=\"sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A&download=S&arqid='|| oat.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>&nbsp;&nbsp;' ELSE '' END ||
					  '<img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| oat.arqid ||'\" onclick=\"excluirArquivo('|| oat.arqid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'|| arq.arqnome||'.'|| arq.arqextensao AS arqid,";
	    }   
    } else {
    	$arqid = "";
    }   
    
    $sql = "SELECT 		oat.oatid,
						oat.coordid,
						oat.oatnome, 
						oat.oatdesc,
						$arqid
						oat.oatanoproposta,
						oat.oatatividade,
						TRIM(TO_CHAR(COALESCE(sis.sifvalorloa,0),'999G999G990D99')) AS sifvalorloa,
						TRIM(TO_CHAR(COALESCE(sis.sifvaloroutras,0),'999G999G990D99')) AS sifvaloroutras,
						sis.sifid,
						oat.ateid,
						oat.oateducaoinfantil, 
						oat.oatfundamentalinicial, 
						oat.oatfundamentalfinal, 
						oat.oatensinomedio, 
						oat.oatnaoseaplica
  			FROM 		sisfor.outraatividade oat
  			LEFT JOIN   public.arquivo arq ON arq.arqid = oat.arqid
  			LEFT JOIN	sisfor.sisfor sis ON sis.oatid = oat.oatid
  			LEFT JOIN 	catalogocurso2014.coordenacao cor ON cor.coordid = oat.coordid
  						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";

    $atividades = $db->pegaLinha($sql);

    if ($tipo == 'html') {
        return $atividades;
    } else {
        $atividades["oatnome"] = iconv("ISO-8859-1", "UTF-8", $atividades["oatnome"]);
        $atividades["oatdesc"] = iconv("ISO-8859-1", "UTF-8", $atividades["oatdesc"]);
        $atividades["sifvalorloa"] = iconv("ISO-8859-1", "UTF-8", $atividades["sifvalorloa"]);
        echo simec_json_encode($atividades);
    }
}

function excluirOutrasAtividades($post) {
    global $db;
    
    extract($post);

    if ($oatid != '') {
        $sql = "UPDATE sisfor.outraatividade SET oatstatus = 'I' WHERE oatid = {$oatid};
				UPDATE sisfor.sisfor SET sifstatus = 'I' WHERE oatid = {$oatid};";
        $db->executar($sql);
    }
    
    if($siftipoplanejamento == FASE02){
  		$sql = "SELECT sifid FROM sisfor.sisfor WHERE oatid = {$oatid}";	  	
    	
  		$sifid  = $db->pegaUm($sql);
    }

    if($sifid) {
    	$sql = "UPDATE sisfor.orcamentoplanejamentofase2 SET orcstatus = 'I' WHERE sifid = {$sifid}";
    	$db->executar($sql);
    }
    
	$db->commit();
}

function salvarCursoCatalogo($post) {
    global $db;

    extract($post);
    $unitpocod = selecionarUnidade($unicod);
    
    if($siftipoplanejamento == FASE02){
    	$url_sucesso = "principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2";
    	$url_insucesso = "principal/coordenador/cadastrocursocatalogo2&acao=A";
    } else {
    	$url_sucesso = "principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies";
    	$url_insucesso = "principal/coordenador/cadastrocursocatalogo&acao=A";
    }

    foreach ($curid AS $c) {
        $sifvalorloa = desformata_valor($post['sifvalorloa_' . $c]);
        $sifvaloroutras = desformata_valor($post['sifvaloroutras_' . $c]);
        $sifqtdvagas = $post['sifqtdvagas_' . $c];
        $sifaprovado2013 = $post['sifaprovado2013_' . $c];

        $sqlc = "INSERT INTO sisfor.cursonaovinculado (curid, cnvstatus) VALUES ({$c}, 'A') RETURNING cnvid; ";
        $cnvid = $db->pegaUm($sqlc);

        if ($cnvid) {
            $sql .= "INSERT INTO sisfor.sisfor
	 	    			  (unicod,unitpocod,cnvid,sifstatus,sifvalorloa,sifvaloroutras,sifqtdvagas,sifaprovado2013,siftipoplanejamento)
		 			 VALUES ({$unicod},'{$unitpocod}',{$cnvid},'A','{$sifvalorloa}','{$sifvaloroutras}',{$sifqtdvagas},'{$sifaprovado2013}',{$siftipoplanejamento});";
        }
    }

    if ($db->executar($sql)) {
        $db->commit();
        $db->sucesso($url_sucesso, '', 'Curso cadastrado com sucesso!', 'S', 'S');
    } else {
        $db->insucesso('Não foi possível realizar a operação.', '', $url_insucesso);
    }
}

function alterarCursoCatalogo($post) {
    global $db;
    extract($post);

    foreach ($curid AS $c) {
        $sifvalorloa = desformata_valor($post['sifvalorloa_' . $c]);
        $sifvaloroutras = desformata_valor($post['sifvaloroutras_' . $c]);
        $sifqtdvagas = $post['sifqtdvagas_' . $c];
        $sifaprovado2013 = $post['sifaprovado2013_' . $c];

        $sql = "UPDATE   sisfor.sisfor
				SET	  	 sifvalorloa = '{$sifvalorloa}',
				 		 sifvaloroutras = '{$sifvaloroutras}',
				 		 sifqtdvagas = {$sifqtdvagas},
				 		 sifaprovado2013 = '{$sifaprovado2013}'
				WHERE 	 cnvid = {$cnvid}";
    }

    if ($db->executar($sql)) {
        $db->commit();
        $db->sucesso('principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies', '', 'Curso alterado com sucesso!', 'S', 'S');
    } else {
        $db->insucesso('Não foi possível realizar a operação.', '', 'principal/coordenador/cadastrocursocatalogo&acao=A');
    }
}

function excluirCursoCatalogo($cnvid) {
    global $db;

    if ($cnvid != '') {
        $sql = "UPDATE sisfor.cursonaovinculado SET cnvstatus = 'I' WHERE cnvid = {$cnvid};
				UPDATE sisfor.sisfor SET sifstatus = 'I' WHERE cnvid = {$cnvid};";
    }

    if ($db->executar($sql)) {
        $db->commit();
    }
}

function listarCursoCatalogo() {
    global $db;

    $cabecalho = array('Ação', 'Secretaria', 'Curso', 'Qtd. Vagas', 'Valor LOA', 'Valor Outras', 'Valor Total');

    $acao = "'<img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| sis.cnvid ||'\" onclick=\"excluirCursoCatalogo('|| sis.cnvid ||');\" style=\"cursor:pointer;\"/>' AS acao,";

    $sql = "SELECT 			$acao
							cor.coordsigla,
							cur.curdesc, 
							sis.sifqtdvagas,
							TO_CHAR(COALESCE(sis.sifvalorloa,0),'999G999G990D99') AS sifvalorloa,
							TO_CHAR(COALESCE(sis.sifvaloroutras,0),'999G999G990D99') AS sifvaloroutras,
							TO_CHAR(COALESCE(sis.sifvalorloa,0) + COALESCE(sis.sifvaloroutras,0),'999G999G990D99') AS valortotal,
							sis.sifaprovado2013					
  			FROM 			sisfor.cursonaovinculado cnv
  			INNER JOIN		catalogocurso2014.curso cur ON cur.curid = cnv.curid AND cur.curstatus = 'A'
  			LEFT JOIN 		catalogocurso2014.coordenacao cor ON cor.coordid = cur.coordid
  			LEFT JOIN		sisfor.sisfor sis ON sis.cnvid = cnv.cnvid 
  			WHERE 			cnv.cnvstatus = 'A' AND sis.unicod = '{$_SESSION['sisfor']['unicod']}'";

    $db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '');
}

function selecionarCursoCatalogo($cnvid = NULL, $siftipoplanejamento = NULL){
    global $db;

    $cabecalho = array('Ação', 'Código', 'Cursos do Catálogo 2014', 'Secretaria responsável', 'Qtd. Vagas', 'Valor LOA', 'Valor Outras', 'Curso aprovado em 2013?');

    $aryWhere[] = "abr.abcexibirsisfor = 't'";
	
    if ($cnvid) {
        $aryWhere[] = "cnv.cnvstatus = 'A'";
        $aryWhere[] = "sif.cnvid = {$cnvid}";

        $acao = "'<input type=\"checkbox\" name=\"curid[]\" id=\"curid\" value=\"'|| cur.curid ||'\" checked>&nbsp;&nbsp;
				  <img border=\"0\" src=\"../imagens/print.gif\" id=\"'|| cur.curid ||'\" onclick=\"imprimirCurso('|| cur.curid ||');\" style=\"cursor:pointer;\"/>' AS acao,";

        $sql = "SELECT 	$acao 
	   					cur.curdesc,
	            		coo.coordsigla,
	                	CASE WHEN sif.sifqtdvagas IS NOT NULL THEN '<input id=\"sifqtdvagas_'|| cur.curid ||'\" type=\"text\" class=\"normal\" onkeyup=\"this.value=mascaraglobal(\'####\',this.value);\" value=\"'|| sif.sifqtdvagas ||'\" name=\"sifqtdvagas_'|| cur.curid ||'\" size=\"6\">' ELSE '<input id=\"sifqtdvagas_'|| cur.curid ||'\" type=\"text\" class=\"normal\" onkeyup=\"this.value=mascaraglobal(\'####\',this.value);\" value=\"\" name=\"sifqtdvagas_'|| cur.curid ||'\" size=\"6\">' END AS sifqtdvagas,
	                	CASE WHEN sif.sifvalorloa IS NOT NULL THEN '<input id=\"sifvalorloa_'|| cur.curid ||'\" type=\"text\" class=\"normal\" onblur=\"atualizarSaldo(this.value,'|| coo.coordid ||');\" onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" value=\"'|| TRIM(TO_CHAR(COALESCE(sif.sifvalorloa,0),'999G999G990D99')) ||'\" name=\"sifvalorloa_'|| cur.curid ||'\" size=\"10\" maxlenght=\"9\">' ELSE '<input id=\"sifvalorloa_'|| cur.curid ||'\" type=\"text\" class=\"normal\" onblur=\"atualizarSaldo(this.value,'|| coo.coordid ||');\" onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" value=\"\" name=\"sifvalorloa_'|| cur.curid ||'\" size=\"10\" maxlenght=\"9\">' END AS sifvalorloa, 
	                	CASE WHEN sif.sifvaloroutras IS NOT NULL THEN '<input id=\"sifvaloroutras_'|| cur.curid ||'\" type=\"text\" class=\"normal\" onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" value=\"'|| TRIM(TO_CHAR(COALESCE(sif.sifvaloroutras,0),'999G999G990D99')) ||'\" name=\"sifvaloroutras_'|| cur.curid ||'\" size=\"10\" maxlenght=\"9\">' ELSE '<input id=\"sifvaloroutras_'|| cur.curid ||'\" type=\"text\" class=\"normal\" onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" value=\"\" name=\"sifvaloroutras_'|| cur.curid ||'\" size=\"10\" maxlenght=\"9\">' END AS sifvaloroutras,
	                	CASE WHEN sif.sifaprovado2013 = 't' THEN '<input id=\"sifaprovado2013_'|| cur.curid ||'\" type=\"radio\" name=\"sifaprovado2013_'|| cur.curid ||'\" value=\"t\" checked=\"checked\">Sim<input id=\"sifaprovado2013_'|| cur.curid ||'\" type=\"radio\" name=\"sifaprovado2013_'|| cur.curid ||'\" value=\"f\">Não'
	    					 WHEN sif.sifaprovado2013 = 'f' THEN '<input id=\"sifaprovado2013_'|| cur.curid ||'\" type=\"radio\" name=\"sifaprovado2013_'|| cur.curid ||'\" value=\"t\">Sim<input id=\"sifaprovado2013_'|| cur.curid ||'\" type=\"radio\" name=\"sifaprovado2013_'|| cur.curid ||'\" value=\"f\" checked=\"checked\">Não'
	    					 								ELSE '<input id=\"sifaprovado2013_'|| cur.curid ||'\" type=\"radio\" name=\"sifaprovado2013_'|| cur.curid ||'\" value=\"t\">Sim<input id=\"sifaprovado2013_'|| cur.curid ||'\" type=\"radio\" name=\"sifaprovado2013_'|| cur.curid ||'\" value=\"f\">Não' END AS sifaprovado2013
			FROM		sisfor.cursonaovinculado cnv 
			INNER JOIN  sisfor.sisfor sif ON sif.cnvid = cnv.cnvid
			INNER JOIN 	catalogocurso2014.curso cur ON cnv.curid = cur.curid
			LEFT JOIN 	catalogocurso2014.abrangenciacurso abr ON abr.curid = cur.curid 
			INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "
			ORDER BY	coo.coordsigla, cur.curdesc";
    } else {
	
		if($siftipoplanejamento == FASE02){
		
			$aryWhere[] = "cur.curid NOT IN (select
  
              cur.curid
				 
				from sisfor.sisfor s
				inner join workflow.documento d on d.docid = s.docidprojeto
				left join workflow.historicodocumento h on h.hstid = d.hstid
				inner join workflow.estadodocumento e on e.esdid = d.esdid
				inner join public.unidade u on u.unicod = s.unicod
				left join workflow.documento dctur on dctur.docid = s.docidcomposicaoequipe
				left join workflow.estadodocumento ectur on ectur.esdid = dctur.esdid  
				left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
				left join catalogocurso2014.curso cur on cur.curid = ieo.curid
				left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
				left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
				left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
				left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
				left join seguranca.usuario usu on usu.usucpf = s.usucpf
				left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
				left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid
				where sifstatus='A'   and d.esdid = ".ESD_PROJETO_VALIDADO."  and cur.curid is not null
				 and u.unicod = '{$_SESSION['sisfor']['unicod']}' ) 
															";			
		}
		
        $aryWhere[] = "(coo.coordsigla ILIKE '%SEB%' OR coo.coordsigla ILIKE '%SECADI%')";

        $acao = "'<input id=\"curid\" name=\"curid[]\" value=\"'|| cur.curid ||'\" type=\"checkbox\" class=\"'|| SUBSTR(coordsigla,1,3) ||'\">&nbsp;&nbsp;
				  <img border=\"0\" src=\"../imagens/print.gif\" id=\"'|| cur.curid ||'\" onclick=\"imprimirCurso('|| cur.curid ||');\" style=\"cursor:pointer;\"/>' AS acao,";

        $sql = "SELECT 		DISTINCT $acao
		   					cur.curid,
		   					cur.curdesc,
		            		coo.coordsigla,
		                	'<input id=\"sifqtdvagas_'|| cur.curid ||'\" type=\"text\" class=\"'|| SUBSTR(coordsigla,1,3) ||'\" onkeyup=\"this.value=mascaraglobal(\'####\',this.value);\" value=\"\" name=\"sifqtdvagas_'|| cur.curid ||'\" size=\"6\">' AS sifqtdvagas,
		                	'<input id=\"sifvalorloa_'|| cur.curid ||'\" class=\"'|| SUBSTR(coordsigla,1,3) ||'\" type=\"text\" class=\"normal\" onblur=\"atualizarSaldo(this.value,'|| coo.coordid ||');\" onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" value=\"\" name=\"sifvalorloa_'|| cur.curid ||'\" size=\"10\" maxlenght=\"9\">' AS sifvalorloa, 
		                	'<input id=\"sifvaloroutras_'|| cur.curid ||'\" type=\"text\" class=\"'|| SUBSTR(coordsigla,1,3) ||'\" onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" value=\"\" name=\"sifvaloroutras_'|| cur.curid ||'\" size=\"10\" maxlenght=\"9\">' AS sifvaloroutras,
		                	'<input id=\"sifaprovado2013_'|| cur.curid ||'\" type=\"radio\" class=\"'|| SUBSTR(coordsigla,1,3) ||'\" name=\"sifaprovado2013_'|| cur.curid ||'\" value=\"t\">Sim<input id=\"sifaprovado2013_'|| cur.curid ||'\" type=\"radio\" class=\"'|| SUBSTR(coordsigla,1,3) ||'\" name=\"sifaprovado2013_'|| cur.curid ||'\" value=\"f\">Não' AS sifaprovado2013
				FROM        catalogocurso2014.curso cur
				LEFT JOIN   catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
				LEFT JOIN 	catalogocurso2014.abrangenciacurso abr ON abr.curid = cur.curid 
							" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "
				ORDER BY	coo.coordsigla, cur.curdesc";
    }

    $alinhamento = array('center', '', '', 'center', 'center', 'center', 'center');
    $tamanho = array('7%', '', '', '', '', '', '12%');
    $db->monta_lista($sql, $cabecalho, '50', '10', 'N', 'center', 'N', 'formulario_curso', $tamanho, $alinhamento);
}

function verificarCadastroCoordenador($iusd) {
    global $db;
    if ($iusd) {
        $aryWhere[] = "i.iusd = {$iusd}";

        $sql = "SELECT 			i.iuscpf,
								i.iusnome, 
								i.iusdatanascimento, 
								m.estuf AS estuf_nascimento, 
					   			m.muncod AS muncod_nascimento, 
					   			i.iussexo,
								i.iusnomemae, 
								i.nacid, 
								i.eciid,
								i.iusnomeconjuge, 
								d.itdufdoc, 
								d.tdoid, 
								d.itdnumdoc, 
								d.itddataexp, 
								d.itdnoorgaoexp,
								i.tvpid, 
								i.funid, 
								i.foeid, 
								f.iufsituacaoformacao,
					   			f.iufdatainiformacao, 
					   			f.iufdatafimformacao, 								
								e.ientipo, 
								e.iencep, 
								mm.estuf AS estuf_endereco,
								mm.muncod AS muncod_endereco, 
								e.ienlogradouro, 
								e.ienbairro, 
								e.iennumero, 
								i.iusemailprincipal, 
								i.iusagenciasugerida, 
								i.iusagenciaend, 
								i.iustermocompromisso, 
								cf.cufcodareageral, 
								f.cufid
				FROM 			sisfor.identificacaousuario i
				INNER JOIN		sisfor.sisfories sie ON i.iuscpf = sie.usucpf
				INNER JOIN 		sisfor.tipoperfil t ON t.iusd = i.iusd AND t.pflcod = " . PFL_COORDENADOR_INST . "
				LEFT  JOIN 		territorios.municipio m ON m.muncod = i.muncod 
				LEFT  JOIN 		sisfor.identiusucursoformacao f ON f.iusd = i.iusd 
				LEFT  JOIN 		sisfor.identusutipodocumento d ON d.iusd = i.iusd 
				LEFT  JOIN 		sisfor.identificaoendereco e ON e.iusd = i.iusd
				LEFT  JOIN 		territorios.municipio mm ON mm.muncod = e.muncod
				LEFT  JOIN 		territorios.municipio ma ON ma.muncod = i.muncodatuacao
				LEFT  JOIN 		sisfor.cursoformacao cf ON cf.cufid = f.cufid 
								" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "
				ORDER BY 		i.iusd";

        $aResultado = $db->carregar($sql);
    }

    if (!empty($aResultado)) {
        if ($aResultado) {
            $aVerificacao = array('iuscpf', 'iusnome', 'iusdatanascimento', 'estuf_nascimento', 'muncod_nascimento', 'iussexo', 'iusnomemae', 'nacid', 'eciid', 'itdufdoc', 'tdoid', 'itdnumdoc', 'itddataexp', 'itdnoorgaoexp', 'tvpid', 'funid',
                'foeid', 'iufsituacaoformacao', 'iufdatainiformacao', 'ientipo', 'iencep', 'estuf_endereco', 'muncod_endereco', 'ienlogradouro', 'ienbairro', 'iennumero', 'iusemailprincipal',
                'iusagenciasugerida', 'iusagenciaend', 'iustermocompromisso');

            if ($aResultado[0]['eciid'] == ECI_CASADO || $aResultado[0]['eciid'] == ECI_UNIAO_ESTAVEL) {
                array_push($aVerificacao, 'iusnomeconjuge');
            }

            if ($aResultado[0]['foeid'] == FOE_ESPECIALIZACAO || $aResultado[0]['foeid'] == FOE_MESTRADO || $aResultado[0]['foeid'] == FOE_DOUTORADO || $aResultado[0]['foeid'] == FOE_SUPERIOR_COMPLETO_PEDAGOGIA || $aResultado[0]['foeid'] == FOE_SUPERIOR_COMPLETO_LICENCIATURA || $aResultado[0]['foeid'] == FOE_SUPERIOR_COMPLETO_OUTRO) {
                array_push($aVerificacao, 'cufcodareageral');
                array_push($aVerificacao, 'cufid');
            }

            if ($aResultado[0]['iufsituacaoformacao'] == 'C') {
                array_push($aVerificacao, 'iufdatafimformacao');
            }

            foreach ($aResultado as $count => $linha) {
                $status = 'S';
                foreach ($aVerificacao as $campo) {
                    if (trim($linha[$campo]) == '') {
                        $status = 'N';
                        break;
                    }
                }
                foreach ($aVerificacao as $campo) {
                    unset($aResultado[$count][$campo]);
                }
                return $status;
            }
        }
    } else {
        return $aResultado = array();
    }
}

function listarCursosPropostoMEC($dados = array()) {
    global $db;

    if ($_SESSION['sisfor']['unicod']) {
        $docid = pegarDocidPlanejamento($_SESSION['sisfor']['unicod']);
        $esdid = pegarEstadoDocumento($docid);
    }

    if ($esdid == ESD_VALIDADO_MEC) {
        $planejamentofechado = true;
        $projeto = "";
    } else {
        $planejamentofechado = false;
        $projeto = "|| '<img src=\"../imagens/send.png\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"acessarCursoDireto('||sif.sifid||')\">'";
    }
    
    $perfil = pegarPerfil($_SESSION['usucpf']);

    $propmec = array('label' => 'Proposta MEC', 'colunas' => Array('Ano do Projeto', 'Vagas estimadas', 'Valor estimado (R$)'));
    $propies = array('label' => 'Proposta IES', 'colunas' => array('Vagas Propostas', 'LOA (R$)', 'Outras Fontes (R$)', 'Total (R$)'));

    $cabecalho = array('Ação', 'Secretaria responsável', 'Código Curso', 'Nome do curso', 'CPF', 'Coordenador Curso na IES', 'Opção', 'Justificativa', $propmec, $propies);

    $aryWhere[] = "(coo.coordsigla ILIKE '%SEB%' OR coo.coordsigla ILIKE '%SECADI%')";
    $aryWhere[] = "ieo.ieostatus = 'A'";
    $aryWhere[] = "abr.abcexibirsisfor = 't'";
    if($dados['siftipoplanejamento']) $aryWhere[] = "sif.siftipoplanejamento = '".$dados['siftipoplanejamento']."'";

    if ($_SESSION['sisfor']['unicod']) {
        $aryWhere[] = "ieo.unicod = '{$_SESSION['sisfor']['unicod']}'";
    }

    if ($dados['iuscpf']) {
        $aryWhere[] = "CASE WHEN adj.iuscpf is not null THEN sif.sifid=adj.sifid 
        					WHEN ger.iuscpf is not null THEN sif.sifid=ger.sifid 
        					ELSE ius.iuscpf = '{$dados['iuscpf']}' END";
    }

    if (in_array(PFL_ADMINISTRADOR, $perfil) || in_array(PFL_SUPER_USUARIO, $perfil)) {
        $acao = "CASE WHEN sif.sifid IS NULL THEN '<img border=\"0\" title=\"PROPOSTA IES\" src=\"../imagens/alterar.gif\" id=\"'|| ieo.ieoid ||'\" onclick=\"adicionarPropostaIES('|| ieo.ieoid ||','|| ieo.unicod ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
									 	 	 ELSE '<img border=\"0\" title=\"PROPOSTA IES\" src=\"../imagens/alterar.gif\" id=\"'|| sif.sifid ||'\" onclick=\"alterarPropostaIES('|| sif.sifid ||');\" style=\"cursor:pointer;\"/>&nbsp;
									 	 	 	   <img border=\"0\" title=\"EXCLUIR CURSO\" src=\"../imagens/excluir.gif\" id=\"'|| sif.sifid ||'\" onclick=\"excluirCursoMEC('|| sif.sifid ||');\" style=\"cursor:pointer;\"/>&nbsp;' END  || 
				'<img border=\"0\" title=\"COORDENADOR CURSO\"src=\"../imagens/usuario.gif\" id=\"' || ieo.ieoid ||'\" onclick=\"gerenciarCoordenadorCursoIEOID('|| ieo.unicod ||','|| ieo.ieoid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
				 <img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| cur.curid ||'\" onclick=\"imprimirCurso('|| cur.curid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' AS acao,";
    } elseif (in_array(PFL_CONSULTAGERAL, $perfil)) {
        $acao = "'<img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| cur.curid ||'\" onclick=\"imprimirCurso('|| cur.curid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' AS acao,";
    } elseif ($planejamentofechado || $esdid == WF_PLAN_ANALISE_MEC) {
        $acao = "'<img border=\"0\" title=\"COORDENADOR CURSO\"src=\"../imagens/usuario.gif\" id=\"' || ieo.ieoid ||'\" onclick=\"gerenciarCoordenadorCursoIEOID('|| ieo.unicod ||','|| ieo.ieoid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
				  <img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| cur.curid ||'\" onclick=\"imprimirCurso('|| cur.curid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' AS acao,";
    } elseif (in_array(PFL_EQUIPE_MEC, $perfil) || in_array(PFL_COORDENADOR_INST, $perfil)) {
        $acao = "CASE WHEN sif.sifid IS NULL THEN '<img border=\"0\" title=\"PROPOSTA IES\" src=\"../imagens/alterar.gif\" id=\"'|| ieo.ieoid ||'\" onclick=\"adicionarPropostaIES('|| ieo.ieoid ||','|| ieo.unicod ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
									 	 	 ELSE '<img border=\"0\" title=\"PROPOSTA IES\" src=\"../imagens/alterar.gif\" id=\"'|| sif.sifid ||'\" onclick=\"alterarPropostaIES('|| sif.sifid ||');\" style=\"cursor:pointer;\"/>&nbsp;' END || 
				 CASE WHEN sif.sifopcao <> '2' THEN '<img border=\"0\" title=\"COORDENADOR CURSO\"src=\"../imagens/usuario.gif\" id=\"' || ieo.ieoid ||'\" onclick=\"gerenciarCoordenadorCursoIEOID('|| ieo.unicod ||','|| ieo.ieoid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' ELSE '' END ||
				 '<img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| cur.curid ||'\" onclick=\"imprimirCurso('|| cur.curid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' AS acao,";
    }

    $sql = "SELECT 			DISTINCT " . (($dados['consulta']) ? "''," : $acao) . "
							coo.coordsigla,
							cur.curid || ' ' AS curid,
							cur.curdesc,
							substr(ius.iuscpf,1,3) || '.' || substr(ius.iuscpf,4,3)|| '.' || substr(ius.iuscpf,7,3) || '-' || substr(ius.iuscpf,10,2) AS iuscpf,
							--ius.iusnome as iusnome,
							CASE WHEN ger.iuscpf IS NOT NULL THEN ger.iusnome
    							 WHEN ius.iuscpf IS NOT NULL THEN ius.iusnome 
    							 ELSE adj.iusnome END $projeto as iusnome,
	    					CASE WHEN sif.sifopcao = '1' THEN 'Aceita'
    							 WHEN sif.sifopcao = '2' THEN 'Rejeita'
    							 WHEN sif.sifopcao = '3' THEN 'Repactua'
    							 ELSE '' END AS sifopcao,
    						CASE WHEN sif.sifjustificativa IS NOT NULL THEN '" . (($dados['consulta']) ? "-" : "<img border=\"0\" title=\"JUSTIFICATIVA\" src=\"../imagens/lista_azul.gif\" id=\"' || sif.sifjustificativa ||'\" onclick=\"abrirJustificativa('|| sif.sifid ||');\" style=\"cursor:pointer;\"/>") . "' ELSE ' - ' END AS justificativa,							
							ieo.ieoanoprojeto || ' ' AS ieoanoprojeto,
							ieo.ieoqtdvagas,
							COALESCE(ieo.valor_estimado,'0')::numeric(20,2) AS valor_estimado,
							COALESCE(sif.sifqtdvagas,0) AS sifqtdvagas,
							COALESCE(sifvalorloa,0)::numeric(20,2) AS sifvalorloa,
							COALESCE(sifvaloroutras,0)::numeric(20,2) AS sifvaloroutras,
							(COALESCE(sif.sifvalorloa,0) + COALESCE(sif.sifvaloroutras,0))::numeric(20,2) AS sifvalortotal
			FROM       	 	catalogocurso2014.iesofertante ieo
			INNER JOIN 		catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
			LEFT JOIN 		catalogocurso2014.abrangenciacurso abr ON abr.curid = cur.curid 
			INNER JOIN   	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
			LEFT JOIN		sisfor.sisfor sif ON sif.ieoid = ieo.ieoid AND sif.sifstatus = 'A'
			LEFT JOIN		sisfor.tipoperfil tpe ON tpe.tpeid = sif.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_CURSO . " 
			LEFT JOIN       (SELECT t2.pflcod, i2.iuscpf, t2.sifid, i2.iusnome FROM sisfor.tipoperfil t2 INNER JOIN sisfor.identificacaousuario i2 ON t2.iusd = i2.iusd AND sifid is not null AND t2.pflcod=".PFL_COORDENADOR_CURSO.") as ger ON ger.sifid = sif.sifid and ger.iuscpf='{$dados['iuscpf']}'
			LEFT JOIN       (SELECT t2.pflcod, i2.iuscpf, t2.sifid, i2.iusnome FROM sisfor.tipoperfil t2 INNER JOIN sisfor.identificacaousuario i2 ON t2.iusd = i2.iusd AND sifid is not null AND t2.pflcod=".PFL_COORDENADOR_ADJUNTO_IES.") as adj ON adj.sifid = sif.sifid and adj.iuscpf='{$dados['iuscpf']}'					
			LEFT JOIN		sisfor.identificacaousuario ius ON ius.iuscpf = sif.usucpf AND ius.iusstatus = 'A'
			LEFT JOIN		workflow.documento doc ON doc.docid = sif.docid
			LEFT JOIN		workflow.estadodocumento est ON est.esdid = doc.esdid
							" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "
			ORDER BY		coo.coordsigla, cur.curdesc";
    
    $param['totalLinhas'] = true;
    $tamanho = array('7%', '8%', '4%', '15%', '', '', '', '', '', '', '', '', '', '', '');
    $alinhamento = array('center', 'center', 'center', '', '', '', 'center', 'center', 'center', 'center', 'right', 'right', 'right', 'right', 'right', 'right');
    $db->monta_lista($sql, $cabecalho, '50', '10', 'S', 'center', 'S', '', $tamanho, $alinhamento, '', $param);
}

function excluirCursoMEC($sifid) {
    global $db;

    if ($sifid != '') {
        $sql = "UPDATE sisfor.sisfor SET sifstatus = 'I' WHERE sifid = {$sifid}";
    }

    if ($db->executar($sql)) {
        $db->commit();
    }
}

function salvarPropostaIES($post) {
    global $db;

    extract($post);

    $unitpocod = selecionarUnidade($unicod);
    $sifvalorloa = desformata_valor($sifvalorloa);
    $sifvaloroutras = desformata_valor($sifvaloroutras);
    $sifjustificativa = $sifjustificativa ? "'{$sifjustificativa}'" : "null";
    $sifopcao = $sifopcao ? "'{$sifopcao}'" : "null";

    if ($sifid) {
        $sql = "UPDATE 	sisfor.sisfor
   				SET 	sifqtdvagas = '{$sifqtdvagas}',
   						sifvalorloa = '{$sifvalorloa}',
   						sifvaloroutras = '{$sifvaloroutras}',
   						sifopcao = $sifopcao,
   						sifjustificativa = $sifjustificativa
 				WHERE   sifid = {$sifid}";
    } else {
        $sql = "INSERT INTO sisfor.sisfor (unicod,unitpocod,ieoid,sifstatus,sifqtdvagas,sifvalorloa,sifvaloroutras,sifopcao,sifjustificativa)
		 		VALUES ({$unicod},'{$unitpocod}',{$ieoid},'A',{$sifqtdvagas},'{$sifvalorloa}','{$sifvaloroutras}',{$sifopcao},$sifjustificativa) RETURNING sifid";
    }

    if ($db->executar($sql)) {
        $db->commit();
        $db->sucesso('principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies', '', 'Proposta IES cadastrada com sucesso!', 'S', 'S');
    } else {
        $db->insucesso('Não foi possível realizar a operação.', '', 'principal/coordenador/propostaies&acao=A');
    }
}

function visualizarPropostaIES($post) {
    global $db;

    extract($post);

    if ($sifid) {
        $aryWhere[] = "sifid = {$sifid}";
    }

    if ($ocuid) {
        $aryWhere[] = "ocuid = {$ocuid}";
    }

    if ($oatid) {
        $aryWhere[] = "oatid = {$oatid}";
    }

    if ($cnvid) {
        $aryWhere[] = "cnvid = {$cnvid}";
    }

    $sql = "SELECT  	ieoid,
    					cnvid, 
    					ocuid,
    					oatid,
    					sifid,
    					sifqtdvagas,
    					CASE WHEN (sifvalorloa = '0.00' OR sifvalorloa IS NULL) THEN '0,00' ELSE TRIM(TO_CHAR(sifvalorloa,'9G999G999D99')) END AS sifvalorloa,
    					CASE WHEN (sifvaloroutras = '0.00' OR sifvaloroutras IS NULL) THEN '0,00' ELSE TRIM(TO_CHAR(sifvaloroutras,'9G999G999D99')) END AS sifvaloroutras,
    					sifopcao,
    					sifjustificativa
    		FROM 		sisfor.sisfor
    					" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";

    $propostaies = $db->pegaLinha($sql);

    if ($propostaies['ieoid']) {
        $sql = "SELECT 		cur.curdesc, cur.coordid
    			FROM 		catalogocurso2014.iesofertante ieo
				INNER JOIN 	catalogocurso2014.curso cur ON ieo.curid = cur.curid 
				LEFT JOIN   catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
				WHERE 		ieo.ieoid = {$propostaies['ieoid']}";

        $curso = $db->pegaLinha($sql);

        $propostaies['curso'] = $curso['curdesc'];
        $propostaies['coordenacao'] = $curso['coordid'];
    }

    if ($propostaies['cnvid']) {
        $sql = "SELECT 		cur.curdesc, cur.coordid
    			FROM 		sisfor.cursonaovinculado cnv
				INNER JOIN 	catalogocurso2014.curso cur ON cnv.curid = cur.curid
				LEFT JOIN   catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
				WHERE		cnv.cnvid = {$propostaies['cnvid']}";

        $curso = $db->pegaLinha($sql);
        $propostaies['curso'] = $curso['curdesc'];
        $propostaies['coordenacao'] = $curso['coordid'];
    }

    if ($propostaies['ocuid']) {
        $sql = "SELECT	   out.ocunome, out.coordid
    			FROM	   sisfor.outrocurso out
    			LEFT JOIN  catalogocurso2014.coordenacao coo ON coo.coordid = out.coordid
    			WHERE	   ocuid = {$propostaies['ocuid']}";

        $curso = $db->pegaLinha($sql);
        $propostaies['curso'] = $curso['ocunome'];
        $propostaies['coordenacao'] = $curso['coordid'];
    }

    if ($propostaies['oatid']) {
        $sql = "SELECT	   oat.oatnome, oat.coordid
    			FROM	   sisfor.outraatividade oat
    			LEFT JOIN  catalogocurso2014.coordenacao coo ON coo.coordid = oat.coordid
    			WHERE	   oatid = {$propostaies['oatid']}";

        $curso = $db->pegaLinha($sql);
        $propostaies['curso'] = $curso['oatnome'];
        $propostaies['coordenacao'] = $curso['coordid'];
    }

    return $propostaies;
}

function listarCursosPropostoIES($dados = array(),$siftipoplanejamento = NULL) {
    global $db;

    if ($_SESSION['sisfor']['unicod']) {
        $docid = pegarDocidPlanejamento($_SESSION['sisfor']['unicod']);
        $esdid = pegarEstadoDocumento($docid);
        
        $docidplan2 = pegarDocidPlanejamento2($_SESSION['sisfor']['unicod']);
        $esdidplan2 = pegarEstadoDocumento($docidplan2);              
    }

    if($siftipoplanejamento == FASE02){
	    if ($esdidplan2 == WF_PLAN_FECHADO2) {
	        $planejamentofechado = true;
	    } else {
	        $planejamentofechado = false;
	    }
	    $projeto = "|| '<img src=\"../imagens/send.png\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"acessarCursoDireto('||sif.sifid||')\">'";
    } else {
    	if ($esdid == ESD_VALIDADO_MEC) {    			   
	        $planejamentofechado = true;
	    } else {
	        $planejamentofechado = false;
	    }    	
	    
	    if ($esdid == ESD_VALIDADO_MEC) {    			   
		    $projeto = "";
		} else {
			$projeto = "|| '<img src=\"../imagens/send.png\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"acessarCursoDireto('||sif.sifid||')\">'";
		}  	    
    }

    $propies = array('label' => 'Proposta IES', 'colunas' => array('Vagas Propostas', 'LOA (R$)', 'Outras Fontes (R$)', 'Total (R$)'));

    $cabecalho = array('Ação', 'Tipo de Opção', 'Secretaria responsável', 'Nome do curso', 'Coordenador Curso na IES', $propies);

    $aryWhere[] = "(sif.cnvid IS NOT NULL OR sif.ocuid IS NOT NULL)";
    $aryWhere[] = "sif.sifstatus = 'A'";

    if ($siftipoplanejamento) {
        $aryWhere[] = "sif.siftipoplanejamento = {$siftipoplanejamento}";
    }     
    
    if ($_SESSION['sisfor']['unicod']) {
        $aryWhere[] = "sif.unicod = '{$_SESSION['sisfor']['unicod']}'";
    }

    if ($_SESSION['usucpf']) {
        $perfil = pegarPerfil($_SESSION['usucpf']);
    }

    if ($dados['iuscpf']) {
        $aryWhere[] = "CASE WHEN adj.iuscpf is not null THEN sif.sifid=adj.sifid 
        					WHEN ger.iuscpf is not null THEN sif.sifid=ger.sifid 
        					ELSE ius.iuscpf = '{$dados['iuscpf']}' END";
    }

    if (in_array(PFL_ADMINISTRADOR, $perfil) || in_array(PFL_SUPER_USUARIO, $perfil) || (in_array(PFL_COORDENADOR_INST, $perfil) && $esdid <> ESD_VALIDADO_MEC)) {
    	
        $acao = "CASE WHEN sif.ocuid IS NOT NULL THEN '<img border=\"0\" title=\"ALTERAR CURSO\" src=\"../imagens/alterar.gif\" id=\"'|| sif.sifid ||'\" onclick=\"selecionarOutroCurso('|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
													   <img border=\"0\" title=\"EXCLUIR CURSO\" src=\"../imagens/excluir.gif\" id=\"'|| sif.ocuid ||'\" onclick=\"excluirCursoOutros('|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' 
					  WHEN sif.cnvid IS NOT NULL THEN '<img border=\"0\" title=\"ALTERAR CURSO\" src=\"../imagens/alterar.gif\" id=\"'|| sif.cnvid ||'\" onclick=\"selecionarCursoNVinculado('|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
					  								   <img border=\"0\" title=\"EXCLUIR CURSO\" src=\"../imagens/excluir.gif\" id=\"'|| sif.cnvid ||'\" onclick=\"excluirCursoCatalogo('|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' END ||
				 CASE WHEN sif.ocuid IS NOT NULL THEN '<img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| sif.ocuid ||'\" onclick=\"imprimirCursoOCUID('|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
					  WHEN sif.cnvid IS NOT NULL THEN '<img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| sif.cnvid ||'\" onclick=\"imprimirCursoCNVID('|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' END ||
				 CASE WHEN sif.ocuid IS NOT NULL THEN '<img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || sif.sifid ||'\" onclick=\"gerenciarCoordenadorCursoOCUID('|| sif.unicod ||','|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' 
				 	  WHEN sif.cnvid IS NOT NULL THEN '<img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || sif.sifid ||'\" onclick=\"gerenciarCoordenadorCursoCNVID('|| sif.unicod ||','|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
				 END  ||
				 CASE WHEN ocu.arqid IS NOT NULL THEN '<a href=\"sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A&download=S&arqid='|| ocu.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>&nbsp;&nbsp;' ELSE '' END AS acao,";
    } elseif (in_array(PFL_CONSULTAGERAL, $perfil)) {

        $acao = "CASE WHEN sif.ocuid IS NOT NULL THEN '<img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| sif.ocuid ||'\" onclick=\"imprimirCursoOCUID('|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
					  WHEN sif.cnvid IS NOT NULL THEN '<img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| sif.cnvid ||'\" onclick=\"imprimirCursoCNVID('|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' END AS acao,";
    } elseif ($planejamentofechado || $esdid == ESD_VALIDADO_MEC) {
		if( $siftipoplanejamento != FASE02){
	        $acao = "CASE WHEN sif.ocuid IS NOT NULL THEN '<img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| sif.ocuid ||'\" onclick=\"imprimirCursoOCUID('|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
						  WHEN sif.cnvid IS NOT NULL THEN '<img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| sif.cnvid ||'\" onclick=\"imprimirCursoCNVID('|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' END ||
					 CASE WHEN sif.ocuid IS NOT NULL THEN '<img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || sif.sifid ||'\" onclick=\"gerenciarCoordenadorCursoOCUID('|| sif.unicod ||','|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' 
					 	  WHEN sif.cnvid IS NOT NULL THEN '<img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || sif.sifid ||'\" onclick=\"gerenciarCoordenadorCursoCNVID('|| sif.unicod ||','|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
					 END  ||
					 CASE WHEN ocu.arqid IS NOT NULL THEN '<a href=\"sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A&download=S&arqid='|| ocu.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>&nbsp;&nbsp;' ELSE '' END AS acao,";
		}else{
			$acao = "CASE WHEN sif.ocuid IS NOT NULL THEN '<img border=\"0\" title=\"ALTERAR CURSO\" src=\"../imagens/alterar.gif\" id=\"'|| sif.sifid ||'\" onclick=\"selecionarOutroCurso('|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
													   <img border=\"0\" title=\"EXCLUIR CURSO\" src=\"../imagens/excluir.gif\" id=\"'|| sif.ocuid ||'\" onclick=\"excluirCursoOutros('|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
					  WHEN sif.cnvid IS NOT NULL THEN '<img border=\"0\" title=\"ALTERAR CURSO\" src=\"../imagens/alterar.gif\" id=\"'|| sif.cnvid ||'\" onclick=\"selecionarCursoNVinculado('|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
					  								   <img border=\"0\" title=\"EXCLUIR CURSO\" src=\"../imagens/excluir.gif\" id=\"'|| sif.cnvid ||'\" onclick=\"excluirCursoCatalogo('|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' END ||
				 CASE WHEN sif.ocuid IS NOT NULL THEN '<img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| sif.ocuid ||'\" onclick=\"imprimirCursoOCUID('|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
					  WHEN sif.cnvid IS NOT NULL THEN '<img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| sif.cnvid ||'\" onclick=\"imprimirCursoCNVID('|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' END ||
				 CASE WHEN sif.ocuid IS NOT NULL THEN '<img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || sif.sifid ||'\" onclick=\"gerenciarCoordenadorCursoOCUID('|| sif.unicod ||','|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
				 	  WHEN sif.cnvid IS NOT NULL THEN '<img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || sif.sifid ||'\" onclick=\"gerenciarCoordenadorCursoCNVID('|| sif.unicod ||','|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
				 END  ||
				 CASE WHEN ocu.arqid IS NOT NULL THEN '<a href=\"sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A&download=S&arqid='|| ocu.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>&nbsp;&nbsp;' ELSE '' END AS acao,";
			
		}
	} elseif (in_array(PFL_EQUIPE_MEC, $perfil)) {
        $acao = "CASE WHEN sif.ocuid IS NOT NULL THEN '<img border=\"0\" title=\"ALTERAR CURSO\" src=\"../imagens/alterar.gif\" id=\"'|| sif.sifid ||'\" onclick=\"selecionarOutroCurso('|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
					  WHEN sif.cnvid IS NOT NULL THEN '<img border=\"0\" title=\"ALTERAR CURSO\" src=\"../imagens/alterar.gif\" id=\"'|| sif.cnvid ||'\" onclick=\"selecionarCursoNVinculado('|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' END ||
				 CASE WHEN sif.ocuid IS NOT NULL THEN '<img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| sif.ocuid ||'\" onclick=\"imprimirCursoOCUID('|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
					  WHEN sif.cnvid IS NOT NULL THEN '<img border=\"0\" title=\"DADOS CURSO\" src=\"../imagens/print.gif\" id=\"'|| sif.cnvid ||'\" onclick=\"imprimirCursoCNVID('|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' END ||
				 CASE WHEN sif.ocuid IS NOT NULL THEN '<img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || sif.sifid ||'\" onclick=\"gerenciarCoordenadorCursoOCUID('|| sif.unicod ||','|| sif.ocuid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' 
				 	  WHEN sif.cnvid IS NOT NULL THEN '<img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || sif.sifid ||'\" onclick=\"gerenciarCoordenadorCursoCNVID('|| sif.unicod ||','|| sif.cnvid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;'
				 END  ||
				 CASE WHEN ocu.arqid IS NOT NULL THEN '<a href=\"sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A&download=S&arqid='|| ocu.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>&nbsp;&nbsp;' ELSE '' END AS acao,";
    }

    $sql = "SELECT			" . (($dados['consulta']) ? "'' AS acao," : $acao) . "
							CASE WHEN sif.ocuid IS NOT NULL THEN  'Fora Catálogo'
						    	 WHEN sif.cnvid IS NOT NULL THEN  'Catálogo' END AS tipo_opcao,
					        CASE WHEN sif.ocuid IS NOT NULL THEN  ocu.sigla
						    	 WHEN sif.cnvid IS NOT NULL THEN  cnv.sigla END AS sigla,
					        CASE WHEN sif.ocuid IS NOT NULL THEN  ocu.nome
						    	 WHEN sif.cnvid IS NOT NULL THEN  cnv.nome END AS nome,
							ius.iusnome $projeto,
							COALESCE(sif.sifqtdvagas,0) AS sifqtdvagas,
							COALESCE(sifvalorloa,0)::numeric(20,2) AS sifvalorloa,
							COALESCE(sifvaloroutras,0)::numeric(20,2) AS sifvaloroutras,
							(COALESCE(sif.sifvalorloa,0) + COALESCE(sif.sifvaloroutras,0))::numeric(20,2) AS sifvalortotal
			FROM  			sisfor.sisfor sif
			LEFT JOIN 		(SELECT 		coo.coordsigla AS sigla, cur.curdesc AS nome, cnv.cnvid 
							 FROM 			sisfor.cursonaovinculado cnv 
							 INNER JOIN 	catalogocurso2014.curso cur ON cnv.curid = cur.curid
							 INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
							 WHERE			cnv.cnvstatus = 'A') AS cnv ON cnv.cnvid = sif.cnvid
			LEFT JOIN 		(SELECT 		coo.coordsigla AS sigla, ocu.ocunome AS nome, ocu.ocuid, ocu.arqid
							 FROM 			sisfor.outrocurso ocu
							 INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = ocu.coordid
							 LEFT JOIN   	public.arquivo arq ON arq.arqid = ocu.arqid
							 WHERE			ocu.ocustatus = 'A') AS ocu ON ocu.ocuid = sif.ocuid
			LEFT JOIN 		sisfor.tipoperfil tpe ON tpe.tpeid = sif.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_CURSO . " 
			LEFT JOIN       (SELECT t2.pflcod, i2.iuscpf, t2.sifid FROM sisfor.tipoperfil t2 INNER JOIN sisfor.identificacaousuario i2 ON t2.iusd = i2.iusd AND sifid is not null AND t2.pflcod=".PFL_COORDENADOR_ADJUNTO_IES.") as adj ON adj.sifid = sif.sifid and adj.iuscpf='{$dados['iuscpf']}'
			LEFT JOIN       (SELECT t2.pflcod, i2.iuscpf, t2.sifid, i2.iusnome FROM sisfor.tipoperfil t2 INNER JOIN sisfor.identificacaousuario i2 ON t2.iusd = i2.iusd AND sifid is not null AND t2.pflcod=".PFL_COORDENADOR_CURSO.") as ger ON ger.sifid = sif.sifid and ger.iuscpf='{$dados['iuscpf']}'
			LEFT JOIN		sisfor.identificacaousuario ius ON ius.iuscpf = sif.usucpf AND ius.iusstatus = 'A'
			LEFT JOIN		workflow.documento doc ON doc.docid = sif.docid
			LEFT JOIN		workflow.estadodocumento est ON est.esdid = doc.esdid			
							" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "
			ORDER BY		2,3";

    $param['totalLinhas'] = true;
    $param['ordena'] = false;
    $tamanho = array('9%', '', '', '', '', '', '', '', '', '', '', '', '');
    $alinhamento = array('center', 'center', 'center', '', 'center', 'center', 'right', 'right', 'right', 'right');
    $db->monta_lista($sql, $cabecalho, '50', '10', 'S', 'center', 'S', '', $tamanho, $alinhamento, '', $param);
    
}

function validaCoordenadoresFase2() {
	global $db;

	$unicod = $_SESSION['sisfor']['unicod'];
	if (!$unicod) return false;
	
	if ($unicod) {
		$docid = pegarDocidPlanejamento($unicod);
		$esdid = pegarEstadoDocumento($docid);

		$docidplan2 = pegarDocidPlanejamento2($unicod);
		$esdidplan2 = pegarEstadoDocumento($docidplan2);
	}

	$siftipoplanejamento = FASE02;
	
	if($siftipoplanejamento == FASE02){
		if ($esdidplan2 == WF_PLAN_FECHADO2) {
			$planejamentofechado = true;
		} else {
			$planejamentofechado = false;
		}
	} else {
		if ($esdid == WF_PLAN_FECHADO) {
			$planejamentofechado = true;
		} else {
			$planejamentofechado = false;
		}
	}

	$aryWhere[] = "(sif.cnvid IS NOT NULL OR sif.ocuid IS NOT NULL)";
	$aryWhere[] = "sif.sifstatus = 'A'";

	if ($siftipoplanejamento) {
		$aryWhere[] = "sif.siftipoplanejamento = {$siftipoplanejamento}";
	}

	$aryWhere[] = "sif.unicod = '{$unicod}'";

	$sql = "SELECT			
							ius.iusnome
			FROM  			sisfor.sisfor sif
			LEFT JOIN 		(SELECT 		coo.coordsigla AS sigla, cur.curdesc AS nome, cnv.cnvid
							 FROM 			sisfor.cursonaovinculado cnv
							 INNER JOIN 	catalogocurso2014.curso cur ON cnv.curid = cur.curid
							 INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
							 WHERE			cnv.cnvstatus = 'A') AS cnv ON cnv.cnvid = sif.cnvid
			LEFT JOIN 		(SELECT 		coo.coordsigla AS sigla, ocu.ocunome AS nome, ocu.ocuid, ocu.arqid
							 FROM 			sisfor.outrocurso ocu
							 INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = ocu.coordid
							 LEFT JOIN   	public.arquivo arq ON arq.arqid = ocu.arqid
							 WHERE			ocu.ocustatus = 'A') AS ocu ON ocu.ocuid = sif.ocuid
			LEFT JOIN 		sisfor.tipoperfil tpe ON tpe.tpeid = sif.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_CURSO . "
			LEFT JOIN       (SELECT t2.pflcod, i2.iuscpf, t2.sifid FROM sisfor.tipoperfil t2 INNER JOIN sisfor.identificacaousuario i2 ON t2.iusd = i2.iusd AND sifid is not null AND t2.pflcod=".PFL_COORDENADOR_ADJUNTO_IES.") as adj ON adj.sifid = sif.sifid and adj.iuscpf='{$dados['iuscpf']}'
			LEFT JOIN		sisfor.identificacaousuario ius ON ius.iuscpf = sif.usucpf AND ius.iusstatus = 'A'
			LEFT JOIN		workflow.documento doc ON doc.docid = sif.docid
			LEFT JOIN		workflow.estadodocumento est ON est.esdid = doc.esdid
			" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "
				and ius.iusnome is null
			";

	$boCoordenador = $db->carregar($sql);
	
	if( $boCoordenador && count($boCoordenador ) > 0 ){
		
		return false;
		
	}
	return true;
}

function pegaValorNaoAplicadoFase2(){
	
	global $db;
	
	$siftipoplanejamento = FASE02;
	
	 
  	$aryWhere[] = "(sif.cnvid IS NOT NULL OR sif.ocuid IS NOT NULL)";
    $aryWhere[] = "sif.sifstatus = 'A'";

 	if ($siftipoplanejamento) {
        $aryWhere[] = "sif.siftipoplanejamento = {$siftipoplanejamento}";
    }     
    
   
    
    if ($_SESSION['sisfor']['unicod']) {
        $aryWhere[] = "sif.unicod = '{$_SESSION['sisfor']['unicod']}'";
    }

    if ($_SESSION['usucpf']) {
        $perfil = pegarPerfil($_SESSION['usucpf']);
    }

//     if ($dados['iuscpf']) {
//         $aryWhere[] = "CASE WHEN adj.iuscpf is not null THEN sif.sifid=adj.sifid 
//         					WHEN ger.iuscpf is not null THEN sif.sifid=ger.sifid 
//         					ELSE ius.iuscpf = '{$dados['iuscpf']}' END";
//     }
	//-- 
	$aryWhere2[] = "sif.sifstatus = 'A'  and oat.oatstatus = 'A' ";

	
	if ($_SESSION['sisfor']['unicod']) {
		$aryWhere2[] = "sif.unicod = '{$_SESSION['sisfor']['unicod']}'";
	}
	
	if ($_SESSION['usucpf']) {
		$perfil = pegarPerfil($_SESSION['usucpf']);
	}
	
	if ($siftipoplanejamento) {
		$aryWhere2[] = "sif.siftipoplanejamento = {$siftipoplanejamento}";
	}
	
	$sql = "SELECT			sum( (COALESCE(sif.sifvalorloa,0) )) AS sifvalortotal
			FROM  			sisfor.sisfor sif
			LEFT JOIN 		(SELECT 		coo.coordsigla AS sigla, cur.curdesc AS nome, cnv.cnvid
							 FROM 			sisfor.cursonaovinculado cnv
							 INNER JOIN 	catalogocurso2014.curso cur ON cnv.curid = cur.curid
							 INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
							 WHERE			cnv.cnvstatus = 'A') AS cnv ON cnv.cnvid = sif.cnvid
			LEFT JOIN 		(SELECT 		coo.coordsigla AS sigla, ocu.ocunome AS nome, ocu.ocuid, ocu.arqid
							 FROM 			sisfor.outrocurso ocu 
							 INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = ocu.coordid
							 LEFT JOIN   	public.arquivo arq ON arq.arqid = ocu.arqid
							 WHERE			ocu.ocustatus = 'A') AS ocu ON ocu.ocuid = sif.ocuid
			LEFT JOIN 		sisfor.tipoperfil tpe ON tpe.tpeid = sif.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_CURSO . "
			LEFT JOIN       (SELECT t2.pflcod, i2.iuscpf, t2.sifid FROM sisfor.tipoperfil t2 INNER JOIN sisfor.identificacaousuario i2 ON t2.iusd = i2.iusd AND sifid is not null AND t2.pflcod=".PFL_COORDENADOR_ADJUNTO_IES.") as adj ON adj.sifid = sif.sifid and adj.iuscpf='{$dados['iuscpf']}'
				LEFT JOIN		sisfor.identificacaousuario ius ON ius.iuscpf = sif.usucpf AND ius.iusstatus = 'A'
				LEFT JOIN		workflow.documento doc ON doc.docid = sif.docid
				LEFT JOIN		workflow.estadodocumento est ON est.esdid = doc.esdid
				" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "

						";
	
	$sifvalortotal = $db->pegaUm( $sql );
	
	
	 $sql2 = "SELECT 		
							sum( COALESCE(sifvalorloa,0)::numeric(20,2) ) AS sifvalorloa
			FROM       	 	sisfor.sisfor sif
			LEFT JOIN		sisfor.outraatividade oat ON sif.oatid = oat.oatid
			LEFT JOIN 		sisfor.tipoperfil tpe ON tpe.tpeid = sif.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_CURSO . "
			LEFT JOIN		sisfor.identificacaousuario ius ON ius.iuscpf = sif.usucpf AND ius.iusstatus = 'A'
			LEFT JOIN		workflow.documento doc ON doc.docid = sif.docid
			LEFT JOIN		workflow.estadodocumento est ON est.esdid = doc.esdid
			LEFT JOIN 		catalogocurso2014.coordenacao coo ON coo.coordid = oat.coordid
							" . (is_array($aryWhere2) ? ' WHERE ' . implode(' AND ', $aryWhere2) : '') . "
			
									";
	
	$totalloaoutras = $db->pegaUm( $sql2 );

	$valor = $sifvalortotal + $totalloaoutras;
	
	return $valor;
}

function listarAtividadesPropostaIES($siftipoplanejamento = NULL) {
    global $db;
    
    if ($_SESSION['usucpf']) {
        $perfil = pegarPerfil($_SESSION['usucpf']);
    }    
    
    if($siftipoplanejamento == FASE02){
        if ($_SESSION['sisfor']['unicod']) {
	        $docidplan2 = pegarDocidPlanejamento2($_SESSION['sisfor']['unicod']);
    	    $esdidplan2 = pegarEstadoDocumento($docidplan2);        
    	}    	
            	
	    if($esdidplan2 == ESD_VALIDADO_MEC2) {
	        $planejamentofechado = true;
	        $projeto = "";
	    } else {
	        $planejamentofechado = false;
	        $projeto = "|| '<img src=\"../imagens/send.png\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"acessarCursoDireto('||sif.sifid||')\">'";
	    }
	    
	    $propies = array('label' => 'Proposta IES', 'colunas' => array('Ano Proposta', 'LOA (R$)','Outras Fontes (R$)'));
    	//$acao = "'<img id=\"img_dimensao_' || sif.sifid  || '\" src=\"/imagens/mais.gif\" style=\"cursor: pointer\" onclick=\"carregarListaCustoAtividade(this.id,' || sif.sifid  || ');\"/> '||";
        $detalhe = "";
        $alinhamento = array('left', 'center', '', '', '', 'right','right');	    

        if (in_array(PFL_ADMINISTRADOR, $perfil) || in_array(PFL_SUPER_USUARIO, $perfil) || (in_array(PFL_COORDENADOR_INST, $perfil) && $esdidplan2 == ESD_EM_ELABORACAO2)) {
	        $acao .= "'<img border=\"0\" title=\"ALTERAR ATIVIDADE\" src=\"../imagens/alterar.gif\" id=\"'|| sif.sifid ||'\" onclick=\"selecionarAtividade('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
			 		  <img border=\"0\" title=\"EXCLUIR ATIVIDADE\" src=\"../imagens/excluir.gif\" id=\"'|| oat.oatid ||'\" onclick=\"excluirAtividade('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
					  <img border=\"0\" title=\"DADOS ATIVIDADE\" src=\"../imagens/print.gif\" id=\"'|| oat.oatid ||'\" onclick=\"imprimirCursoOATID('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
					  <img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || oat.oatid ||'\" onclick=\"gerenciarCoordenadorAtividadeOATID('|| sif.unicod ||','|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' AS acao1,";
	    } elseif (in_array(PFL_CONSULTAGERAL, $perfil)) {
	        $acao .= "'<img border=\"0\" title=\"DADOS ATIVIDADE\" src=\"../imagens/print.gif\" id=\"'|| oat.oatid ||'\" onclick=\"imprimirCursoOATID('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' AS acao,";
	    } elseif ($planejamentofechado || $esdidplan2 == ESD_VALIDADO_MEC2) {
	        $acao .= "'<img border=\"0\" title=\"DADOS ATIVIDADE\" src=\"../imagens/print.gif\" id=\"'|| oat.oatid ||'\" onclick=\"imprimirCursoOATID('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
					  <img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || oat.oatid ||'\" onclick=\"gerenciarCoordenadorAtividadeOATID('|| sif.unicod ||','|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' AS acao2,";
	    } elseif (in_array(PFL_EQUIPE_MEC, $perfil)) {
	        $acao .= "'<img border=\"0\" title=\"ALTERAR ATIVIDADE\" src=\"../imagens/alterar.gif\" id=\"'|| sif.sifid ||'\" onclick=\"selecionarAtividade('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
					  <img border=\"0\" title=\"DADOS ATIVIDADE\" src=\"../imagens/print.gif\" id=\"'|| oat.oatid ||'\" onclick=\"imprimirCursoOATID('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
					  <img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || oat.oatid ||'\" onclick=\"gerenciarCoordenadorAtividadeOATID('|| sif.unicod ||','|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' AS acao3,";
	    } elseif(in_array(PFL_COORDENADOR_CURSO, $perfil) || in_array(PFL_COORDENADOR_ADJUNTO_IES, $perfil)) {
	    	$acao .= "";
	    }    	        
	    
    } else {
	    if ($_SESSION['sisfor']['unicod']) {
	        $docid = pegarDocidPlanejamento($_SESSION['sisfor']['unicod']);
	        $esdid = pegarEstadoDocumento($docid);    	
	    }    
    	
    	if ($esdid == ESD_VALIDADO_MEC) {    			   
	        $planejamentofechado = true;
	        $projeto = "";
	    } else {
	        $planejamentofechado = false;
	        $projeto = "|| '<img src=\"../imagens/send.png\" style=\"cursor:pointer;\" align=\"absmiddle\" onclick=\"acessarCursoDireto('||sif.sifid||')\">'";
	    }    	

	    $propies = array('label' => 'Proposta IES', 'colunas' => array('Ano Proposta', 'LOA (R$)'));
    	$acao = "";
    	$detalhe = "";
    	$alinhamento = array('center', 'center', '', '', '', 'right');	

        if (in_array(PFL_ADMINISTRADOR, $perfil) || in_array(PFL_SUPER_USUARIO, $perfil) || (in_array(PFL_COORDENADOR_INST, $perfil) && $esdid <> ESD_VALIDADO_MEC)) {
	        $acao .= "'<img border=\"0\" title=\"ALTERAR ATIVIDADE\" src=\"../imagens/alterar.gif\" id=\"'|| sif.sifid ||'\" onclick=\"selecionarAtividade('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
			 		  <img border=\"0\" title=\"EXCLUIR ATIVIDADE\" src=\"../imagens/excluir.gif\" id=\"'|| oat.oatid ||'\" onclick=\"excluirAtividade('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
					  <img border=\"0\" title=\"DADOS ATIVIDADE\" src=\"../imagens/print.gif\" id=\"'|| oat.oatid ||'\" onclick=\"imprimirCursoOATID('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
					  <img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || oat.oatid ||'\" onclick=\"gerenciarCoordenadorAtividadeOATID('|| sif.unicod ||','|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' AS acao,";
	    } elseif (in_array(PFL_CONSULTAGERAL, $perfil)) {
	        $acao .= "'<img border=\"0\" title=\"DADOS ATIVIDADE\" src=\"../imagens/print.gif\" id=\"'|| oat.oatid ||'\" onclick=\"imprimirCursoOATID('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' AS acao,";
	    } elseif ($planejamentofechado || $esdid == ESD_VALIDADO_MEC) {
	        $acao .= "'<img border=\"0\" title=\"DADOS ATIVIDADE\" src=\"../imagens/print.gif\" id=\"'|| oat.oatid ||'\" onclick=\"imprimirCursoOATID('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
					  <img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || oat.oatid ||'\" onclick=\"gerenciarCoordenadorAtividadeOATID('|| sif.unicod ||','|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' AS acao,";
	    } elseif (in_array(PFL_EQUIPE_MEC, $perfil)) {
	        $acao .= "'<img border=\"0\" title=\"ALTERAR ATIVIDADE\" src=\"../imagens/alterar.gif\" id=\"'|| sif.sifid ||'\" onclick=\"selecionarAtividade('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
					  <img border=\"0\" title=\"DADOS ATIVIDADE\" src=\"../imagens/print.gif\" id=\"'|| oat.oatid ||'\" onclick=\"imprimirCursoOATID('|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
					  <img border=\"0\" title=\"COORDENADOR CURSO\" src=\"../imagens/usuario.gif\" id=\"' || oat.oatid ||'\" onclick=\"gerenciarCoordenadorAtividadeOATID('|| sif.unicod ||','|| oat.oatid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' AS acao,";
	    } elseif(in_array(PFL_COORDENADOR_CURSO, $perfil) || in_array(PFL_COORDENADOR_ADJUNTO_IES, $perfil)) {
	    	$acao .= "";
	    }    	
    }
    
    if ($siftipoplanejamento ) {
        $aryWhere[] = "sif.siftipoplanejamento = {$siftipoplanejamento}";
    }    

    $cabecalho = array('Ação', 'Secretaria responsável', 'Nome da atividade', 'Coordenador Atividade na IES', $propies);

    $aryWhere[] = "oat.oatstatus = 'A'";

    if ($_SESSION['sisfor']['unicod']) {
        $aryWhere[] = "sif.unicod = '{$_SESSION['sisfor']['unicod']}'";
    } else {
    	$aryWhere[] = "sif.usucpf='".$_SESSION['usucpf']."'";
    }

    $sql = "SELECT 			DISTINCT $acao
							coo.coordsigla,
							oat.oatnome,
							ius.iusnome $projeto,
							oat.oatanoproposta || ' ' AS oatanoproposta,
							COALESCE(sifvalorloa,0)::numeric(20,2) AS sifvalorloa,
							COALESCE(sifvaloroutras,0)::numeric(20,2) AS sifvaloroutras
							$detalhe
			FROM       	 	sisfor.sisfor sif
			LEFT JOIN		sisfor.outraatividade oat ON sif.oatid = oat.oatid
			LEFT JOIN 		sisfor.tipoperfil tpe ON tpe.tpeid = sif.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_CURSO . "
			LEFT JOIN		sisfor.identificacaousuario ius ON ius.iuscpf = sif.usucpf AND ius.iusstatus = 'A'
			LEFT JOIN		workflow.documento doc ON doc.docid = sif.docid
			LEFT JOIN		workflow.estadodocumento est ON est.esdid = doc.esdid
			LEFT JOIN 		catalogocurso2014.coordenacao coo ON coo.coordid = oat.coordid
							" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "
			ORDER BY		coo.coordsigla,oat.oatnome";
  
    $param['totalLinhas'] = true;
    $param['ordena']      = false;
    $db->monta_lista($sql, $cabecalho, '50', '10', 'S', 'center', 'S', '', '', $alinhamento, null, $param);
}

function recuperarCoordenadorIES($iusd) {
    global $db;

    $sql = "SELECT 			unidsc || ' - ' || ius.iusnome AS cies
			FROM 			sisfor.identificacaousuario ius
			INNER JOIN		sisfor.sisfories sie ON ius.iuscpf = sie.usucpf
			LEFT JOIN 		sisfor.tipoperfil tpe ON tpe.tpeid = sie.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_INST . "
			INNER JOIN 		public.unidade uni ON uni.unicod = sie.unicod
			WHERE 			ius.iusd = {$iusd}";

    $cies = $db->pegaUm($sql);
    return $cies;
}

function excluirArq($arqid) {
    global $db;

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec("outrocurso", array(), "sisfor");
    $file->excluiArquivoFisico($arqid);

    $sql = "UPDATE  sisfor.outrocurso
   			SET 	arqid = null
 			WHERE 	arqid = {$arqid}";

    if ($db->executar($sql)) {
        $db->commit();
        echo 'S';
    } else {
        echo 'N';
    }
}

function recuperarPropostaMEC($post) {
    global $db;

    extract($post);

    if ($sifid) {
        $aryWhere[] = "sif.sifid = {$sifid}";
    }

    if ($ieoid) {
        $aryWhere[] = "ieo.ieoid = {$ieoid}";
    }

    $sql = "SELECT		ieo.ieoqtdvagas,
						CASE WHEN (ieo.valor_estimado = '0.00' OR ieo.valor_estimado IS NULL) THEN '0,00' ELSE TRIM(TO_CHAR(ieo.valor_estimado,'9G999G999D99')) END AS valor_estimado
			FROM   	 	catalogocurso2014.iesofertante ieo
			LEFT JOIN	sisfor.sisfor sif ON sif.ieoid = ieo.ieoid
						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";

    $proposta = $db->pegaLinha($sql);
    $proposta["ieoqtdvagas"] = iconv("ISO-8859-1", "UTF-8", $proposta["ieoqtdvagas"]);
    $proposta["valor_estimado"] = iconv("ISO-8859-1", "UTF-8", $proposta["valor_estimado"]);

    echo simec_json_encode($proposta);
}

function pegarDocid($sifid) {
    global $db;

    $sifid = (integer) $sifid;
    $sql = "SELECT docid FROM sisfor.sisfor WHERE sifid = {$sifid}";
    return (integer) $db->pegaUm($sql);
}

function pegarDocidPlanejamento($unicod) {
    global $db;

    $sql = "SELECT docid FROM sisfor.sisfories WHERE unicod = '{$unicod}'";
    $docid = $db->pegaUm($sql);
    return trim($docid);
}

function pegarEstadoDocumento($docid) {
    global $db;
    if ($docid) {
        $sql = "SELECT		ed.esdid
				FROM		workflow.documento d
				INNER JOIN	workflow.estadodocumento ed ON ed.esdid = d.esdid
				WHERE		d.docid = {$docid}";
        $estado = $db->pegaUm($sql);
        return $estado;
    } else {
        return false;
    }
}

function criarDocumento($sifid) {
    global $db;

    if (empty($sifid)) {
        return false;
    }

    $docid = pegarDocid($sifid);

    if (!$docid) {
        $docdsc = "SISFOR Tramitação Curso - " . $sifid;
        $docid = wf_cadastrarDocumento(WF_TPDID_SISFOR, $docdsc);
        if ($sifid) {
            $sql = "UPDATE 	sisfor.sisfor
					SET		docid = {$docid} 
					WHERE 	sifid = {$sifid}";

            $db->executar($sql);
            $db->commit();
            return $docid;
        } else {
            return false;
        }
    } else {
        return $docid;
    }
}

function criarDocumentoPlanejamento($unicod) {
    global $db;

    if (!isset($_SESSION['sisfor']['unicod'])) {
        return false;
    }

    if (empty($_SESSION['sisfor']['unicod'])) {
        return false;
    }

    if (empty($unicod)) {
        return false;
    }

    $docid = pegarDocidPlanejamento($unicod);

    if(empty($docid)){
        $docdsc = "SISFOR Tramitação Planejamento - " . $unicod;
        $docid = wf_cadastrarDocumento(WF_TPDID_SISFOR_PLAN, $docdsc);
        $unitpocod = selecionarUnidade($unicod);
        if (empty($unicod)) {
            return false;
        } else {
        	$sql = "SELECT sieid FROM sisfor.sisfories WHERE unicod = '{$unicod}'";
        	$sieid = $db->pegaUm($sql);
        	
        	if($sieid){
               $sql = "UPDATE sisfor.sisfories SET docid = {$docid} WHERE unicod = '{$unicod}'";
        	} else {
	            $sql = "INSERT INTO sisfor.sisfories (unicod, unitpocod, docid, siestatus) VALUES ('{$unicod}', '{$unitpocod}', {$docid}, 'A')";
        	}
 			$db->executar($sql);  
            $db->commit();
            return $docid;
        }
    } else {
        return $docid;
    }
}

function recuperarCursoNVinculado($cnvid) {
    global $db;

    $aryWhere[] = "cnv.cnvstatus = 'A'";

    if ($cnvid) {
        $aryWhere[] = "cnv.cnvid = {$cnvid}";
    }

    $sql = "SELECT 		cur.curid
			FROM 		sisfor.cursonaovinculado cnv 
			INNER JOIN 	catalogocurso2014.curso cur ON cnv.curid = cur.curid
			INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";

    $curid = $db->pegaUm($sql);
    return $curid;
}

function verificarSaldoSEB() {
    global $db;

    $aryWhereSEB[] = "vpl.unicod = '{$_SESSION['sisfor']['unicod']}'";
    $aryWhereSEB[] = "vpl.vppstatus = 'A' AND vpl.vppsecretaria = '1'";

    $sql = "SELECT 		(COALESCE(vpl.vppvalor,0) - COALESCE(sis.sifvalorloa,0)) AS saldoies
			FROM 		sisfor.valorprevistoploa vpl 
			LEFT JOIN 	(SELECT 		SUM(sifvalorloa) AS sifvalorloa, sif.unicod FROM sisfor.sisfor sif
						INNER JOIN 		catalogocurso2014.iesofertante ieo ON sif.ieoid = ieo.ieoid AND ieo.ieostatus = 'A'
						INNER JOIN 		catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
						LEFT JOIN  		catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
						WHERE			ieo.ieoid IS NOT NULL AND sifstatus = 'A' AND (coo.coordsigla ILIKE '%SEB%') GROUP BY sif.unicod) AS sis ON sis.unicod = vpl.unicod
						" . (is_array($aryWhereSEB) ? ' WHERE ' . implode(' AND ', $aryWhereSEB) : '') . "";

    $seb = $db->pegaUm($sql);
    return $seb;
}

function verificarSaldoSECADI() {
    global $db;

    $aryWhereSECADI[] = "vpl.unicod = '{$_SESSION['sisfor']['unicod']}'";
    $aryWhereSECADI[] = "vpl.vppstatus = 'A' AND vpl.vppsecretaria = '2'";

    $sql = "SELECT 		(COALESCE(vpl.vppvalor,0) - COALESCE(sis.sifvalorloa,0)) AS saldoies
			FROM 		sisfor.valorprevistoploa vpl 
			LEFT JOIN 	(SELECT 		SUM(sifvalorloa) AS sifvalorloa, sif.unicod FROM sisfor.sisfor sif
						INNER JOIN 		catalogocurso2014.iesofertante ieo ON sif.ieoid = ieo.ieoid AND ieo.ieostatus = 'A'
						INNER JOIN 		catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
						LEFT JOIN  		catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
						WHERE			ieo.ieoid IS NOT NULL AND sifstatus = 'A' AND (coo.coordsigla ILIKE '%SECADI%') GROUP BY sif.unicod) AS sis ON sis.unicod = vpl.unicod
						" . (is_array($aryWhereSECADI) ? ' WHERE ' . implode(' AND ', $aryWhereSECADI) : '') . "";

    $secadi = $db->pegaUm($sql);
    return $secadi;
}

function verificarCoordenacao($coordid = null) {
    global $db;

    $sql = "SELECT coordsigla FROM catalogocurso2014.coordenacao WHERE (coordsigla ILIKE '%SEB%') AND coordid = {$coordid} AND coordstatus = 'A'";
    $seb = $db->pegaUm($sql);

    if ($seb) {
        return 'seb';
    }

    $sql = "SELECT coordsigla FROM catalogocurso2014.coordenacao WHERE (coordsigla ILIKE '%SECADI%') AND coordid = {$coordid} AND coordstatus = 'A'";
    $secadi = $db->pegaUm($sql);

    if ($secadi) {
        return 'secadi';
    }
}

function recuperarJustificativa($sifid) {
    global $db;

    $sql = "SELECT sifjustificativa FROM sisfor.sisfor WHERE sifid = {$sifid} AND sifstatus = 'A'";
    $rs = $db->pegaUm($sql);
    return $rs;
}

function verificarCursoMEC() {
    global $db;

    $aryWhere[] = "(coo.coordsigla ILIKE '%SEB%' OR coo.coordsigla ILIKE '%SECADI%')";
    $aryWhere[] = "sif.sifopcao IS NULL AND ieo.ieostatus = 'A' AND sif.siftipoplanejamento = ".FASE01."";

    if ($_SESSION['sisfor']['unicod']) {
        $aryWhere[] = "ieo.unicod = '{$_SESSION['sisfor']['unicod']}'";
    }

    $sql = "SELECT 			COUNT(cur.curid) AS total
			FROM       	 	catalogocurso2014.iesofertante ieo
			INNER JOIN 		catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
			INNER JOIN   	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
			LEFT JOIN		sisfor.sisfor sif ON sif.ieoid = ieo.ieoid
							" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";

    $cursos = $db->pegaLinha($sql);
    return $cursos['total'];
}

function finalizarPlanejamento() {
    global $db;

    $saldoseb = verificarSaldoSEB();
    $saldosecadi = verificarSaldoSECADI();
    $habilitacurso = verificarCursoMEC();

    $sql = "SELECT COUNT(sifid) AS qtdcatalogo FROM sisfor.sisfor WHERE unicod = '{$_SESSION['sisfor']['unicod']}' AND cnvid IS NOT NULL AND sifstatus = 'A' AND sifaprovado2013 IS NULL AND siftipoplanejamento = ".FASE01."";

    $catalogo = $db->pegaUm($sql);

    $sql = "SELECT 			COUNT(sifid) AS coordenadormec
			FROM 			sisfor.sisfor sif 
			INNER JOIN		catalogocurso2014.iesofertante ieo ON ieo.ieoid = sif.ieoid
			INNER JOIN 		catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
			WHERE 			sif.unicod = '{$_SESSION['sisfor']['unicod']}' AND sif.sifstatus = 'A' AND sif.usucpf IS NULL AND sif.sifopcao IN ('1','3') AND sif.siftipoplanejamento = ".FASE01."";

 	
    $coordmec = $db->pegaUm($sql);

    $sql = "SELECT  			COUNT(sifid) AS coordatividade
			FROM 	   		sisfor.sisfor sif 
			INNER JOIN 		sisfor.outraatividade oat ON sif.oatid = oat.oatid
			WHERE 			sif.unicod = '{$_SESSION['sisfor']['unicod']}' AND sif.sifstatus = 'A' AND sif.usucpf IS NULL AND sif.siftipoplanejamento = ".FASE01."";

    $coordatividade = $db->pegaUm($sql);

    $sql = "SELECT 	 		COUNT(sifid) AS coordoutrocurso
			FROM 	   		sisfor.sisfor sif
			INNER JOIN 		sisfor.outrocurso out ON sif.ocuid = out.ocuid
			WHERE 			sif.unicod = '{$_SESSION['sisfor']['unicod']}' AND sif.sifstatus = 'A' AND sif.usucpf IS NULL AND sif.siftipoplanejamento = ".FASE01."";

    $coordoutrocurso = $db->pegaUm($sql);


    $sql = "SELECT 	 		COUNT(sifid) AS coordnaovinculado
			FROM 	   		sisfor.sisfor sif
			INNER JOIN 		sisfor.cursonaovinculado cnv ON sif.cnvid = cnv.cnvid
			WHERE 			sif.unicod = '{$_SESSION['sisfor']['unicod']}' AND sif.sifstatus = 'A' AND sif.usucpf IS NULL AND sif.siftipoplanejamento = ".FASE01."";

    $coordnaovinculado = $db->pegaUm($sql);

//    ver($coordmec, $coordatividade, $coordoutrocurso, $coordnaovinculado);
  //  die;
//     if ($coordmec <> 0 || $coordatividade <> 0 || $coordoutrocurso <> 0 || $coordnaovinculado <> 0) {
//         return "Favor informar o Coordenador do Curso.";
//     }

    if ($catalogo <> 0) {
        return "Favor informar se os Cursos do Catálogo MEC foram aprovados em 2013.";
    }

    if ($habilitacurso <> 0) {
        return "Não é possível finalizar o planejamento porque há cursos não analisados pela Instituição. Por favor, indique para cada curso proposto pelo MEC se a Instituição aceita, rejeita ou repactua a proposta.";
    }

    if ($saldoseb < 0 && $saldosecadi < 0) {
        return "Não há saldo suficiente de recursos LOA.";
    }
    
    if( !validaCoordenadoresFase2() ){
    	return "Não é possivel finalizar esta fase Sem ter cadastrado um Coordenador para cada curso.";
    }
    return true;
}

function form_msgPlanejamento() {
    global $db;

    $saldoseb = verificarSaldoSEB();
    $saldosecadi = verificarSaldoSECADI();

    if ($saldoseb > 0 || $saldosecadi > 0) {
        echo "<div>Há recursos orçamentários não programados para utilização em 2014. Confirma a finalização do planejamento?</div>";
    } else {
        echo "<div>Confirma a finalização do planejamento?</div>";
    }
}

function msgPlanejamento() {
    $retorno = Array('boo' => true, 'msg' => '');
    $retorno = simec_json_encode($retorno);
    echo $retorno;
}

function verificarSaldoSecretaria($post) {
    global $db;
    
    extract($post);

    if($ocuid){
		$sql = "SELECT coordid FROM sisfor.outrocurso WHERE ocuid = {$ocuid}";
		$coordenacaoid = $db->pegaUm($sql);  	
    }
    
    if($oatid){
		$sql = "SELECT coordid FROM sisfor.outraatividade WHERE oatid = {$oatid}";
		$coordenacaoid = $db->pegaUm($sql);
    }
    
    if($coordenacaoid){
    	$coordenacao = verificarCoordenacao($coordenacaoid);
    }
    $coord = verificarCoordenacao($coordid);
    
    if($coord == $coordenacao){
  	   echo 'S';
    } else {	
	    if ($coord == 'seb') {
	        $seb = carregarDadosSEB();
	        $recurso = (desformata_valor($seb['valortotal']) + desformata_valor($loa));
	        $saldoseb = formata_valor(desformata_valor($seb['saldoies']) - $recurso);
	        if ($saldoseb <= 0) {
	            echo 'N';
	        } else {
	            echo 'S';
	        }
	    }
	
	    if ($coord == 'secadi') {
	        $secadi = carregarDadosSECADI();
	        $recurso = (desformata_valor($secadi['valortotal']) + desformata_valor($loa));
	        $saldosecadi = formata_valor(desformata_valor($secadi['saldoies']) - $recurso);
	        if ($saldosecadi <= 0) {
	            echo 'N';
	        } else {
	            echo 'S';
	        }
	    }
    }
}

function atualizarSaldo($post) {
    global $db;

    extract($post);

    $coord = verificarCoordenacao($coordid);

    if ($coord == 'seb') {
        $seb = carregarDadosSEB();
        $recurso = (desformata_valor($seb['valortotal']) + desformata_valor($loa));
        $saldo = formata_valor(desformata_valor($seb['saldoies']) - $recurso);
    }

    if ($coord == 'secadi') {
        $secadi = carregarDadosSECADI();
        $recurso = (desformata_valor($secadi['valortotal']) + desformata_valor($loa));
        $saldo = formata_valor(desformata_valor($secadi['saldoies']) - $recurso);
    }

    $resultado["secretaria"] = trim($coord);
    $resultado["saldo"] = iconv("ISO-8859-1", "UTF-8", $saldo);

    $saldo = desformata_valor($saldo);

    if ($saldo < 0) {
        $resultado["resultado"] = "N";
    } else {
        $resultado["resultado"] = "S";
    }
    echo simec_json_encode($resultado);
}

function relatorioCursosMEC($tipo) {
    global $db;

    $sql = "SELECT 			sif.unicod, 
							uni.unidsc,
							ide.iusnome, 
							ide.iusemailprincipal, 
							'(' || idt.itedddtel || ') ' || idt.itenumtel AS telefone,
							cur.curid,
							cur.curdesc,
							coo.coordsigla, 
							ieo.ieoqtdvagas,
							COALESCE(ieo.valor_estimado,'0')::numeric(20,2) AS valor_estimado,
							CASE WHEN sif.sifopcao = '1' THEN 'Aceita' 
								 WHEN sif.sifopcao = '2' THEN 'Rejeita'
								 WHEN sif.sifopcao = '3' THEN 'Repactua' END AS sifopcao,
							sif.sifjustificativa,
							sif.sifqtdvagas,
							sif.sifvalorloa,
							sif.sifvaloroutras
			FROM 			sisfor.sisfor sif 
			INNER JOIN 		catalogocurso2014.iesofertante ieo ON sif.ieoid = ieo.ieoid AND sif.sifstatus = 'A'
			INNER JOIN 		catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
			INNER JOIN 		catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
			INNER JOIN 		public.unidade uni ON uni.unicod = sif.unicod
			LEFT JOIN		sisfor.sisfories sie ON sie.unicod = sif.unicod
			LEFT JOIN 		sisfor.tipoperfil tpe ON tpe.tpeid = sie.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_INST . "				
			LEFT JOIN 		sisfor.identificacaousuario ide ON ide.iuscpf = sif.usucpf AND ide.iusstatus = 'A'				
			LEFT JOIN 		sisfor.identificacaotelefone idt ON idt.iusd = ide.iusd AND idt.itetipo = 'T'
			ORDER BY		uni.unidsc, ide.iusnome";

    $cabecalho = array('Código da IES', 'Nome da IES', 'Coordenador Institucional', 'E-mail', 'Telefone', 'Código do curso', 'Curso', 'Secretaria/ diretoria responsável', 'Proposta MEC – Vagas', 'Proposta MEC – Valor', 'Posição da IES', 'Justificativa', 'Proposta IES – Vagas', 'Proposta IES – Valor', 'Proposta IES – Outras fontes');
    if ($tipo == 'html') {
        $db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '');
    } else {
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=SIMEC_RelatCursoMEC" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=SIMEC_RelatCursoMEC" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");
        $db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%');
    }
}

function relatorioCursosCatalogo($tipo) {
    global $db;

    $sql = "SELECT 			sif.unicod, 
							uni.unidsc,
							ide.iusnome, 
							ide.iusemailprincipal, 
							'(' || idt.itedddtel || ') ' || idt.itenumtel AS telefone,
							cur.curid,
							cur.curdesc,
							coo.coordsigla, 
							CASE WHEN sif.sifaprovado2013 = 't' THEN 'Sim'
								 WHEN sif.sifaprovado2013 = 'f' THEN 'Não' 
								 ELSE ' - ' END AS sifaprovado2013,
							sif.sifqtdvagas,
							sif.sifvalorloa,
							sif.sifvaloroutras
    		
			FROM 			sisfor.sisfor sif 
			INNER JOIN 		sisfor.cursonaovinculado cnv ON cnv.cnvid = sif.cnvid AND sif.sifstatus = 'A'
			INNER JOIN 		catalogocurso2014.curso cur ON cnv.curid = cur.curid AND cur.curstatus = 'A'
			INNER JOIN 		catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
			INNER JOIN 		public.unidade uni ON uni.unicod = sif.unicod
			LEFT JOIN		sisfor.sisfories sie ON sie.unicod = sif.unicod
			LEFT JOIN 		sisfor.tipoperfil tpe ON tpe.tpeid = sie.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_INST . "				
			LEFT JOIN 		sisfor.identificacaousuario ide ON ide.iuscpf = sif.usucpf AND ide.iusstatus = 'A'				
			LEFT JOIN 		sisfor.identificacaotelefone idt ON idt.iusd = ide.iusd AND idt.itetipo = 'T'
			ORDER BY		uni.unidsc, ide.iusnome";

    $cabecalho = array('Código da IES', 'Nome da IES', 'Coordenador Institucional', 'E-mail', 'Telefone', 'Código do curso', 'Curso', 'Secretaria/ diretoria responsável', 'Aprovado em 2013', 'Proposta IES – Vagas', 'Proposta IES – Valor', 'Proposta IES – Outras fontes');
    $alinhamento = array('center', '', 'left', 'left', '', 'center', '', 'center', 'center', '', '', '');

    if ($tipo == 'html') {
        $db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '', '', $alinhamento);
    } else {
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=SIMEC_RelatCursoCatalogo" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=SIMEC_RelatCursoCatalogo" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");
        $db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%');
    }
}


function relatorioCursosValidadosData($tipo) {
	global $db;

	$sql = "select  u.unicod,
					u.unidsc,
					COALESCE(usu.usunome,'Não cadastrado'),
					case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
			     		when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
			     		when s.ocuid is not null then oc.ocunome end as curso,
					(select to_char(atidatainicio,'dd/mm/YYYY') as atidatainicio from sisfor.atividadescurso where atidesc='Publicação de edital' and sifid=s.sifid and atistatus='A' order by atidatainicio desc limit 1) as publicacaoeditalini,
					(select to_char(atidatafim,'dd/mm/YYYY') as atidatafim from sisfor.atividadescurso where atidesc='Publicação de edital' and sifid=s.sifid and atistatus='A' order by atidatainicio desc limit 1) as publicacaoeditalfim,
			
					(select to_char(atidatainicio,'dd/mm/YYYY') as atidatainicio from sisfor.atividadescurso where atidesc='Processo seletivo' and sifid=s.sifid and atistatus='A' order by atidatainicio desc limit 1) as processoseletivoini,
					(select to_char(atidatafim,'dd/mm/YYYY') as atidatafim from sisfor.atividadescurso where atidesc='Processo seletivo' and sifid=s.sifid and atistatus='A' order by atidatainicio desc limit 1) as processoseletivofim,
						
					(select to_char(atidatainicio,'dd/mm/YYYY') as atidatainicio from sisfor.atividadescurso where atidesc='Período de matrículas no curso' and sifid=s.sifid and atistatus='A' order by atidatainicio desc limit 1) as periodomatriculaini,
					(select to_char(atidatafim,'dd/mm/YYYY') as atidatafim from sisfor.atividadescurso where atidesc='Período de matrículas no curso' and sifid=s.sifid and atistatus='A' order by atidatainicio desc limit 1) as periodomatriculafim,
			
    				(select to_char(atidatainicio,'dd/mm/YYYY') as dt1 from  sisfor.atividadescurso where atidesc in('Início do curso') and sifid=s.sifid and atistatus='A' limit 1) as dataInicioini,
					(select to_char(atidatafim,'dd/mm/YYYY') as dt2 from  sisfor.atividadescurso where atidesc in('Início do curso') and sifid=s.sifid and atistatus='A' limit 1) as dataIniciofim,
    				(select to_char(atidatainicio,'dd/mm/YYYY') as dt3 from  sisfor.atividadescurso where atidesc in('Término do curso') and sifid=s.sifid and atistatus='A' limit 1) as dataFimini,
					(select to_char(atidatafim,'dd/mm/YYYY') as dt4 from  sisfor.atividadescurso where atidesc in('Término do curso') and sifid=s.sifid and atistatus='A' limit 1) as dataFimfim
			
						
	 
			from sisfor.sisfor s 
			inner join workflow.documento d on d.docid = s.docidprojeto 
			left join workflow.historicodocumento h on h.hstid = d.hstid 
			inner join workflow.estadodocumento e on e.esdid = d.esdid 
			inner join public.unidade u on u.unicod = s.unicod 
			left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid 
			left join catalogocurso2014.curso cur on cur.curid = ieo.curid 
			left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
			left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid 
			left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid 
			left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid 
			left join seguranca.usuario usu on usu.usucpf = s.usucpf 
			left join sisfor.outrocurso oc on oc.ocuid = s.ocuid 
			left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid
			where sifstatus='A' and d.esdid='".ESD_PROJETO_VALIDADO."'";

	$cabecalho = array('Código da IES', 'Nome da IES', 'Coordenador Institucional', 'Curso', 'Publicação de edital (Início)', 'Publicação de edital  (Término)', 'Processo seletivo (Início)', 'Processo seletivo (Término)', 'Período de matrículas no curso (Início)', 'Período de matrículas no curso (Término)','Início do curso (Início)','Início do curso (Término)','Término do curso (Início)','Término do curso (Término)');

	if ($tipo == 'html') {
		$db->monta_lista($sql, $cabecalho, '500', '10', '', '', '', '', '');
	} else {
		header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
		header("Pragma: no-cache");
		header("Content-type: application/xls; name=SIMEC_RelatCursoCatalogo" . date("Ymdhis") . ".xls");
		header("Content-Disposition: attachment; filename=SIMEC_RelatCursoVali" . date("Ymdhis") . ".xls");
		header("Content-Description: MID Gera excel");
		$db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%');
	}
}

function relatorioCursosForaCatalogo($tipo) {
    global $db;

    $sql = "SELECT 			sif.unicod, 
							uni.unidsc,
							ide.iusnome, 
							ide.iusemailprincipal, 
							'(' || idt.itedddtel || ') ' || idt.itenumtel AS telefone,
							ocu.ocunome,
							coo.coordsigla, 
							ocu.ocudesc,
							sif.sifqtdvagas,
							sif.sifvalorloa,
							sif.sifvaloroutras
			FROM 			sisfor.sisfor sif
			INNER JOIN 		sisfor.outrocurso ocu ON  ocu.ocuid = sif.ocuid AND sif.sifstatus = 'A'
			INNER JOIN 		public.unidade uni ON uni.unicod = sif.unicod
			LEFT JOIN 		catalogocurso2014.coordenacao coo ON coo.coordid = ocu.coordid
			LEFT JOIN		sisfor.sisfories sie ON sie.unicod = sif.unicod
			LEFT JOIN 		sisfor.tipoperfil tpe ON tpe.tpeid = sie.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_INST . "				
			LEFT JOIN 		sisfor.identificacaousuario ide ON ide.iuscpf = sif.usucpf AND ide.iusstatus = 'A'		
			LEFT JOIN 		sisfor.identificacaotelefone idt ON idt.iusd = ide.iusd AND idt.itetipo = 'T'
			ORDER BY		uni.unidsc, ide.iusnome";

    $cabecalho = array('Código da IES', 'Nome da IES', 'Coordenador Institucional', 'E-mail', 'Telefone', 'Curso', 'Secretaria/ diretoria responsável', 'Descrição', 'Proposta IES – Vagas', 'Proposta IES – Valor LOA', 'Proposta IES – Valor Outras fontes');

    if ($tipo == 'html') {
        $db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '');
    } else {
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=SIMEC_RelatCursoForaCatalogo" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=SIMEC_RelatCursoForaCatalogo" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");
        $db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%');
    }
}

function relatorioOutrasAtividades($tipo) {
    global $db;

    $sql = "SELECT 			sif.unicod, 
							uni.unidsc,
							ide.iusnome, 
							ide.iusemailprincipal, 
							'(' || idt.itedddtel || ') ' || idt.itenumtel AS telefone,
							coo.coordsigla, 
							CASE WHEN oat.oatatividade = '1' THEN 'Seminário SEB'
										WHEN oat.oatatividade = '2' THEN 'Seminário SECADI' 
										WHEN oat.oatatividade = '3' THEN 'Custeio ComFor' END AS oatatividade, 
							oat.oatnome, 
							oat.oatdesc, 
							sif.sifvalorloa
			FROM 			sisfor.sisfor sif
			INNER JOIN 		sisfor.outraatividade oat ON oat.oatid = sif.oatid AND oat.oatstatus = 'A'
			INNER JOIN 		public.unidade uni ON uni.unicod = sif.unicod
			LEFT JOIN 		catalogocurso2014.coordenacao coo ON coo.coordid = oat.coordid
			LEFT JOIN		sisfor.sisfories sie ON sie.unicod = sif.unicod
			LEFT JOIN 		sisfor.tipoperfil tpe ON tpe.tpeid = sie.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_INST . "			
			LEFT JOIN 		sisfor.identificacaousuario ide ON ide.iuscpf = sif.usucpf AND ide.iusstatus = 'A'
			LEFT JOIN 		sisfor.identificacaotelefone idt ON idt.iusd = ide.iusd AND idt.itetipo = 'T'
			ORDER BY		uni.unidsc, ide.iusnome";

    $cabecalho = array('Código da IES', 'Nome da IES', 'Coordenador Institucional', 'E-mail', 'Telefone', 'Secretaria', 'Tipo de Atividade', 'Nome da Atividade', 'Descrição', 'Proposta IES – Valor LOA');

    if ($tipo == 'html') {
        $db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '');
    } else {
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=SIMEC_RelatOutrasAtividades" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=SIMEC_RelatOutrasAtividades" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");
        $db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%');
    }
}

function relatorioProjeto($tipo) {
    global $db;

    $sql = "-- RELATÓRIO 1 – Cursos propostos pelo MEC
			SELECT 	DISTINCT	sif.unicod AS cod_ies, 
				                en.entnome AS ies,        
				                'Proposta MEC' AS tipo_curso,
				                cur.curid AS cod_curso,
				                cur.curdesc AS curso,     
				                ide.iusnome AS coordenador, 
				                ide.iusemailprincipal AS email,
				                CASE WHEN esd.esddsc IS NULL THEN 'Não iniciado' ELSE esd.esddsc END AS situacao
			FROM 				sisfor.sisfor sif
			INNER JOIN 			catalogocurso2014.iesofertante ieo ON sif.ieoid = ieo.ieoid
			INNER JOIN 			catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
			INNER JOIN 			catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
			INNER JOIN 			public.unidade uni ON uni.unicod = sif.unicod
			INNER JOIN 			entidade.entidade en ON en.entunicod = sif.unicod
			INNER JOIN 			entidade.funcaoentidade fun ON fun.entid = en.entid AND funid IN (12,11,44,102)
            LEFT JOIN 			workflow.documento doc on doc.docid = sif.docid 
            LEFT JOIN 			workflow.estadodocumento esd on esd.esdid = doc.esdid  
			LEFT JOIN 			sisfor.identificacaousuario ide ON ide.iuscpf = sif.usucpf AND ide.iusstatus = 'A'
			LEFT JOIN 			sisfor.identificacaotelefone idt ON idt.iusd = ide.iusd AND idt.itetipo = 'T'

			UNION ALL

			--- RELATÓRIO 2 – Cursos Propostos pela IES – Cursos do Catálogo MEC
			select 
				                sif.unicod as cod_ies, 
				                en.entnome as ies,        
				                'Proposta IES – Catálogo' as tipo_curso,
				                cur.curid as cod_curso,
				                cur.curdesc as curso,     
				                ide.iusnome as coordenador, 
				                ide.iusemailprincipal as email,
				                CASE when esd.esddsc is null then 'Não iniciado' else esd.esddsc end as situacao
			FROM 				sisfor.sisfor sif
			INNER JOIN 			sisfor.cursonaovinculado cnv ON cnv.cnvid = sif.cnvid
			INNER JOIN 			catalogocurso2014.curso cur ON cnv.curid = cur.curid AND cur.curstatus = 'A'
			INNER JOIN 			catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
			INNER JOIN 			public.unidade uni ON uni.unicod = sif.unicod
			INNER JOIN 			entidade.entidade en ON en.entunicod = sif.unicod
			INNER JOIN 			entidade.funcaoentidade fun ON fun.entid = en.entid AND funid IN (12,11,44,102)
            LEFT JOIN 			workflow.documento doc ON doc.docid = sif.docid 
            LEFT JOIN 			workflow.estadodocumento esd ON esd.esdid = doc.esdid  
			LEFT JOIN 			sisfor.identificacaousuario ide ON ide.iuscpf = sif.usucpf AND ide.iusstatus = 'A'
			LEFT JOIN 			sisfor.identificacaotelefone idt ON idt.iusd = ide.iusd AND idt.itetipo = 'T'

			UNION ALL   

			-- RELATÓRIO 3 – Cursos Propostos pela IES – Cursos fora do Catálogo
			SELECT 	          	sif.unicod as cod_ies, 
			        	        en.entnome as ies,        
			            	    'Proposta IES – Fora do Catálogo' as tipo_curso,
			                	ocu.ocuid as cod_curso,
				                ocu.ocunome as curso,                
				                ide.iusnome AS coordenador, 
				                ide.iusemailprincipal as email,
				                CASE WHEN esd.esddsc IS NULL THEN 'Não iniciado' ELSE esd.esddsc END AS situacao
			FROM 				sisfor.sisfor sif
			INNER JOIN 			sisfor.outrocurso ocu ON  ocu.ocuid = sif.ocuid AND ocu.ocustatus = 'A'
			INNER JOIN 			public.unidade uni ON uni.unicod = sif.unicod
			INNER JOIN 			entidade.entidade en ON en.entunicod = sif.unicod
			INNER JOIN 			entidade.funcaoentidade fun ON fun.entid = en.entid AND funid IN (12,11,44,102)
			LEFT JOIN 			catalogocurso2014.coordenacao coo ON coo.coordid = ocu.coordid 
			LEFT JOIN 			workflow.documento doc on doc.docid = sif.docid 
            LEFT JOIN 			workflow.estadodocumento esd on esd.esdid = doc.esdid  
			LEFT JOIN 			sisfor.identificacaousuario ide ON ide.iuscpf = sif.usucpf AND ide.iusstatus = 'A'
			LEFT JOIN 			sisfor.identificacaotelefone idt ON idt.iusd = ide.iusd AND idt.itetipo = 'T'
			ORDER BY 			1,5";

    $cabecalho = array('Código da IES', 'Nome da IES', 'Tipo Curso', 'Código do curso', 'Curso', 'Coordenador Institucional', 'E-mail', 'Situação');
    $tamanho = array('5%', '15%', '5%', '5%', '20%', '12%', '5%', '5%');

    if ($tipo == 'html') {
        $db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '', $tamanho);
    } else {
        header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
        header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
        header("Pragma: no-cache");
        header("Content-type: application/xls; name=SIMEC_RelatCursoMEC" . date("Ymdhis") . ".xls");
        header("Content-Disposition: attachment; filename=SIMEC_RelatorioProjeto" . date("Ymdhis") . ".xls");
        header("Content-Description: MID Gera excel");
        $db->monta_lista_tabulado($sql, $cabecalho, 100000, 5, 'N', '100%');
    }
}

function totalCursosPropostoMEC() {
    global $db;

    $aryWhere[] = "(coo.coordsigla ILIKE '%SEB%' OR coo.coordsigla ILIKE '%SECADI%')";
    $aryWhere[] = "ieo.ieostatus = 'A'";

    if ($_SESSION['sisfor']['unicod']) {
        $aryWhere[] = "ieo.unicod = '{$_SESSION['sisfor']['unicod']}'";
    }

    $sql = "SELECT 			DISTINCT COUNT(cur.curid) AS total
			FROM       	 	catalogocurso2014.iesofertante ieo
			INNER JOIN 		catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
			INNER JOIN   	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
			LEFT JOIN		sisfor.sisfor sif ON sif.ieoid = ieo.ieoid
			LEFT JOIN		sisfor.tipoperfil tpe ON tpe.tpeid = sif.tpeid AND tpe.pflcod = " . PFL_COORDENADOR_CURSO . "
			LEFT JOIN		sisfor.identificacaousuario ius ON ius.iuscpf = sif.usucpf AND ius.iusstatus = 'A'
			LEFT JOIN		workflow.documento doc ON doc.docid = sif.docid
			LEFT JOIN		workflow.estadodocumento est ON est.esdid = doc.esdid
							" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";

    $total = $db->pegaUm($sql);
    return $total;
}

function historicoUsuario($iuscpf) {
    global $db;

    echo "<p align='center'><b>Histórico de gerenciamento de usuários</b></p>";
    $sql = "SELECT htudsc, to_char(htudata,'dd/mm/YYYY HH24:MI') AS htudata FROM seguranca.historicousuario WHERE usucpf = '{$iuscpf}' AND sisid = '" . SIS_SISFOR . "'";
    $cabecalho = array("Motivo", "Data");
    $db->monta_lista_simples($sql, $cabecalho, 150, 10, 'N', '', 'N');
}

function definirAbrangencia($dados) {
    global $db;
    $sql = "SELECT
			'<center>" . (($dados['consulta']) ? "" : "<img src=\"../imagens/excluir.gif\" border=\"0\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirAbrangencia('||a.abrid||');\">") . "</center>' as acao2,
			m.estuf||' - '||m.mundescricao as descricao,
			CASE WHEN a.esfera='M' THEN 'Municipal' 
				 WHEN a.esfera='E' THEN 'Estadual' 
				 WHEN a.esfera='N' THEN 'Nacional' END esfera
			FROM sisfor.abrangenciacurso a
			INNER JOIN territorios.municipio m ON m.muncod = a.muncod
			WHERE a.sifid='" . $dados['sifid'] . "' AND polid IS NULL
			ORDER BY 2";

    $cabecalho = array("&nbsp;", "UF/ Município", "Abrangência");
    $db->monta_lista_simples($sql, $cabecalho, 1000, 5, 'S', '100%', $par2);
}

function excluirAbrangencia($dados) {
    global $db;

    $db->executar("DELETE FROM sisfor.abrangenciacurso WHERE abrid='" . $dados['abrid'] . "'");
    $db->commit();
}

function excluirPlanoAtividades($dados) {
    global $db;

    $sql = "DELETE FROM sisfor.atividadescurso WHERE atiidpai='" . $dados['atiid'] . "'";
    $db->executar($sql);

    $sql = "DELETE FROM sisfor.atividadescurso WHERE atiid='" . $dados['atiid'] . "'";
    $db->executar($sql);

    $db->commit();
}

function carregarPlanoAtividades($dados) {
    global $db;

    $sql = "SELECT * FROM sisfor.atividadescurso a 
			WHERE a.sifid='" . $dados['sifid'] . "' AND atiidpai IS NULL ORDER BY a.atiid";

    $atividades = $db->carregar($sql);

    echo "<table class=\"listagem\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\" width=\"100%\">";
    echo "<tr>";
    echo "<td width=\"50%\" class=\"SubTituloCentro\">ATIVIDADES / SUBATIVIDADES</td><td class=\"SubTituloCentro\" colspan=\"2\">PERÍODO DE EXECUÇÃO</td>";
    echo "</tr>";

    echo "<tr>";
    echo "<td width=\"50%\" align=\"center\">&nbsp;</td><td align=\"center\">Início</td><td align=\"center\">Término</td>";
    echo "</tr>";


    if ($atividades[0]) {

        foreach ($atividades as $atidesc => $at) {

            echo "<tr>";
            echo "<td width=\"50%\" colspan=3><b><img src=../imagens/seta_filho.gif> " . ((!$dados['consulta']) ? "<img src=\"../imagens/excluir.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirPlanoAtividades('" . $at['atiid'] . "');\">" : "") . " " . $at['atidesc'] . "</b></td>";
            echo "</tr>";

            $sql = "SELECT * FROM sisfor.atividadescurso a 
					WHERE a.atiidpai='" . $at['atiid'] . "' ORDER BY a.atiid";

            $subatividades = $db->carregar($sql);

            if ($subatividades[0]) {
                foreach ($subatividades as $su) {
                    echo "<tr>";
                    echo "<td width=\"50%\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=../imagens/seta_filho.gif> " . ((!$dados['consulta']) ? "<img src=\"../imagens/excluir.gif\" align=\"absmiddle\" style=\"cursor:pointer;\" onclick=\"excluirPlanoAtividades('" . $su['atiid'] . "');\">" : "") . " " . $su['atidesc'] . "</td>";
                    if ($dados['consulta'])
                        echo "<td align=\"center\">" . formata_data($su['atidatainicio']) . "</td>";
                    else
                        echo "<td>" . campo_data2('atidatainicio[' . $su['atiid'] . ']', 'S', 'S', 'Inicío', 'S', '', '', $su['atidatainicio'], '', '', 'atidatainicio_' . $su['atiid']) . "</td>";
                    if ($dados['consulta'])
                        echo "<td align=\"center\">" . formata_data($su['atidatafim']) . "</td>";
                    else
                        echo "<td>" . campo_data2('atidatafim[' . $su['atiid'] . ']', 'S', 'S', 'Término', 'S', '', '', $su['atidatafim'], '', '', 'atidatafim_' . $su['atiid']) . "</td>";
                    echo "</tr>";
                }
            }

            if (!$dados['consulta']) {
                echo "<tr>";
                echo "<td width=\"50%\" colspan=3>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src=../imagens/seta_filho.gif> Adicionar Subatividade : ";

                $pamid = $db->pegaUm("SELECT pamid FROM sisfor.planoatividadesmodelo WHERE pamdsc='" . $at['atidesc'] . "'");

                $sql = "SELECT pamid as codigo, pamdsc as descricao FROM sisfor.planoatividadesmodelo WHERE pamtipo='S' " . (($pamid) ? " AND (pamidpai='" . $pamid . "' OR pamidpai IS NULL)" : "") . " ORDER BY pamdsc";
                $db->monta_combo('pamid_sb_' . $at['atiid'], $sql, 'S', 'Selecione', '', '', '', '', 'N', 'pamid_sb_' . $at['atiid'], '');

                echo " <input type=button value=\"Adicionar\" onclick=\"adicionarModeloPlanoAtividade('pamid_sb_" . $at['atiid'] . "','" . $at['atiid'] . "');\">";

                echo "</td>";
                echo "</tr>";
            }
        }
    } else {
        echo "<tr>";
        echo "<td class=\"SubTituloEsquerda\" colspan=3>Nenhuma Subatividade foi cadastrada</td>";
        echo "</tr>";
    }

    if (!$dados['consulta']) {
        echo "<tr>";
        echo "<td colspan=3><img src=../imagens/seta_filho.gif> Adicionar atividade : ";

        $sql = "SELECT pamid as codigo, pamdsc as descricao FROM sisfor.planoatividadesmodelo WHERE pamtipo='A' ORDER BY pamdsc";
        $db->monta_combo('pamid_at', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'pamid_at', '');

        echo " <input type=button value=\"Adicionar\" onclick=\"adicionarModeloPlanoAtividade('pamid_at','');\"></td>";
        echo "</tr>";
    }


    echo "</table>";
}

function atualizarDadosProjeto($dados) {
    global $db;

    $sql = "UPDATE sisfor.sisfor SET 
									 sifvigenciadtini='" . formata_data_sql($dados['sifvigenciadtini']) . "',
									 sifvigenciadtfim='" . formata_data_sql($dados['sifvigenciadtfim']) . "',
									 siforigemrecursos='" . $dados['siforigemrecursos'] . "',
									 siftipocertificacao='" . $dados['siftipocertificacao'] . "',
									 sifprodmaterialdidatico=" . (($dados['sifprodmaterialdidatico']) ? "'" . $dados['sifprodmaterialdidatico'] . "'" : "NULL") . ",
									 sifprofmagisterio=" . (($dados['sifprofmagisterio']) ? "'" . $dados['sifprofmagisterio'] . "'" : "NULL") . ",
									 sifmetodologia=" . (($dados['sifmetodologia']) ? "'" . substr($dados['sifmetodologia'], 0, 5000) . "'" : "NULL") . ",
									 sifnumvagasofertadas=" . (($dados['sifnumvagasofertadas']) ? "'" . $dados['sifnumvagasofertadas'] . "'" : "NULL") . ",
									 sifqtdvagas=" . (($dados['sifqtdvagas']) ? "'" . $dados['sifqtdvagas'] . "'" : "NULL") . ",
									 sifemailmatricula=" . (($dados['sifemailmatricula']) ? "'" . $dados['sifemailmatricula'] . "'" : "NULL") . ",
									 sifdddtelmatricula=" . (($dados['sifdddtelmatricula']) ? "'" . substr($dados['sifdddtelmatricula'], 0, 2) . "'" : "NULL") . ",
									 siftelmatricula=" . (($dados['siftelmatricula']) ? "'" . $dados['siftelmatricula'] . "'" : "NULL") . ",
									 sifcargahorariapresencial=" . (($dados['sifcargahorariapresencial']) ? "'" . $dados['sifcargahorariapresencial'] . "'" : "NULL") . ",
									 sifcargahorariadistancia=" . (($dados['sifcargahorariadistancia']) ? "'" . $dados['sifcargahorariadistancia'] . "'" : "NULL") . "
			WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "'";

    $db->executar($sql);

    if (!$_SESSION['sisfor']['curid']) {

        $sql = "UPDATE sisfor.outrocurso SET ocuobjetivo=" . (($dados['ocuobjetivo']) ? "'" . substr($dados['ocuobjetivo'], 0, 5000) . "'" : "NULL") . ", 
											 ocuementa=" . (($dados['ocuementa']) ? "'" . substr($dados['ocuementa'], 0, 5000) . "'" : "NULL") . "
				WHERE ocuid=(SELECT ocuid FROM sisfor.sisfor WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "')";

        $db->executar($sql);
    }

    $db->commit();

    echo "<script>
			alert('Dados do projeto foram gravados com sucesso');
			window.location='sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=dados_projeto';
		  </script>";
}

function montaCabecalhoCoordenadorCurso() {
    global $db;

    if (!$_SESSION['sisfor']['sifid'])
        die("<script>
				alert('Problemas na navegação. Você esta sendo redirecionado apra página principal. Tente novamente!');
				window.location='sisfor.php?modulo=inicio&acao=C';
			 </script>");
    
    $sql = "select
    			COALESCE(usu.usunome,'Não cadastrado') as coordenador,
    			uniabrev||' - '||unidsc as universidade,
			    case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc
				    when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc
				    when s.ocuid is not null then oc.ocunome
				    when s.oatid is not null then oatnome end as curso,
				    
			    case when s.ieoid is not null then cor.coordsigla
				    when s.cnvid is not null then cor2.coordsigla
				    when s.ocuid is not null then cor3.coordsigla
				    when s.oatid is not null then cor4.coordsigla end as secretaria,
    		
    			to_char(sifvigenciadtini,'dd/mm/YYYY') as sifvigenciadtini,
    			to_char(sifvigenciadtfim,'dd/mm/YYYY') as sifvigenciadtfim
					    
 			     		from sisfor.sisfor s
    			     		inner join public.unidade u on u.unicod = s.unicod
    			     		left join workflow.documento dctur on dctur.docid = s.docidcomposicaoequipe
    			     		left join workflow.estadodocumento ectur on ectur.esdid = dctur.esdid
    			     		left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
    			     		left join catalogocurso2014.curso cur on cur.curid = ieo.curid
    			     		left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
    			     		left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
    			     		left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
    			     		left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
    			     		left join seguranca.usuario usu on usu.usucpf = s.usucpf
    			     		left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
    			     		left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid
    			     		left join sisfor.outraatividade oat on oat.oatid = s.oatid
    			     		left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.oatid 
    		where s.sifid='".$_SESSION['sisfor']['sifid']."'";
    
    $arrCabecalho = $db->pegaLinha($sql);
    
    $perbolsistas = $db->pegaLinha("select to_char(min((fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,4,'0')||'-01')::date),'mm/YYYY') as inicio, to_char(max((fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,4,'0')||'-01')::date),'mm/YYYY') as fim from sisfor.folhapagamentoprojeto fp 
								     inner join sisfor.folhapagamento f on f.fpbid = fp.fpbid 
								     where fp.sifid='".$_SESSION['sisfor']['sifid']."'");
    
    $percursistas = $db->pegaLinha("select to_char(min((fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,4,'0')||'-01')::date),'mm/YYYY') as inicio, to_char(max((fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,4,'0')||'-01')::date),'mm/YYYY') as fim from sisfor.folhapagamentocursista fc
								     inner join sisfor.folhapagamento f on f.fpbid = fc.fpbid
								     where fc.sifid='".$_SESSION['sisfor']['sifid']."'");
    


    echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
    echo '<tr><td class="SubTituloDireita" width=20%>IES</td><td colspan=5>' . $arrCabecalho['universidade'] . '</td></tr>';
    echo '<tr><td class="SubTituloDireita" width=20%>Curso</td><td colspan=5>' . $arrCabecalho['curso'] . '</td></tr>';
    echo '<tr><td class="SubTituloDireita" width=20%>Coordenador do curso</td><td colspan=5>' . $arrCabecalho['coordenador'] . '</td></tr>';
    echo '<tr><td class="SubTituloDireita" width=20%>Vigência do projeto</td><td>' .(($arrCabecalho['sifvigenciadtini'] && $arrCabecalho['sifvigenciadtfim'])?$arrCabecalho['sifvigenciadtini'] . ' até ' . $arrCabecalho['sifvigenciadtfim']:'-') . '</td>
    		  <td class="SubTituloDireita" width=20%>Período de referência dos bolsistas</td><td>' . (($perbolsistas['inicio'] && $perbolsistas['fim'])?$perbolsistas['inicio'] . ' até ' . $perbolsistas['fim']:'Não cadastrado') . '</td>
    		  <td class="SubTituloDireita" width=20%>Período de referência dos cursistas</td><td>' . (($percursistas['inicio'] && $percursistas['fim'])?$percursistas['inicio'] . ' até ' . $percursistas['fim']:'Não cadastrado') . '</td></tr>';
    echo '</table>';
}

function listarPolosCurso($dados) {
    global $db;

    if (!$dados['consulta'])
        echo '<p><input type="button" value="Inserir polos" onclick="inserirPolos(\'\');"></p>';

    $sql = "SELECT '" . (($dados['consulta']) ? "" : "<img src=../imagens/alterar.gif style=\"cursor:pointer;\" onclick=\"inserirPolos(\''||p.polid||'\');\"> <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirPolo('||p.polid||');\">") . "' as acao, 
					polnome, 
					polnumvagas, 
					(array_to_string(array(SELECT '<input " . (($dados['consulta']) ? "disabled" : "") . " type=radio name=polo_'||a.polid||' value=\"'||m.muncod||'\" onclick=\"definirSede(this);\" '||CASE WHEN a.abrsede=true THEN 'checked' ELSE '' END||'> '||m.estuf||' / '||m.mundescricao FROM territorios.municipio m INNER JOIN sisfor.abrangenciacurso a ON a.muncod=m.muncod WHERE a.polid=p.polid ORDER BY m.estuf, m.mundescricao), '<br>')) 
			FROM sisfor.poloscurso p 
			WHERE sifid='" . $dados['sifid'] . "' AND polstatus='A'";
    $cabecalho = array("&nbsp;", "Nome do polo", "Número de vagas do polo", "Abrangência (Definir a sede)");
    $db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%', $par2);
}

function harevaPolo($dados) {
    global $db;

    $sql = "UPDATE sisfor.sisfor SET sifpossuipolo=" . (($dados['sifpossuipolo']) ? $dados['sifpossuipolo'] : "null") . " WHERE sifid='" . $dados['sifid'] . "'";
    $db->executar($sql);
    $db->commit();
}

function definirSede($dados) {
    global $db;

    $polid = substr($dados['polo'], 5);

    $sql = "UPDATE sisfor.abrangenciacurso SET abrsede=false WHERE polid='" . $polid . "'";
    $db->executar($sql);

    $sql = "UPDATE sisfor.abrangenciacurso SET abrsede=true WHERE polid='" . $polid . "' AND muncod='" . $dados['muncod'] . "'";
    $db->executar($sql);

    $db->commit();
}

function removerMunicipiosAbrangencia($dados) {
	global $db;
	
	$sql = "DELETE FROM sisfor.abrangenciacurso WHERE sifid='".$dados['sifid']."' AND polid IS NULL";
	$db->executar($sql);
	$db->commit();
	
}

function removerPolo($dados) {
    global $db;

    if ($dados['sifid'])
        $wh = "sifid='" . $dados['sifid'] . "'";
    if ($dados['polid'])
        $wh = "polid='" . $dados['polid'] . "'";

    $sql = "DELETE FROM sisfor.abrangenciacurso WHERE polid IN(SELECT polid FROM sisfor.poloscurso WHERE {$wh})";
    $db->executar($sql);

    $sql = "UPDATE sisfor.poloscurso SET polstatus='I' WHERE {$wh}";
    $db->executar($sql);
    $db->commit();
}

function inserirModeloPlanoAtividade($dados) {
    global $db;

    $sql = "INSERT INTO sisfor.atividadescurso(
            sifid, atidesc, atistatus, atiidpai)
			SELECT '" . $dados['sifid'] . "' as sifid, pamdsc, 'A', " . (($dados['atiidpai']) ? $dados['atiidpai'] : "NULL") . " as atiidpai FROM sisfor.planoatividadesmodelo WHERE pamid='" . $dados['pamid'] . "'";

    $db->executar($sql);
    $db->commit();
}

function atualizarEstruturaCurso($dados) {
    global $db;

    if ($dados['atidatainicio']) {
        foreach ($dados['atidatainicio'] as $atiid => $dt) {
            $sql = "UPDATE sisfor.atividadescurso SET atidatainicio='" . formata_data_sql($dt) . "' WHERE atiid='" . $atiid . "'";
            $db->executar($sql);
        }
    }

    if ($dados['atidatafim']) {
        foreach ($dados['atidatafim'] as $atiid => $dt) {
            $sql = "UPDATE sisfor.atividadescurso SET atidatafim='" . formata_data_sql($dt) . "' WHERE atiid='" . $atiid . "'";
            $db->executar($sql);
        }
    }


    $sql = "UPDATE sisfor.sisfor SET sifobsplanoatividades='" . substr($dados['sifobsplanoatividades'], 0, 5000) . "',
									 sifseduc=" . $dados['sifseduc'] . ",
									 sifseducjustificativa='" . $dados['sifseducjustificativa'] . "',
									 sifundime=" . $dados['sifundime'] . ",
									 sifundimejustificativa='" . $dados['sifundimejustificativa'] . "',
									 sifuncme=" . $dados['sifuncme'] . ",
									 sifuncmejustificativa='" . $dados['sifuncmejustificativa'] . "',
									 sifforumestadualpermanente=" . $dados['sifforumestadualpermanente'] . ",
									 sifforumestadualpermanentejustificativa='" . $dados['sifforumestadualpermanentejustificativa'] . "',
									 sifmsoc='" . $dados['sifmsoc'] . "',
									 sifoutrasarticulacoes=" . (($dados['sifoutrasarticulacoes']) ? "'" . $dados['sifoutrasarticulacoes'] . "'" : "NULL") . "
			WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "'";

    $db->executar($sql);

    $db->commit();


    echo "<script>
			alert('Dados da estrura do curso foram gravados com sucesso');
			window.location='sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=estrutura_curso';
		  </script>";
}

function carregarListaCustos($dados) {
    global $db;
    
    
    if($dados['execucao']) {
    
    	$sql = "(
				SELECT g.gdedesc,
					   'Verba',
					   '<input type=\"text\" style=\"text-align:;\" name=\"valorprevisto_'||o.orcid||'\" size=\"16\" maxlength=\"14\" value=\"'||trim(to_char(o.orcvlrunitario,'999g999g999d99'))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"valorprevisto_'||o.orcid||'\" title=\"Valor previsto\" readonly=\"readonly\" class=\" disabled\">' as valorprevisto,
					   '<input type=\"text\" style=\"text-align:;\" name=\"orcvalorexecutado['||o.orcid||']\" size=\"16\" maxlength=\"14\" value=\"'||CASE WHEN (o.orcvlrexecutado IS NULL OR o.orcvlrexecutado=0) THEN '0,00' ELSE trim(coalesce(to_char(o.orcvlrexecutado,'999g999g999d99'),'')) END||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);calcularOrcamentoExecucao();\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"valorexecutado_'||o.orcid||'\" title=\"Valor executado\" ".(($dados['consulta'])?"readonly=\"readonly\" class=\" disabled\"":" class=\" normal\"").">' as valorexecutado,
					   '<input type=\"text\" style=\"text-align:;\" name=\"porcorcvalorexecutado['||o.orcid||']\" size=\"4\" maxlength=\"3\" value=\"'||CASE WHEN o.orcvlrunitario>0 THEN round((o.orcvlrexecutado/o.orcvlrunitario)*100,1) ELSE 0 END||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"porcvalorexecutado_'||o.orcid||'\" title=\"% Valor executado\" readonly=\"readonly\" class=\" disabled\">' as porcvalorexecutado,
					   '<input type=\"text\" style=\"text-align:;\" name=\"saldo_'||o.orcid||'\" size=\"16\" maxlength=\"14\" value=\"'||CASE WHEN o.orcvlrunitario-coalesce(orcvlrexecutado,0) != 0 THEN trim(to_char(o.orcvlrunitario-coalesce(orcvlrexecutado,0),'999g999g999d99')) ELSE '0,00' END||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"saldo_'||o.orcid||'\" title=\"Saldo\" readonly=\"readonly\" class=\" disabled\">' as saldo
				FROM sisfor.orcamento o
				INNER JOIN sisfor.grupodespesa g ON g.gdeid = o.gdeid
				WHERE o.sifid='".$dados['sifid']."' AND o.orcstatus='A'
				ORDER BY g.gdedesc
				) UNION ALL (
    
				SELECT '<b>TOTAIS</b>' as tot,
					   '&nbsp;' as tot2,
					   '<input type=\"text\" style=\"text-align:;\" name=\"totalvalorprevisto\" size=\"16\" maxlength=\"14\" value=\"'||trim(to_char(SUM(o.orcvlrunitario),'999g999g999d99'))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"totalvalorprevisto\" title=\"Total Valor Previsto\" readonly=\"readonly\" class=\" disabled\">' as totalvalorprevisto,
					   '<input type=\"text\" style=\"text-align:;\" name=\"totalvalorexecutado\" size=\"16\" maxlength=\"14\" value=\"'||trim(coalesce(to_char(SUM(o.orcvlrexecutado),'999g999g999d99'),''))||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"totalvalorexecutado\" title=\"Total Valor executado\" readonly=\"readonly\" class=\" disabled\">' as totalvalorexecutado,
					   '<input type=\"text\" style=\"text-align:;\" name=\"porctotalvalorexecutado\" size=\"4\" maxlength=\"3\" value=\"'||CASE WHEN SUM(o.orcvlrunitario)>0 THEN round((SUM(o.orcvlrexecutado)/SUM(o.orcvlrunitario))*100,1) ELSE 0.00 END||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"porctotalvalorexecutado\" title=\"% Total Valor executado\" readonly=\"readonly\" class=\" disabled\">' as xx,
					   '<input type=\"text\" style=\"text-align:;\" name=\"totalsaldo\" size=\"16\" maxlength=\"14\" value=\"'||CASE WHEN (SUM(o.orcvlrunitario)-coalesce(SUM(o.orcvlrexecutado),0)) !=0 THEN trim(to_char(SUM(o.orcvlrunitario)-coalesce(SUM(o.orcvlrexecutado),0),'999g999g999d99')) ELSE '0,00' END||'\" onkeyup=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" id=\"totalsaldo\" title=\"Saldo\" readonly=\"readonly\" class=\" disabled\">' as totalsaldo
				FROM sisfor.orcamento o
				INNER JOIN sisfor.grupodespesa g ON g.gdeid = o.gdeid
				WHERE o.sifid='".$dados['sifid']."' AND o.orcstatus='A'
    
				)";
    
    	$cabecalho = array("Elementos de Despesa","Unidade de medida","Valor previsto (R$)","Valor executado (R$)","% Valor executado","Saldo (R$)");
    	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','S');
    
    
    } else {
    
    
	    if($dados['alpid']) {
	    	
	    	$alpautorizado = $db->pegaUm("SELECT alpautorizado FROM sisfor.alterarprojeto WHERE alpid='".$dados['alpid']."'");
	    	
	    	if($alpautorizado) $dados['consulta'] = true;
	    	
	    	$fnc = "_s";
	    	
	    	$wh=" AND o.orcstatus IN('S','A') AND alpid='".$dados['alpid']."'";
	    } else {
	    	$wh=" AND o.orcstatus='A'";
	    }
	
	
	    $sql = "SELECT " . (($dados['consulta']) ? "''" : "'<center><img src=../imagens/alterar.gif style=\"cursor:pointer;\" onclick=\"inserirCustos{$fnc}(\''||o.orcid||'\');\"> <img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirCustos{$fnc}(\''||o.orcid||'\');\"></center>'") . " as acao, g.gdedesc, 'Verba', o.orcvlrunitario, o.orcdescricao
				FROM sisfor.orcamento o
				INNER JOIN sisfor.grupodespesa g ON g.gdeid = o.gdeid
				WHERE o.sifid='" . $dados['sifid'] . "' {$wh}
				ORDER BY g.gdedesc";
	    $cabecalho = array("&nbsp;", "Grupo de Despesa", "Unidade de Medida", "Valor total (R$)", "Detalhamento");
	    $db->monta_lista_simples($sql, $cabecalho, 50, 5, 'S', '100%', 'S');
    
    }
}

function atualizarCusto($dados) {
    global $db;
    $sql = "UPDATE sisfor.orcamento SET gdeid='" . $dados['gdeid'] . "', 
    									orcvlrunitario='" . (($dados['orcvlrunitario'])?str_replace(array(".", ","), array("", "."), $dados['orcvlrunitario']):'0.00') . "',
    									orcvlrloa2014= '" . str_replace(array(".", ","), array("", "."), $dados['orcvlrloa2014']) . "',
    									orcvlrloa2015= '" . str_replace(array(".", ","), array("", "."), $dados['orcvlrloa2015']) . "',
    									orcvlrloa2016= '" . str_replace(array(".", ","), array("", "."), $dados['orcvlrloa2016']) . "',
    									orcdescricao='" . $dados['orcdescricao'] . "'
			WHERE orcid='" . $dados['orcid'] . "'";

    $db->executar($sql);
    $db->commit();
    
    if($dados['alpid']) $al = array("alert" => "Custo inserido com sucesso", "javascript" => "window.opener.carregarListaCustos_s(".$dados['alpid'].");window.close();");
    else $al = array("alert" => "Custo inserido com sucesso", "javascript" => "window.opener.carregarListaCustos();window.close();");

    alertlocation($al);
}

function excluirCustos($dados) {
    global $db;
    $sql = "DELETE FROM sisfor.orcamento WHERE orcid='" . $dados['orcid'] . "'";
    $db->executar($sql);
    $db->commit();
}

function inserirCusto($dados) {
    global $db;
    $sql = "INSERT INTO sisfor.orcamento(
            sifid, gdeid, orcvlrunitario, orcvlrloa2014, orcvlrloa2015, orcvlrloa2016, 
            orcstatus, orcdescricao, alpid)
    		VALUES ('" . $dados['sifid'] . "', 
    				'" . $dados['gdeid'] . "', 
    				'" . str_replace(array(".", ","), array("", "."), $dados['orcvlrunitario']) . "',
    				'" . str_replace(array(".", ","), array("", "."), $dados['orcvlrloa2014']) . "',
    				'" . str_replace(array(".", ","), array("", "."), $dados['orcvlrloa2015']) . "',
    				'" . str_replace(array(".", ","), array("", "."), $dados['orcvlrloa2016']) . "', 
    				'".(($dados['alpid'])?"S":"A")."', 
    				'" . $dados['orcdescricao'] . "',
    				".(($dados['alpid'])?"'".$dados['alpid']."'":"NULL").");";

    $db->executar($sql);
    $db->commit();
	
    if($dados['alpid']) $al = array("alert" => "Item inserido com sucesso.", "javascript" => "window.opener.carregarListaCustos_s(".$dados['alpid'].");window.close();");
    else $al = array("alert" => "Item inserido com sucesso.", "javascript" => "window.opener.carregarListaCustos();window.close();");
    alertlocation($al);
}

function carregarOrcamento($dados) {
    global $db;
    $sql = "SELECT * FROM sisfor.orcamento o
			INNER JOIN sisfor.grupodespesa g ON o.gdeid = g.gdeid
			WHERE orcid='" . $dados['orcid'] . "'";

    $orcamento = $db->pegaLinha($sql);

    return $orcamento;
}

function inserirQtdEquipeIES($dados) {
    global $db;

    if ($_FILES['arquivo']['error'] == 0) {

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec("sisfor", array(), "sisfor", false);
        $file->setUpload(NULL, "arquivo", false);
        $arqid = $file->getIdArquivo();

        $db->executar("UPDATE sisfor.sisfor SET arqidmemoria='" . $arqid . "' WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "'");
    }

    if ($dados['epiqtd']) {
        foreach ($dados['epiqtd'] as $pflcod => $epiqtd) {

            $epiid = $db->pegaUm("SELECT epiid FROM sisfor.equipeies WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "' AND pflcod='" . $pflcod . "'");

            if ($epiid) {

                $sql = "UPDATE sisfor.equipeies SET epiqtd=" . (($epiqtd) ? $epiqtd : '0') . ", epivalor=" . (($dados['epivalor'][$pflcod]) ? str_replace(array(".", ","), array("", "."), $dados['epivalor'][$pflcod]) : '0.00') . " WHERE epiid='" . $epiid . "'";

                $db->executar($sql);
            } else {

                $sql = "INSERT INTO sisfor.equipeies(
            			sifid, pflcod, epiqtd, epivalor)
    					VALUES ('" . $_SESSION['sisfor']['sifid'] . "', '" . $pflcod . "', " . (($epiqtd) ? $epiqtd : '0') . ", " . (($dados['epivalor'][$pflcod]) ? str_replace(array(".", ","), array("", "."), $dados['epivalor'][$pflcod]) : '0.00') . ");";

                $db->executar($sql);
            }
        }

        $db->commit();
    }

    $al = array("alert" => "Valores gravados com sucesso.", "location" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=equipeies");
    alertlocation($al);
}

function downloadArquivo($dados) {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec("sisfor", NULL, "sisfor");
    $file->getDownloadArquivo($dados['arqid']);
}

function inserirArquivoCalculadora($dados) {
    global $db;

    if ($_FILES['arquivo']['error'] == 0) {

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $campos = array("tcadsc" => "'" . $dados['tcadsc'] . "'", "tcasecretaria" => "'" . $dados['tcasecretaria'] . "'");
        $file = new FilesSimec("tabelacalculadora", $campos, "sisfor");
        $file->setUpload(NULL, "arquivo");
    }


    $al = array("alert" => "Arquivo inserido com sucesso", "location" => "sisfor.php?modulo=principal/gerenciartabelascalc&acao=A");
    alertlocation($al);
}

function carregarQuantitativoPorPerfil($dados) {
    global $db;

    if($dados['alpid']) {
    	$sql = "(
				SELECT pp.pfldsc as perfil, round(p.plpvalor,2)::text, '<input " . (($dados['consulta']) ? "disabled" : "") . " type=\"text\" style=\"text-align:;float:right;\" value=\"'|| COALESCE((SELECT epiqtd::text FROM sisfor.equipeies_s WHERE pflcod=p.pflcod AND sifid='" . $dados['sifid'] . "' AND alpid='".$dados['alpid']."'),'') ||'\" name=\"epiqtd_s['||p.pflcod||']\" size=\"5\" maxlength=\"4\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onkeyup=\"this.value=mascaraglobal(\'####\',this.value);preencherTotais();\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'####\',this.value);\" title=\"Qtd. Bolsas\" " . (($consulta) ? "class=\"disabled\" readonly=\"readonly\"" : "class=\"obrigatorio normal\"") . " id=\"epiqtd_s_'||p.pflcod||'\"> <input type=\"hidden\" name=\"dadosperfil_s[]\" id=\"'||p.pflcod||'\" value=\"'||p.plpvalor||'\">' as qtd, '<input type=\"text\" style=\"text-align:;float:right;\" name=\"epivalor_s['||p.pflcod||']\" size=\"15\" maxlength=\"14\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'###.###.###,##\',this.value);\" title=\"Valor total\" class=\"disabled\" readonly=\"readonly\" id=\"epivalor_s_'||p.pflcod||'\" value=\"'|| COALESCE((SELECT CASE WHEN epivalor=0 THEN '0,00' ELSE trim(to_char(epivalor,'999g999g999d99')) END as epivalor FROM sisfor.equipeies_s WHERE pflcod=p.pflcod AND sifid='" . $dados['sifid'] . "' AND alpid='".$dados['alpid']."'),'') ||'\">' as vlr FROM sisfor.pagamentoperfil p
				INNER JOIN seguranca.perfil pp ON pp.pflcod = p.pflcod WHERE pp.pflcod!='".PFL_COORDENADOR_INST."' ORDER BY pp.pflnivel
				) UNION ALL (
				SELECT '', '<span style=float:right><b>Totais:</b></span>', '<input type=\"text\" style=\"text-align:;float:right;\" value=\"'|| COALESCE((SELECT sum(epiqtd)::text FROM sisfor.equipeies_s WHERE sifid='" . $dados['sifid'] . "' ANd alpid='".$dados['alpid']."'),'') ||'\" id=\"epiqtdtotal_s\" size=\"5\" maxlength=\"4\" class=\"disabled\" readonly=\"readonly\">' as totalqtd, '<input type=\"text\" style=\"text-align:;float:right;\" value=\"'|| COALESCE((SELECT trim(to_char(sum(epivalor),'999g999g999d99')) as epivalor FROM sisfor.equipeies_s WHERE sifid='" . $dados['sifid'] . "' AND alpid='".$dados['alpid']."'),'') ||'\" id=\"epivalortotal_s\" size=\"15\" maxlength=\"14\" class=\"disabled\" readonly=\"readonly\">' as totalvalor
				)";
    } else {
    	
		if($dados['sifid']) $_SESSION['sisfor']['sifid'] = $dados['sifid']; 
    	
	    $sql = "(
				SELECT pp.pfldsc as perfil, round(p.plpvalor,2)::text, '<input " . (($dados['consulta']) ? "disabled" : "") . " type=\"text\" style=\"text-align:;float:right;\" value=\"'|| COALESCE((SELECT epiqtd::text FROM sisfor.equipeies WHERE pflcod=p.pflcod AND sifid='" . $_SESSION['sisfor']['sifid'] . "'),'') ||'\" name=\"epiqtd['||p.pflcod||']\" size=\"5\" maxlength=\"4\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onkeyup=\"this.value=mascaraglobal(\'####\',this.value);preencherTotais();\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'####\',this.value);\" title=\"Qtd. Bolsas\" " . (($consulta) ? "class=\"disabled\" readonly=\"readonly\"" : "class=\"obrigatorio normal\"") . " id=\"epiqtd_'||p.pflcod||'\"> <input type=\"hidden\" name=\"dadosperfil[]\" id=\"'||p.pflcod||'\" value=\"'||p.plpvalor||'\">' as qtd, '<input type=\"text\" style=\"text-align:;float:right;\" name=\"epivalor['||p.pflcod||']\" size=\"15\" maxlength=\"14\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);this.value=mascaraglobal(\'###.###.###,##\',this.value);\" title=\"Valor total\" class=\"disabled\" readonly=\"readonly\" id=\"epivalor_'||p.pflcod||'\" value=\"'|| COALESCE((SELECT CASE WHEN epivalor=0 THEN '0,00' ELSE trim(to_char(epivalor,'999g999g999d99')) END as epivalor FROM sisfor.equipeies WHERE pflcod=p.pflcod AND sifid='" . $_SESSION['sisfor']['sifid'] . "'),'') ||'\">' as vlr FROM sisfor.pagamentoperfil p
				INNER JOIN seguranca.perfil pp ON pp.pflcod = p.pflcod WHERE pp.pflcod!='".PFL_COORDENADOR_INST."' ORDER BY pp.pflnivel
				) UNION ALL (
				SELECT '', '<span style=float:right><b>Totais:</b></span>', '<input type=\"text\" style=\"text-align:;float:right;\" value=\"'|| COALESCE((SELECT sum(epiqtd)::text FROM sisfor.equipeies WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "'),'') ||'\" id=\"epiqtdtotal\" size=\"5\" maxlength=\"4\" class=\"disabled\" readonly=\"readonly\">' as totalqtd, '<input type=\"text\" style=\"text-align:;float:right;\" value=\"'|| COALESCE((SELECT trim(to_char(sum(epivalor),'999g999g999d99')) as epivalor FROM sisfor.equipeies WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "'),'') ||'\" id=\"epivalortotal\" size=\"15\" maxlength=\"14\" class=\"disabled\" readonly=\"readonly\">' as totalvalor
				)";
    }

    $cabecalho = array("Função", "Valor unitário (R$)", "Qtd. Bolsas", "Valor total (R$)");
    $db->monta_lista_simples($sql, $cabecalho, 1000, 5, 'N', '100%', '', true, false, false, false);
}

function anexarProjetoCurso($dados) {
    global $db;

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $campos = array("sifid" => "'" . $_SESSION['sisfor']['sifid'] . "'");
    $file = new FilesSimec("anexoProjetoCurso", $campos, "sisfor");
    $file->setUpload(NULL, "arquivo");

    $al = array("alert" => "Documento gravado com sucesso", "location" => "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=dados_projeto");
    alertlocation($al);
}

function dataLimiteTramitacao() {
	global $db;
	
	$siftipoplanejamento = $db->pegaUm("SELECT siftipoplanejamento FROM sisfor.sisfor WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "'");
	
	if($siftipoplanejamento!='3')
		if(date("Y-m-d H:i:s") > '2014-11-17 10:00:00') return 'As tramitações dos projetos foram interrompidas';
	
	return true;
}

function condicaoEnviarProjetoAnalise() {
    global $db;
    
    $perfis = pegaPerfilGeral();
    
    if($_SESSION['usucpf']!='' && $_SESSION['usucpf']!='' && !in_array(PFL_ADMINISTRADOR,$perfis) && !$db->testa_superuser()) {// gatilho solicitado pela manuelita
    	 
    	$dt = dataLimiteTramitacao();
    
    	if(strlen($dt)>1) return $dt;
    
    }
    
    $siftipoplanejamento = $db->pegaUm("SELECT siftipoplanejamento FROM sisfor.sisfor WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "'");
    
    if($siftipoplanejamento=='2') {
    	
    	$sql = "SELECT sieid FROM sisfor.sisfories s 
				INNER JOIN workflow.documento d ON s.docidplan2 = d.docid 
				WHERE d.esdid!=1225 AND s.unicod IN(SELECT unicod FROM sisfor.sisfor WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "')";
    	
    	$sieid = $db->pegaUm($sql);
    	
    	if(!$sieid) return 'IES não finalizou o planejamento 2';
    	
    }
    
    $totalplan = $db->pegaUm("SELECT (COALESCE(sifvalorloa,0) + COALESCE(sifvaloroutras,0))::numeric(20,2) as totalplan FROM sisfor.sisfor WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "'");
    $orcvlrtotal = $db->pegaUm("SELECT SUM(orcvlrunitario) as orcvlrtotal FROM sisfor.orcamento WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "' AND orcstatus='A'");
    
    if(!$orcvlrtotal) {
    	return 'Orçamento detalhado deve ser cadastrado';
    }

    if ($orcvlrtotal > $totalplan) {
        return 'O orçamento detalhado não pode ser maior que o planejamento da IES';
    }
    
    $sifsomenteorcamentoobr = $db->pegaUm("SELECT sifsomenteorcamentoobr FROM sisfor.sisfor WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "'");
    
    if($sifsomenteorcamentoobr=='t') {
    	return true;
    }

    $sifnumvagasofertadas = $db->pegaUm("SELECT sifnumvagasofertadas FROM sisfor.sisfor WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "'");
    $polnumvagas = $db->pegaUm("SELECT SUM(polnumvagas) as polnumvagas FROM sisfor.poloscurso WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "' AND polstatus='A'");

    if ($polnumvagas > $sifnumvagasofertadas) {
        return 'Número de vagas distribuidas nos polos não pode ser maior que as vagas ofertadas';
    }
    
    $dadosprojeto = $db->pegaLinha("SELECT sifid FROM sisfor.sisfor 
    						  WHERE sifid='".$_SESSION['sisfor']['sifid']."' AND 
    						  		(sifprofmagisterio IS NULL OR 
    		                         sifnumvagasofertadas IS NULL OR 
    							     sifvigenciadtini IS NULL OR 
    								 sifvigenciadtfim IS NULL OR 
    								 siforigemrecursos IS NULL)");
    
    if($dadosprojeto) {
    	return 'Preencher as informações da tela Dados gerais do projeto';
    }

    $sifpossuipolo = $db->pegaUm("SELECT sifpossuipolo FROM sisfor.sisfor WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "'");

    if ($sifpossuipolo == "t") {
        $poloscadastrados = $db->carregarColuna("SELECT polid FROM sisfor.poloscurso WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "' AND polstatus='A'");

        if (!count($poloscadastrados))
            return 'O projeto possui polos, porém não foram cadastrados nenhum polo';

        if ($poloscadastrados) {
            foreach ($poloscadastrados as $polid) {
                $qtdabrpolo = $db->pegaUm("SELECT count(*) FROM sisfor.abrangenciacurso WHERE polid='" . $polid . "'");
                if (!$qtdabrpolo)
                    return 'Existem polos sem abrangência';
            }
        }
    } elseif ($sifpossuipolo == "f") {

        $qtdabr = $db->pegaUm("SELECT count(*) FROM sisfor.abrangenciacurso WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "'");
        if (!$qtdabr)
            return 'Projeto sem abrangência';
    }

    return true;
}

function verificarPerfilBolsa($iusd) {
    global $db;

    $sql = "SELECT DISTINCT pflcod FROM sisfor.tipoperfil WHERE iusd = {$iusd} AND tpestatus = 'A'";
    $arrPflcod = $db->carregar($sql);
    !$arrPflcod ? $arrPflcod = array() : $arrPflcod = $arrPflcod;
    $arrBolsa = array();
    foreach ($arrPflcod as $pflcod) {
        $arrBolsa[] = $pflcod['pflcod'];
    }
    return $arrBolsa;
}

function verificarBolsa($iusd, $pflcod) {
    global $db;

    if ($pflcod == PFL_COORDENADOR_INST) {
        $tpebolsa = "tpebolsa AS tpebolsaies";
    } else {
        $tpebolsa = "tpebolsa AS tpebolsacurso";
    }

    $sql = "SELECT DISTINCT pflcod , $tpebolsa FROM sisfor.tipoperfil WHERE iusd = {$iusd} AND tpestatus = 'A' AND pflcod = {$pflcod}";
    $bolsa = $db->pegaLinha($sql);
    return $bolsa;
}

function salvarBolsa($post) {
    global $db;

    extract($post);

    $sql = "UPDATE 	sisfor.tipoperfil 
			SET 	tpebolsa = '{$tpebolsaies}'
			WHERE 	pflcod = " . PFL_COORDENADOR_INST . " AND iusd = {$iusd}";
    $db->executar($sql);

    $sql = "UPDATE 	sisfor.tipoperfil 
			SET 	tpebolsa = '{$tpebolsacurso}'	
			WHERE 	pflcod = " . PFL_COORDENADOR_CURSO . " AND iusd = {$iusd}";
    $db->executar($sql);

    ($tpebolsaies == 't') ? $pflcod = PFL_COORDENADOR_INST : $pflcod = PFL_COORDENADOR_CURSO;
    ($tpebolsaies == 'f') ? $pflcoda = PFL_COORDENADOR_INST : $pflcoda = PFL_COORDENADOR_CURSO;

    if ($tpebolsaies == 't' || $tpebolsacurso == 't') {
        $sql = "SELECT pboid FROM sisfor.perfilbolsalog WHERE iusd = '{$iusd}' AND pbostatus = 'A'";
        $pboid = $db->pegaUm($sql);

        if (empty($pboid)) {
            $sql = "INSERT INTO 	sisfor.perfilbolsalog (iusd, pflcod, usucpf, pbodatagravacao, pbostatus)
	    			VALUES 							   ($iusd, $pflcod, '{$iuscpf}', now(), 'A')";
            $db->executar($sql);
        } else {
            $sql = "UPDATE sisfor.perfilbolsalog SET pbostatus = 'I' WHERE pflcod = {$pflcoda} AND usucpf = '{$iuscpf}'";
            $db->executar($sql);
            $sql = "INSERT INTO 	sisfor.perfilbolsalog (iusd, pflcod, usucpf, pbodatagravacao, pbostatus)
	    			VALUES 							   ($iusd, $pflcod, '{$iuscpf}', now(), 'A')";
            $db->executar($sql);
        }
    }

    $db->commit();

    if ($tipo == 'ies') {
        $location = "sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=dadosies";
    } else {
        $location = "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=dadoscurso";
    }
    $al = array("alert" => "Bolsa Selecionada com Sucesso!", "location" => $location, "javascript" => "window.opener.location.reload();window.close();");
    alertlocation($al);
}

function montarFormularioEquipe($pflcod, $tpeid = null) {
    global $db;

    $estadoAtual = wf_pegarEstadoAtual($_SESSION['sisfor']['docidcomposicaoequipe']);
    $podeEditar = $estadoAtual['esdid'] == ESD_CADASTRAMENTO_FINALIZADO ? 'N' : 'S';

    $acoes = "''";
    if ('S' == $podeEditar) {
        $acoes = "'<a style=\"margin: 0 -5px 0 5px;\" class=\"carregar_dados\" pflcod=\"{$pflcod}\" href=\"/sisfor/ajax.php?atualizarFormulario=1&tpeid=' || tp.tpeid || '&pflcod={$pflcod}\" ><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\"></a>
                     <a style=\"margin: 0 -5px 0 5px;\" class=\"deleta_tipo_perfil\" pflcod=\"{$pflcod}\" href=\"/sisfor/ajax.php?deletaTipoPerfil=1&tpeid=' || tp.tpeid || '&pflcod={$pflcod}\" ><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>'";
    }

    $sql = "select  {$acoes} as acao,
                    tp.tpeid, iu.iusnome, iu.iuscpf, iu.iusemailprincipal, tp.tpeqtdbolsa, (select count(*) from sisfor.pagamentobolsista where tpeid=tp.tpeid) as tpeqtdbolsapg
            from sisfor.tipoperfil tp
                    inner join sisfor.identificacaousuario iu on iu.iusd = tp.iusd
            where tp.sifid = '{$_SESSION['sisfor']['sifid']}'
            and tp.pflcod = '$pflcod' order by iu.iusnome";
    $dados = $db->carregar($sql);

    $totalBolsas = 0;
    $carregar = array('iusd' => null, 'iusnome' => null, 'iusemailprincipal' => null, 'tpeqtdbolsa' => null);
    if ($dados && is_array($dados)) {
        foreach ($dados as $key => $dado) {
            $totalBolsas += $dado['tpeqtdbolsa'];
            if ($tpeid == $dado['tpeid']) {
                $carregar['tpeid'] = $dado['tpeid'];
                $carregar['iuscpf'] = $dado['iuscpf'];
                $carregar['iusnome'] = $dado['iusnome'];
                $carregar['iusemailprincipal'] = $dado['iusemailprincipal'];
                $carregar['tpeqtdbolsa'] = $dado['tpeqtdbolsa'];
            }
            unset($dados[$key]['tpeid']);
        }
    } else {
        $dados = array();
    }
    ?>

    <form method="post" id="formulario_equipe_<?php echo $pflcod; ?>" class="formulario-equipe">
        <input type="hidden" name="tpeid" id="tpeid" value="<?php echo $carregar['tpeid'] ?>">
        <input type="hidden" name="bolsas_original" id="qtd_original_<?php echo $pflcod ?>" value="<?php echo $carregar['tpeqtdbolsa'] ?>">
        <input type="hidden" name="pflcod" id="pflcod" value="<?php echo $pflcod; ?>">
        <input type="hidden" name="envioForm" value="1">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">
            <tr>
                <td class="SubTituloDireita" width="10%">CPF</td>
                <td><?php echo campo_texto('iuscpf', "S", $podeEditar, "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf_' . $pflcod . '" pflcod="' . $pflcod . '" tipo="cpf_bolsista"', '', $carregar['iuscpf']); ?></td>
                <td rowspan="5" width="60%" valign="top">
                    <?php
                    $cabecalho = array('Ação', 'Nome', 'CPF', 'E-mail', 'Qtd. Bolsa','Qtd. Bolsa PG');
                    $db->monta_lista_simples($dados, $cabecalho, 5000, 5000, 'N', '100%');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Nome</td>
                <td><?php echo campo_texto('iusnome', "S", "N", "Nome", 40, 150, "", "", '', '', 0, 'id="iusnome_' . $pflcod . '"', '', $carregar['iusnome']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">E-mail</td>
                <td><?php echo campo_texto('iusemailprincipal', "S", $podeEditar, "Principal", 40, 60, "", "", '', '', 0, 'id="iusemailprincipal_' . $pflcod . '"', '', $carregar['iusemailprincipal']); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Qtd Bolsas</td>
                <td><?php echo campo_texto('tpeqtdbolsa', "S", $podeEditar, "Quantidade de Bolsas", 3, 60, "", "", '', '', 0, 'id="tpeqtdbolsa_' . $pflcod . '" pflcod="' . $pflcod . '" tipo="qtd_bolsas"', '', $carregar['tpeqtdbolsa']); ?></td>
            </tr>
            <?php if ('S' == $podeEditar) { ?>
                <tr>
                    <td align="center" bgcolor="#CCCCCC" colspan="2">
                        <input type="button" pflcod="<?php echo $pflcod; ?>" class="botao_enviar"  name="salvar" value="Salvar">
                    </td>
                </tr>
            <?php } ?>
        </table>
    </form>

    <script type="text/javascript">
        jQuery(function() {
            var bolsasRestantes = parseInt(jQuery('#bolsas_ofertadas_<?php echo $pflcod; ?>').html()) - parseInt('<?php echo $totalBolsas; ?>');
            jQuery('#bolsas_restantes_<?php echo $pflcod; ?>').html(bolsasRestantes);
        });
    </script>
    <?php
}

function montarListaEquipe() {
    global $db;
    $acoes = "";
        $acoes = "' <input type=\"checkbox\" name=\"chk['||foo.pflcod||'][]\" value=\"'||foo.iuscpf||'\"> 
                    <a style=\"margin: 0 -5px 0 5px;\" class=\"carregar_dados\"  href=\"/sisfor/ajax.php?atualizarFormulario=1&tpeid=' || foo.tpeid || '&pflcod={1}\" ><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\"></a>
                     <a style=\"margin: 0 -5px 0 5px;\" class=\"deleta_tipo_perfil\" href=\"/sisfor/ajax.php?deletaTipoPerfil=1&tpeid=' || foo.tpeid || '&pflcod={1}\" ><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>'";
    
        $sql = "select  {$acoes} as acao,  
                        CASE WHEN (foo.status='A' AND foo.perfil IS NOT NULL) THEN 'Ativo'
                             WHEN foo.status='B' THEN 'Bloqueado'
                             WHEN foo.status='P' THEN 'Pendente' 
                             ELSE 'Não cadastrado' END as situacao,
                        foo.iusnome, foo.iuscpf, foo.iusemailprincipal, foo.pfldsc
                from (
                    select
                    (SELECT usu.suscod from seguranca.usuario_sistema usu WHERE usu.usucpf = iu.iuscpf and usu.sisid = ".SIS_SISFOR.") as status,
                    (SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=iu.iuscpf AND pflcod=tp.pflcod) as perfil,
                    tp.tpeid, iu.iusnome, iu.iuscpf, iu.iusemailprincipal, p.pfldsc, p.pflcod
                   
                    from sisfor.tipoperfil tp
                    inner join sisfor.identificacaousuario iu on iu.iusd = tp.iusd
                    inner join sisfor.equipeies e on e.pflcod = tp.pflcod
                    inner join seguranca.perfil p on p.pflcod = e.pflcod
                    where e.sifid = '{$_SESSION['sisfor']['sifid']}' and tp.sifid = '{$_SESSION['sisfor']['sifid']}'
                ) foo  
                     ";

     $cabecalho = array('Ação','Status', 'Nome', 'CPF', 'E-mail', 'Cargo');
     $db->monta_lista($sql,$cabecalho,100,10,'N','center','N','formulario','','',null,array('ordena'=>false));
                    
     echo '<p align=center>
                <input type="button" id="ativarmarcados" value="Ativar Marcados" onclick="enviarMarcado(\'A\');">
                <input type="button" id="bloquearmarcados" value="Bloquear Marcados" onclick="enviarMarcado(\'B\');">
           </p>';

}


function ativarEquipe($dados) {
	global $db;
	
	if($dados['chk']) {
		
		foreach($dados['chk'] as $pflcod => $cpfs) {
			
			foreach($cpfs as $cpf) {
				
				$sql = "SELECT * FROM sisfor.identificacaousuario WHERE iuscpf='".str_replace(array(".","-"),array(""),$cpf)."'";
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
			    
			    $db->executar("UPDATE sisfor.identificacaousuario SET iusstatus='A' WHERE iuscpf='".str_replace(array(".","-"),array(""),$cpf)."'");
			    
		 		$remetente = array("nome" => "SIMEC - MÓDULO SISFOR","email" => $identificacaousuario['iusemailprincipal']);
 				$destinatario = $identificacaousuario['iusemailprincipal'];
 				$usunome = $identificacaousuario['iusnome'];
 				$assunto = "Cadastro no SIMEC - MÓDULO SISFOR";
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
		 		
			    $existe_sis = $db->pegaUm("select usucpf from seguranca.usuario_sistema where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and sisid='".SIS_SISFOR."'");
			    	
			    if(!$existe_sis) {
			    		
			    	$sql = "INSERT INTO seguranca.usuario_sistema(
			         	    usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
			     			VALUES ('".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', ".SIS_SISFOR.", 'A', NULL, NOW(), 'A');";
				    	
			     	$db->executar($sql);
				    	
			    } else {
		 	    	$sql = "UPDATE seguranca.usuario_sistema SET susstatus='A', suscod='".$dados['suscod']."' WHERE usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' AND sisid='".SIS_SISFOR."'";
		 	    	$db->executar($sql);
			    }
			    
			    $sql = "INSERT INTO seguranca.historicousuario(htudsc, htudata, usucpf, sisid, suscod, usucpfadm)
    					VALUES ('Mudança realizada pela ferramenta de gerencia do SISFOR.', 
    							NOW(), 
    							'".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', 
    							'".$_SESSION['sisid']."', '".$dados['suscod']."', '".$_SESSION['usucpf']."');";
			    
	 	    	$db->executar($sql);
			    
                        $existe_pfl = $db->pegaUm("select usucpf from seguranca.perfilusuario where usucpf='".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."' and pflcod='".$pflcod."'");

                        if(!$existe_pfl) {
                            $sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".str_replace(array(".","-"),array(""),$identificacaousuario['iuscpf'])."', '".$pflcod."');";
                            $db->executar($sql);
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


function excluirCursistaAvaliacao($dados) {
	global $db;
	
	$db->executar("DELETE FROM sisfor.cursistaavaliacoes WHERE cucid='".$dados['cucid']."'");
	$db->executar("DELETE FROM sisfor.cursistacurso WHERE cucid='".$dados['cucid']."'");
	$db->commit();

}

function montarFormularioCursista($sifid, $fpbid, $iusd = null) {
    global $db;

    if(!$sifid) die("<p align=center><b>PROBLEMAS PARA IDENTIFICAR O CURSO. FAÇA O LOGOUT E TENTE NOVAMENTE.</b></p>");
    if($_REQUEST['curnome_buscar']) 	    $fil[] = "c.curnome ilike '%".$_REQUEST['curnome_buscar']."%'";
    if($_REQUEST['pendencias']) 		    $fil[] = "(cavparticipacao is null or cavsituacao is null or cavatividadesrealizadas is null)";
    if($_REQUEST['cursistasincompleto'])    $fil[] = "(curescolaridade is null or currede is null or curcontratacao is null or cursexo is null or curdatanascimento is null or curraca is null or curdeficiencia is null or curfuncao is null)";
    if($_REQUEST['cavparticipacao_buscar'] || $_REQUEST['cavparticipacao_buscar']==='0') $fil[] = "cavparticipacao='".str_replace(",",".",$_REQUEST['cavparticipacao_buscar'])."'";
    if($_REQUEST['cavatividadesrealizadas_buscar'] || $_REQUEST['cavatividadesrealizadas_buscar']==='0') $fil[] = "cavatividadesrealizadas='".str_replace(",",".",$_REQUEST['cavatividadesrealizadas_buscar'])."'";
    if($_REQUEST['cavsituacao_buscar']) 		    $fil[] = "cavsituacao='".$_REQUEST['cavsituacao_buscar']."'";
    
    
    $sql = "select c.curid, c.curnome, c.curcpf, c.curemail, cc.cucid, a.cavid, a.cavparticipacao,a.cavsituacao, a.cavatividadesrealizadas, a.iusdorientador, a.cavtotal, cc.iusdavaliador, cc.estuf, cc.muncod
            from sisfor.cursistacurso cc
                inner join sisfor.cursista c on c.curid = cc.curid
                left  join sisfor.cursistaavaliacoes a on a.cucid = cc.cucid
            where cc.sifid = '$sifid' ".(($iusd)?" and cc.iusdavaliador='".$iusd."'":"")." ".(($fil)?"and ".implode(" and ", $fil):"")."
            and cc.fpbid = '$fpbid'";
    
   $total_cursistas = $db->pegaUm("select count(*) from ({$sql}) foo");
   
   $sql .= " order by c.curnome 
        	 limit 100 offset ".(($_REQUEST['pgina'])?($_REQUEST['pgina']*100):"0");

    $dados = $db->carregar($sql);

    $estadoAtual['esdid'] = null;
    if($_REQUEST['fpbid']){
        $docid = pegarDocidAvaliacaoCursista($sifid, $fpbid);
        $estadoAtual = wf_pegarEstadoAtual($docid);
    }

    $podeEditar = ESD_EM_CADASTRAMENTO == $estadoAtual['esdid'] ? 'S' : 'N';

    $totalBolsas = 0;
    $carregar = array('cucid' => null, 'curid' => null, 'curnome' => null, 'curemail' => null);



    if (!$dados[0] && !$fil) {

        
        $sql1 = "select fpbanoreferencia, fpbmesreferencia from sisfor.folhapagamento where fpbid = ".$fpbid;
        $dados1 = $db->carregar($sql1);
        $mes = $dados1[0][fpbmesreferencia];
        $ano = $dados1[0][fpbanoreferencia];
        
        if($mes == 1){
            $mes = 12;
            $ano = $ano -1;
        }
        else{
            $mes = $mes -1;
        }
        $sql2 = "select fpbid from sisfor.folhapagamento where fpbanoreferencia = '$ano' and fpbmesreferencia = '$mes'";
        $dados2 = $db->carregar($sql2);
        $fpbid_query = $dados2[0][fpbid];
        

        $sql = "select c.curid, c.curnome, c.curcpf, c.curemail, cc.cucid,cc.sifid,cc.cucstatus, a.cavid, a.cavparticipacao,a.cavsituacao, a.cavatividadesrealizadas, a.iusdorientador, a.cavtotal, cc.iusdavaliador, cc.estuf, cc.muncod
            from sisfor.cursistacurso cc
                inner join sisfor.cursista c on c.curid = cc.curid
                left  join sisfor.cursistaavaliacoes a on a.cucid = cc.cucid
            where cc.sifid = '$sifid'
            and cc.fpbid = '$fpbid_query' ";
        $dados3 = $db->carregar($sql);
        
        if ($dados3) {

            foreach ($dados3 as $key) {
				$sql = "SELECT cucid FROM sisfor.cursistacurso WHERE curid='".$key['curid']."' AND sifid='".$key['sifid']."' AND fpbid='".$fpbid."'";
				$id_cursista_curso = $db->pegaUm($sql);
				
				if(!$id_cursista_curso) {
	               $sql = "insert into sisfor.cursistacurso  (curid, sifid, fpbid, cucstatus, iusdavaliador, estuf, muncod) VALUES  ('".$key['curid']."','".$key['sifid']."','".$fpbid."','".$key['cucstatus']."','".$key['iusdavaliador']."',".(($key['estuf'])?"'".$key['estuf']."'":"NULL").",".(($key['muncod'])?"'".$key['muncod']."'":"NULL").") RETURNING cucid ;";
    	           $id_cursista_curso = $db->pegaUm($sql);
    	       	}
    	       	
    	       	$sql = "SELECT cavid FROM sisfor.cursistaavaliacoes WHERE cucid='{$id_cursista_curso}'";
    	       	$cavid = $db->pegaUm($sql);
    	       	 
    	       	if(!$cavid) {
    	       	
	                $sql1 = "insert into sisfor.cursistaavaliacoes  (cucid, cavsituacao) VALUES ($id_cursista_curso,'$key[cavsituacao]') "; 
	                $db->executar($sql1);
                
                }
            }

            
            $sql = "select c.curid, c.curnome, c.curcpf, c.curemail, cc.cucid, a.cavid, a.cavparticipacao,a.cavsituacao, a.cavatividadesrealizadas, a.iusdorientador, a.cavtotal, cc.iusdavaliador, cc.estuf, cc.muncod
            from sisfor.cursistacurso cc
                inner join sisfor.cursista c on c.curid = cc.curid
                left  join sisfor.cursistaavaliacoes a on a.cucid = cc.cucid
            where cc.sifid = '$sifid' ".(($iusd)?"and cc.iusdavaliador='".$iusd."'":"")."
            and cc.fpbid = '$fpbid' ";
            
            $total_cursistas = $db->pegaUm("select count(*) from ({$sql}) foo");
            
            $sql .= " order by c.curnome
        	 limit 100 offset 0";

            $dados = $db->carregar($sql);
        }
    }


    if ($dados && is_array($dados)) {
    } else {
        $dados = array();
    }

	$sifid_ = trim($_SESSION['sisfor']['sifid']);
	if(!empty($sifid_)){
		$sqlOrientador = "  select iu.iusd as codigo, iusnome as descricao
                        from sisfor.identificacaousuario iu
                            inner join sisfor.tipoperfil tp on tp.iusd = iu.iusd
                        where sifid = {$sifid_}
                        order by descricao";

		$dadosOrientador = $db->carregar($sqlOrientador);
	}else{
		$dadosOrientador = array();
	}

    ?>

    <?php if('S' == $podeEditar){ ?>
    
    <?php $sql = "select sifcpfobrigatorio from sisfor.sisfor where sifid = '".$_SESSION['sisfor']['sifid']."'";
$cpf_obrigatorio = $db->pegaUm($sql);?>
        <form method="post" id="formulario_cursista" class="formulario_cursista">
            <input type="hidden" name="cucid" id="cucid" value="<?php echo $carregar['cucid']; ?>">
            <input type="hidden" name="envioForm" value="1">
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">
                <tr>
                    <td class="SubTituloDireita" width="10%">CPF</td>
                    <td>
                        <?php if($cpf_obrigatorio == "t"){
                            echo campo_texto('curcpf', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="curcpf"', '', $carregar['curcpf']);}
                        else{
                            echo campo_texto('curcpf', "N", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="curcpf"', '', $carregar['curcpf']);
                        }?>
                    </td>
                        
                </tr>
                <tr>
                    <td class="SubTituloDireita">Nome</td>
                    <td><?php if($cpf_obrigatorio == "t"){  
                        echo campo_texto('curnome', "S", "N", "Nome", 60, 150, "", "", '', '', 0, 'id="curnome"', '', $carregar['curnome']);
                        }
                        else{
                            echo campo_texto('curnome', "S", "S", "Nome", 60, 150, "", "", '', '', 0, 'id="curnome"', 'this.value=removeAcento(this.value.toUpperCase());', $carregar['curnome']);
                        }
                        ?>
                    
                    </td>
                    
                </tr>
                <tr>
                    <td class="SubTituloDireita">E-mail</td>
                    <td><?php echo campo_texto('curemail', "S", "S", "Principal", 60, 60, "", "", '', '', 0, 'id="curemail"', '', $carregar['curemail']); ?></td>
                </tr>

                <tr>
                    <td class="SubTituloDireita">Estado</td>
                    <td>
        <?php
        $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
        $db->monta_combo('estuf', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF2', '', '', '', 'S', 'estuf', '');
        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Município:</td>
                    <td id="td_municipio2">
        <?php
        if ($estuf_nascimento) {
            $sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf = '{$estuf_nascimento}' ORDER BY mundescricao";
            $db->monta_combo('muncod_nascimento', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_nascimento', '');
        } else {
            echo "Selecione uma UF";
        }
        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Avaliador:</td>
                    <? if($iusd) : ?>
					<td>
					<? echo $db->pegaUm("SELECT iusnome FROM sisfor.identificacaousuario WHERE iusd='{$iusd}'")?>
					<input type="hidden" name="iusdavaliador" id="iusdavaliador" value="<?=$iusd ?>">
					</td>                    
                    <? else : ?>
                    <td><? $db->monta_combo('iusdavaliador', $dadosOrientador, $podeEditar, 'Selecione', '', '', '', '', 'N', 'iusdavaliador', ''); ?></td>
                    <? endif; ?>
                    
                </tr>

                <tr>
                    <td align="center" bgcolor="#CCCCCC" colspan="2">
                        <input type="button" class="botao_enviar" id="#botao_enviar" name="salvar" value="Inserir Cursista">
                    </td>
                </tr>
            </table>
        </form>
    <?php } ?>
    
    <script>
	function excluirCursistaAvaliacao(cucid) {
		
		var conf = confirm('Deseja realmente excluir cursista deste período?');
		
		if(conf) {
			ajaxatualizar('requisicao=excluirCursistaAvaliacao&cucid='+cucid,'');
			window.location='sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=cadastrar_cursista&fpbid=<?=$_REQUEST['fpbid'] ?>';
		}
		
	}

	function editarNome(nome, curid) {
		
		var fname=prompt("Digite novo nome:",nome);
		
		if(fname) {
			
			if(fname=='') {
				alert('Nome em branco');
				return false;
			}

			ajaxatualizar('requisicao=atualizarNomeCursista&curid='+curid+'&curnome='+fname,'');
			window.location='sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=cadastrar_cursista&fpbid=<?=$_REQUEST['fpbid'] ?>';

		}

		
	}

	function editarEmail(email, curid) {
		
		var fname=prompt("Digite novo email:",email);
		
		if(fname) {
			
			if(fname=='') {
				alert('Email em branco');
				return false;
			}
			
		    if(!validaEmail(fname)) {
		    	alert('Email inválido');
		    	return false;
		    }

			ajaxatualizar('requisicao=atualizarEmailCursista&curid='+curid+'&curemail='+fname,'');
			window.location='sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=cadastrar_cursista&fpbid=<?=$_REQUEST['fpbid'] ?>';

		}

		
	}

	function editarAvaliador(iusdavaliador, cucid) {

		jQuery("#iusdavaliador_").val(iusdavaliador);
		jQuery("#cucid_").val(cucid);
		 
		jQuery("#modalAvaliador").dialog({
            draggable:true,
            resizable:true,
            width: 600,
            height: 300,
            modal: true,
         	close: function(){} 
        });
	
	}

	function atualizarAvaliador() {

		if(jQuery("#iusdavaliador_").val()=='') {
			alert('Selecione um avaliador');
			return false;
		}
		
		ajaxatualizar('requisicao=atualizarAvaliadorCursista&cucid='+jQuery("#cucid_").val()+'&iusdorientador='+jQuery("#iusdavaliador_").val(),'');
		
		jQuery("#modalAvaliador").dialog('close');
		
		window.location='sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=cadastrar_cursista&fpbid=<?=$_REQUEST['fpbid'] ?>';
		
	}

	function exibirInformacoesCursistas(curid) {

		ajaxatualizar('requisicao=exibirInformacoesCursistas&curid='+curid,'modalFormulario');
		
	    jQuery("#modalFormulario").dialog({
	        draggable: true,
	        resizable: true,
	        width: 800,
	        height: 600,
	        modal: true,
	        close: function() {
	        }
	    });

	}
    </script>
    	<?
	
	include_once APPRAIZ . "includes/library/simec/Grafico.php";
	
	$grafico = new Grafico();
	
	$sql = "select
	case when cavsituacao='e' then 'Evadido'
	     when cavsituacao='f' then 'Faleceu'
	     when cavsituacao='c' then 'Cursando'
	     when cavsituacao='a' then 'Matriculado'
	     when cavsituacao='b' then 'Desvinculado'
	     when cavsituacao='d' then 'Trancado'
		 when cavsituacao='p' then 'Aprovado'
		 when cavsituacao='r' then 'Reprovado' end as descricao,
	m.mesdsc || ' / ' || fpbanoreferencia as categoria,
	count(*) as valor
	from sisfor.cursistacurso cc
	inner join sisfor.cursistaavaliacoes ca on ca.cucid = cc.cucid
	inner join sisfor.folhapagamento f on f.fpbid = cc.fpbid
	inner join public.meses m ON m.mescod::integer = f.fpbmesreferencia
	where sifid={$sifid}
	group by ca.cavsituacao, m.mesdsc, f.fpbanoreferencia, f.fpbid
	order by f.fpbid, descricao";
	
	echo '<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">';
	echo '<tr>';
	echo '<td>';
	$grafico->setTitulo('Situação dos cursistas por mês')->setTipo(Grafico::K_TIPO_COLUNA)->setWidth('100%')->gerarGrafico($sql);
	echo '</td>';
	echo '</tr>';
	echo '</table>';
	
	
	?>
    
    <div id="modalFormulario" style="display:none;"></div>
	<div id="modalAvaliador" style="display: none;">
	<input type="hidden" name="cucid_" id="cucid_" value="">
	
	<p><b>Avaliador : </b><? $db->monta_combo('iusdavaliador_', $dadosOrientador, $podeEditar, 'Selecione', '', '', '', '', 'N', 'iusdavaliador_', ''); ?></p>
	<p align="center"><input type="button" name="atualizar" value="Atualizar" onclick="atualizarAvaliador();"></p>
	</div>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">
        <tr>
            <td>
                <h3 style="text-align: center; margin-top: 0">Avaliar Cursista</h3>
                <form method="post" id="formulario_buscar_cursista" action="sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=cadastrar_cursista&fpbid=<?=$fpbid?>">
                <table class="listagem" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center" witdh="100%">
                <tr>
	                <td colspan="2" class="SubTituloCentro">Buscar</td>
                </tr>
                <tr>
	                <td class="SubTituloDireita">Nome</td>
	                <td><?=campo_texto('curnome_buscar', "N", "S", "Nome do cursista", 30, 60, "", "", '', '', 0, 'id="curnome_buscar"', '', $_REQUEST['curnome_buscar']); ?></td>
                </tr>
                <tr>
	                <td class="SubTituloDireita">Participação</td>
	                <td><? $db->monta_combo('cavparticipacao_buscar', opcoesComboParticipacao(), 'S', 'Selecione', '', '', '', '', 'N', 'cavparticipacao_buscar', '', (($_REQUEST['cavparticipacao_buscar']||$_REQUEST['cavparticipacao_buscar']==='0')?(($_REQUEST['cavparticipacao_buscar'])?$_REQUEST['cavparticipacao_buscar']:'0.0'):''), '','style="font-size:x-small;"'); ?></td>
                </tr>
                <tr>
	                <td class="SubTituloDireita">Atividades realizadas</td>
	                <td><? $db->monta_combo('cavatividadesrealizadas_buscar', opcoesComboAtividades(), 'S', 'Selecione', '', '', '', '', 'N', 'cavatividadesrealizadas_buscar', '', (($_REQUEST['cavatividadesrealizadas_buscar']||$_REQUEST['cavatividadesrealizadas_buscar']==='0')?(($_REQUEST['cavatividadesrealizadas_buscar'])?$_REQUEST['cavatividadesrealizadas_buscar']:'0.0'):''), '','style="font-size:x-small;"'); ?></td>
                </tr>
                <tr>
	                <td class="SubTituloDireita">Situação</td>
	                <td><? $db->monta_combo('cavsituacao_buscar', opcoesComboSituacoes(), 'S', 'Selecione', '', '', '', '', 'N', 'cavsituacao_buscar', '', (($_REQUEST['cavsituacao_buscar']||$_REQUEST['cavsituacao_buscar']==='0')?(($_REQUEST['cavsituacao_buscar'])?$_REQUEST['cavsituacao_buscar']:'0'):''), '','style="font-size:x-small;"'); ?></td>
                </tr>
                
                <tr>
	                <td class="SubTituloDireita">Filtrar sem avaliações</td>
	                <td><input type="checkbox" name="pendencias" value="1" <?=(($_REQUEST['pendencias'])?'checked':'') ?>></td>
                </tr>
                <tr>
	                <td class="SubTituloDireita">Filtrar cursistas cadastro incompleto</td>
	                <td><input type="checkbox" name="cursistasincompleto" value="1" <?=(($_REQUEST['cursistasincompleto'])?'checked':'') ?>></td>
                </tr>
                <tr>
	                <td class="SubTituloDireita">&nbsp;</td>
	                <td><input type="submit" name="buscar_cursista" value="Buscar"> <input type="button" name="vertodos" value="Ver todos" onclick="window.location='sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=cadastrar_cursista&fpbid=<?=$fpbid ?>';"></td>
                </tr>
                </table>
                </form>
                <?php if('S' == $podeEditar){ ?>
                    <form method="post" id="formulario_avaliacao" class="formulario_avaliacao">
                <?php } ?>
                    <input type="hidden" name="acao" value="gravar" />
                    <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem tabela" style="margin-top: 10px;">
                        <thead>
                            <tr align="center">
                                <td>CPF</td>
                                <td>Nome</td>
                                <td>E-mail</td>
                                <td>Participação</td>
                                <td>Atividades Realizadas</td>
                                <td>Situação</td>
                                <td>Avaliador</td>
                            </tr>
                            <tr align="center">
                                <td colspan="3" align="right"><b>Selecionar todos</b></td>
                                <td><? $db->monta_combo('todosfrequencia', opcoesComboParticipacao(), $podeEditar, 'Selecione', '', '', '', '', 'N', 'todosparticipacao', '', '', '','style="font-size:x-small;"'); ?></td>
                                <td><? $db->monta_combo('todosatividadesrealizadas', opcoesComboAtividades(), $podeEditar, 'Selecione', '', '', '', '', 'N', 'todosatividadesrealizadas', '', '', '','style="font-size:x-small;"'); ?></td>
                                <td><? $db->monta_combo('todassituacoes', opcoesComboSituacoes(), $podeEditar, 'Selecione', '', '', '', '', 'N', 'todassituacoes', '', '', '','style="font-size:x-small;"'); ?></td>
                                <td>&nbsp;</td>
                            </tr>
                        </thead>
                        <tbody>
    <?php
    if ($dados && is_array($dados)) {
        foreach ($dados as $count => $dado) {
            ?>
                                    <tr <?php if($dado['cavsituacao'] && $dado['cavsituacao'] != "c" && $dado['cavsituacao'] != "a"&& $dado['cavsituacao'] != "p"){?> style="background-color: #FF7171" <?}?> <?=((!$dado['cavparticipacao'] || !$dado['cavatividadesrealizadas'] || !$dado['cavsituacao'])?'style="background-color: #FFE4B5"':'')?> >
                                        <td>
                                        <span style="font-size:x-small;">
                                            <input type="hidden" name="avaliacao[<?php echo $count; ?>][curid]" value="<?php echo $dado['curid'] ?>" />
                                            <input type="hidden" name="avaliacao[<?php echo $count; ?>][cucid]" value="<?php echo $dado['cucid'] ?>" />
                                            <input type="hidden" name="avaliacao[<?php echo $count; ?>][cavid]" value="<?php echo $dado['cavid'] ?>" />
                                            <span style="white-space: nowrap;">
                                            <? if($podeEditar == 'S' && $db->testa_superuser()) :?>
                                            <img src="../imagens/excluir.gif" style="cursor:pointer;" onclick="excluirCursistaAvaliacao('<?=$dado['cucid'] ?>')"> 
                                            <? endif; ?>
                                            <img src="../imagens/folder_user.png" style="cursor:pointer;" onclick="exibirInformacoesCursistas('<?=$dado['curid'] ?>')">
                                            </span>
            <?php echo (($dado['curcpf'])?mascaraglobal($dado['curcpf'],"###.###.###-##"):"") ?>
            							</span>
                                        </td>
                                        <td><span style="font-size:x-small;"><?php echo $dado['curnome'] ?></span> <?=((($podeEditar=='S' && !$dado['curcpf'])||($podeEditar == 'S' && $db->testa_superuser()))?'<img src="../imagens/seta_baixo.png" style="cursor:pointer;" onclick="editarNome(\''.$dado['curnome'].'\','.$dado['curid'].');">':'') ?></td>
                                        <td><span style="font-size:x-small;"><?php echo $dado['curemail'] ?></span> <?=(($podeEditar=='S')?'<img src="../imagens/seta_baixo.png" style="cursor:pointer;" onclick="editarEmail(\''.$dado['curemail'].'\','.$dado['curid'].');">':'') ?></td>
                                        <td align="center">
            <?php
            $cavparticipacao = ('0' === $dado['cavparticipacao']) ? '0.0' : number_format($dado['cavparticipacao'], '1', ',', '.');
            $db->monta_combo("avaliacao[{$count}][cavparticipacao]", opcoesComboParticipacao(), $podeEditar, 'Selecione', '', '', '', '200', 'N', '', '', $cavparticipacao, '', 'item_count="' . $count . '" style="font-size:x-small;"', 'cb_participacao soma soma_' . $count);
            ?>
                                        </td>
                                        <td align="center">
            <?php
            $cavatividadesrealizadas = ('0' === $dado['cavatividadesrealizadas']) ? '0.0' : number_format($dado['cavatividadesrealizadas'], '1', ',', '.');
            $db->monta_combo("avaliacao[{$count}][cavatividadesrealizadas]", opcoesComboAtividades(), $podeEditar, 'Selecione', '', '', '', '200', 'N', '', '', $cavatividadesrealizadas, '', 'item_count="' . $count . '" style="font-size:x-small;"', 'cb_atividades_realizadas soma soma_' . $count);
            ?>
                                        </td>
                                        
                                        <td align="center"> 
                                            
                                         <?php   $db->monta_combo("avaliacao[{$count}][cavsituacao]", opcoesComboSituacoes(), $podeEditar, 'Selecione', '', '', '', '200', 'N', '', '', $dado['cavsituacao'], '', 'item_count="' . $count . '" style="font-size:x-small;"', 'cb_situacao soma soma_' . $count);?>
                                        
                                        
                                        <td>
                                        <span style="font-size:x-small;">
            <?php
            $aOrientador = array();
            foreach ($dadosOrientador as $orientador) {
                $aOrientador[$orientador['codigo']] = $orientador['descricao'];
            }
            echo $aOrientador[$dado['iusdavaliador']];
            ?>
            
            <?=(($podeEditar=='S')?'<img src="../imagens/seta_baixo.png" style="cursor:pointer;" onclick="editarAvaliador('.$dado['iusdavaliador'].','.$dado['cucid'].');">':'') ?>
            							</span>
                                        </td>
                                    </tr>
        <?php } ?>
                                <tr>
                                    <td bgcolor="white" colspan="9">
									<b>Total de cursistas cadastrados : <?=$total_cursistas ?></b> 
									<span style="float:right"><b>Páginas</b>
									<?
									
									$pgs = ceil($total_cursistas/100);
									if($_REQUEST['cursistasincompleto']) $f_req .= '&cursistasincompleto=1';
									if($_REQUEST['pendencias'])          $f_req .= '&pendencias=1';
									for($i=1;$i<=$pgs;$i++) {
										if($i==($_REQUEST['pgina']+1)) $pg[] = "<span style=\"cursor:pointer;\" onclick=\"window.location='sisfor.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=cadastrar_cursista".$f_req."&fpbid=".$fpbid."&pgina=".($i-1)."';\"><b>".$i."</b></span>";
										else $pg[] = "<span style=cursor:pointer; onclick=\"window.location='sisfor.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=cadastrar_cursista".$f_req."&fpbid=".$fpbid."&pgina=".($i-1)."';\">".$i."</span>";
									}
									
									if($pg) echo implode(" | ",$pg);
																		
									?>
									</span>
                                    </td>
                                </tr>
                                <tr>
                                    <td align="center" bgcolor="#CCCCCC" colspan="9">
                                        <?php if('S' == $podeEditar){ ?>
                                            <input type="button" class="botao_enviar_avaliacao" id="botao_enviar_avaliacao"  name="salvar" value="Salvar Avaliações">
                                        <?php } ?>
                                    </td>
                                </tr>
    <?php } else { ?>
                                <tr bgcolor="" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#ffffcc';">
                                    <td colspan="4" align="center"><span style="color: red;">Nenhum cursista vinculado</span></td>
                                </tr>
    <?php } ?>
                        </tbody>
                    </table>
                <?php if('S' == $podeEditar){ ?>
                    </form>
                <?php } ?>
            </td>
            <td valign="top">
    <?php
    $docid = pegarDocidAvaliacaoCursista($_SESSION['sisfor']['sifid'], $_REQUEST['fpbid']);
    wf_desenhaBarraNavegacao($docid, array('sifid' => $_SESSION['sisfor']['sifid'], "fpbid" => $_REQUEST['fpbid'], 'tipo'=>'cursista'));
    ?>
            </td>
        </tr>
    </table>

    <script type="text/javascript" src="/estrutura/js/funcoes.js"></script>
    <script type="text/javascript">
                                jQuery(function() {
                                    jQuery('#todosparticipacao').change(function() {
                                        jQuery('.cb_participacao').val(jQuery(this).val()).change();
                                    });

                                    jQuery('#todosatividadesrealizadas').change(function() {
                                        jQuery('.cb_atividades_realizadas').val(jQuery(this).val()).change();
                                    });

                                    jQuery('#todassituacoes').change(function() {
                                        jQuery('.cb_situacao').val(jQuery(this).val()).change();
                                    });

                                    jQuery('#todosorientador').change(function() {
                                        jQuery('.cb_orientador').val(jQuery(this).val()).change();
                                    });

                                    jQuery('.soma').change(function() {
                                        var item = jQuery(this).attr('item_count');
                                        console.log(item);
                                        atualizaTotal('soma_' + item, 'cavtotal_' + item, 'campo');
                                    });

                                    jQuery('#botao_enviar_avaliacao').click(function() {
                                        jQuery('#formulario_avaliacao').submit();
                                    });
                                });
    </script>
    <?php
}

function atualizarNomeCursista($dados) {
	global $db;
	
	$sql = "UPDATE sisfor.cursista SET curnome='".$dados['curnome']."' WHERE curid='".$dados['curid']."'";
	
	$db->executar($sql);
	$db->commit();
	
}

function atualizarEmailCursista($dados) {
	global $db;

	$sql = "UPDATE sisfor.cursista SET curemail='".$dados['curemail']."' WHERE curid='".$dados['curid']."'";

	$db->executar($sql);
	$db->commit();

}

function atualizarAvaliadorCursista($dados) {
	global $db;
	
	$sql = "UPDATE sisfor.cursistacurso SET iusdavaliador='".$dados['iusdorientador']."' WHERE cucid='".$dados['cucid']."'";
	echo $sql;
	
	$db->executar($sql);
	$db->commit();

}

function calcularCustoAlunoCusteio($dados) {
    global $db;

    $totalcusteio = $db->pegaUm("SELECT sum(orcvlrunitario) as soma FROM sisfor.orcamento WHERE sifid='" . $dados['sifid'] . "' AND orcstatus='A'");
    $sifprofmagisterio = $db->pegaUm("SELECT sifprofmagisterio FROM sisfor.sisfor WHERE sifid='" . $dados['sifid'] . "'");

    if ($sifprofmagisterio) {

		if($dados['retorna']) return number_format(($totalcusteio / $sifprofmagisterio), 2, ",", ".");
		else echo "R$ " . number_format(($totalcusteio / $sifprofmagisterio), 2, ",", ".");
		
    } else {

    	if($dados['retorna']) return "0,00";
    	else echo "R$ 0,00";
    	
    }

}

function calcularCustoAlunoBolsa($dados) {
	global $db;

	$totalbolsa = $db->pegaUm("SELECT sum(epivalor) as soma FROM sisfor.equipeies WHERE sifid='" . $dados['sifid'] . "'");
	$sifprofmagisterio = $db->pegaUm("SELECT sifprofmagisterio FROM sisfor.sisfor WHERE sifid='" . $dados['sifid'] . "'");

    if ($sifprofmagisterio) {

		if($dados['retorna']) return number_format(($totalbolsa / $sifprofmagisterio), 2, ",", ".");
		else echo "R$ " . number_format(($totalbolsa / $sifprofmagisterio), 2, ",", ".");
		
    } else {

    	if($dados['retorna']) return "0,00";
    	else echo "R$ 0,00";
    	
    }
}

function notificarCoordenadorLocal($sifid) {
    global $db;

    $sql = "SELECT i.iusnome as nome, i.iusemailprincipal as email, COALESCE(curdesc,'-') as curso, curcontemail as emailcontato, curcontdesc as nomecontato 
			FROM sisfor.sisfor s 
			INNER JOIN sisfor.identificacaousuario i ON i.iuscpf = s.usucpf 
			LEFT JOIN catalogocurso2014.iesofertante io ON io.ieoid = s.ieoid 
			LEFT JOIN catalogocurso2014.curso cur ON cur.curid = io.curid 
			LEFT JOIN workflow.documento doc ON doc.docid = s.docid 
			WHERE s.sifid='{$sifid}'";


    echo $sql;
    exit;

    $foo = $db->pegaLinha($sql);

    if ($foo) {

        require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
        require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

        $mensagem = new PHPMailer();
        $mensagem->persistencia = $db;

        $mensagem->Host = "localhost";
        $mensagem->Mailer = "smtp";
        $mensagem->FromName = "SIMEC";
        $mensagem->From = "noreply@mec.gov.br";
        $mensagem->Subject = "SIMEC - SISFOR - Retornando para elaboração";

        $mensagem->AddAddress($foo['email'], $foo['nome']);


        $mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>O projeto do curso {$foo['curso']} no SIMEC - SISFOR foi devolvido para ajustes. Seguem os comentários feitos pelo analista do MEC : </p>
		<p>{$_REQUEST['cmddsc']}</p>
		<br/><br/>
		<p>Equipe MEC</p>
		<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha</p>
					";

        $mensagem->IsHTML(true);
        $resp = $mensagem->Send();

        $mensagem = new PHPMailer();
        $mensagem->persistencia = $db;

        $mensagem->Host = "localhost";
        $mensagem->Mailer = "smtp";
        $mensagem->FromName = "SIMEC";
        $mensagem->From = "noreply@mec.gov.br";
        $mensagem->Subject = "SIMEC - SISFOR - Tramitação no curso";

        $mensagem->AddAddress($foo['emailcontato'], $foo['nomecontato']);


        $mensagem->Body = "<p>Prezado(a) {$foo['nomecontato']},</p>
		<p>O projeto do curso {$foo['curso']} no SIMEC - SISFOR foi devolvido para ajustes. Seguem os comentários feitos pelo analista do MEC : </p>
		<p>{$_REQUEST['cmddsc']}</p>
		<br/><br/>
		<p>Equipe MEC</p>
		";

        $mensagem->IsHTML(true);
        $resp = $mensagem->Send();
    }

    return true;
}

function opcoesComboAtividadesEquipe() {
    return array(
        array('codigo' => 'A', 'descricao' => 'Realizou as atividades'),
        array('codigo' => 'I', 'descricao' => 'Não realizou as atividades'),
        array('codigo' => 'N', 'descricao' => 'Não se aplica'),
    );
}

function opcoesComboAtividades() {
    return array(
        array('codigo' => '5,0', 'descricao' => 'Realizou as atividades integralmente'),
        array('codigo' => '3,5', 'descricao' => 'Realizou as atividades suficientemente'),
        array('codigo' => '2,0', 'descricao' => 'Realizou as atividades insuficientemente'),
        array('codigo' => '0', 'descricao' => 'Não realizou as atividades'),
    );
}

function opcoesComboSituacoes() {
    return array(
        array('codigo' => 'c', 'descricao' => 'Cursando'),
        array('codigo' => 'e', 'descricao' => 'Evadido'),
        array('codigo' => 'f', 'descricao' => 'Falecido'),
		array('codigo' => 'a', 'descricao' => 'Matriculado'),
	    array('codigo' => 'b', 'descricao' => 'Desvinculado'),
		array('codigo' => 'd', 'descricao' => 'Trancado'),
		array('codigo' => 'p', 'descricao' => 'Aprovado'),
		array('codigo' => 'r', 'descricao' => 'Reprovado'),
        
    );
}

function opcoesComboParticipacao() {
    return array(
        array('codigo' => '5,0', 'descricao' => 'Participou integralmente'),
        array('codigo' => '3,5', 'descricao' => 'Participou suficientemente'),
        array('codigo' => '2,0', 'descricao' => 'Participou insuficientemente'),
        array('codigo' => '0', 'descricao' => 'Não Participou'),
    );
}

function carregarPeriodoReferencia($dados) {
    global $db;

    $sql = "SELECT f.fpbid as codigo, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao
			FROM sisfor.folhapagamento f
			INNER JOIN sisfor.folhapagamentoprojeto rf ON rf.fpbid = f.fpbid
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE rf.sifid='" . $dados['sifid'] . "' AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd') 
	 		ORDER BY (fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date";

    $sql_tot = "SELECT count(*) as tot FROM ($sql) as foo";
    $tot = $db->pegaUm($sql_tot);

    if (!$tot) {
        echo "<br><div style=\"width: 90%;padding: 10px;border: 5px solid gray;margin: 0px;\">Não existem períodos de referências cadastrados.</div><br>";
    } else {
        if (!$dados['somentecombo'])
            echo "Selecione período de referência : ";
        $db->monta_combo('fpbid', $sql, 'S', 'Selecione', 'selecionarPeriodoReferencia', '', '', '', 'S', 'fpbid', '', $dados['fpbid']);
    }
}

function carregarPeriodoReferenciaCursista($dados) {
    global $db;

    $sql = "SELECT f.fpbid as codigo, m.mesdsc || '/' || fpbanoreferencia as descricao
			FROM sisfor.folhapagamento f
			INNER JOIN sisfor.folhapagamentocursista rf ON rf.fpbid = f.fpbid
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE rf.sifid='" . $dados['sifid'] . "'
			AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
 			ORDER BY (fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date
			";

    $sql_tot = "SELECT count(*) as tot FROM ($sql) as foo";

    $tot = $db->pegaUm($sql_tot);
    if (!$tot) {
        echo "<br><div style=\"width: 90%;padding: 10px;border: 5px solid gray;margin: 0px;\">Não existem períodos de referências cadastrados.</div><br>";
    } else {
        if (!$dados['somentecombo'])
            echo "Selecione período de referência : ";
        $db->monta_combo('fpbid', $sql, 'S', 'Selecione', 'selecionarPeriodoReferencia', '', '', '', 'S', 'fpbid', '', $dados['fpbid']);
    }
}

function recuperarMensarios($sifid, $fpbid) {
    global $db;

    $sql = "select replace(to_char(iu.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf,
    			   iu.iusd,
                   iu.iusnome,
                   iu.iusemailprincipal,
                   p.pfldsc,
                   m.menid,
                   ma.mavid,
                   ma.iusdavaliador,
                   ma.iusdorientador,
                   ma.motivoavaliacao,
                   ma.mavparticipacao,
                   ma.mavatividadesrealizadas,
                   ma.mavtotal,
                   ia.iusnome as iusnomeavaliador,
                   m.tpeid,
                   m.fpbid
            from sisfor.mensario m
                inner join sisfor.tipoperfil t on t.tpeid = m.tpeid
                inner join seguranca.perfil p on p.pflcod = t.pflcod
                inner join sisfor.identificacaousuario iu on iu.iusd = t.iusd
                left  join  sisfor.mensarioavaliacoes ma on ma.menid = m.menid 
                left join sisfor.identificacaousuario ia on ia.iusd = ma.iusdorientador 
            where t.sifid = '$sifid'
            and m.fpbid = '$fpbid' 
    		order by p.pflnivel, iu.iusnome";

    return $db->carregar($sql);
}

function recuperarCursista($sifid, $fpbid) {
    global $db;

    $sql = "select replace(to_char(c.curcpf::numeric, '000:000:000-00'), ':', '.') as curcpf,
                   c.curnome,
                   c.curemail,
                   c.curid,
                   cc.cucid,
                   cc.sifid,
                   cc.fpbid
            from sisfor.cursistacurso cc
                inner join sisfor.cursista c on c.curid = cc.curid
            where cc.sifid = '$sifid'
            and cc.fpbid = '$fpbid' ";

    return $db->carregar($sql);
}

function cadastrarPeriodoReferencia($dados) {
    global $db;

    $al = null;
    $cursosSelecionados = is_array($dados['curso']) ? $dados['curso'] : array();

    if(count($cursosSelecionados)){
        if (is_array($dados['smesini'])) {

            $sifids = array_keys($dados['smesini']);

            if ($cursosSelecionados) {
                foreach ($cursosSelecionados as $sifid) {



                    $sql = "DELETE FROM sisfor.folhapagamentoprojeto WHERE sifid='" . $sifid . "' and docid is null";
                    $db->executar($sql);

                    $sql = "select foo.fpbid from (
                            select fpbid, fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0') as dt from sisfor.folhapagamento order by (fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0')||'-01')::date ) foo
                            where foo.dt >= '" . $dados['sanoinicio'][$sifid] . "-" . str_pad($dados['smesini'][$sifid], 2, "0", STR_PAD_LEFT) . "'
                            AND foo.dt <= '" . $dados['sanofim'][$sifid] . "-" . str_pad($dados['smesfim'][$sifid], 2, "0", STR_PAD_LEFT) . "'";

                    $fpbids = $db->carregarColuna($sql);

                    if ($fpbids) {
                        foreach ($fpbids as $key => $fpbid) {
							
							$rfuid = $db->pegaUm("SELECT rfuid FROM sisfor.folhapagamentoprojeto WHERE sifid={$sifid} and fpbid={$fpbid}");
							
							if(!$rfuid) {

	                            $sql = "INSERT INTO sisfor.folhapagamentoprojeto(
	                                    sifid, fpbid, rfuparcela)
	                                    VALUES ('" . $sifid . "', '" . $fpbid . "', '" . ($key + 1) . "');";
	
	                            $db->executar($sql);
                            
                            } else {
								$sql = "UPDATE sisfor.folhapagamentoprojeto SET rfuparcela=".($key + 1)." WHERE rfuid='{$rfuid}'";
								$db->executar($sql);
							}
                        }
                    }

                }
                $db->commit();
            }
            $al = array("alert" => "Período de referência aprovado com sucesso", "location" => "sisfor.php?modulo=principal/mec/mec&acao=A&aba=configuracoes");
        }
        if (is_array($dados['smesinicur'])) {

            $sifids = array_keys($dados['smesinicur']);

            if ($cursosSelecionados) {
                foreach ($cursosSelecionados as $sifid) {

                    $sql = "DELETE FROM sisfor.folhapagamentocursista WHERE rfcid IN(
			        		SELECT DISTINCT rfcid FROM sisfor.folhapagamentocursista f 
							LEFT JOIN sisfor.cursistacurso c ON c.sifid = f.sifid AND c.fpbid = f.fpbid
							WHERE f.sifid='" . $sifid . "' AND c.cucid is null
            				)";
                    
                    $db->executar($sql);

                    $sql = "select foo.fpbid from (
                            select fpbid, fpbanoreferencia||'-'||lpad(fpbmesreferencia::text,2,'0') as dt from sisfor.folhapagamento ) foo
                            where foo.dt >= '" . $dados['sanoiniciocur'][$sifid] . "-" . str_pad($dados['smesinicur'][$sifid], 2, "0", STR_PAD_LEFT) . "'
                            AND foo.dt <= '" . $dados['sanofimcur'][$sifid] . "-" . str_pad($dados['smesfimcur'][$sifid], 2, "0", STR_PAD_LEFT) . "'";

                    $fpbids = $db->carregarColuna($sql);

                    if ($fpbids) {
                        foreach ($fpbids as $key => $fpbid) {

							$rfcid = $db->pegaUm("SELECT rfcid FROM sisfor.folhapagamentocursista WHERE sifid='".$sifid."' AND fpbid='".$fpbid."'");
							
							if(!$rfcid) {

	                            $sql = "INSERT INTO sisfor.folhapagamentocursista(
	                                    sifid, fpbid)
	                                    VALUES ('" . $sifid . "', '" . $fpbid . "');";
	
	                            $db->executar($sql);
	                            
                            }
                        }
                    }
                }
            }

            $db->commit();
            
            //limpando lixo deixado pra trás
            $sql = "delete from sisfor.folhapagamentoprojeto 
					using (
					
					select min(fp.rfuid) as r, fp.sifid, fp.rfuparcela from sisfor.folhapagamentoprojeto fp 
					inner join (
					select sifid, rfuparcela, count(*) from sisfor.folhapagamentoprojeto 
					group by sifid, rfuparcela 
					having count(*) > 1
					) foo on foo.sifid = fp.sifid and foo.rfuparcela = fp.rfuparcela 
					left join workflow.documento d on d.docid = fp.docid 
					left join workflow.estadodocumento e on e.esdid = d.esdid 
					where (e.esdid is null or e.esdid=".ESD_AVALIACAO.") 
					group by fp.sifid, fp.rfuparcela
					order by fp.sifid, fp.rfuparcela
					
					) foo2 where foo2.r = rfuid";
            
            $db->executar($sql);
            $db->commit();

            $al = array("alert" => "Período de referência aprovado com sucesso", "location" => "sisfor.php?modulo=principal/mec/mec&acao=A&aba=configuracoes");
        }
    } else {
        $al = array("alert" => "Favor selecionar ao menos um curso para gravar.", "location" => "sisfor.php?modulo=principal/mec/mec&acao=A&aba=configuracoes");
    }

    if (!$al) {
        $al = array("alert" => "Erro ao processar operação", "location" => "sisfor.php?modulo=principal/mec/mec&acao=A&aba=configuracoes");
    }
    alertlocation($al);
}

function pegarDadosCurso($dados) {
    global $db;

    $dadosprojeto = $db->pegaLinha("SELECT sifcargahorariapresencial,
									   sifcargahorariadistancia,
									   sifdddtelmatricula,
									   siftelmatricula,
									   sifemailmatricula,
									   sifmetodologia,
									   uniabrev||' - '||unidsc as universidade,
										case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc 
										     when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc 
										     when s.ocuid is not null then oc.ocunome 
										     when s.oatid is not null then oatnome end as curnome,

										case when s.ieoid is not null then cor.coordid 
										     when s.cnvid is not null then cor2.coordid 
										     when s.ocuid is not null then cor3.coordid 
										     when s.oatid is not null then cor4.coordid end as coordid
								FROM sisfor.sisfor s 
								INNER JOIN public.unidade u on u.unicod = s.unicod
								left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
								left join catalogocurso2014.curso cur on cur.curid = ieo.curid
								left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
								left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
								left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
								left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
								left join seguranca.usuario usu on usu.usucpf = s.usucpf
								left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
								left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
								left join sisfor.outraatividade oat on oat.oatid = s.oatid 
								left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid 

								WHERE s.sifid='" . $dados['sifid'] . "'");

    return $dadosprojeto;
}

function recusarProjetoCM($sifid) {
    global $db;

    $curso = pegarDadosCurso(array('sifid' => $sifid));

    $sql = "SELECT DISTINCT u.usunome, u.usuemail FROM seguranca.usuario u
			INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf
			INNER JOIN sisfor.usuarioresponsabilidade r ON r.usucpf = u.usucpf AND r.pflcod = pu.pflcod
			WHERE pu.pflcod='" . PFL_COORDENADOR_MEC . "' AND r.coordid='" . $curso['coordid'] . "' AND r.rpustatus='A'";

    $resp = $db->carregar($sql);

    $usunome = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf='" . $_SESSION['usucpf'] . "'");


    require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
    require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

    if ($resp[0]) {
        foreach ($resp as $r) {

            $mensagem = new PHPMailer();
            $mensagem->persistencia = $db;
            $mensagem->Host = "localhost";
            $mensagem->Mailer = "smtp";
            $mensagem->FromName = "SISFOR - Sistema de Gestão e Monitoramento da Formação Continuada do MEC";
            $mensagem->From = "simec@mec.gov.br";
            $mensagem->AddAddress($r['usuemail'], $r['usunome']);
            $mensagem->Subject = "Tramitação de projeto no SISFOR";
            $mensagem->Body = "<p>Prezado(a) " . $r['usunome'] . ", o projeto do curso " . $curso['curnome'] . ", da " . $curso['universidade'] . ", foi recusado por " . $usunome . ". Para visualizar o parecer e tramitar o projeto, siga os seguintes passos:</p>
							   <p>
 							   1)  Acesse o SisFor e localize o projeto do curso em Principal >> Análise de cursos – Projeto >> Recusado – Em análise do Coordenador MEC;<br>
 							   2) Clique na lupa ( <img src=http://simec.mec.gov.br/imagens/lupa.gif> ) ao lado do nome do projeto;<br>
							   3) No canto direito, clique no botão 'histórico';<br>
							   4) Ao abrir a janela de tramitações, identifique na coluna 'O que aconteceu' a situação 'Recusado – Em análise do Coordenador MEC';<br>
							   5) No final da linha, clique no triângulo laranja e leia o parecer;<br>
							   6) Caso concorde com a análise, clique no botão de ação 'Recusar - Enviar para Diretor MEC'. Caso discorde, clique no botão de ação 'Devolver para Equipe MEC';<br>
							   7) Escreva os comentários e clique em 'Tramitar'.
 							   </p>";
            $mensagem->IsHTML(true);
            $mensagem->Send();
        }
    }

    return true;
}

function aprovarProjetoCM($sifid) {
    global $db;

    $curso = pegarDadosCurso(array('sifid' => $sifid));

    $sql = "SELECT DISTINCT u.usunome, u.usuemail FROM seguranca.usuario u 
			INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf 
			INNER JOIN sisfor.usuarioresponsabilidade r ON r.usucpf = u.usucpf AND r.pflcod = pu.pflcod  
			WHERE pu.pflcod='" . PFL_COORDENADOR_MEC . "' AND r.coordid='" . $curso['coordid'] . "' AND r.rpustatus='A'";

    $resp = $db->carregar($sql);

    $usunome = $db->pegaUm("SELECT usunome FROM seguranca.usuario WHERE usucpf='" . $_SESSION['usucpf'] . "'");

    require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
    require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

    if ($resp[0]) {
        foreach ($resp as $r) {

            $mensagem = new PHPMailer();
            $mensagem->persistencia = $db;
            $mensagem->Host = "localhost";
            $mensagem->Mailer = "smtp";
            $mensagem->FromName = "SISFOR - Sistema de Gestão e Monitoramento da Formação Continuada do MEC";
            $mensagem->From = "simec@mec.gov.br";
            $mensagem->AddAddress($r['usuemail'], $r['usunome']);
            $mensagem->Subject = "Tramitação de projeto no SISFOR";
            $mensagem->Body = "<p>Prezado(a) " . $r['usunome'] . ", o projeto do curso " . $curso['curnome'] . ", da " . $curso['universidade'] . ", foi aprovado por " . $usunome . ". Para visualizar o parecer e tramitar o projeto, siga os seguintes passos:</p>
							   <p>
 							   1)  Acesse o SisFor e localize o projeto do curso em Principal >> Análise de cursos – Projeto >> Aprovado – Em análise do Coordenador MEC;<br>
 							   2) Clique na lupa ( <img src=http://simec.mec.gov.br/imagens/lupa.gif> ) ao lado do nome do projeto;<br>
							   3) No canto direito, clique no botão 'histórico';<br>
							   4) Ao abrir a janela de tramitações, identifique na coluna 'O que aconteceu' a situação 'Aprovado – Em análise do Coordenador MEC';<br>
							   5) No final da linha, clique no triângulo laranja e leia o parecer;<br>
							   6) Caso concorde com a análise, clique no botão de ação 'Aprovado - Enviar para Diretor MEC'. Caso discorde, clique no botão de ação 'Devolver para Equipe MEC';<br>
							   7) Escreva os comentários e clique em 'Tramitar'.
 							   </p>";
            $mensagem->IsHTML(true);
            $mensagem->Send();
        }
    }

    return true;
}

function salvarOferta2015_IES($dados) {
    global $db;

    if ($dados['oferta']) {
        foreach ($dados['oferta'] as $estuf => $arr) {
            foreach ($arr as $curid => $vlr) {
                $ofeid_e = $db->pegaUm("SELECT ofeid FROM sisfor.oferta2015 WHERE unicod='" . $dados['unicod'] . "' AND curid='" . $curid . "' AND estuf='" . $estuf . "'");

                if ($ofeid_e)
                    $sql = "UPDATE sisfor.oferta2015 SET ofeqtdies='" . (($vlr) ? $vlr : "0") . "' WHERE ofeid='" . $ofeid_e . "'";
                else
                    $sql = "INSERT INTO sisfor.oferta2015(unicod, curid, estuf, ofeqtdies) VALUES ('" . $dados['unicod'] . "', '" . $curid . "', '" . $estuf . "', '" . (($vlr) ? $vlr : "0") . "');";

                $db->executar($sql);
                $db->commit();
            }
        }
    }

    $al = array("alert" => "Oferta 2015 gravada com sucesso", "location" => "sisfor.php?modulo=principal/coordenador/oferta2015&acao=A");
    alertlocation($al);
}

function salvarOferta2015_UF($dados) {
	global $db;

	if($dados['uf']) {
		foreach($dados['uf'] as $unicod => $vlr) {
			$ofeid_e = $db->pegaUm("SELECT ofeid FROM sisfor.oferta2015 WHERE unicod='".$unicod."' AND curid='".$dados['curid']."' AND estuf='".$dados['estuf']."'");
				
			if($ofeid_e) $sql = "UPDATE sisfor.oferta2015 SET ofeqtdforum='".(($vlr)?$vlr:"0")."' WHERE ofeid='".$ofeid_e."'";
			else $sql = "INSERT INTO sisfor.oferta2015(unicod, curid, estuf, ofeqtdforum) VALUES ('".$unicod."', '".$dados['curid']."', '".$dados['estuf']."', '".(($vlr)?$vlr:"0")."');";
				
			$db->executar($sql);
			$db->commit();
				
		}
	}

	$al = array("alert"=>"Oferta 2015 gravada com sucesso","location"=>"sisfor.php?modulo=principal/forumestadual/oferta2015&acao=A");
	alertlocation($al);

}

function finalizarOferta2015($dados) {
    global $db;

    $db->executar("UPDATE sisfor.sisfories SET sieoferta2015ies=true WHERE unicod='" . $_SESSION['sisfor']['unicod'] . "'");
    $db->commit();

    $al = array("alert" => "Oferta 2015 finalizada com sucesso", "location" => "sisfor.php?modulo=principal/coordenador/oferta2015&acao=A");
    alertlocation($al);
}

function reabrirOferta2015($dados) {
    global $db;

    $db->executar("UPDATE sisfor.sisfories SET sieoferta2015ies=false WHERE unicod='" . $_SESSION['sisfor']['unicod'] . "'");
    $db->commit();

    $al = array("alert" => "Oferta 2015 reaberta com sucesso", "location" => "sisfor.php?modulo=principal/coordenador/oferta2015&acao=A");
    alertlocation($al);
}

function verificarFluxoValidado($docid) {
    $estadoAtualProjeto = wf_pegarDocumento($docid);
    if (ESD_PROJETO_VALIDADO != $estadoAtualProjeto['esdid']) {
        $location = "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso&acao=A&aba=visualizacao_projeto";
        $al = array("alert" => "Não é possível compor a equipe enquanto o projeto não estiver validado!", "location" => $location);
        alertlocation($al);
    }
}

function verificarFluxoComposicaoEquipe($docid) {
    $estadoAtualProjeto = wf_pegarDocumento($docid);

    if (ESD_CADASTRAMENTO_FINALIZADO != $estadoAtualProjeto['esdid']) {
        $location = "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=compor_equipe";
        $al = array("alert" => "A composição de equipe ainda não foi finalizada. Favor verificar", "location" => $location);
        alertlocation($al);
    }
}

function verificarFluxoPagamento($docid) {
    $estadoAtualProjeto = wf_pegarDocumento($docid);

    if (ESD_CADASTRAMENTO_FINALIZADO != $estadoAtualProjeto['esdid']) {
        $location = "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=compor_equipe";
        $al = array("alert" => "A composição de equipe ainda não foi finalizada. Favor verificar", "location" => $location);
        alertlocation($al);
    }
}

function pegarDocidFolhaPagamentoProjeto($sifid, $fpbid) {
    global $db;
    $sql = "select docid from sisfor.folhapagamentoprojeto where sifid = '$sifid' and fpbid = $fpbid";
    $docid = $db->pegaUm($sql);
    if (!$docid) {
        $docid = wf_cadastrarDocumento(WF_TPDID_SISFOR_AVALIACAO, "Folha de Pagamento do projeto {$sifid} e folha {$fpbid}");

        $db->executar("UPDATE sisfor.folhapagamentoprojeto SET docid = '$docid' where sifid = '$sifid' and fpbid = $fpbid");
        $db->commit();
    }

    return $docid;
}

function pegarDocidAvaliacaoCursista($sifid, $fpbid) {
    global $db;
    $sql = "select docid from sisfor.folhapagamentocursista where sifid = '$sifid' and fpbid = $fpbid";

    $docid = $db->pegaUm($sql);
    if (!$docid) {
        $docid = wf_cadastrarDocumento(WF_TPDID_SISFOR_CADASTRAMENTO, "Folha de Pagamento do cursista {$sifid} e folha {$fpbid}");

        $db->executar("UPDATE sisfor.folhapagamentocursista SET docid = '$docid' where sifid = '$sifid' and fpbid = $fpbid");
        $db->commit();
    }

    return $docid;
}

function validarEnvioAnaliseMEC($sifid, $fpbid)
{
    global $db;
    
    $fpbstatus = $db->pegaUm("SELECT fpbstatus FROM sisfor.folhapagamento WHERE fpbid='$sifid'");
    
    if($fpbstatus=='I') {
		return 'O período de referência não esta mais APTO para enviar para pagamento';
	}
    

    // @RN Verificar se toda a equipe está avaliada
    $sql = "select sum(pendencias) as pendencias
            from (
                select case
                        when coalesce(mavatividadesrealizadas, '0') = '0' then 1
                        else 0
                    end as pendencias
                from sisfor.mensario m
                    inner join sisfor.tipoperfil t on t.tpeid = m.tpeid
                    inner join sisfor.identificacaousuario iu on iu.iusd = t.iusd
                    left  join  sisfor.mensarioavaliacoes ma on ma.menid = m.menid
                where fpbid = '$fpbid'
                and sifid = '$sifid'
            ) as foo";

    $pendencias = $db->pegaUm($sql);

    if($pendencias){
        return "Não é possível enviar para análise enquanto todos os membros da equipe nao forem avaliados.";
    }


    // @RN Verificar se o cadastro de cursista para o MESMO PERÍODO DE REFERÊNCIA está finalizado
    $sql = "select count(*) as existePeriodoCursista
            from sisfor.folhapagamentocursista
            where fpbid = '$fpbid'
            and sifid = '$sifid'";

    $existePeriodoCursista = $db->pegaUm($sql);

    if($existePeriodoCursista){
        $estadoAtual['esdid'] = null;
        if($_REQUEST['fpbid']){
            $docid = pegarDocidAvaliacaoCursista($sifid, $fpbid);
            $estadoAtual = wf_pegarEstadoAtual($docid);
        }

        if(ESD_CADASTRAMENTO_FINALIZADO != $estadoAtual['esdid']){
            return "Não é possível enviar para análise enquanto o cadastro de cursistas para o MESMO PERÍODO DE REFERÊNCIA não estiver finalizado.";
        }
    }


    return true;
}

function validarCadastro($sifid, $fpbid = null, $tipo = null)
{
    global $db;

    if('cursista' == $tipo){

        $sql = "select count(*)
                from sisfor.cursista c
                    inner join  sisfor.cursistacurso cc on cc.curid = c.curid
                where fpbid = '$fpbid'
                and sifid = '$sifid' ";

        $qtd = $db->pegaUm($sql);

        if(!$qtd){
            return "É necessário gravar ao menos um cursista para finalizar o cadastro.";
        }

        // @RN Verificar se toda a equipe está avaliada
        $sql = "select  sum(pendencias_participacao) + sum(pendencias_atividades) + sum(pendencias_situacao) as pendencias
                from (
                    select case
                        when coalesce(cavparticipacao, -1) = -1 then 1
                        else 0
                    end as pendencias_participacao,
                    case
                        when coalesce(cavatividadesrealizadas, -1) = -1 then 1
                        else 0
                    end as pendencias_atividades,
                    case
                        when coalesce(cavsituacao, '') = '' then 1
                        else 0
                    end as pendencias_situacao
                    from sisfor.cursista c
                    inner join  sisfor.cursistacurso cc on cc.curid = c.curid
                    left  join  sisfor.cursistaavaliacoes ca on ca.cucid = cc.cucid
                    where fpbid = '$fpbid'
                    and sifid = '$sifid'
                ) as foo";

        $pendencias = $db->pegaUm($sql);

        if($pendencias){
            return "Não é possível finalizar o cadastramento enquanto todos os cursistas não forem avaliados.";
        }
    }

    return true;
}

function atualizarEmail($dados) {
	global $db;

	$sql = "UPDATE sisfor.identificacaousuario SET iusemailprincipal='".$dados['iusemailprincipal']."' WHERE iusd='".$dados['iusd']."'";
	$db->executar($sql);
	$db->commit();

}

//Planejamento - Fase 2
function pegarDocidPlanejamento2($unicod) {
    global $db;

    $sql = "SELECT docidplan2 FROM sisfor.sisfories WHERE unicod = '{$unicod}'";
    $docidplan2 = $db->pegaUm($sql);
    return trim($docidplan2);
}

//Planejamento - Fase 2
function criarDocumentoPlanejamento2($unicod) {
    global $db;

    if (!isset($_SESSION['sisfor']['unicod'])) {
        return false;
    }

    if (empty($_SESSION['sisfor']['unicod'])) {
        return false;
    }

    if (empty($unicod)) {
        return false;
    }

    $docid = pegarDocidPlanejamento2($unicod);

    if (empty($docid)) {
        $docdsc = "SISFOR Tramitação Planejamento - Fase 2 - " . $unicod;
        $docidplan2 = wf_cadastrarDocumento(WF_TPDID_SISFOR_PLAN2, $docdsc);
        $unitpocod = selecionarUnidade($unicod);
        if (empty($unicod)) {
            return false;
        } else {
            $sql = "UPDATE sisfor.sisfories SET docidplan2 = {$docidplan2} WHERE unicod = '{$unicod}'";
            $db->executar($sql);
            $db->commit();
            return $docid;
        }
    } else {
        return $docid;
    }
}

//Planejamento - Fase 2
function carregarDadosPlanejamento(){
	global $db;
	
	$sql = "SELECT SUM(VPPVALOR) AS loa FROM sisfor.valorprevistoploa WHERE unicod = '{$_SESSION['sisfor']['unicod']}'";
	$saldoloa = $db->pegaUm($sql);
	
	$sql = "SELECT 		COALESCE(SUM(orcvlrunitario),0) AS projeto
			FROM 		sisfor.orcamento o
			INNER JOIN 	sisfor.sisfor s  ON o.sifid = s.sifid
			INNER JOIN 	workflow.documento d on d.docid = s.docidprojeto
			INNER JOIN 	workflow.estadodocumento e on e.esdid = d.esdid		
			WHERE 		s.unicod = '{$_SESSION['sisfor']['unicod']}' AND e.esdid = ".ESD_PROJETO_VALIDADO." and s.sifstatus = 'A' and o.orcstatus = 'A' and s.siftipoplanejamento  = 1";
	
	$saldoprojeto = $db->pegaUm($sql);

	$saldoUsado = pegaValorNaoAplicadoFase2();

	$saldodisp = $saldoloa - $saldoprojeto;
	$saldocomp = $saldodisp - $saldoUsado;
	
	$planejamento = array('saldoloa'=>$saldoloa,'saldoprojeto'=>$saldoprojeto,'saldodisp'=>$saldodisp,'saldocomp'=>$saldocomp,'saldoacoes'=>$saldogasto['recurso'],'saldoaditivo'=>$saldoaditivo['aditivo']);
	return $planejamento;
}


function form_existeSaldoFase02() {
	global $db;

	$sql = "SELECT SUM(VPPVALOR) AS loa FROM sisfor.valorprevistoploa WHERE unicod = '{$_SESSION['sisfor']['unicod']}'";
	$saldoloa = $db->pegaUm($sql);
	
	$sql = "SELECT 		COALESCE(SUM(orcvlrunitario),0) AS projeto
	FROM 		sisfor.orcamento o
	INNER JOIN 	sisfor.sisfor s  ON o.sifid = s.sifid
	INNER JOIN 	workflow.documento d on d.docid = s.docidprojeto
	INNER JOIN 	workflow.estadodocumento e on e.esdid = d.esdid
	WHERE 		s.unicod = '{$_SESSION['sisfor']['unicod']}' AND e.esdid = ".ESD_PROJETO_VALIDADO." and s.sifstatus = 'A' and o.orcstatus = 'A' and s.siftipoplanejamento  = 1";
	
	$saldoprojeto = $db->pegaUm($sql);
	
	$saldoUsado = pegaValorNaoAplicadoFase2();
	
	$saldodisp = $saldoloa - $saldoprojeto;
	$saldocomp = $saldodisp - $saldoUsado;
	
	if( ( $saldodisp - $saldocomp ) > 0 ){
		echo "<div>Ainda há saldo de recursos orçamentários. Deseja realmente finalizar a Fase 2?</div>";
	} else {
		echo "<div>Confirma a finalização da fase 2?</div>";
	}
}

function existeSaldoFase02() {
	$retorno = Array('boo' => true, 'msg' => '');
	$retorno = simec_json_encode($retorno);
	echo $retorno;
}

function atualizarSaldoPlanejamento2($post){
	global $db;

    extract($post);
    
	$planejamento = carregarDadosPlanejamento();
	extract($planejamento);
	
	$saldo = formata_valor($saldodisp - $saldoacoes - $saldoaditivo - desformata_valor($loa));
	$resultado["saldo"] = iconv("ISO-8859-1", "UTF-8", $saldo);
	
    if ($saldo < 0) {
        $resultado["resultado"] = "N";
    } else {
        $resultado["resultado"] = "S";
    }
    echo simec_json_encode($resultado);
}

function salvarCustoAtividade($post){
    global $db;

    extract($post);

    $orcvlrunitario = desformata_valor($orcvlrunitario);

    $sql = "INSERT INTO sisfor.orcamentoplanejamentofase2
	   			  (sifid, gdeid, orcvlrunitario, orcstatus, orcdescricao, orctipo)
			VALUES ({$sifid},{$gdeid},'{$orcvlrunitario}','A','{$orcdescricao}', {$orctipo}) RETURNING orcid";

    $orcid = $db->pegaUm($sql);   
    
    if ($orcid) {
	    $sql = "SELECT sifvalorloa, sifvaloroutras FROM sisfor.sisfor WHERE sifid = {$sifid}";	
	    $valor = $db->pegaLinha($sql);
	
    	if($orctipo == '1'){
	    	$sifvalorloa = $valor['sifvalorloa'] + desformata_valor($orcvlrunitario);
	    	$sifvaloroutras = empty($valor['sifvaloroutras']) ? 'NULL' : "'{$valor['sifvaloroutras']}'";
	    } elseif($orctipo == '2'){
	    	$sifvalorloa = empty($valor['sifvalorloa']) ? 'NULL' : "'{$valor['sifvalorloa']}'";
	    	$sifvaloroutras = $valor['sifvaloroutras'] + desformata_valor($orcvlrunitario);
	    }
	    $sql = "UPDATE sisfor.sisfor SET sifvalorloa = $sifvalorloa, sifvaloroutras = $sifvaloroutras WHERE sifid = {$sifid}";
    }
    
    if($db->executar($sql)) {
        $db->commit();
        $db->sucesso('principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2', '', 'Custo Atividade cadastrado com sucesso!', 'S', 'S');
    } else {
        $db->insucesso('Não foi possível realizar a operação.', '', 'principal/coordenador/inserircustoatividade&acao=A');
    }
}

function alterarCustoAtividade($post){
    global $db;

    extract($post);

    $orcvlrunitario = desformata_valor($orcvlrunitario);
    
    $sql = "SELECT orcvlrunitario FROM sisfor.orcamentoplanejamentofase2 WHERE orcid = {$orcid}";
    $loaantigo = $db->pegaUm($sql);
    
    $sql = "SELECT sifvalorloa, sifvaloroutras FROM sisfor.sisfor WHERE sifid = {$sifid}";	
	$valor = $db->pegaLinha($sql);   	
    
    if($orctipo=='1'){
    	$sifvalorloa = ($valor['sifvalorloa'] + $orcvlrunitario) - $loaantigo;
    	$sql = "UPDATE sisfor.sisfor SET sifvalorloa = {$sifvalorloa} WHERE sifid = {$sifid}";
    } elseif($orctipo=='2'){
    	$sifvaloroutras = ($valor['sifvaloroutras'] + $orcvlrunitario) - $loaantigo;
    	$sql = "UPDATE sisfor.sisfor SET sifvaloroutras = {$sifvaloroutras} WHERE sifid = {$sifid}";
    }
	
    $db->executar($sql);    

    $sql = "UPDATE 	sisfor.orcamentoplanejamentofase2
    		SET		gdeid = {$gdeid}, 
    				orcvlrunitario = '{$orcvlrunitario}',
    				orcdescricao = '{$orcdescricao}',
    				orctipo = {$orctipo}
    		WHERE	orcid = {$orcid}";
    
    $db->executar($sql);
        
    if ($db->commit()) {
        $db->sucesso('principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2', '', 'Custo Atividade atividade alterado com sucesso!', 'S', 'S');
    } else {
        $db->insucesso('Não foi possível realizar a operação.', '', 'principal/coordenador/inserircustoatividade&acao=A');
    }
}


function visualizarCustoAtividade($orcid, $tipo = null) {
    global $db;

    $aryWhere[] = "orcstatus = 'A'";

    if ($orcid) {
        $aryWhere[] = "orcid = {$orcid}";
    }

    $sql = "SELECT 		orcid,
    					sifid,
						gdeid,
						orcdescricao,
						orctipo,
						TRIM(TO_CHAR(COALESCE(orcvlrunitario,0),'999G999G990D99')) AS orcvlrunitario
  			FROM 		sisfor.orcamentoplanejamentofase2
  						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";

    $custo = $db->pegaLinha($sql);

    if ($tipo == 'html') {
        return $custo;
    } else {
    	$custo["gdeid"] = iconv("ISO-8859-1", "UTF-8", $custo["gdeid"]);
        $custo["orcdescricao"] = iconv("ISO-8859-1", "UTF-8", $custo["orcdescricao"]);
        $custo["orcvlrunitario"] = iconv("ISO-8859-1", "UTF-8", $custo["orcvlrunitario"]);
        echo simec_json_encode($custo);
    }
}

function excluirCustoAtividade($orcid) {
    global $db;
    
    $sql = "SELECT sifid, orcvlrunitario, orctipo FROM sisfor.orcamentoplanejamentofase2 WHERE orcid = {$orcid}";
    $orc = $db->pegaLinha($sql);

    $sql = "SELECT sifvalorloa, sifvaloroutras FROM sisfor.sisfor WHERE sifid = {$orc['sifid']}";	
	$valor = $db->pegaLinha($sql);   	
    
    if($orc['orctipo']=='1'){
    	$sifvalorloa = $valor['sifvalorloa'] - $orc['orcvlrunitario'];
    	$sql = "UPDATE sisfor.sisfor SET sifvalorloa = {$sifvalorloa} WHERE sifid = {$orc['sifid']}";
    } elseif($orc['orctipo']=='2'){
    	$sifvaloroutras = $valor['sifvaloroutras'] - $orc['orcvlrunitario'];
    	$sql = "UPDATE sisfor.sisfor SET sifvaloroutras = {$sifvaloroutras} WHERE sifid = {$orc['sifid']}";
    }
	
    $db->executar($sql);
    
    if ($orcid != '') {
        $sql = "UPDATE sisfor.orcamentoplanejamentofase2 SET orcstatus = 'I' WHERE orcid = {$orcid}";
    }

    if ($db->executar($sql)) {
        $db->commit();
    }
}

function listarCustoAtividade($sifid = NULL,$acao = NULL){
    global $db;

    $aryWhere[] = "orc.orcstatus = 'A'";

    if ($sifid) {
        $aryWhere[] = "orc.sifid = {$sifid}";
    }

    if($acao == 'S'){
	    $acao = "'<img border=\"0\" src=\"../imagens/alterar.gif\" id=\"'|| orc.orcid ||'\" onclick=\"visualizarCustoAtividade('|| orc.orcid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
				  <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| orc.orcid ||'\" onclick=\"excluirCustoAtividade('|| orc.orcid ||');\" style=\"cursor:pointer;\"/>' AS acao,";
	    $cabecalho = array('Ação', 'Grupo de Despesa', 'Unidade de Medida', 'Valor Total (R$)', 'Detalhamento','Fonte de Recurso');
	    $tamanho = array('5%', '8%', '10%', '10%', '28%', '15%');
	    $alinhamento = array('center','center', 'center', 'right','left','left');    	
    } else { 
		$acao = "";    	
		$cabecalho = array('Grupo de Despesa', 'Unidade de Medida', 'Valor Total (R$)', 'Detalhamento','Fonte de Recurso');
    	$tamanho = array('8%', '8%', '10%', '28%', '15%');
    	$alinhamento = array('center', 'right','left','left');		
    }
    
    $sql = "SELECT 		$acao
    					gru.gdedesc,
    					'Verba' AS unidademedida,
    					COALESCE(orc.orcvlrunitario,0)::numeric(20,2) AS orcvlrunitario,
    					orc.orcdescricao,
    					CASE WHEN orc.orctipo = '1' THEN 'LOA' 
    						 WHEN orc.orctipo = '2' THEN 'Outras Fontes' END AS orctipo						
  			FROM 		sisfor.orcamentoplanejamentofase2 orc
  			LEFT JOIN	sisfor.grupodespesa gru ON gru.gdeid = orc.gdeid
  						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";
    

    $param['totalLinhas'] = false;
    $db->monta_lista($sql, $cabecalho, '50', '10', 'S', 'center', 'S', '', $tamanho, $alinhamento, '', $param);    
}

function exibirListaCustoAtividade($post){
	global $db;
	
	extract($post);
	listarCustoAtividade($sifid,'S');
}

function selecionarFasePlanejamento($dados){
    global $db;
    
    extract($dados);
    
    if($oatid){
    	$aryWhere[] = "oatid = {$oatid}";
    }
    
    if($ocuid){
    	$aryWhere[] = "ocuid = {$ocuid}";
    }
    
    if($cnvid){
    	$aryWhere[] = "cnvid = {$cnvid}";
    }
    
    if($ieoid){
    	$aryWhere[] = "ieoid = {$ieoid}";
    }
    
    $sql = "SELECT 		siftipoplanejamento 
    		FROM 		sisfor.sisfor
 						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";
    
    $siftipoplanejamento = $db->pegaUm($sql);
    
    return $siftipoplanejamento;
}


function listarCoordenadores(){
    global $db;
	
	$cabecalho = array('Código do curso', 'Nome do curso', 'Corrdenação', 'Telefone 1','Telefone 2','Coordenador(a)','E-mail','Site','Informação');
    $tamanho = array('8%', '30%', '30%', '10%', '10%','15%');
    $alinhamento = array('center', 'left','left','left');	
    
    $sql = "SELECT		curid AS cod_curso,
						curdesc AS curso,
						coorddesc AS coordenacao,
						curconttel AS telefone_1,
						curconttel2 AS telefone_2,
						curcontdesc AS coordenador,
						curcontemail AS email,
						curcontsite AS site,
						curcontinfo AS informacao
			FROM        catalogocurso2014.curso cur
			INNER JOIN  catalogocurso2014.coordenacao coor on coor.coordid = cur.coordid
			WHERE 		cur.curstatus = 'A'
			ORDER BY 	1,2,3";
    

    $param['totalLinhas'] = true;
    $db->monta_lista($sql, $cabecalho, '50', '10', 'S', 'center', 'N', '', $tamanho, $alinhamento, '', $param);    	
}

function listarCursoValidado(){

    global $db;
	
	  	$wh[] = "d.esdid=".ESD_PROJETO_VALIDADO."";
	    
	  	$wh[] = "u.unicod='{$_SESSION['sisfor']['unicod']}'";
	  	  
	    
	    $sql = "select
	  
	    uniabrev||' - '||unidsc as universidade,
	    case when s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc
	    when s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc
	    when s.ocuid is not null then oc.ocunome end as curso,
	    e.esddsc,
	    coalesce(ectur.esddsc,'Não iniciou') as esddsc_comporturma,
	    s.sifprofmagisterio,
	    (select sum(orcvlrunitario) as orcvlrunitario from sisfor.orcamento where sifid=s.sifid) as valortotal,
	    case when s.ieoid is not null then cor.coordsigla
	    when s.cnvid is not null then cor2.coordsigla
	    when s.ocuid is not null then cor3.coordsigla end as secretaria,
	    case when s.ieoid is not null then case when s.sifopcao='1' then 'Curso Proposto pelo MEC – Aceito'
	    when s.sifopcao='2' then 'Curso Proposto pelo MEC – Rejeitado'
	    when s.sifopcao='3' then 'Curso Proposto pelo MEC – Repactuado'
	    else 'não definido' end
	    when s.cnvid is not null then 'Curso Proposto pela IES – Do Catálogo'
	    when s.ocuid is not null then 'Curso Proposto pela IES – Fora do Catálogo' end as tipo,
	    to_char(htddata, 'dd/mm/YYYY HH24:MI') as htddata
	    from sisfor.sisfor s
	    inner join workflow.documento d on d.docid = s.docidprojeto
	    left join workflow.historicodocumento h on h.hstid = d.hstid
	    inner join workflow.estadodocumento e on e.esdid = d.esdid
	    inner join public.unidade u on u.unicod = s.unicod
	    left join workflow.documento dctur on dctur.docid = s.docidcomposicaoequipe
	    left join workflow.estadodocumento ectur on ectur.esdid = dctur.esdid
	    left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
	    left join catalogocurso2014.curso cur on cur.curid = ieo.curid
	    left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
	    left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
	    left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
	    left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
	    left join seguranca.usuario usu on usu.usucpf = s.usucpf
	    left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
	    left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid
	    where sifstatus='A' ".(($wh)?" AND ".implode(" AND ", $wh):"")."
	    order by unidsc";
	    
	    $cabecalho = array("Universidade","Curso","Situação","Equipe","Vagas previstas (meta)","Valor do projeto","Secretaria","Tipo de curso","Data de tramitação");
	    
	   
	    
		$param['managerOrder'][8]['campo'] = 'htddata';
	    $param['managerOrder'][8]['alias'] = 'htddata';
	    
	    
	    $db->monta_lista($sql,$cabecalho,50,10,'N','center','','','','','',$param);
	    
}


function selecionarCursoAditivo($sifid = NULL){
    global $db;

    $cabecalho = array('Ação', 'Código do curso', 'Nome do curso', 'Coordenador do curso', 'Secretaria/ Diretoria responsável no MEC','Data de validação', 'Qtd. Vagas', 'Valor LOA', 'Valor Outras', 'Qtd. Vagas Aditivo','Valor LOA Aditivo');
    
    $aryWhere[] = "e.esdid = ".ESD_PROJETO_VALIDADO." AND s.unicod = '{$_SESSION['sisfor']['unicod']}' AND s.sifstatus = 'A' AND s.siftipoplanejamento = ".FASE01."";

    if($sifid){
    	$aryWhere[] = "s.sifid = {$sifid}";
    	$acao = "'<input type=\"checkbox\" name=\"sifid[]\" id=\"sifid\" value=\"'|| s.sifid ||'\" checked>' AS acao,";
    	
    	$select = "CASE WHEN s.sifqtdvagaaditivo IS NOT NULL THEN '<input id=\"sifqtdvagaaditivo_'|| s.sifid ||'\" type=\"text\" class=\"normal\" onkeyup=\"this.value=mascaraglobal(\'####\',this.value);\" value=\"'|| s.sifqtdvagaaditivo ||'\" name=\"sifqtdvagas_'|| s.sifid ||'\" size=\"6\">' ELSE '<input id=\"sifqtdvagaaditivo_'|| s.sifid ||'\" type=\"text\" class=\"normal\" onkeyup=\"this.value=mascaraglobal(\'####\',this.value);\" value=\"\" name=\"sifqtdvagaaditivo_'|| s.sifid ||'\" size=\"6\">' END AS sifqtdvagaaditivo,
	               CASE WHEN s.sifvalorloaaditivo IS NOT NULL THEN '<input id=\"sifvalorloaaditivo_'|| s.sifid ||'\" type=\"text\" class=\"normal\" onblur=\"atualizarSaldo(this.value);\" onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" value=\"'|| TRIM(TO_CHAR(COALESCE(s.sifvalorloaaditivo,0),'999G999G990D99')) ||'\" name=\"sifvalorloaaditivo_'|| s.sifid ||'\" size=\"10\" maxlenght=\"9\">' ELSE '<input id=\"sifvalorloaaditivo_'|| s.sifid ||'\" type=\"text\" class=\"normal\" onblur=\"atualizarSaldo(this.value);\" onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" value=\"\" name=\"sifvalorloaaditivo_'|| s.sifid ||'\" size=\"10\" maxlenght=\"9\">' END AS sifvalorloaaditivo";
    	
    } else {
	    $acao = "'<input type=\"checkbox\" name=\"sifid[]\" id=\"sifid\" value=\"'|| s.sifid ||'\">' AS acao,";

    	$select = "'<input id=\"sifqtdvagaaditivo_'|| s.sifid ||'\" type=\"text\" class=\"normal\" onkeyup=\"this.value=mascaraglobal(\'####\',this.value);\" value=\"\" name=\"sifqtdvagaaditivo_'|| s.sifid ||'\" size=\"6\">' AS sifqtdvagaaditivo,
	               '<input id=\"sifvalorloaaditivo_'|| s.sifid ||'\" type=\"text\" class=\"normal\" onblur=\"atualizarSaldo(this.value);\" onkeyup=\"this.value=mascaraglobal(\'[.###],##\',this.value);\" value=\"\" name=\"sifvalorloaaditivo_'|| s.sifid ||'\" size=\"10\" maxlenght=\"9\">' AS sifvalorloaaditivo";
    }
        
    $sql = "SELECT		$acao
    					CASE WHEN s.ieoid IS NOT NULL THEN ieo.codigo
     						 WHEN s.cnvid IS NOT NULL THEN cnv.codigo 
     						 ELSE NULL END AS codigo_curso,
						CASE WHEN s.ieoid IS NOT NULL THEN ieo.nome
						     WHEN s.cnvid IS NOT NULL THEN cnv.nome 
						     WHEN s.ocuid IS NOT NULL THEN ocu.nome END AS nome, 
  						ius.iusnome,
						CASE WHEN s.ieoid IS NOT NULL THEN ieo.sigla
						     WHEN s.cnvid IS NOT NULL THEN cnv.sigla
						     WHEN s.ocuid IS NOT NULL THEN ocu.sigla END AS sigla,
						TO_CHAR(h.htddata,'dd/mm/yyyy') AS datavalidacao,
	                	CASE WHEN s.sifnumvagasofertadas IS NULL THEN 0 ELSE s.sifnumvagasofertadas END AS sifnumvagasofertadas,
	                	s.sifvalorloa,
	                	s.sifvaloroutras,
	                	$select					     
			FROM 	    sisfor.sisfor s 
			INNER JOIN 	workflow.documento d ON d.docid = s.docidprojeto 
			INNER JOIN 	workflow.estadodocumento e ON e.esdid = d.esdid
			INNER JOIN 	workflow.historicodocumento h ON h.hstid = d.hstid
			LEFT JOIN 	sisfor.tipoperfil tpe ON tpe.tpeid = s.tpeid AND tpe.pflcod = 1105
			LEFT JOIN	sisfor.identificacaousuario ius ON ius.iuscpf = s.usucpf AND ius.iusstatus = 'A'
			LEFT JOIN  (SELECT 		cnv.curid AS codigo, coo.coordsigla AS sigla, cur.curdesc AS nome, cnv.cnvid 
						FROM 		sisfor.cursonaovinculado cnv 
						INNER JOIN 	catalogocurso2014.curso cur ON cnv.curid = cur.curid
						INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
						WHERE		cnv.cnvstatus = 'A') AS cnv ON cnv.cnvid = s.cnvid
			LEFT JOIN  (SELECT 		coo.coordsigla AS sigla, ocu.ocunome AS nome, ocu.ocuid
			 			FROM 		sisfor.outrocurso ocu
			 			INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = ocu.coordid
			 			WHERE		ocu.ocustatus = 'A') AS ocu ON ocu.ocuid = s.ocuid
			LEFT JOIN (SELECT 		cur.curid AS codigo, coo.coordsigla AS sigla, cur.curdesc AS nome, ieo.ieoid
			 		   FROM 		catalogocurso2014.iesofertante ieo							
			 		   INNER JOIN 	catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
			  		   INNER JOIN  	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
					   WHERE		ieo.ieostatus = 'A') AS ieo ON ieo.ieoid = s.ieoid
					   ".(is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '')."
			ORDER BY 	nome";        

    $alinhamento = array('center', 'center', 'left', 'left', 'center', 'center','center','right','right');
    $tamanho = array('7%', '7%', '25%', '15%', '10%', '7%', '7%','8%','8%','8%','8%');
    $db->monta_lista($sql, $cabecalho, '50', '10', 'N', 'center', 'N', 'formulario_curso', $tamanho, $alinhamento);
}

function alterarCursoAditivo($post) {
    global $db;
    
    extract($post);

    foreach ($sifid AS $s) {
        $sifvalorloaaditivo = desformata_valor($post['sifvalorloaaditivo_' . $s]);
        $sifqtdvagaaditivo = $post['sifqtdvagaaditivo_' . $s];

        $sql = "UPDATE   sisfor.sisfor
				SET	  	 sifvalorloaaditivo = '{$sifvalorloaaditivo}',
				 		 sifqtdvagaaditivo = {$sifqtdvagaaditivo},
				 		 sifaditivo = 't'
				WHERE 	 sifid = {$s}";
    }

    if ($db->executar($sql)) {
        $db->commit();
        $db->sucesso('principal/coordenador/coordenador_ies&acao=A&aba=planejamentoies2', '', 'Aditivo cadastrado com sucesso!', 'S', 'S');
    } else {
        $db->insucesso('Não foi possível realizar a operação.', '', 'principal/coordenador/cadastrocursocatalogo2&acao=A');
    }
}


function listarCursoAditivo(){
    global $db;

    $cabecalho = array('Ação', 'Código do curso', 'Nome do curso', 'Coordenador do curso', 'Secretaria/ Diretoria responsável no MEC','Data de validação', 'Qtd. Vagas', 'Valor LOA', 'Valor Outras', 'Qtd. Vagas Aditivo','Valor LOA Aditivo');
    
    $aryWhere[] = "e.esdid = ".ESD_PROJETO_VALIDADO." AND s.unicod = '{$_SESSION['sisfor']['unicod']}' AND s.sifstatus = 'A' AND s.siftipoplanejamento = ".FASE01." AND s.sifaditivo = 't'";
   	$aryWhere[] = "s.sifvalorloaaditivo IS NOT NULL AND s.sifqtdvagaaditivo IS NOT NULL";
    
    $acao = "'<img border=\"0\" src=\"../imagens/alterar.gif\" id=\"'|| s.sifid ||'\" onclick=\"alterarCursoAditivo('|| s.sifid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;' AS acao,";
        
    $sql = "SELECT		$acao
    					CASE WHEN s.ieoid IS NOT NULL THEN ieo.codigo
     						 WHEN s.cnvid IS NOT NULL THEN cnv.codigo 
     						 ELSE NULL END || ' ' AS codigo_curso,
						CASE WHEN s.ieoid IS NOT NULL THEN ieo.nome
						     WHEN s.cnvid IS NOT NULL THEN cnv.nome 
						     WHEN s.ocuid IS NOT NULL THEN ocu.nome END AS nome, 
  						ius.iusnome,
						CASE WHEN s.ieoid IS NOT NULL THEN ieo.sigla
						     WHEN s.cnvid IS NOT NULL THEN cnv.sigla
						     WHEN s.ocuid IS NOT NULL THEN ocu.sigla END AS sigla,
						TO_CHAR(h.htddata,'dd/mm/yyyy') AS datavalidacao,
	                	CASE WHEN s.sifqtdvagas IS NULL THEN 0 ELSE s.sifqtdvagas END AS sifqtdvagas,
	                	s.sifvalorloa,
	                	s.sifvaloroutras,					     
						s.sifqtdvagaaditivo,
						s.sifvalorloaaditivo
			FROM 	    sisfor.sisfor s 
			INNER JOIN 	workflow.documento d ON d.docid = s.docidprojeto 
			INNER JOIN 	workflow.estadodocumento e ON e.esdid = d.esdid
			INNER JOIN 	workflow.historicodocumento h ON h.hstid = d.hstid
			LEFT JOIN 	sisfor.tipoperfil tpe ON tpe.tpeid = s.tpeid AND tpe.pflcod = 1105
			LEFT JOIN	sisfor.identificacaousuario ius ON ius.iuscpf = s.usucpf AND ius.iusstatus = 'A'
			LEFT JOIN  (SELECT 		cnv.curid AS codigo, coo.coordsigla AS sigla, cur.curdesc AS nome, cnv.cnvid 
						FROM 		sisfor.cursonaovinculado cnv 
						INNER JOIN 	catalogocurso2014.curso cur ON cnv.curid = cur.curid
						INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
						WHERE		cnv.cnvstatus = 'A') AS cnv ON cnv.cnvid = s.cnvid
			LEFT JOIN  (SELECT 		coo.coordsigla AS sigla, ocu.ocunome AS nome, ocu.ocuid
			 			FROM 		sisfor.outrocurso ocu
			 			INNER JOIN 	catalogocurso2014.coordenacao coo ON coo.coordid = ocu.coordid
			 			WHERE		ocu.ocustatus = 'A') AS ocu ON ocu.ocuid = s.ocuid
			LEFT JOIN (SELECT 		cur.curid AS codigo, coo.coordsigla AS sigla, cur.curdesc AS nome, ieo.ieoid
			 		   FROM 		catalogocurso2014.iesofertante ieo							
			 		   INNER JOIN 	catalogocurso2014.curso cur ON cur.curid = ieo.curid AND cur.curstatus = 'A'
			  		   INNER JOIN  	catalogocurso2014.coordenacao coo ON coo.coordid = cur.coordid
					   WHERE		ieo.ieostatus = 'A') AS ieo ON ieo.ieoid = s.ieoid
					   ".(is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '')."
			ORDER BY 	nome";        

    $alinhamento = array('center', 'center', 'left', 'left', 'center', 'center','center','right','right');
    $tamanho = array('5%', '7%', '25%', '15%', '10%', '7%', '6%','6%','6%','6%','6%');
    $db->monta_lista($sql, $cabecalho, '50', '10', 'S', 'center', 'S', 'formulario_curso', $tamanho, $alinhamento);
}

function reiniciarSenha($dados) {
	global $db;

	$sql = "UPDATE seguranca.usuario SET ususenha='".md5_encrypt_senha("simecdti","")."' WHERE usucpf='".$dados['usucpf']."'";
	$db->executar($sql);

	$sql = "UPDATE seguranca.usuario_sistema SET suscod='A' WHERE usucpf='".$dados['usucpf']."' AND sisid='".SIS_SISFOR."'";
	$db->executar($sql);

	$db->commit();

	$arrUsu = $db->pegaLinha("SELECT usunome, usuemail FROM seguranca.usuario WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['usucpf'])."'");

	$remetente = array("nome" => "SIMEC - MÓDULO SISFOR","email" => $arrUsu['usuemail']);
	$destinatario = $arrUsu['usuemail'];
	$usunome = $arrUsu['usunome'];

	$assunto = "Atualização de senha no SIMEC - MÓDULO SISFOR";
	$conteudo = "<br/><span style='background-color: red;'><b>Esta é uma mensagem gerada automaticamente pelo sistema. </b></span><br/><span style='background-color: red;'><b>Por favor, não responda. Pois, neste caso, a mesma será descartada.</b></span><br/><br/>";
	$conteudo .= sprintf("%s %s, <p>Você foi cadastrado no SIMEC, módulo SISFOR. Sua conta está ativa e, para acessa-la basta entrar no SIMEC (http://simec.mec.gov.br), digitar o seu CPF e senha.</p>
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

	$al = array("alert"=>"Senha reiniciada com sucesso","location"=>"sisfor.php?modulo=".$dados['modulo']."&acao=A&aba=".$dados['aba']);
	alertlocation($al);


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

	$sql = "SELECT * FROM sisfor.identificacaousuario i INNER JOIN sisfor.tipoperfil t ON t.iusd = i.iusd WHERE i.iusd='".$dados['iusdantigo']."'";
	$identificacaousuario_antigo = $db->pegaLinha($sql);

	if(!$identificacaousuario_antigo) {
		$al = array("alert"=>"Usuário a ser substituido não foi encontrado","location"=>$_SERVER['HTTP_REFERER']);
		alertlocation($al);
	}
	
	$sql = "SELECT i.iusd, t.tpeid, i.iusnome FROM sisfor.identificacaousuario i LEFT JOIN sisfor.tipoperfil t ON t.iusd = i.iusd WHERE i.iuscpf='".str_replace(array(".","-"),array("",""),$dados['iuscpf_'])."'";
	$identificacaousuario_novo = $db->pegaLinha($sql);
	
	if(!$identificacaousuario_novo['iusd']) {

		$sql = "INSERT INTO sisfor.identificacaousuario(
 	            iuscpf, iusnome, iusemailprincipal, muncodatuacao,
 	            iusdatainclusao, iusstatus)
 			    VALUES ('".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."', '".$dados['iusnome_']."', '".$dados['iusemailprincipal_']."',".(($identificacaousuario_antigo['muncodatuacao'])?"'".$identificacaousuario_antigo['muncodatuacao']."'":"NULL").",
 			            NOW(), 'A') RETURNING iusd;";
		
		$identificacaousuario_novo['iusd'] = $db->pegaUm($sql);
	}

	$sql = "UPDATE sisfor.tipoperfil SET iusd='".$identificacaousuario_novo['iusd']."' WHERE iusd='".$identificacaousuario_antigo['iusd']."' ".(($_SESSION['sisfor']['sifid'])?" AND sifid='".$_SESSION['sisfor']['sifid']."'":"")." AND pflcod='".$identificacaousuario_antigo['pflcod']."'";
	$db->executar($sql);
	
	$existe_vinculo = $db->pegaUm("SELECT count(*) as num FROM sisfor.tipoperfil WHERE iusd='".$identificacaousuario_antigo['iusd']."'");
	
	if(!$existe_vinculo) {

		$sql = "DELETE FROM sisfor.usuarioresponsabilidade WHERE usucpf='".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."'";
		$db->executar($sql);

		$sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf='".$identificacaousuario_antigo['iuscpf']."' AND pflcod='".$dados['pflcod_']."'";
		$db->executar($sql);

	}
	
	if($identificacaousuario_antigo['pflcod']==PFL_COORDENADOR_INST) {
		$sql = "UPDATE sisfor.sisfories SET usucpf = '".str_replace(array(".","-"),array(""),$dados['iuscpf_'])."' where unicod='".$dados['unicod']."'";
		$db->executar($sql);
	}


	$db->commit();

	if(!$dados['noredirect']) {
		$al = array("alert"=>"Troca efetuada com sucesso.","location"=>$_SERVER['HTTP_REFERER']);
		alertlocation($al);
	} else {
		return true;
	}
}

function finalizarPlanejamento2() {
    global $db;

    $planejamento = carregarDadosPlanejamento();
    extract($planejamento);

    $saldo = $saldodisp - $saldoacoes; 


    $sql = "SELECT  			COUNT(sifid) AS coordatividade
			FROM 	   		sisfor.sisfor sif 
			INNER JOIN 		sisfor.outraatividade oat ON sif.oatid = oat.oatid
			WHERE 			sif.unicod = '{$_SESSION['sisfor']['unicod']}' AND sif.sifstatus = 'A' AND sif.usucpf IS NULL AND sif.siftipoplanejamento = ".FASE02."";

    $coordatividade = $db->pegaUm($sql);

    $sql = "SELECT 	 		COUNT(sifid) AS coordoutrocurso
			FROM 	   		sisfor.sisfor sif 
			INNER JOIN 		sisfor.outrocurso out ON sif.ocuid = out.ocuid
			WHERE 			sif.unicod = '{$_SESSION['sisfor']['unicod']}' AND sif.sifstatus = 'A' AND sif.usucpf IS NULL AND sif.siftipoplanejamento = ".FASE02."";

    $coordoutrocurso = $db->pegaUm($sql);


    $sql = "SELECT 	 		COUNT(sifid) AS coordnaovinculado
			FROM 	   		sisfor.sisfor sif
			INNER JOIN 		sisfor.cursonaovinculado cnv ON sif.cnvid = cnv.cnvid
			WHERE 			sif.unicod = '{$_SESSION['sisfor']['unicod']}' AND sif.sifstatus = 'A' AND sif.usucpf IS NULL AND sif.siftipoplanejamento = ".FASE02."";

    $coordnaovinculado = $db->pegaUm($sql);


    if ($coordatividade <> 0 || $coordoutrocurso <> 0 || $coordnaovinculado <> 0){
        return "Favor informar o Coordenador do Curso.";
    }

    if ($saldo && $saldo < 0) {
        return "Não há saldo suficiente de recursos LOA.";
    }
    return true;
}


function ativarTodos($dados) {
	global $db;
	
	/* configurações */
	ini_set("memory_limit", "2048M");
	set_time_limit(0);
	/* FIM configurações */
	
	
	$sql = "select i.iuscpf, t.pflcod from sisfor.identificacaousuario i 
			inner join sisfor.tipoperfil t on t.iusd = i.iusd 
			inner join sisfor.sisfor s on s.sifid = t.sifid 
			inner join workflow.documento d on d.docid = s.docidprojeto 
			inner join workflow.documento d2 on d2.docid = s.docidcomposicaoequipe 
            left join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.sisid=177 
			where i.iustermocompromisso is null and d.esdid=1187 and d2.esdid=1205 and i.iusstatus='A' and (us.usucpf is null or us.suscod!='A')";
	
	$todos = $db->carregar($sql);
	
	$arr['suscod'] = 'A';
	$arr['noredirect'] = true;
		
	if($todos[0]) {
		foreach($todos as $td) {
			$arr['chk'][$td['pflcod']][] = $td['iuscpf'];
		}
	}
	
	ativarEquipe($arr);
	
}

function solicitarAlteracaoProjetoOrcamento($dados) {
	global $db;
	
	$sql = "INSERT INTO sisfor.alterarprojeto(
            alptipo, alpautorizado, usucpfautorizou, alpdtautorizou, 
            usucpfsolicitou, alpdtsolicitou, sifid)
    		VALUES ('orcamento', NULL, NULL, NULL, '".$_SESSION['usucpf']."', NOW(), '".$dados['sifid']."') RETURNING alpid;";
	
	$alpid = $db->pegaUm($sql);
	
	$sql = "INSERT INTO sisfor.orcamento(
            sifid, orcstatus, orcquantidade, orcvlrunitario, orcvlrtotal, 
            gdeid, orcdescricao, orcvlrloa2014, orcvlrloa2015, orcvlrloa2016, 
            alpid)
			SELECT sifid, 'S', orcquantidade, orcvlrunitario, orcvlrtotal, 
            gdeid, orcdescricao, orcvlrloa2014, orcvlrloa2015, orcvlrloa2016, 
            '{$alpid}' as alpid FROM sisfor.orcamento WHERE orcstatus='A' AND sifid='".$dados['sifid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	ob_clean();
	
	echo $alpid;

}

function solicitarAlteracaoProjetoMeta($dados) {
	global $db;

	$sql = "INSERT INTO sisfor.alterarprojeto(
            alptipo, alpautorizado, usucpfautorizou, alpdtautorizou,
            usucpfsolicitou, alpdtsolicitou, sifid)
    		VALUES ('meta', NULL, NULL, NULL, '".$_SESSION['usucpf']."', NOW(), '".$dados['sifid']."') RETURNING alpid;";

	$alpid = $db->pegaUm($sql);

	$sql = "INSERT INTO sisfor.sisfor_s(
            sifid, sifprofmagisterio, 
            alpid)
	SELECT sifid, sifprofmagisterio,
	'{$alpid}' as alpid FROM sisfor.sisfor WHERE sifid='".$dados['sifid']."'";

	$db->executar($sql);
	$db->commit();

	ob_clean();

	echo $alpid;

}

function solicitarAlteracaoProjetoVigencia($dados) {
	global $db;

	$sql = "INSERT INTO sisfor.alterarprojeto(
            alptipo, alpautorizado, usucpfautorizou, alpdtautorizou,
            usucpfsolicitou, alpdtsolicitou, sifid)
    		VALUES ('vigencia', NULL, NULL, NULL, '".$_SESSION['usucpf']."', NOW(), '".$dados['sifid']."') RETURNING alpid;";

	$alpid = $db->pegaUm($sql);

	$sql = "INSERT INTO sisfor.sisfor_s(
	sifid, sifvigenciadtini, sifvigenciadtfim,
	alpid)
	SELECT sifid, sifvigenciadtini, sifvigenciadtfim,
	'{$alpid}' as alpid FROM sisfor.sisfor WHERE sifid='".$dados['sifid']."'";

	$db->executar($sql);
	$db->commit();

	ob_clean();

	echo $alpid;

}

function carregarMeta($dados) {
	global $db;
	
	$sifprofmagisterio = $db->pegaUm("SELECT sifprofmagisterio FROM sisfor.sisfor WHERE sifid='".$dados['sifid']."'");
	echo campo_texto('sifprofmagisterio', "S", (($dados['consulta'])?'N':'S'), "Profissionais do magistério da Educação Básica", 7, 6, "######", "", '', '', 0, 'id="sifprofmagisterio"', '', $sifprofmagisterio );
	
}

function carregarMeta_s($dados) {
	global $db;
	
	$sifprofmagisterio = $db->pegaUm("SELECT sifprofmagisterio FROM sisfor.sisfor_s WHERE sifid='".$dados['sifid']."' ANd alpid='".$dados['alpid']."'");
	echo campo_texto('sifprofmagisterio_s', "S", (($dados['consulta'])?'N':'S'), "Profissionais do magistério da Educação Básica", 7, 6, "######", "", '', '', 0, 'id="sifprofmagisterio_s"', '', $sifprofmagisterio );
	
}

function carregarVigencia_s($dados) {
	global $db;
	
	$vigencia = $db->pegaLinha("SELECT sifvigenciadtini, sifvigenciadtfim FROM sisfor.sisfor_s WHERE sifid='".$dados['sifid']."' ANd alpid='".$dados['alpid']."'");
	
	echo campo_texto('sifvigenciadtini_s', "S", 'N', "", 12, 10, "##/##/####", "", '', '', 0, 'id="sifvigenciadtini_s"', '', formata_data($vigencia['sifvigenciadtini']) );
	echo ' ate ';
	echo campo_texto('sifvigenciadtfim_s', "S", (($dados['consulta'])?'N':'S'), "", 12, 10, "##/##/####", "", '', '', 0, 'id="sifvigenciadtfim_s"', '', formata_data($vigencia['sifvigenciadtfim']) );
	
}

function carregarVigencia($dados) {
	global $db;

	$vigencia = $db->pegaLinha("SELECT sifvigenciadtini, sifvigenciadtfim FROM sisfor.sisfor WHERE sifid='".$dados['sifid']."'");

	echo campo_texto('sifvigenciadtini', "S", (($dados['consulta'])?'N':'S'), "", 12, 10, "##/##/####", "", '', '', 0, 'id="sifvigenciadtini"', '', formata_data($vigencia['sifvigenciadtini']) );
	echo ' ate ';
	echo campo_texto('sifvigenciadtfim', "S", (($dados['consulta'])?'N':'S'), "", 12, 10, "##/##/####", "", '', '', 0, 'id="sifvigenciadtfim"', '', formata_data($vigencia['sifvigenciadtfim']) );

}


function solicitarAlteracaoProjetoBolsas($dados) {
	global $db;

	$sql = "INSERT INTO sisfor.alterarprojeto(
            alptipo, alpautorizado, usucpfautorizou, alpdtautorizou,
            usucpfsolicitou, alpdtsolicitou, sifid)
    		VALUES ('bolsas', NULL, NULL, NULL, '".$_SESSION['usucpf']."', NOW(), '".$dados['sifid']."') RETURNING alpid;";

	$alpid = $db->pegaUm($sql);

	$sql = "INSERT INTO sisfor.equipeies_s(
            sifid, pflcod, epiqtd, epivalor, alpid)
	SELECT sifid, pflcod, epiqtd, epivalor, '{$alpid}' as alpid FROM sisfor.equipeies WHERE sifid='".$dados['sifid']."'";

	$db->executar($sql);
	$db->commit();

	ob_clean();

	echo $alpid;

}

function gravarMeta_s($dados) {
	global $db;
	
	$sql = "UPDATE sisfor.sisfor_s SET sifprofmagisterio='".$dados['sifprofmagisterio_s']."' WHERE alpid='".$dados['alpid']."' AND sifid='".$_SESSION['sisfor']['sifid']."'";
	$db->executar($sql);
	$db->commit();
}

function gravarVigencia_s($dados) {
	global $db;
	
	$sql = "UPDATE sisfor.sisfor_s SET sifvigenciadtini='".formata_data_sql($dados['sifvigenciadtini_s'])."', sifvigenciadtfim='".formata_data_sql($dados['sifvigenciadtfim_s'])."' WHERE alpid='".$dados['alpid']."' AND sifid='".$_SESSION['sisfor']['sifid']."'";
	$db->executar($sql);
	$db->commit();
}

function inserirQtdEquipeIES_s($dados) {
	global $db;
	if ($dados['epiqtd_s']) {
		foreach ($dados['epiqtd_s'] as $pflcod => $epiqtd) {

			$qtd_pagamentos = $db->pegaUm("SELECT count(*) FROM sisfor.pagamentobolsista p 
										   INNER JOIN sisfor.tipoperfil t ON t.tpeid = p.tpeid  
										   INNER JOIN workflow.documento d ON d.docid = p.docid WHERE d.esdid!='".ESD_PAGAMENTO_NAO_AUTORIZADO."' AND p.pflcod='".$pflcod."' AND t.sifid='".$_SESSION['sisfor']['sifid']."'");
			
			if($qtd_pagamentos<=$epiqtd) {
	
			$epiid = $db->pegaUm("SELECT epiid FROM sisfor.equipeies_s WHERE sifid='" . $_SESSION['sisfor']['sifid'] . "' AND pflcod='" . $pflcod . "' AND alpid='".$dados['alpid']."'");
	
			if ($epiid) {
	
				$sql = "UPDATE sisfor.equipeies_s SET epiqtd=" . (($epiqtd) ? $epiqtd : '0') . ", epivalor=" . (($dados['epivalor_s'][$pflcod]) ? str_replace(array(".", ","), array("", "."), $dados['epivalor_s'][$pflcod]) : '0.00') . " WHERE epiid='" . $epiid . "'";
	
				$db->executar($sql);
			} else {
	
				$sql = "INSERT INTO sisfor.equipeies_s(
		            			sifid, pflcod, epiqtd, epivalor, alpid)
		    					VALUES ('" . $_SESSION['sisfor']['sifid'] . "', '" . $pflcod . "', " . (($epiqtd) ? $epiqtd : '0') . ", " . (($dados['epivalor_s'][$pflcod]) ? str_replace(array(".", ","), array("", "."), $dados['epivalor_s'][$pflcod]) : '0.00') . ", '".$dados['alpid']."');";
	
				$db->executar($sql);
			}
			
			} else {
				echo 'ATENÇÃO!! Essas alterações de quantitativos não foram gravados, pois as bolsas ja foram pagas.'."\n\n".'Perfil : '.$db->pegaUm("SELECT pfldsc FROM seguranca.perfil WHERE pflcod='".$pflcod."'").' possui '.$qtd_pagamentos.' pagamentos realizados ('.$epiqtd.' solicitados).'."\n";
			}
		}
	
		$db->commit();
	}
}

function confirmarSolicitacao($dados) {
	global $db;
	
	$sql = "UPDATE sisfor.alterarprojeto SET ".(($dados['alpjustificativa'])?"alpjustificativa='".substr(utf8_decode($dados['alpjustificativa']),0,2000)."',":"")." alpautorizado='".$dados['alpautorizado']."'  WHERE alpid='".$dados['alpid']."'";
	$db->executar($sql);
	
	$dados['tipo'] = $db->pegaUm("SELECT alptipo FROM sisfor.alterarprojeto WHERE alpid='".$dados['alpid']."'");
	
	if($dados['alpautorizado']=='2') {
		ob_start();
		if($dados['tipo']=='orcamento') {
			carregarListaCustos(array('sifid'=>$dados['sifid'],'consulta'=>true));
			$alphistorico = ob_get_contents();

			$sql = "DELETE FROM sisfor.orcamento WHERE sifid='".$dados['sifid']."' AND orcstatus='A'";
			$db->executar($sql);
			
			$sql = "UPDATE sisfor.orcamento SET orcstatus='A' WHERE alpid='".$dados['alpid']."'";
			$db->executar($sql);
			
		}
		if($dados['tipo']=='bolsas') {
			carregarQuantitativoPorPerfil(array('sifid'=>$dados['sifid'],'consulta'=>true));
			$alphistorico = ob_get_contents();
			
			$sql = "DELETE FROM sisfor.equipeies WHERE sifid='".$dados['sifid']."'";
			$db->executar($sql);
			
			$sql = "INSERT INTO sisfor.equipeies (
            		sifid, pflcod, epiqtd, epivalor)
    				SELECT sifid, pflcod, epiqtd, epivalor FROM sisfor.equipeies_s WHERE sifid='".$dados['sifid']."' AND alpid='".$dados['alpid']."'";
			$db->executar($sql);
		}
		if($dados['tipo']=='meta') {
			carregarMeta(array('sifid'=>$dados['sifid'],'consulta'=>true));
			$alphistorico = ob_get_contents();
				
			$sql = "UPDATE sisfor.sisfor SET sifprofmagisterio=(SELECT sifprofmagisterio FROM sisfor.sisfor_s WHERE sifid='".$dados['sifid']."' AND alpid='".$dados['alpid']."') WHERE sifid='".$dados['sifid']."'";
			$db->executar($sql);
		}
		if($dados['tipo']=='vigencia') {
			carregarVigencia(array('sifid'=>$dados['sifid'],'consulta'=>true));
			$alphistorico = ob_get_contents();
		
			$sql = "UPDATE sisfor.sisfor SET 
    										sifvigenciadtini=(SELECT sifvigenciadtini FROM sisfor.sisfor_s WHERE sifid='".$dados['sifid']."' AND alpid='".$dados['alpid']."'), 
    										sifvigenciadtfim=(SELECT sifvigenciadtfim FROM sisfor.sisfor_s WHERE sifid='".$dados['sifid']."' AND alpid='".$dados['alpid']."') 
    				WHERE sifid='".$dados['sifid']."'";
			$db->executar($sql);
		}
		
		ob_end_clean();
		
		$sql = "UPDATE sisfor.alterarprojeto SET alphistorico='".addslashes($alphistorico)."',  usucpfautorizou='".$_SESSION['usucpf']."', alpdtautorizou=NOW() WHERE alpid='".$dados['alpid']."'";
		$db->executar($sql);


	}
	
	if($dados['alpautorizado']=='3') {
		ob_start();
		if($dados['tipo']=='orcamento') {
			carregarListaCustos(array('sifid'=>$dados['sifid'],'consulta'=>true));
			$alphistorico = ob_get_contents();
		}
		if($dados['tipo']=='bolsas') {
			carregarQuantitativoPorPerfil(array('sifid'=>$dados['sifid'],'consulta'=>true));
			$alphistorico = ob_get_contents();
		}
		if($dados['tipo']=='meta') {
			carregarMeta(array('sifid'=>$dados['sifid'],'consulta'=>true));
			$alphistorico = ob_get_contents();
		}
		
		ob_end_clean();
	
		$sql = "UPDATE sisfor.alterarprojeto SET alphistorico='".addslashes($alphistorico)."',  usucpfautorizou='".$_SESSION['usucpf']."', alpdtautorizou=NOW() WHERE alpid='".$dados['alpid']."'";
		$db->executar($sql);
	
	}
	
	
	$db->commit();
	
}

function carregarHistoricoSolicitacao($dados) {
	global $db;
	
	$alphistorico = $db->pegaUm("SELECT alphistorico FROM sisfor.alterarprojeto WHERE alpid='".$dados['alpid']."'");
	
	if($alphistorico) echo $alphistorico;
	else {
		if($dados['tipo']=='orcamento') {
			carregarListaCustos(array('sifid'=>$dados['sifid'],'consulta'=>true));
		}
		if($dados['tipo']=='bolsas') {
			carregarQuantitativoPorPerfil(array('sifid'=>$dados['sifid'],'consulta'=>true));
		}
		if($dados['tipo']=='meta') {
			carregarMeta(array('sifid'=>$dados['sifid'],'consulta'=>true));
		}
		if($dados['tipo']=='vigencia') {
			carregarVigencia(array('sifid'=>$dados['sifid'],'consulta'=>true));
		}
	}

}

function verificarCriacaoPagamento($dados) {
	global $db;
	
	$tipoperfil = $db->pegaLinha("SELECT tpeqtdbolsa, sifid, pflcod FROM sisfor.tipoperfil WHERE tpeid='".$dados['tpeid']."'");
	
	$qtdbolsaspagas_vaga = $db->pegaUm("SELECT count(*) as qtdbolsas FROM sisfor.pagamentobolsista p INNER JOIN workflow.documento d ON d.docid = p.docid WHERE tpeid='".$dados['tpeid']."' AND d.esdid!='".ESD_PAGAMENTO_NAO_AUTORIZADO."'");
	
	if($tipoperfil['tpeqtdbolsa'] <= $qtdbolsaspagas_vaga) {
		$erro[] = "- Quantidade total de bolsas a serem pagas esta menor do que a quantidade de bolsas enviadas para pagamento ( {$tipoperfil['tpeqtdbolsa']} total de bolsas / {$qtdbolsaspagas_vaga} enviadas para pagamento )";
	}
	
	$qtdmaxima_curso = $db->pegaUm("SELECT epiqtd FROM sisfor.equipeies WHERE sifid='".$tipoperfil['sifid']."' AND pflcod='".$tipoperfil['pflcod']."'");
	
	$qtdbolsaspagas_curso = $db->pegaUm("SELECT count(*) as qtdbolsas FROM sisfor.pagamentobolsista p INNER JOIN workflow.documento d ON d.docid = p.docid INNER JOIN sisfor.tipoperfil tt ON tt.tpeid = p.tpeid WHERE tt.sifid='".$tipoperfil['sifid']."' AND p.pflcod='".$tipoperfil['pflcod']."' AND d.esdid!='".ESD_PAGAMENTO_NAO_AUTORIZADO."'");
	
	if($qtdmaxima_curso <= $qtdbolsaspagas_curso) {
		$erro[] = "- Quantidade total de bolsas a serem pagas no curso para este perfil esta menor do que a quantidade de bolsas enviadas para pagamento ( {$qtdmaxima_curso} total de bolsas do perfil / {$qtdbolsaspagas_curso} enviadas para pagamento )";
	}

	$existe_pagamento = $db->pegaUm("SELECT i.iusnome||' : Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia as existe FROM sisfor.pagamentobolsista p 
									 INNER JOIN sisfor.identificacaousuario i ON i.iusd = p.iusd 
									 INNER JOIN sisfor.folhapagamento pa ON pa.fpbid = p.fpbid 
									 INNER JOIN public.meses m ON m.mescod::integer = pa.fpbmesreferencia
									 WHERE p.tpeid='".$dados['tpeid']."' AND p.fpbid='".$dados['fpbid']."'");
	
	if($existe_pagamento) {
		$erro[] = "- Pagamento ja foi gerado para período de referência ( {$existe_pagamento} )";
	}
	
	return $erro;
}

function verificarAtualizacaoPagamento($dados) {
	global $db;

	$tipoperfil = $db->pegaLinha("SELECT tpeqtdbolsa, sifid, pflcod FROM sisfor.tipoperfil WHERE tpeid='".$dados['tpeid']."'");

	$qtdbolsaspagas_vaga = $db->pegaUm("SELECT count(*) as qtdbolsas FROM sisfor.pagamentobolsista WHERE tpeid='".$dados['tpeid']."'");

	if($qtdbolsaspagas_vaga) {
		if($tipoperfil['tpeqtdbolsa'] < $qtdbolsaspagas_vaga) {
			$erro[] = "- Quantidade total de bolsas a serem pagas esta menor do que a quantidade de bolsas enviadas para pagamento ( {$tipoperfil['tpeqtdbolsa']} total de bolsas / {$qtdbolsaspagas_vaga} enviadas para pagamento )";
		}
	}

	$qtdmaxima_curso = $db->pegaUm("SELECT epiqtd FROM sisfor.equipeies WHERE sifid='".$tipoperfil['sifid']."' AND pflcod='".$tipoperfil['pflcod']."'");

	$qtdbolsaspagas_curso = $db->pegaUm("SELECT count(*) as qtdbolsas FROM sisfor.pagamentobolsista p INNER JOIN sisfor.tipoperfil tt ON tt.tpeid = p.tpeid WHERE tt.sifid='".$tipoperfil['sifid']."' AND p.pflcod='".$tipoperfil['pflcod']."'");

	if($qtdbolsaspagas_curso) {
		if($qtdmaxima_curso < $qtdbolsaspagas_curso) {
			$erro[] = "- Quantidade total de bolsas a serem pagas no curso para este perfil esta menor do que a quantidade de bolsas enviadas para pagamento ( {$qtdmaxima_curso} total de bolsas do perfil / {$qtdbolsaspagas_curso} enviadas para pagamento )";
		}
	}

	return $erro;
}

function aprovarTrocaNomesSGB($dados) {
	global $db;

	if($dados['cpf']) {
		foreach($dados['cpf'] as $cpf) {
			if($dados['nome_receita'][$cpf]) {
				$sql = "UPDATE sisfor.identificacaousuario SET iusnome='".$dados['nome_receita'][$cpf]."' WHERE iuscpf='".str_replace(array(".","-"),array("",""),$cpf)."'";
				$db->executar($sql);
			}
		}
		$db->commit();
	}

	$al = array("alert"=>"Trocas realizadas com sucesso","location"=>"sisfor.php?modulo=".$_REQUEST['modulo']."&acao=A&aba=aprovarnomes");
	alertlocation($al);

}

function salvarRelatorioMensal($dados) {
	global $db;
	
	if($dados['atividadescomfordsc']) {
		foreach($dados['atividadescomfordsc'] as $rmrid => $rmrdescricao) {
			$sql = "UPDATE sisfor.relatoriomensalatvresposta SET rmrdescricao='{$rmrdescricao}' WHERE rmrid='{$rmrid}'";
			$db->executar($sql);
		}
	}
	
	if($dados['atividadescomfordt']) {
		foreach($dados['atividadescomfordt'] as $rmrid => $rmrdata) {
			$sql = "UPDATE sisfor.relatoriomensalatvresposta SET rmrdata=".(($rmrdata)?"'".formata_data_sql($rmrdata)."'":"NULL")." WHERE rmrid='{$rmrid}'";
			$db->executar($sql);
		}
	}
	
	$db->commit();
	

	$sql = "UPDATE sisfor.relatoriomensal SET remobssituacoescursos='".substr($dados['remobssituacoescursos'],0,2000)."',
    										  remobsexecucaofinanceira='".substr($dados['remobsexecucaofinanceira'],0,2000)."',
    										  remoutroscomentario='".substr($dados['remoutroscomentario'],0,2000)."',
    										  rematividades=".(($rematividades)?"'".implode("¨¨",$rematividades)."'":"NULL").",
											  remcargo=".(($dados['remcargo'])?"'".$dados['remcargo']."'":"NULL").",
											  remunidadelotacao=".(($dados['remunidadelotacao'])?"'".$dados['remunidadelotacao']."'":"NULL").",
											  remlinkcurriculolattes=".(($dados['remlinkcurriculolattes'])?"'".$dados['remlinkcurriculolattes']."'":"NULL")."
    		WHERE remid='".$dados['remid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Relatório Mensal gravado com sucesso","location"=>"sisfor.php?modulo=".$dados['modulo']."&acao=A&aba=relatoriomensal&fpbid=".$dados['fpbid']);
	alertlocation($al);
	
}

function carregarRelatorioMensalParticipante($dados) {
	global $db;
	
	if($dados['repid']) {
		$sql = "DELETE FROM sisfor.relatoriomensalparticipante WHERE repid='".$dados['repid']."'";
		$db->executar($sql);
		$db->commit();
	}
	
	if($dados['repcpf'] && $dados['repnome']) {

		$sql = "INSERT INTO sisfor.relatoriomensalparticipante(
			            rmrid, repcpf, repnome, repcargo)
			    VALUES ('".$dados['rmrid']."', '".str_replace(array(".","-"),array("",""),$dados['repcpf'])."', '".$dados['repnome']."','".$dados['repcargo']."');";
		
		$db->executar($sql);
		$db->commit();
		
	}
	
	$sql = "SELECT ".(($dados['consulta'])?"''":"'<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirParticipantes('||repid||');\">'")." as acao, repcpf, repnome, repcargo FROM sisfor.relatoriomensalparticipante WHERE rmrid='".$dados['rmrid']."'";
	$cabecalho = array("&nbsp;","CPF","Nome","Cargo");
	$db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%');

}

function carregarRelatorioMensalAtividades($dados) {
	global $db;
	
	if($dados['remid'] && $dados['rmaid']) {

		$sql = "INSERT INTO sisfor.relatoriomensalatvresposta(
				remid, rmaid)
				VALUES ('".$dados['remid']."', '".$dados['rmaid']."');";
		
		$db->executar($sql);
		$db->commit();
	}
	
	if($dados['rmrid']) {
	
		$sql = "DELETE FROM sisfor.relatoriomensalatvresposta WHERE rmrid='".$dados['rmrid']."'";
		
		$db->executar($sql);
		$db->commit();
	}
	
	if($dados['remid']) {
		$sql = "SELECT ".(($dados['consulta'])?"''":"'<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\">'")."||CASE WHEN a.rmaparticipantes=true THEN '<input type=hidden id=\"participantes_'||r.rmrid||'\" onclick=\"carregarParticipantes(this,'||r.rmrid||');\">' ELSE '' END as acao, 
	    				a.rmadsc, 
	    			   ".(($dados['consulta'])?"coalesce(rmrdescricao,'')":"'<center><textarea id=\"atividadescomfordsc'||r.rmrid||'\" name=\"atividadescomfordsc['||r.rmrid||']\" cols=\"50\" rows=\"3\" onmouseover=\"MouseOver( this );\" onfocus=\"MouseClick( this );\" onmouseout=\"MouseOut( this );\" onblur=\"MouseBlur( this );\" style=\"width:50ex;\" class=\"txareanormal\">'||coalesce(rmrdescricao,'')||'</textarea></center>'")." as descricao,
	    			   ".(($dados['consulta'])?"coalesce(to_char(rmrdata,'dd/mm/YYYY'),'')":"'<center style=\"white-space: nowrap;\"><input title=\"Data atividade\" type=\"text\" id=\"atividadescomfordt'||r.rmrid||'\" name=\"atividadescomfordt['||r.rmrid||']\" value=\"'||coalesce(to_char(rmrdata,'dd/mm/YYYY'),'')||'\" size=\"12\" style=\"text-align: right;\" maxlength=\"10\" class=\"normal \" onkeyup=\"this.value=mascaraglobal(\'##/##/####\',this.value);\" onchange=\"\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"validando_data(this);MouseBlur(this);this.value=mascaraglobal(\'##/##/####\',this.value)\"> <img src=\"../includes/JsLibrary/date/displaycalendar/images/calendario.gif\" align=\"absmiddle\" border=\"0\" style=\"cursor:pointer\" title=\"Escolha uma Data\" onclick=\"displayCalendar(document.getElementById(\'atividadescomfordt'||r.rmrid||'\'),\'dd/mm/yyyy\',this)\"></center>'")." as data 
	    		FROM sisfor.relatoriomensalatvresposta r 
	    		INNER JOIN sisfor.relatoriomensalatividades a ON a.rmaid = r.rmaid
	    		WHERE r.remid='".$dados['remid']."' 
	    		ORDER BY r.rmrid";
	} else {
		$sql = array();
	}
	
	$cabecalho = array("&nbsp;","Atividades","Descrição","Data da atividade");
	$db->monta_lista_simples($sql, $cabecalho, 50, 5, 'N', '100%');
	
	
	
}

function enviarProjetoMensalAnalise($remid) {
	global $db, $servidor_bd, $servidor_bd_siafi, $porta_bd, $porta_bd_siafi, $nome_bd, $nome_bd_siafi, $usuario_db, $usuario_db_siafi, $senha_bd, $senha_bd_siafi;
	
	$_REQUEST['fpbid'] = $db->pegaUm("SELECT fpbid FROM sisfor.relatoriomensal WHERE remid='{$remid}'");
	$versaoimpressao = true;
	$wf_func = true;
	
	
	ob_start();
	include_once APPRAIZ_SISFOR.'mec/relatoriomensal.inc';
	$rel = ob_get_contents();
	ob_end_clean();
	
	$sql = "UPDATE sisfor.relatoriomensal SET remversaoanalise='".addslashes($rel)."' WHERE remid='{$remid}'";
	$db->executar($sql);
	$db->commit();
	
	

	return true;
}

function carregarBlocoParticipantes($dados) {
	global $db;
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	if(!$dados['consulta']) {
	
		echo '<tr>';
		echo '<td class="SubTituloDireita" width="10%">CPF</td>';
		echo '<td>';
		echo campo_texto('iuscpf__'.$dados['rmrid'], "N", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf__'.$dados['rmrid'].'"', '', '', 'if(this.value!=\'\'){carregaUsuario_(\'__'.$dados['rmrid'].'\');}');
		echo '</td>';
		echo '<td class="SubTituloDireita" width="10%">Nome</td>';
		echo '<td>';
		echo campo_texto('iusnome__'.$dados['rmrid'], "S", "N", "Nome", 50, 150, "", "", '', '', 0, 'id="iusnome__'.$dados['rmrid'].'"', '');
		echo '</td>';
		echo '<td class="SubTituloDireita" width="10%">Cargo</td>';
		echo '<td>';
		echo campo_texto('iuscargo__'.$dados['rmrid'], "S", "S", "Cargo", 30, 150, "", "", '', '', 0, 'id="iuscargo__'.$dados['rmrid'].'"', '');
		echo '</td>';
		
		echo '</tr>';
		echo '<tr>';
		echo '<td colspan="6" class="SubTituloCentro"><input type="button" name="adicionar" value="Adicionar participante" onclick="adicionarParticipante('.$dados['rmrid'].');"></td>';
		echo '</tr>';
	
	}
	echo '<tr>';
	echo '<td colspan="6" id="td_listaparticipantes_'.$dados['rmrid'].'">';
	carregarRelatorioMensalParticipante(array('rmrid'=>$dados['rmrid'], 'consulta'=> $dados['consulta']));
	echo '</td>';
	echo '</tr>';
	echo '</table>';

}

function corrigirWorkflowAvaliacao($dados) {
	global $db;
	
	$sql = "select docdsc, count(*) from workflow.documento where tpdid=191 group by docdsc having count(*)>1";
	$arr = $db->carregar($sql);
	
	$cc=0;
	if($arr[0]) {
		foreach($arr as $a) {
			$xx = explode("Folha de Pagamento do projeto ",$a['docdsc']);
			$vl = explode(" e folha ",$xx[1]);
			
			$sql = "select e.esdid from sisfor.folhapagamentoprojeto f 
					inner join workflow.documento d on d.docid = f.docid 
					inner join workflow.estadodocumento e on e.esdid = d.esdid
					where sifid=".$vl[0]." and fpbid=".$vl[1];
			
			$esdid = $db->pegaUm($sql);
			
			if($esdid!=1207) {

				$sql = "select * from workflow.documento where tpdid=191 and docdsc='".$a['docdsc']."' order by docid";
				$docs = $db->carregar($sql);
				
				$docat = false;
				if($docs[0]) {
					foreach($docs as $d) {
						if($d['esdid']==1207) $docat = $d['docid'];
					}
				}
				
				if($docat) {
					$db->executar("update sisfor.folhapagamentoprojeto set docid=".$docat." where sifid=".$vl[0]." and fpbid=".$vl[1]);
					$db->commit();
					$cc++;
					echo "atualizacoes:".$cc."<br>";
					echo "update sisfor.folhapagamentoprojeto set docid=".$docat." where sifid=".$vl[0]." and fpbid=".$vl[1]."<br>";
				}

			}
			

		}
	}
}


function carregarEncontroPresencial($dados) {
	global $db;
	$sql = "SELECT '".(($dados['consulta'])?"":"<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"removerEncontroPresencial('||aofid||');\">")."' as acao, aofnome, to_char(aofdata,'dd/mm/YYYY') as aofdata, aofcargahoraria FROM sisfor.avaliacaofinalencontropresencial WHERE sifid='".$_SESSION['sisfor']['sifid']."'";
	$cabecalho = array("&nbsp;","Nome do evento / Atividade","Data do evento / Atividade","Carga horária");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}

function removerEncontroPresencialCoordenador($dados) {
	global $db;
	
	$sql = "DELETE FROM sisfor.avaliacaofinalencontropresencial WHERE aofid='".$dados['aofid']."'";
	$db->executar($sql);
	$db->commit();
	
}

function gravarEncontroPresencial($dados) {
	global $db;

	$sql = "INSERT INTO sisfor.avaliacaofinalencontropresencial(
            sifid, aofnome, aofdata, aofcargahoraria)
    		VALUES ('".$_SESSION['sisfor']['sifid']."', '".utf8_decode($dados['aofnome'])."', '".formata_data_sql($dados['aofdata'])."', '".$dados['aofcargahoraria']."');";

	$db->executar($sql);
	$db->commit();
}



function carregarRegionais($dados) {
	global $db;

	$sql = "SELECT a.afrid, a.afrnome, m.estuf||'/'||m.mundescricao as municipiosede, a.afrnumformadoresreg FROM sismedio.avaliacaofinalcgregional a
	        INNER JOIN territorios.municipio m ON m.muncod = a.afrmuncodsede
	        WHERE iusd='".$_SESSION['sismedio']['universidade']['iusd']."'";

	//$regionais = $db->carregar($sql);

	if($regionais[0]) {
		foreach($regionais as $key => $reg) {
			$arr[$key]['acao']          = "<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"removerRegional('".$reg['afrid']."');\">";
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

function salvarExecucao2014($dados) {
	global $db;
	
	if($dados['sifvalorempenhado']) {
		foreach($dados['sifvalorempenhado'] as $sifid => $sifvalorempenhado) {
			$sql = "UPDATE sisfor.sisfor SET sifvalorempenhado=".(($sifvalorempenhado)?"'".str_replace(array(".",","),array("","."),$sifvalorempenhado)."'":"NULL")." WHERE sifid='".$sifid."'";
			$db->executar($sql);
		}
	}
	
	$db->commit();
	
	$al = array("alert"=>"Execução 2014 gravada com sucesso","location"=>"sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=execucao2014");
	alertlocation($al);

}

function confirmarFinalizacaoExecucao2014($dados) {
	global $db;
	
	$sql = "UPDATE sisfor.execucao2014 SET planejamentofinalizado2014=true, justificativaempenho2014=".(($dados['justificativaempenho2014'])?"'".$dados['justificativaempenho2014']."'":"NULL")." WHERE unicod='".$_SESSION['sisfor']['unicod']."'";
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Execução 2014 finalizada com sucesso","location"=>"sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=execucao2014");
	alertlocation($al);
	
}

function retornarExecucao2014($dados) {
	global $db;

	$sql = "UPDATE sisfor.execucao2014 SET planejamentofinalizado2014=false WHERE unicod='".$_SESSION['sisfor']['unicod']."'";
	$db->executar($sql);
	$db->commit();

	$al = array("alert"=>"Execução 2014 retornada com sucesso","location"=>"sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=execucao2014");
	alertlocation($al);

}

function gravarAvaliacaoFinalCurso($dados) {
	global $db;
	
	if($dados['atidatainicioexec']) {
		foreach($dados['atidatainicioexec'] as $atiid => $atidatainicioexec) {
			$sql_plano[] = "UPDATE sisfor.atividadescurso SET atidatainicioexec=".(($atidatainicioexec)?"'".$atidatainicioexec."'":"NULL")." WHERE atiid='".$atiid."';";
		}
	}
	
	if($dados['atidatafimexec']) {
		foreach($dados['atidatafimexec'] as $atiid => $atidatafimexec) {
			$sql_plano[] = "UPDATE sisfor.atividadescurso SET atidatafimexec=".(($atidatafimexec)?"'".$atidatafimexec."'":"NULL")." WHERE atiid='".$atiid."';";
		}
	}
	
	if($sql_plano) {
		$db->executar(implode("",$sql_plano));
		$db->commit();
	}
	
	if($dados['orcvalorexecutado']) {
		foreach($dados['orcvalorexecutado'] as $orcid => $orcvalorexecutado) {
			$sql_orcamento[] = "UPDATE sisfor.orcamento SET orcvlrexecutado=".(($orcvalorexecutado)?"'".str_replace(array(".",","),array("","."),$orcvalorexecutado)."'":"NULL")." WHERE orcid='".$orcid."';";
		}
	}
	
	if($sql_orcamento) {
		$db->executar(implode("",$sql_orcamento));
		$db->commit();
	}
	
	$sql_abrangencia[] = "UPDATE sisfor.abrangenciacurso SET abrconfirmaatendimento=false WHERE sifid='".$_SESSION['sisfor']['sifid']."';";
	
	if($dados['abrmuncod']) {
		foreach($dados['abrmuncod'] as $muncod) {
			$sql_abrangencia[] = "UPDATE sisfor.abrangenciacurso SET abrconfirmaatendimento=true WHERE sifid='".$_SESSION['sisfor']['sifid']."' AND muncod='".$muncod."';";
		}
	}
	
	if($sql_abrangencia) {
		$db->executar(implode("",$sql_abrangencia));
		$db->commit();
	}
	
	$afcid = $db->pegaUm("SELECT afcid FROM sisfor.avaliacaofinal WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
	
	if(!$afcid) {
		$docid = wf_cadastrarDocumento( WF_TPDID_AVALIACAOFINAL, 'ID : '.$_SESSION['sisfor']['sifid'] );
		$afcid = $db->pegaUm("INSERT INTO sisfor.avaliacaofinal(sifid, docid)
							  VALUES ('".$_SESSION['sisfor']['sifid']."', '".$docid."') RETURNING afcid;");
	}
	
	$sql = "UPDATE sisfor.avaliacaofinal
   			SET afcexecucacaoprojetoini=".(($dados['afcexecucacaoprojetoini'])?"'".$dados['afcexecucacaoprojetoini']."'":"NULL").", 
	            afcexecucacaoprojetofim=".(($dados['afcexecucacaoprojetofim'])?"'".$dados['afcexecucacaoprojetofim']."'":"NULL").", 
       			afcjustificativamudancaplano=".(($dados['afcjustificativamudancaplano'])?"'".$dados['afcjustificativamudancaplano']."'":"NULL").", 
	    		afcestruturafisicasuporte=".(($dados['afcestruturafisicasuporte'])?"'".implode(";",$dados['afcestruturafisicasuporte'])."'":"NULL").", 
       			afcarticulacaoinstitucional=".(($dados['afcarticulacaoinstitucional'])?"'".$dados['afcarticulacaoinstitucional']."'":"NULL").", 
	    		afcarticulacaocomfor=".(($dados['afcarticulacaocomfor'])?"'".$dados['afcarticulacaocomfor']."'":"NULL").", 
	    		afcsobreconteudocurso=".(($dados['afcsobreconteudocurso'])?"'".$dados['afcsobreconteudocurso']."'":"NULL").", 
       			afcsobremetodologia=".(($dados['afcsobremetodologia'])?"'".$dados['afcsobremetodologia']."'":"NULL").", 
				afccriteriosavaliacao=".(($dados['afccriteriosavaliacao'])?"'".$dados['afccriteriosavaliacao']."'":"NULL").", 
				afcarticulacaomec=".(($dados['afcarticulacaomec'])?"'".$dados['afcarticulacaomec']."'":"NULL").", 
       			afclicoesaprendidas=".(($dados['afclicoesaprendidas'])?"'".$dados['afclicoesaprendidas']."'":"NULL").", 
				afcoutroscomentarios=".(($dados['afcoutroscomentarios'])?"'".$dados['afcoutroscomentarios']."'":"NULL").", 
				afcplanejamentopedagogico=".(($dados['afcplanejamentopedagogico'])?"'".$dados['afcplanejamentopedagogico']."'":"NULL").", 
       			afcorganizacaopedagogica=".(($dados['afcorganizacaopedagogica'])?"'".$dados['afcorganizacaopedagogica']."'":"NULL")."
 			WHERE afcid='{$afcid}'";
	
	$db->executar($sql);
	$db->commit();
	
	$al = array("alert"=>"Avaliação Final gravada com sucesso","location"=>"sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=avaliacaofinal");
	alertlocation($al);

}

function selecionarBolsaPagamento($dados) {
	global $db;
	
	$sql = "(
	
									SELECT '<input type=radio name=tpebolsa[] value=\"'||t.tpeid||'\" '||CASE WHEN t.tpebolsa=TRUE THEN 'checked' ELSE '' END||'>' as radio, p.pfldsc,
											uniabrev||' - '||unidsc as universidade,
											CASE WHEN s.ieoid is not null then cur.curid  ||' - '|| cur.curdesc
											     WHEN s.cnvid is not null then cur2.curid ||' - '|| cur2.curdesc
											     WHEN s.ocuid is not null then oc.ocunome end as curso
	
									FROM sisfor.tipoperfil t
									INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod
									INNER JOIN sisfor.sisfor s ON s.sifid = t.sifid
									INNER JOIN workflow.documento d ON d.docid = s.docidprojeto
									INNER JOIN public.unidade u on u.unicod = s.unicod
									LEFT JOIN catalogocurso2014.iesofertante ieo ON ieo.ieoid = s.ieoid
									LEFT JOIN catalogocurso2014.curso cur on cur.curid = ieo.curid
									LEFT JOIN sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
									LEFT JOIN catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
									LEFT JOIN sisfor.outrocurso oc on oc.ocuid = s.ocuid
									WHERE t.iusd='".$dados['iusd']."' AND d.esdid='".ESD_PROJETO_VALIDADO."'
	
									) UNION ALL (
            		
									SELECT '<input type=radio name=tpebolsa[] value=\"'||t.tpeid||'\" '||CASE WHEN t.tpebolsa=TRUE THEN 'checked' ELSE '' END||'>' as radio, p.pfldsc,
											uniabrev||' - '||unidsc as universidade, '<center>-</center>' as curso
											
	
									FROM sisfor.tipoperfil t
									INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
									INNER JOIN sisfor.identificacaousuario i ON i.iusd = t.iusd 
									INNER JOIN sisfor.sisfories s ON s.usucpf = i.iuscpf
									INNER JOIN public.unidade u on u.unicod = s.unicod
									WHERE t.iusd='".$dados['iusd']."' AND t.pflcod='".PFL_COORDENADOR_INST."'
	
	)";
		
	$cabecalho = array("&nbsp;","Perfil","Universidade","Curso");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%','',true, false, false, false);
	
	
}

function inserirMunicipioAbrangencia($dados) {
	global $db;
	
	$sql = "INSERT INTO sisfor.abrangenciacurso(
            muncod, sifid, abrstatus, abrconfirmaatendimento)
    		VALUES ('".$dados['muncod']."', '".$dados['sifid']."', 'A', TRUE);";
	
	$db->executar($sql);
	$db->commit();
	
}

function gravarInformacoesCursistas($dados) {
	global $db;
	if(is_Date($dados['curdatanascimento'])) $dados['curdatanascimento'] = formata_data_sql($dados['curdatanascimento']);
	else $dados['curdatanascimento'] = null;
	
	$sql = "UPDATE sisfor.cursista
			   SET curescolaridade=".(($dados['curescolaridade'])?"'".$dados['curescolaridade']."'":"NULL").", 
			       currede=".(($dados['currede'])?"'".$dados['currede']."'":"NULL").", 
				   curcontratacao=".(($dados['curcontratacao'])?"'".$dados['curcontratacao']."'":"NULL").", 
				   cursexo=".(($dados['cursexo'])?"'".$dados['cursexo']."'":"NULL").", 
				   curdatanascimento=".(($dados['curdatanascimento'])?"'".$dados['curdatanascimento']."'":"NULL").", 
			       curraca=".((is_numeric($dados['curraca']))?"'".$dados['curraca']."'":"NULL").",
				   curdeficiencia=".(($dados['curdeficiencia'])?"'".$dados['curdeficiencia']."'":"NULL").",
				   curinep=".(($dados['curinep'])?"'".$dados['curinep']."'":"NULL").",
				   curfuncao=".(($dados['curfuncao'])?"'".$dados['curfuncao']."'":"NULL")." 
			 WHERE curid='".$dados['curid']."'";
	
	$db->executar($sql);
	
	
	if(is_numeric($_SESSION['sisfor']['sifid'])) {

		$sql = "UPDATE sisfor.cursistacurso SET estuf=".(($dados['estuf_endereco'])?"'".$dados['estuf_endereco']."'":"NULL").", muncod=".(($dados['muncod_endereco'])?"'".$dados['muncod_endereco']."'":"NULL")." WHERE sifid='".$_SESSION['sisfor']['sifid']."' AND curid='".$dados['curid']."'";
		$db->executar($sql);
		
	}
	
	$db->commit();
	
	if(!$dados['noredirect']) {
		$al = array("alert"=>"Cursista gravado com sucesso","location"=>"sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=cadastrar_cursista&fpbid=".$dados['fpbid']);
		alertlocation($al);
	}

}

function exibirInformacoesCursistas($dados) {
	global $db;
	
	echo '<script>';
	echo 'function gravarInformacoesCursistas(){if(document.getElementById("curdatanascimento").value!=\'\') {if(!validaData(document.getElementById("curdatanascimento"))) {alert("Data de Nascimento inválida - Formato dd/mm/YYYY");return false;}}document.getElementById("formularioInformacoesCursistas").submit();}';
	echo '</script>';

	echo '<form method="post" name="formulario" id="formularioInformacoesCursistas" enctype="multipart/form-data">';
	echo '<input type="hidden" name="requisicao" value="gravarInformacoesCursistas">';
	echo '<input type="hidden" name="curid" value="'.$dados['curid'].'">';
	echo '<input type="hidden" name="noredirect" value="'.$dados['noredirect'].'">';
	
	$cursista = $db->pegaLinha("SELECT * FROM sisfor.cursista WHERE curid='".$dados['curid']."'");
	
	$terr = $db->pegaLinha("SELECT DISTINCT estuf, muncod FROM sisfor.cursistacurso WHERE curid='".$dados['curid']."' AND sifid='".$_SESSION['sisfor']['sifid']."' ".(($dados['fpbid'])?"AND fpbid='".$dados['fpbid']."'":"")."");
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="25%">Orientações</td>';
	echo '<td>Preencher os dados complementares dos cursistas. O objetivo é traçar perfil dos participantes.</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="25%">CPF</td>';
	echo '<td>'.(($cursista['curcpf'])?mascaraglobal($cursista['curcpf'],'###.###.###-##'):'-').'</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="25%">Nome</td>';
	echo '<td>'.$cursista['curnome'].'</td>';
	echo '</tr>';
	echo '<tr>';
	echo '<td class="SubTituloDireita" width="25%">Escolaridade</td>';
	echo '<td>';
	$sql = "SELECT foeid as codigo, foedesc as descricao FROM sisfor.formacaoescolaridade ORDER BY foecod";
	$db->monta_combo('curescolaridade', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'curescolaridade', '', $cursista['curescolaridade']);
	echo '</td>';
	echo '</tr>';
    echo '<tr>';
    echo '<td class="SubTituloDireita" width="25%">Rede</td>';
    echo '<td><input type="radio" name="currede" value="M" '.(($cursista['currede']=='M')?'checked':'').'> Municipal <input type="radio" name="currede" value="E" '.(($cursista['currede']=='E')?'checked':'').'> Estadual <input type="radio" name="currede" value="F" '.(($cursista['currede']=='F')?'checked':'').'> Federal <input type="radio" name="currede" value="A" '.(($cursista['currede']=='A')?'checked':'').'> Não se aplica</td>';
    echo '</tr>';
    echo '<tr>';
    echo '<td class="SubTituloDireita" width="25%">Vínculo com a rede de ensino</td>';
    echo '<td>';
    
    $sql = "SELECT pk_cod_tipo_contratacao as codigo, no_contratacao as descricao FROM sisfor.tab_tipo_contratacao";
    
    $db->monta_combo('curcontratacao', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'curcontratacao', '', $cursista['curcontratacao']);
    
	echo '</td>';
    echo '</tr>';
    echo '<tr>';
    echo '<td class="SubTituloDireita" width="25%">UF onde atua</td>';
    echo '<td>';
    
    $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
    $db->monta_combo('estuf_endereco', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '', 'S', 'estuf_endereco', '', $terr['estuf']);
    
    echo '</td>';
    echo '</tr>';
    echo '<tr>';
    echo '<td class="SubTituloDireita" width="25%">Município onde atua</td>';
    echo '<td id="td_municipio3">';
    
    if($terr['muncod']) {
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$terr['estuf']."'";
		$db->monta_combo('muncod_endereco', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'muncod_endereco', '', $terr['muncod']);
	}
    
    echo '</td>';
    echo '</tr>';
    echo '<tr>';
    echo '<td class="SubTituloDireita" width="25%">Sexo</td>';
    echo '<td><input type="radio" name="cursexo" value="M" '.(($cursista['cursexo']=='M')?'checked':'').'> Masculino <input type="radio" name="cursexo" value="F" '.(($cursista['cursexo']=='F')?'checked':'').'> Feminino</td>';
    echo '</tr>';
    echo '<tr>';
    echo '<td class="SubTituloDireita" width="25%">Data de Nascimento</td>';
    echo '<td>';
    echo campo_texto( 'curdatanascimento', 'S', 'S', '', 11, 10, '##/##/####', '','','','','id="curdatanascimento"', '', formata_data($cursista['curdatanascimento']));
    echo '</td>';
    echo '</tr>';
    echo '<tr>';
    echo '<td class="SubTituloDireita" width="25%">Raça/ Etnia</td>';
    echo '<td>';
    $arrRaca = array(0 => array('codigo' => '0','descricao' => 'Não declarada'),
    				  1 => array('codigo' => '1','descricao' => 'Branca'),
					  2 => array('codigo' => '2','descricao' => 'Preta'),
					  3 => array('codigo' => '3','descricao' => 'Parda'),
					  4 => array('codigo' => '4','descricao' => 'Amarela'),
					  5 => array('codigo' => '5','descricao' => 'Indígena'),
					 );


    $db->monta_combo('curraca', $arrRaca, 'S', 'Selecione', '', '', '', '', 'S', 'curraca', '', ((is_numeric($cursista['curraca']))?'0'.$cursista['curraca']:""));
    echo '</td>';
    echo '</tr>';
    
    echo '<tr>';
    echo '<td class="SubTituloDireita" width="25%">Possui deficiência?</td>';
    echo '<td>';
    $arrDeficiencia = array(0 => array('codigo' => '1','descricao' => 'Cegueira'),
    						1 => array('codigo' => '2','descricao' => 'Surdocegueira'),
    						2 => array('codigo' => '3','descricao' => 'Baixa visão'),
    						3 => array('codigo' => '4','descricao' => 'Deficiência física'),
    						4 => array('codigo' => '5','descricao' => 'Surdez'),
    						5 => array('codigo' => '6','descricao' => 'Deficiência intelectual'),
							6 => array('codigo' => '7','descricao' => 'Deficiência auditiva'),
							7 => array('codigo' => '8','descricao' => 'Deficiência multipla'),
							8 => array('codigo' => '9','descricao' => 'Não declarado'),
							9 => array('codigo' => '10','descricao' => 'Não possui deficiência')
    );
    
    $db->monta_combo('curdeficiencia', $arrDeficiencia, 'S', 'Selecione', '', '', '', '', 'S', 'curdeficiencia', '', $cursista['curdeficiencia']);
    echo '</td>';
    echo '</tr>';
    
    
    echo '<tr>';
    echo '<td class="SubTituloDireita" width="25%">Escola onde atua</td>';
    echo '<td>';
    
    if($terr['muncod']) {
	    $sql = "SELECT pk_cod_entidade as codigo, pk_cod_entidade||' - '||no_escola as descricao FROM educacenso_2014.tb_escola_inep_2014 WHERE co_municipio='".$terr['muncod']."'";
		$db->monta_combo('curinep', $sql, 'S', 'Selecione', '', '', '', '400', 'N', 'curinep', '', $cursista['curinep']);
	} else {
		echo 'É necessário definir o município onde atua';
	}
    
    echo '</td>';
    echo '</tr>';
    

    echo '<tr>';
    echo '<td class="SubTituloDireita" width="25%">Função que exerce</td>';
    echo '<td>';
    $arrFuncao = array(0 => array('codigo' => '1','descricao' => 'Auxiliar de Educação Infantil'),
    		1 => array('codigo' => '2','descricao' => 'Conselheiro Escolar'),
    		2 => array('codigo' => '3','descricao' => 'Conselheiro Municipal de Educação'),
    		3 => array('codigo' => '4','descricao' => 'Coordenador Pedagógico'),
    		4 => array('codigo' => '5','descricao' => 'Diretor'),
    		5 => array('codigo' => '6','descricao' => 'Dirigente de Educação'),
    		6 => array('codigo' => '7','descricao' => 'Docente'),
    		7 => array('codigo' => '8','descricao' => 'Estudante'),
			8 => array('codigo' => '9','descricao' => 'Formador de NTE'),
			9 => array('codigo' => '10','descricao' => 'Intérprete de Libras'),
			10 => array('codigo' => '11','descricao' => 'Monitor de Atividade Complementar'),
			11 => array('codigo' => '12','descricao' => 'Profissional de Assistência'),
			12 => array('codigo' => '13','descricao' => 'Profissional de Saúde'),
			13 => array('codigo' => '14','descricao' => 'Técnico da Secretaria de Educação'),
			14 => array('codigo' => '15','descricao' => 'Técnico de NTE'),
			15 => array('codigo' => '16','descricao' => 'Vice-Diretor'),
			16 => array('codigo' => '99','descricao' => 'Outros')

    );
    
    $db->monta_combo('curfuncao', $arrFuncao, 'S', 'Selecione', '', '', '', '', 'S', 'curfuncao', '', $cursista['curfuncao']);
    echo '</td>';
    echo '</tr>';
   
    
    echo '<tr>';
    echo '<td class="SubTituloCentro" colspan="2"><input type="button" name="gravar" value="Gravar" onclick="gravarInformacoesCursistas();"></td>';
    echo '</tr>';
    
    echo '</table>';
    echo '</form>';
	
	
}

function verificarFormacaoCompleta($dados) {
	global $db;
	$sql = "SELECT foecompleto FROM sisfor.formacaoescolaridade WHERE foeid='".$dados['foeid']."'";
	$foecompleto = $db->pegaUm($sql);
	echo (($foecompleto=="t")?"T":"");
	echo (($foecompleto=="f")?"F":"");
}


function condicaoEnviarAvaliacao() {
	global $db;
	
	$avaliacaofinal = $db->pegaLinha("SELECT * FROM sisfor.avaliacaofinal WHERE sifid='".$_SESSION['sisfor']['sifid']."'");
	if($avaliacaofinal) extract($avaliacaofinal);
	
	if(!$afcexecucacaoprojetoini) $erro.= 'Período de execução do projeto não preenchido\n<br>';
	if(!$afcexecucacaoprojetofim) $erro.= 'Período de execução do projeto não preenchido\n<br>';
	
	$sql = "SELECT count(*) as atividadesvazias FROM sisfor.atividadescurso a
			WHERE a.atiidpai IN(
					
			SELECT a.atiid FROM sisfor.atividadescurso a
			WHERE a.sifid='" . $_SESSION['sisfor']['sifid'] . "' AND atiidpai IS NULL
					
			) AND (atidatainicioexec IS NULL OR atidatafimexec IS NULL)";
					
	$atividadesvazias = $db->pegaUm($sql);
	
	if($atividadesvazias) $erro .= 'Plano de atividades não preenchido\n<br>';
	if(!$afcplanejamentopedagogico) $erro.= 'Planejamento Pedagógico do Curso em branco\n<br>';
	if(!$afcorganizacaopedagogica) $erro.= 'Organização Pedagógica do Curso em branco\n<br>';
	if(!$afcestruturafisicasuporte) $erro.= 'Estrutura Física e Suporte não preenchido\n<br>';
	
	$sql = "SELECT count(*) FROM sisfor.orcamento WHERE sifid='".$_SESSION['sisfor']['sifid']."' AND orcvlrexecutado IS NULL AND orcstatus='A'";
	
	$orcamentovazio = $db->pegaUm($sql);
	
	if($orcamentovazio) $erro.= 'Orçamento não preenchido\n<br>';
	if(!$afcarticulacaoinstitucional) $erro.= 'Sobre a Articulação Institucional em branco\n<br>';
	if(!$afcsobreconteudocurso) $erro.= 'Sobre o conteúdo do curso em branco\n<br>';
	if(!$afcsobremetodologia) $erro.= 'Sobre a metodologia em branco\n<br>';
	if(!$afccriteriosavaliacao) $erro.= 'Sobre os critérios de avaliação em branco\n<br>';
	if(!$afcarticulacaocomfor) $erro.= 'Sobre a articulação com o COMFOR em branco\n<br>';
	if(!$afcarticulacaomec) $erro.= 'Sobre a articulação com MEC em branco\n<br>';
	if(!$afclicoesaprendidas) $erro.= 'Lições Aprendidas em branco\n<br>';
	if(!$afcoutroscomentarios) $erro.= 'Outros comentários em branco\n<br>';
	
	
	$fpbid_max = $db->pegaUm("SELECT max((fpbanoreferencia||'-'||fpbmesreferencia||'-15')::date) as fpbid_max FROM sisfor.folhapagamentoprojeto fp
						  INNER JOIN sisfor.folhapagamento f ON f.fpbid = fp.fpbid
						  WHERE fp.sifid=".$_SESSION['sisfor']['sifid']);
	
	
	$matriculados = $db->pegaUm("SELECT count(distinct curid) FROM sisfor.cursistacurso WHERE sifid='".$_SESSION['sisfor']['sifid']."' and fpbid=".$fpbid_max);
	
	$aprovados = $db->pegaUm("SELECT count(distinct c.curid) FROM sisfor.cursistacurso c
						  INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
						  WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='p' and c.fpbid=".$fpbid_max);
	
	$reprovados = $db->pegaUm("SELECT count(distinct c.curid) FROM sisfor.cursistacurso c
						  INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
						  WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='r' and c.fpbid=".$fpbid_max);
	
	$evadidos = $db->pegaUm("SELECT count(distinct c.curid) FROM sisfor.cursistacurso c
						  INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
						  WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='e' and c.fpbid=".$fpbid_max);
	
	$falecidos = $db->pegaUm("SELECT count(distinct c.curid) FROM sisfor.cursistacurso c
						  INNER JOIN sisfor.cursistaavaliacoes ca ON ca.cucid = c.cucid
						  WHERE c.sifid='".$_SESSION['sisfor']['sifid']."' AND cavsituacao='f' and c.fpbid=".$fpbid_max);

	if($matriculados != ($aprovados+$reprovados+$evadidos+$falecidos)) {
		$erro.= 'O somatório dos Aprovados+Reprovados+Evadidos+Falecidos deve ser igual ao número de inscritos\n<br>';
	}
	
	$sql = "select count(DISTINCT c.curid) as total
			from sisfor.cursistacurso cc
			inner join sisfor.cursista c on c.curid = cc.curid
			left  join sisfor.cursistaavaliacoes a on a.cucid = cc.cucid
			where cc.sifid = '".$_SESSION['sisfor']['sifid']."' and cavsituacao in('p','r') and (curescolaridade is null or currede is null or curcontratacao is null or cursexo is null or curdatanascimento is null or curraca is null or curdeficiencia is null or curfuncao is null)";
	
	$cursistasdadosincompletos = $db->pegaUm($sql);
	
	if($cursistasdadosincompletos) $erro.= 'Existem cusistas com dados incompletos\n<br>';
	
	
	$sql = " select  'Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia || ' - Avaliação dos cursista não finalizada' as msg from sisfor.folhapagamentocursista fc 
    		 inner join workflow.documento d on d.docid = fc.docid 
    		 inner join sisfor.folhapagamento f on f.fpbid = fc.fpbid 
    		 inner join public.meses m on m.mescod::integer = f.fpbmesreferencia
    		 where fc.sifid='".$_SESSION['sisfor']['sifid']."' AND d.esdid='".ESD_EM_CADASTRAMENTO."' order by f.fpbid";
	
	$cursistasnfinalizados = $db->carregarColuna($sql);
	
	if($cursistasnfinalizados) {
		$erro .= implode('\n<br>',$cursistasnfinalizados).'\n<br>';
	}
	
	
	
	return (($erro)?$erro:true);
}

function exibirRelatorioFinalInstitucional($dados) {
	global $db, $servidor_bd, $servidor_bd_siafi, $porta_bd, $porta_bd_siafi, $nome_bd, $nome_bd_siafi, $usuario_db, $usuario_db_siafi, $senha_bd, $senha_bd_siafi;
	
	$sql = "SELECT remversaoanalise, remid, docid FROM sisfor.relatoriomensal WHERE iusd={$dados['iusd']} AND fpbid={$dados['fpbid']}";
	$relatoriomensal = $db->pegaLinha($sql);
	
	echo '<table align="center" width="100%">';
	echo '<tr>';
	echo '<td>'.(($relatoriomensal['remversaoanalise'])?$relatoriomensal['remversaoanalise']:'<h2>Não é possível visualizar o relatório</h2>').'</td>';
	echo '<td valign=top>';
	wf_desenhaBarraNavegacao( $relatoriomensal['docid'], array('remid' => $relatoriomensal['remid']) );
    echo '</td>';
	echo '</tr>';
	echo '</table>';
	

}

function exibirRelatorioFinalCurso($dados) {
	global $db;
	$_SESSION['sisfor']['sifid'] = $dados['sifid'];
	
	echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';
	echo '<link href="../includes/Estilo.css" rel="stylesheet" type="text/css"/>';
	echo '<link href="../includes/listagem.css" rel="stylesheet" type="text/css"/>';
	
	$consulta = true;
	
	include APPRAIZ_SISFOR.'coordenador_curso/avaliacaofinal.inc';
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
				$db->executar("UPDATE sisfor.pagamentobolsista SET remid=null WHERE docid='".$docid."'");
				$db->commit();
			}
				
		}
	}

	$al = array("alert"=>"Reenvio agendado com sucesso","location"=>"sisfor.php?modulo=principal/mec/mec&acao=A&aba=reenviarpagamentos");
	alertlocation($al);


}

function is_Date($str){

	$str = str_replace('/', '-', $str);
	$stamp = strtotime($str);
	if (is_numeric($stamp)){

		$month = date( 'm', $stamp );
		$day   = date( 'd', $stamp );
		$year  = date( 'Y', $stamp );

		return checkdate($month, $day, $year);

	}
	return false;
}

function condicaoReabrirCadastramento($tipo) {
	global $db;
	if($tipo=='cursista') {

		$perfis = pegaPerfilGeral();
		
		if(!$perfis) 
			$perfis = array();

		if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis) || in_array(PFL_EQUIPE_MEC,$perfis) || in_array(PFL_COORDENADOR_MEC,$perfis) || in_array(PFL_DIRETOR_MEC,$perfis) ) {
			return true;
		} else {
			return false;
		}
		
	} else {
		return true;
	}
}

function definirCurso($dados) {
	global $db;
	
	$sql = "UPDATE sisfor.planejamento2015 SET lcoaceito=".(($dados['lcoaceito'])?'true':'false').", lcoaceitojustificativa=".(($dados['lcoaceitojustificativa'])?"'".$dados['lcoaceitojustificativa']."'":"NULL")." WHERE lcoid='".$dados['lcoid']."'";
	$db->executar($sql);
	$db->commit();
	
	if(!$dados['noredirect']) {
		$al = array("alert"=>"Curso rejeitado com sucesso","location"=>"sisfor.php?modulo=principal/coordenador/coordenador_ies&acao=A&aba=planejamento2015");
		alertlocation($al);
	}
	
}

function atualizarPlanejamento2015($dados) {
	global $db;
	
	if($dados['lcovalor']) {
		foreach($dados['lcovalor'] as $lcoid => $lcovalor) {
			$sql = "UPDATE sisfor.planejamento2015 SET lcovalor=".(($lcovalor)?str_replace(array('.',','),array('','.'),$lcovalor):"NULL")." WHERE lcoid='".$lcoid."'";
			$db->executar($sql);
		}
		
		$db->commit();
	}
	
	$al = array("alert"=>"Valores atualizados com sucesso","location"=>"sisfor.php?modulo=principal/mec/planejamento2015&acao=A");
	alertlocation($al);
	
	
}

function exibirJustificativaPlanejamento2015($dados) {
	global $db;
	
	echo $db->pegaUm("SELECT lcoaceitojustificativa FROM sisfor.planejamento2015 WHERE lcoid='".$dados['lcoid']."'");

}

function resumoPlanejamento2015($dados) {
	global $db;

	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="2" cellPadding="3" border="0" align="center">';

	echo '<tr>';
	echo '<td class="SubTituloCentro">&nbsp;</td>';
	echo '<td class="SubTituloCentro">SEB</td>';
	echo '<td class="SubTituloCentro">SECADI</td>';
	echo '<td class="SubTituloCentro">Total</td>';
	echo '</tr>';

	$sql = "select
			sum(foo.lcovalor) as lcovalor,
			foo.secretaria
		
			from (
			select
			lcovalor,
			case when (cor.coordsigla ilike '%SEB%' or cor2.coordsigla ilike '%SEB%' or cor3.coordsigla ilike '%SEB%' or cor4.coordsigla ilike '%SEB%') then 'SEB'
				 when (cor.coordsigla ilike '%SECADI%' or cor2.coordsigla ilike '%SECADI%' or cor3.coordsigla ilike '%SECADI%' or cor4.coordsigla ilike '%SECADI%') then 'SECADI'
			end as secretaria
			from sisfor.planejamento2015 p
			inner join sisfor.sisfor s on s.sifid = p.sifid 
			inner join workflow.documento doc on doc.docid = s.docidprojeto 
			inner join workflow.estadodocumento e on e.esdid = doc.esdid 
			left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
			left join catalogocurso2014.curso cur on cur.curid = ieo.curid
			left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
			left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
			left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
			left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
			left join seguranca.usuario usu on usu.usucpf = s.usucpf
			left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
			left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid
			left join sisfor.outraatividade oat on oat.oatid = s.oatid
			left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid
			where p.lcoano='2014' and e.esdid='".ESD_PROJETO_VALIDADO."' 
			".(($dados['coordid'])?"and (cor.coordid='".$dados['coordid']."' or cor2.coordid='".$dados['coordid']."' or cor3.coordid='".$dados['coordid']."' or cor4.coordid='".$dados['coordid']."')":"")."		  								   		
			".(($dados['unicod'])?"and p.unicod='".$dados['unicod']."'":"")." 
        	".(($dados['nomecurso'])?"and (cur.curid||' '||cur.curdesc ilike '%".$dados['nomecurso']."%' OR cur2.curid||' '||cur2.curdesc ilike '%".$dados['nomecurso']."%' OR oc.ocunome ilike '%".$dados['nomecurso']."%' OR oat.oatnome ilike '%".$dados['nomecurso']."%')":"")."	
			) foo 
			group by foo.secretaria";
	
	$valores2014 = $db->carregar($sql);
	
	if($valores2014[0]) {
		foreach($valores2014 as $vl) {
			$cursoandamento[$vl['secretaria']] = $vl['lcovalor'];
		}
	}
	
	echo '<tr>';
	echo '<td class="SubTituloEsquerda">Valores destinados a cursos validados 2014 - em andamento</td>';
	echo '<td align=right>'.number_format($cursoandamento['SEB'],2,",",".").'</td>';
	echo '<td align=right>'.number_format($cursoandamento['SECADI'],2,",",".").'</td>';
	echo '<td align=right>'.number_format(($cursoandamento['SEB']+$cursoandamento['SECADI']),2,",",".").'</td>';
	echo '</tr>';
	
	$sql = "select
			sum(foo.lcovalor) as lcovalor,
			foo.secretaria
		
			from (
			select
			lcovalor,
			case when lcosecretaria ilike '%SEB%' then 'SEB'
				 when lcosecretaria ilike '%SECADI%' then 'SECADI'
			end as secretaria
			from sisfor.planejamento2015 p
			where p.lcoano='2015'
			".(($dados['coordid'])?"and p.lcosecretaria in(select coordsigla from catalogocurso2014.coordenacao where coordid='".$dados['coordid']."')":"")."
			".(($dados['unicod'])?"and p.unicod='".$dados['unicod']."'":"")."
			".(($dados['nomecurso'])?"and p.lconome ilike '%".$dados['nomecurso']."%'":"")."
			) foo
			group by foo.secretaria";
	
	$valores2015 = $db->carregar($sql);
	
	if($valores2015[0]) {
		foreach($valores2015 as $vl) {
			$cursooferta[$vl['secretaria']] = $vl['lcovalor'];
		}
	}
	
	echo '<tr>';
	echo '<td class="SubTituloEsquerda">Valores destinados a oferta 2015</td>';
	echo '<td align="right">'.number_format($cursooferta['SEB'],2,",",".").'</td>';
	echo '<td align="right">'.number_format($cursooferta['SECADI'],2,",",".").'</td>';
	echo '<td align="right">'.number_format(($cursooferta['SEB']+$cursooferta['SECADI']),2,",",".").'</td>';
	echo '</tr>';
	
	echo '<tr>';
	echo '<td class="SubTituloDireita">Total</td>';
	echo '<td align="right">'.number_format($cursoandamento['SEB']+$cursooferta['SEB'],2,",",".").'</td>';
	echo '<td align="right">'.number_format($cursoandamento['SECADI']+$cursooferta['SECADI'],2,",",".").'</td>';
	echo '<td align="right">'.number_format(($cursoandamento['SEB']+$cursoandamento['SECADI']+$cursooferta['SEB']+$cursooferta['SECADI']),2,",",".").'</td>';
	echo '</tr>';

	echo '</table>';


}
?>