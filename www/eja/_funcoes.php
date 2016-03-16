<?PHP

/**
 * functionName atualizaComboMunicipioMantenedora
 *
 * @author Luciano F. Ribeiro
 *
 * @param string $estuf sigla do estado.
 * @return string  retorna a o combo com os municípios referente ao estado (UF).
 *
 * @version v1
 */
function atualizaComboMunicipio($estuf) {
    global $db;

    $estuf = $estuf['estuf'];

    $sql = "
            SELECT  m.muncod AS codigo,
                    m.mundescricao AS descricao
            FROM territorios.municipio AS m
            LEFT JOIN territorios.estado AS u ON u.estuf = m.estuf
            WHERE u.estuf = '{$estuf}'
            ORDER BY m.mundescricao
        ";
    $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', 300, 'S', 'muncod', false, $muncod, null);
    die();
}

/**
 * Busca dados da escola que o usuário está vinculado
 *
 * @global type $db
 * @param integer $cpf
 * @param array $listaPerfil
 * @return array
 */
function buscarEscolaUsuarioLogado($cpf, $listaPerfil) {
    global $db;
    $sql = "
        SELECT
            e.entid,
            e.entnome,
            est.estuf,
            ur.rpustatus
        FROM
            eja.usuarioresponsabilidade ur
            INNER JOIN entidade.entidade e ON e.entid = ur.entid
            LEFT JOIN entidade.endereco ende ON ende.entid = e.entid
            INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
            INNER JOIN territorios.estado est ON est.estuf = mun.estuf
        WHERE
            ur.usucpf = '%s'
            AND ur.pflcod IN( %s )
            AND ur.rpustatus='A'";
    $query = vsprintf($sql, array($cpf, join($listaPerfil, ',')));
    $listaEscola = $db->carregar($query);
    return $listaEscola;
}

/**
 * Busca dados da escola
 *
 * @global type $db
 * @param integer $entid
 * @return array
 */
function buscarDadosEscola($entid) {
    global $db;
    $sql = "
        SELECT
            ent.entid,
            ent.entcodent,
            ent.entnome,
            ende.estuf,
            mun.mundescricao,
            ende.endlog || ', ' || ende.endcom || ' - ' || ende.endbai as endereco,
            professor.entid AS identf,
            professor.entnome AS dir
        FROM entidade.entidade ent
            LEFT JOIN entidade.funcaoentidade feEscola on feEscola.entid = ent.entid
            LEFT JOIN entidade.funentassoc assocprofessores on assocprofessores.entid = ent.entid
            LEFT JOIN entidade.funcaoentidade feProfessor on feProfessor.fueid = assocprofessores.fueid
            LEFT JOIN entidade.entidade professor on professor.entid = feProfessor.entid
            LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid
            LEFT JOIN entidade.entidadedetalhe entd ON entd.entid = ent.entid
            INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
            INNER JOIN territorios.estado est ON est.estuf = mun.estuf
        WHERE
            ent.entstatus='A'
            AND feEscola.funid in (3,4)
            AND ent.entid = '$entid'";

    $dados = $db->pegalinha($sql);
    return $dados;
}

/**
 * Busca escolas por municipio
 *
 * @global type $db
 * @param integer $muncod
 * @return array
 */
function buscarEscolas($muncod) {
    global $db;
    $sql = "
        SELECT
            ent.entid,
            ent.entcodent,
            ent.entnome,
            ende.estuf,
            mun.mundescricao,
            ende.endlog || ', ' || ende.endcom || ' - ' || ende.endbai as endereco,
            professor.entid AS identf,
            professor.entnome AS dir
        FROM entidade.entidade ent
            LEFT JOIN entidade.funcaoentidade feEscola on feEscola.entid = ent.entid
            LEFT JOIN entidade.funentassoc assocprofessores on assocprofessores.entid = ent.entid
            LEFT JOIN entidade.funcaoentidade feProfessor on feProfessor.fueid = assocprofessores.fueid
            LEFT JOIN entidade.entidade professor on professor.entid = feProfessor.entid
            LEFT JOIN entidade.endereco ende ON ende.entid = ent.entid
            LEFT JOIN entidade.entidadedetalhe entd ON entd.entid = ent.entid
            INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
            INNER JOIN territorios.estado est ON est.estuf = mun.estuf
        WHERE
            ent.entstatus='A'
            AND feEscola.funid in (3,4)
            AND mun.muncod = '$muncod'";

    $listaEscola = $db->carregar($sql);
    return $listaEscola;
}

/**
 * Busca número da turma ultima turma criada
 *
 * @global type $db
 * @param type $entid
 * @return type
 */
function buscarSequencialUltimaTurma($entid) {
    global $db;

    $sql = "
        SELECT
            t.ntesequencial
        FROM
            eja.novaturmaeja t
        WHERE
            t.pk_cod_entidade = '%s'
        ORDER BY
            t.ntesequencial DESC
        LIMIT 1
    ";
    $query = vsprintf($sql, array($entid));
    $ntesequencial = $db->pegaUm($query);
    return $ntesequencial;
}

/**
 * Busca quantidade de alunos da turma
 *
 * @global type $db
 * @param integer $nteid
 * @return integer
 */
function buscarQuantidadeAluno($nteid) {
    global $db;

    $sql = "
        SELECT
            COUNT(a.ateid)
        FROM
            eja.alunoturmaeja a
        WHERE
            a.nteid = '%s'
    ";
    $query = vsprintf($sql, array($nteid));
    $qtdAlunosAtivos = $db->pegaUm($query);
    return $qtdAlunosAtivos;
}

