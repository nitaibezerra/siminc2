<?PHP

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    function atualizaComboUnidade( $dados ){
        global $db;

        $entid = $_SESSION['academico']['entid'];
        $orgid = $_SESSION['academico']['orgid'];

        $opc = $dados['opc'];

        if( $opc == 'U' ){
            $AND = "AND cmpdatainauguracao IS NULL";
        }

        if ($orgid == 1) {
            $funid = ACA_ID_CAMPUS;
        } else {
            $funid = ACA_ID_UNED;
        }

        $sql = "
            SELECT  e.entid AS codigo,
                    upper(e.entnome) AS descricao
            FROM entidade.entidade e2

            JOIN academico.campus c ON c.entid = e2.entid
            JOIN entidade.entidade e ON e2.entid = e.entid
            JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
            JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
            JOIN entidade.endereco ed ON ed.entid = e.entid
            JOIN territorios.municipio mun ON mun.muncod = ed.muncod

            WHERE ea.entid = {$entid} AND e.entstatus = 'A' AND ef.funid = {$funid} {$AND}

            ORDER BY e.entnome
        ";
        $db->monta_combo('entidcampus', $sql, 'S', "Selecione a Unidade/Obra...", 'buscarEndereco', '', '', 548, 'N', 'entidcampus', '', $entidcampus, 'Unidade/Obra', '', 'chosen-select');
        die();
    }


    function buscaDirigentes( $dados ){
        global $db;

        $opc    = $dados['opc'];
        $entid  = $dados['entid'];

        if( $opc == 'R' ){
            $AND    = " AND fea2.entid = {$_SESSION['academico']['entid']}";
            $funid  = '21';
        }else{
            $AND    = " AND fea2.entid = {$entid}";
            $funid  = '24';
        }

        $sql = "
            SELECT  ent.entnome
            FROM entidade.funcao fun

            LEFT JOIN entidade.funcaoentidade fen ON fen.funid = fun.funid AND fen.entid IN (
                    SELECT  fen2.entid
                    FROM entidade.funentassoc fea2
                    LEFT JOIN entidade.funcaoentidade fen2 on fea2.fueid = fen2.fueid
                    WHERE fun.funid = fen2.funid {$AND}
            )

            LEFT JOIN entidade.entidade ent ON fen.entid = ent.entid

            WHERE fun.funid = {$funid}
        ";
        $resp['entnome'] = $db->pegaUm($sql);
        
        if($resp != ''){
            echo simec_json_encode($resp);
        } else {
            $resp["entnome"] = "";
            echo simec_json_encode($resp);
        }
        die();
    }

    function buscaDadosInauguracao($ingid) {
        global $db;
        $sql = "
            SELECT  inu.*, endObra.*
            FROM academico.inauguracao inu
            INNER JOIN academico.endobra endObra ON endObra.ingid = inu.ingid
            WHERE inu.ingid = {$ingid}
        ";
        $inauguracao = $db->carregar($sql);

        if (!empty($inauguracao[0])) {
            $inauguracao[0]['ingdtinstprov'] = formata_data($inauguracao[0]['ingdtinstprov']);
            $inauguracao[0]['ingdtinstdef'] = formata_data($inauguracao[0]['ingdtinstdef']);
            $inauguracao[0]['ingdtinclusao'] = formata_data($inauguracao[0]['ingdtinclusao']);
            $inauguracao[0]['ingsuginauguracao'] = formata_data($inauguracao[0]['ingsuginauguracao']);
            $inauguracao[0]['ingvlrinvestimento'] = formata_valor($inauguracao[0]['ingvlrinvestimento']);
            $inauguracao[0]['ingvlrinvmobequip'] = formata_valor($inauguracao[0]['ingvlrinvmobequip']);
            return $inauguracao[0];
        }
    }

    function buscarEndereco( $dados ){
        global $db;

        $opc    = $dados['opc'];
        $entid  = $dados['entid'];

        if( $opc == 'R' && $entid == 'NI' ){
            $entid = $_SESSION['academico']['entid'];
        }

        $sql = "
            SELECT  e.endcep, e.endlog, e.endcom, e.endbai, m.muncod, m.estuf, e.endnum
            FROM entidade.endereco e

            LEFT JOIN territorios.municipio m ON m.muncod = e.muncod

            WHERE e.entid = {$entid}
        ";
        $dados_end = $db->pegaLinha($sql);

        if($dados_end != ''){
//            $dados_end["endlog"] = iconv("ISO-8859-1", "UTF-8", $dados_end["endlog"]);
//            $dados_end["endcom"] = iconv("ISO-8859-1", "UTF-8", $dados_end["endcom"]);
//            $dados_end["endbai"] = iconv("ISO-8859-1", "UTF-8", $dados_end["endbai"]);
            echo simec_json_encode($dados_end);
        } else {
            $dados_end["endcep"] = "";
            $dados_end["endlog"] = "";
            $dados_end["endcom"] = "";
            $dados_end["endbai"] = "";
            $dados_end["muncod"] = "";
            $dados_end["estuf"] = "";
            $dados_end["endnum"] = "";
            echo simec_json_encode($dados_end);
        }
        die();
    }

    /**
    * functionName buscaListagemInauguracao
    *
    * @author Luciano F. Ribeiro
    *
    * @param string
    * @return string
    *
    * @version v1
    */
    function buscaListagemInauguracao ( $dados ){
        global $db;

        $entidies = $_SESSION['academico']['entid'];

        if( $dados['pesq_ingtipo'] != '' ){
            $ingtipo = $dados['pesq_ingtipo'];
            $WHERE .= " AND ingtipo = '{$ingtipo}'";
        }

        if( $dados['pesq_entidcampus'] != "" ){
            $entidcampus = $dados['pesq_entidcampus'];
            $WHERE .= " AND e2.entid = {$entidcampus}";
        }

        if( $dados['pesq_ingdscobra'] != "" ){
            $ingdscobra = $dados['pesq_ingdscobra'];
            $WHERE .= " AND ingdscobra iLike ('%{$ingdscobra}%')";
        }


        $acao = "
            '<img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer;\" onclick=\"excluirInauguracao('|| ingid ||')\">'
            '<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer; margin-left:5px;\" onclick=\"alterarInauguracao('|| ingid ||')\">'
            '<img align=\"absmiddle\" src=\"/imagens/pdf.gif\" width=\"18\" style=\"cursor: pointer; margin-left:5px;\" onclick=\"imprimirInauguracao('|| ingid ||')\">'
        ";

        $sql = "
            SELECT  {$acao},
                    e1.entnome AS instituicao,
                    CASE WHEN ingtipo = 'U'
                        THEN 'Unidade'
                        ELSE 'Obra'
                    END AS ingtipo,
                    e2.entnome AS unidade,
                    ingdscobra,
                    ingdscreitor,
                    to_char(ingdtinstprov, 'DD/MM/YYYY') AS ingdtinstprov,
                    to_char(ingdtinstdef, 'DD/MM/YYYY') AS ingdtinstdef

            FROM academico.inauguracao AS ina

            JOIN entidade.entidade e1 ON e1.entid = ina.entidies
            LEFT JOIN entidade.entidade e2 ON e2.entid = ina.entidcampus

            WHERE entidies = {$entidies} {$WHERE}
            ORDER BY ingid
        ";
        $cabecalho = array("Ação", "Instituição", "Tipo", "Unidade", "Obra", "Reitor", "Instalações Porvisórias", "Instalações Definitivas");
        $alinhamento = Array('center', '', '', '', '');
        $tamanho = Array('', '', '', '', '');

        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);

    }

    /**
     * functionName carregarFotosObrasInaug
     *
     * @author Luciano F. Ribeiro
     *
     * @param string
     * @return string
     *
     * @version v1
     */
    function carregarFotosObrasInaug($ingid) {
        global $db;
        if ($ingid > 0) {
            $sql = "
                SELECT  arq.arqid, arq.arqdescricao, fot.*
                FROM academico.fotosobra fot
                LEFT JOIN public.arquivo arq ON arq.arqid = fot.arqid
                WHERE fot.ingid = {$ingid}
                ORDER BY 1
            ";
            $fotos = $db->carregar($sql);

            if ($fotos[0] != '') {
                $i = 1;
                foreach ($fotos as $fot) {
                    $fotosp[$i] = $fot;
                    $i++;
                }
            }
            return $fotosp;
        } else {
            return false;
        }
    }

    /**
     * functionName cadastroNovoCurso
     *
     * @author Luciano F. Ribeiro
     *
     * @param string
     * @return string formulario
     *
     * @version v1
     */
    function cadastroNovoCurso() {
        global $db;
?>
        <form name="formulario_curso" id="formulario_curso" method="POST">
            <input type="hidden" name="requisicao_curso" id="requisicao_curso" value=""/>
            <input type="hidden" name="crsid" id="crsid" value=""/>

            <table bgcolor="#f5f5f5" align="center" class="tabela">
                <tr>
                    <td class="subtitulodireita" colspan="2" style="text-align: center; font-size: 12px;"><h4>Cadastro de Cursos</h4></td>
                </tr>
                <tr>
                    <td class="subtitulodireita" width="25%">Nome do Curso:</td>
                    <td>
                        <?PHP
                            echo campo_texto('crsdsc', 'S', 'S', 'Nome do Curso', 57, 100, '', '', '', '', '', 'id="crsdsc"', '', '');
                        ?>
                    </td>
                </tr>
                <tr id="tr_nome">
                    <td class="subtitulodireita">Número de Matrículas:</td>
                    <td>
                        <?PHP
                            echo campo_texto('crsmatricula', 'S', 'S', 'Número de Matrículas', 57, 50, '######', '', '', '', '', 'id="crsmatricula"', '', '');
                        ?>
                    </td>
                </tr>
                <tr id="tr_sigla">
                    <td class="subtitulodireita">Tipo:</td>
                    <td>
                        <?PHP
                            $sql = "
                                SELECT  tpcid AS codigo,
                                        tpcdsc AS descricao
                                FROM academico.tipocurso
                                ORDER BY descricao
                            ";
                            $db->monta_combo('tpcid', $sql, 'S', "Selecione...", '', '', '', 382, 'S', 'tpcid', '', $tpcid, 'Tipo de Curso');
                        ?>
                    </td>
                </tr>
                <tr id="tr_mantenedora">
                    <td class="subtitulodireita">Regular ou BF ou e-Tec:</td>
                    <td colspan="2">
                        <?PHP
                            $sql = array(
                                array('codigo' => 'R', 'descricao' => 'Modalidade - Regular'),
                                array('codigo' => 'B', 'descricao' => 'Modalidade - Bolsa formação'),
                                array('codigo' => 'E', 'descricao' => 'Modalidade - e-Tec')
                            );
                            $db->monta_combo('crsmodalidade', $sql, 'S', "Selecione...", '', '', '', 382, 'S', 'crsmodalidade', '', $crsmodalidade, 'Tipo de Curso');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" colspan="2" style="text-align:center">
                        <input type="button" name="salvar" value=" Salvar " id="salvar" onclick="salvarDadosFomulario('C');"> 
                        <input type="button" name="btnFechar" value="Cancelar" id="btnFechar" class="modalCloseImg simplemodal-close">
                    </td>
                </tr>
            </table>
        </form>
<?PHP
        die();
    }

    /**
     * functionName excluirInauguracao
     *
     * @author Luciano F. Ribeiro
     *
     * @param string
     * @return string
     *
     * @version v1
     */
    function excluirInauguracao( $dados ){
        global $db;

        $ingid = $dados['ingid'];

        $sql = "
            DELETE FROM academico.curso WHERE ingid = {$ingid};
            DELETE FROM academico.endobra WHERE ingid = {$ingid};
            DELETE FROM academico.fotosobra WHERE ingid = {$ingid};
            DELETE FROM academico.inauguracao WHERE ingid = {$ingid}
            RETURNING ingid
        ";
        $ingid = $db->pegaUm($sql);

        if ($ingid > 0) {
            $db->commit();
            $db->sucesso('principal/edu_profissional/autorizacao_inauguracao');
        } else {
            $db->rollback();
            $db->insucesso('Operação não Realizada. Por favor tente novamente mais tarde!', '', 'principal/edu_profissional/autorizacao_inauguracao&acao=A');
        }
    }

    function excluirImagemIng( $dados ) {
        global $db;

        $arqid = $dados['arqid'];

        if ($arqid != '') {
            $sql = " DELETE from academico.fotosobra WHERE arqid = {$arqid} ";
        }

       if ($db->executar($sql) ) {
            $db->commit();
            $db->sucesso( 'principal/edu_profissional/autorizacao_inauguracao', '', 'Foto excluida com sucesso!');
        } else {
            $db->rollback();
            $db->insucesso( 'Operação não Realizada. Por favor tente novamente mais tarde!', '', 'principal/edu_profissional/autorizacao_inauguracao&acao=A' );
        }
    }

    /**
     * functionName excluirCursoGridTemp
     *
     * @author Luciano F. Ribeiro
     *
     * @param string
     * @return string
     *
     * @version v1
     */
    function excluirCursoGridTemp($dados) {
        global $db;
        $ingid = (int) $dados['ingid'];
        $crsid = (int) $dados['id'];

        if (!empty($ingid) AND ! empty($crsid)) {
            $sql = "DELETE FROM academico.curso WHERE crsid = {$crsid} AND ingid = {$ingid} ";
            $resp = $db->executar($sql);
            $db->commit();
        }

        unset($_SESSION['grid']['cursos'][$crsid]);
        sort($_SESSION['grid']['cursos']);

        foreach ($_SESSION['grid']['cursos'] as $key => $value) {
            $value['acao'] = "<img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirCursoGridTemp('{$key}')\">";
            $_SESSION['grid']['cursos'][$key] = $value;
        }
        $array = $_SESSION['grid']['cursos'];

        $cabecalho = array("Ação", "Curso", "Número de matrículas", "Tipo", "Regular ou Bf ou e-Tec");
        $alinhamento = Array('center', '', '', '', '');
        $tamanho = Array('5%', '', '', '', '');
        $db->monta_lista($array, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
        die();
    }

    /**
     * functionName salvarCurso
     *
     * @author Luciano F. Ribeiro
     *
     * @param string
     * @return string
     *
     * @version v1
     */
    function salvarCurso($dados) {
        global $db;

        $n_array_cursos = count($_SESSION['grid']['cursos']);

        if ($n_array_cursos > 0 && is_array($_SESSION['grid']['cursos'])) {
            sort($_SESSION['grid']['cursos']);
            $count = $n_array_cursos++;
        } else {
            $count = 0;
        }

        $acao = "
            <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirCursoGridTemp('{$count}')\">
        ";

        $sql = array("acao" => $acao, "crsdsc" => "{$dados['crsdsc']}", "crsmatricula" => "{$dados['crsmatricula']}", "tpcid" => "{$dados['tpcid']}", "crsmodalidade" => "{$dados['crsmodalidade']}");

        if (!is_array($_SESSION['grid']['cursos'])) {
            $_SESSION['grid']['cursos'] = array();
        }

        array_push($_SESSION['grid']['cursos'], $sql);
        $sql_array = $_SESSION['grid']['cursos'];

        $cabecalho = array("Ação", "Curso", "Número de matrículas", "Tipo", "Regular ou Bf ou e-Tec");
        $alinhamento = Array('center', '', '', '', '');
        $tamanho = Array('5%', '', '', '', '');
        $db->monta_lista($sql_array, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
        die();
    }

    /**
     * functionName salvarCurso
     *
     * @author Luciano F. Ribeiro
     *
     * @param string
     * @return string
     *
     * @version v1
     */
    function salvarInauguracao($dados) {
        global $db;

        $usucpf = $_SESSION['usucpf'];

        $ingid = trim($dados['ingid']);

        $entidies = $dados['entidies'];
        $entidcampus = $dados['entidcampus'] == '' ? 'NULL' : $dados['entidcampus'];

        $ingtipo            = $dados['ingtipo'];
        
        $ingdscreitor       = trim( $dados['ingdscreitor'] );
        $ingnomedirigente   = trim( $dados['ingnomedirigente'] );

        $ingdtinstprov      = $dados['ingdtinstprov'];
        $ingdtinstdef       = $dados['ingdtinstdef'];
        $ingsuginauguracao  = $dados['ingsuginauguracao'];
        $ingdscobra         = addslashes(trim($dados['ingdscobra']));
        $ingnumdocprevisto  = $dados['ingnumdocprevisto'];
        $ingnumdocnomeados  = $dados['ingnumdocnomeados'];
        $ingnumtecprevisto  = $dados['ingnumtecprevisto'];
        $ingnumtecnomeados  = $dados['ingnumtecnomeados'];
        $ingvlrinvestimento = formata_valor_sql($dados['ingvlrinvestimento']);
        $ingvlrinvmobequip  = formata_valor_sql($dados['ingvlrinvmobequip']);

        #OBSERVAÇÕES
        $inghistoricound        = addslashes(trim($dados['inghistoricound'])); #REFERENTE A PERGUNTA M
        $inginfgeral            = addslashes(trim($dados['inginfgeral'])); #REFERENTE A PERGUNTA N
        $inginfcondobra         = addslashes(trim($dados['inginfcondobra'])); #REFERENTE A PERGUNTA O
        $ingobservacoes         = addslashes(trim($dados['ingiobservacoes'])); #REFERENTE A PERGUNTA P
        $inginaugjustificativa  = addslashes(trim($dados['inginaugjustificativa'])); #REFERENTE A PERGUNTA JUSTIFICATIVA

        #ENDEREÇO
        $edoid          = $dados['edoid'];
        $edocep         = str_replace("-", "", $dados['edocep']);
        $edodsc         = $dados['edodsc'];
        $edocomplemento = $dados['edocomplemento'];
        $edobairro      = $dados['edobairro'];
        $estuf          = $dados['estuf'];
        $muncod         = $dados['muncod'];

        if ($ingid == '') {
            $sql = "
                    INSERT INTO academico.inauguracao(
                            entidies, entidcampus, usucpf, ingtipo, ingdtinstprov, ingdscobra, ingdtinstdef, ingdscreitor, ingnomedirigente, ingnumdocprevisto, ingnumdocnomeados,
                            ingnumtecprevisto, ingnumtecnomeados, ingvlrinvestimento, ingvlrinvmobequip, inghistoricound, inginfcondobra, inginfgeral, ingiobservacoes,
                            ingsuginauguracao, inginaugjustificativa
                        ) VALUES (
                            {$entidies}, {$entidcampus}, '{$usucpf}', '{$ingtipo}', '{$ingdtinstprov}', '{$ingdscobra}', '{$ingdtinstdef}', '{$ingdscreitor}', '{$ingnomedirigente}', '{$ingnumdocprevisto}',
                            '{$ingnumdocnomeados}', '{$ingnumtecprevisto}', '{$ingnumtecnomeados}', '{$ingvlrinvestimento}', '{$ingvlrinvmobequip}', '{$inghistoricound}', '{$inginfcondobra}', '{$inginfgeral}',
                            '{$ingobservacoes}', '{$ingsuginauguracao}', '{$inginaugjustificativa}'
                    ) RETURNING ingid;
                ";
                $opc = 'I';
        } else {
            $sql = "
                    UPDATE academico.inauguracao
                        SET entidcampus         = {$entidcampus},
                            ingtipo             = '{$ingtipo}',
                            ingdtinstprov       = '{$ingdtinstprov}',
                            ingdscobra          = '{$ingdscobra}',
                            ingdtinstdef        = '{$ingdtinstdef}',
                            ingdscreitor        = '{$ingdscreitor}',
                            ingnomedirigente    = '{$ingnomedirigente}',
                            ingnumdocprevisto   = '{$ingnumdocprevisto}',
                            ingnumdocnomeados   = '{$ingnumdocnomeados}',
                            ingnumtecprevisto   = '{$ingnumtecprevisto}',
                            ingnumtecnomeados   = '{$ingnumtecnomeados}',
                            ingvlrinvestimento  = '{$ingvlrinvestimento}',
                            ingvlrinvmobequip   = '{$ingvlrinvmobequip}',
                            inghistoricound     = '{$inghistoricound}',
                            inginfcondobra      = '{$inginfcondobra}',
                            inginfgeral         = '{$inginfgeral}',
                            ingiobservacoes     = '{$ingobservacoes}',
                            ingsuginauguracao   = '{$ingsuginauguracao}',
                            inginaugjustificativa = '{$inginaugjustificativa}'
                        WHERE ingid = {$ingid} RETURNING ingid;
                ";
                $opc = 'U';
        }
        $resp_ingid = $db->pegaUm($sql);

        if ($resp_ingid > 0) {
            if ($edoid == '') {
                $sql = "
                        INSERT INTO academico.endobra(
                                ingid, estuf, edodsc, edocomplemento, edobairro, muncod, edocep
                            )VALUES(
                                {$resp_ingid}, '{$estuf}', '{$edodsc}', '{$edocomplemento}', '{$edobairro}', '{$muncod}', '{$edocep}'
                        ) RETURNING edoid;
                    ";
            } else {
                $sql = "
                        UPDATE academico.endobra
                            SET estuf           = '{$estuf}',
                                edodsc          = '{$edodsc}',
                                edocomplemento  = '{$edocomplemento}',
                                edobairro       = '{$edobairro}',
                                muncod          = '{$muncod}',
                                edocep          = '{$edocep}'
                        WHERE edoid = {$edoid} RETURNING edoid;
                    ";
            }
            $resp_edoid = $db->pegaUm($sql);
        }

        if ($resp_ingid > 0) {
            $array_curos = $_SESSION['grid']['cursos'];

            $sql_sel = " SELECT crsid FROM academico.curso WHERE ingid = {$resp_ingid} ";
            $resp = $db->pegaUm($sql_sel);

            if ($resp > 0) {
                $sql_del = " DELETE FROM academico.curso WHERE ingid = {$resp_ingid} RETURNING crsid; ";
                $resp_crsid = $db->pegaUm($sql_del);
            }

            if( count($array_curos) > 0 ){
                $sql_curso = '';

                foreach( $array_curos as $value ){
                    $crsdsc = $value['crsdsc'];
                    $crsmatricula = $value['crsmatricula'];
                    $tpcid = $value['tpcid'];
                    $crsmodalidade = $value['crsmodalidade'];

                    $sql_curso .= "
                        INSERT INTO academico.curso(
                                ingid, tpcid, crsmatricula, crsdsc, crsmodalidade
                            ) VALUES (
                                {$resp_ingid}, {$tpcid}, '{$crsmatricula}', '{$crsdsc}', '{$crsmodalidade}'
                        ) RETURNING crsid;
                    ";
                }
                $resp_curso = $db->executar($sql_curso);
            }

            if( $resp_curso > 0 ){
                unset($_SESSION['grid']['cursos']);
            }
        }

        $files = $_FILES;
        if (is_array($files)) {
            foreach ($files as $campo => $file) {

                if (!empty($file['tmp_name'])) {
                    $file = new FilesSimec('fotosobra', array('ingid' => $resp_ingid), 'academico');
                    $file->setUpload($campo, $campo);
                }
            }
        }

        if( $resp_ingid > 0 ){

            criarDocumentoInauguracao( $resp_ingid );

            $db->commit();
            $db->sucesso( 'principal/edu_profissional/autorizacao_inauguracao' );
        } else {
            $db->rollback();
            $db->insucesso( 'Operação não Realizada. Por favor tente novamente mais tarde!', '', 'principal/edu_profissional/autorizacao_inauguracao&acao=A' );
        }
    }

    /**
     * functionName atualizaComboMunicipio
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $CEP cep
     * @return string json "array" com dados referentes ao endereço.
     *
     * @version v1
     */
    function atualizaComboMunicipioInauguracao($dados) {
        global $db;

        $estuf = $dados['estuf'];
        $muncod = $dadosInstitucional['muncod'];

        $sql = "
            SELECT  muncod AS codigo,
                    mundescricao AS descricao
            FROM territorios.municipio
            WHERE estuf =  '{$estuf}'
            ORDER BY descricao
        ";
        $db->monta_combo('muncod', $sql, 'S', "Selecione...", '', '', '', 180, 'S', 'muncod', '', $muncod, 'Município');
        die();
    }

    /**
     * functionName buscarEndereceCEP
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $CEP cep
     * @return string json "array" com dados referentes ao endereço.
     *
     * @version v1
     */
    function buscarEndereceCEP($dados) {
        global $db;

        $cep = str_replace('-', '', $dados['cep']);

        $sql = "
            SELECT * FROM cep.v_endereco2 WHERE cep = '{$cep}'
        ";
        $dados = $db->pegaLinha($sql);

        if ($dados != '') {
            $dados["logradouro"] = iconv("ISO-8859-1", "UTF-8", $dados["logradouro"]);
            $dados["bairro"] = iconv("ISO-8859-1", "UTF-8", $dados["bairro"]);
            $dados["muncod"] = iconv("ISO-8859-1", "UTF-8", $dados["muncod"]);
            echo simec_json_encode($dados);
        } else {
            $dados["logradouro"] = "";
            $dados["bairro"] = "";
            $dados["estado"] = "";
            $dados["muncod"] = "";
            echo simec_json_encode($dados);
        }
        die;
    }


    # ------------------------------------------------ WORK-FLOW ---------------------------------------- #

    function pegarDocidInauguracao( $ingid ){
	global $db;

	$sql = "
            SELECT docid
            FROM academico.inauguracao
            WHERE ingid = {$ingid}
	";
	return $db->pegaUm($sql);
    }

    function criarDocumentoInauguracao( $ingid ){
	global $db;

	require_once APPRAIZ . 'includes/workflow.php';

        $tpdid = WF_FLUXO_INAUGURACOES_INSTITUTOS;

	$docid = pegarDocidInauguracao( $ingid );
	if( $docid == '' ){
            $docdsc = "Autorização de Inauguração de n° {$ingid}";
            $docid = wf_cadastrarDocumento($tpdid, $docdsc);
            $sql = " UPDATE academico.inauguracao SET docid = {$docid} WHERE ingid = {$ingid} ";
            $db->executar($sql);
            $db->commit();
	}
	return $docid;
    }