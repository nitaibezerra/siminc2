<?

function excluirPessoa($dados) {
	global $db;
	
	if( $dados['tpeid'] ){
		$sql = "DELETE FROM pdeinterativo2013.pessoatipoperfil WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND pesid=".$dados['pesid']." AND tpeid = ".$dados['tpeid'];
		$db->executar($sql);
		$sql = "SELECT count(tpeid) FROM pdeinterativo2013.pessoatipoperfil WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND pesid=".$dados['pesid'];
		$qtdFuncao = $db->pegaUm($sql);
	}
	if( $qtdFuncao < 1 ){
		$sql = "DELETE FROM pdeinterativo2013.detalhepessoa WHERE pesid='".$dados['pesid']."'";
		$db->executar($sql);
		$sql = "DELETE FROM pdeinterativo2013.direcao WHERE pesid='".$dados['pesid']."'";
		$db->executar($sql);
		$sql = "UPDATE pdeinterativo2013.pessoa SET pesstatus='I' WHERE pesid='".$dados['pesid']."'";
		$db->executar($sql);
	}
	
	$db->commit();
	
	echo "Dirigente removido com sucesso";
}

function atualizarPessoa($dados) {
	global $db;
	
	$arrPessoa = $db->pegaLinha("SELECT pesid FROM pdeinterativo2013.pessoa WHERE usucpf='".str_replace(array(".","-"),array("",""),$dados['usucpf'])."'");
	
	$pesid_ = $arrPessoa['pesid'];
	
	if($pesid_!=$dados['pesid']) {
		die("<script>
				alert('CPF ja existe no sistema. Exclua e insira novamente.');
				window.close();
			 </script>");
	}
	
	$sql = "UPDATE pdeinterativo2013.pessoa SET
            usucpf='".str_replace(array(".","-"),array("",""),$dados['usucpf'])."', 
            pesnome='".$dados['pesnome']."', 
            pesstatus='A' 
            WHERE pesid='".$dados['pesid']."'";

	$db->executar($sql);
	
	
	if($dados['tpeid']) $peaid_n = $db->pegaUm("select peaid from pdeinterativo2013.perfilarea where tpeid='".$dados['tpeid']."'");
	if($peaid_n) $ptpid = $db->pegaUm("SELECT ptpid FROM pdeinterativo2013.pessoatipoperfil p INNER JOIN pdeinterativo2013.perfilarea a ON a.tpeid = p.tpeid WHERE pesid='".$dados['pesid']."' AND peaid='".$peaid_n."'");
	
	if($dados['tpeid']) {
		
		if($ptpid) {
			$sql = "UPDATE pdeinterativo2013.pessoatipoperfil SET tpeid='".$dados['tpeid']."' WHERE ptpid='{$ptpid}';";
			$db->executar($sql);
		} else {
			$sql = "INSERT INTO pdeinterativo2013.pessoatipoperfil(tpeid, pesid, pdeid)
	    			VALUES ('".$dados['tpeid']."', '".$dados['pesid']."', '".$_SESSION['pdeinterativo2013_vars']['pdeid']."');";
			
			$db->executar($sql);
		}
		
	}
	
	$db->commit();
	
	
	$dpeid = $db->pegaUm("SELECT dpeid FROM pdeinterativo2013.detalhepessoa WHERE pesid='".$dados['pesid']."'");
	if($dpeid) {
		$sql = "UPDATE pdeinterativo2013.detalhepessoa SET
				".((trim($dados['tenid']))?"tenid='".trim($dados['tenid'])."',":"")." 
				".(($dados['dpetelefoneddd'])?"dpetelefone='".$dados['dpetelefoneddd'].$dados['dpetelefone']."',":"")." 
			    ".(($dados['dpeemail'])?"dpeemail='".$dados['dpeemail']."',":"")."
			    dpestatus='A' 
			    WHERE pesid='".$dados['pesid']."'";
	} else {
		$sql = "INSERT INTO pdeinterativo2013.detalhepessoa(
			            pesid, 
			            fgtid, 
			            ".((trim($dados['tenid']))?"tenid,":"")." 
			            segid, 
			            ".((trim($dados['dpetelefone']))?"dpetelefone,":"")." 
			            ".((trim($dados['dpeemail']))?"dpeemail,":"")."
			            dpestatus)
			    VALUES ('".$dados['pesid']."', NULL, 
			    		".((trim($dados['tenid']))?"'".trim($dados['tenid'])."',":"")."  
	    				NULL, 
	    				".((trim($dados['dpetelefone']))?"'".$dados['dpetelefoneddd'].$dados['dpetelefone']."',":"")."
	    				".((trim($dados['dpeemail']))?"'".$dados['dpeemail']."',":"")." 
			            'A');";
	}
	$db->executar($sql);

	$dirid = $db->pegaUm("SELECT dirid FROM pdeinterativo2013.direcao WHERE pesid='".$dados['pesid']."'");
	
	if($dirid) {
		$sql = "UPDATE pdeinterativo2013.direcao SET
			    ".(($dados['dirqtdanoexerce'])?"dirqtdanoexerce='".$dados['dirqtdanoexerce']."',":"")." 
			    ".(($dados['dirqtdmesesexerce'])?"dirqtdmesesexerce='".$dados['dirqtdmesesexerce']."',":"")." 
			    ".(($dados['dirformescolha'])?"dirformescolha='".$dados['dirformescolha']."',":"")." 
			    ".(($dados['direscolagestor'])?"direscolagestor='".$dados['direscolagestor']."',":"")." 
			    ".(($dados['dircursodistan'])?"dircursodistan='".$dados['dircursodistan']."',":"")."
			    dirstatus='A' 
			    WHERE pesid='".$dados['pesid']."'";
		
		$db->executar($sql);
		
	} else {
		
		if($dados['dirqtdanoexerce']   || 
		   $dados['dirqtdmesesexerce'] || 
		   $dados['dirformescolha']    || 
		   $dados['direscolagestor']   || 
		   $dados['dircursodistan']) {
			   	
			$sql = "INSERT INTO pdeinterativo2013.direcao(
				            pesid, 
				    		".(($dados['dirqtdanoexerce'])?"dirqtdanoexerce,":"")." 
				    		".(($dados['dirqtdmesesexerce'])?"dirqtdmesesexerce,":"")." 
				    		".(($dados['dirformescolha'])?"dirformescolha,":"")." 
				    		".(($dados['direscolagestor'])?"direscolagestor,":"")." 
				    		".(($dados['dircursodistan'])?"dircursodistan,":"")."
				            dirstatus)
				    VALUES ('".$dados['pesid']."', 
				    		".(($dados['dirqtdanoexerce'])?"'".$dados['dirqtdanoexerce']."',":"")." 
				    		".(($dados['dirqtdmesesexerce'])?"'".$dados['dirqtdmesesexerce']."',":"")." 
				    		".(($dados['dirformescolha'])?"'".$dados['dirformescolha']."',":"")." 
				    		".(($dados['direscolagestor'])?"'".$dados['direscolagestor']."',":"")." 
				    		".(($dados['dircursodistan'])?"'".$dados['dircursodistan']."',":"")." 
				    		'A');";
				
			$db->executar($sql);
			   	
		}
		
	}
	
	$db->commit();
	
	if(!$dados['return_id']) {
		echo "<script>
				alert('Dados gravados com sucesso');
				window.opener.".(($dados['carregar_funcao_opener'])?$dados['carregar_funcao_opener']:"carregarInfoEquipeGestora_4_1()").";
				window.close();
			  </script>";
	}
	

}