/**
 * Busca dados da Turma
 *
 * @global type $db
 * @param integer $nteid
 * @return array
 */
function buscarDadosTurma($nteid) {
    global $db;

    $sql = "
        SELECT
            t.nteid,
            t.pk_cod_entidade,
            t.ntesequencial,
            t.ntedataenviofnde,
            t.ntefechaturma,
            t.ntedatafechaturma,
            t.ntestatus,
            t.nivid,
            t.ntedatainiturma,
            nt.nivqtmaxaluno
        FROM
            eja.novaturmaeja t
            JOIN eja.nivelturma nt ON(t.nivid = nt.nivid)
        WHERE
            t.nteid = '%s'
    ";
    $query = vsprintf($sql, array($nteid));
    $turma = $db->pegalinha($query);

    return $turma;
}

/**
 * Busca lista de publico prioritario
 *
 * @global type $db
 * @return type
 */
function listarPublicoPrioritario() {
    global $db;
    $sql = "
        SELECT
            pupid AS codigo,
            pupdescricao AS descricao
        FROM
            eja.publicoprioritario";
    $listaPublicoPrioritario = $db->carregar($sql);
    return $listaPublicoPrioritario;
}

/**
 * Verifica se o CPF ja foi cadastrado em um outro registro
 *
 * @param stdClass $parametros
 */
function validarAlunoCPF(stdClass $parametros) {

    # Verifica se o CPF ja foi cadastrado em um outro registro
    $alunoCPFCadastrado = buscarDadosAluno((object) array(
                'notInAteid' => (int) $parametros->notInAteid,
                'cpf' => $parametros->cpf
    ));

    if ($alunoCPFCadastrado) {
        $scriptRedirecionar = "
            <script>
                alert('Atenção, o aluno {$alunoCPFCadastrado['atenomedoaluno']} com o CPF {$alunoCPFCadastrado['atecpf']} já foi cadastrado, por favor cadastre um aluno com o número de CPF diferente.');
                window.location = window.location
            </script>";
        echo $scriptRedirecionar;
        die;
    }
}

/**
 * Verifica se a turma extá fechada para pode cadastrar ou excluir aluno e mostra mensagem de erro
 *
 * @param integer $nteid
 * @param string $descricaoOperacao Cadastrar ou Excluir
 * @return boolean
 */
function validarTurmaFechada($nteid, $descricaoOperacao) {
    $operacao = true;

    # Busca dados da turma
    $turma = buscarDadosTurma($nteid);

    if ($turma['ntefechaturma'] == 'F') {
        $js = "
            <script>
                alert('Atenção, Não foi possível {$descricaoOperacao} o aluno porque a turma está fechada.');
            </script>";
        echo $js;
        $operacao = false;
    }

    return $operacao;
}

/**
 * Valida se a turma possui 35 alunos e exibe mensagem de validação
 *
 * @param integer $nteid
 * @return boolean
 */
function validarTurmaQtdMaximaAlunos($nteid) {
    $operacao = true;

    # busca dados da turma
    $turma = buscarDadosTurma($nteid);

    # Busca lista de alunos
    $listaAluno = buscarAlunos($nteid);

    if (count($listaAluno) >= $turma['nivqtmaxaluno']) {
        $js = "
            <script>
                alert('Atenção, não foi possível cadastrar o aluno porque a quantidade máxima permitida para o nível dessa turma é {$turma['nivqtmaxaluno']} alunos.');
            </script>";
        echo $js;
        $operacao = false;
    }

    return $operacao;
}

/**
 * Busca lista de alunos da turma
 *
 * @global type $db
 * @param integer $nteid
 * @return array
 */
function buscarAlunos($nteid) {
    global $db;

    $sql = "
        SELECT
			a.ateid,
            a.atecpf,
            a.atenomedoaluno,
            to_char(a.atedatanasc, 'DD/MM/YYYY'),
            a.atenomedamae
        FROM
            eja.alunoturmaeja a
        WHERE
            a.nteid = {$nteid}
        ORDER BY
            a.ateid
    ";

    $listaAlunos = $db->carregar($sql);
    return $listaAlunos;
}

/**
 * Busca dados do Aluno
 *
 * @param stdClass $filtro Filtros
 * @return array
 */
function buscarDadosAluno(stdClass $filtro) {
    global $db;
    $aluno = array();
    $where = NULL;

    if ($filtro->ateid)
        $where[] = " a.ateid = {$filtro->ateid} ";
    if ($filtro->cpf)
        $where[] = " a.atecpf = '{$filtro->cpf}' ";
    if (isset($filtro->notInAteid))
        $where[] = " a.ateid NOT IN( {$filtro->notInAteid} ) ";

    if (!empty($where)) {
        $sql = "
            SELECT
                a.ateid,
                a.nteid,
                a.atecpf,
                a.atenomedoaluno,
                to_char(a.atedatanasc, 'DD/MM/YYYY') AS atedatanasc,
                a.atenomedamae,
                a.atesexo,
                a.pupid
            FROM
                eja.alunoturmaeja a
            WHERE
                " . join(' AND ', $where);

        $aluno = $db->pegalinha($sql);
    }
    return $aluno;
}

