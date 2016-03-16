<?

function carregarEncaminhamentos($dados) {
	header('Content-Type: text/html; charset=iso-8859-1');
	global $db;
	$sql = "SELECT 
				'<center><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"removerEncaminhamento('||enc.ecmid||')\"> <img src=\"../imagens/anexo.gif\" style=\"cursor:pointer;\" onclick=\"abrirAnexoEncaminhamento('||pee.pecid||')\"></center>' as acao, 
				enc.ecmdescricao, 
				pes.pesnome,
				to_char(enc.ecmprazo,'DD/MM/YYYY') as prazo, 
				'<center><input type=\"checkbox\" name=\"pevencaminhamento['||pecid||']\" value=\"C\" '||CASE WHEN pecstatus='C' THEN 'checked' ELSE '' END||'></center>' 
			FROM atendimento.encaminhamento enc
			INNER JOIN atendimento.pevencaminhamento pee ON pee.ecmid = enc.ecmid
			INNER JOIN atendimento.pessoaevento      pev ON pev.pevid = pee.pevid
			INNER JOIN atendimento.pessoa      		 pes ON pes.pesid = pev.pesid
			WHERE pev.evtid='".$dados['evtid']."' AND enc.ecmstatus='A' ORDER BY enc.ecmid";
	$cabecalho = array("","Encaminhamento","Nome","Prazo","Concluído");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}

function inserirEncaminhamentoPorPessoa($dados) {
	global $db;
	$data = formata_data_sql($dados['ecmprazo']);
	$arrValData = explode('-', $data);
	
	if( $arrValData[0] == '' || $arrValData[1] == '' || $arrValData[2] == '' ){
		echo 'data';
		exit;
	}

	$sql = "SELECT * FROM atendimento.pessoaevento WHERE pevid='".$dados['pevid']."'";
	$pessoaevento = $db->pegaLinha($sql);

	$sql = "INSERT INTO atendimento.encaminhamento(
	evtid, ecmdescricao, ecmstatus, ecmprazo)
	VALUES (NULL, '".iconv("UTF-8", "ISO-8859-1", $dados['ecmdescricao'])."', 'A', '".$data."') RETURNING ecmid;";

	$ecmid = $db->pegaUm($sql);

	$sql = "INSERT INTO atendimento.pevencaminhamento(
	pevid, ecmid, pecstatus)
	VALUES ('".$dados['pevid']."', '".$ecmid."', 'A');";

	$db->executar($sql);
	$db->commit();
}


function salvarParticipantes($dados) {
	global $db;	
	if($dados['pesid']) {
		foreach($dados['pesid'] as $pevid => $pesid) {

			if($pesid) {
				$sql = "UPDATE atendimento.pessoa SET 
							pesnome='".iconv("UTF-8", "ISO-8859-1", $dados['pesnome'][$pevid])."',
							pesemail=".utf8_decode(($dados['pesemail'][$pevid])?"'".$dados['pesemail'][$pevid]."'":"NULL").",
							pestelefone=".(($dados['pestelefone'][$pevid])?"'".$dados['pestelefone'][$pevid]."'":"NULL").",
							pescpf=".(($dados['pescpf'][$pevid])?"'".$dados['pescpf'][$pevid]."'":"NULL").",
							pesstatus='A' 
						WHERE 
							pesid='".$pesid."'";

				$db->executar($sql);

				$sql = "UPDATE atendimento.pessoaevento SET 
							pesid='".$pesid."', 
							pevcargo='".iconv("UTF-8", "ISO-8859-1", $dados['pevcargo'][$pevid])."',
							pevstatus='A', 
							estuf = '".$dados['estuf'][$pevid]."', 
							muncod = '".$dados['muncod'][$pevid]."', 
							pevorgao = '".$dados['pevorgao'][$pevid]."' 
						WHERE 
							pevid='".$pevid."'";
				$db->executar($sql);

			} else {
					$sqli = "INSERT INTO atendimento.pessoa(
					pesnome, pesemail, pestelefone, pescpf, pesstatus)
					VALUES ('".iconv("UTF-8", "ISO-8859-1", $dados['pesnome'][$pevid])."',
					".(($dados['pesemail'][$pevid])?"'".$dados['pesemail'][$pevid]."'":"NULL").",
					".(($dados['pestelefone'][$pevid])?"'".$dados['pestelefone'][$pevid]."'":"NULL").",
					".(($dados['pescpf'][$pevid])?"'".$dados['pescpf'][$pevid]."'":"NULL").",
					'A') RETURNING pesid;";
						
					$pesid = $db->pegaUm($sqli);
						
					$sql = "UPDATE atendimento.pessoaevento SET 
								pevcargo='".iconv("UTF-8", "ISO-8859-1", $dados['pevcargo'][$pevid])."', 
								pesid='".$pesid."', 
								pevstatus='A', 
								estuf = '".$dados['estuf'][$pevid]."', 
								muncod = '".$dados['muncod'][$pevid]."', 
								pevorgao = '".$dados['pevorgao'][$pevid]."'
							WHERE pevid='".iconv("UTF-8", "ISO-8859-1", $pevid)."'";
					$db->executar($sql);
			}

		}
	}

	$db->commit();
}