function inserirPessoa($dados) {
	global $db;
	
	$pesid = $db->pegaUm("SELECT pesid FROM pdeinterativo2013.pessoa WHERE usucpf='".trim(str_replace(array(".","-"," ","/"),array("","","",""),$dados['usucpf']))."'");
	
	if(!$pesid) {
		
		$sql = "INSERT INTO pdeinterativo2013.pessoa(
	            usucpf, pesnome, pesstatus, pflcod)
	    		VALUES ('".trim(str_replace(array(".","-"," ","/"),array("","","",""),$dados['usucpf']))."', 
	    				'".$dados['pesnome']."', 
	    				'A', 
	    				NULL) RETURNING pesid;";

		$pesid = $db->pegaUm($sql);
		
		if($pesid && $dados['tpeid']) {
			
			$sql = "INSERT INTO pdeinterativo2013.pessoatipoperfil(
            		tpeid, pesid, pdeid)
    				VALUES ('".$dados['tpeid']."', '".$pesid."', '".$_SESSION['pdeinterativo2013_vars']['pdeid']."');";
			
			$db->executar($sql);
		}
		
		$sql = "INSERT INTO pdeinterativo2013.detalhepessoa(
			            pesid, fgtid, tenid, segid, dpetelefone, 
			            dpeemail, dpestatus)
			    VALUES ('".$pesid."', NULL, '".$dados['tenid']."',  
			    		NULL, '".$dados['dpetelefoneddd'].$dados['dpetelefone']."', 
			            '".$dados['dpeemail']."', 'A');";
		
		$db->executar($sql);
		
		if($dados['dirqtdanoexerce']   || 
		   $dados['dirqtdmesesexerce'] || 
		   $dados['dirformescolha']    || 
		   $dados['direscolagestor']   || 
		   $dados['dircursodistan']) {
		   	
			$sql = "INSERT INTO pdeinterativo2013.direcao(
				            pesid, dirqtdanoexerce, dirqtdmesesexerce, dirformescolha, 
				            direscolagestor, dircursodistan, dirstatus)
				    VALUES ('".$pesid."', ".(($dados['dirqtdanoexerce'])?"'".$dados['dirqtdanoexerce']."'":"0").", 
				    		".(($dados['dirqtdmesesexerce'])?"'".$dados['dirqtdmesesexerce']."'":"NULL").", 
				    		".(($dados['dirformescolha'])?"'".$dados['dirformescolha']."'":"NULL").", 
				    		".(($dados['direscolagestor'])?"'".$dados['direscolagestor']."'":"NULL").", 
				    		".(($dados['dircursodistan'])?"'".$dados['dircursodistan']."'":"NULL").", 'A');";
			
			$db->executar($sql);
		   	
	   }
		
		$db->commit();

		if($dados['return_id']) {
			
			return $pesid;
			
		} else {
			
			echo "<script>
					alert('Dados gravados com sucesso');
					window.opener.carregarInfoEquipeGestora_4_1();
					window.close();
				  </script>";
			
		}
		
	} else {
		
		if($dados['return_id']) {
			
			$dados['pesid']=$pesid;
			atualizarPessoa($dados);			
			return $pesid;
			
		} else {
			
			$dados['pesid'] = $pesid;
			atualizarPessoa($dados);
			
		}
		
	}
	
}