function buscarEstadoUsuarioLogado($cpf) {
    global $db;

    $sql = "
        SELECT DISTINCT
            u.estuf,
            u.estuf || ' - ' || u.estdescricao AS estdescricao
        FROM
            eja.usuarioresponsabilidade ur
            JOIN territorios.estado u ON ur.estuf=u.estuf
        WHERE
            ur.rpustatus='A'
            AND ur.usucpf = '{$cpf}'
    ";

    $estadoUsuarioLogado = $db->pegalinha($sql);

    return $estadoUsuarioLogado;
}

function buscarMunicipioUsuarioLogado($cpf) {
    global $db;

    $sql = "
            SELECT
                m.estuf,
                u.estuf || ' - ' || u.estdescricao AS estdescricao,
                m.muncod,
                m.estuf || ' - ' || m.mundescricao as mundescricao
            FROM
                eja.usuarioresponsabilidade ur
                JOIN territorios.municipio m ON ur.muncod = m.muncod
                JOIN territorios.estado u ON m.estuf = u.estuf
            WHERE
                ur.rpustatus='A'
                AND ur.usucpf = '{$cpf}'
    ";

    $municipioUsuarioLogado = $db->pegalinha($sql);
    return $municipioUsuarioLogado;
}

/**
 * Exibe na tela os dados da Escola
 *
 * @param integer $entid
 * @return VOID
 */
function mostrarDadosEscola($entid) {
    $elementoCabecalho = '';
    $listaPerfil = pegaPerfilGeral();

    if (in_array(PERFIL_EJA_DIRETOR_ESCOLA, $listaPerfil)) {
        $dadosEscola = $entid ? buscarDadosEscola($entid) : NULL;
        $elementoCabecalho = "
            <fieldset>
                <div class='form-group'>
                    <strong>Código INEP:</strong> {$dadosEscola['entcodent']}
                    <br />
                    <strong>Nome da Escola:</strong> {$dadosEscola['entnome']}
                    <br />
                    <strong>Endereço da Escola:</strong> {$dadosEscola['endereco']}
                    <br />
                    <strong>UF:</strong> {$dadosEscola['estuf']}
                    <br />
                    <strong>Município:</strong> {$dadosEscola['mundescricao']}
                    <br />
                </div>
            </fieldset>
        ";
    }

    return $elementoCabecalho;
}

#-------------------------------------------------------- FUNÇÕES DA GERAÇÃO DE LOTES -----------------------------------------------------#

/**
 * functionName buscaNumPortaria
 *
 * @author Luciano F. Ribeiro
 *
 * @param string $lotnumero id da tabela lote.
 * @return string retorna dados do lote gerado.
 *
 * @version v1
 */
function buscaNumPortaria($lotnumero) {
    global $db;

    $sql = "
            SELECT lotenumportaria, lotedataportaria FROM eja.lote WHERE lotnumero = {$lotnumero} GROUP BY lotenumportaria, lotedataportaria;
        ";
    $data = $db->pegaLinha($sql);

    return $data;
}

/**
 * functionName excluirLoteGerado
 *
 * @author Luciano F. Ribeiro
 *
 * @param string $lotnumero id da tabela lote.
 * @return string retorna a exclusão do lote e atualização das tabelas novaturmaeja e alunotuirmaeja.
 *
 * @version v1
 */
function excluirLoteGerado($dados) {
    global $db;

    $lotnumero = $dados['lotnumero'];

    if ($lotnumero) {
        #BUSCA OS MUNICÍPIOS DO LOTE ESPECIFICADO.
        $sql_lot = " SELECT '\''|| lotmuncod ||'\'' FROM eja.lote where lotstatus = 'E' AND lotnumero = {$lotnumero}; ";
        $lotmuncod = implode(',', $db->carregarColuna($sql_lot));

        if ($lotmuncod != '') {
            #BUSCA AS TURMAS DOS MUNICÍPIOS CARREGADOS PELA SQL ACIMA.
            $sql_turm = "
                    SELECT  nte.nteid
                    FROM eja.novaturmaeja AS nte

                    JOIN entidade.entidade ent ON ent.entid = nte.pk_cod_entidade
                    JOIN entidade.endereco ende ON ende.entid = ent.entid
                    JOIN territorios.municipio mun ON mun.muncod = ende.muncod
                    JOIN territorios.estado est ON est.estuf = mun.estuf

                    WHERE nte.ntestatus = 'A' AND ntenumerolote = '{$lotnumero}' AND ntefechaturma = 'F' AND nteenviofnde = 'E' AND mun.muncod IN ( $lotmuncod )
                ";
                    
            $nteid = implode(',', $db->carregarColuna($sql_turm));

            if ($nteid != '') {
                #BUSCA OS ALUNOS DAS TURMAS CARREGADAS PELA SQL ACIMA.
                $sql_alu = "
                        SELECT  a.ateid
                        FROM eja.alunoturmaeja AS a
                        WHERE a.atestatus = 'E' AND a.nteid IN ( $nteid );
                    ";
                $ateid = implode(',', $db->carregarColuna($sql_alu));
            }
        }

        if ($ateid != '') {
            #UPDATE TABELA ALUNOS - MUDANÇA DE STATUS PARA NULL.
            $sql_up_alu = " UPDATE eja.alunoturmaeja SET atestatus = NULL WHERE atestatus = 'E' AND ateid IN ( $ateid ) RETURNING ateid; ";
            $res_ateid = $db->pegaUm($sql_up_alu);
        }

        if ($nteid != '') {
            #UPDATE TABELA TURMAS - MUDANÇA DE STATUS PARA V.
            $sql_up_turm = " UPDATE eja.novaturmaeja SET nteenviofnde = 'V', ntedataenviofnde = NULL, ntenumerolote = NULL WHERE nteenviofnde = 'E' AND nteid IN ( $nteid ) RETURNING nteid; ";
            $res_nteid = $db->pegaUm($sql_up_turm);
        }

        $sqlLoteEstado = "SELECT  lesid
                FROM eja.loteestado loe
                WHERE lesnumero = {$lotnumero}";
        $loteEstado = $db->pegaUm($sqlLoteEstado);
        
        if ($loteEstado) {
            $sql_up_turm = " UPDATE eja.novaturmaeja SET nteenviofnde2p = NULL, ntedataenviofnde2p = NULL WHERE nteenviofnde = 'E' AND nteid IN ( $nteid )";
            $db->executar($sql_up_turm);
            $sql_up_alu = " UPDATE eja.alunoturmaeja SET atestatus2p = NULL WHERE atestatus = 'E' AND ateid IN ( $ateid )";
            $db->executar($sql_up_alu);
        }

        if ($res_nteid > 0 && $res_ateid > 0) {
            #DELETA O LOTE GETADO NA TABELA LOTE DE ACORDO COM O NÚMERO DO LOTE.
            $sql_del_lot_estado = " DELETE FROM eja.loteestado WHERE lesnumero = {$lotnumero}";
            $db->executar($sql_del_lot_estado);
            $sql_del_lot = " DELETE FROM eja.lote WHERE lotnumero = {$lotnumero} RETURNING lotid; ";
            $res_lotid = $db->pegaUm($sql_del_lot);
        }
    }

    if ($res_lotid > 0) {
        $db->commit();
        $db->sucesso('principal/gerarlote/gerar_lote', '', "O lote nº {$lotnumero} foi excluido com sucesso!");
    } else {
        $db->rollback();
        $db->insucesso('Não foi possível excluir o Lote, tente novamente mais tarde!', '', 'principal/gerarlote/gerar_lote&acao=A');
    }
}

