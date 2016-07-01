<?php
/**
 * $Id: _funcoes.php 102361 2015-09-11 18:50:20Z maykelbraz $
 */

/**
 * Recebe campo do sql do tipo array e formata a saida utilizando o bootstrap label primary
 * @author Lindalberto Filho.
 * @param array $campo Ex: 'campo1,campo2'
 * @return string
 */
function formataCampoArray($campo){
	if($campo == null || trim($campo) == ''){
		return "";
	}

	$campos = explode(',', $campo);
	$campo = '';
	foreach ($campos as $cam){
		$campo .= '<span class="label label-primary">'.$cam.'</span> <br/>';
	}

	return $campo;
}


function disparaEmail($pdlid,$usucpf){
	require_once APPRAIZ . 'includes/funcoes.inc';

	if($pdlid == null || $usucpf == null){
		echo "<script>alert('Não podemos enviar o e-mail contendo informações do processo devido a falta de dados.')</script>";
		return;
	}

	global $db;
	$sql = <<<DML
		SELECT
			su.usunome as nome,
			su.usuemail as email
		FROM progorc.pedidolimite pp
		INNER JOIN seguranca.usuario su ON(su.usucpf = pp.usucpf)
		WHERE pp.pdlid = $pdlid
DML;

	$destinatario = $db->pegaLinha($sql);
	$sql = <<<DML
		SELECT
			usunome as nome,
			usuemail as email
		FROM seguranca.usuario
		WHERE usucpf = '$usucpf'
DML;
	$remetente = $db->pegaLinha($sql);

	$mensagem = "
		Prezado Usuário,<br/>
		Um de seus Pedidos de Limite, N° {$pdlid}, foi devolvido para que sejam efetuadas correções.<br/>
		Acesse o módulo SPO - Limites Orçamentários no SIMEC para maiores detalhes.<br/><br/>
		Atenciosamente,<br/>
		<a href=\"www.simec.mec.gov.br\">SIMEC</a>
	";

	if($remetente){
		if(enviar_email($remetente, $destinatario['email'], 'SPO - Limites Orçamentários ', '<p style="text-align:justify;">'.$mensagem.'</p>', $cc, $remetente['email'])){
			echo "<script>alert('Enviamos um e-mail de notificação confirmando a ação.');</script>";
			return true;
		}else
			echo "<script>alert('Falha ao disparar e-mail.');</script>";
	}else
		echo "<script>alert('O Sistema não pode encontrar os dados cadastrais de um ou mais usuários.');</script>";

	return false;
}

/* Função para montar o Relatório Dinâmico */

