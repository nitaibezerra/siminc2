<?php

/**
 * Caso o documento não estaja criado cria um novo
 *
 * @param string $colid
 * @return integer
 */
function criarDocumento( $colid ) {
	global $db;

	$docid = pegarDocid($colid);

	if( ! $docid ) {
		// recupera o tipo do documento
		$tpdid = TPDID_PNLD;

		// descrição do documento
		$docdsc = "Cadastro de LIVRO DIDÁTICO (PNLD) - n°" . $colid;

		// cria documento do WORKFLOW
		$docid = wf_cadastrarDocumento( $tpdid, $docdsc );

		// atualiza o plano de trabalho
		$sql = "
			UPDATE livro.colecao SET docid = ".$docid."
			WHERE colid = ".$colid." and prsano = '".$_SESSION['exercicio']."';
		";

		$db->executar( $sql );
		$db->commit();
	}

	return $docid;
}

/**
 * Pega o id do documento na coleção
 *
 * @param integer $colid
 * @return integer
 */
function pegarDocid( $colid ) {
	global $db;

	$sql = "
		SELECT docid
		FROM livro.colecao
		WHERE colid = ".(integer)$colid." and prsano = '".$_SESSION['exercicio']."';
	";

	return (integer) $db->pegaUm( $sql );
}

/**
 * Pega o estado atual do workflow
 *
 * @param integer $colid
 * @return integer
 */
function pegarEstadoAtual( $colid ) {
	global $db;

	$docid = pegarDocid( $colid );

	$sql = "select
				ed.esdid
			from
				workflow.documento d
			inner join
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where
				d.docid = " . $docid;

	$estado = (integer) $db->pegaUm( $sql );

	return $estado;
}

//Recupera Perfis do Usuario e armazena em um array();

function arrayPerfil()
{
	/*** Variável global de conexão com o bando de dados ***/
	global $db;

	/*** Executa a query para recuperar os perfis no módulo ***/
	$sql = "SELECT
				pu.pflcod
			FROM
				seguranca.perfilusuario pu
			INNER JOIN
				seguranca.perfil p ON p.pflcod = pu.pflcod
								  AND p.sisid = ".SISID_PNLD."
			WHERE
				pu.usucpf = '".$_SESSION['usucpf']."'
			ORDER BY
				p.pflnivel";
	$pflcod = $db->carregarColuna($sql);

	/*** Retorna o array com o(s) perfil(is) ***/
	return (array)$pflcod;
}

/**
 * Funções WorkFlow
 */

function validaEnvioAnaliseIPES($colid){
	global $db;

	$rreid = $db->pegaUm("SELECT count(rreid) FROM livro.resenharecurso WHERE colid = $colid and rrestatus = 'A'");

	if( !empty($rreid) ) return true;

	return 'Para enviar para análise IPES é necessário informar um processo.';
}