function excluirAgendamento($dados) {
	global $db;
	$sql = "UPDATE atendimento.anexoencaminhamento SET ancstatus='I' WHERE ancid='".substr($dados['ancid'])."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Tem certeza que deseja excluir?');alert('Anexo excluído com sucesso');window.location='atendimento.php?modulo=principal/eventoeventoanexo&acao=A&requisicao=telaInserirAnexoEncaminhamento&pecid=".$dados['pecid']."';</script>";
}

function excluirPessoaEvento($dados) {
	global $db;
	$sql = "UPDATE atendimento.pessoaevento SET pevstatus='I' WHERE pevid='".$dados['pevid']."'";
	$db->executar($sql);
	$db->commit();
}

function excluirEncaminhamento($dados) {
	global $db;
	$sql = "UPDATE atendimento.pevencaminhamento SET pecstatus='I' WHERE ecmid='".$dados['ecmid']."'";
	$db->executar($sql);
	$sql = "UPDATE atendimento.encaminhamento SET ecmstatus='I' WHERE ecmid='".$dados['ecmid']."'";
	$db->executar($sql);
	$db->commit();
}

function buscarPessoas($dados) {
	header('Content-Type: text/html; charset=iso-8859-1');
	global $db;

	if($dados['q']) {
		$filtro[] = "p.pesnome ilike '%".iconv("UTF-8", "ISO-8859-1", trim($dados['q']))."%'";
		$filtroe[] = "usunome ilike '%".iconv("UTF-8", "ISO-8859-1", trim($dados['q']))."%'";
	}

	$sql[] = "(SELECT DISTINCT p.pesid as id, UPPER(p.pesnome) as nome, p.pesemail as email, p.pestelefone as telefone, p.pescpf as cpf, pe.pevcargo as cargo FROM atendimento.pessoa p Left JOIN atendimento.pessoaevento pe ON pe.pesid=p.pesid WHERE p.pesstatus='A'".(($filtro)?" AND ".implode(" AND ",$filtro):"").")";
	$sql[] = "(SELECT NULL as id, UPPER(usunome) as nome, usuemail as email, '('||usufoneddd||')'||usufonenum as telefone, usucpf as cpf, usufuncao as cargo FROM seguranca.usuario ".(($filtroe)?" WHERE ".implode(" AND ",$filtroe):"").")";
	
	$sql = "SELECT nome, id, email, telefone, cpf, cargo FROM (".implode("UNION", $sql).") foo GROUP BY nome, id, email, telefone, cpf, cargo Order by nome LIMIT 10";
	$pessoas = $db->carregar($sql);
	if($pessoas[0]) {
		foreach($pessoas as $pessoa) {
			$resultado[] = "{$pessoa['nome']}|{$pessoa['email']}|{$pessoa['telefone']}|{$pessoa['cpf']}|{$pessoa['id']}|{$pessoa['cargo']}";
		}
	}
	if($resultado) echo implode("\n",$resultado);
}

function buscarPessoasCPF($cpf,$tipo) {
	global $db;
	header('Content-Type: text/html; charset=iso-8859-1');
	switch ($tipo){
		case 1:
			$sql = "SELECT UPPER(p.pesnome) as nome, p.pesid as id, p.pesemail as email, p.pestelefone as telefone, p.pescpf as cpf, pe.pevcargo as cargo 
			FROM atendimento.pessoa p 
			LEFT JOIN atendimento.pessoaevento pe ON pe.pesid = p.pesid 
			WHERE p.pesstatus = 'A' AND p.pescpf = '".trim($cpf)."'";
			$rs = $db->pegalinha($sql);
			if(empty($rs)){
				$sql = "SELECT UPPER(usunome) as nome, NULL as id, usuemail as email, '('||usufoneddd||')'||usufonenum as telefone, usucpf as cpf, usufuncao as cargo 
				FROM seguranca.usuario WHERE usucpf = '".trim($cpf)."'";		
			}
      		break;
    	case 2:
        	$sql = "SELECT UPPER(p.pesnome) as nome, p.pesid as id, p.pesemail as email, p.pestelefone as telefone, p.pescpf as cpf, pe.pevcargo as cargo, pe.estuf as uf, pe.muncod as municipio, pe.pevorgao as orgao 
			FROM atendimento.pessoa p
			LEFT JOIN atendimento.pessoaevento pe ON pe.pesid = p.pesid 
			WHERE p.pesstatus = 'A' AND p.pescpf = '".trim($cpf)."'";
        	break;
    	case 3:
    		$sql = "SELECT UPPER(p.pesnome) as nome, p.pesid as id, p.pesemail as email, p.pestelefone as telefone, p.pescpf as cpf, pe.pevcargo as cargo, pa.estuf as uf, pe.muncod as municipio, pe.pevorgao as orgao 
			FROM atendimento.pessoa p 
			LEFT JOIN atendimento.pessoaevento pe ON pe.pesid = p.pesid 
			LEFT JOIN atendimento.parlamentar pa ON pa.pesid = p.pesid
			WHERE p.pesstatus = 'A' AND p.pescpf = '".trim($cpf)."'"; 
    		break;
	}
	$pessoas = $db->carregar($sql);
	if($pessoas[0]) {
		foreach($pessoas as $pessoa) {
			 $resultado = array($pessoa['cpf'],$pessoa['email'],$pessoa['telefone'],utf8_encode($pessoa['nome']),$pessoa['id'],utf8_encode($pessoa['cargo']),$pessoa['uf'],$pessoa['municipio'],$pessoa['orgao']);
		}
	}
	echo simec_json_encode($resultado);
}

