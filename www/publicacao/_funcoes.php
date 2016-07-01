<?php
/* 
 * SQL para o fluxo de Solicitações.
 */
$_SESSION['sqlPessoasTramitar'] = "
        SELECT DISTINCT
            usu.usunome,
            usu.usuemail
        FROM
            seguranca.usuario usu
        JOIN
            seguranca.perfilusuario pru
        USING
            (usucpf)
        JOIN
            seguranca.perfil prf
        ON
            prf.pflcod = pru.pflcod
        WHERE
            pru.pflcod IN
            (
                SELECT
                    pflcod
                FROM
                    workflow.estadodocumentoperfil
                WHERE
                    aedid = %d)
        AND NOT prf.pflsuperuser
        ORDER BY
            1";
function tratarSimNao($texto){
    switch ($texto) {
        case 'N': return '<span class="label label-danger">Não</span>';
        case 'S': return '<span class="label label-success">Sim</span>';
    }
    return $texto;
}

function tratarBotoesAcoes($texto){
    
    $tipo = explode("_",$texto);
    
    switch ($tipo[1]) {
        case 't': return "<span title=\"Confirmar Teste\" class=\"testado btn btn-info btn-sm glyphicon glyphicon-ok-circle\" value=\"{$tipo[0]}\"></span>";
        case 'e': return "<span title=\"Executar\" class=\"executado btn btn-warning btn-sm glyphicon glyphicon-cog\" value=\"{$tipo[0]}\"></span>";
        case 's': return "<span title=\"SQL Executado em Produção\" class=\"sqlproducao btn btn-warning btn-sm glyphicon glyphicon-cog\" value=\"{$tipo[0]}\"></span>";
        case 'p': return "<span title=\"Confirmar Publicado\" class=\"publicado btn btn-success btn-sm glyphicon glyphicon-flag\" value=\"{$tipo[0]}\"></span>";    
    }
    return $texto;
}


/**
 * Caso o documento não estaja criado cria um novo
 *
 * @param string $solid
 * @return integer
 * @todo Abstrair dentro do módulo do workflow.
 */
function criarDocumento($solid) {
    global $db;

    $docid = pegarDocid($solid);

    if (!$docid) {
        // recupera o tipo do documento
        $tpdid = 239;
        // descrição do documento
        $docdsc = "Pedido de publicação N°" . $solid;
        // cria documento do WORKFLOW
        $docid = wf_cadastrarDocumento($tpdid, $docdsc);
        // atualiza o plano de trabalho
        $sql = "UPDATE publicacao.solicitacao SET  docid = " . $docid . "  WHERE solid = " . $solid;
        $db->executar($sql);
        $db->commit();
    }

    return $docid;
}

/**
 * Pega o id do documento do plano de trabalho
 *
 * @param integer $solid
 * @return integer
 * @todo Abstrair dentro do módulo do workflow.
 */
function pegarDocid($solid) {
    global $db;
    $sql = "Select	docid
			FROM publicacao.solicitacao
			WHERE solid = " . $solid;
    return $db->pegaUm($sql);
}

function checkboxEnviar($solid, $dados) {
    global $db;
    $arquivosPublicar = $db->pegaUm("SELECT  sol.solconteudo FROM publicacao.solicitacao sol WHERE solid = {$solid}");
    if (($dados['esddsc']=='Cópia de arquivos executada.'
            || $dados['esddsc'] == 'SQL executado em produção.') 
            && $arquivosPublicar <> '' ) {
        return <<<HTML
        <input type="checkbox" value="{$solid}" data-toggle="toggle" 
        data-on="<span class='glyphicon glyphicon-ok'></span>" data-off="&nbsp;" data-size="mini" />
HTML;
    } else {
        return '<center>-</center>';
    }
}

/**
 * Pega o arqid do anexo caso exista para o solid
 *
 * @param integer $solid
 * @return integer
 * @todo Carregar carregarDados($dados).
 */
function pegarArqid($solid) {
    global $db;
    $sql = "Select	arqid
			FROM publicacao.solicitacao
                        JOIN publicacao.anexogeral using (solid)
			WHERE solid = " . $solid;
    return $db->pegaUm($sql);
}

function retornaComentarioHistorico($dados) {
     global $db;
        $sql = "
		select 
                    COALESCE(cd.cmddsc, '-') as comentario
		from workflow.historicodocumento hd
			inner join workflow.acaoestadodoc ac on
				ac.aedid = hd.aedid
			inner join workflow.estadodocumento ed on
				ed.esdid = ac.esdidorigem
			inner join seguranca.usuario us on
				us.usucpf = hd.usucpf
			left join workflow.comentariodocumento cd on
				cd.hstid = hd.hstid
		where
			hd.hstid = {$dados[0]} 
	";
        $comentario = $db->pegaUm($sql);
    if ($comentario != '-') {
        $saida = $comentario;//montaItemAccordion("Detalhar", rand(0, 99999999), $comentario, array('aberto' => false));
    } else {
        $saida = '-';
    }
    return $saida;
}