function gerenciarDirecao($dados) {
	global $db;
	
	echo "<script language=\"JavaScript\" src=\"../includes/funcoes.js\"></script>";
	echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>";
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	echo "<script type=text/javascript src=/includes/prototype.js></script>";
	echo "<script language=\"javascript\" type=\"text/javascript\" src=\"../includes/webservice/cpf.js\" /></script>";
	
	echo "<script>";
	
	echo "function validarDirecao(){
		  if(document.getElementById('pesnome').value=='') {alert('Preencha um nome');return false;}
		  if(document.getElementById('usucpf').value=='') {alert('Preencha um cpf');return false;}
		  document.getElementById('usucpf').value=mascaraglobal('###.###.###-##',document.getElementById('usucpf').value);
		  if(!validar_cpf(document.getElementById('usucpf').value)) {alert('CPF inválido');return false;}
		  if(document.getElementById('tpeid').value=='') {alert('Selecione um perfil');return false;}
		  if(document.getElementById('dpetelefoneddd').value.length!=2) {alert('Preencha o DDD');return false;}
		  if(document.getElementById('dpetelefone').value.length=='') {alert('Preencha o Telefone');return false;}
		  if(document.getElementById('dpeemail').value.length=='') {alert('Preencha o Email');return false;}
		  if(!validaEmail(document.getElementById('dpeemail').value)) {alert('Email inválido');return false;}
		  if(document.getElementById('tenid').value.length=='') {alert('Preencha a Escolaridade');return false;}
		  document.getElementById('dirqtdanoexerce').value=mascaraglobal('###',document.getElementById('dirqtdanoexerce').value);
		  if(document.getElementById('dirqtdanoexerce').value.length=='') {alert('Preencha o tempo de função');return false;}
		  if(document.getElementById('dirformescolha').value.length=='') {alert('Selecione a forma de escolha');return false;}
		  var escolagestor_marcado = parseInt(jQuery(\"[name^='direscolagestor']:enabled:checked\").length);
		  if(escolagestor_marcado==0){alert('Selecione se Conhece o programa Escola de Gestores?');return false;}
		  var cursodistan_marcado = parseInt(jQuery(\"[name^='dircursodistan']:enabled:checked\").length);
		  if(cursodistan_marcado==0){alert('Selecione se Deseja participar de cursos a distância com ênfase na gestão escolar?');return false;}
		  
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
		  		divCarregado();
				return false;
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
		
		$dadospessoa = $db->pegaLinha("SELECT pes.pesnome, pes.usucpf, ptp.tpeid, 
											  SUBSTR(dpetelefone,1,2) as dpetelefoneddd,
											  SUBSTR(dpetelefone,4,8) as dpetelefone,
											  dpeemail, 
											  dps.tenid,
											  dir.dirqtdanoexerce,
											  dir.dirqtdmesesexerce,
											  dirformescolha,
											  direscolagestor,
											  dircursodistan
									   FROM pdeinterativo2013.pessoa pes 
									   LEFT JOIN pdeinterativo2013.pessoatipoperfil ptp ON ptp.pesid = pes.pesid 
									   LEFT JOIN pdeinterativo2013.direcao dir ON dir.pesid = pes.pesid
									   LEFT JOIN pdeinterativo2013.detalhepessoa dps ON dps.pesid = pes.pesid 
									   WHERE pes.pesid='".$dados['pesid']."'");
		extract($dadospessoa);
		
	} else {
		echo "<input type=hidden name=requisicao value=inserirPessoa>";
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
	echo "<td>".campo_texto('usucpf', "S", (($dados['apeid']==APE_DIRETOR)?"N":"S"), "CPF", 16, 14, "###.###.###-##", "", '', '', 0,'id="usucpf"','',mascaraglobal($usucpf,"###.###.###-##"),'carregaUsuario();')."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Perfil:</td>";
	
	$sql = "SELECT tp.tpeid as codigo, tp.tpedesc as descricao FROM pdeinterativo2013.tipoperfil tp 
			INNER JOIN pdeinterativo2013.perfilarea pa ON pa.tpeid = tp.tpeid 
			WHERE pa.apeid='".$dados['apeid']."'";
	
	echo "<td>".$db->monta_combo('tpeid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'tpeid', true, $tpeid)."</td>";
	echo "</tr>";
	
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Telefone:</td>";
	echo "<td>".campo_texto('dpetelefoneddd', "N", "S", "Telefone", 2, 3, "##", "", '', '', 0,'id="dpetelefoneddd"','',$dpetelefoneddd)." ".campo_texto('dpetelefone', "S", "S", "Telefone", 9, 10, "########", "", '', '', 0,'id="dpetelefone"','',$dpetelefone)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Email:</td>";
	echo "<td>".campo_texto('dpeemail', "S", "S", "Email", 45, 50, "", "", '', '', 0,'id="dpeemail"','',$dpeemail)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Escolaridade:</td>";
	
	$sql = "SELECT tenid as codigo, tendesc as descricao 
			FROM pdeinterativo2013.tipoescolaridade 
			WHERE tenstatus='A'";
	
	echo "<td>".$db->monta_combo('tenid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'tenid', true, $tenid)."</td>";
	
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Tempo que exerce esta função nesta escola:</td>";
	$sql = "SELECT mescod as codigo, mescod as descricao FROM public.meses";
	echo "<td>".campo_texto('dirqtdanoexerce', "N", "S", "Tempo que exerce esta função nesta escola", 2, 3, "###", "", '', '', 0,'id="dirqtdanoexerce"','',$dirqtdanoexerce)." Ano(s) ".$db->monta_combo('dirqtdmesesexerce', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'dirqtdmesesexerce', true, $dirqtdmesesexerce)." Meses</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Forma de escolha:</td>";
	$dados = array(0 => array("codigo"=>"M","descricao"=>"Misto"),
				   1 => array("codigo"=>"O","descricao"=>"Outro"),
				   2 => array("codigo"=>"E","descricao"=>"Eleição"),
				   3 => array("codigo"=>"C","descricao"=>"Concurso"),
				   4 => array("codigo"=>"I","descricao"=>"Indicação")
				   );
	echo "<td>".$db->monta_combo('dirformescolha', $dados, 'S', 'Selecione', '', '', '', '', 'S', 'dirformescolha', true, $dirformescolha)."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Conhece o programa Escola de Gestores?</td>";
	echo "<td><input type=radio name=direscolagestor value=S ".(($direscolagestor=="S")?"checked":"")."> Sim <input type=radio name=direscolagestor value=N ".(($direscolagestor=="N")?"checked":"")."> Não</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" width=\"40%\">Deseja participar de cursos a distância com ênfase na gestão escolar?</td>";
	echo "<td><input type=radio name=dircursodistan value=S ".(($dircursodistan=="S")?"checked":"")."> Sim <input type=radio name=dircursodistan value=N ".(($dircursodistan=="N")?"checked":"")."> Não</td>";
	echo "</tr>";
	
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\" colspan=\"2\"><input type=button name=salvar value=Salvar onclick=validarDirecao();> <input type=button value=Cancelar onclick=window.close();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
}


function diagnostico_4_1_direcao($dados) {
	global $db;
	
	$dados['abacod']="diagnostico_4_1_direcao";
	
	salvarJustificativaEvidencias($dados);
	
	salvarRespostasPorEscola();
	
	if(verificaDadosDirecao()){
		salvarAbaResposta($dados['abacod']);
	}else{
		removerAbaResposta($dados['abacod']);
	}
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
	
}

function verificaDadosDirecao()
{
	global $db;
	
	$flag = $db->pegaLinha("SELECT naoexistevicediretor, naoexistesecretario FROM pdeinterativo2013.flag WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
	
	if($flag['naoexistevicediretor']!="t") {
		
		$c_vice = $db->pegaUm("SELECT count(p.pesid) FROM pdeinterativo2013.pessoa p 
								INNER JOIN pdeinterativo2013.pessoatipoperfil ptp ON ptp.pesid = p.pesid 
								WHERE ptp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND ptp.tpeid='".TPE_VICEDIRETOR."'");
		
		if($c_vice==0) return false;
	}
	
	if($flag['naoexistesecretario']!="t") {
		
		$c_secr = $db->pegaUm("SELECT count(p.pesid) FROM pdeinterativo2013.pessoa p 
								INNER JOIN pdeinterativo2013.pessoatipoperfil ptp ON ptp.pesid = p.pesid 
								WHERE ptp.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND ptp.tpeid='".TPE_SECRETARIO."'");
		
		if($c_secr==0) return false;
	}
	
	return true;

}

function diagnostico_4_2_processos($dados) {
	global $db;
	
	$dados['abacod']="diagnostico_4_2_processos";
	
	salvarJustificativaEvidencias($dados);
	salvarRespostasPorEscola();
	salvarAbaResposta($dados['abacod']);
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
	
}

function carregarInfoEquipeGestora_4_1($dados) {
	global $db;
?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center" id="tbl_direcao">
		<tr>
		<td class="SubTituloCentro" width="5%">&nbsp;</td>
		<td class="SubTituloCentro">Perfil</td>
		<td class="SubTituloCentro">Nome</td>
		<td class="SubTituloCentro">CPF</td>
		<td class="SubTituloCentro">Telefone</td>
		<td class="SubTituloCentro">Email</td>
		<td class="SubTituloCentro">Escolaridade</td>
		<td class="SubTituloCentro">Tempo que exerce esta função nesta escola</td>
		<td class="SubTituloCentro">Forma de escolha</td>
		<td class="SubTituloCentro">Conhece a Escola de Gestores</td>
		<td class="SubTituloCentro" width="15%">Deseja participar de cursos a distância com ênfase na gestão escolar?</td>
		</tr>
		<tr>
		<td width="5%" align="center">&nbsp;</td>
		<td colspan="10" class="SubTituloEsquerda">Diretor(a)</td>
		</tr>
		<? 
		$sql = "SELECT pe.pesid, tp.tpedesc, usu.usunome, pe.usucpf, dp.dpetelefone, dp.dpeemail, te.tendesc, di.dirqtdanoexerce||' ano(s) '||di.dirqtdmesesexerce||' mes(es)' as tempo_escola,
				CASE WHEN dirformescolha='C' THEN 'Concurso' 
					 WHEN dirformescolha='E' THEN 'Eleição' 
					 WHEN dirformescolha='I' THEN 'Indicação' 
					 WHEN dirformescolha='M' THEN 'Misto' 
					 WHEN dirformescolha='O' THEN 'Outro'
					 END as dirformescolha,
				CASE WHEN direscolagestor='S' THEN 'Sim'
					 WHEN direscolagestor='N' THEN 'Não' 
					 END as direscolagestor,
				CASE WHEN dircursodistan='S' THEN 'Sim'
					 WHEN dircursodistan='N' THEN 'Não' 
					 END as dircursodistan
				FROM pdeinterativo2013.pessoa pe
				INNER JOIN seguranca.usuario usu ON usu.usucpf = pe.usucpf 
				LEFT JOIN pdeinterativo2013.pessoatipoperfil ptp ON ptp.pesid = pe.pesid
				LEFT JOIN pdeinterativo2013.detalhepessoa dp ON dp.pesid = pe.pesid 
				LEFT JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = ptp.pdeid 
				LEFT JOIN pdeinterativo2013.tipoperfil tp ON tp.tpeid = ptp.tpeid 
				LEFT JOIN pdeinterativo2013.tipoescolaridade te ON te.tenid = dp.tenid 
				LEFT JOIN pdeinterativo2013.direcao di ON di.pesid = pe.pesid
				WHERE pd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND pe.pflcod = ".PDEESC_PERFIL_DIRETOR." ORDER BY pe.pesnome";
		
		$diretor = $db->pegaLinha($sql);
		
		if($diretor) :
		?>
		<tr>
		<td width="5%" align="center">
			<img src="../imagens/alterar.gif" onclick="gerenciarDirecao('<?=APE_DIRETOR ?>','<?=$diretor['pesid'] ?>');" style="cursor:pointer;">
		</td>
		<td nowrap><?=$diretor['tpedesc'] ?></td>
		<td nowrap><?=$diretor['usunome'] ?></td>
		<td><?=mascaraglobal($diretor['usucpf'],'###.###.###-##') ?></td>
		<td><?=$diretor['dpetelefone'] ?></td>
		<td><?=$diretor['dpeemail'] ?></td>
		<td><?=$diretor['tendesc'] ?></td>
		<td nowrap><?=$diretor['tempo_escola'] ?></td>
		<td><?=$diretor['dirformescolha'] ?></td>
		<td align="center"><?=$diretor['direscolagestor'] ?></td>
		<td align="center"><?=$diretor['dircursodistan'] ?></td>
		</tr>
		<? else: ?>
		<tr>
		<td width="5%">&nbsp;</td>
		<td colspan="10">Diretor(a) não cadastrado(a) nesta escola</td>
		</tr>
		<? endif; ?>
		
		<tr>
		
		<?php $sql = "	select 
							naoexistevicediretor
						from
							pdeinterativo2013.flag
						where
							pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}" ?>
		<?php $naoExisteViceDiretor = $db->pegaUm($sql) ?>
		<?php 

		$sql = "SELECT pe.pesid, tp.tpeid, tp.tpedesc, pe.pesnome, pe.usucpf, dp.dpetelefone, dp.dpeemail, te.tendesc, di.dirqtdanoexerce||' ano(s) '||di.dirqtdmesesexerce||' mes(es)' as tempo_escola,
				CASE WHEN dirformescolha='C' THEN 'Concurso' 
					 WHEN dirformescolha='E' THEN 'Eleição' 
					 WHEN dirformescolha='I' THEN 'Indicação' 
					 WHEN dirformescolha='M' THEN 'Misto' 
					 WHEN dirformescolha='O' THEN 'Outro'
					 END as dirformescolha,
				CASE WHEN direscolagestor='S' THEN 'Sim'
					 WHEN direscolagestor='N' THEN 'Não' 
					 END as direscolagestor,
				CASE WHEN dircursodistan='S' THEN 'Sim'
					 WHEN dircursodistan='N' THEN 'Não' 
					 END as dircursodistan
				FROM pdeinterativo2013.pessoa pe 
				INNER JOIN pdeinterativo2013.pessoatipoperfil pt ON pt.pesid = pe.pesid 
				INNER JOIN pdeinterativo2013.detalhepessoa dp ON dp.pesid = pe.pesid 
				INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = pt.pdeid 
				INNER JOIN pdeinterativo2013.tipoperfil tp ON tp.tpeid = pt.tpeid 
				INNER JOIN pdeinterativo2013.tipoescolaridade te ON te.tenid = dp.tenid 
				INNER JOIN pdeinterativo2013.direcao di ON di.pesid = pe.pesid
				WHERE pd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND tp.tpeid='".TPE_VICEDIRETOR."'";
		
		$vicediretores = $db->carregar($sql);
		
		if($vicediretores[0]){
			$naoExisteViceDiretor = "f";	
		}
		?>
		<td width="5%" align="center"><img src="../imagens/gif_inclui.gif" onclick="gerenciarDirecao('<?=APE_VICEDIRETOR ?>','');" style="cursor:pointer;"></td>
		<td colspan="10" class="SubTituloEsquerda">
			<input type="hidden" name="validarExisteViceDiretor" id="validarExisteViceDiretor" value="<?=(($vicediretores[0])?"1":"0") ?>">
			Vice-Diretor(a)
			<span style="margin-left:20px" > 
				<input type="checkbox" name="chk_naoExisteVice" value="1" onclick="naoExisteViceDiretor()" <?php echo $naoExisteViceDiretor == "t" ? "checked='checked'" : "" ?> />
				Não existe Vice-Diretor
			</span>
			</td>
		</tr>
		<? 
		
		if($vicediretores[0]) :
			foreach($vicediretores as $vicediretor) :
		?>
		<tr>
		<td width="5%" align="center">
			<img src="../imagens/alterar.gif" onclick="gerenciarDirecao('<?=APE_VICEDIRETOR ?>','<?=$vicediretor['pesid'] ?>');" style="cursor:pointer;"> 
			<img src="../imagens/excluir.gif" onclick="excluirPessoa('<?=$vicediretor['pesid'] ?>','<?=$vicediretor['tpeid'] ?>');" style="cursor:pointer;"></td>
		<td nowrap><?=$vicediretor['tpedesc'] ?></td>
		<td nowrap><?=$vicediretor['pesnome'] ?></td>
		<td><?=mascaraglobal($vicediretor['usucpf'],'###.###.###-##') ?></td>
		<td><?=$vicediretor['dpetelefone'] ?></td>
		<td><?=$vicediretor['dpeemail'] ?></td>
		<td><?=$vicediretor['tendesc'] ?></td>
		<td nowrap><?=$vicediretor['tempo_escola'] ?></td>
		<td><?=$vicediretor['dirformescolha'] ?></td>
		<td align="center"><?=$vicediretor['direscolagestor'] ?></td>
		<td align="center"><?=$vicediretor['dircursodistan'] ?></td>
		</tr>
		<?
			endforeach; 
		else: 
		?>
		<tr>
		<td width="5%">&nbsp;</td>
		<td colspan="10">Vice-Diretor(a) não cadastrado(a) nesta escola</td>
		</tr>
		<? 
		endif; 
		?>
		<tr>
		<?php $sql = "	select 
							naoexistesecretario
						from
							pdeinterativo2013.flag
						where
							pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}" ?>
		<?php $naoExisteSecretario = $db->pegaUm($sql) ?>
		<? 
		$sql = "SELECT pe.pesid, tp.tpeid, tp.tpedesc, pe.pesnome, pe.usucpf, dp.dpetelefone, dp.dpeemail, te.tendesc, COALESCE(di.dirqtdanoexerce,0)||' ano(s) '||COALESCE(di.dirqtdmesesexerce,0)||' mes(es)' as tempo_escola,
				CASE WHEN dirformescolha='C' THEN 'Concurso' 
					 WHEN dirformescolha='E' THEN 'Eleição' 
					 WHEN dirformescolha='I' THEN 'Indicação' 
					 WHEN dirformescolha='M' THEN 'Misto' 
					 WHEN dirformescolha='O' THEN 'Outro'
					 END as dirformescolha,
				CASE WHEN direscolagestor='S' THEN 'Sim'
					 WHEN direscolagestor='N' THEN 'Não' 
					 END as direscolagestor,
				CASE WHEN dircursodistan='S' THEN 'Sim'
					 WHEN dircursodistan='N' THEN 'Não' 
					 END as dircursodistan
				FROM pdeinterativo2013.pessoa pe 
				INNER JOIN pdeinterativo2013.pessoatipoperfil pt ON pt.pesid = pe.pesid 
				INNER JOIN pdeinterativo2013.detalhepessoa dp ON dp.pesid = pe.pesid 
				INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = pt.pdeid 
				INNER JOIN pdeinterativo2013.tipoperfil tp ON tp.tpeid = pt.tpeid 
				INNER JOIN pdeinterativo2013.tipoescolaridade te ON te.tenid = dp.tenid 
				INNER JOIN pdeinterativo2013.direcao di ON di.pesid = pe.pesid
				WHERE pd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND tp.tpeid='".TPE_SECRETARIO."'";
		$secretarios = $db->carregar($sql);
		
		if($secretarios[0]){
			$naoExisteSecretario = "f";	
		}
				
		?>
		<td width="5%" align="center"><img src="../imagens/gif_inclui.gif" onclick="gerenciarDirecao('<?=APE_SECRETARIA ?>','');" style="cursor:pointer;"></td>
		<td colspan="10" class="SubTituloEsquerda">
		<input type="hidden" name="validarExisteSecretario" id="validarExisteSecretario" value="<?=(($secretarios[0])?"1":"0") ?>">
		Secretário(a) da escola
			<span style="margin-left:20px" > 
				<input type="checkbox" name="chk_naoExisteSecretario" value="1" onclick="naoExisteSecretario()" <?php echo $naoExisteSecretario == "t" ? "checked='checked'" : "" ?> />
				Não existe Secretário(a) da escola
			</span>
		</td>
		</tr>
		<?
		
		if($secretarios[0]) :
			foreach($secretarios as $secretario) :
		?>
		<tr>
		<td width="5%" align="center">
			<img src="../imagens/alterar.gif" onclick="gerenciarDirecao('<?=APE_SECRETARIA ?>','<?=$secretario['pesid'] ?>');" style="cursor:pointer;"> 
			<img src="../imagens/excluir.gif" onclick="excluirPessoa('<?=$secretario['pesid'] ?>','<?=$secretario['tpeid'] ?>');" style="cursor:pointer;"></td>
		<td nowrap><?=$secretario['tpedesc'] ?></td>
		<td nowrap><?=$secretario['pesnome'] ?></td>
		<td><?=mascaraglobal($secretario['usucpf'],'###.###.###-##') ?></td>
		<td><?=$secretario['dpetelefone'] ?></td>
		<td><?=$secretario['dpeemail'] ?></td>
		<td><?=$secretario['tendesc'] ?></td>
		<td nowrap><?=$secretario['tempo_escola'] ?></td>
		<td><?=$secretario['dirformescolha'] ?></td>
		<td align="center"><?=$secretario['direscolagestor'] ?></td>
		<td align="center"><?=$secretario['dircursodistan'] ?></td>
		</tr>
		<?
			endforeach; 
		else: 
		?>
		<tr>
		<td width="5%">&nbsp;</td>
		<td colspan="10">Secretário(a) da escola não cadastrado(a) nesta escola</td>
		</tr>
		<? 
		endif; 
		?>
		<tr>
		<?
		
		
		$sql = "select 
					naoexisteequipe
				from
					pdeinterativo2013.flag
				where
					pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}";
		
		$naoExisteEquipe = $db->pegaUm($sql);

		$sql = "SELECT pe.pesid, tp.tpeid, tp.tpedesc, pe.pesnome, pe.usucpf, dp.dpetelefone, dp.dpeemail, te.tendesc, COALESCE(di.dirqtdanoexerce,0)||' ano(s) '||COALESCE(di.dirqtdmesesexerce,0)||' mes(es)' as tempo_escola,
				CASE WHEN dirformescolha='C' THEN 'Concurso' 
					 WHEN dirformescolha='E' THEN 'Eleição' 
					 WHEN dirformescolha='I' THEN 'Indicação' 
					 WHEN dirformescolha='M' THEN 'Misto' 
					 WHEN dirformescolha='O' THEN 'Outro'
					 END as dirformescolha,
				CASE WHEN direscolagestor='S' THEN 'Sim'
					 WHEN direscolagestor='N' THEN 'Não' 
					 END as direscolagestor,
				CASE WHEN dircursodistan='S' THEN 'Sim'
					 WHEN dircursodistan='N' THEN 'Não' 
					 END as dircursodistan
				FROM pdeinterativo2013.pessoa pe 
				INNER JOIN pdeinterativo2013.pessoatipoperfil pt ON pt.pesid = pe.pesid 
				INNER JOIN pdeinterativo2013.detalhepessoa dp ON dp.pesid = pe.pesid 
				INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = pt.pdeid 
				INNER JOIN pdeinterativo2013.tipoperfil tp ON tp.tpeid = pt.tpeid 
				INNER JOIN pdeinterativo2013.tipoescolaridade te ON te.tenid = dp.tenid 
				INNER JOIN pdeinterativo2013.direcao di ON di.pesid = pe.pesid
				WHERE pd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND tp.tpeid IN(SELECT tpeid FROM pdeinterativo2013.perfilarea WHERE apeid='".APE_EQUIPEPEDAGOGICA."')";
		
		$equipes = $db->carregar($sql);
		
		if($equipes[0]){
			$naoExisteEquipe = "f";	
		}
		
		?>
		<td width="5%" align="center"><img src="../imagens/gif_inclui.gif" onclick="gerenciarDirecao('<?=APE_EQUIPEPEDAGOGICA ?>','');" style="cursor:pointer;"></td>
		<td colspan="10" class="SubTituloEsquerda">
		<input type="hidden" name="validarExisteEquipePedagogica" id="validarExisteEquipePedagogica" value="<?=(($equipes[0])?"1":"0") ?>">
		Equipe Pedagógica
			<span style="margin-left:20px" > 
				<input type="checkbox" name="chk_naoExisteEquipe" value="1" onclick="naoExisteEquipe()" <?php echo $naoExisteEquipe == "t" ? "checked='checked'" : "" ?> />
				Não existe Equipe Pedagógica
			</span>

		</td>
		</tr>
		<?
		if($equipes[0]) :
			foreach($equipes as $equipe) :
		?>
		<tr>
		<td width="5%" align="center">
			<img src="../imagens/alterar.gif" onclick="gerenciarDirecao('<?=APE_EQUIPEPEDAGOGICA ?>','<?=$equipe['pesid'] ?>');" style="cursor:pointer;"> 
			<img src="../imagens/excluir.gif" onclick="excluirPessoa('<?=$equipe['pesid'] ?>','<?=$equipe['tpeid'] ?>');" style="cursor:pointer;"></td>
		<td nowrap><?=$equipe['tpedesc'] ?></td>
		<td nowrap><?=$equipe['pesnome'] ?></td>
		<td><?=mascaraglobal($equipe['usucpf'],'###.###.###-##') ?></td>
		<td><?=$equipe['dpetelefone'] ?></td>
		<td><?=$equipe['dpeemail'] ?></td>
		<td><?=$equipe['tendesc'] ?></td>
		<td nowrap><?=$equipe['tempo_escola'] ?></td>
		<td><?=$equipe['dirformescolha'] ?></td>
		<td align="center"><?=$equipe['direscolagestor'] ?></td>
		<td align="center"><?=$equipe['dircursodistan'] ?></td>
		</tr>
		<?
			endforeach; 
		else: 
		?>
		<tr>
		<td width="5%">&nbsp;</td>
		<td colspan="10">Equipe Pedagógica não cadastrado(a) nesta escola</td>
		</tr>
		<? 
		endif; 
		?>
	</table>
<?	
	verificaPermissao(PDEESC_PERFIL_DIRETOR);
}

function naoExisteViceDiretor()
{
	global $db;
	
	$sql = "SELECT pe.pesid, tp.tpedesc, pe.pesnome, pe.usucpf, dp.dpetelefone, dp.dpeemail, te.tendesc, di.dirqtdanoexerce||' ano(s) '||di.dirqtdmesesexerce||' mes(es)' as tempo_escola,
				CASE WHEN dirformescolha='C' THEN 'Concurso' 
					 WHEN dirformescolha='E' THEN 'Eleição' 
					 WHEN dirformescolha='I' THEN 'Indicação' 
					 WHEN dirformescolha='M' THEN 'Misto' 
					 WHEN dirformescolha='O' THEN 'Outro'
					 END as dirformescolha,
				CASE WHEN direscolagestor='S' THEN 'Sim'
					 WHEN direscolagestor='N' THEN 'Não' 
					 END as direscolagestor,
				CASE WHEN dircursodistan='S' THEN 'Sim'
					 WHEN dircursodistan='N' THEN 'Não' 
					 END as dircursodistan
				FROM pdeinterativo2013.pessoa pe 
				INNER JOIN pdeinterativo2013.pessoatipoperfil pt ON pe.pesid = pt.pesid
				INNER JOIN pdeinterativo2013.detalhepessoa dp ON dp.pesid = pe.pesid 
				INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = pt.pdeid 
				INNER JOIN pdeinterativo2013.tipoperfil tp ON tp.tpeid = pt.tpeid 
				INNER JOIN pdeinterativo2013.tipoescolaridade te ON te.tenid = dp.tenid 
				INNER JOIN pdeinterativo2013.direcao di ON di.pesid = pe.pesid
				WHERE pd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND tp.tpeid='".TPE_VICEDIRETOR."'";
		
	$vicediretores = $db->carregar($sql);
	
	if($vicediretores){
		echo "Não é possível realizar esta operação pois existe(m) Vice-Diretore(s) cadastrado(s).";
		exit;
	}
	
	$sql = "	select 
							count(*)
						from
							pdeinterativo2013.flag
						where
							pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}";
	$existe = $db->pegaUm($sql);
	
	if($existe){
		$sql = "update pdeinterativo2013.flag set naoexistevicediretor = true where pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}";
	}else{
		$sql = "insert into pdeinterativo2013.flag (naoexistevicediretor,pdeid) values (true,{$_SESSION['pdeinterativo2013_vars']['pdeid']});";
	}
	$db->executar($sql);
	$db->commit();
	
}

function naoExisteEquipe()
{
	global $db;
	
	$sql = "SELECT pe.pesid, tp.tpedesc, pe.pesnome, pe.usucpf, dp.dpetelefone, dp.dpeemail, te.tendesc, di.dirqtdanoexerce||' ano(s) '||di.dirqtdmesesexerce||' mes(es)' as tempo_escola,
			CASE WHEN dirformescolha='C' THEN 'Concurso' 
				 WHEN dirformescolha='E' THEN 'Eleição' 
				 WHEN dirformescolha='I' THEN 'Indicação' 
				 WHEN dirformescolha='M' THEN 'Misto' 
				 WHEN dirformescolha='O' THEN 'Outro'
				 END as dirformescolha,
			CASE WHEN direscolagestor='S' THEN 'Sim'
				 WHEN direscolagestor='N' THEN 'Não' 
				 END as direscolagestor,
			CASE WHEN dircursodistan='S' THEN 'Sim'
				 WHEN dircursodistan='N' THEN 'Não' 
				 END as dircursodistan
			FROM pdeinterativo2013.pessoa pe 
			INNER JOIN pdeinterativo2013.pessoatipoperfil pt ON pt.pesid = pe.pesid 
			INNER JOIN pdeinterativo2013.detalhepessoa dp ON dp.pesid = pe.pesid 
			INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = pt.pdeid 
			INNER JOIN pdeinterativo2013.tipoperfil tp ON tp.tpeid = pt.tpeid 
			INNER JOIN pdeinterativo2013.tipoescolaridade te ON te.tenid = dp.tenid 
			INNER JOIN pdeinterativo2013.direcao di ON di.pesid = pe.pesid
			WHERE pd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND tp.tpeid IN(SELECT tpeid FROM pdeinterativo2013.perfilarea WHERE apeid='".APE_EQUIPEPEDAGOGICA."')";
	
	$equipes = $db->carregar($sql);
	
	
	if($equipes[0]){
		echo "Não é possível realizar esta operação pois existe Equipe Pedagógica cadastrada.";
		exit;
	}
	
	$sql = "	select 
							count(*)
						from
							pdeinterativo2013.flag
						where
							pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}";
	$existe = $db->pegaUm($sql);
	
	if($existe){
		$sql = "update pdeinterativo2013.flag set naoexisteequipe = true where pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}";
	}else{
		$sql = "insert into pdeinterativo2013.flag (naoexisteequipe,pdeid) values (true,{$_SESSION['pdeinterativo2013_vars']['pdeid']});";
	}
	$db->executar($sql);
	$db->commit();
	
}

function naoExisteSecretario()
{
	global $db;
	
	$sql = "SELECT pe.pesid, tp.tpedesc, pe.pesnome, pe.usucpf, dp.dpetelefone, dp.dpeemail, te.tendesc, di.dirqtdanoexerce||' ano(s) '||di.dirqtdmesesexerce||' mes(es)' as tempo_escola,
				CASE WHEN dirformescolha='C' THEN 'Concurso' 
					 WHEN dirformescolha='E' THEN 'Eleição' 
					 WHEN dirformescolha='I' THEN 'Indicação' 
					 WHEN dirformescolha='M' THEN 'Misto' 
					 WHEN dirformescolha='O' THEN 'Outro'
					 END as dirformescolha,
				CASE WHEN direscolagestor='S' THEN 'Sim'
					 WHEN direscolagestor='N' THEN 'Não' 
					 END as direscolagestor,
				CASE WHEN dircursodistan='S' THEN 'Sim'
					 WHEN dircursodistan='N' THEN 'Não' 
					 END as dircursodistan
				FROM pdeinterativo2013.pessoa pe 
				INNER JOIN pdeinterativo2013.pessoatipoperfil pt ON pe.pesid = pt.pesid
				INNER JOIN pdeinterativo2013.detalhepessoa dp ON dp.pesid = pe.pesid 
				INNER JOIN pdeinterativo2013.pdinterativo pd ON pd.pdeid = pt.pdeid 
				INNER JOIN pdeinterativo2013.tipoperfil tp ON tp.tpeid = pt.tpeid 
				INNER JOIN pdeinterativo2013.tipoescolaridade te ON te.tenid = dp.tenid 
				INNER JOIN pdeinterativo2013.direcao di ON di.pesid = pe.pesid
				WHERE pd.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND tp.tpeid='".TPE_SECRETARIO."'";
		
	$secretarios = $db->carregar($sql);
	
	if($secretarios){
		echo "Não é possível realizar esta operação pois existe(m) Secretário(s) cadastrado(s).";
		exit;
	}
	
	$sql = "	select 
							count(*)
						from
							pdeinterativo2013.flag
						where
							pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}";
	$existe = $db->pegaUm($sql);
	
	if($existe){
		$sql = "update pdeinterativo2013.flag set naoexistesecretario = true where pdeid = {$_SESSION['pdeinterativo2013_vars']['pdeid']}";
	}else{
		$sql = "insert into pdeinterativo2013.flag (naoexistesecretario,pdeid) values (true,{$_SESSION['pdeinterativo2013_vars']['pdeid']});";
	}
	$db->executar($sql);
	$db->commit();
	
}


function gerenciarProgramasFinancas($dados) {
	global $db;
	monta_titulo( 'Incluir programa', '&nbsp;' );
	if($dados['fprid']) {
		$programasfinancas = $db->pegaLinha("SELECT * FROM pdeinterativo2013.respostafinancaprograma WHERE fprid='".$dados['fprid']."'");
		$requisicao = "atualizarProgramasFinancas";
		extract($programasfinancas);
		$dadostd['fprprograma'] = (($fprprograma)?$fprprograma:"");
		$dadostd['fproutra'] = (($fproutra)?$fproutra:"");
		$dadostd['proid'] = (($proid)?$proid:"");
		$dadostd['fprfonte'] = (($fprfonte)?$fprfonte:"");
	} else {
		$requisicao = "inserirProgramasFinancas";
	}
?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<script>
	function gerenciarProgramasFinancas() {
	
		if(jQuery('#fprfonte').val() == '') {
			alert('Selecione uma fonte');
			return false;
		}
		
		if(jQuery('#fprvalorrep').val() != '') {
			jQuery('#fprvalorrep').val(mascaraglobal('###.###.###.###,##',jQuery('#fprvalorrep').val()));
		}

		if(jQuery('#proid')) {
			if(jQuery('#proid').val() == '') {
				alert('Selecione um programa');
				return false;
			}
		}
		
		if(jQuery('#fprprograma')) {
			if(jQuery('#fprprograma').val() == '') {
				alert('Preencha o programa');
				return false;
			}
		}
		
		if(jQuery('#fproutra')) {
			if(jQuery('#fproutra').val() == '') {
				alert('Selecione um programa');
				return false;
			}
		}
		
		jQuery('#formulario').submit();
		
	}
	
	function selecionaFonte(fprfonte) {
		jQuery('#programa_td').html('Carregando...');
		
		jQuery.ajax({
	   		type: "POST",
	   		url: "pdeinterativo2013.php?modulo=principal/diagnostico&acao=A",
	   		data: "requisicao=pegarTdProgramaFinanca&fprfonte="+fprfonte,
	   		async: false,
	   		success: function(msg){jQuery('#programa_td').html(msg);}
	 		});

	}
	</script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<form name="formulario" id="formulario" method="post">
	<input type="hidden" name="requisicao" value="<?=$requisicao ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
		<td class="SubTituloDireita">Fonte:</td>
		<td>
		<?
		$dadosfonte = array(0 => array("codigo" => "E","descricao" => "Estadual"),
							1 => array("codigo" => "F","descricao" => "Federal"),
							2 => array("codigo" => "M","descricao" => "Municipal"),
							3 => array("codigo" => "O","descricao" => "Outras"));
							 
		$db->monta_combo('fprfonte', $dadosfonte, 'S', 'Selecione', 'selecionaFonte', '', '', '200', 'S', 'fprfonte', false, $fprfonte); 
		?>
		</td>
		</tr>
		<tr>
		<td class="SubTituloDireita">Programa:</td>
		<td id="programa_td"><?=pegarTdProgramaFinanca($dadostd) ?></td>
		</tr>
		<tr>
		<td class="SubTituloDireita">Valor (R$) repassado para escola:</td>
		<td><?=campo_texto('fprvalorrep', "S", "S", "Valor (R$) repassado para escola", 21, 50, "###.###.###.###,##", "", '', '', 0, 'id="fprvalorrep"', '', number_format($fprvalorrep,2,',','.') ); ?></td>
		</tr>
		<tr>
		<td class="SubTituloCentro" colspan="2"><input type="button" name="salvar" value="Salvar" onclick="gerenciarProgramasFinancas();"> <input type="button" name="cancelar" value="Cancelar" onclick="window.close();"></td>
		</tr>
	</table>
	</form>
<?
}

function pegarTdProgramaFinanca($dados) {
	global $db;
	
	switch($dados['fprfonte']) {
		case 'F':
			$sql = "SELECT proid as codigo, prodesc as descricao FROM pdeinterativo2013.programa WHERE prostatus='A' and prorepassarec = 'S' order by prodesc";
			$db->monta_combo('proid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'proid', $dados['proid']);
			break;
		case 'E':
		case 'M':
			echo campo_texto('fprprograma', "S", "S", "Nome do programa", 36, 150, "", "", '', '', 0, 'id="fprprograma"', '',  $dados['fprprograma'] );
			break;
		case 'O':
			$dadosoutras = array(0 => array("codigo"=>"C","descricao"=>"Campanha"),
								 1 => array("codigo"=>"O","descricao"=>"Contribuição"));
								 
			$db->monta_combo('fproutra', $dadosoutras, 'S', 'Selecione', '', '', '', '200', 'S', 'fproutra', '', $dados['fproutra']);
			break;
		default:
			echo "Selecione a fonte";
			
	}
}

function excluirProgramasFinancas($dados) {
	global $db;
	
	$sql = "UPDATE pdeinterativo2013.respostafinancaprograma SET fprstatus='I' WHERE fprid='".$dados['fprid']."'";
	$db->executar($sql);
	$db->commit();
	
	$sql = "SELECT COUNT(*) as num FROM pdeinterativo2013.respostafinancaprograma WHERE fprid='".$dados['fprid']."' AND fprstatus='A'";
	$num = $db->pegaUm($sql);
	
	echo $num.":Programa removido com sucesso";
}


function atualizarProgramasFinancas($dados) {
	global $db;
	
	$sql = "UPDATE pdeinterativo2013.respostafinancaprograma
   			SET fprfonte='".$dados['fprfonte']."', 
   				fprprograma=".(($dados['fprprograma'])?"'".$dados['fprprograma']."'":"NULL").", 
   				fproutra=".(($dados['fproutra'])?"'".$dados['fproutra']."'":"NULL").", 
       			fprvalorrep=".(($dados['fprvalorrep'])?"'".str_replace(array(".",","),array("","."),$dados['fprvalorrep'])."'":"NULL").", 
       			proid=".(($dados['proid'])?"'".$dados['proid']."'":"NULL")."
 			WHERE fprid='".$dados['fprid']."'";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Programa atualizado com sucesso');
			window.opener.carregarProgramasFinancas();
			window.opener.carregarDiferencaFontesUsos();
			window.opener.carregarResumoProgramasFinancas();
			window.opener.carregarImagemGrafico();
			window.opener.document.getElementById('td_fontesprogramas_grafico').style.display='';
			window.opener.carregarUsosFinancas();
			window.close();
		  </script>";
}

function inserirProgramasFinancas($dados) {
	global $db;
	
	if(!$dados['fprfonte']) die("<script>alert('Fonte em branco. Tente novamente.');window.close();</script>");
	$dados['fprvalorrep'] = str_replace(array(".",","," "),array("",".",""),$dados['fprvalorrep']);
	$sql = "INSERT INTO pdeinterativo2013.respostafinancaprograma(
            pdeid, fprfonte, fprprograma, fproutra, fprvalorrep, 
            fprstatus, proid)
    VALUES ('".$_SESSION['pdeinterativo2013_vars']['pdeid']."', 
    		'".$dados['fprfonte']."', 
    		".(($dados['fprprograma'])?"'".$dados['fprprograma']."'":"NULL").", 
    		".(($dados['fproutra'])?"'".$dados['fproutra']."'":"NULL").", 
    		".((is_numeric($dados['fprvalorrep']))?"'".$dados['fprvalorrep']."'":"NULL").", 
            'A', 
            ".(($dados['proid'])?"'".$dados['proid']."'":"NULL").");";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
			alert('Programa inserido com sucesso');
			window.opener.carregarProgramasFinancas();
			window.opener.carregarDiferencaFontesUsos();
			window.opener.carregarResumoProgramasFinancas();
			window.opener.carregarImagemGrafico();
			window.opener.document.getElementById('td_fontesprogramas_grafico').style.display='';
			window.opener.carregarUsosFinancas();
			window.close();
		  </script>";
}

function carregarProgramasFinancas($dados) {
	global $db;
	
	$sql = "SELECT '<center><img src=../imagens/alterar.gif onclick=\"gerenciarProgramasFinancas(\''||rfp.fprid||'\');\" style=cursor:pointer;> <img src=../imagens/excluir.gif onclick=\"excluirProgramasFinancas(\''||rfp.fprid||'\');\" style=cursor:pointer;></center>' as acoes, 
				   CASE WHEN fprfonte='E' THEN 'Estadual'
				   		WHEN fprfonte='F' THEN 'Federal' 
				   		WHEN fprfonte='M' THEN 'Municipal'
				   		WHEN fprfonte='O' THEN 'Outras' END as fonte, 
				   CASE WHEN fprprograma IS NOT NULL THEN fprprograma
				   		WHEN fproutra    IS NOT NULL THEN CASE WHEN fproutra='C' THEN 'Campanha'
				   											   WHEN fproutra='O' THEN 'Contribuição' END
				   		WHEN prodesc     IS NOT NULL THEN prodesc END as programa,
				   fprvalorrep									   
			FROM pdeinterativo2013.respostafinancaprograma rfp
			LEFT JOIN pdeinterativo2013.programa pro ON pro.proid = rfp.proid  
			WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND fprstatus='A'";
	
	if($dados['existeRegistros']) {
		$existe = $db->pegaUm($sql);
		return (($existe)?TRUE:FALSE);
	}
	
	$cabecalho = array("&nbsp;","Fonte","Programa","Total(R$)");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'S','100%','S');
}

function pegarTotalProgramasFinancas($pdeid) {
	global $db;
	$total = $db->pegaUm("SELECT SUM(fprvalorrep) as fprvalorrep FROM pdeinterativo2013.respostafinancaprograma 
			  			  WHERE pdeid='".$pdeid."' AND fprstatus='A'");
	return $total;
}

function carregarResumoProgramasFinancas($dados) {
	global $db;
	$total = pegarTotalProgramasFinancas($_SESSION['pdeinterativo2013_vars']['pdeid']);
	
	$sql = "SELECT CASE WHEN fprfonte='E' THEN 'Estadual'
				   		WHEN fprfonte='F' THEN 'Federal' 
				   		WHEN fprfonte='M' THEN 'Municipal'
				   		WHEN fprfonte='O' THEN 'Outras' END as fonte, 
				   SUM(fprvalorrep) as fprvalorrep,
				   ".(($total > 0)?"(SUM(fprvalorrep)/".$total."*100)":"'0'")." as porcent								   
			FROM pdeinterativo2013.respostafinancaprograma 
			WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND fprstatus='A'
			GROUP BY fprfonte";
	
	$cabecalho = array("Fonte","Total(R$)", "%");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'S','100%','S');
	
}

function montaGraficoFinancas($dados) {
	global $db;
	
	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_pie.php');
	
	switch($dados['tipo']) {
		case 'usos':
			$sql = "SELECT SUM(fuscapital) as capital, SUM(fuscusteio) as custeio 
					FROM pdeinterativo2013.respostafinancauso 
					WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND fusstatus='A'";
			$usos = $db->pegaLinha($sql);
			if($usos['capital']) {
				$data[] = $usos['capital'];
				$legendas[] = 'Capital';
			}
			if($usos['custeio']) {
				$data[] = $usos['custeio'];
				$legendas[] = 'Custeio';
			}
			$titulo="Usos - Natureza das despesas";
			break;
		case 'fontesprogramas':
			$sql = "SELECT CASE WHEN fprfonte='E' THEN 'Estadual'
						   		WHEN fprfonte='F' THEN 'Federal' 
						   		WHEN fprfonte='M' THEN 'Municipal'
						   		WHEN fprfonte='O' THEN 'Outras' END as fonte, 
						   SUM(fprvalorrep) as fprvalorrep								   
					FROM pdeinterativo2013.respostafinancaprograma 
					WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND fprstatus='A' 
					GROUP BY fprfonte";
			
			$datas = $db->carregar($sql);
			
			if($datas[0]) {
				foreach($datas as $d) {
					$data[] 	= $d['fprvalorrep'];
					$legendas[] = $d['fonte'];
				}
			} else {
				$data 	  = array();
				$legendas = array();
			}
			
			$titulo="Resumo - Totais";
			break;
	}
	
	// Create the Pie Graph. 
	$graph = new PieGraph(350,250);
	// Set A title for the plot
	$graph->title->Set($titulo);
	// Create
	if($data) {
		$p1 = new PiePlot($data);
		$graph->Add($p1);
		$p1->SetCenter(0.35,0.5);	
		$p1->ShowBorder();
		$p1->SetColor('black');
		$p1->SetLegends($legendas); 
		$p1->SetSliceColors(array('#1E90FF','#2E8B57','#ADFF2F','#DC143C','#BA55D3'));
	}
	$graph->Stroke();
	
}

function carregarDiferencaFontesUsos($dados) {
	global $db;
	$sql = "SELECT COALESCE(SUM(fustotal),0.00) as total  
			FROM pdeinterativo2013.financacategoria fc 
			INNER JOIN pdeinterativo2013.respostafinancauso rf ON rf.fcaid = fc.fcaid AND rf.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' 
			WHERE fc.fcastatus='A'";
	
	$total = $db->pegaUm($sql);
	
	$totalprogramas = pegarTotalProgramasFinancas($_SESSION['pdeinterativo2013_vars']['pdeid']);
	$totalprogramas = (($totalprogramas)?$totalprogramas:0.00);
	if($totalprogramas>=$total) {
		$saldoexe = "<b>".number_format(($totalprogramas-$total),2,",",".")."</b>";
	} else {
		$saldoexe = "<font color=red><b>-".number_format(($total-$totalprogramas),2,",",".")."</b></font>";
	}
	echo "<table class=listagem width=100%>";
	echo "<tr>";
	echo "<td class=SubTituloEsquerda>Saldo do exercício (recursos recebidos - recursos gastos)(R$)</td>";
	echo "<td align=right>".$saldoexe."</td>";
	echo "</tr>";
	echo "</table>";
	
}

function carregarUsosFinancas($dados) {
	global $db;
	
	$sql = "SELECT 'Total', COALESCE(SUM(rf.fuscusteio),0.00) as custeio, COALESCE(SUM(rf.fuscapital),0.00) as capital,
				   COALESCE(SUM(fustotal),0.00) as total, '100' as porcent 
			FROM pdeinterativo2013.financacategoria fc 
			INNER JOIN pdeinterativo2013.respostafinancauso rf ON rf.fcaid = fc.fcaid AND rf.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' 
			WHERE fc.fcastatus='A'";
	
	$listatotal = $db->pegaLinha($sql);
	
	if($dados['existeRegistros']) {
		return (($listatotal)?TRUE:FALSE);
	}
	
	$sql = "SELECT fc.fcadesc, '<input onblur=\"gerenciarUsos();\" value=\"'|| COALESCE(trim(to_char(rf.fuscusteio,'999g999g999d99')),'0,00') ||'\" '|| CASE WHEN fcabloqueado='C' THEN 'disabled class=disabled' ELSE 'class=normal' END ||' name=fuscusteio['||fc.fcaid||'] type=text style=text-align:right; size=18 onKeyUp=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\">' as custeio, '<input onblur=\"gerenciarUsos();\" value=\"'|| COALESCE(trim(to_char(rf.fuscapital,'999g999g999d99')),'0,00') ||'\" '|| CASE WHEN fcabloqueado='P' THEN 'disabled class=disabled' ELSE 'class=normal' END ||' name=fuscapital['||fc.fcaid||'] type=text class=normal style=text-align:right; size=18 onKeyUp=\"this.value=mascaraglobal(\'###.###.###,##\',this.value);\">' as capital,
				   COALESCE(fustotal,0.00) as total, ".(($listatotal['total'] > 0)?"(fustotal/".$listatotal['total']."*100)":"0")." as porcent 
			FROM pdeinterativo2013.financacategoria fc 
			LEFT JOIN pdeinterativo2013.respostafinancauso rf ON rf.fcaid = fc.fcaid AND rf.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' 
			WHERE fc.fcastatus='A' ORDER BY fc.fcadesc";
		
	$lista = $db->carregar($sql);
	
	$lista[] = array("<p align=right><b>Total:</b></p>",$listatotal['custeio'],$listatotal['capital'],$listatotal['total'],$listatotal['porcent']);

	$cabecalho = array("Categoria das despesas","Custeio(R$)","Capital(R$)","Total(R$)","em(%)");
	$db->monta_lista_simples($lista,$cabecalho,50,5,'N','100%','S','');

}

function gerenciarUsos($dados) {
	global $db;
	
	if($dados['fuscusteio']) {
		foreach($dados['fuscusteio'] as $fcaid => $custeio) {
			if($custeio) {
				$sql = "SELECT fusid FROM pdeinterativo2013.respostafinancauso WHERE fcaid='".$fcaid."' AND pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
				$fusid = $db->pegaUm($sql);
				if($fusid) $sql = "UPDATE pdeinterativo2013.respostafinancauso SET fuscusteio=".((str_replace(array(".",","),array("","."),$custeio) > 0 && is_numeric(str_replace(array(".",","),array("","."),$custeio)))?"'".str_replace(array(".",","),array("","."),$custeio)."'":"NULL")." WHERE fusid='".$fusid."';";
				else $sql = "INSERT INTO pdeinterativo2013.respostafinancauso(pdeid, fuscusteio, fusstatus, fcaid) VALUES ('".$_SESSION['pdeinterativo2013_vars']['pdeid']."', ".((str_replace(array(".",","),array("","."),$custeio) > 0 && is_numeric(str_replace(array(".",","),array("","."),$custeio)))?"'".str_replace(array(".",","),array("","."),$custeio)."'":"NULL").", 'A', '".$fcaid."');";
				$db->executar($sql);
				$db->commit();
			}
		}
	}
	
	if($dados['fuscapital']) {
		foreach($dados['fuscapital'] as $fcaid => $capital) {
			if($capital) {
				$sql = "SELECT fusid FROM pdeinterativo2013.respostafinancauso WHERE fcaid='".$fcaid."' AND pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
				$fusid = $db->pegaUm($sql);
				if($fusid) $sql = "UPDATE pdeinterativo2013.respostafinancauso SET fuscapital=".((str_replace(array(".",","),array("","."),$capital) > 0 && is_numeric(str_replace(array(".",","),array("","."),$capital)))?"'".str_replace(array(".",","),array("","."),$capital)."'":"NULL")." WHERE fusid='".$fusid."';";
				else $sql = "INSERT INTO pdeinterativo2013.respostafinancauso(pdeid, fuscapital, fusstatus, fcaid) VALUES ('".$_SESSION['pdeinterativo2013_vars']['pdeid']."', ".((str_replace(array(".",","),array("","."),$capital) > 0 && is_numeric(str_replace(array(".",","),array("","."),$capital)))?"'".str_replace(array(".",","),array("","."),$capital)."'":"NULL").", 'A', '".$fcaid."');";
				$db->executar($sql);
				$db->commit();
			}
		}
	}
	
	$db->executar("UPDATE pdeinterativo2013.respostafinancauso SET fustotal=(coalesce(fuscapital,0.00)+coalesce(fuscusteio,0.00)) WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
	$db->commit();
	
	echo "Atualizados com sucesso";

}


function montarGraficoBarraFinancas($dados) {
	global $db;

	require_once ('../../includes/jpgraph/jpgraph.php');
	require_once ('../../includes/jpgraph/jpgraph_bar.php');
	
	$sql = "SELECT fustotal, SUBSTR(fcadesc,1,10) as fcadesc 
			FROM pdeinterativo2013.respostafinancauso u
			INNER JOIN pdeinterativo2013.financacategoria c ON c.fcaid = u.fcaid 
			WHERE u.pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND u.fusstatus='A' 
			ORDER BY fcadesc";
	
	$categorias = $db->carregar($sql);
	if($categorias[0]) {
		foreach($categorias as $cat) {
			$datay[]    = $cat['fustotal'];
			$legendas[] = $cat['fcadesc']; 
		}
	} else {
		$datay[] = 0;
		$legendas[] = "Em branco";
	}
	
	// Create the graph. These two calls are always required
	$graph = new Graph(500,250,'auto');
	$graph->SetScale("textlin");
	$graph->xaxis->SetFont(FF_FONT0,FS_NORMAL,7);	
	$graph->xaxis->SetTickLabels($legendas);

	// Create the bar plots
	$b1plot = new BarPlot($datay);
	// ...and add it to the graPH
	$graph->Add($b1plot);
	$b1plot->SetColor("white");
	$b1plot->SetFillColor("#1111cc");
	$b1plot->value->Show();
	$b1plot->SetWidth(20);
	$b1plot->value->SetFont(FF_FONT0,FS_NORMAL,7);
	$graph->title->Set("Usos - Destinação dos recursos");
	// Display the graph
	$graph->Stroke();
	
}

function diagnostico_4_3_financas($dados) {
	global $db;
	
	salvarRespostasPorEscola();
	
	$recebeurecursosanoanterior = $db->pegaUm("SELECT recebeurecursosanoanterior FROM pdeinterativo2013.flag WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
	if($recebeurecursosanoanterior=="t") {
		// verificando se não existe alguma das opções
		$fprid = $db->pegaUm("SELECT fprid FROM pdeinterativo2013.respostafinancaprograma WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND fprstatus='A'");
		$fusid = $db->pegaUm("SELECT fusid FROM pdeinterativo2013.respostafinancauso WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."' AND fustotal>0");
		
		if(!$fprid || !$fusid) {
			  die("<script>
					alert('Existem informações incompletas. Adicione um programa e/ou Preencha a categoria');
					window.location=window.location;
				  </script>");
		}
	}

	
	salvarAbaResposta("diagnostico_4_3_financas");
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
}


function diagnostico_4_4_sintesedimensao4($dados,$salvaAba=true) {
	global $db;
	if($dados['critico']) {
		foreach($dados['critico'] as $indice => $valor) {
			$sql = "UPDATE pdeinterativo2013.respostapergunta SET critico=".$valor." WHERE repid='".$indice."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($dados['pessoas']) {
		foreach($dados['pessoas'] as $indice => $valor) {
			$pesids = explode(",",$indice);
			foreach($pesids as $pesid) {
				$sql = "UPDATE pdeinterativo2013.pessoa SET critico=".$valor." WHERE pesid='".$pesid."'";
				$db->executar($sql);
				$db->commit();
			}
		}
	}
	
	if($salvaAba){
		salvarAbaResposta("diagnostico_4_4_sintesedimensao4");
		$db->commit();
		echo "<script>
				alert('Dados gravados com sucesso');
				window.location='".$dados['togo']."';
			  </script>";
	}
	
}

function gravarRecebimentoRecursosAnoAnterior($dados) {
	global $db;
	
	$sql = "SELECT flaid FROM pdeinterativo2013.flag WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
	$flaid = $db->pegaUm($sql);
	
	if($flaid) {
		$db->executar("UPDATE pdeinterativo2013.flag SET recebeurecursosanoanterior=".(($dados['recebeurecursosanoanterior'])?$dados['recebeurecursosanoanterior']:"NULL")." WHERE flaid='".$flaid."'");
	} else {
		$db->executar("INSERT INTO pdeinterativo2013.flag(pdeid, recebeurecursosanoanterior)
    				   VALUES ('".$_SESSION['pdeinterativo2013_vars']['pdeid']."', ".(($dados['recebeurecursosanoanterior'])?$dados['recebeurecursosanoanterior']:"NULL").")");
	}
	$db->commit();
	
	if($dados['recebeurecursosanoanterior']=="FALSE") {
		$db->executar("UPDATE pdeinterativo2013.respostafinancaprograma SET fprstatus='I' WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'");
	}
	$db->commit();
	
}


?>