function inserirEvento($dados) {
	global $db;
	$sql = "
		INSERT INTO atendimento.evento(
			agdid,
			evtassunto,
			evtdataini,
			evtdatafim,
			evtobservacoes,
			evtstatus,
			evtpauta,
			preid)
		VALUES (
			'".$_SESSION['atendimento_var']['agdid']."',
			'".substr(utf8_decode($dados['evtassunto']),0,250)."',
			NOW(),
			NULL,
			NULL,
			'B',
			'".substr(utf8_decode($dados['evtpauta']),0,4999)."',
			NULL)
		RETURNING evtid;";
	$evtid = $db->pegaUm($sql);
	$db->commit();
	echo $evtid;
}

function excluirEvento($dados) {
	global $db;
		$sql = "UPDATE atendimento.evento SET evtstatus='I' WHERE evtid='".$dados['evtid']."'";
    $db->executar($sql);
	$db->commit();
	echo "<script>;window.location='atendimento.php?modulo=principal/evento&acao=A';</script>";
}


function atualizarEvento($dados) {
	global $db;
	if($dados['pevencaminhamento']) {
		foreach($dados['pevencaminhamento'] as $pecid => $status) {
			$sql = "UPDATE atendimento.pevencaminhamento SET pecstatus='".$status."' WHERE pecid='".$pecid."'";
			$db->executar($sql);
		}
	}

	$sql = "SELECT CASE WHEN qtdenc=qtdencconc THEN 'C' ELSE 'A' END as evtstatus
	FROM (
	SELECT (SELECT COUNT(*) FROM atendimento.pevencaminhamento pv INNER JOIN atendimento.pessoaevento pe ON pe.pevid=pv.pevid WHERE pe.evtid=ev.evtid AND pecstatus!='I') as qtdenc,
	(SELECT COUNT(*) FROM atendimento.pevencaminhamento pv INNER JOIN atendimento.pessoaevento pe ON pe.pevid=pv.pevid WHERE pe.evtid=ev.evtid AND pecstatus='C') as qtdencconc
	FROM atendimento.evento ev WHERE evtid='".$dados['evtid']."') foo";

	$evtstatus=$db->pegaUm($sql);

	if(empty($dados['preid'])){
		$programa = "";
	} else {
		$programa = ",preid='".$dados['preid']."'";
	}
	
	$sql = "UPDATE atendimento.evento
			SET 
				evtassunto='".substr($dados['evtassunto'],0,250)."',
				evtdataini='".formata_data_sql($dados['evtdataini'])." ".$dados['evtdatainihora']."',
				evtdatafim=".(($dados['evtdatafim'])?"'".formata_data_sql($dados['evtdatafim'])." ".$dados['evtdatafimhora']."'":"NULL").",
				evtobservacoes=".(($dados['evtobservacoes'])?"'".$dados['evtobservacoes']."'":"NULL").",
				evtencaminhamentos=".(($dados['evtencaminhamentos'])?"'".$dados['evtencaminhamentos']."'":"NULL").",
				evtstatus='".$evtstatus."',
				evtpauta='".substr($dados['evtpauta'],0,4999)."'
				$programa
			WHERE evtid='".$dados['evtid']."';";
	$db->executar($sql);
	$db->commit();
	$evtid = $dados['evtid'];
	echo "<script>alert('Evento gravado com sucesso');window.location='atendimento.php?modulo=principal/gerenciarevento&acao=A&evtid=$evtid';</script>";
}

function inserirPessoaPorTipo($dados) {
	global $db;
	$sql = "INSERT INTO atendimento.pessoaevento(
	pesid, evtid, tppid, pevcargo, pevstatus)
	VALUES (NULL, '".$dados['evtid']."', '".$dados['tppid']."', NULL, 'B') RETURNING pevid;";
	$pevid = $db->pegaUm($sql);
	$db->commit();
}