/**
 * functionName excluirSegundaParcela
 *
 * @author Kamyla Sakamoto
 *
 * @param string $lotnumero id da tabela lote.
 * @return string retorna a exclusão do lote e atualização das tabelas novaturmaeja e alunotuirmaeja.
 *
 * @version v1
 */
function excluirSegundaParcela($dados) {
    global $db;

    $lotnumero = $dados['lotnumero'];

    if ($lotnumero) {
        #BUSCA OS MUNICÍPIOS DO LOTE ESPECIFICADO.
        $sql_lot = " SELECT '\''|| lotmuncod ||'\'' FROM eja.lote where lotstatus = 'E' AND lotnumero = {$lotnumero}; ";
        $lotmuncod = implode(',', $db->carregarColuna($sql_lot));
        
//        #BUSCA O ANO DO LOTE
//        $sql_exercicio = " SELECT distinct lotexercicio FROM eja.lote where lotstatus = 'E' AND lotnumero = {$lotnumero}; ";
//        $exercicio = $db->carregar($sql_exercicio);
        

        if ($lotmuncod != '') {
            #BUSCA AS TURMAS DOS MUNICÍPIOS CARREGADOS PELA SQL ACIMA.
            $sql_turm = "
                    SELECT  nte.nteid
                    FROM eja.novaturmaeja AS nte

                    JOIN entidade.entidade ent ON ent.entid = nte.pk_cod_entidade
                    JOIN entidade.endereco ende ON ende.entid = ent.entid
                    JOIN territorios.municipio mun ON mun.muncod = ende.muncod
                    JOIN territorios.estado est ON est.estuf = mun.estuf

                    WHERE nte.ntestatus = 'A' AND ntefechaturma = 'F' AND ntenumerolote2p = '{$lotnumero}' AND nteenviofnde2p = 'E' AND mun.muncod IN ( $lotmuncod )
                ";

            $nteid = implode(',', $db->carregarColuna($sql_turm));

            if ($nteid != '') {
                #BUSCA OS ALUNOS DAS TURMAS CARREGADAS PELA SQL ACIMA.
                $sql_alu = "
                        SELECT  a.ateid
                        FROM eja.alunoturmaeja AS a
                        WHERE a.atestatus2p = 'E' AND a.nteid IN ( $nteid );
                    ";
                $ateid = implode(',', $db->carregarColuna($sql_alu));
            }
        }

        if ($ateid != '') {
            #UPDATE TABELA ALUNOS - MUDANÇA DE STATUS PARA NULL.
            $sql_up_alu = " UPDATE eja.alunoturmaeja SET atestatus2p = 'C' WHERE atestatus2p = 'E' AND ateid IN ( $ateid ) RETURNING ateid; ";
            $res_ateid = $db->pegaUm($sql_up_alu);
        }

        if ($nteid != '') {
            #UPDATE TABELA TURMAS - MUDANÇA DE STATUS PARA V.
            $sql_up_turm = " UPDATE eja.novaturmaeja SET nteenviofnde2p = 'V', ntedataenviofnde2p = NULL, ntenumerolote2p = NULL WHERE nteenviofnde2p = 'E' AND nteid IN ( $nteid ) RETURNING nteid; ";
            $res_nteid = $db->pegaUm($sql_up_turm);
        }

        if ($res_nteid > 0 && $res_ateid > 0) {
            #DELETA O LOTE GETADO NA TABELA LOTE DE ACORDO COM O NÚMERO DO LOTE.
            $sql_del_lot = " DELETE FROM eja.lote WHERE lotnumero = {$lotnumero} RETURNING lotid; ";
            $res_lotid = $db->pegaUm($sql_del_lot);
        }
    }

    if ($res_lotid > 0) {
        $db->commit();
        $db->sucesso('principal/confirmarcpf/segunda_parcela', '', "O lote (Segunda Parcela) nº {$lotnumero} foi excluido com sucesso!");
    } else {
        $db->rollback();
        $db->insucesso('Não foi possível excluir o Lote (Segunda Parcela), tente novamente mais tarde!', '', 'principal/confirmarcpf/segunda_parcela&acao=A');
    }
}

