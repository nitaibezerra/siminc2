<?

function diagnostico_5_1_estudantes($dados) {
	global $db;
	$dados['abacod']="diagnostico_5_1_estudantes";
	
	salvarJustificativaEvidencias($dados);
	
	salvarRespostasPorEscola();
	
	salvarAbaResposta($dados['abacod']);
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
}

function diagnostico_5_2_docentes_parcial($dados) {
	global $db;
	$sql = "UPDATE pdeinterativo.respostadocenteformacao rdf SET rdfstatus='I' 
			FROM ( SELECT rdoid FROM pdeinterativo.respostadocente WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' ) rd
			WHERE rd.rdoid = rdf.rdoid;";
	$db->executar($sql);
	$db->commit();
	$sql = "UPDATE pdeinterativo.respostadocente SET rdostatus='I' WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
	$db->executar($sql);
	$db->commit();
	
	if($dados['rdovinculo']) {
		foreach($dados['rdovinculo'] as $coddocente => $rdovinculo) {
			$rdoid = $db->pegaUm("INSERT INTO pdeinterativo.respostadocente(
						            pdeid, pk_cod_docente, rdovinculo, rdostatus)
								    VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$coddocente."', ".(($dados['rdovinculo'][$coddocente])?"'".$dados['rdovinculo'][$coddocente]."'":"NULL").", 'A') RETURNING rdoid;");
			
			$db->commit();
			
			$arrDocentes[$coddocente]['rdoid'] = $rdoid;
		}
	}
	
	if($dados['rdoformapro']) {
		foreach($dados['rdoformapro'] as $coddocente => $rdoformapro) {
			if($arrDocentes[$coddocente]['rdoid']) {
				
				$db->executar("UPDATE pdeinterativo.respostadocente SET 
								rdoformapro=".(($dados['rdoformapro'][$coddocente])?"'".$dados['rdoformapro'][$coddocente]."'":"NULL").",
								rdocritico=NULL
							   WHERE rdoid='".$arrDocentes[$coddocente]['rdoid']."';");
				
				$db->commit();
				
			} else {
				
				$rdoid = $db->pegaUm("INSERT INTO pdeinterativo.respostadocente(
							            pdeid, pk_cod_docente, rdoformapro, rdostatus)
									    VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$coddocente."', ".(($dados['rdoformapro'][$coddocente])?"'".$dados['rdoformapro'][$coddocente]."'":"NULL").", 'A') RETURNING rdoid;");
				
				$db->commit();
				
				$arrDocentes[$coddocente]['rdoid'] = $rdoid;
				
			}
		}
	}
	
	if($dados['rdodeseja']) {
		foreach($dados['rdodeseja'] as $coddocente => $arrDis) {
			foreach($arrDis as $coddisciplina => $deseja) {
				if($arrDocentes[$coddocente]['dis'][$coddisciplina]) {
					
					$db->executar("UPDATE pdeinterativo.respostadocenteformacao
   								   SET rdodeseja=".$dados['rdodeseja'][$coddocente][$coddisciplina]."
 								   WHERE rdfid='".$arrDocentes[$coddocente]['dis'][$coddisciplina]."'");
					$db->commit();
					
				} else {
					
					if($dados['rdodeseja'][$coddocente][$coddisciplina]) {
						$rdfid = $db->pegaUm("INSERT INTO pdeinterativo.respostadocenteformacao(
							     		       rdoid, pk_cod_disciplina, rdodeseja, rdfstatus)
							    				VALUES ('".$arrDocentes[$coddocente]['rdoid']."', '".$coddisciplina."', ".$dados['rdodeseja'][$coddocente][$coddisciplina].", 'A') RETURNING rdfid;");
						
						$db->commit();
						
						$arrDocentes[$coddocente]['dis'][$coddisciplina] = $rdfid;
					}
				}
			}
		}
		
	}
	
	include_once APPRAIZ.'includes/classes/cacheSimec.class.inc';
	$cache = new cache(false);
	$cache->apagarCache("diagnostico_5_2_docentes_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_EQUIPE_FNDE);
	$cache->apagarCache("diagnostico_5_2_docentes_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_CONSULTA_ESTADUAL);
	$cache->apagarCache("diagnostico_5_2_docentes_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_CONSULTA_MUNICIPAL);
	$cache->apagarCache("diagnostico_5_2_docentes_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEESC_PERFIL_CONSULTA);
	$cache->apagarCache("diagnostico_5_2_docentes_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEESC_PERFIL_DIRETOR);
	$cache->apagarCache("diagnostico_5_2_docentes_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_ESTADUAL);
	$cache->apagarCache("diagnostico_5_2_docentes_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_MUNICIPAL);
	$cache->apagarCache("diagnostico_5_2_docentes_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_EQUIPE_MEC);
	$cache->apagarCache("diagnostico_5_2_docentes_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_SUPER_USUARIO);
	$cache->apagarCache("diagnostico_5_2_docentes_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_PAR_ESTADUAL);
	$cache->apagarCache("diagnostico_5_2_docentes_".$_SESSION['pdeinterativo_vars']['pdeid']."_".PDEINT_PERFIL_COMITE_PAR_MUNICIPAL);
	$cache->apagarCache("diagnostico_5_2_docentes_".$_SESSION['pdeinterativo_vars']['pdeid']."_semperfil");
	
	apagarCachePdeInterativo();
	$sql = "select flaid from pdeinterativo.flag where pdeid = ".$_SESSION['pdeinterativo_vars']['pdeid'];
	$flaid = $db->pegaUm($sql);
	if($flaid){
		$sql = "update pdeinterativo.flag set atualizaplano = true where flaid = $flaid";
	}else{
		$sql = "insert into pdeinterativo.flag (pdeid,atualizaplano) values (".$_SESSION['pdeinterativo_vars']['pdeid'].",true)";
	}
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
	
	
}

function diagnostico_5_2_docentes($dados) {
	global $db;

	$sql = "UPDATE pdeinterativo.respostadocenteformacao rdf SET rdfstatus='I' 
			FROM ( SELECT rdoid FROM pdeinterativo.respostadocente WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' ) rd
			WHERE rd.rdoid = rdf.rdoid;";
	$db->executar($sql);
	$db->commit();
	$sql = "UPDATE pdeinterativo.respostadocente SET rdostatus='I' WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";
	$db->executar($sql);
	$db->commit();
	
	if($dados['rdovinculo']) {
		foreach($dados['rdovinculo'] as $coddocente => $rdovinculo) {
			$rdoid = $db->pegaUm("INSERT INTO pdeinterativo.respostadocente(
						            pdeid, pk_cod_docente, rdovinculo, rdostatus)
								    VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$coddocente."', ".(($dados['rdovinculo'][$coddocente])?"'".$dados['rdovinculo'][$coddocente]."'":"NULL").", 'A') RETURNING rdoid;");
			
			$db->commit();
			
			$arrDocentes[$coddocente]['rdoid'] = $rdoid;
		}
	}
	
	if($dados['rdoformapro']) {
		foreach($dados['rdoformapro'] as $coddocente => $rdoformapro) {
			if($arrDocentes[$coddocente]['rdoid']) {
				
				$db->executar("UPDATE pdeinterativo.respostadocente SET 
								rdoformapro=".(($dados['rdoformapro'][$coddocente])?"'".$dados['rdoformapro'][$coddocente]."'":"NULL").",
								rdocritico=NULL
							   WHERE rdoid='".$arrDocentes[$coddocente]['rdoid']."';");
				
				$db->commit();
				
			} else {
				
				$rdoid = $db->pegaUm("INSERT INTO pdeinterativo.respostadocente(
							            pdeid, pk_cod_docente, rdoformapro, rdostatus)
									    VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', '".$coddocente."', ".(($dados['rdoformapro'][$coddocente])?"'".$dados['rdoformapro'][$coddocente]."'":"NULL").", 'A') RETURNING rdoid;");
				
				$db->commit();
				
				$arrDocentes[$coddocente]['rdoid'] = $rdoid;
				
			}
		}
	}
	
	if($dados['rdodeseja']) {
		foreach($dados['rdodeseja'] as $coddocente => $arrDis) {
			foreach($arrDis as $coddisciplina => $deseja) {
				if($arrDocentes[$coddocente]['dis'][$coddisciplina]) {
					
					$db->executar("UPDATE pdeinterativo.respostadocenteformacao
   								   SET rdodeseja=".$dados['rdodeseja'][$coddocente][$coddisciplina]."
 								   WHERE rdfid='".$arrDocentes[$coddocente]['dis'][$coddisciplina]."'");
					$db->commit();
					
				} else {
					
					if($dados['rdodeseja'][$coddocente][$coddisciplina]) {
						$rdfid = $db->pegaUm("INSERT INTO pdeinterativo.respostadocenteformacao(
							     		       rdoid, pk_cod_disciplina, rdodeseja, rdfstatus)
							    				VALUES ('".$arrDocentes[$coddocente]['rdoid']."', '".$coddisciplina."', ".$dados['rdodeseja'][$coddocente][$coddisciplina].", 'A') RETURNING rdfid;");
						
						$db->commit();
						
						$arrDocentes[$coddocente]['dis'][$coddisciplina] = $rdfid;
					}
				}
			}
		}
		
	}
	
	$dados['abacod']="diagnostico_5_2_docentes";
	
	salvarJustificativaEvidencias($dados);
	
	salvarRespostasPorEscola();
	
	salvarAbaResposta($dados['abacod']);
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
}


function diagnostico_5_3_demaisprofissionais($dados) {
	global $db;
	salvarRespostasPorEscola();
	
	salvarAbaResposta("diagnostico_5_3_demaisprofissionais");
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
}




function gerenciarMembroConselho($dados) {
	global $db;
	
	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	echo "<script type=text/javascript src=/includes/prototype.js></script>";
	echo "<script language=\"javascript\" type=\"text/javascript\" src=\"../includes/webservice/cpf.js\" /></script>";
	
	echo "<script>";
	
	echo "function validarMembroConselho(){
		  document.getElementById('usucpf').value = mascaraglobal('###.###.###-##',document.getElementById('usucpf').value);
		  if(document.getElementById('pesnome').value=='') {alert('Preencha um nome');return false;}
		  if(!validar_cpf(document.getElementById('usucpf').value)) {alert('CPF inválido');return false;}
		  if(document.getElementById('usucpf').value=='') {alert('Preencha um cpf');return false;}
		  if(document.getElementById('tpeid').value=='') {alert('Selecione um perfil');return false;}
		  if(document.getElementById('dpetelefoneddd').value.length!=2) {alert('Preencha o DDD');return false;}
		  if(document.getElementById('dpetelefone').value.length=='') {alert('Preencha o Telefone');return false;}
		  if(document.getElementById('dpeemail').value.length=='') {alert('Preencha o Email');return false;}
		  if(!validaEmail(document.getElementById('dpeemail').value)) {alert('Email inválido');return false;}
		  if(document.getElementById('tenid').value.length=='') {alert('Preencha a Escolaridade');return false;}
		  divCarregando();
		  document.getElementById('form_direcao').submit();
		  }";
	
	echo "function carregaUsuario() {
		    divCarregando();
			var usucpf=document.getElementById('usucpf').value;
			usucpf = usucpf.replace('-','');
			usucpf = usucpf.replace('.','');
			usucpf = usucpf.replace('.','');
			
   			var comp = new dCPF();
			comp.buscarDados(usucpf);
			var arrDados = new Object();
			if(!comp.dados.no_pessoa_rf){
				alert('CPF Inválido');
				return false;
		  		divCarregado();
			}
			document.getElementById('pesnome').value=comp.dados.no_pessoa_rf;
		    divCarregado();
		  }";
	
	echo "</script>";
	
	echo "<form method=post id=form_direcao>";
	echo "<input type=hidden name=apeid value=".$dados['apeid'].">";
	
	if($dados['pesid']) {
		echo "<input type=hidden name=requisicao value=atualizarPessoa>";
		echo "<input type=hidden name=pesid value=".$dados['pesid'].">";
		echo "<input type=hidden name=carregar_funcao_opener value=\"carregarMembrosConselho()\">";
		
		$dadospessoa = $db->pegaLinha("SELECT pes.pesnome, pes.usucpf, ptp.tpeid, 
											  SUBSTR(dpetelefone,1,3) as dpetelefoneddd,
											  SUBSTR(dpetelefone,4,8) as dpetelefone,
											  dpeemail, 
											  dps.tenid,
											  dir.dirqtdanoexerce,
											  dir.dirqtdmesesexerce,
											  dirformescolha,
											  direscolagestor,
											  dircursodistan
									   FROM pdeinterativo.pessoa pes 
									   LEFT JOIN pdeinterativo.pessoatipoperfil ptp ON ptp.pesid = pes.pesid 
									   LEFT JOIN pdeinterativo.direcao dir ON dir.pesid = pes.pesid
									   LEFT JOIN pdeinterativo.detalhepessoa dps ON dps.pesid = pes.pesid 
									   WHERE pes.pesid='".$dados['pesid']."'");
		extract($dadospessoa);
		
	} else {
		echo "<input type=hidden name=requisicao value=inserirMembroConselho>";
	}
	
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\">Incluir</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Nome:</td>";
	echo "<td>".campo_texto('pesnome', "S", "N", "Nome", 40, 180, "", "", '', '', 0,'id="pesnome"','',$pesnome)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">CPF:</td>";
	echo "<td>".campo_texto('usucpf', "S", "S", "CPF", 16, 14, "###.###.###-##", "", '', '', 0,'id="usucpf"','',mascaraglobal($usucpf,"###.###.###-##"),'carregaUsuario();')."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Segmento:</td>";
	
	$sql = "SELECT tp.tpeid as codigo, tp.tpedesc as descricao FROM pdeinterativo.tipoperfil tp 
			INNER JOIN pdeinterativo.perfilarea pa ON pa.tpeid = tp.tpeid 
			WHERE pa.apeid='".$dados['apeid']."'";
	
	echo "<td>".$db->monta_combo('tpeid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'tpeid', true, $tpeid)."</td>";
	echo "</tr>";
	
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Telefone:</td>";
	echo "<td>".campo_texto('dpetelefoneddd', "N", "S", "Telefone", 2, 3, "##", "", '', '', 0,'id="dpetelefoneddd"','',$dpetelefoneddd)." ".campo_texto('dpetelefone', "S", "S", "Telefone", 9, 10, "########", "", '', '', 0,'id="dpetelefone"','',$dpetelefone)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Email:</td>";
	echo "<td>".campo_texto('dpeemail', "S", "S", "Email", 40, 50, "", "", '', '', 0,'id="dpeemail"','',$dpeemail)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Escolaridade:</td>";
	
	$sql = "SELECT tenid as codigo, tendesc as descricao 
			FROM pdeinterativo.tipoescolaridade 
			WHERE tenstatus='A' order by tendesc";
	
	echo "<td>".$db->monta_combo('tenid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'tenid', true, $tenid)."</td>";
	
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\"><input type=button name=salvar value=Salvar onclick=validarMembroConselho();> <input type=button value=Cancelar onclick=window.close();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
}

function inserirMembroConselho($dados) {
	global $db;
	
	$dados['return_id']=true;
	$pesid = inserirPessoa($dados);
	
	$mceid = $db->pegaUm("SELECT mceid FROM pdeinterativo.membroconselho WHERE pesid='".$pesid."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
	
	if(!$mceid) {
	
		$sql = "INSERT INTO pdeinterativo.membroconselho(
	            pesid, mcestatus, pdeid)
	    		VALUES ('".$pesid."', 'A', '".$_SESSION['pdeinterativo_vars']['pdeid']."');";
		
		$db->executar($sql);
		$db->commit();
		
	}
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.opener.carregarMembrosConselho();
			window.close();
		  </script>";
}

function carregarMembrosConselho($dados) {
	global $db;
	
	$sql = "SELECT '<center><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"gerenciarMembroConselho(\'".APE_MEMBROSCONSELHO."\', \''||p.pesid||'\');\"> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirMembroConselho(\''||p.pesid||'\')\"></center>' as acoes,
				   p.pesnome,
				   p.usucpf||'&nbsp;',
				   t.tpedesc,
				   e.tendesc,
				   d.dpetelefone,
				   d.dpeemail  
			FROM pdeinterativo.pessoa p 
			INNER JOIN pdeinterativo.membroconselho m ON m.pesid = p.pesid 
			LEFT JOIN pdeinterativo.pessoatipoperfil ptp ON ptp.pesid = p.pesid  
			LEFT JOIN pdeinterativo.tipoperfil t ON t.tpeid = ptp.tpeid 
			LEFT JOIN pdeinterativo.perfilarea pa ON pa.tpeid = t.tpeid
			LEFT JOIN pdeinterativo.detalhepessoa d ON d.pesid = p.pesid 
			LEFT JOIN pdeinterativo.tipoescolaridade e ON e.tenid = d.tenid 
			WHERE m.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND pa.apeid='".APE_MEMBROSCONSELHO."' 
			ORDER BY p.pesnome";
	
	$cabecalho = array("&nbsp;","Nome","CPF","Segmento","Escolaridade","Telefone","E-mail");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%','N');

}

function excluirMembroConselho($dados) {
	global $db;
	$db->executar("DELETE FROM pdeinterativo.membroconselho WHERE pesid='".$dados['pesid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
	$db->commit();
	
	echo "Membro removido com sucesso";
}

function diagnostico_5_4_paisecomunidade($dados) {
	global $db;
	
	$dados['abacod']="diagnostico_5_4_paisecomunidade";
	
	salvarJustificativaEvidencias($dados);
	
	
	salvarRespostasPorEscola();
	
	$rpcid = $db->pegaUm("SELECT rpcid FROM pdeinterativo.respostapaiscomunidade WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
	$dados['rpcperiodicidade'] = (($dados['rpcpossuiconcelho']=="TRUE")?$dados['rpcperiodicidade']:"");
	
	if($dados['rpcpossuiconcelho']=="FALSE") {
		$db->executar("DELETE FROM pdeinterativo.membroconselho WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
		$db->commit();
	}
	
	if($rpcid) {
		$sql = "UPDATE pdeinterativo.respostapaiscomunidade
				   SET rpcpossuiconcelho=".(($dados['rpcpossuiconcelho'])?$dados['rpcpossuiconcelho']:"NULL").", 
				   	   rpcperiodicidade=".(($dados['rpcperiodicidade'])?"'".$dados['rpcperiodicidade']."'":"NULL").",
				   	   rpcunidadeexecutora=".(($dados['rpcunidadeexecutora'])?"'".$dados['rpcunidadeexecutora']."'":"NULL")."  
				 WHERE rpcid='".$rpcid."';";
	} else {
		$sql = "INSERT INTO pdeinterativo.respostapaiscomunidade(
	            pdeid, rpcpossuiconcelho, rpcperiodicidade, rpcstatus,rpcunidadeexecutora)
	    		VALUES ('".$_SESSION['pdeinterativo_vars']['pdeid']."', 
	    				".(($dados['rpcpossuiconcelho'])?$dados['rpcpossuiconcelho']:"NULL").", 
	    				".(($dados['rpcperiodicidade'])?"'".$dados['rpcperiodicidade']."'":"NULL").",
	    				'A',
	    				".(($dados['rpcunidadeexecutora'])? "'".$dados['rpcunidadeexecutora']."'":"NULL").");";		
	}
	
	$db->executar($sql);
	$db->commit();
	
	salvarAbaResposta($dados['abacod']);
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
	
}


function gerenciarDemaisProfissionais($dados) {
	global $db;
	
	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	echo "<script type=text/javascript src=/includes/prototype.js></script>";
	echo "<script language=\"javascript\" type=\"text/javascript\" src=\"../includes/webservice/cpf.js\" /></script>";
	
	echo "<script>";
	
	echo "function validarDemaisProfissionais(){
		  if(document.getElementById('pesnome').value=='') {alert('Preencha um nome');return false;}
		  if(document.getElementById('usucpf').value=='') {alert('Preencha um cpf');return false;}
		  if(!validar_cpf(document.getElementById('usucpf').value)) {alert('CPF inválido');return false;}
		  if(document.getElementById('tenid').value.length=='') {alert('Preencha a Escolaridade');return false;}
		  if(document.getElementById('dpevinculo').value=='') {alert('Preencha o vínculo');return false;}
		  divCarregando();
		  document.getElementById('form_dprofissionais').submit();
		  }";
	
	echo "function carregaUsuario() {
		    divCarregando();
			var usucpf=document.getElementById('usucpf').value;
			usucpf = usucpf.replace('-','');
			usucpf = usucpf.replace('.','');
			usucpf = usucpf.replace('.','');
			
   			var comp = new dCPF();
			comp.buscarDados(usucpf);
			var arrDados = new Object();
			if(!comp.dados.no_pessoa_rf){
				alert('CPF Inválido');
				return false;
		  		divCarregado();
			}
			document.getElementById('pesnome').value=comp.dados.no_pessoa_rf;
		    divCarregado();
		  }";
	
	
	echo "</script>";
	echo "<body>";
	echo "<form method=post id=form_dprofissionais>";
	echo "<input type=hidden name=apeid value=".$dados['apeid'].">";
	
	if($dados['pesid']) { 
		
		if(!is_numeric($dados['pesid'])) die("<script>alert('Problemas para abrir dados dos prossifionais.');window.close();</script>");
		
		echo "<input type=hidden name=requisicao value=atualizarDemaisProfissionais>";
		echo "<input type=hidden name=pesid value=".$dados['pesid'].">";
		
		$dadospessoa = $db->pegaLinha("SELECT pes.pesnome, pes.usucpf, 
											  dps.tenid, 
											  dpi.dpevinculo,
											  dpi.dpepartcurso
									   FROM pdeinterativo.pessoa pes
									   LEFT JOIN pdeinterativo.detalhepessoa dps ON dps.pesid = pes.pesid 
									   LEFT JOIN pdeinterativo.demaisprofissionais dpi ON dpi.pesid = pes.pesid  
									   WHERE pes.pesid='".$dados['pesid']."'");
		extract($dadospessoa);
		
	} else {
		echo "<input type=hidden name=requisicao value=inserirDemaisProfissionais>";
	}
	
	
	echo "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\"	align=\"center\">";
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\">Incluir</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Nome:</td>";
	echo "<td>".campo_texto('pesnome', "S", "N", "Nome", 45, 180, "", "", '', '', 0,'id="pesnome"','',$pesnome)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">CPF:</td>";
	echo "<td>".campo_texto('usucpf', "S", "S", "CPF", 16, 14, "###.###.###-##", "", '', '', 0,'id="usucpf"','',mascaraglobal($usucpf,"###.###.###-##"),'carregaUsuario();')."</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Escolaridade:</td>";
	
	$sql = "SELECT tenid as codigo, tendesc as descricao 
			FROM pdeinterativo.tipoescolaridade 
			WHERE tenstatus='A'";
	
	echo "<td>".$db->monta_combo('tenid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'tenid', true, $tenid)."</td>";
	
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Área de atuação:</td>";
	
	$sql = "SELECT aa.aadis, aa.aaddesc ".(($dados['pesid'])?", pa.pesid, pa.paaoutra":"")." FROM pdeinterativo.areaatuacao aa 
			".(($dados['pesid'])?"LEFT JOIN pdeinterativo.pessoaareaatuacao pa ON pa.aadis = aa.aadis AND pa.pesid='".$dados['pesid']."'":"");
	
	$areasatuacao = $db->carregar($sql);
	
	if($areasatuacao[0]) {
		$tabela .= "<table width=100% bgcolor=white>";
		$i=0;
		foreach($areasatuacao as $area) {
			if($i==0) $tabela .= "<tr>";
			$tabela .= "<td><input ".(($area['aadis']==AAD_OUTRA)?"onclick=\"if(this.checked){document.getElementById('div_outra').style.display='';}else{document.getElementById('div_outra').style.display='none';document.getElementById('outra').value='';}\"":"")." type=checkbox name=aadis[] value=".$area['aadis']." ".(($area['pesid'])?"checked":"")."> ".$area['aaddesc']." ".(($area['aadis']==AAD_OUTRA)?"<br><div ".(($area['pesid'])?"":"style=display:none;")." id=div_outra><input class=normal type=text name=outra id=outra value=".$area['paaoutra']."></div>":"")."</td>";
			$i++;
			if($i==2) {
				$tabela .= "</tr>";
				$i=0;
			}
			
		}
		$tabela .= "</table>";
		
	}
	
	echo "<td>".$tabela."</td>";
	echo "</tr>";
	
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Vínculo:</td>";
	$vinculo = array(0 => array("codigo" => "E","descricao" => "Efetivo"),
					 1 => array("codigo" => "C","descricao" => "Contratado"));
	echo "<td>".$db->monta_combo('dpevinculo', $vinculo, 'S', 'Selecione', '', '', '', '200', 'S', 'dpevinculo', true, $dpevinculo)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Deseja participar de algum curso técnico oferecido pelo programa Profuncionário?</td>";
	echo "<td><input type=radio name=dpepartcurso value=S ".(($dpepartcurso=="S")?"checked":"")."> Sim <input type=radio name=dpepartcurso value=N ".(($dpepartcurso=="N")?"checked":"")."> Não</td>";
	echo "</tr>";

	
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=\"2\"><input type=button name=salvar value=Salvar onclick=validarDemaisProfissionais();> <input type=button value=Cancelar onclick=window.close();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
	echo "</body>";
}

function inserirDemaisProfissionais($dados) {
	global $db;

	$dados['return_id']=true;
	$pesid = inserirPessoa($dados);
		
	if($dados['aadis']) {
		
		$db->executar("DELETE FROM pdeinterativo.pessoaareaatuacao WHERE pesid='".$pesid."'");
		$db->commit();
		
		foreach($dados['aadis'] as $aadis) {
			
			$paaid = $db->pegaUm("SELECT paaid FROM pdeinterativo.pessoaareaatuacao WHERE pesid='".$pesid."' AND aadis='".$aadis."'");
			
			if(!$paaid) {
				$sql = "INSERT INTO pdeinterativo.pessoaareaatuacao(
		            	pesid, aadis, paaoutra, pdeid)
		    			VALUES ('".$pesid."', '".$aadis."', ".(($aadis==AAD_OUTRA)?"'".$dados['outra']."'":"NULL").", '".$_SESSION['pdeinterativo_vars']['pdeid']."');";
				
				$db->executar($sql);
				$db->commit();
			}
		}

	}
	
	$dpeid = $db->pegaUm("SELECT dpeid FROM pdeinterativo.demaisprofissionais WHERE pesid='".$pesid."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
	
	if(!$dpeid) {
		
		$sql = "INSERT INTO pdeinterativo.demaisprofissionais(
	            pesid, dpevinculo, dpepartcurso, dpestatus, pdeid)
	    		VALUES ('".$pesid."', '".$dados['dpevinculo']."', 
	    		".(($dados['dpepartcurso'])?"'".$dados['dpepartcurso']."'":"NULL").", 'A',
	    		'".$_SESSION['pdeinterativo_vars']['pdeid']."');";
		
		$db->executar($sql);
		$db->commit();
		
	}
	
	echo "<script>
			alert('Profissional inserido com sucesso');
			window.opener.carregarDemaisProfissionais();
			window.close();
		  </script>";
}

function carregarDemaisProfissionais($dados) {
	global $db;
	
	$sql = "SELECT '<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"gerenciarDemaisProfissionais(\''||p.pesid||'\');\"> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirDemaisProfissionais(\''||p.pesid||'\')\"></center>' as acoes,
				   p.pesnome,
				   p.usucpf||'&nbsp;',
				   e.tendesc,
				   CASE WHEN dpevinculo='E' THEN 'Efetivo'
				   		WHEN dpevinculo='C' THEN 'Contratado' END as vinculo,
				   p.pesid as areaatuacao
			FROM pdeinterativo.pessoa p 
			INNER JOIN pdeinterativo.demaisprofissionais dd ON dd.pesid = p.pesid 
			LEFT JOIN pdeinterativo.detalhepessoa d ON d.pesid = p.pesid 
			LEFT JOIN pdeinterativo.tipoescolaridade e ON e.tenid = d.tenid 
			WHERE dd.pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'";

	$dprofissionais = $db->carregar($sql);
	if($dprofissionais[0]) {
		foreach($dprofissionais as $dprof) {
			
			$sql = "SELECT aa.aaddesc FROM pdeinterativo.areaatuacao aa 
			 		INNER JOIN pdeinterativo.pessoaareaatuacao pa ON pa.aadis = aa.aadis 
			 		WHERE pesid='".$dprof['areaatuacao']."'";
			$areas = $db->carregarColuna($sql);
			if($areas) $dprof['areaatuacao'] = implode(", ",$areas);
			else $dprof['areaatuacao'] = "-";
			
			$demaisprofissionais[] = $dprof;
		}
	} else $demaisprofissionais=array();
	$cabecalho = array("&nbsp;","Nome","CPF","Escolaridade","Vínculo","Área de atuação");
	$db->monta_lista_simples($demaisprofissionais,$cabecalho,50,5,'N','100%','N');

}

function atualizarDemaisProfissionais($dados) {
	global $db;
	
	$dados['return_id']=true;
	atualizarPessoa($dados);
	
	$sql = "UPDATE pdeinterativo.demaisprofissionais
   			SET dpevinculo=".(($dados['dpevinculo'])?"'".$dados['dpevinculo']."'":"NULL").", 
   				dpepartcurso=".(($dados['dpepartcurso'])?"'".$dados['dpepartcurso']."'":"NULL")." 
 			WHERE pesid='".$dados['pesid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."';";
	$db->executar($sql);
	$db->commit();

	$db->executar("DELETE FROM pdeinterativo.pessoaareaatuacao WHERE pesid='".$dados['pesid']."'");
	$db->commit();
	
	if($dados['aadis']) {
		foreach($dados['aadis'] as $aadis) {
			$sql = "INSERT INTO pdeinterativo.pessoaareaatuacao(
	            	pesid, aadis, paaoutra, pdeid)
	    			VALUES ('".$dados['pesid']."', '".$aadis."', ".(($aadis==AAD_OUTRA)?"'".$dados['outra']."'":"NULL").", '".$_SESSION['pdeinterativo_vars']['pdeid']."');";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	echo "<script>
			alert('Profissional atualizado com sucesso');
			window.opener.carregarDemaisProfissionais();
			window.close();
		  </script>";
	
}

function excluirDemaisProfissionais($dados) {
	global $db;
	$db->executar("DELETE FROM pdeinterativo.demaisprofissionais WHERE pesid='".$dados['pesid']."' AND pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
	$db->commit();
	
	echo "Profissional removido com sucesso";
}

function diagnostico_5_5_sintesedimensao5($dados) {
	global $db;

	if($dados['critico']) {
		foreach($dados['critico'] as $indice => $valor) {
			$sql = "UPDATE pdeinterativo.respostapergunta SET critico=".$valor." WHERE repid='".$indice."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($dados['respostapaiscomunidade']) {
		foreach($dados['respostapaiscomunidade'] as $campo => $vl) {
			$db->executar("UPDATE pdeinterativo.respostapaiscomunidade SET ".$campo."=".$vl." WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."'");
			$db->commit();
		}
	}
	
	if($dados['respostadocente']) {
		foreach($dados['respostadocente'] as $campo => $vl) {
			$db->executar("UPDATE pdeinterativo.respostadocente SET rdocritico=".$vl." WHERE pdeid='".$_SESSION['pdeinterativo_vars']['pdeid']."' AND rdoid IN(".$campo.")");
			$db->commit();
		}
	}
	
	salvarAbaResposta("diagnostico_5_5_sintesedimensao5");
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
}

?>