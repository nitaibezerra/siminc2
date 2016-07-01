<?PHP

    /**
    * functionName buscarDadosAdicionaisDirigentes
    *
    * @author Luciano F. Ribeiro
    *
    * @param string
    * @return string formulario
    *
    * @version v1
    */
    function buscarDadosAdicionaisDirigentes( $mcsid ){
        global $db;

        $sql = "
            SELECT  mcsid,
                    arqid,
                    tpmid,
                    entid,
                    to_char(mcsvigenciainicial, 'DD/MM/YYYY') AS mcsvigenciainicial,
                    to_char(mcsvigenciafinal, 'DD/MM/YYYY') AS mcsvigenciafinal,
                    mcssituacao,
                    mcsdscmembro,
                    tipomandato,
                    nummandato

            FROM academico.membroconselho
            WHERE mcsid = {$mcsid}
        ";
        return $db->pegaLinha($sql);
    }

    /**
    * functionName exibirListaDirigentes
    *
    * @author Luciano F. Ribeiro
    *
    * @param string
    * @return string grid.
    *
    * @version v1
    */
    function exibirListaDirigentes( $dados ){
        global $db;

        $entid = $dados['entid'];

        $acao_dow = "
            <a href=\"academico.php?modulo=principal/dadosdirigentes&acao=A&download=S&arqid='|| mb.arqid ||'\">
                <img src=\"../imagens/anexo.gif\" border=\"0\">
            </a>
        ";

        $sql = "
            SELECT  DISTINCT
                    CASE
                        WHEN fun.funid = 21 THEN '<b>Reitor(a):</b>'
                        WHEN fun.funid = 24 THEN '<b>Dirigente do Campus:</b>'
                        WHEN fun.funid = 40 THEN '<b>Interlocutor Institucional:</b>'
                    END AS funid,

                    ent.entnome,
                    to_char(mb.mcsvigenciainicial, 'DD/MM/YYYY') AS mcsvigenciainicial,
                    to_char(mb.mcsvigenciafinal, 'DD/MM/YYYY') AS mcsvigenciafinal,

                    CASE WHEN mb.mcssituacao = 't'
                        THEN 'Ativo'
                        ELSE CASE WHEN mb.mcssituacao = 'f'
                                THEN 'Inativo'
                                ELSE ''
                             END
                    END AS mcssituacao,

                    CASE WHEN mb.arqid > 0
                        THEN '{$acao_dow}'
                        ELSE CASE WHEN mb.mcsid IS NULL
                                THEN ''
                                ELSE 'Não Anexado'
                             END
                    END AS arqid
            FROM entidade.funcaoentidade AS fen

            LEFT JOIN entidade.funcao AS fun ON fun.funid = fen.funid
            LEFT JOIN academico.membroconselho AS mb ON mb.entid = fen.entid
            LEFT JOIN academico.tipomembro AS tm ON tm.tpmid = mb.tpmid
            LEFT JOIN entidade.entidade AS ent ON ent.entid = mb.entid
            LEFT JOIN entidade.funentassoc AS fua ON fua.fueid = fen.fueid AND fua.feaid = mb.feaid

            WHERE fua.entid = {$entid} AND mb.tpmid IS NULL AND fen.funid IN (24, 40)

            ORDER BY mcssituacao, funid ASC
        ";
        $cabecalho = array( "Cargo", "Dirigente", 'Inicio da Vigência' , 'Fim da Vigencia' ,  "Situação", "Ato Legal");
        $alinhamento = Array('', '', '', '', 'center');
        $tamanho = Array('' , '' , '' ,'','');

        $param['totalLinhas'] = false;

        echo '<table  bgcolor="#f5f5f5" align="center" class="tabela" border="0">';
        echo '<tr> <td colspan="6" class="subTituloCentro"> DIRIGENTES DOS CAMPUS </td> </tr>';
        echo '<tr> <td>';
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento, '', $param);
        echo '</td> </tr>';
        echo '</table>';


        echo '<br>';