/**
 * functionName listagemGrupoLote
 *
 * @author Luciano F. Ribeiro
 *
 * @param string $lotnumero id da tabela lote.
 * @return string retorna retorna a listagem dos lotes gerados.
 *
 * @version v1
 */
function listagemGrupoLote($lotnumero) {
    global $db;
    if (is_array($lotnumero)) {
        $lotnumero = $lotnumero['dados'][0];
    }
    $sql = "
                SELECT  lotuf,
                        mundescricao as lotnomemunicipio,
                        lotmuncod,
                        lotcnpj,
                        lotqtaluno,
                        lotvlcusteio,
                        lotnumparcela,
                        lotvlparcela,
                        lotexercicio
                FROM eja.lote lot
                INNER JOIN territorios.municipio mun ON mun.muncod = lot.lotmuncod
                WHERE lotnumero = {$lotnumero}  order by lotuf, lotnomemunicipio";
//ver($sql,d);
    $cabecalho = array("UF", "Município", "Código do IBGE", "CNPJ", "Total Alunos", "Valor Total", "Parcela", "Valor da Parcela", "Exercicio");

    $colunaTotalAlunos = array("lotqtaluno");
    $colunasValorAluno = array("lotvlcusteio", "lotvlparcela");

    $listagem = new Simec_Listagem();
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sql);
    $listagem->addCallbackDeCampo($colunaTotalAlunos, "mascaraNumero");
    $listagem->addCallbackDeCampo($colunasValorAluno, "mascaraMoeda");
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $colunaTotalAlunos);
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $colunasValorAluno);
    $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

    $sqlEstado = "
                SELECT   lesuf, lesrazaosocial, lesesfera, lescnpj, 
            lesqtaluno, lesvlcusteio, lesvlparcela,   lesnumparcela,
            lesexercicio
                FROM eja.loteestado lot
                WHERE lesnumero = {$lotnumero}";
                
    $loteEstado = $db->carregar($sqlEstado);
        
    if($loteEstado){
        $cabecalho = array("UF", "Razão Social", "Esfera","CNPJ", "Total Alunos", "Valor Total", "Parcela", "Valor da Parcela", "Exercicio");

        $colunaTotalAlunos = array("lesqtaluno");
        $colunasValorAluno = array("lesvlcusteio", "lesvlparcela");

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sqlEstado);
        $listagem->addCallbackDeCampo($colunaTotalAlunos, "mascaraNumero");
        $listagem->addCallbackDeCampo($colunasValorAluno, "mascaraMoeda");
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $colunaTotalAlunos);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $colunasValorAluno);
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
        
    }
}