function montaExtratoDinamico($post) {

	global $db;
	$listagem = new Simec_Listagem();
	/* Muda o tipo do objeto  */
	if ($post['requisicao'] == 'exportarXLS') {
		$listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
	}
	$cabecalho = array();
	/* Retorna vazio caso não seja selecionada nenhuma coluna. */
	if (count($post['dados']['colunas']['qualitativo']) == 0 || count($post['dados']['quantitativo']) == 0) {
		$sql = "SELECT 1 WHERE 1 <> 1 ";
	}

	/* Tratando as colunas do Qualitativo */
    if (count($post['dados']['cols-qualit']) > 0) {
        foreach ($post['dados']['cols-qualit'] as $valor) {
			$titulo = $db->pegaLinha("SELECT crldsc FROM progorc.colunasextrato WHERE crlcod = '{$valor}' AND crltipo = 'QL'");
            $titulo = $titulo['crldsc'];
            // Cabeçalho
            array_push($cabecalho, $titulo);
            // Query
            $select .= " {$valor} ,";
        }
        $select = substr($select, 0, strlen($select) - 1);
        $groupby = $select;
	}

    /* Tratando as colunas do Quantitativo */
    if (count($post['dados']['cols-quant']) > 0) {
        $select .= ", ";
        foreach ($post['dados']['cols-quant'] as $valor) {
			$titulo = $db->pegaLinha("SELECT crldsc FROM progorc.colunasextrato WHERE crlcod = '{$valor}' AND crltipo = 'QT'");
			$titulo = $titulo['crldsc'];
			array_push($cabecalho, $titulo);
			// Query
			/* Testa se a coluna quantitativa é de Expressão */
			$colunaExpressao = $db->pegaLinha("SELECT crlexpquantitativo, crlexpcallback, crlexpcomtotal, crlexpaddgroupby FROM altorc.colunasextrato WHERE crlcod = '{$valor}' AND crltipo = 'QT' AND crlexpquantitativo IS NOT NULL");

			if (!$colunaExpressao) {
				$select .= " SUM({$valor}) AS {$valor} ,";
				$listagem->addCallbackDeCampo("{$valor}", 'mascaraMoeda');
				$listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, "{$valor}");
			} else {
				$select .= " {$colunaExpressao['crlexpquantitativo']} AS {$valor} ,";
				/* Caso tenha função Callback */
				if ($colunaExpressão['crlexpcallback'] != '') {
					$listagem->addCallbackDeCampo("{$valor}", $colunaExpressao['crlexpcallback']);
				}
				/* Caso seja para totalizar */
				if ($colunaExpressao['crlexpcomtotal']) {
					$listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, "{$valor}");
				}

				$groupby .= $colunaExpressao['crlexpaddgroupby'];
			}
		}
		$select = substr($select, 0, strlen($select) - 1);
	}

	/* Filtros */

        $post['dados']['filtros']['exercicio'][0]=$_SESSION['exercicio'];
	if (count($post['dados']['filtros']) > 0) {
		$where = 'WHERE ';
                #ver($post['dados']['filtros'],d);
		foreach ($post['dados']['filtros'] as $chave => $valor) {
			if($valor == null || $valor == '')
				continue;
			/* @TODO  Lembrar de tratar tipo de dado depois que organizar a tabela */
			$valor = implode($valor, "','");
			$where .= " $chave IN ('{$valor}') AND";
		}
		$where = substr($where,0,strlen($where) - 3);
	}

	/* Montando a Query */
	if ($select != '' && $groupby != '') {
		$sql = " SELECT DISTINCT {$select}
		FROM
		progorc.vwpedidoslimitecompleta
		{$where}
		GROUP BY
		{$groupby}
		ORDER BY 1 ";
	}

	#ver($post, $sql, $cabecalho, d);
	$dados = $db->carregar($sql);
	if (!is_array($dados)) {
		$dados = array();
	}
	$listagem->setDados($dados);
	$listagem->setCabecalho($cabecalho);
	$listagem->setFormOff();
	/* Mostrar a query em um hidden na tela */
	$saida['listagem'] = $listagem;
	$saida['sql'] = $sql;

	/* Imprime de acordo com a chamada */
	if ($post['requisicao'] == 'exportarXLS') {
		$_REQUEST['_p'] = 'all';
		$listagem->render();
		die();
	} else {
		return $saida;
	}
}

function enviarEmailAjusteUO() {
	global $db;

	$sql = <<<DML
SELECT org.codigo AS org_codigo, aca.codigo AS aca_codigo
  FROM planacomorc.acao_programatica apr
    INNER JOIN planacomorc.orgao org USING(id_orgao)
    INNER JOIN planacomorc.acao aca USING(id_acao)
  WHERE id_acao_programatica = {$acaidentificadorunicosiop}
DML;
	$data = $db->pegaLinha($sql);
	$msg = "A seguinte UO {$data['org_codigo']} teve um pedido de limite encaminhado para ajustes.";

	$sql = "SELECT u.usunome, u.usuemail
              FROM seguranca.usuario u
        INNER JOIN seguranca.perfilusuario p ON p.usucpf = u.usucpf
             WHERE p.pflcod='" . PFL_CGP_GESTAO . "'";

	$usrs = $db->carregar($sql);

	if ($usrs [0]) {
		foreach ($usrs as $us) {
			$arDest = array();
			$arDest [] = $us ['usuemail'];
			enviar_email(array
			(
				'nome' => 'Limites Orçamentário',
				'email' => $_SESSION['email_sistema']
			), $arDest, 'Pedidos de Limites', $msg);
		}
	}

	$sql = "SELECT DISTINCT usucpf AS cpf
		     FROM seguranca.usuario_sistema
			WHERE sisid = {$_SESSION['sisid']}
			  AND susstatus = 'A'";

	$usuarios = $db->carregar($sql);

	foreach ($usuarios as  $valor){
		$params['sisid']=$_SESSION['sisid'];
		$params['usucpf']=$valor['cpf'];
		$params['mensagem']="O Comunicado {$_POST['nomeArquivo']} foi cadastrado.";
		$params['url']='/progorc/progorc.php?modulo=inicio&acao=C';

		cadastrarAvisoUsuario($params);
	}


	return true;
}