# ---------------------------------------------------------- INTERLOCUTOR INSTITUCIONAL DO CAMPUS --------------------------------------------------- #

        $acao_dow = "
            <a href=\"academico.php?modulo=principal/dadosdirigentes&acao=A&download=S&arqid='|| mb.arqid ||'\">
                <img src=\"../imagens/anexo.gif\" border=\"0\">
            </a>
        ";

        $sql = "
            SELECT  DISTINCT
                    '<b>'||fun.fundsc||':</b>' AS fundsc,
                    ent.entnome,
                    tm.tpmdsc,
                    to_char(mb.mcsvigenciainicial, 'DD/MM/YYYY') AS mcsvigenciainicial,
                    to_char(mb.mcsvigenciafinal, 'DD/MM/YYYY') AS mcsvigenciafinal,

                    CASE WHEN mb.mcssituacao = 't'
                        THEN 'Ativo'
                        ELSE CASE WHEN mb.mcssituacao = 'f'
                                THEN 'Inativo'
                                ELSE ''
                             END
                    END AS mcssituacao,

                    CASE WHEN mb.arqid > 0
                        THEN '{$acao_dow}'
                        ELSE CASE WHEN mb.mcsid IS NULL
                                THEN ''
                                ELSE 'Não Anexado'
                             END
                    END AS arqid

            FROM entidade.funcaoentidade AS fen

            LEFT JOIN entidade.funcao AS fun ON fun.funid = fen.funid
            LEFT JOIN academico.membroconselho AS mb ON mb.entid = fen.entid
            LEFT JOIN academico.tipomembro AS tm ON tm.tpmid = mb.tpmid
            LEFT JOIN entidade.entidade AS ent ON ent.entid = mb.entid
            LEFT JOIN entidade.funentassoc AS fua ON fua.fueid = fen.fueid AND fua.feaid = mb.feaid

            WHERE fua.entid = {$entid} AND tm.tpmnivel = 'C' AND fen.funid IN (123)

            ORDER BY fundsc
        ";
        $cabecalho = array("Cargo", "Dirigente", "Inicio da Vigência" , "Membro", "Fim da Vigencia", "Situação", "Ato Legal");
        $alinhamento = Array('', '', '', '', 'center');
        $tamanho = Array('' , '' , '' ,'','');

        $param['totalLinhas'] = false;

        echo '<table  bgcolor="#f5f5f5" align="center" class="tabela" border="0">';
        echo '<tr> <td colspan="6" class="subTituloCentro"> CONSELHO DO CAMPUS </td> </tr>';
        echo '<tr> <td>';
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento, '', $param);
        echo '</td> </tr>';
        echo '</table>';

        echo '<br>';
        die();
    }

    /**
    * functionName excluirDocAnexo
    *
    * @author Luciano F. Ribeiro
    *
    * @param string
    * @return string grid.
    *
    * @version v1
    */
    function excluirDocAnexo( $dados ) {
        global $db;

        $mcsid = $dados['mcsid'];
        $arqid = $dados['arqid'];

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        if( $mcsid != '' && $arqid != '' ){
            $sql = " UPDATE academico.membroconselho SET arqid = NULL WHERE mcsid = {$mcsid} ";
        }

        if( $db->executar($sql) ){
            $file = new FilesSimec('membroconselho', $campos = array(), 'academico');
            $file->excluiArquivoFisico( $arqid );

            $db->commit();
            $parametros = "&entid={$dados['entid']}&funid={$dados['funid']}&opc={$dados['opc']}";
            $db->sucesso('principal/dados_adicionais_dirigentes', $parametros);
        }
    }

    /**
    * functionName listagemCampusEndidade
    *
    * @author Luciano F. Ribeiro
    *
    * @param string
    * @return string formulario
    *
    * @version v1
    */
    function listagemCampusEndidade( $entid, $orgid ){
        global $db;

        switch ( $orgid ){
            case '1':
                $funid = ACA_ID_CAMPUS;
                break;
            case '2':
                $funid = ACA_ID_UNED;
                break;
        }

        $acoes = "
            <img id=\"img_dimensao_' || e.entid || '\" src=\"/imagens/mais.gif\" style=\"cursor: pointer\" onclick=\"carregarListaDirigentes(this.id,\'' || e.entid || '\');\"/>
        ";

        if( $funid != '' ){
            $sql = "
                SELECT  '{$acoes}',
                        '<a style=\"cursor:pointer;\" onclick=\"abredadoscampus(' || e.entid || ');\">' || upper(e.entnome) || '</a>' AS entnome,
                        upper(mun.mundescricao) AS municipio,
                        upper(mun.estuf) AS uf,
                        '<tr style=\"display:none;\" id=\"listaDirigentes_' || e.entid || '\" ><td></td><td id=\"trA_' || e.entid || '\" colspan=\"11\"></td></tr>' as listaDirigentes

                FROM entidade.entidade e2

                INNER JOIN entidade.entidade e ON e2.entid = e.entid
                INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
                INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
                LEFT JOIN entidade.endereco ed ON ed.entid = e.entid
                LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod

                WHERE ea.entid = {$entid} AND e.entstatus = 'A' AND ef.funid = {$funid}

                ORDER BY e.entnome
            ";
        }
        $cabecalho = array('Ação', 'Campus', 'Município', 'UF', '');
        $alinhamento = Array('center', '', '', '', '', '', '');
        $tamanho = Array('3%', '', '', '', '');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
    }

    /**
    * functionName listagemConselhoSuperior
    *
    * @author Luciano F. Ribeiro
    *
    * @param string
    * @return string formulario
    *
    * @version v1
    */
    function listagemConselhoSuperior( $entid ){
        global $db;

        if( $_SESSION['academico']['entidadenivel'] != 'campus' ){
            $and = "AND tm.tpmnivel = 'I' AND fen.funid IN (123)";
        }else{
            $and = "AND tm.tpmnivel = 'C' AND fen.funid IN (123)";
        }

        $acao_alt = "
            <img src=\"/imagens/alterar.gif\" style=\"cursor: pointer; margin-left:5px;\" onclick=\"editardirigente( \'' || ent.entid || '\',\' 123\', \'ED\', \'A\' );\" title=\"Cadastrar Dados\">
            <img src=\"/imagens/excluir.gif\" style=\"cursor: pointer; margin-left:5px;\" onclick=\"excluirDirigente( \'' || ent.entid || '\',\'' || fun.funid || '\',\'' || mb.mcsid || '\' );\" title=\"Excluir Dados\">
        ";
        $acao_dow = "
            <a href=\"academico.php?modulo=principal/dadosdirigentes&acao=A&download=S&arqid='|| mb.arqid ||'\">
                <img src=\"../imagens/anexo.gif\" border=\"0\">
            </a>
        ";

        $sql = "
            SELECT  DISTINCT
                    '{$acao_alt}' AS acao_alt,
                    '<b>'||fun.fundsc||':</b>' AS fundsc,
                    ent.entnome,
                    tm.tpmdsc,
                    to_char(mb.mcsvigenciainicial, 'DD/MM/YYYY') AS mcsvigenciainicial,
                    to_char(mb.mcsvigenciafinal, 'DD/MM/YYYY') AS mcsvigenciafinal,

                    CASE WHEN mb.mcssituacao = 't'
                        THEN 'Ativo'
                        ELSE CASE WHEN mb.mcssituacao = 'f'
                                THEN 'Inativo'
                                ELSE ''
                             END
                    END AS mcssituacao,

                    CASE WHEN mb.arqid > 0
                        THEN '{$acao_dow}'
                        ELSE CASE WHEN mb.mcsid IS NULL
                                THEN ''
                                ELSE 'Não Anexado'
                             END
                    END AS arqid

            FROM entidade.funcaoentidade AS fen

            JOIN entidade.funcao AS fun ON fun.funid = fen.funid
            LEFT JOIN academico.membroconselho AS mb ON mb.entid = fen.entid
            LEFT JOIN academico.tipomembro AS tm ON tm.tpmid = mb.tpmid
            LEFT JOIN entidade.entidade AS ent ON ent.entid = fen.entid
            LEFT JOIN entidade.funentassoc AS fua ON fua.fueid = fen.fueid
            LEFT JOIN entidade.funentassoc AS fua2 ON fua2.feaid = mb.feaid

            WHERE fua.entid = {$entid} {$and}

            ORDER BY fundsc
        ";
        $cabecalho = array( "Ação" , "Cargo", "Dirigente", "Tipo de Membro", "Inicio da Vigencia" , "Fim da Vigencia" ,  "Situação", "Ato Legal");
        $alinhamento = Array('center', 'right', '', '', 'center', 'center', 'center', 'center');
        $tamanho = Array('4%' , '' , '' , '' ,'','');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento, '', '');
    }

    /**
    * functionName listagemConselhoSuperior
    *
    * @author Luciano F. Ribeiro
    *
    * @param string
    * @return string formulario
    *
    * @version v1
    */
    function liatgemDirigentesInstituicao( $entid ){
        global $db;

       if( $_SESSION['academico']['entidadenivel'] != 'campus' ){
            $and = "AND mb.tpmid IS NULL AND fen.funid IN (21,40)";
            $order = "ORDER BY funid, mb.mcsvigenciafinal DESC";
        }else{
            $and = "AND mb.tpmid IS NULL AND fen.funid IN (24,40)";
            $order = "ORDER BY funid, mb.mcsvigenciafinal ASC";
        }

        $acao_alt = "
           <img src=\"/imagens/alterar.gif\" style=\"cursor: pointer; margin-left:5px;\" onclick=\"editardirigente( \'' || ent.entid || '\',\'' || fun.funid || '\', \'ED\', \'I\' );\" title=\"Alterar Dados\">
           <img src=\"/imagens/excluir.gif\" style=\"cursor: pointer; margin-left:5px;\" onclick=\"excluirDirigente( \'' || ent.entid || '\',\'' || fun.funid || '\',\'' || mb.mcsid || '\' );\" title=\"Excluir Dados\">
        ";

        $acao_dow = "
           <a href=\"academico.php?modulo=principal/dadosdirigentes&acao=A&download=S&arqid='|| mb.arqid ||'\">
               <img src=\"../imagens/anexo.gif\" border=\"0\">
           </a>
        ";

        $sql = "
            SELECT  '{$acao_alt}' as acao_p,

                    CASE
                        WHEN fun.funid = 21 THEN '<b>Reitor(a):</b>'
                        WHEN fun.funid = 24 THEN '<b>Dirigente do Campus:</b>'
                        WHEN fun.funid = 40 THEN '<b>Interlocutor Institucional:</b>'
                    END AS funid,

                    ent.entnome,
                    to_char(mb.mcsvigenciainicial, 'DD/MM/YYYY') AS mcsvigenciainicial,
                    to_char(mb.mcsvigenciafinal, 'DD/MM/YYYY') AS mcsvigenciafinal,

                    CASE WHEN mb.mcssituacao = 't'
                        THEN 'Ativo'
                        ELSE CASE WHEN mb.mcssituacao = 'f'
                                THEN 'Inativo'
                                ELSE ''
                             END
                    END AS mcssituacao,

                    CASE WHEN mb.arqid > 0
                        THEN '{$acao_dow}'
                        ELSE CASE WHEN mb.mcsid IS NULL
                                THEN ''
                                ELSE 'Não Anexado'
                             END
                    END AS arqid

            FROM entidade.funcaoentidade AS fen

            JOIN entidade.funcao AS fun ON fun.funid = fen.funid
            LEFT JOIN academico.membroconselho AS mb ON mb.entid = fen.entid
            LEFT JOIN academico.tipomembro AS tm ON tm.tpmid = mb.tpmid
            LEFT JOIN entidade.entidade AS ent ON ent.entid = fen.entid
            LEFT JOIN entidade.funentassoc AS fua ON fua.fueid = fen.fueid AND fua.feaid = mb.feaid
            LEFT JOIN entidade.funentassoc AS fua2 ON fua2.feaid = mb.feaid

            WHERE fua.entid = {$entid} AND mb.mcssituacao = 'f' {$and}

            {$order}
        ";
//            ver($sql,d);
        $cabecalho = array( "Ação" , "Cargo", "Dirigente", "Inicio da Vigencia" , "Fim da Vigencia" ,  "Situação", "Ato Legal");
        $alinhamento = Array('center', 'right', '', 'center', 'center', 'center', 'center');
        $tamanho = Array('4%' , '' , '' , '' ,'','');
        $param['totalLinhas'] = false;

        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento, '', $param);
    }

    /**
    * functionName listagemAtoLegalDirigentes
    *
    * @author Luciano F. Ribeiro
    *
    * @param string
    * @return string formulario
    *
    * @version v1
    */
    function listagemAtoLegalDirigentes( $mcsid ){
        global $db;

        $acao = "
            <img border=\"0\" onclick=\"excluirDocAnexo('|| a.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" />
            &nbsp;
            <a title=\"Download\" href=\"academico.php?modulo=principal/dados_adicionais_dirigentes&acao=A&download=S&arqid=' || a.arqid || '\">
                <img src=\"../imagens/anexo.gif\" border=\"0\">
            </a>
        ";
        $down = "<a title=\"Download\" href=\"academico.php?modulo=principal/dados_adicionais_dirigentes&acao=A&request=download&arqid=' || a.arqid || '\">' || a.arqdescricao || '</a>";

        $sql = "
            SELECT  '{$acao}' as acao,
                    '{$down}' as descricao,
                    a.arqnome||'.'||a.arqextensao,
                    to_char(arqdata, 'DD/MM/YYYY') as aqrdata
            FROM academico.membroconselho AS m
            JOIN public.arquivo AS a ON a.arqid = m.arqid
            WHERE arqstatus = 'A' AND mcsid = {$mcsid}
        ";
        $cabecalho = Array("Ação", "Descrição",  "Nome do arquivo", "Data da Inclusão");
        //$whidth = Array('20%', '60%', '20%');
        $align  = Array('center', '', '', '');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');
    }

    /**
    * functionName salvarDadosFomulario
    *
    * @author Luciano F. Ribeiro
    *
    * @param string
    * @return string formulario
    *
    * @version v1
    */
    function salvarDadosFomulario( $dados, $files ){
        global $db;

        $opc   = $_REQUEST['opc'];
        $entid = $dados['entid'];
        $funid = $dados['funid'];
        $feaid = $dados['feaid'];

        $mcsid                  = $dados['mcsid'];
        $dg_entid               = $dados['dg_entid'];
        $tpmid                  = $dados['tpmid'] == '' ? 'NULL' : $dados['tpmid'];
        $mcsvigenciainicial     = $dados['mcsvigenciainicial'];
        $mcsvigenciafinal       = $dados['mcsvigenciafinal'];
        $mcssituacao            = $dados['mcssituacao'];
        $mcsdscmembro           = $dados['mcsdscmembro'] ? "'".$dados['mcsdscmembro']."'" : 'NULL';
        $tipomandato            = $dados['tipomandato'] ?  "'".$dados['tipomandato']."'" : 'NULL';
        $nummandato             = $dados['nummandato'] ?  "'".$dados['nummandato']."'" : 'NULL';

        if( $mcsid == '' && $feaid != '' ){
            $sql = "
                INSERT INTO academico.membroconselho(
                        tpmid, entid, mcsvigenciainicial, mcsvigenciafinal, mcssituacao, feaid, mcsdscmembro, tipomandato, nummandato
                    ) VALUES (
                        {$tpmid}, '{$dg_entid}', '{$mcsvigenciainicial}', '{$mcsvigenciafinal}', '{$mcssituacao}', {$feaid}, {$mcsdscmembro}, {$tipomandato}, {$nummandato}
                ) RETURNING mcsid
            ";
            $resp_mcsid = $db->pegaUm($sql);
        }else{
            if( $mcsid != '' ){
                $sql = "
                    UPDATE academico.membroconselho
                        SET tpmid               = {$tpmid},
                            mcsvigenciainicial  = '{$mcsvigenciainicial}',
                            mcsvigenciafinal    = '{$mcsvigenciafinal}',
                            mcssituacao         = '{$mcssituacao}',
                            mcsdscmembro        = {$mcsdscmembro},
                            tipomandato         = {$tipomandato},
                            nummandato          = {$nummandato}
                    WHERE mcsid = {$mcsid} RETURNING mcsid
                ";
            $resp_mcsid = $db->pegaUm($sql);
            }
        }

        if( is_array($files) && $resp_mcsid > 0 ){
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            if($files['arquivo']['tmp_name'] ){
                $file = new FilesSimec('membroconselho', array(), 'academico');
                $file->setUpload('Rede federal - Dados Adicionais Dirigentes - Ato Legal', null, false, 'arqid');
                $arqid = $file->getIdArquivo();
            }

            if( $arqid > 0 ){
                $sql = "
                    UPDATE academico.membroconselho
                        SET arqid = {$arqid}
                    WHERE mcsid = {$resp_mcsid} RETURNING mcsid
                ";
                $db->executar($sql);
            }
        }

        if( $resp_mcsid > 0 ){
            $db->commit();
            $parametros = "&funid={$funid}&opc={$opc}";
            $db->sucesso( 'principal/dadosdirigentes', $parametros, 'Operação realizada com sucesso', 'S', 'S' );
        } else {
            $db->rollback();
            $db->insucesso( 'Operação não Realizada. Por favor tente novamente mais tarde!', '', 'principal/edu_profissional/autorizacao_inauguracao&acao=A' );
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
    function selecionarDirigenteCadastro( $dados ){
        global $db;

        $sit = $dados['sit'];

        if( $_SESSION['academico']['entidadenivel'] != 'campus' ){
            $funid = "21";
            $cargo = "Reitor(a)";
            $entid = $_SESSION['academico']['entid'];
        }else{
            $funid = "24";
            $cargo = "Dirigente do Campus";
            $entid = $_SESSION['academico']['entidcampus'];
        }
?>
        <table bgcolor="#f5f5f5" align="center" class="tabela">
            <tr>
                <td class="subtitulodireita" colspan="2" style="text-align: center;"><h4>Selecione o tipo de Dirigente</h4></td>
            </tr>
            <tr>
                <td class="subtitulodireita" width="40%">Selecione o Tipo de Dirigente a ser Cadastrado:</td>
                <td>
                    <input type="radio" name="pesq_ingtipo" id="ingtipo_r" value="R" class="modalCloseImg simplemodal-close" onclick="editardirigente(<?=$entid;?>, '<?=$funid;?>', 'NV', '<?=$sit;?>' );"> <?=$cargo;?>
                    <br>
                    <input type="radio" name="pesq_ingtipo" id="ingtipo_u" value="U" class="modalCloseImg simplemodal-close" onclick="editardirigente(<?=$entid;?>, '40', 'NV', '<?=$sit;?>' );"> Interlocutor Institucional
                </td>
            </tr>
        </table>
<?PHP
        die();
    }

    /**
    * functionName verificaMandato
    *
    * @author Luciano F. Ribeiro
    *
    * @param string
    * @return string número do mandado 1 - para o primeiro e 2 - para o segundo
    *
    * @version v1
    */    
    function verificaMandato($entid, $feaid, $mcsid){
        global $db;

        if( $mcsid > 0 ){
            $_WHERE = "WHERE mcsid = {$mcsid}";
        }else{
            $_WHERE = "WHERE entid = {$entid} AND feaid = {$feaid} AND mcssituacao = 't'";
        }

        $sql = "
            SELECT  nummandato,
                    CASE WHEN EXTRACT ( YEAR FROM AGE(mcsvigenciafinal, mcsvigenciainicial) ) >= 4
                        THEN 'S'
                        ELSE 'N'
                    END AS intevalo_anos_madato
                    
            FROM academico.membroconselho {$_WHERE}
        ";
        $nummandato = $db->pegaLinha($sql);
        return $nummandato;
    }

?>