function gerarLoteEstado($lotnumero) {
    global $db;
    if (is_array($lotnumero)) {
        $lotnumero = $lotnumero['dados'][0];
    }
    $sql = "SELECT  distinct lotuf
                FROM eja.lote lot
                INNER JOIN territorios.municipio mun ON mun.muncod = lot.lotmuncod
                WHERE lotnumero = {$lotnumero}";

    $qntEstados = $db->carregar($sql);
    
    $sqlLoteEstado = "SELECT  lesid
                FROM eja.loteestado loe
                WHERE lesnumero = {$lotnumero}";
                
    $loteEstado = $db->pegaUm($sqlLoteEstado);
    
    if ($qntEstados[1]) {
        $mensagem = 'Lote para o estado não pode ser gerado. Existem municípios de UF diferentes !';
    } else if($loteEstado) {
        $mensagem = 'Já existe um lote de Estado para o Lote de número '.$lotnumero.'.';
    }else if(!$loteEstado){
        
            
        $sqlLote = "  SELECT  lotuf,
                        mundescricao as lotnomemunicipio,
                        lotmuncod,
                        lotnumero,
                        lotqtaluno,
                        lotvlcusteio,
                        lotnumparcela,
                        lotvlparcela,
                        lotexercicio,
                        lotenumportaria,
                        lotedataportaria
                FROM eja.lote lot
                INNER JOIN territorios.municipio mun ON mun.muncod = lot.lotmuncod
                WHERE lotnumero = {$lotnumero}  order by lotuf, lotnomemunicipio";

        $lote = $db->carregar($sqlLote);

        foreach ($lote as $loteEstado) {
            $qtaluno += $loteEstado['lotqtaluno'];
            $custeio += $loteEstado['lotvlcusteio'];
            $vlparcela += $loteEstado['lotvlparcela'];
        }
        
       $sqlEstadoRazao = " SELECT DISTINCT entrazaosocial,
                entnumcpfcnpj
FROM entidade.entidade ent
INNER JOIN entidade.funcaoentidade fne ON fne.entid = ent.entid
INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
INNER JOIN territorios.estado est ON est.estuf = mun.estuf
WHERE funid = '6'
  AND fuestatus = 'A'
  AND entstatus = 'A'
  AND ende.estuf = '{$loteEstado['lotuf']}'";
  
  $razaoSocial = $db->carregar($sqlEstadoRazao);

  $razaoSocial[0]['entrazaosocial'] = trim($razaoSocial[0]['entrazaosocial']);
  
        $sqlEstado = "INSERT INTO eja.loteestado(
            lesnumero, lesuf, lesrazaosocial, lesesfera, lescnpj, 
            lesqtaluno, lesvlcusteio, lesvlparcela, lesnumparcela, lesdtgerado, 
            lesstatus, lesexercicio, lesenumportaria, lesedataportaria )
    VALUES ('{$loteEstado['lotnumero']}','{$loteEstado['lotuf']}', '{$razaoSocial[0]['entrazaosocial']}', '1', '{$razaoSocial[0]['entnumcpfcnpj']}', 
            '$qtaluno', '$custeio', '$custeio', '{$loteEstado['lotnumparcela']}', NOW(), 'A',
            '{$loteEstado['lotexercicio']}', '{$loteEstado['lotnumportaria']}' , '{$loteEstado['lotedataportaria']}')";

            $db->executar($sqlEstado);
            
            $sqlTurmaAluno = "
                    UPDATE eja.novaturmaeja SET  nteenviofnde2p = 'E',  ntedataenviofnde2p = 'NOW()' WHERE nteid IN (select distinct nteid from  eja.novaturmaeja nov inner join eja.lote lot on lot.lotnumero = nov.ntenumerolote where lot.lotnumero = '{$lotnumero}');
                    UPDATE eja.alunoturmaeja SET atestatus2p = 'E' WHERE nteid IN (select distinct nteid from  eja.novaturmaeja nov inner join eja.lote lot on lot.lotnumero = nov.ntenumerolote where lot.lotnumero = '{$lotnumero}');
                ";
            $db->executar($sqlTurmaAluno);
            $db->commit($sqlTurmaAluno);
            
			if($db->commit($sqlEstado)){
				$mensagem = 'Lote para Estado criado com sucesso!';
			}else{
				$mensagem = 'Não foi possível criar Lote para Estado, tente novamente mais tarde!';
			}
    }
    echo $mensagem;
}

/**
 * functionName mascaraMoeda
 *
 * @author Maykel
 *
 * @info Formata um valor numérico no formato de reais, sem o R$.
 * @param mixed $valor Valor para ser formatado.
 * @return String
 *
 * @version v1
 */
function mascaraMoeda($valor) {
    $valor = number_format($valor, 2, ',', '.');

    if (false !== strpos($valor, '-')) {
        $valor = '<span style="color:red"><b>' . $valor . '</b></span>';
    }

    return $valor;
}

/**
 * functionName mascaraNumero
 *
 * @author Maykel
 *
 * @info Formata um valor numérico no formato tradicional 2 casas.
 * @param mixed $valor Valor para ser formatado.
 * @return String
 *
 * @version v1
 */
function mascaraNumero($valor) {
    $valor = number_format($valor, 0, ',', '.');
    if (false !== strpos($valor, '-')) {
        $valor = '<span style="color:red"><b>' . $valor . '</b></span>';
    }

    return $valor;
}

/**
 * functionName salvarDadosCriacaoLote
 *
 * @author Luciano F. Ribeiro
 *
 * @param string $dados REQUEST do formulario
 * @return string persistencia dos dados
 *
 * @version v1
 */
function salvarDadosCriacaoLote($dados) {
    global $db;

    if (!$dados['linha_mun']) {
        echo "<script>alert('Favor selecionar pelo menos um município, para que possa ser gerado o lote.');window.location.href = 'eja.php?modulo=principal/gerarlote/gerar_lote&acao=A'</script>";
    }
    $linha_mun = $dados['linha_mun'];
    $turma_mun = $dados['turma_mun'];


    $sql = "
            SELECT MAX(lotnumero) AS lotnumero FROM eja.lote;
        ";
    $lotnumero = $db->pegaUm($sql) + 1;

    foreach ($linha_mun as $valor) {

        #ESTA CONDIÇÃO É NECESSÁRIO DEVIDO O USO DE UM addcslashes EM PRODUÇÃO. E ESSE CONDIÇÃO FAZ TODA A DIFERENÇA CONO TRATAMENTO DO ENCODE.
//        if ($_SERVER['SERVER_ADDR'] != '127.0.0.1') {
//            $valor = stripcslashes($valor);
//        }
        $valor = (str_replace("\\", '', $valor));
        $valor = (str_replace("'", '"', $valor));
        $valor = json_decode($valor,true);

        //$valor['mundescricao'] = iconv("UTF-8", "ISO-8859-1", $valor['mundescricao']);

        $lotvltparcela = round(($valor['valor_total_alunos'] / 2), 2);

        $sql = "
                INSERT INTO eja.lote(
                    lotnumero, lotuf, lotnomemunicipio, lotmuncod, lotcnpj, lotqtaluno,
                    lotvlcusteio, lotvlparcela, lotnumparcela, lotdtgerado, lotstatus, lotexercicio
                )VALUES(
                    '{$lotnumero}', '{$valor['estuf']}', '{$valor['mundescricao']}', '{$valor['muncod']}', '{$valor['entnumcpfcnpj']}', '{$valor['total_alunos']}',
                    '{$valor['valor_total_alunos']}', '{$lotvltparcela}', '1', 'NOW()', 'E', '{$valor['ano_exercicio']}'
                ) RETURNING lotid;
            ";
        $lotid = $db->pegaUm($sql);
    }

    if ($lotid > 0) {
        foreach ($turma_mun as $turmas) {
            $turmas = json_decode(str_replace("'", "\"", str_replace("\\", "", $turmas)), true);
            $turmas = implode(',', $turmas);

            $sql = "
                    UPDATE eja.novaturmaeja SET nteenviofnde = 'E', ntenumerolote = '{$lotnumero}', ntedataenviofnde = 'NOW()' WHERE nteid IN ({$turmas});
                    UPDATE eja.alunoturmaeja SET atestatus = 'E' WHERE nteid IN ({$turmas}) RETURNING ateid;
                ";
            $ateid = $db->pegaUm($sql);
        }
    }

    if ($ateid > 0) {
        $db->commit();
        $db->sucesso('principal/gerarlote/gerar_lote', '', "Foi gerado com sucesso o Lote de número: {$lotnumero}!");
    } else {
        $db->rollback();
        $db->insucesso('Não foi possível gerar o Lote, tente novamente mais tarde!', '', 'principal/gerarlote/gerar_lote&acao=A');
    }
}