function listarPessoasPorTipo($dados) {
	header('Content-Type: text/html; charset=iso-8859-1');
	global $db;
	unset($_REQUEST['requisicaoAjax']);	
	$cabecalho = array("&nbsp;","CPF","Nome","E-mail","Telefone","Cargo");
	$colunas = '';
	if( $dados['tppid'] != 1 ){
		$cabecalho = array("&nbsp;","CPF","Nome","E-mail","Telefone","Cargo","UF","Município","Orgão");
		$colunas = "
				pev.estuf,
				pev.muncod,
				'<center>
					<input id=\"pevorgao_'||pev.pevid||'\" type=\"text\" style=\"text-align;\" name=\"pevorgao['||pev.pevid||']\" size=\"20\" maxlength=\"100\" 
						   value=\"'||COALESCE(pev.pevorgao,'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" 
						   onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);salvarParticipantes('||pev.tppid||');\" title=\"Cargo\" class=\"obrigatorio normal\">
				</center>' as orgao,";
	}
	
	$sql = "SELECT 
				'<center>
					<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"removerPessoaEvento(\''||pev.pevid||'\',\''||pev.tppid||'\');\"> 
					<img src=../imagens/salvar.png style=cursor:pointer; 
					onclick=\"'||CASE WHEN pes.pesid IS NULL 
									THEN 'alert(\'Digite o nome do participante\');' 
									ELSE 'inserirEncaminhamentoPorPessoa('||pev.pevid||');' 
								 END||'\">
				</center>' as acao,
				'<center>
					<input id=\"pescpf_'||pev.pevid||'\" type=\"text\" style=\"text-align;\" name=\"pescpf['||pev.pevid||']\" size=\"15\" maxlength=\"11\" 
						   value=\"'||COALESCE(pes.pescpf,'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" 
						   onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);buscarDadosCPF('||pev.pevid||','||pev.tppid||');\" 
						   title=\"CPF\" class=\"obrigatorio normal\">
				</center>' as pescpf,
				'<center>
					<input id=\"pesid_'||pev.pevid||'\" type=\"hidden\" name=\"pesid['||pev.pevid||']\" value=\"'||COALESCE(pes.pesid::text,'')||'\">
					<input id=\"pesnome_'||pev.pevid||'\" type=\"text\" style=\"text-align;\" name=\"pesnome['||pev.pevid||']\" size=\"45\" maxlength=\"100\" 
						   value=\"'||COALESCE(pes.pesnome,'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" 
						   onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" 
						   title=\"Nome\" class=\"obrigatorio normal\">
				</center>' as pesnome,
				'<center>
					<input id=\"pesemail_'||pev.pevid||'\" type=\"text\" style=\"text-align;\" name=\"pesemail['||pev.pevid||']\" size=\"50\" maxlength=\"100\" 
						   value=\"'||COALESCE(pes.pesemail,'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" 
						   onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" title=\"E-mail\" class=\"obrigatorio normal\">
				</center>' as pesemail,
				'<center>
					<input id=\"pestelefone_'||pev.pevid||'\" type=\"text\" style=\"text-align;\" name=\"pestelefone['||pev.pevid||']\" size=\"18\" maxlength=\"100\" 
						   value=\"'||COALESCE(pes.pestelefone,'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" 
						   onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" title=\"Telefone\" 
						   class=\"obrigatorio normal\">
				</center>' as pestelefone,
				'<center>
					<input id=\"pevcargo_'||pev.pevid||'\" type=\"text\" style=\"text-align;\" name=\"pevcargo['||pev.pevid||']\" size=\"30\" maxlength=\"100\" 
						   value=\"'||COALESCE(pev.pevcargo,'')||'\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);this.select();\" 
						   onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" title=\"Cargo\" class=\"obrigatorio normal\">
				</center>' as pevcargo,
				$colunas
				pev.pevid
			FROM 
				atendimento.pessoaevento pev
			LEFT JOIN atendimento.pessoa pes ON pev.pesid = pes.pesid
			WHERE 
				pev.evtid='".$dados['evtid']."' 
				AND pev.tppid='".$dados['tppid']."' 
				AND pev.pevstatus IN('A','B') ORDER BY pev.pevid";

	$arrDados = $db->carregar($sql);
	
	if( $dados['tppid'] != 1 ){
		if( is_array( $arrDados ) ){
			foreach( $arrDados as $k => $r ){
				$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
				$estuf = $arrDados[$k]['estuf'];
				$arrDados[$k]['estuf'] = '<center>'.$db->monta_combo('estuf['.$arrDados[$k]['pevid'].']', $sql, 'S', 'Selecione', 'carregarMunicipiosAjax(this.id);cat', '', '', '', 
													  'N', 'estuf_'.$arrDados[$k]['pevid'], true, $arrDados[$k]['estuf']).'</center>';
				
				$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$estuf."' ORDER BY mundescricao";
				$arrDados[$k]['muncod'] = '<center>'.$db->monta_combo('muncod['.$arrDados[$k]['pevid'].']', $sql, 'S', 'Selecione', '', '', '', '', 
													  'N', 'muncod_'.$arrDados[$k]['pevid'], true, $arrDados[$k]['muncod']).'</center>';
			}
		}
	}
	
	$param['width'] = '100%';

	$db->monta_lista_array($arrDados,$cabecalho,50,5,'N','center',$html=array(),$arrayDeTiposParaOrdenacao=array(),$formName = "formlista",$param);
}

function carregarMunicipiosAjax( $dados ){
	header('Content-Type: text/html; charset=iso-8859-1');
	global $db;
	$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$dados['uf']."' ORDER BY mundescricao";
	echo $db->monta_combo('muncod['.$dados['pevid'].']', $sql, 'S', 'Selecione', '', '', '', '','N', 'muncod_'.$dados['pevid']);
}

function cabecalhoAgenda() {
	global $db;
	$html .= "<table class=\"tabela\" bgcolor=\"#f5f5f5\" cellSpacing=\"1\" cellPadding=\"3\" align=\"center\">";
	$html .= "<tr><td class=\"SubTituloDireita\" width=50%><b>Agenda:</></td><td>".$_SESSION['atendimento_var']['agddescricao']."</td></tr>";
	if($_SESSION['atendimento_var']['evtid']) {
		$html .= "<tr><td class=\"SubTituloDireita\" width=50%><b>Assunto do evento:</b></td><td>".$db->pegaUm("SELECT evtassunto FROM atendimento.evento WHERE evtid='".$_SESSION['atendimento_var']['evtid']."'")."</td></tr>";
	}
	$html .= "</table>";
	echo $html;
}