function cabecalhoLivro($colid, $lvdid){
    global $db;

    if( $colid ){
        $sql = "
            SELECT  ed.edtnome as editora,
                    u.usunome as responsavel,
                    c.comdsc as componente,
                    col.coltitulo as colecao
            FROM livro.colecao col
            INNER JOIN livro.editora ed ON ed.edtid = col.edtid
            INNER JOIN livro.responsaveleditora re ON re.edtid = ed.edtid
            LEFT JOIN seguranca.usuario u ON u.usucpf = re.reecpf
            INNER JOIN livro.componente c ON c.comid = col.comid
            WHERE col.colid = ".$colid." and col.prsano = '".$_SESSION['exercicio']."'
        ";
        $dados = $db->pegaLinha( $sql );

        $sql = "
            SELECT  ld.lvdcodigo as codigo,
                    ld.lvdtitulo as titulo,
                    CASE WHEN ld.lvdtipo = 'L' THEN 'Livro' ELSE 'Manual' END as tipo,
                    ld.lvdanoedicao as ano,
                    ld.lvdnumeropaginas as paginas,
                    cp.cpsdsc,
                    ld.lvdautor as autor
            FROM livro.colecao col
            JOIN livro.livrodidatico ld ON ld.colid = col.colid
            JOIN livro.composicao cp ON cp.cpsid = ld.cpsid
            WHERE col.colid = ".$colid." and col.prsano = '".$_SESSION['exercicio']."'
            ORDER BY codigo, titulo, tipo
        ";
        $cabecalho = Array("<center>Código</center>", "<center>Título</center>", "<center>Tipo</center>", "<center>Ano</center>", "<center>Páginas</center>", "<center>Composição</center>", "<center>Autor</center>");

        echo '
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                <tr>
                    <td class="SubTituloDireita" width="10%">Editora:</td>
                    <td>
                        '.$dados['editora'].'
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="10%">Responsável:</td>
                    <td>
                        '.$dados['responsavel'].'
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="10%">Componente:</td>
                    <td>
                        '.$dados['componente'].'
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="10%">Coleção:</td>
                    <td>
                        '.$dados['colecao'].'
                    </td>
                </tr>
            </table>
            <br>
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                <tr>
                    <td class="SubTituloCentro"width="10%">RELAÇÃO DE OBRAS</td>
                </tr>
            </table>
            <br>
        ';
        $alinhamento = Array('center', 'left', 'left', 'right', 'right', 'center', 'center');
        $tamanho = Array('5%', '23%', '3%', '3%', '3%', '9%', '45%');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
        echo '<br>';
    } else {
        $sql = "
            SELECT  ed.edtnome as editora,
                    u.usunome as responsavel,
                    c.comdsc as componente,
                    lvdtitulo as livro
            FROM livro.livrodidatico l
            INNER JOIN livro.editora ed ON ed.edtid = l.edtid
            LEFT JOIN livro.usuarioresponsabilidade re ON re.edtid = ed.edtid
            LEFT JOIN seguranca.usuario u ON u.usucpf = re.usucpf
            INNER JOIN livro.componente c ON c.comid = l.comid
            WHERE l.lvdid = ".$lvdid." and l.prsano = '".$_SESSION['exercicio']."'
            LIMIT 1
        ";
        $dados = $db->pegaLinha( $sql );

        $sql = "
            SELECT  ld.lvdcodigo as codigo,
                    ld.lvdtitulo as titulo,
                    CASE WHEN ld.lvdtipo = 'L' THEN 'Livro' ELSE 'Manual' END as tipo,
                    ld.lvdanoedicao as ano,
                    ld.lvdnumeropaginas as paginas,
                    cp.cpsdsc,
                    ld.lvdautor as autor
            FROM livro.livrodidatico ld
            JOIN livro.composicao cp ON cp.cpsid = ld.cpsid
            WHERE ld.lvdid = ".$lvdid." and ld.prsano = '".$_SESSION['exercicio']."'
            ORDER BY codigo, titulo, tipo
        ";
        $cabecalho = Array("<center>Código</center>", "<center>Título</center>", "<center>Tipo</center>", "<center>Ano</center>", "<center>Páginas</center>", "<center>Composição</center>", "<center>Autor</center>");

        echo '
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                <tr>
                    <td class="SubTituloDireita" width="10%">Editora:</td>
                    <td>
                        '.$dados['editora'].'
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="10%">Responsável:</td>
                    <td>
                        '.$dados['responsavel'].'
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="10%">Componente:</td>
                    <td>
                        '.$dados['componente'].'
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="10%">Livro Regional:</td>
                    <td>
                        '.$dados['livro'].'
                    </td>
                </tr>
            </table>
            <br>
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                <tr>
                    <td class="SubTituloCentro"width="10%">RELAÇÃO DE OBRAS</td>
                </tr>
            </table>
            <br>
        ';
        $alinhamento = Array('center', 'left', 'left', 'right', 'right', 'center', 'center');
        $tamanho = Array('5%', '23%', '3%', '3%', '3%', '9%', '45%');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
        echo '<br>';
    }
}

function checkPerfil($pflcods, $testa_superuser = true) {
    global $db;

    if ($db->testa_superuser() && $testa_superuser) {

        return true;
    } else {

        if (is_array($pflcods)) {
            $pflcods = array_map("intval", $pflcods);
            $pflcods = array_unique($pflcods);
        } else {
            $pflcods = array((integer) $pflcods);
        }
        if (count($pflcods) == 0) {
            return false;
        }
        $sql = "
            select count(*)
            from seguranca.perfilusuario
            where usucpf = '" . $_SESSION['usucpf'] . "' and pflcod in ( " . implode(",", $pflcods) . " )
        ";
        return $db->pegaUm($sql) > 0;
    }
}

function verificaDataAberturaFechamento( $tipo ){
    global $db;

    #VISUALIAZAR PARECERES - V;
    #ANEXAR RECURSOS (LIBERA BOTÃO ANEXAR) - R;
    #ACESSO AO AMBIENTE RESPOSTA E RECURSOS PARA PERFIS COORDENAÇÃO E TECNICO - A;

    if( $tipo == 'V' ){
        if( !$db->testa_superuser() ){
            if( ( strtotime( date('F d Y G:i') ) >= strtotime( DATA_ABERTURA_VISUALIZA_PARECER) ) && ( strtotime( date('F d Y G:i') ) <= strtotime( DATA_ENCERRAMENTO_VISUALIZA_PARECER ) ) ){
                return 'S';
            }else{
                return 'N';
            }
        }else{
            return 'S';
        }
    }
    
    if( $tipo == 'R' ){
        if( !$db->testa_superuser() ){
            if( ( strtotime( date('Y-m-d') ) >= strtotime( DATA_ABERTURA_RECURSO_EDITORA) ) && ( strtotime( date('Y-m-d') ) <= strtotime( DATA_ENCERRAMENTO_RECURSO_EDITORA ) ) ){
                return 'S';
            }else{
                return 'N';
            }
        }else{
            return 'S';
        }
    }
    
    if( $tipo == 'A' ){
        if( !$db->testa_superuser() ){
            if( ( strtotime( date('Y-m-d') ) >= strtotime( DATA_ABERTURA_RECURSO_RESPOSTA_COORD_TEC) ) && ( strtotime( date('Y-m-d') ) <= strtotime( DATA_ENCERRAMENTO_RECURSO_RESPOSTA_COORD_TEC ) ) ){
                return 'S';
            }else{
                return 'N';
            }
        }else{
            return 'S';
        }
    }
}