/**
 * functionName salvarDadosSegundaParcela
 *
 * @author Kamyla Sakamoto
 *
 * @param string $dados REQUEST do formulario
 * @return string persistencia dos dados
 *
 * @version v1
 */
function salvarDadosSegundaParcela($dados) {
    global $db;

    if (!$dados['linha_mun']) {
        echo "<script>alert('Favor selecionar pelo menos um município, para que possa ser gerado lote da segunda parcela.');window.location.href = 'eja.php?modulo=principal/confirmarcpf/segunda_parcela&acao=A'</script>";
    }
    $linha_mun = $dados['linha_mun'];
    $turma_mun = $dados['turma_mun'];
//        ver($turma_mun,d);

    $sql = "
            SELECT MAX(lotnumero) AS lotnumero FROM eja.lote;
        ";
    $lotnumero = $db->pegaUm($sql) + 1;

    foreach ($linha_mun as $valor) {

        #ESTA CONDIÇÃO É NECESSÁRIO DEVIDO O USO DE UM addcslashes EM PRODUÇÃO. E ESSE CONDIÇÃO FAZ TODA A DIFERENÇA CONO TRATAMENTO DO ENCODE.
        if ($_SERVER['SERVER_ADDR'] != '127.0.0.1') {
            $valor = stripcslashes($valor);
        }
        $valor = json_decode(str_replace("'", '"', $valor), true);

        $valor['mundescricao'] = iconv("UTF-8", "ISO-8859-1", $valor['mundescricao']);

//            $lotvltparcela = round( ($valor['valor_total_alunos']/2), 2);

        $sql = "
                INSERT INTO eja.lote(
                    lotnumero, lotuf, lotmuncod, lotcnpj, lotqtaluno,
                    lotvlcusteio, lotvlparcela, lotnumparcela, lotdtgerado, lotstatus, lotexercicio
                )VALUES(
                    '{$lotnumero}', '{$valor['estuf']}', '{$valor['muncod']}', '{$valor['entnumcpfcnpj']}', '{$valor['total_alunos']}',
                    '{$valor['valor_custeio']}', '{$valor['valor_parcela']}', '2', 'NOW()', 'E', '{$valor['ano_exercicio']}'
                ) RETURNING lotid;
            ";
        $lotid = $db->pegaUm($sql);
    }


    if ($lotid > 0) {
        foreach ($turma_mun as $turmas) {
            $turmas = json_decode(str_replace("'", "\"", str_replace("\\", "", $turmas)), true);
            $turmas = implode(',', $turmas);

            $sql = "
                    UPDATE eja.novaturmaeja SET nteenviofnde2p = 'E', ntenumerolote2p = '{$lotnumero}', ntedataenviofnde2p = 'NOW()' WHERE nteid IN ({$turmas});
                    UPDATE eja.alunoturmaeja SET atestatus2p = 'E' WHERE nteid IN ({$turmas}) and atestatus2p = 'C' RETURNING ateid;
                ";
//                    ver($sql,d);
            $ateid = $db->pegaUm($sql);
        }
    }

    if ($ateid > 0) {
        $db->commit();
        $db->sucesso('principal/confirmarcpf/segunda_parcela', '', "Foi gerado com sucesso o Lote da 2ª Parcela de número: {$lotnumero}!");
    } else {
        $db->rollback();
        $db->insucesso('Não foi possível gerar o Lote, tente novamente mais tarde!', '', 'principal/confirmarcpf/segunda_parcela&acao=A');
    }
}

/**
 * functionName salvarDadosPortaria
 *
 * @author Luciano F. Ribeiro
 *
 * @param string $id dados do REQUEST.
 * @return string persiste os dados do formulario.
 *
 * @version v1
 */