function listarEventos($dados) {
	header('Content-Type: text/html; charset=iso-8859-1');
	global $db;
	if($dados['evtassunto']) {
		$filtro[] = "evtassunto ilike '%".$dados['evtassunto']."%'";
	}
	if($dados['evtdataini']) {
		$filtro[] = "evtdataini>='".formata_data_sql($dados['evtdataini'])."'";
	}
	if($dados['evtdatafim']) {
		$filtro[] = "evtdatafim>='".formata_data_sql($dados['evtdatafim'])."'";
	}
	if($dados['pesnome']) {
		$filtro[] = "ev.evtid IN(SELECT DISTINCT evtid FROM atendimento.pessoaevento pe INNER JOIN atendimento.pessoa ps ON ps.pesid = pe.pesid WHERE ps.pesnome ilike '%".$dados['pesnome']."%')";
	}

	$sql = "SELECT acao, evtdataini, evtassunto, qtdenc, qtdencconc, (qtdenc-qtdencconc) as qtdpendente, situacao FROM (
	SELECT '<center><img src=../imagens/alterar.gif border=0 style=cursor:pointer; onclick=\"window.location=\'atendimento.php?modulo=principal/gerenciarevento&acao=A&evtid='||evtid||'\';\">&nbsp&nbsp<img src=../imagens/excluir.gif border=0 style=cursor:pointer; onclick=\"excluirEvento( '||evtid||' )\"></center>' as acao,
	to_char(evtdataini,'dd/mm/YYYY HH24:MI') as evtdataini,
	evtassunto,
	(SELECT COUNT(*) FROM atendimento.pevencaminhamento pv INNER JOIN atendimento.pessoaevento pe ON pe.pevid=pv.pevid WHERE pe.evtid=ev.evtid AND pecstatus!='I') as qtdenc,
	(SELECT COUNT(*) FROM atendimento.pevencaminhamento pv INNER JOIN atendimento.pessoaevento pe ON pe.pevid=pv.pevid WHERE pe.evtid=ev.evtid AND pecstatus='C') as qtdencconc,
	CASE WHEN evtstatus='A' THEN 'Em Aberto'
	WHEN evtstatus='C' THEN 'Concluído' END as situacao
	FROM atendimento.evento ev WHERE evtstatus !='I' AND agdid='".$_SESSION['atendimento_var']['agdid']."'".(($filtro)?" AND ".implode(" AND ",$filtro):"").") foo";
	
	$cabecalho = array("&nbsp;","Data","Assunto","Qtd Encaminhamentos","Qtd Encaminhamentos Concluídos","Qtd Encaminhamentos Pendentes","Situação do evento");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);

}

function carregarAbasEvento() {
	// monta menu padrão contendo informações sobre as entidades
	$menu = array(0 => array("id" => 1, "descricao" => "Evento", "link" => "/atendimento/atendimento.php?modulo=principal/gerenciarevento&acao=A&evtid=".$_SESSION['atendimento_var']['evtid']),
			1 => array("id" => 2, "descricao" => "Documento Anexo", "link" => "/atendimento/atendimento.php?modulo=principal/eventoeventoanexo&acao=A"),
			2 => array("id" => 3, "descricao" => "Observações", "link" => "/atendimento/atendimento.php?modulo=principal/eventoobservacoes&acao=A"),
			3 => array("id" => 4, "descricao" => "Vínculos", "link" => "/atendimento/atendimento.php?modulo=principal/eventovinculos&acao=A")
	);

	return $menu;
}

function listarAgendas($dados) {
	header('Content-Type: text/html; charset=iso-8859-1');
	global $db;
	$perfis = pegaPerfilGeral();
	if(in_array(PFL_GESTORAGENDA,$perfis)) {
		$inner = "INNER JOIN atendimento.usuarioresponsabilidade ur ON ur.agdid = agd.agdid AND ur.usucpf='".$_SESSION['usucpf']."' AND ur.pflcod='".PFL_GESTORAGENDA."' AND ur.rpustatus='A'";
	}
	$sql = "SELECT '<center><img src=../imagens/consultar.gif border=0 style=cursor:pointer; onclick=\"window.location=\'atendimento.php?modulo=principal/evento&acao=A&agdid='||agd.agdid||'\';\">
	&nbsp&nbsp<img src=../imagens/excluir.gif border=0 style=cursor:pointer; onclick=\"excluirAgenda( '||agd.agdid||' )\"></center>' as acao, '<span style=\"cursor:pointer;\" onclick=\"editarAgenda(\''||agd.agddescricao||'\',\''||agd.agdid||'\');\">'||agd.agddescricao||'</span>' as descricao
	FROM atendimento.agenda agd
	{$inner}
	WHERE agd.agdstatus='A' ORDER BY agd.agdid";
	$cabecalho = array("&nbsp;","Agenda");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
}

function inserirAgenda($dados) {
	global $db;
	$sql = "INSERT INTO atendimento.agenda(agddescricao, agdstatus) VALUES ('".utf8_decode(substr($dados['agddescricao'],0,100))."', 'A');";
	$db->executar($sql);
	$db->commit();
}

function atualizarAgenda($dados) {
	global $db;
	$sql = "UPDATE atendimento.agenda SET agddescricao='".utf8_decode(substr($dados['agddescricao'],0,100))."' WHERE agdid='".$dados['agdid']."'";
	$db->executar($sql);
	$db->commit();
}