/**
 * Victor Martins Machado
 * Função que retorna as responsabilidades do usuário
 * 
 * @param string $usucpf - CPF do usuário
 * @param string $tprsigla - sigla do tipo de responsabilidade
 * @param string $tiporetorno - tipo do retorno {
 * 				ALL - Código e Descrição, 
 * 				COD - somente o Código, 
 * 				DSC - somente a descrição}
 * 
 * @return Retorna um array vazio, se não houver dados, ou um array com os dados informados no parâmetro $tiporetorno
 */
function retornaResponsabilidades($usucpf, $tprsigla, $tiporetorno = 'all'){
	global $db;	
	
	$sqlPfl = "select
					p.pflcod
				from livro.tprperfil tpp
				inner join seguranca.perfil p on tpp.pflcod = p.pflcod
				inner join livro.tiporesponsabilidade tpr on tpp.tprcod = tpr.tprcod
				where tpr.tprsigla = '{$tprsigla}'";
	
	$pflcod = $db->carregar($sqlPfl);
	$pflcod = $pflcod[0]['pflcod'];
	
	if ($pflcod != '') {
		switch ($tprsigla) {
			case "E":
				$sqlRespUsuario = "
	                        SELECT  e.edtid as codigo,
	                                e.edtnome as descricao
	                        FROM livro.editora e
	                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.edtid = ur.edtid)
		
	                        WHERE e.edtstatus = 'A' AND ur.rpustatus = 'A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'
	                    ";
				break;
			case "J":
				$sqlRespUsuario = "
	                        SELECT  e.ediid as codigo,
									e.edinome as descricao
	                        FROM livro.ejaeditora e
		
	                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.ediid = ur.ediid)
		
				WHERE ur.rpustatus = 'A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'
	                    ";
				break;
		
			case "N":
				$sqlRespUsuario = "
	                        SELECT
	                        	e.edpid as codigo,
								e.edpnomefantasia as descricao
	                        FROM livro.pnaiceditora e
	                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.edpid = ur.edpid)
			
				WHERE ur.rpustatus = 'A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'";
					
				break;

            case "C":
                $sqlRespUsuario = "
	                        SELECT
	                        	e.cedid as codigo,
								e.cedrazaosocial as descricao
	                        FROM livro.campoeditora e
	                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.cedid = ur.cedid)

				WHERE ur.rpustatus = 'A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'";

                break;
		
			case "L":
				$sqlRespUsuario = "
	                        SELECT  e.colid as codigo,
	                                e.coltitulo as descricao
	                        FROM livro.colecao e
	                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.colid = ur.colid)
		
				WHERE e.colstatus = 'A' AND ur.rpustatus = 'A' AND ur.usucpf = '%s' AND ur.pflcod = '%s'
	                    ";
				break;
			case "P":
				$sqlRespUsuario = "
	                        SELECT  e.comid as codigo,
									e.comdsc as descricao
	                        FROM livro.componente e
	                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.comid = ur.comid)
	                        WHERE e.comstatus = 'A' AND ur.rpustatus = 'A' AND ur.usucpf = '%s'  AND ur.pflcod = '%s'
	                    ";
				break;
			case "D":
				$sqlRespUsuario = "
	                        SELECT  e.comid as codigo,
									e.comdsc as descricao
	                        FROM livro.componente e
	                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.comid = ur.comid)
	                        WHERE e.comstatus = 'A' AND ur.rpustatus = 'A' AND ur.usucpf = '%s'  AND ur.pflcod = '%s'
	                    ";
				break;
			case "U":
				$sqlRespUsuario = "
	                        SELECT  e.unvid as codigo,
									e.univdsc as descricao
	                        FROM livro.ptauniversidade e
	                        INNER JOIN livro.usuarioresponsabilidade ur ON (e.unvid = ur.unvid)
	                        WHERE ur.rpustatus = 'A' AND ur.usucpf = '%s'  AND ur.pflcod = '%s'
	                    ";
				break;
		}
		
		if(!$sqlRespUsuario) continue;
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
		
		$respUsuario = $db->carregar($query);
		//$respUsuario = $respUsuario[0];
		
		switch ($tiporetorno){
			case 'all':
				return is_array($respUsuario) ? $respUsuario : array();
				break;
			case 'cod':
				if (is_array($respUsuario)){
					$codResp = array();
					foreach ($respUsuario as $r){
						$codResp [] = $r['codigo'];
					}
					return $codResp;
				} else {
					return array();
				}
				break;
			case 'dsc':
						if (is_array($respUsuario)){
					$codResp = array();
					foreach ($respUsuario as $r){
						$codResp [] = $r['descricao'];
					}
					return $codResp;
				} else {
					return array();
				}
				break;
		}
	} else {
		return array();
	}
	
}

?>