function salvarDadosPortaria($dados) {
    global $db;

    $lotnumero = trim($dados['lotnumero']);
    $lotenumportaria = trim(strtoupper($dados['lotenumportaria']));
    $lotedataportaria = trim(strtoupper($dados['lotedataportaria']));

    if ($lotenumportaria != '' && $lotedataportaria != '') {
        $sqlEstado = "
                UPDATE eja.loteestado
                    SET lesenumportaria  = '{$lotenumportaria}',
                        lesedataportaria = '{$lotedataportaria}'
                WHERE lesnumero = {$lotnumero}";
        $db->executar($sqlEstado);
        
        $sql = "
                UPDATE eja.lote
                    SET lotenumportaria  = '{$lotenumportaria}',
                        lotedataportaria = '{$lotedataportaria}'
                WHERE lotnumero = {$lotnumero} RETURNING lotid;
            ";
        $lotid = $db->pegaUm($sql);
    }

    if ($lotid > 0) {
        $db->commit();
        $db->sucesso('principal/gerarlote/impressao_portaria', '&lotnumero=' . $lotnumero, "A Impressão da Portaria pode ser realizada. Clique no botão Imprimir!");
    } else {
        $db->rollback();
        $db->sucesso('principal/gerarlote/impressao_portaria', '&lotnumero=' . $lotnumero, "Não foi possível realizar a operação, tente novamente mais tarde!");
    }
}

/**
 * functionName serializa_linha
 *
 * @author Luciano F. Ribeiro
 *
 * @param string $id identificador necessario para criar a ação é particular a ação desejada.
 * @param string $data dados criados na listagem no grid. Para esse caso especifico é o "value" do checkbox.
 * @return string acão.
 *
 * @version v1
 */
function serializa_linha($id, $data) {
    global $db;
    if ($id != 2) {
        $sql = "
            SELECT  nte.nteid
            FROM eja.alunoturmaeja ate

            JOIN eja.novaturmaeja nte ON nte.nteid = ate.nteid
            JOIN entidade.entidade ent ON ent.entid = nte.pk_cod_entidade
            JOIN
            entidade.endereco ende ON ende.entid = ent.entid
            JOIN territorios.municipio mun ON mun.muncod = ende.muncod
            JOIN territorios.estado est ON est.estuf = mun.estuf

            WHERE mun.muncod = '{$data['muncod']}' AND ( (atecpf <> '' OR atecpf IS NOT NULL) AND (atestatus IS NULL OR atestatus <> 'E') ) AND nteenviofnde = 'V'
            GROUP BY nte.nteid
            ORDER BY nte.nteid
        ";
    } else {
        $sql = "
            SELECT  nte.nteid
            FROM eja.alunoturmaeja ate

            JOIN eja.novaturmaeja nte ON nte.nteid = ate.nteid
            JOIN entidade.entidade ent ON ent.entid = nte.pk_cod_entidade
            JOIN entidade.endereco ende ON ende.entid = ent.entid
            JOIN territorios.municipio mun ON mun.muncod = ende.muncod
            JOIN territorios.estado est ON est.estuf = mun.estuf

            WHERE mun.muncod = '{$data['muncod']}' AND ( (atecpf <> '' OR atecpf IS NOT NULL) AND (atestatus2p IS NULL OR atestatus2p <> 'E') ) AND nteenviofnde2p = 'V'
            GROUP BY nte.nteid
            ORDER BY nte.nteid
        ";
    }
    $nteid = $db->carregarColuna($sql);

//    foreach ($data as $key => $resposta) {
//        $data[$key] = iconv("ISO-8859-1", "UTF-8", $data[$key]);
//    }
    
    
    $dadosSerializados = str_replace('"', "'", simec_json_encode($data));
    $dadosTurmas = str_replace('"', "'", simec_json_encode($nteid));

    $result = "
            <input type=\"checkbox\" name=\"linha_mun[]\" id=\"linha_{$data['muncod']}\" checked=\"checked\" value=\"{$dadosSerializados}\" onclick=\"desabilita_linha(this);\"/>
            <input type=\"hidden\" name=\"turma_mun[]\" id=\"turma_{$data['muncod']}\" value=\"{$dadosTurmas}\"/>
        ";
    return $result;
}

//Usuarios de PI pode usar o eja independente de data limite
function verificaUsuEsp($cpf) {
    global $db;
    $dataAtual = date('Y/m/d');
    $dataLimite = date('2015/08/17');
    $perfil = pegaPerfilGeral();

    if (strtotime($dataAtual) >= strtotime($dataLimite)) {
        $sqlMun = "select muemuncod from eja.munespecial";
        $arrMunEsp = implode("','", $db->carregarColuna($sqlMun));

        $sqlEntid = "select muecodinep from eja.munespecial";
        $arrEntidEsp = implode("','", $db->carregarColuna($sqlEntid));
        if (in_array(PERFIL_EJA_GESTOR_MUNICIPAL, $perfil) && $arrMunEsp) {
            $sqlUsu = "select usucpf from eja.usuarioresponsabilidade where  rpustatus = 'A' and muncod IN ('{$arrMunEsp}') and usucpf = '$cpf'";
        } else if (in_array(PERFIL_EJA_GESTOR_ESTADUAL, $perfil)) {
            $sqlUsu = "select usucpf from eja.usuarioresponsabilidade where  rpustatus = 'A' and estuf = 'PI' and usucpf = '$cpf'";
        } else if (in_array(PERFIL_EJA_DIRETOR_ESCOLA, $perfil) && $arrEntidEsp) {
            $sqlUsu = "select usucpf from eja.usuarioresponsabilidade where  rpustatus = 'A' and entid in ('{$arrEntidEsp}') and usucpf = '$cpf'";
        }
        if ($sqlUsu) {
            $result = $db->pegaUm($sqlUsu);
        }
    } 
    return $result;
}