function inserirAnexoEvento($dados) {
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$campos = array("evtid"=>$_SESSION['atendimento_var']['evtid'],"anedescricao"=>(($dados['anedescricao'])?"'".$dados['anedescricao']."'":"NULL"),"taeid"=>$dados['taeid']);
	$file = new FilesSimec("anexoevento", $campos, "atendimento");
	$arqdescricao = $dados['anedescricao'];
	if( $file->setUpload($arqdescricao, "arquivo", true ) ) {
		echo "<script>alert('Gravado com sucesso');window.location='atendimento.php?modulo=principal/eventoeventoanexo&acao=A';</script>";
	} else {
		echo "<script>alert('Problemas para gravar o arquivo');window.location='atendimento.php?modulo=principal/eventoeventoanexo&acao=A';</script>";
	}
}

function downloadAnexoEvento($dados) {
	global $db;
	if($dados['arqid']) {
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$file = new FilesSimec("anexoevento", array(), "atendimento");
		$file->getDownloadArquivo($dados['arqid']);
	}
}

function excluirAnexoEvento($dados) {
	global $db;
	$sql = "UPDATE atendimento.anexoevento SET anestatus='I' WHERE aneid='".$dados['aneid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Anexo excluído com sucesso');window.location='atendimento.php?modulo=principal/eventoeventoanexo&acao=A';</script>";
}

function apagarAgenda($dados) {
	global $db;
	$sql ="UPDATE atendimento.agenda  SET agdstatus= 'I' WHERE agdid='".$dados['agdid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Agenda excluída com sucesso');window.location='atendimento.php?modulo=principal/agenda&acao=A';</script>";
}


function excluirAnexoEncaminhamento($dados) {
	global $db;
	$sql = "UPDATE atendimento.anexoencaminhamento SET ancstatus='I' WHERE ancid='".$dados['ancid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Anexo excluído com sucesso');window.location='atendimento.php?modulo=principal/eventoeventoanexo&acao=A&requisicao=telaInserirAnexoEncaminhamento&pecid=".$dados['pecid']."';</script>";
}

function inserirObservacoesEvento($dados) {
	global $db;
	$sql = "INSERT INTO atendimento.eventoobservacoes(
	evtid, evbobservacoes, usucpf, evbdatains)
	VALUES ('".$_SESSION['atendimento_var']['evtid']."', '".$dados['evbobservacoes']."', '".$_SESSION['usucpf']."', NOW());";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Observação inserida com sucesso');window.location='atendimento.php?modulo=principal/eventoobservacoes&acao=A';</script>";
}

function excluirObservacoesEvento($dados) {
	global $db;
	$sql = "UPDATE atendimento.eventoobservacoes SET evbstatus='I' WHERE evbid='".$dados['evbid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Observação excluída com sucesso');window.location='atendimento.php?modulo=principal/eventoobservacoes&acao=A';</script>";
}