function comunicarUnidadesPedidoRetornadoAjustesUO($pdlid,$usucpf)
{
    global $db;
    $sql = <<<DML
        SELECT
            usuemail
        FROM seguranca.usuario
        WHERE usucpf = '{$usucpf}'
DML;
    $email = $db->pegaUm($sql);
    if (!$email || trim($email) == '') {
        return true;
    }

    $msg = <<<HTML
<p>O pedido de limite orçamentário "{$pdlid}" foi retornado para correção.</p>
<p>Por favor, acesse o módulo "SPO - Limites Orçamentários", no <a href="http://simec.mec.gov.br">SIMEC</a>, e faça as correções necessárias.</p>
HTML;

    $rest = enviarEmail(
        array(array('usuemail' => $email, 'usunome' => $unicod)),
        'Correção pendente',
        $msg,
        array(),
        array(
            'usunome' => 'Pedido de Limite - Limites Orçamentários',
            'usuemail' => $_SESSION['email_sistema']
        ),
        null,
        false
    );
    include_once APPRAIZ . "spo/autoload.php";
    /*
    * Grava a notificação de aviso para o usuário
    */
    $params['sisid']= $_SESSION['sisid'];
    $params['usucpf']= $usucpf;
    $params['mensagem'] = "O pedido de limite orçamentário \"{$pdlid}\" foi retornado para correção.";
    $params['url']='/progorc/progorc.php?modulo=principal/limite/listar&acao=A&mes=&anexo=III&unicod=';
    cadastrarAvisoUsuario($params);

    return true;
}

function enviarEmail( array $destinatarios, $assunto, $conteudo, array $arquivos, $remetenteInformado = array(), $destinoArquivo = null, $condicao = true )
{
    require_once APPRAIZ . "includes/Email.php";
    $objetoEmail = new Email();
    # identifica o remetente
    $remetente = $objetoEmail->pegarUsuario( $_SESSION['usucpforigem'] );
    if (!$remetente->usucpf ) {
        return false;
    }
    $objetoEmail->Host = 'smtp2.mec.gov.br';
    $objetoEmail->CharSet = 'ISO-8895-1';
    $objetoEmail->Timeout = 60;
    #$this->SMTPDebug = true;
    $objetoEmail->From     = isset( $remetenteInformado["usuemail"] ) ? $remetenteInformado["usuemail"] : $remetente->usuemail;
    $objetoEmail->FromName = isset( $remetenteInformado["usunome"]  ) ? $remetenteInformado["usunome"]  : $remetente->usunome;

    # identifica os destinatários
    foreach ( $destinatarios as &$destinatario ) {
        $objetoEmail->AddBCC( $destinatario["usuemail"], $destinatario["usunome"] );
    }
    # anexa os arquivos
    foreach ( $arquivos as $arquivo ) {
        if ( $arquivo["error"] == UPLOAD_ERR_NO_FILE ) {
            continue;
        }

        $objetoEmail->AddAttachment( $destinoArquivo, basename( $destinoArquivo ) );
    }

    # formata assunto, conteudo e envia a mensagem
    $objetoEmail->Subject = Email::ASSUNTO . str_replace( "\'", "'", $assunto );
    $objetoEmail->Body    = str_replace( "\'", "'", utf8_decode($conteudo));
    $objetoEmail->IsHTML( true );
    set_time_limit(180);

    if(!$objetoEmail->Send()) {
        return false;
    }

    if($condicao){
        return $objetoEmail->registrar( $remetente, $destinatarios, $assunto, $conteudo );
    }else{
        return true;
    }
}

function processaStatus($esdid, $linha) {
    if ($esdid == STDOC_ATENDIDO) {
        if ($linha['pdljustificativa'] == 'Cadastrado por carga automática') {
            return '<span style="color:green"; class="glyphicon glyphicon-thumbs-up"></span> - Atend. em Lote';
        }
        if (($linha['reducao'] == $linha['atendido_r']) && ($linha['ampliacao'] == $linha['atendido_a'])) {
            return '<span style="color:green"; class="glyphicon glyphicon-thumbs-up"></span> - Atendido';
        } else {
            return '<span style="color:green"; class="glyphicon glyphicon-exclamation-sign"></span> - Atend. Parcial';
        }
    }
    switch ($esdid) {
        case STDOC_NAO_ENVIADO: return '<span style="color:red" class="glyphicon glyphicon-eye-close"></span>  Não enviado';
        case STDOC_ANALISE_SPO: return '<span style="color:blue" class="glyphicon glyphicon-transfer"></span>  Análise SPO';
        case STDOC_ACERTOS_SPO: return '<span style="color:blue" class="glyphicon glyphicon-transfer"></span>  Acertos SPO';
        case STDOC_ANALISE_SECRETARIA: return '<span style="color:blue" class="glyphicon glyphicon-transfer"></span>  Análise Secretaria';
        case STDOC_ACERTOS_SECRETARIA: return '<span style="color:blue" class="glyphicon glyphicon-transfer"></span>  Acertos Secretaria';
        case STDOC_RECUSADO: return '<span  style="color:red" class="glyphicon glyphicon-thumbs-down"></span>  Recusado';
        case STDOC_JUNTA_ORCAMENTARIA: return '<span style="color:blue" class="glyphicon glyphicon-transfer"></span>  Análise Junta Orçamentária';
    }
}