function telaInserirAnexoEncaminhamento($dados) {
	global $db;
	$dadosCabecalho = $db->pegaLinha("SELECT ecm.ecmdescricao, pes.pesnome FROM atendimento.pevencaminhamento p
			INNER JOIN atendimento.encaminhamento ecm ON ecm.ecmid=p.ecmid
			INNER JOIN atendimento.pessoaevento pev ON pev.pevid=p.pevid
			INNER JOIN atendimento.pessoa pes ON pes.pesid=pev.pesid
			WHERE pecid='".$dados['pecid']."'");
	?>
<script
	language="JavaScript" src="../includes/funcoes.js"></script>
<link
	rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link
	rel='stylesheet' type='text/css' href='../includes/listagem.css' />
<script>
	function salvarAnexoEncaminhamento() {
		if(document.getElementById('arquivo').value=='') {
			alert('Selecione um arquivo');
			return false;
		}
		document.getElementById('formulario').submit();
	}
	function downloadAnexoEncaminhamento(arqid) {
		window.location='atendimento.php?modulo=principal/eventoeventoanexo&acao=A&requisicao=downloadAnexoEvento&arqid='+arqid;
	}
	function excluirAnexoEncaminhamento(ancid) {
		var conf = confirm('Deseja excluir este anexo?');
		if(conf) {
			window.location='atendimento.php?modulo=principal/eventoeventoanexo&acao=A&requisicao=excluirAnexoEncaminhamento&pecid=<?=$dados['pecid'] ?>&ancid='+ancid;
		}
	}
	</script>
<form method="post" id="formulario" name="formulario"
	enctype="multipart/form-data">
	<input type="hidden" name="requisicao"
		value="inserirAnexoEncaminhamento"> <input type="hidden" name="pecid"
		value="<?=$dados['pecid'] ?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
		align="center">
		<tr>
			<td class="SubTituloDireita"><b>Encaminhamento:</b></td>
			<td><?=$dadosCabecalho['ecmdescricao'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Participante:</b></td>
			<td><?=$dadosCabecalho['pesnome'] ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Arquivo:</td>
			<td><input type="file" name="arquivo" id="arquivo"> <img border="0"
				title="Indica campo obrigatório." src="../imagens/obrig.gif" /></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Descrição:</td>
			<td><? echo campo_textarea( 'ancdescricao', 'N', 'S', '', '80', '5', '100'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2"><input type="button"
				name="salvar" value="Salvar" onclick="salvarAnexoEncaminhamento();">
			</td>
		</tr>
		<tr>
			<td colspan="2"><?
			$sql = "SELECT '<img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirAnexoEncaminhamento('||aec.ancid||')\"> <img src=../imagens/anexo.gif style=cursor:pointer; onclick=\"downloadAnexoEncaminhamento('||aec.arqid||')\">' as acao,
			aec.ancdescricao
			FROM atendimento.anexoencaminhamento aec
			WHERE aec.pecid='".$dados['pecid']."' AND aec.ancstatus='A'";

			$cabecalho = array("&nbsp;", "Descrição");
			$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
			?></td>
		</tr>
	</table>
</form>
<?
}

function inserirVinculoEvento($dados) {
	global $db;

	if($dados['entnumcpfcnpj']) {
		require_once APPRAIZ . "includes/classes/entidades.class.inc";
		$entidade = new Entidades();
		$entidade->carregarEntidade($dados);
		$entidade->adicionarFuncoesEntidade($dados['funcoes']);
		$entidade->salvar();
		$sql = "INSERT INTO atendimento.eventovinculos(entid, evtid, evvstatus) VALUES ('".$entidade->getEntId()."', '".$_SESSION['atendimento_var']['evtid']."', 'A');";
		$db->executar($sql);
	}

	if($dados['estuf']) {
		foreach($dados['estuf'] as $estuf) {
			$sql = "INSERT INTO atendimento.eventovinculos(estuf, evtid, evvstatus) VALUES ('".$estuf."', '".$_SESSION['atendimento_var']['evtid']."', 'A');";
			$db->executar($sql);
		}
	}
	if($dados['muncod']) {
		foreach($dados['muncod'] as $muncod) {
			$sql = "INSERT INTO atendimento.eventovinculos(muncod, evtid, evvstatus) VALUES ('".$muncod."', '".$_SESSION['atendimento_var']['evtid']."', 'A');";
			$db->executar($sql);
		}
	}
	$db->commit();

	echo "<script>
	alert('Vínculos adicionados com sucesso.');
	window.opener.carregarVinculo('".$dados['tipo']."');
	window.close();
	</script>";
}

function carregarVinculo($dados) {
	global $db;
	$permissao = permissoesPerfil();
	if($dados['tipo']=="E") {
		$sql = "SELECT '".(($permissao['adicionarevento'])?"<img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirVinculo('||ev.evvid||');\">":"&nbsp;")."' as acao, estdescricao FROM atendimento.eventovinculos ev
		INNER JOIN territorios.estado es ON es.estuf=ev.estuf
		WHERE ev.evvstatus='A'";

		$cabecalho = array("&nbsp;", "Estado");
		$db->monta_lista_simples($sql,$cabecalho,5000,5,'N','100%',$par2);
	}
	if($dados['tipo']=="M") {
		$sql = "SELECT '".(($permissao['adicionarevento'])?"<img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirVinculo('||ev.evvid||');\">":"&nbsp;")."' as acao, mu.estuf, mu.mundescricao FROM atendimento.eventovinculos ev
		INNER JOIN territorios.municipio mu ON mu.muncod=ev.muncod
		WHERE ev.evvstatus='A'";
		$cabecalho = array("&nbsp;", "Estado", "Município");
		$db->monta_lista_simples($sql,$cabecalho,5000,5,'N','100%',$par2);
	}
	if($dados['tipo']=="P") {
		$sql = "SELECT '".(($permissao['adicionarevento'])?"<img src=../imagens/excluir.gif style=\"cursor:pointer;\" onclick=\"excluirVinculo('||ev.evvid||');\">":"&nbsp;")."' as acao, en.entnome FROM atendimento.eventovinculos ev
		INNER JOIN entidade.entidade en ON en.entid=ev.entid
		WHERE ev.evvstatus='A'";
		$cabecalho = array("&nbsp;", "Empresa");
		$db->monta_lista_simples($sql,$cabecalho,5000,5,'N','100%',$par2);
	}
}

function excluirVinculo($dados) {
	global $db;
	$sql = "UPDATE atendimento.eventovinculos SET evvstatus='I' WHERE evvid='".$dados['evvid']."'";
	$db->executar($sql);
	$db->commit();
	echo "<script>alert('Vínculo excluido com sucesso');window.location='atendimento.php?modulo=principal/eventovinculos&acao=A';</script>";
}

function telaInserirVinculo($dados) {
	global $db;
	?>
<script
	language="JavaScript" src="../includes/funcoes.js"></script>
<link
	rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link
	rel='stylesheet' type='text/css' href='../includes/listagem.css' />
<script>
	function adicionarVinculo() {
		document.getElementById('formulario').submit();
	}
	function filtrarEstado(estuf) {
		window.location='atendimento.php?modulo=principal/eventovinculos&acao=A&requisicao=telaInserirVinculo&tipo=M&estuf_='+estuf;
	}
	function excluirAnexoEncaminhamento(ancid) {
		var conf = confirm('Deseja excluir este anexo?');
		if(conf) {
			window.location='atendimento.php?modulo=principal/eventoeventoanexo&acao=A&requisicao=excluirAnexoEncaminhamento&pecid=<?=$dados['pecid'] ?>&ancid='+ancid;
		}
	}
	</script>
<? 
if($dados['tipo']=="E") :
?>
<form method="post" id="formulario" name="formulario"
	enctype="multipart/form-data">
	<input type="hidden" name="requisicao" value="inserirVinculoEvento">
	<?
	$sql = "SELECT '<input type=checkbox name=\"estuf[]\" value=\"'||estuf||'\" >' as acao, estdescricao FROM territorios.estado ORDER BY estdescricao";
	$cabecalho = array("&nbsp;","Estado");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	?>
	<p align="center">
		<input type="button" name="inserir" value="Adicionar"
			onclick="adicionarVinculo();">
	</p>
</form>
<?
endif;
if($dados['tipo']=="M") :
?>
<form method="post" id="formulario" name="formulario"
	enctype="multipart/form-data">
	<input type="hidden" name="requisicao" value="inserirVinculoEvento">
	<?
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
		align="center">
		<tr>
			<td class="SubTituloDireita">Estado:</td>
			<td><? $db->monta_combo('estuf_', $sql, 'S', 'Selecione', 'filtrarEstado', '', '', '200', 'S', 'estuf', '',$dados['estuf_']); ?>
			</td>
		</tr>
	</table>
	<?
	if($dados['estuf_']) {
		$sql = "SELECT '<input type=checkbox name=\"muncod[]\" value=\"'||muncod||'\" >' as acao, mundescricao FROM territorios.municipio WHERE estuf='".$dados['estuf_']."' ORDER BY mundescricao";
		$cabecalho = array("&nbsp;","Município");
		$db->monta_lista_simples($sql,$cabecalho,5000,5,'N','95%',$par2);
	}
	?>
	<p align="center">
		<input type="button" name="inserir" value="Adicionar"
			onclick="adicionarVinculo();">
	</p>
</form>
<?
endif;
if($dados['tipo']=="P") :

require_once APPRAIZ . "includes/classes/entidades.class.inc";

$entidade = new Entidades();

echo $entidade->formEntidade("atendimento.php?modulo=principal/eventovinculos&acao=A&requisicao=inserirVinculoEvento&tipo=P",
		array("funid" => FUN_VINCULO_ATENDIMENTO),
		array("enderecos"=>array(1))
);
?>
<script>
	document.getElementById('tr_njuid').style.display='none';
	document.getElementById('tr_tpctgid').style.display='none';
	document.getElementById('tr_tpcid').style.display='none';
	document.getElementById('tr_tplid').style.display='none';
	document.getElementById('tr_tpsid').style.display='none';
	document.getElementById('tr_entobs').style.display='none';
	document.getElementById('tr_funcoescadastradas').style.display='none';
	document.getElementById('tr_entcodent').style.display='none';
	document.getElementById('tr_entnuninsest').style.display='none';
	document.getElementById('tr_entunicod').style.display='none';
	document.getElementById('tr_entungcod').style.display='none';
	
	</script>
<?
endif;

}

function inserirAnexoEncaminhamento($dados) {
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$campos = array("pecid"=>$dados['pecid'],"ancdescricao"=>(($dados['ancdescricao'])?"'".$dados['ancdescricao']."'":"NULL"));
	$file = new FilesSimec("anexoencaminhamento", $campos, "atendimento");
	$arqdescricao = $dados['ancdescricao'];
	if( $file->setUpload($arqdescricao, "arquivo", true ) ) {
		echo "<script>alert('Gravado com sucesso');window.location='atendimento.php?modulo=principal/eventoeventoanexo&acao=A&requisicao=telaInserirAnexoEncaminhamento&pecid=".$dados['pecid']."';</script>";
	} else {
		echo "<script>alert('Problemas para gravar o arquivo');window.location='atendimento.php?modulo=principal/eventoeventoanexo&acao=A&requisicao=telaInserirAnexoEncaminhamento&pecid=".$dados['pecid']."';</script>";
	}
}

function permissoesPerfil() {
	global $db;
	$perfis = pegaPerfilGeral();

	if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis)) {
		$permissoes['adicionaragenda'] = true;
	}
	if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis)) {
		$permissoes['removeragenda'] = true;
	}

	if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis) || in_array(PFL_GESTORAGENDA,$perfis)) {
		$permissoes['adicionarevento'] = true;
	}
	if($db->testa_superuser() || in_array(PFL_ADMINISTRADOR,$perfis) || in_array(PFL_GESTORAGENDA,$perfis)) {
		$permissoes['removerevento'] = true;
	}

	return $permissoes;

}

/*
 * salvarBiografia()
 * Salva os dados da biografia do parlamentar
 * 
 * $dados: array com os dados da biografia
 *     $dados['parid'] = ID do parlamentar
 *     $dados['bipid'] = ID da biografia
 *     $dados['bipdescricao'] = texto da biografia
 */
function salvarBiografia($dados){
	global $db;
	if($dados['bipid'])
		$sql = "
			UPDATE atendimento.biografiaparlamentar SET
				bipdescricao = '".$dados['bipdescricao']."'
			WHERE bipid = ".$dados['bipid'];
	else
		$sql = "
			INSERT INTO atendimento.biografiaparlamentar(
				parid,
				bipdescricao)
			VALUES(
				".$dados['parid'].",
				'".$dados['bipdescricao']."')
		";
	$db->executar($sql);
	$db->commit();
}

?>