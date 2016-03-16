<?PHP
    #------------------------------------------------------------- FUNÇÕES MODULO RECONHECIMENTO DE SABERES E COMPETÊNCIA --------------------------------------------------#
    #AS FUNÇÕES SÃO: (EM ORDER ALFABETICA)
    # - atualizaComboMunicipio;
    # - buscarEndereceCEP;


    /**
     * functionName atualizaComboEixoTec_2
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer extid
     * @return string combo do eixo de técnologia 2.
     *
     * @version v1
    */
    function atualizaComboEixoTec_2( $dados ){
        global $db;

        $extid_tec_1 = $dados['extid_tec_1'] != '' ? $dados['extid_tec_1'] : '0';
        if($extid_tec_1 != ''){
            $sql = "
                SELECT  extid as codigo,
                        extdsc as descricao
                FROM rcs.eixotecnologico
                WHERE extid <> {$extid_tec_1}
                ORDER BY extdsc
            ";
        }else{
            $sql = array();
        }
        $db->monta_combo('extid_tec_2', $sql, 'S', "Selecione...", 'atualizaComboEixoTec_3', '', '', 420, 'N', 'extid_tec_2', '', $extid_tec_2, '2º Eixo');
        die();
    }

    /**
     * functionName atualizaComboEixoTec_3
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer extid
     * @return string combo do eixo de técnologia 3.
     *
     * @version v1
    */
    function atualizaComboEixoTec_3( $dados ){
        global $db;
        $extid_tec[] = $dados['extid_tec_1'] != '' ? $dados['extid_tec_1'] : '0';
        $extid_tec[] = $dados['extid_tec_2'] != '' ? $dados['extid_tec_2'] : '0';

        $extid_tec = implode(',', $extid_tec);
        if($extid_tec != ''){
            $sql = "
                SELECT  extid as codigo,
                        extdsc as descricao
                FROM rcs.eixotecnologico
                WHERE extid NOT IN ({$extid_tec})
                ORDER BY extdsc
            ";
        }else{
            $sql = array();
        }
        $db->monta_combo('extid_tec_3', $sql, 'S', "Selecione...", '', '', '', 420, 'N', 'extid_tec_3', '', $extid_tec_3, '3º Eixo');
        die();
    }

    /**
     * functionName atualizaComboEixoCog_2
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer extid
     * @return string combo do eixo de cognitivo 2.
     *
     * @version v1
    */
    function atualizaComboEixoCog_2( $dados ){
        global $db;
        $extid_cog_1 = $dados['extid_cog_1'] != '' ? $dados['extid_cog_1'] : '0';
        if($extid_cog_1 != ''){
            $sql = "
                SELECT  extid as codigo,
                        extdsc as descricao
                FROM rcs.eixotecnologico
                WHERE extid <> {$extid_cog_1}
                ORDER BY extdsc
            ";
        }else{
            $sql = array();
        }
        $db->monta_combo('extid_cog_2', $sql, 'S', "Selecione...", 'atualizaComboEixoCog_3', '', '', 420, 'N', 'extid_cog_2', '', $extid_cog_2, '2º Eixo');
        die();
    }

    /**
     * functionName atualizaComboEixoCog_3
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer extid
     * @return string combo do eixo de cognitivo 3.
     *
     * @version v1
    */
    function atualizaComboEixoCog_3( $dados ){
        global $db;
        $extid_cog[] = $dados['extid_cog_1'] != '' ? $dados['extid_cog_1'] : '0';
        $extid_cog[] = $dados['extid_cog_2'] != '' ? $dados['extid_cog_2'] : '0';

        $extid_cog = implode(',', $extid_cog);

        if($extid_cog != ''){
            $sql = "
                SELECT  extid as codigo,
                        extdsc as descricao
                FROM rcs.eixotecnologico
                WHERE extid NOT IN ({$extid_cog})
                ORDER BY extdsc
            ";
        }else{
            $sql = array();
        }
        $db->monta_combo('extid_cog_3', $sql, 'S', "Selecione...", '', '', '', 420, 'N', 'extid_cog_3', '', $extid_cog_3, '3º Eixo');
        die();
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
    function atualizaComboMunicipio($dados){
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
    function buscarEndereceCEP( $dados ){
        global $db;

        $cep = str_replace('-', '', $dados['cep']);

        $sql = "
            SELECT * FROM cep.v_endereco2 WHERE cep = '{$cep}'
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            $dados["logradouro"]    = iconv("ISO-8859-1", "UTF-8", $dados["logradouro"]);
            $dados["bairro"]        = iconv("ISO-8859-1", "UTF-8", $dados["bairro"]);
            $dados["muncod"]        = iconv("ISO-8859-1", "UTF-8", $dados["muncod"]);
            echo simec_json_encode( $dados );
        }else{
            $dados["logradouro"]    = "";
            $dados["bairro"]        = "";
            $dados["estado"]        = "";
            $dados["muncod"]        = "";
            echo simec_json_encode( $dados );
        }
        die;
    }

   /**
     * functionName buscarProfessorCPF
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $siape cep
     * @return string json "array" com dados referentes ao servidor.
     *
     * @version v1
    */
    function buscarProfessorCPF( $dados ){
        global $db;

        /*
         * NEB = Não Existe na Base;
         * ECB = Existe CPF na Base;
        */

        $perfil = pegaPerfilGeral();

        $srpnumcpf = str_replace( '.', '', str_replace('-', '', $dados['srpnumcpf']) );

        if( $srpnumcpf != '' ){
            $sql = "
                SELECT srpid FROM rcs.servidoresprofessor WHERE srpnumcpf = '{$srpnumcpf}' AND srpstatus = 'A'
            ";
            $srpid = $db->pegaUm($sql);
        }

        if( $srpid > 0 ){
            $result = buscarDadosProfessores( $srpid );
            $result["tp_result"] = "ECB";

            if( in_array(PERFIL_RCS_SUPER_USUARIO, $perfil) || in_array(PERFIL_RCS_ADMINISTRADOR, $perfil) ){
                $result["permicao"] = "autorizado";
            }else{
                $result["permicao"] = "naoAutorizado";
            }
        }else{
            $result = array("tp_result"=>"NEB", "permicao" => "autorizado");
        }

        if($result != ''){
            echo simec_json_encode( $result );
        }
        die;
    }

   /**
     * functionName buscarDadosPorSIAPE_CPF_Professor
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $siape cep
     * @return string json "array" com dados referentes ao servidor.
     *
     * @version v1
    */
    function buscarDadosPorSIAPE_CPF_Professor( $dados ){
        global $db;

        $dsamatricula   = str_replace('-', '', $dados['dsamatricula']);
        $usucpf         = str_replace( '.', '', str_replace('-', '', $dados['srpnumcpf']) );

        if( $dsamatricula != '' AND $usucpf == '' ){
            $where = " WHERE dsamatricula = '{$dsamatricula}' ";
        }else{
            $where = " WHERE usucpf = '{$usucpf}' ";
        }

        $sql = "
            SELECT  srpid,
                    TRIM(srplotacao) AS srplotacao,
                    UPPER(srpdsc) as srpdsc,
                    dsamatricula,
                    TRIM( replace(to_char(cast(usucpf as bigint), '000:000:000-00'), ':', '.') ) as usucpf
            FROM rcs.servidoresprofessor
            {$where}
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            $dados["srpdsc"]        = iconv("ISO-8859-1", "UTF-8", trim($dados["srpdsc"]));
            $dados["dsamatricula"]  = iconv("ISO-8859-1", "UTF-8", trim($dados["dsamatricula"]));
            $dados["usucpf"]        = iconv("ISO-8859-1", "UTF-8", trim($dados["usucpf"]));
            echo simec_json_encode( $dados );
        }else{
            $dados["srpdsc"]        = "";
            $dados["dsamatricula"]  = "";
            $dados["usucpf"]        = "";
            echo simec_json_encode( $dados );
        }
        die;
    }

  /**
     * functionName buscarDadosPorCPF_ParaMontagemBanca
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados CPF é usado para buscar o usuário
     * @return string json "array" com dados referentes ao servidor.
     *
     * @version v1
    */
    function buscarDadosPorCPF_ParaMontagemBanca( $dados ){
        global $db;

        $srpnumcpf  = str_replace( '.', '', str_replace('-', '', $dados['srpnumcpf']) );

        $perfil     = pegaPerfilGeral($_SESSION['usucpf']);

        if( in_array(PERFIL_RCS_INTERLOCUTOR, $perfil) || in_array(PERFIL_RCS_ADM_INSTITUCIONAL, $perfil) ){
            $case = "
                CASE WHEN prof.dsamatricula IS NULL
                    THEN 'N'
                    ELSE 'S'
                END AS prof_equipe,
            ";
            $join = "
                LEFT JOIN(
                    SELECT s.dsamatricula
                    FROM rcs.servidoresprofessor AS s
                    WHERE CAST(s.srplotacao AS integer) IN (SELECT entid FROM rcs.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}')
                ) AS prof ON prof.dsamatricula = s.dsamatricula
            ";
        }else{
            $case = " 'S' AS prof_equipe, ";
            $join = "";
        }

        if( $srpnumcpf > 0 ){
            $sql = "
                SELECT  s.srpid,

                        {$case}

                        s.srplotacao AS srplotacao,
                        UPPER(s.srpdsc) as srpdsc,
                        s.dsamatricula,
                        TRIM( replace(to_char(cast(s.srpnumcpf as bigint), '000:000:000-00'), ':', '.') ) as srpnumcpf,
                        rof.orgid AS orgid
                FROM rcs.servidoresprofessor AS s

                LEFT JOIN rcs.avaliado AS a ON a.srpid = s.srpid
                JOIN entidade.entidade ee ON ee.entid = cast(s.srplotacao as integer)
                JOIN entidade.funcaoentidade efe ON efe.entid = ee.entid
                JOIN rcs.orgaofuncao rof ON rof.funid = efe.funid
                JOIN rcs.orgao ro ON ro.orgid = rof.orgid AND ro.orgstatus = 'A'

                {$join}

                WHERE s.srpstatus = 'A' AND s.srpnumcpf = '{$srpnumcpf}' 
            ";
            $dados = $db->pegaLinha($sql);
        }

        if( $srpnumcpf != $_SESSION['usucpf'] ){
            if($dados != ''){
                $dados["srpdsc"]        = iconv("ISO-8859-1", "UTF-8", trim($dados["srpdsc"]));
                $dados["dsamatricula"]  = iconv("ISO-8859-1", "UTF-8", trim($dados["dsamatricula"]));
                $dados["srpnumcpf"]     = iconv("ISO-8859-1", "UTF-8", trim($dados["srpnumcpf"]));
                $dados["usu_cpf_igual"] = "N";
            }else{
                $dados["srpdsc"]        = "";
                $dados["dsamatricula"]  = "";
                $dados["srpnumcpf"]     = "";
                $dados["usu_cpf_igual"] = "N";
            }
        }else{
            $dados["usu_cpf_igual"] = "S";
        }
        echo simec_json_encode( $dados );
        die;
    }

    /**
     * functionName buscarDadosProfessores
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é request do formulario de cadastro dos dados do professor
     * @return string dados do professor.
     *
     * @version v1
    */
    function buscarDadosProfessores( $srpid ){
        global $db;

        $sql = "
            SELECT  s.srpid,
                    TRIM( s.dsamatricula ) as dsamatricula,
                    srpdsc,
                    TRIM( replace(to_char(cast(srpnumcpf as bigint), '000:000:000-00'), ':', '.') ) as srpnumcpf,
                    cast(srplotacao as integer) as srplotacao,
                    to_char(srpdtnascimento, 'DD/MM/YYYY') as srpdtnascimento,
                    srpemail,
                    srpcurriculolattes,
                    --replace(to_char(cast(srpcep as bigint), '00000-000'), ':', '.') as srpcep,

                    CASE WHEN TRIM(srpcep) != ''
                        THEN replace(to_char(cast(srpcep as bigint), '00000-000'), ':', '.')
                        ELSE '00000-000'
                    END as srpcep,

                    srplogradouro,
                    TRIM( srpnumero ) AS srpnumero,
                    srpcomplemento,
                    srpbairro,
                    estuf,
                    muncod,
                    TRIM( srpdddtelcomercial ) AS srpdddtelcomercial,
                    TRIM( srptelcomercial ) AS srptelcomercial,

                    TRIM( srptdddelcelular ) AS srptdddelcelular,
                    TRIM( srptelcelular ) AS srptelcelular,

                    TRIM( srpdddtelresidencial ) AS srpdddtelresidencial,
                    TRIM( srptelresidencial ) AS srptelresidencial,

                    srptipoavaliador,
                    srptipoavaliado,
                    srpstatus,
                    --EIXO TÉCNOLOGICO.
                    ie_tec_1.extid AS extid_tec_1,
                    ie_tec_2.extid AS extid_tec_2,
                    ie_tec_3.extid AS extid_tec_3,
                    --EIXO COGNITIVO.
                    ie_cog_1.extid AS extid_cog_1,
                    ie_cog_2.extid AS extid_cog_2,
                    ie_cog_3.extid AS extid_cog_3,

                    dsatermoaceite01,
                    dsatermoaceite02,
                    dsatermoaceite03

                FROM rcs.servidoresprofessor AS s

                LEFT JOIN rcs.itemeixo AS ie_tec_1 ON ie_tec_1.srpid = s.srpid AND ie_tec_1.itxnivel = 1 AND ie_tec_1.itxarea = 'T'
                LEFT JOIN rcs.itemeixo AS ie_tec_2 ON ie_tec_2.srpid = s.srpid AND ie_tec_2.itxnivel = 2 AND ie_tec_2.itxarea = 'T'
                LEFT JOIN rcs.itemeixo AS ie_tec_3 ON ie_tec_3.srpid = s.srpid AND ie_tec_3.itxnivel = 3 AND ie_tec_3.itxarea = 'T'

                LEFT JOIN rcs.itemeixo AS ie_cog_1 ON ie_cog_1.srpid = s.srpid AND ie_cog_1.itxnivel = 1 AND ie_cog_1.itxarea = 'C'
                LEFT JOIN rcs.itemeixo AS ie_cog_2 ON ie_cog_2.srpid = s.srpid AND ie_cog_2.itxnivel = 2 AND ie_cog_2.itxarea = 'C'
                LEFT JOIN rcs.itemeixo AS ie_cog_3 ON ie_cog_3.srpid = s.srpid AND ie_cog_3.itxnivel = 3 AND ie_cog_3.itxarea = 'C'

                LEFT JOIN rcs.dadossiape AS ds ON ds.dsamatricula = s.dsamatricula

                LEFT JOIN entidade.entidade AS ent ON ent.entid = cast(s.srplotacao as integer)

                WHERE s.srpid = {$srpid};
            ";
            $dados = $db->pegaLinha($sql);
            return $dados;
    }

     /**
     * functionName buscarGraduacaoProf
     *
     * @author Luciano F. Ribeiro
     *
     * @param string ...
     * @return string
     *
     * @version v1
    */
    function buscarGraduacaoProf( $dados ){
        global $db;

        $srpnumcpf = $dados['srpnumcpf'];

        $sql = "
             SELECT i.grdid as codigo,
                    g.grddsc as descricao
             FROM rcs.itemgraduacao i
             RIGHT JOIN rcs.graduacao g ON g.grdid = i.grdid
             WHERE i.srpid = (SELECT srpid FROM rcs.servidoresprofessor WHERE srpstatus = 'A' AND srpnumcpf = '{$srpnumcpf}' LIMIT 1)
             ORDER BY g.grddsc
        ";
        $grdid = $db->carregar($sql);

        $sql = "
             SELECT  grdid as codigo,
                     grddsc as descricao
             FROM rcs.graduacao
             WHERE 1 = 1
             ORDER BY grddsc
         ";
        #codigo é o campo que vai ser usado para fazer a busca. descricao é o nome do campo ou titulo do campo.
        $arrayCampoPesquisa = array(
            Array(
                'codigo' => 'grddsc',
                'descricao' => 'Graduação'
            )
        );
        combo_popup('grdid', $sql, 'Graduação', '400x400', '0', '', '', 'S', false, false, 3, 550 , null, null, false, $arrayCampoPesquisa, $grdid, true, true, "", false, null , null);
        die();
    }

    /**
     * functionName buscarEspecializacaoProf
     *
     * @author Luciano F. Ribeiro
     *
     * @param string ...
     * @return string entidades relacionadas ao professor.
     *
     * @version v1
    */
    function buscarEspecializacaoProf( $dados ){
        global $db;

        $srpnumcpf = $dados['srpnumcpf'];

        $sql = "
            SELECT  i.sbcid as codigo,
                    a.arcdsc ||' - '||s.sbcdsc AS descricao
            FROM rcs.itemcursopos AS i

            JOIN rcs.subareacurso AS s ON s.sbcid = i.sbcid
            JOIN rcs.areacurso AS a ON a.arcid = s.arcid

            WHERE i.icptipo = 'E' AND i.srpid = (SELECT srpid FROM rcs.servidoresprofessor WHERE srpstatus = 'A' AND srpnumcpf = '{$srpnumcpf}' LIMIT 1)
            ORDER BY descricao
        ";
        $resp_espid = $db->carregar($sql);

        $sql_1 = "
             SELECT  arcid as codigo,
                     arcdsc as descricao
             FROM rcs.areacurso
             ORDER BY arcdsc
         ";
        $sql_2 = "
             SELECT  sbcid as codigo,
                     sbcdsc as descricao
             FROM rcs.subareacurso
         ";

        $sql = array(
            'combo_1'=>
                array(
                    'titulo_combo' => 'Área - Cursos',
                    'sql_combo' => $sql_1,
                    'name_combo' => 'arcid',
                    'id_combo' => 'arcid'
                ),
            'combo_2'=>
                array(
                    'titulo_combo' => 'Sub área - Cursos',
                    'sql_combo' => $sql_2,
                    'where'=> 'WHERE arcid = %s',
                    'orderby'=> 'ORDER BY sbcdsc;',
                    'name_combo' => 'sbcid',
                    'id_combo' => 'sbcid'
            )
        );
        select_popup('espid', $sql, 'Especialização', '300x680', 'S', 3, 550, null, $resp_espid, $funcao);
        die();
    }

    /**
     * functionName buscarMestradoProf
     *
     * @author Luciano F. Ribeiro
     *
     * @param string ...
     * @return string
     *
     * @version v1
    */
    function buscarMestradoProf( $dados ){
        global $db;

        $srpnumcpf = $dados['srpnumcpf'];

        $sql = "
            SELECT  i.sbcid as codigo,
                    a.arcdsc ||' - '||s.sbcdsc AS descricao
            FROM rcs.itemcursopos AS i

            JOIN rcs.subareacurso AS s ON s.sbcid = i.sbcid
            JOIN rcs.areacurso AS a ON a.arcid = s.arcid

            WHERE i.icptipo = 'M' AND i.srpid = (SELECT srpid FROM rcs.servidoresprofessor WHERE srpstatus = 'A' AND srpnumcpf = '{$srpnumcpf}' LIMIT 1)
            ORDER BY descricao
        ";
        $resp_mesid = $db->carregar($sql);

        $sql_1 = "
             SELECT  arcid as codigo,
                     arcdsc as descricao
             FROM rcs.areacurso
             ORDER BY arcdsc
         ";
        $sql_2 = "
             SELECT  sbcid as codigo,
                     sbcdsc as descricao
             FROM rcs.subareacurso
         ";

        $sql = array(
            'combo_1'=>
                array(
                    'titulo_combo' => 'Área - Cursos',
                    'sql_combo' => $sql_1,
                    'name_combo' => 'arcid',
                    'id_combo' => 'arcid'
                ),
            'combo_2'=>
                array(
                    'titulo_combo' => 'Sub área - Cursos',
                    'sql_combo' => $sql_2,
                    'where'=> 'WHERE arcid = %s',
                    'orderby'=> 'ORDER BY sbcdsc;',
                    'name_combo' => 'sbcid',
                    'id_combo' => 'sbcid'
            )
        );
        select_popup('mesid', $sql, 'Mestrado', '300x680', 'S', 3, 550, null, $resp_mesid, $funcao);
        die();
    }

    /**
     * functionName buscarDoutoradoProf
     *
     * @author Luciano F. Ribeiro
     *
     * @param string ...
     * @return string
     *
     * @version v1
    */
    function buscarDoutoradoProf( $dados ){
        global $db;

        $srpnumcpf = $dados['srpnumcpf'];

        $sql = "
            SELECT  i.sbcid as codigo,
                    a.arcdsc ||' - '||s.sbcdsc AS descricao
            FROM rcs.itemcursopos AS i

            JOIN rcs.subareacurso AS s ON s.sbcid = i.sbcid
            JOIN rcs.areacurso AS a ON a.arcid = s.arcid

            WHERE i.icptipo = 'D' AND i.srpid = (SELECT srpid FROM rcs.servidoresprofessor WHERE srpstatus = 'A' AND srpnumcpf = '{$srpnumcpf}' LIMIT 1)
            ORDER BY descricao
        ";
        $resp_douid = $db->carregar($sql);

        $sql_1 = "
              SELECT  arcid as codigo,
                      arcdsc as descricao
              FROM rcs.areacurso
              ORDER BY arcdsc
          ";
        $sql_2 = "
             SELECT  sbcid as codigo,
                     sbcdsc as descricao
             FROM rcs.subareacurso
         ";

        $sql = array(
            'combo_1'=>
                array(
                    'titulo_combo' => 'Área - Cursos',
                    'sql_combo' => $sql_1,
                    'name_combo' => 'arcid',
                    'id_combo' => 'arcid'
                ),
            'combo_2'=>
                array(
                    'titulo_combo' => 'Sub área - Cursos',
                    'sql_combo' => $sql_2,
                    'where'=> 'WHERE arcid = %s',
                    'orderby'=> 'ORDER BY sbcdsc;',
                    'name_combo' => 'sbcid',
                    'id_combo' => 'sbcid'
            )
        );
        select_popup('douid', $sql, 'Doutorado', '300x680', 'S', 3, 550, null, $resp_douid, $funcao);
        die();
    }

    /**
     * functionName buscaUnidadesAssociadas
     *
     * @author Luciano F. Ribeiro
     *
     * @param string ...
     * @return string entidades relacionadas ao professor.
     *
     * @version v1
    */
    function buscaUnidadesAssociadas( $cpf ){
        global $db;

        $usucpf = str_replace( '.', '', str_replace('-','',$cpf) );
        $pflcod = PERFIL_RCS_PROFESSOR;

        $sql = "
            SELECT  DISTINCT e.entid as codigo,
                    o.orgdesc AS tipo_ensino,
                    entsig ||' - '|| initcap(e.entnome) AS nome_unidade
            FROM entidade.entidade e

            INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
            INNER JOIN rcs.orgaofuncao of ON of.funid = fe.funid
            INNER JOIN rcs.orgao o ON o.orgid = of.orgid
            INNER JOIN rcs.usuarioresponsabilidade ur ON ur.entid = e.entid AND ur.rpustatus = 'A'

            WHERE ur.rpustatus='A' AND ur.usucpf = '{$usucpf}' AND ur.pflcod = {$pflcod}

            ORDER BY o.orgdesc
        ";
        $dados = $db->carregar($sql);

        $html = "<table align=\"center\" bgcolor=\"#f5f5f5\" border=\"0\" cellpadding=\"3\" cellspacing=\"1\" class=\"tabela\">";
            $html .= "<tr>";
                $html .= "<td class=\"subTituloCentro\" colspan=\"2\" style=\"text-align: center; text-transform:uppercase;\"> Unidades Relacionadas </td>";
            $html .= "</tr>";

            if($dados != ''){
                $html .= "<tr>";
                    $html .= "<td width=\"20%\" class=\"subTituloCentro\" style=\"text-align: left; text-transform:uppercase;\"> Tipo de Ensino </td>";
                    $html .= "<td width=\"80%\" class=\"subTituloCentro\" style=\"text-align: left; text-transform:uppercase;\"> Unidade </td>";
                $html .= "</tr>";

                foreach ($dados as $value) {
                    $html .= "<tr>";
                        $html .= "<td class=\"SubtituloEsquerda\" style=\"font-weight: normal;\">{$value['tipo_ensino']}</td>";
                        $html .= "<td class=\"SubtituloEsquerda\" style=\"font-weight: normal;\">{$value['nome_unidade']}</td>";
                    $html .= "</tr>";
                }
            }else{
                $html .= "<tr>";
                    //$html .= "<td colspan=\"2\" class=\"subTituloCentro\" style=\"color: red; text-align: center; text-transform:uppercase;\">Não a unidades associadas há esse professor. É necessário a associação para dar continuidade ao cadastro.</td>";
                    $html .= "<td colspan=\"2\" class=\"subTituloCentro\" style=\"color: red; text-align: center; text-transform:uppercase;\"> &nbsp </td>";
                $html .= "</tr>";
            }
        echo $html;

        echo '</table>';
    }

    /**
     * functionName buscaUnidadesAssociadas
     *
     * @author Luciano F. Ribeiro
     *
     * @param string ...
     * @return string entidades relacionadas ao professor.
     *
     * @version v1
    */
    function buscaEntidAssociado( $cpf ){
        global $db;

        $usucpf = str_replace( '.', '', str_replace('-','',$cpf) );
        $pflcod = PERFIL_RCS_PROFESSOR;

        $sql = "
            SELECT  DISTINCT e.entid as entid,
                    o.orgdesc AS tipo_ensino,
                    entsig ||' - '|| initcap(e.entnome) AS nome_unidade
            FROM entidade.entidade e

            INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
            INNER JOIN rcs.orgaofuncao of ON of.funid = fe.funid
            INNER JOIN rcs.orgao o ON o.orgid = of.orgid
            INNER JOIN rcs.usuarioresponsabilidade ur ON ur.entid = e.entid AND ur.rpustatus = 'A'

            WHERE ur.rpustatus='A' AND ur.usucpf = '{$usucpf}' AND ur.pflcod = {$pflcod}

            ORDER BY o.orgdesc
        ";
        return $dados = $db->pegaLinha($sql);
    }

    /**
     * functionName downloadModeloArqExport
     *
     * @author Luciano F. Ribeiro
     *
     * @param string ...
     * @return string arquvio modelo de carga.
     *
     * @version v1
    */
    function downloadModeloArqExport(){
        global $db;

        $_VALIDACAO = array(
            0 => array(
                    "label"		=> "matriculasiape",
                    "valor"		=> '1234567'
                ),
            1 => array(
                    "label"		=> "nomeservidor",
                    "valor"		=> 'Jose da Silva'
                ),
            2 => array(
                    "label"		=> "codigoorgao",
                    "valor"		=> '7894561'
                ),
            3 => array(
                    "label"		=> "cpf",
                    "valor"		=> '78512545689'
                ),
            4 => array(
                    "label"		=> "datanascimento",
                    "valor"		=> "99/99/9999"
                )
            );

        if($_VALIDACAO) {
            foreach($_VALIDACAO as $key => $valid) {
                if($key == ( count($_VALIDACAO)-1) ){
                    $sep="";
                }else{
                    $sep=";";
                }
                $csvlabel .= $valid['label'].$sep;
                $arrEx[] = array("valor" => $valid['valor'], "sep" => $sep);
            }
        }
        $csvlabel .= "\n";
        $i = 0;
        while( $i < 5){
            $i++;
            foreach ($arrEx as $arr){
                $csvlabel .= $arr['valor'].$arr['sep'];
            }
            $csvlabel .= "\n";
        }
        header("Content-Type: text/html; charset=ISO-8859-1");
        header("Content-type: application/octet-stream");
        header("Content-Disposition: attachment; filename=\"codigo_modelo.csv\"");
        echo $csvlabel;
        exit;
    }

    /**
     * functionName exportarDados
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é post do formulario de criação da banca avaliadora
     * @return string grid.
     *
     * @version v1
    */
    function exportarDados( $dados, $files ){
        global $db;

        #VARIAVEL INICIALIZADA COM N - NÃO EXISTE ERROS;
        $erros = 'N';

        if( $files["arquivo"]["tmp_name"] ) {
	    $csvarray = file( $files["arquivo"]["tmp_name"] );
	}else{
            $erros = 'S';
            $msg = "Arquivo não foi enviado corretamente, verifique o tamanho do arquivo, o tipo do e o formato dos campos e tente novamente!";
	}

        if( count($csvarray) > 10000){
            $erros = 'S';
            $msg = "O número de registros nesse arquivo é maior que 10.000, esse número não suportado pelo sistema!";
        }

        if( $erros == 'N' ){
            $SQL = "DELETE FROM rcs.dadossiape_temp RETURNING dsamatricula";
            $db->pegaUm($SQL);

            foreach( $csvarray as $dados ){
                $dadosExp = explode( ';', $dados );

                $dsamatricula   = trim( $dadosExp[0] );
                $dsadsc         = trim( $dadosExp[1] );
                $dsaorgao       = trim( $dadosExp[2] );
                $dsacpf         = trim( $dadosExp[3] );
                $dsadtnasc      = empty( $dadosExp[4] ) ? 'NULL' : "'".trim( formata_data_sql( $dadosExp[4] ) )."'";
                $dsatipo        = trim( $dadosExp[5] );

                if( !empty( $dsamatricula ) ){
                    #CONTROLE PARA QUE NÃO OCORRA INSERÇÃO DE CPF REPETIDO.
                    if( $dsacpf != $controle ){
                        $sql = "
                            INSERT INTO rcs.dadossiape_temp(
                                    dsamatricula, dsadsc, dsaorgao, dsacpf, dsadtnasc, dsatipo, dt_realizacao_carga
                                )VALUES(
                                    '{$dsamatricula}', '{$dsadsc}', '{$dsaorgao}', '{$dsacpf}', {$dsadtnasc}, '{$dsatipo}', 'NOW()'
                            ) RETURNING dsamatricula;
                        ";
                        $matricula_siap = $db->pegaUm($sql);
                    }
                    $controle = $dsacpf;
                }
            }

            if( $matricula_siap > 0 ){
                $db->commit();
                $resut = 'OK';
            }else{
                $db->rollback();
                $resut = 'ERROR';
            }
        }

        if( $resut == 'OK' ){
            $sql = "
                SELECT dsamatricula, dsacpf, dsadsc
                FROM rcs.dadossiape_temp
                WHERE dsacpf NOT IN( SELECT dsacpf FROM rcs.dadossiape )
                ORDER BY dsamatricula
            ";
            $arryServidor = $db->carregar($sql);

            #USADO APENAS NA TELA DE "CARGA DOS DADOS SIAPE".
            $_SESSION['rsc']['array']['lista_servidor'] = $arryServidor;

            $sql = "
                INSERT INTO rcs.dadossiape (dsamatricula, dsadsc, dsaorgao, dsacpf, dsadtnasc, dsatipo, dt_realizacao_carga)

                SELECT dsamatricula, dsadsc, dsaorgao, dsacpf, dsadtnasc, dsatipo, dt_realizacao_carga
                FROM rcs.dadossiape_temp
                WHERE dsacpf NOT IN( SELECT dsacpf FROM rcs.dadossiape )
                ORDER BY dsamatricula
            ";

            if( $db->executar($sql) ){
                $db->commit();
                $_SESSION['rsc']['array']['executar_listagem'] = 'S';

                if( !empty($arryServidor) ){
                    $msg = 'Operação foi realizada com sucesso.\nConfira a listagem dos Servidores que foram carregados na base de dados do RSC!';
                }else{
                    $msg = 'Não há dados a serem carregados na Base de Dados do RSC. Possívelmente os Servidores já estavam cadastrados no RSC.';
                }

                $db->sucesso( 'principal/carga/carga_dados_siape','', $msg );
            }else{
                $db->rollback();
                $db->sucesso( 'principal/carga/carga_dados_siape','', 'Não foi possível executar a carga, tente novamente mais tarde!' );
            }
        }else{
            $db->rollback();
            $db->sucesso( 'principal/carga/carga_dados_siape','', 'Não foi possível executar a carga, ocorreu algum problema!\n'.$msg );
        }
    }


    /**
     * functionName exibirListaAvaliadores
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é post do formulario de criação da banca avaliadora
     * @return string grid.
     *
     * @version v1
    */
    function exibirListaAvaliadores( $dados ){
        global $db;

        $avaid = $dados['avaid'];

        $acao = "
            <img src=\"../imagens/seta_filho.gif\" />
            <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirAvaliador('||a.avrid||' , this);\" title=\"Excluir Avaliação\" >
        ";

        $sql = "
            SELECT  DISTINCT '{$acao}' as acao ,
                    s.srpdsc,
                    CASE WHEN srptipoavaliador IS NULL
                        THEN 'Não definido'
                        ELSE
                            CASE WHEN srptipoavaliador IS TRUE
                                THEN 'Sim'
                                ELSE 'Não'
                            END
                    END AS avaliador,
                    CASE WHEN srptipoavaliado IS NULL
                        THEN 'Não definido'
                        ELSE
                            CASE WHEN srptipoavaliado IS TRUE
                                THEN 'Sim'
                                ELSE 'Não'
                            END
                    END as avaliado,
                    s.dsamatricula,
                    TRIM( replace(to_char(cast(s.usucpf as bigint), '000:000:000-00'), ':', '.') ) as usucpf,
                    CASE
                        WHEN avrtipoavaliador = 'AEX' THEN 'Avaliador Externo'
                        WHEN avrtipoavaliador = 'AES' THEN 'Avaliador Externo Suplente'
                        WHEN avrtipoavaliador = 'AIN' THEN 'Avaliador Interno'
                        WHEN avrtipoavaliador = 'AIS' THEN 'Avaliador Interno Suplente'
                    END AS avrtipoavaliador,
                    e.entnome,
                    s.srpemail,
                    s.srptelcelular

            FROM rcs.avaliador AS a

            JOIN rcs.servidoresprofessor AS s ON s.srpid = a.srpid
            --COMENTADO PARA EVITAR A A OCORRENCIA DO PRODUTO CARTESIANO
            JOIN rcs.usuarioresponsabilidade AS u ON u.usucpf = s.usucpf AND u.rpustatus = 'A'
            JOIN entidade.entidade AS e ON e.entid = u.entid

            WHERE a.avaid = {$avaid}

            ORDER BY avrtipoavaliador
        ";
        $cabecalho = array( 'Ação' , "Professor", 'Avaliador' , 'Avaliado' ,  "SIAPE", "CPF", "Tipo de Avaliador", "Lotado", "E-mail", "Celular");
        //$cabecalho = array( 'Ação' , "Professor", 'Avaliador' , 'Avaliado' ,  "SIAPE", "CPF", "Tipo de Avaliador", "E-mail", "Celular");
        $alinhamento = Array('', '', '', '', '', '', '');
        $tamanho = Array('3%' , '22%' , '3%' , '3%' ,'7%','7%','10%','30%', '10%', '8%');

        echo '<br><table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="" width="98%">';
        echo '<tr style="padding:1px;"><td class="subTituloCentro" colspan="3" style="text-align: center; text-transform:uppercase;">Banca Avaliadora</td></tr>';
        echo '</table>';

        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
        die();
    }

    /**
     * functionName excluirBancaAvaliadora
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é post do formulario de criação da banca avaliadora
     * @return string exclução da banca avaliadora.
     *
     * @version v1
    */
    function excluirBancaAvaliadora( $dados ){
        global $db;

        $avaid = $dados['avaid'];

        $sql = " DELETE FROM rcs.avaliador WHERE avaid = {$avaid}; ";
        $sql .= " DELETE FROM rcs.avaliado WHERE avaid = {$avaid} RETURNING avaid; ";

        $avaid = $db->pegaUm($sql);

        if( $avaid > 0 ){
            $db->commit();
            $db->sucesso( 'principal/sorteio_avaliadores/cad_sorteio_avaliadores','', 'Registro excluido com sucesso!');
        }else{
            $db->rollback();
            $db->insucesso('Não foi possível excluir os Dados, tente novamente mais tarde!', '', 'principal/sorteio_avaliadores/cad_sorteio_avaliadores&acao=A');
        }
    }

    /**
     * functionName excluirAvaliador
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é post do formulario de cadastro dos dados do avaliador do professor
     * @return string persistencia.
     *
     * @version v1
    */
    function excluirAvaliador( $dados ){
        global $db;

        $avrid = $dados['avrid'];

        if( $avrid != '' ){
            $sql = "
                SELECT avaid, srpid FROM rcs.avaliador WHERE avrid = {$avrid};
            ";
            $result = $db->pegaLinha($sql);
            $srpid_avaliador = $result['srpid'];

            if( $result['avaid'] != '' ){
                $sql = "
                    SELECT srpid, avanumprocesso FROM rcs.avaliado WHERE avaid = {$result['avaid']};
                ";
                $avaliado = $db->pegaLinha($sql);

                $sql = "
                    INSERT INTO rcs.avaliador_execluido_avaliado(
                            srpid_avaliado, srpid_avaliador, numprocesso
                        ) VALUES (
                            {$avaliado['srpid']}, {$srpid_avaliador}, '{$avaliado['avanumprocesso']}'
                    ) RETURNING aevid;
                ";
                $aevid = $db->pegaUm($sql);
            }
        }

        if( $aevid > 0 ){
            $sql = " DELETE FROM rcs.avaliador WHERE avrid = {$dados['avrid']} RETURNING avrid; ";
            $avaid = $db->pegaUm($sql);

            if( $avaid > 0 ){
                $db->commit();
                $msg = "Avaliador deletado com sucesso!";
            }else{
                $db->rollback();
                $msg = 'Não foi possivél deletar o avaliador, tente novamente mais tarde!';
            }
        } else {
            $db->rollback();
            $msg = 'Não foi possivél deletar o avaliador, tente novamente mais tarde!';
        }
        echo $msg;
        die();
    }

    /**
     * functionName excluirServidorProfessor
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é post do formulario da lista de professores.
     * @return string exclução do professor.
     *
     * @version v1
    */
    function excluirServidorProfessor( $dados ){
        global $db;

        $srpid = $dados['srpid'];

        #VERIFICA SE O USUÁRIO ESTA SENDO AVALIADO, SE TEM UMA BANCA A ELE.
        $sql_ava = "
            SELECT avaid FROM rcs.avaliado WHERE srpid = {$srpid}
        ";
        $avaid = $db->pegaUm($sql_ava);

        #VERIFICA SE O USUÁRIO É AVALIADOR EM ALGUMA BANCA, ESTA AVALIANDO UM OUTRO PROFESSOR.
        $sql_avr = "
            SELECT avrid FROM rcs.avaliador WHERE srpid = {$srpid}
        ";
        $avrid = $db->pegaUm($sql_avr);

        if( $avaid == '' && $avrid == '' ){
            $sql = "UPDATE rcs.servidoresprofessor SET srpstatus = 'I' WHERE srpid = {$srpid} RETURNING srpid;";
            $srpid_up = $db->pegaUm($sql);
        }

       if( $srpid_up > 0 ){
            $db->commit();
            $db->sucesso( 'principal/saberes/lista_grid_isncricao_prof','', 'Registro excluido com sucesso!');
        }else{
            $db->rollback();
            $db->insucesso('Registro não excluido, Existe uma banca montada a ele ou ele é um avaliador!', '', 'principal/saberes/lista_grid_isncricao_prof&acao=A');
        }
    }

    /**
     * functionName executaAcoesVerificacao
     *
     * @author Luciano F. Ribeiro
     *
     * @param
     * @return
     *
     * @version v1
    */
    function executaAcoesVerificacao(){
?>
        <script lang="JavaScript">
            verificarTipoAvaliacao('V');

            var cpf_prof    = $('#srpnumcpf').val();
            var srpstatus   = $('input:radio[name=srpstatus]:checked').val();

            if( srpstatus == 'A' ){
                if( cpf_prof != '' ){
                    buscarProfessorCPF(cpf_prof);
                }
            }else
                if( srpstatus == 'I' && cpf_prof != '' ){
                    $('#dsamatricula').attr('readonly', true);
                    $('#srplotacao').attr('readonly', true);
                    $('#srplotacao_chosen').addClass('chosen-disabled');
                    $('#texto_info_alt_siape').css("display", "");
            }
        </script>
<?PHP
    }


     /**
     * functionName formularioBancaAvaliadora
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é post do formulario de criação da banca avaliadora
     * @return string modal para a formação da banca avaliadora.
     *
     * @version v1
    */
    function formularioBancaAvaliadora($dados){
?>
        <script language="JavaScript">
            $1_11('#salvarBancaAvaliadora').click(
                function (){
                    var erro;
                    var campos = '';

                    if(!erro){
                        $1_11('form[name=form_modal_bancada]').submit();
                    }
            });
        </script>

       <form action="" method="POST"  name="form_modal_bancada">
           <input type="hidden" name="avaid" value="<?php echo $dados['avaid'] ?>"/>
            <input type="hidden" id="requisicaoModalBancadaAvaliadora" name="requisicao" value="salvarDadosAvaliador"/>
            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela">
                <tr>
                    <td class="subTituloCentro">Formação da Banca Avaliadora</td>
                </tr>
            </table>
            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela">
                <tr>
                    <td class="SubTituloDireita" width="45%"> Avaliadores Externos: </td>
                    <td colspan="2">
                        <?PHP
                            $avaliador_aex = $dados['avaliador_aex'];
                            echo campo_texto('avaliador_aex', 'S', $habilita, 'Avaliadores Externos', 5, 2, '', '', '', '', 0, 'id="avaliador_aex"', '', $avaliador_aex, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="25%"> Avaliadores Internos: </td>
                    <td colspan="2">
                        <?PHP
                            $avaliador_ain = $dados['avaliador_ain'];
                            echo campo_texto('avaliador_ain', 'N', $habilita, 'Avaliadores Internos', 5, 2, '', '', '', '', 0, 'id="avaliador_ain"', '', $avaliador_ain, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="25%"> Avaliadores Externos Suplentes: </td>
                    <td colspan="2">
                        <?PHP
                            $avaliador_aes = $dados['avaliador_aes'];
                            echo campo_texto('avaliador_aes', 'N', $habilita, 'Avaliadores Externos Suplentes', 5, 2, '', '', '', '', 0, 'id="avaliador_aes"', '', $avaliador_aes, '', null);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" width="25%"> Avaliadores Internos Suplentes: </td>
                    <td colspan="2">
                        <?PHP
                            $avaliador_ais = $dados['avaliador_ais'];
                            echo campo_texto('avaliador_ais', 'N', $habilita, 'Avaliadores Internos Suplentes', 5, 2, '', '', '', '', 0, 'id="avaliador_ais"', '', $avaliador_ais, '', null);
                        ?>
                    </td>
                </tr>
            </table>
            <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="tabela">
                <tr>
                    <td style="text-align: center;" colspan="3">
                        <input type="button" id="salvarBancaAvaliadora" name="salvar" value="Salvar" />
                        <input type="button" id="fecharFormBanca" name="fechar" value="Fechar" class="modalCloseImg simplemodal-close"/>
                    </td>
                </tr>
            </table>
        </form>
<?PHP
    exit;
    }

    /**
     * functionName gerarNovoExcel
     *
     * @author Lindalberto
     *
     * @param string não há.
     * @return string arquivo xsl.
     *
     * @version v1
    */
    function gerarNovoExcel(){
    	global $db;
    	$sql = '
            SELECT  srpid AS "ID",
                    dsamatricula AS "Matrícula",
                    usucpf as "CPF Usuário",
                    srpdsc AS "Nome",
                    srpnumcpf AS "CPF Servidor",
                    srplotacao AS "Lotação",
                    srpemail AS "Email",
                    srpcurriculolattes AS "Currículo",
                    srpcep AS "CEP",
                    srplogradouro AS "Logradouro",
                    srpnumero AS "Número",
                    srpcomplemento AS "Complemento",
                    srpbairro AS "Bairro",
                    srpdddtelcomercial AS "DDD Comercial",
                    srptelcomercial AS "Telefone Comercial",
                    srptdddelcelular as "DDD Celular",
                    srptelcelular AS "Telefone Celular",
                    srpdddtelresidencial as "DDD Residencial",
                    srptelresidencial as "Telefone Residencial",
                    srpstatus AS "Status",
                    srpdtinclusao AS "Data de Inclusão",
                    estuf AS "UF",
                    muncod AS "Mun. Codigo",
                    srpdtnascimento AS "Data de Nascimento"
            FROM rcs.servidoresprofessor
            ORDER BY srpdsc
        ';
    	$cabecalho = array("ID","Matrícula", "CPF Usuário", "Nome","CPF Servidor","Lotação","Email","Currículo","CEP",
    			"Logradouro","Número","Complemento","Bairro","DDD Comercial","Telefone Comercial","DDD Celular","Telefone Celular",
    			"DDD Residencial","Telefone Residencial","Status","Data de Inclusão","UF","Mun. Codigo","Data de Nascimento");

    	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
    	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
    	header ( "Pragma: no-cache" );
    	header ( "Content-type: application/vnd.ms-excel");//vnd.ms-excel
    	header ( "Content-Disposition: attachment; filename=SIMEC_RelaçãoProfessores_".date("Ymdhis").".xls");
    	header ( "Content-Description: File Transfer" );// File Transfer - MID Gera excel

    	$db->monta_lista_tabulado($sql,$cabecalho,50000,5);
    	exit;
    	die();

    }

    /**
     * functionName mascara_global
     *
     * @author Luciano F. Ribeiro
     *
     * @param string
     * @return string
     *
     * @version v1
    */
    function mascara_global( $string, $mascara ){
        $string = str_replace(" ","",$string);
        for($i=0;$i<strlen($string);$i++){
            $mascara[strpos($mascara,"#")] = $string[$i];
        }
        return $mascara;
    }

    /**
     * functionName salvarDadosAvaliador
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é post do formulario de cadastro dos dados do avaliador do professor
     * @return string persistencia.
     *
     * @version v1
    */
    function salvarDadosAvaliador( $dados ){
        global $db;

        $srpid          = $dados['srpid'];
        $srplotacao     = $dados['srplotacao'];
        $avanumprocesso = $dados['avanumprocesso'];
        $usucpf         = $_SESSION['usucpf'];

        #TIPO DE ORGÃO - 1=SUPERIOR; 2=PROFISSIONAL; 3=MILITAR
        $orgid = trim($dados['orgid']);

        #QUANTIDADE AVALIADORES POR CATEGORIA.
        $avaliador_aex = trim($dados['avaliador_aex']);
        $avaliador_ain = trim($dados['avaliador_ain']);
        $avaliador_aes = trim($dados['avaliador_aes']);
        $avaliador_ais = trim($dados['avaliador_ais']);


        if( $dados['avaid'] != '' ){
            $avaid = $dados['avaid'];

            $sql = "
                SELECT * FROM rcs.avaliado a
                LEFT JOIN rcs.servidoresprofessor prof ON a.srpid = prof.srpid
                WHERE avaid = {$avaid};
            ";
            $result = $db->pegaLinha($sql);

            $srpid          = $result['srpid'];
            $srplotacao     = $result['srplotacao'];
            $avanumprocesso = $result['avanumprocesso'];

        } else if( $srpid != '' ){
            $sql = " INSERT INTO rcs.avaliado( srpid, avanumprocesso, avacpfinterlocutor) VALUES ( {$srpid}, '{$avanumprocesso}', '{$usucpf}' ) RETURNING avaid; ";
            $avaid = $db->pegaUm($sql);
        }

        if( $avaid > 0 ){
            $resp = criaBancaAvaliadora( $avaid, $srpid, $srplotacao, $orgid, $avaliador_aex, $avaliador_ain, $avaliador_aes, $avaliador_ais );

            if( $resp['ok'] == 'OK' ){
                $db->commit();
                unset( $resp['ok'] );

                $msg = "A banca Avaliadora esta formada por ".implode(', ',$resp).". Operação realizado com sucesso.";
                $db->sucesso( 'principal/sorteio_avaliadores/cad_sorteio_avaliadores&acao=A', '', $msg);
            } else {
                $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/sorteio_avaliadores/cad_sorteio_avaliadores&acao=A');
            }
        }
    }

   /**
     * functionName salvarDadosProfessores
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é post do formulario de cadastro dos dados do professor
     * @return string persistencia.
     *
     * @version v1
    */
    function salvarDadosProfessores( $dados ){
        global $db;

        #VARIAVEL DE CONTROLE
        $dados_validos = 'N';

        #ID DA TABELA servidoresprofessor
        $srpid = $dados['srpid'];

        $dsamatricula       = trim($dados['dsamatricula']);
        $srpnumcpf          = trim( str_replace( '.', '', str_replace('-', '', $dados['srpnumcpf']) ) );
        $srpdsc             = trim( strtoupper($dados['srpdsc']) );
        $srpdtnascimento    = formata_data_sql( $dados['srpdtnascimento'] );
        $srplotacao         = $dados['srplotacao'] != '' ? "'".trim($dados['srplotacao'])."'" : 'NULL';
        $srpemail           = $dados['srpemail'] != '' ? trim( strtolower($dados['srpemail'])) : '';
        $srpcurriculolattes = $dados['srpcurriculolattes'] != '' ? trim($dados['srpcurriculolattes']) : '';

        $srplogradouro      = trim( addslashes($dados['srplogradouro']) );
        $srpnumero          = $dados['srpnumero'] != '' ? trim($dados['srpnumero']) : 's/n';
        $srpcomplemento     = $dados['srpcomplemento'] != '' ? trim( addslashes( $dados['srpcomplemento'] ) ) : 's/c';
        $srpbairro          = trim( addslashes( $dados['srpbairro'] ) );
        $estuf              = ($dados['estuf'])? "'".trim($dados['estuf']."'") : 'NULL';
        $muncod             = ($dados['muncod'])? "'".trim($dados['muncod']."'") : 'NULL';

        #TELEFONES
        $srpdddtelcomercial = $dados['srpdddtelcomercial'] != '' ? trim($dados['srpdddtelcomercial']) : '';
        $srptelcomercial    = $dados['srptelcomercial'] != '' ? trim($dados['srptelcomercial']) : '';

        $srpdddtelresidencial = $dados['srpdddtelresidencial'] != '' ? trim($dados['srpdddtelresidencial']) : '';
        $srptelresidencial  = $dados['srptelresidencial'] != '' ? trim($dados['srptelresidencial']) : '';

        $srptdddelcelular   = $dados['srptdddelcelular'] != '' ? trim($dados['srptdddelcelular']) : '';
        $srptelcelular      = $dados['srptelcelular'] != '' ? trim($dados['srptelcelular']) : '';

        $usucpf             = $_SESSION['usucpf'];

        $dsatermoaceite01   = $dados['dsatermoaceite01'] == 'S' ? 't' : 'f';
        $dsatermoaceite02   = $dados['dsatermoaceite02'] == 'S' ? 't' : 'f';
        $dsatermoaceite03   = $dados['dsatermoaceite03'] == 'S' ? 't' : 'f';

        $srptipoavaliador   = $dados['srptipoavaliador'] ? "'{$dados['srptipoavaliador']}'" : 'NULL';
        $srptipoavaliado    = $dados['srptipoavaliado'] ? "'{$dados['srptipoavaliado']}'" : 'NULL';

        $srpstatus          = $dados['srpstatus'] != '' ? $dados['srpstatus'] : 'A';

        #GRADUAÇÃO
        $grdid = $dados['grdid'];

        #ESPECIALIZAÇÃO
        $espid = $dados['espid'];

        #MESTRADO
        $mesid = $dados['mesid'];

        #DOUDORADO
        $douid = $dados['douid'];

        #MESSAGEM DE ERRO.
        $msg = "Não foi possivél gravar o Dados, tente novamente mais tarde!";

        #VALIDAÇÃO DO CEP:
        $result_cep = validarCep($dados['srpcep']);
        if( $result_cep == 'S' || str_replace("'", "", $srptipoavaliador) == 'f' || $srptipoavaliador == 'NULL'){
            $srpcep = trim( str_replace( '-', '', $dados['srpcep'] ) );
            $dados_validos = 'S';
        }else{
            $msg = "O CEP digitado não é valido, verifique os dados digitados e tente novamente!";
        }

        #VERIFICA SE O CPF JÁ ESTA CADASTRADO:
        if( $srpnumcpf != '' ){
            $SQL = "SELECT srpid FROM rcs.servidoresprofessor WHERE srpnumcpf = '{$srpnumcpf}' AND srpstatus = 'A'";
            $resp = $db->pegaUm($SQL);

            if( $resp > 0 ){
                #SE srpid VIER SEM VALOR, QUER DIZER QUE É UM NOVO REGISTRO. DESSA FORMA NAO PODERA SER CADASTRO.
                if( $srpid == '' ){
                    $cpf_cadastrado = "S";
                    $msg = "O CPF digitado já esta cadastrado no sistema, verifique os dados digitados e tente novamente!";
                }else{
                    $cpf_cadastrado = "N";
                }
            }else{
                $cpf_cadastrado = "N";
            }
        }

        if( $dados_validos == 'S' && $cpf_cadastrado == 'N' ){
            if( $srpid == '' ){
                $sql = "
                    INSERT INTO rcs.servidoresprofessor(
                        dsamatricula, srpnumcpf, srpdsc, srplotacao, srpdtnascimento, srpemail, srpcurriculolattes, srpcep, srplogradouro, srpnumero, srpcomplemento,
                        srpbairro, estuf, muncod, srpdddtelcomercial, srptelcomercial, srptdddelcelular, srptelcelular, srpdddtelresidencial, srptelresidencial, srptipoavaliador, srptipoavaliado, usucpf
                     ) VALUES (
                        '{$dsamatricula}','{$srpnumcpf}','{$srpdsc}',{$srplotacao},'{$srpdtnascimento}','{$srpemail}','{$srpcurriculolattes}','{$srpcep}','{$srplogradouro}','{$srpnumero}','{$srpcomplemento}',
                        '{$srpbairro}',{$estuf},{$muncod},'{$srpdddtelcomercial}','{$srptelcomercial}','{$srptdddelcelular}','{$srptelcelular}','{$srpdddtelresidencial}','{$srptelresidencial}',{$srptipoavaliador},{$srptipoavaliado},'{$usucpf}'
                     ) RETURNING srpid
                ";
            }else{
                $sql = "
                    UPDATE rcs.servidoresprofessor
                        SET dsamatricula        = '{$dsamatricula}',
                            srpdsc              = '{$srpdsc}',
                            srpnumcpf           = '{$srpnumcpf}',
                            srplotacao          = {$srplotacao},
                            srpdtnascimento     = '{$srpdtnascimento}',
                            srpemail            = '{$srpemail}',
                            srpcurriculolattes  = '{$srpcurriculolattes}',
                            srpcep              = '{$srpcep}',
                            srplogradouro       = '{$srplogradouro}',
                            srpnumero           = '{$srpnumero}',
                            srpcomplemento      = '{$srpcomplemento}',
                            srpbairro           = '{$srpbairro}',
                            estuf               = {$estuf},
                            muncod              = {$muncod},
                            srpdddtelcomercial  = '{$srpdddtelcomercial}',
                            srptelcomercial     = '{$srptelcomercial}',
                            srptdddelcelular    = '{$srptdddelcelular}',
                            srptelcelular       = '{$srptelcelular}',
                            srpdddtelresidencial= '{$srpdddtelresidencial}',
                            srptelresidencial   = '{$srptelresidencial}',
                            srptipoavaliador    = {$srptipoavaliador},
                            srptipoavaliado     = {$srptipoavaliado},
                            srpstatus           = '{$srpstatus}'
                    WHERE srpid = {$srpid} RETURNING srpid
                ";
            }
            //ver($sql,d);
            $srpid_db = $db->pegaUm($sql);
        }

        #GRADUAÇÃO
        if( $srpid_db > 0 ){
            if($srpid != ''){
                $sql = "
                    DELETE FROM rcs.itemgraduacao WHERE srpid = {$srpid};
                ";
                $db->executar($sql);
            }
            if($grdid[0] != ''){
                $a = 0;
                foreach( $grdid as $k){
                    if( $grdid[$a] != '' ){
                        $sql_grad .= "
                            INSERT INTO rcs.itemgraduacao( grdid, srpid ) VALUES ( {$k}, {$srpid_db} ) RETURNING grdid;
                        ";
                    }
                    $a = $a + 1;
                }
                $dado = $db->pegaUm($sql_grad);
            }
        }

        #ESPECIALIZAÇÃO
        if( $srpid_db > 0 ){
            if($srpid[0] != ''){
                $sql = "
                    DELETE FROM rcs.itemcursopos WHERE icptipo = 'E' AND srpid = {$srpid};
                ";
                $db->executar($sql);
            }
            if($espid[0] != ''){
                $a = 0;
                foreach( $espid as $k){
                    if( $espid[$a] != '' ){
                        $sql_esp .= "
                            INSERT INTO rcs.itemcursopos( sbcid, srpid, icptipo ) VALUES ( {$k}, {$srpid_db}, 'E' ) RETURNING icpid;
                        ";
                    }
                    $a = $a + 1;
                }
                $dado = $db->pegaUm($sql_esp);
            }
        }

        #MESTRADO
        if( $srpid_db > 0 ){
            if($srpid != ''){
                $sql = "
                    DELETE FROM rcs.itemcursopos WHERE icptipo = 'M' AND srpid = {$srpid};
                ";
                $db->executar($sql);
            }
            if($mesid[0] != ''){
                $a = 0;
                foreach( $mesid as $k){
                    if( $mesid[$a] != '' ){
                        $sql_mest .= "
                            INSERT INTO rcs.itemcursopos( sbcid, srpid, icptipo ) VALUES ( {$k}, {$srpid_db}, 'M' ) RETURNING icpid;
                        ";
                    }
                    $a = $a + 1;
                }
                $dado = $db->pegaUm($sql_mest);
            }
        }

        #DOUTORADO
        if( $srpid_db > 0 ){
            if($srpid != ''){
                $sql = "
                    DELETE FROM rcs.itemcursopos WHERE icptipo = 'D' AND srpid = {$srpid};
                ";
                $db->executar($sql);
            }
            if($douid[0] != ''){
                $a = 0;
                foreach( $douid as $k){
                    if( $douid[$a] != '' ){
                        $sql_dout .= "
                            INSERT INTO rcs.itemcursopos( sbcid, srpid, icptipo ) VALUES ( {$k}, {$srpid_db}, 'D' ) RETURNING icpid;
                        ";
                    }
                    $a = $a + 1;
                }
                $dado = $db->pegaUm($sql_dout);
            }
        }

        #EIXO TÉCNOLOGICO
        if( $srpid_db > 0 ){
            if( $srpid != '' ){
                $sql_eixo = "DELETE FROM rcs.itemeixo WHERE srpid = {$srpid} AND itxarea = 'T';";
            }

            if( $dados['extid_tec_1'] != '' ){
                $sql_eixo .= "INSERT INTO rcs.itemeixo( srpid, extid, itxnivel, itxarea ) VALUES ( {$srpid_db}, {$dados['extid_tec_1']}, 1, 'T' ) RETURNING itxid;";
            }
            if( $dados['extid_tec_2'] != '' ){
                $sql_eixo .= "INSERT INTO rcs.itemeixo( srpid, extid, itxnivel, itxarea ) VALUES ( {$srpid_db}, {$dados['extid_tec_2']}, 2, 'T' ) RETURNING itxid;";
            }
            if( $dados['extid_tec_3'] != '' ){
                $sql_eixo .= "INSERT INTO rcs.itemeixo( srpid, extid, itxnivel, itxarea ) VALUES ( {$srpid_db}, {$dados['extid_tec_3']}, 3, 'T' ) RETURNING itxid;";
            }
            if($sql_eixo != ''){
                $dado = $db->pegaUm($sql_eixo);
            }
        }

        #EIXO COGNITIVO
        if( $srpid_db > 0 ){
            if( $srpid != '' ){
                $sql_eixo = "DELETE FROM rcs.itemeixo WHERE srpid = {$srpid} AND itxarea = 'C';";
            }

            if( $dados['extid_cog_1'] != '' ){
                $sql_eixo .= "INSERT INTO rcs.itemeixo( srpid, extid, itxnivel, itxarea ) VALUES ( {$srpid_db}, {$dados['extid_cog_1']}, 1, 'C' ) RETURNING itxid;";
            }

            if( $dados['extid_cog_2'] != '' ){
                $sql_eixo .= "INSERT INTO rcs.itemeixo( srpid, extid, itxnivel, itxarea ) VALUES ( {$srpid_db}, {$dados['extid_cog_2']}, 2, 'C' ) RETURNING itxid;";
            }
            if( $dados['extid_cog_3'] != '' ){
                $sql_eixo .= "INSERT INTO rcs.itemeixo( srpid, extid, itxnivel, itxarea ) VALUES ( {$srpid_db}, {$dados['extid_cog_3']}, 3, 'C' ) RETURNING itxid;";
            }
            if($sql_eixo != ''){
                $dado = $db->pegaUm($sql_eixo);
            }
        }

        if( $dsamatricula != '' ){
            $sql_aceito = "
                UPDATE rcs.dadossiape
                    SET dsatermoaceite01 = '{$dsatermoaceite01}',
                        dsatermoaceite02 = '{$dsatermoaceite02}',
                        dsatermoaceite03 = '{$dsatermoaceite03}'
                WHERE dsamatricula = '{$dsamatricula}' RETURNING dsamatricula;
            ";
            $dado = $db->pegaUm($sql_aceito);
        }

        if( $srpid_db > 0 ){
            $pflcod = PERFIL_RCS_PROFESSOR;

            #VERIFICANDO SE USUÁRIO JÁ ESTA CADASTRO NO SIMEC.
            $sql_usuario = "SELECT usucpf FROM seguranca.usuario WHERE usucpf = '{$srpnumcpf}'";
            $cpf_true = $db->pegaUm($sql_usuario);

            #INSERINDO NA BASE PARA PODER CADASTRAR NA TABELA USUARIORESPONSABILIDADE
            if( empty($cpf_true) ){
               $sql_insert_usuario = "
                    INSERT INTO seguranca.usuario(
                            usucpf, usunome, usuemail, usustatus, ususenha, usuchaveativacao, pflcod,suscod, muncod, usudatanascimento, entid)
                        VALUES (
                            '{$srpnumcpf}', '{$srpdsc}', '{$srpemail}', 'I', 'semsenha', 'F', '{$pflcod}', 'B', {$muncod}, '{$srpdtnascimento}', {$srplotacao}
                        ) RETURNING usucpf
                ";

                $insert_true = $db->executar($sql_insert_usuario);

                if($insert_true > 0){
                    $usuario_true = $db->commit();
                }

                if($usuario_true > 0){
                    $sql_per_usu = "INSERT INTO seguranca.perfilusuario (pflcod,usucpf) VALUES ($pflcod,'$srpnumcpf')";
                    $db->executar($sql_per_usu);

                    $sql = "SELECT usucpf FROM seguranca.usuario_sistema WHERE usucpf='$srpnumcpf' and sisid=".$_SESSION['sisid'];
                    if ( !$db->pegaUm($sql) ){
                        $sql_usu_sis = "INSERT INTO seguranca.usuario_sistema (sisid,pflcod,susdataultacesso,usucpf,suscod) VALUES ({$_SESSION['sisid']},$pflcod,now(),'$srpnumcpf','B')";
                        $db->executar($sql_usu_sis);
                    }
                    $db->commit();
                    $cpf_true = 1;
                }
            }

            if($cpf_true > 0){
                #LOTAÇAO/ USUARIO RESPONSABILIDADE UNIDADE
                if(!empty($srplotacao) && !empty($srpnumcpf)){
                    #TRANSFORMANDO TODAS AS UNIDADES VINCULADAS AO CPF PARA INATIVAS. DEVE EXISTIR SOMENTE UMA UNIDADE(LOTAÇÃO) VINCULADA PARA UM CPF.
                    $sql_uresp_inativa = "
                        UPDATE rcs.usuarioresponsabilidade
                            SET rpustatus = 'I'
                        WHERE usucpf = '{$srpnumcpf}' AND pflcod = {$pflcod};
                    ";
                    $db->executar($sql_uresp_inativa);

                    $sql_uresp = "
                        INSERT INTO rcs.usuarioresponsabilidade (
                                pflcod, usucpf, rpustatus, rpudata_inc, entid
                            ) VALUES (
                                {$pflcod}, '{$srpnumcpf}', 'A', 'now()', {$srplotacao}
                            );
                    ";
                    $db->executar($sql_uresp);
                }
            }
        }

        if( $srpid_db > 0 ){
            $db->commit();
            if( $srpstatus == 'A' ){
                $db->sucesso( 'principal/saberes/cad_inscricao_prof', '&srpid='.$srpid_db );
            }else{
                $db->sucesso( 'principal/saberes/lista_grid_isncricao_prof', '&srpnumcpf='.$srpnumcpf );
            }
        }else{
            $db->rollback();
            $db->insucesso($msg, '', 'principal/saberes/cad_inscricao_prof&acao=A');
        }
    }

    /**
     * functionName validarCep
     * @name functionName validarCep
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é post do formulario de cadastro dos dados do professor
     *
     * @return string verifica se o CEP é valido caso seja retorna S.
     *
     * @version v1
    */
    function validarCep($cep){
        $cep = trim($cep);

        $avaliaCep = ereg("^[0-9]{5}-[0-9]{3}$", $cep);

        if(!$avaliaCep) {
            $result = 'N';
        }else{
            $result = 'S';
        }
        return $result;
    }

/**
     * Carrega os perfis ativos do usuario no modulo atual, caso ele tenha algum perfil ativo.
     *
     * @global class $db
     * @param string $cpf
     * @return void
     */
    function verificarUsuarioAtivoJS($dados)
    {
       $cpf = $dados['cpf'];
//        if(!$dados['cpf']) $cpf = $_REQUEST['cpf'];

        $cpf = trim(str_replace(array('.' , '-'), '', $cpf));


        global $db;

        $sql = "select
                    *
                from
                    seguranca.perfilusuario pu
                inner join
                    seguranca.perfil p on p.pflcod = pu.pflcod
                and
                    pu.usucpf = '{$cpf}'
                and
                    p.sisid = {$_SESSION['sisid']}
                and
                    pflstatus = 'A'";
        $perfisUsuarioOrigem = $db->carregar($sql);


        if($perfisUsuarioOrigem){
            echo 1;
        } else {
            echo 0;
        }
        exit;
    }

    function verificarUsuarioCadastradoJS($dados){
        $cpf = $dados['cpf'];
        $srpid = $dados['srpid'];
//        if(!$dados['cpf']) $cpf = $_REQUEST['cpf'];

        $cpf = trim(str_replace(array('.' , '-'), '', $cpf));

        global $db;

        if($srpid){
            $and = "AND srpid != '{$srpid}'";
        }

        $sql = "select * from rcs.servidoresprofessor
                WHERE srpnumcpf = '{$cpf}'
                {$and}";
        $perfisUsuarioOrigem = $db->carregar($sql);
//        ver($perfisUsuarioOrigem , $sql,d);
        if($perfisUsuarioOrigem){
            echo 1;
        } else {
            echo 0;
        }
        exit;
    }

    /**
     * functionName verificaSeExisteBancaNumProcesso
     *
     * @name functionName verificaSeExisteBanca
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é post do formulario para a omntagem da banca avaliadora.
     *
     * @return string verifica se a banca avaliadora para o professor avaliado como mesmo número de processo. Se sim retorna S.
     *
     * @version v1
    */
    function verificaSeExisteBancaNumProcesso( $dados ){
        global $db;

        $dsamatricula   = $dados['dsamatricula'];
        $avanumprocesso = $dados['numprocesso'];

        $sql = "
            SELECT  a.srpid
            FROM rcs.servidoresprofessor AS s
            LEFT JOIN rcs.avaliado AS a ON a.srpid = s.srpid
            WHERE s.dsamatricula = '{$dsamatricula}' AND a.avanumprocesso = '{$avanumprocesso}'
        ";
        $existe = $db->pegaUm($sql);

        if( $existe > 0 ){
            $data["existe_banca_nprocesso"] = "S";
        }else{
            $data["existe_banca_nprocesso"] = "N";
        }
        echo simec_json_encode( $data );
        die;
    }

    /**
     * functionName verificaExisteBancaAvaliadora
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $srpid id do servidor professor na qual se criara a banca avaliadora. Paremetro obrigatório.
     * @return string "S" para sim, tem permissão para criar banca avaliadora, "N" para não, já existe banca formada não tem permissão para criar uma nova.
     *
     * @version v1
    */
    function verificaExisteBancaAvaliadora( $srpid ){
        global $db;

        $sql = "
            SELECT  DISTINCT ao.avaid
            FROM rcs.servidoresprofessor AS s
            JOIN rcs.avaliado AS ao ON ao.srpid = s.srpid
            JOIN rcs.avaliador AS ar ON ar.avaid = ao.avaid
            WHERE s.srpid = {$srpid};
        ";
        $avaid = $db->pegaUm($sql);

        if( $avaid > 0 ){
            $result = 'N';#JÁ EXISTE BANCA FORMADA NÃO TEM PERMISSÃO PARA CRIAR UMA NOVA.
        }else{
            $result = 'S';#TEM PERMISSÃO PARA CRIAR A BANCA AVALIADORA.
        }
        $result = 'S';
        return $result;
    }

    function verificaUsusarioAtivo( $dados ){
        global $db;

        #NP - NÃO TEM PERMISSÃO
        #TP - TEM PERMISSÃO

        $srpnumcpf = $dados['srpnumcpf'];

        $sql = "
            SELECT srpnumcpf FROM rcs.servidoresprofessor WHERE srpnumcpf = '{$srpnumcpf}' AND srpstatus = 'A';
        ";
        $resp_cpf = $db->pegaUm($sql);

        if( $resp_cpf > 0 ){
            echo 'NP';
        }else{
            echo 'TP';
        }
        die();
    }

    /**
     * functionName verificaUnidadesAssociadasProfessor
     *
     * @author Luciano F. Ribeiro
     *
     * @param string ...
     * @return string entidades relacionadas ao professor.
     *
     * @version v1
    */
    function verificaUnidadesAssociadasProfessor( $cpf ){
        global $db;

        $usucpf = str_replace( '.', '', str_replace('-','',$cpf) );
        $pflcod = PERFIL_RCS_PROFESSOR;

        $sql = "
            SELECT  DISTINCT e.entid as codigo
            FROM entidade.entidade e

            INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
            INNER JOIN rcs.orgaofuncao of ON of.funid = fe.funid
            INNER JOIN rcs.orgao o ON o.orgid = of.orgid
            INNER JOIN rcs.usuarioresponsabilidade ur ON ur.entid = e.entid AND ur.rpustatus = 'A'

            WHERE ur.rpustatus='A' AND ur.usucpf = '{$usucpf}' AND ur.pflcod = {$pflcod}
        ";
        $entid = $db->pegaUm($sql);

        if( $entid != '' ){
            $existe = 'S';
        }else{
            $existe = 'N';
        }
        return $existe;
    }


# ------------------------------------------------------------------------------------------------------------------------------------ #

    /**
     * functionName buscarAvaliadoresExternos
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer $srplotacao: entidade onde avalido é lotado. entid da tabela rcs.usuarioresponsabilidade;
     * @param integer $extid (opcional): tipo de eixo podendo ser tecnologico e cognitivo. extid da tabela rcs.eixotecnologico;
     * @param integer $avaliador_aex: quantidade de avaliador exerto solicitado para avaliação.
     * @param integer $avaliadores_excludentes (opcional): avaliadores que já foram selecioandos para avaliação. Usado para não repetir avaliador.
     *
     * @return array $arry_srpid: array com os avaliadores selecionados. É usado o srpid paraidentificar os avaliadores.
     *
     * @version v1
    */
    //function buscarAvaliadoresExternos( $srplotacao, $orgid, $grdid = '', $extid = '', $avaliador_aex, $avaliadores_excludentes = '' , $srpid ){
    function buscarAvaliadoresExternos( $srplotacao, $grdid = '', $extid = '', $avaliador_aex, $avaliadores_excludentes = '' , $srpid ){
        global $db;

        #CASO $grdid SEJA VAZIO, BUSCA AVALIADORES SEM GRADUAÇÃO DEFINIDOS.
        if( $grdid != '' ){
            $existe_graduacao = "AND itg.grdid IN ({$grdid})";
        }else{
            $existe_graduacao = "";
        }

        #CASO $extid SEJA VAZIO, BUSCA AVALIADORES SEM EIXOS DEFINIDOS.
        if( $extid != '' ){
            $existe_exios = "AND e.extid IN ({$extid})";
        }else{
            $existe_exios = "";
        }

        #CLAUSULA USADA PARA EXCLUIR DA BUSCA OS AVALIADORES JÁ "BUSCADOS" NA PRIMEIRA SELEÇÃO.
        if( $avaliadores_excludentes != '' ){
            $avaliadores_excludentes = "AND s.srpid NOT IN ({$avaliadores_excludentes})";
        }else{
            $avaliadores_excludentes = "";
        }

        #CASO O AVALIADO SEJA DE UM ORGÃO MILITAR É BUSCADO AVALIADORES QUE TAMBÉM SEJAM DE ORGÃO MILITAR.
//        if( $orgid == 3 ){
//            $join_militar = "
//                JOIN entidade.entidade ee ON ee.entid = cast(s.srplotacao as integer)
//                JOIN entidade.funcaoentidade efe ON efe.entid = ee.entid
//                JOIN rcs.orgaofuncao rof ON rof.funid = efe.funid
//                JOIN rcs.orgao ro ON ro.orgid = rof.orgid AND ro.orgstatus = 'A'
//            ";
//            $where_militar = "AND rof.orgid = 3";
//        }
        $join_militar = '';
        $where_militar= '';

        #AVALIADORES EXTERNOS.
        $sql = "
            SELECT  DISTINCT s.srpid
            FROM rcs.servidoresprofessor AS s
            LEFT JOIN rcs.itemgraduacao itg ON itg.srpid = s.srpid
            LEFT JOIN rcs.itemeixo AS e ON e.srpid = s.srpid
            {$join_militar}
            WHERE s.srpstatus = 'A' AND s.srptipoavaliador = true {$where_militar}

            AND s.srpid <> {$srpid} AND srplotacao <> '{$srplotacao}' {$existe_graduacao} {$existe_exios} {$avaliadores_excludentes}
            --AND s.srpid NOT IN ( SELECT a.srpid FROM rcs.avaliador AS a WHERE avrstatus = 'A' )--PRIMEIRA REGRA - NÃO SELECIONA OS PROFESSORES QUE JÁ SÃO AVALIADORES INDEPEMDENTE DE QUAL BANCA ELE ESTEJA. (NÃO É MAIS USADA)
            AND s.srpid NOT IN ( SELECT v.srpid FROM rcs.avaliado AS a JOIN rcs.avaliador AS v ON v.avaid = a.avaid WHERE a.srpid = {$srpid} )--SEGUNDA REGRA - NÃO SELECIONA OS PROFESSORES QUE JÁ FAZEM PARTE DA BANCA DO AVALIADO.
            AND s.srpid NOT IN (SELECT srpid_avaliador FROM rcs.avaliador_execluido_avaliado WHERE srpid_avaliado = {$srpid}) --O AVALIADOR NÃO PODE VOLTAR PARA A BANCA NA QUAL ELE FOI EXCLUIDO.
            AND s.srpid NOT IN ( SELECT srpid FROM rcs.avaliador WHERE avrtipoavaliador = 'AEX' GROUP BY srpid HAVING COUNT(srpid) > 10 ) --AVALIAÇÃO EXTERNA - O AVALIADOR NÃO PODE ESTA PARTICIPADO EM MAIS DE 5 BANCAS.

            LIMIT {$avaliador_aex};
        ";
        $arry_srpid = $db->carregarColuna($sql);

        return $arry_srpid;
    }

    /**
     * functionName buscarAvaliadoresInternos
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer $srpid: id do avaliado, para que o memso não seja selecionado como o seu prorpio avaliador;
     * @param integer $srplotacao: entidade onde avalido é lotado. entid da tabela rcs.usuarioresponsabilidade;
     * @param integer $extid (opcional): tipo de eixo podendo ser tecnologico e cognitivo. extid da tabela rcs.eixotecnologico;
     * @param integer $avaliador_aex: quantidade de avaliador exerto solicitado para avaliação.
     * @param integer $avaliadores_excludentes (opcional): avaliadores que já foram selecioandos para avaliação. Usado para não repetir avaliador.
     *
     * @return array $result: array com os avaliadores selecionados. É usado o srpid paraidentificar os avaliadores.
     *
     * @version v1
    */
    function buscarAvaliadoresInternos( $srplotacao, $grdid = '', $extid = '', $numero_avaliador, $avaliadores_excludentes = '' , $srpid ){
        global $db;

        #CASO $grdid SEJA VAZIO, BUSCA AVALIADORES SEM GRADUAÇÃO DEFINIDOS.
        if( $grdid != '' ){
            $existe_graduacao = "AND itg.grdid IN ({$grdid})";
        }else{
            $existe_graduacao = "";
        }

        #CASO $extid SEJA VAZIO, BUSCA AVALIADORES SEM EIXOS DEFINIDOS.
        if( $extid != '' ){
            $existe_exios = "AND e.extid IN ({$extid})";
        }else{
            $existe_exios = "";
        }

        #CLAUSULA USADA PARA EXCLUIR DA BUSCA OS AVALIADORES JÁ "BUSCADOS" NA PRIMEIRA SELEÇÃO.
        if( $avaliadores_excludentes != '' ){
            $avaliadores_excludentes = "AND s.srpid NOT IN ({$avaliadores_excludentes})";
        }else{
            $avaliadores_excludentes = "";
        }

        #AVALIADORES INTERNO.
        $sql = "
            SELECT  DISTINCT s.srpid
            FROM rcs.servidoresprofessor AS s
            LEFT JOIN rcs.itemgraduacao itg ON itg.srpid = s.srpid
            LEFT JOIN rcs.itemeixo AS e ON e.srpid = s.srpid
            WHERE s.srpstatus = 'A' AND s.srptipoavaliador = true AND s.srpid <> {$srpid} AND srplotacao = '{$srplotacao}' {$existe_graduacao} {$existe_exios} {$avaliadores_excludentes}
                    --AND s.srpid NOT IN ( SELECT a.srpid FROM rcs.avaliador AS a WHERE avrstatus = 'A' )--PRIMEIRA REGRA - NÃO SELECIONA OS PROFESSORES QUE JÁ SÃO AVALIADORES INDEPEMDENTE DE QUAL BANCA ELE ESTEJA.
                    AND s.srpid NOT IN ( SELECT v.srpid FROM rcs.avaliado AS a JOIN rcs.avaliador AS v ON v.avaid = a.avaid WHERE a.srpid = {$srpid} )--SEGUNDA REGRA - NÃO SELECIONA OS PROFESSORES QUE JÁ FAZEM PARTE DA BANCA DO AVALIADO.
                    --AND s.srpid NOT IN (SELECT srpid_avaliador FROM rcs.avaliador_execluido_avaliado WHERE srpid_avaliado = {$srpid})--TERCEIRA REGRA - NÃO SELECIONA OS PROFESSORES QUE JÁ FORAM AVALIADORES DA BANCA DO AVALIADO.
                    AND s.srpid NOT IN ( SELECT srpid FROM rcs.avaliador WHERE avrtipoavaliador = 'AIN' GROUP BY srpid HAVING COUNT(srpid) > 15 ) --AVALIAÇÃO INTERNA - O AVALIADOR NÃO PODE ESTA PARTICIPADO EM MAIS DE 15 BANCAS.
            LIMIT {$numero_avaliador};
        ";
        $arry_srpid = $db->carregarColuna($sql);

        return $arry_srpid;
    }

    /**
     * functionName buscaEixosProfAvaliado
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer $srpid: id do avaliado, para que o memso não seja selecionado como o seu prorpio avaliador;
     *
     * @return array $resultado: array com as variaveis resultado: existencia de eixos, quais os exisos e os eixos existentes já preparados para "uso" em formato de string, relacionados com o avaliado.
     *
     * @version v1
    */
    function buscaEixosProfAvaliado( $srpid ){
        global $db;

        $resultado['existe_eixos_tec'] = 'N';
        $resultado['existe_eixos_cog'] = 'N';

        #BUSCA OS EIXOS TECNOLOGICOS e COGNITIVOS DO PROFESSOR A SER AVALIADO.
        if( $srpid != '' ){
            #BUSCA OS EIXOS TECNOLOGICOS DO PROFESSOR A SER AVALIADO.
            $sql = "
                SELECT  e.extid
                FROM rcs.servidoresprofessor AS s
                JOIN rcs.itemeixo AS e ON e.srpid = s.srpid
                WHERE e.itxarea = 'T' AND s.srpid = {$srpid}
            ";
            $extid_arr_tec = $db->carregarColuna($sql);

            if( count($extid_arr_tec) > 0 ){
                $resultado['extid_tec'] = implode(',', $extid_arr_tec);
                $resultado['existe_eixos_tec'] = 'S';
            }

            #BUSCA OS EIXOS COGNITIVOS DO PROFESSOR A SER AVALIADO.
            $sql = "
                SELECT  e.extid
                FROM rcs.servidoresprofessor AS s
                JOIN rcs.itemeixo AS e ON e.srpid = s.srpid
                WHERE e.itxarea = 'C' AND s.srpid = {$srpid}
            ";
            $extid_arr_cog = $db->carregarColuna($sql);

            if( count($extid_arr_cog) > 0 ){
                $resultado['extid_cog'] = implode(',', $extid_arr_cog);
                $resultado['existe_eixos_cog'] = 'S';
            }
        }
        return $resultado;
    }

    /**
     * functionName buscaGraduacaoProfAvaliado
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer $srpid: id do avaliado é usado para buscar os ;
     *
     * @return array $resultado: array com os id das graduações do avaliado. Já preparados para "uso" em formato de string.
     *
     * @version v1
    */
    function buscaGraduacaoProfAvaliado( $srpid ){
        global $db;

        $resultado['existe_grdid'] = 'N';

        #BUSCA DAS GRADUAÇÕES DO AVALIADO.
        if( $srpid != '' ){
            $sql = "
                SELECT grdid
                FROM rcs.servidoresprofessor AS s
                JOIN rcs.itemgraduacao AS g ON g.srpid = s.srpid
                WHERE s.srpid = {$srpid}
            ";
            $arr_grdid = $db->carregarColuna($sql);

            if( count($arr_grdid) > 0 ){
                $resultado['array_grdid'] = implode(',', $arr_grdid);
                $resultado['existe_grdid'] = 'S';
            }
        }
        return $resultado;
    }

    /**
     * functionName criaBancaAvaliadora
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer $srpid: id do avaliado, para que o memso não seja selecionado como o seu prorpio avaliador;
     * @param integer $srplotacao: entidade onde avalido é lotado. entid da tabela rcs.usuarioresponsabilidade;
     * @param integer $orgid: id que identifica qual o tipo de orgão o professor esta lotado, sendo: 1=superior; 2=profissional; 3=militar;
     * @param integer $avaliador_aex: número de avaliadores externos desejado para avaliação;
     * @param integer $avaliador_ain - opcional: número de avaliadores internos desejado para avaliação;
     *
     * @param integer $avaliador_aes - opcional: número de avaliadores externos suplentes desejado para avaliação;
     * @param integer $avaliador_ais - opcional: número de avaliadores internos suplentes desejado para avaliação;
     *
     * @return array $result: array com os avaliadores selecionados. É usado o srpid paraidentificar os avaliadores.
     *
     * @version v1
    */
    function criaBancaAvaliadora( $avaid, $srpid, $srplotacao, $orgid, $avaliador_aex, $avaliador_ain, $avaliador_aes, $avaliador_ais ){
        global $db;

        $permissao = verificaExisteBancaAvaliadora( $srpid );

        #BUSCA DOS CURSOS DE GRADUAÇÃO DO PROFESSOR A SER AVALIADO.
        $graduacao = buscaGraduacaoProfAvaliado( $srpid );

        $existe_graduacao = $graduacao['existe_grdid'];
        $grdid_grd        = $graduacao['array_grdid'];

        #BUSCA EIXOS DO PROFESSOR QUE SERÁ AVALIADO.
        $eixos = buscaEixosProfAvaliado( $srpid );

        $extid_tec  = $eixos['extid_tec'];
        $extid_cog  = $eixos['extid_cog'];

        $controle = 0;

        if( $permissao == 'S' ){
            #SELECIONAR AVALIADORES EXTERNOS.
            if( $avaliador_aex > 0 ){
                //$result = selecionaAvaliadoresExternos( $srplotacao, $orgid, $avaliador_aex, $grdid_grd, $extid_tec, $extid_cog , $srpid );
                $result = selecionaAvaliadoresExternos( $srplotacao, $avaliador_aex, $grdid_grd, $extid_tec, $extid_cog , $srpid );
                $retorno = salvarAvaliadores($avaid, $result, 'AEX');
                if( $retorno == 'OK' ){
                    $msg['aex'] = "Avaliadores Externos";
                    $controle = 1;
                }
            }

            #SELECIONAR AVALIADORES EXTERNOS SUPLENTES.
            if( $avaliador_aes > 0 ){
                //$result = selecionaAvaliadoresExternos( $srplotacao, $orgid, $avaliador_aes, $grdid_grd, $extid_tec, $extid_cog , $srpid );
                $result = selecionaAvaliadoresExternos( $srplotacao, $avaliador_aes, $grdid_grd, $extid_tec, $extid_cog , $srpid );
                $retorno = salvarAvaliadores($avaid, $result, 'AES');
                if( $retorno == 'OK' ){
                    $msg['aes'] = "Avaliadores Externos Suplentes";
                    $controle = 1;
                }
            }

            #SELECIONAR AVALIADORES INTERNO.
            if( $avaliador_ain > 0 ){
                $result = selecionaAvaliadoresInternos( $srplotacao, $avaliador_ain, $grdid_grd, $extid_tec, $extid_cog, $srpid );
                $retorno = salvarAvaliadores($avaid, $result, 'AIN');
                if( $retorno == 'OK' ){
                    $msg['ain'] = "Avaliadores Internos";
                    $controle = 1;
                }
            }

            #SELECIONAR AVALIADORES INTERNO SUPLENTES.
            if( $avaliador_ais > 0 ){
                $result = selecionaAvaliadoresInternos( $srplotacao, $avaliador_ais, $grdid_grd, $extid_tec, $extid_cog, $srpid );
                $retorno = salvarAvaliadores($avaid, $result, 'AIS');
                if( $retorno == 'OK' ){
                    $msg['ais'] = "Avaliadores Internos Suplentes";
                    $controle = 1;
                }
            }
            if( $controle == 1){
                $msg['ok'] = 'OK';
                return $msg;
            }
        }else{
            $db->insucesso('Já existe Banca Avaliadora para esse Professor, não é possível formar outra!', '', 'principal/sorteio_avaliadores/cad_sorteio_avaliadores&acao=A');
        }
    }

    /**
     * functionName salvarAvaliadores
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer $avaid: ;
     * @param array $result: ;
     * @param string $tipo_avaliador:
     *
     * @return string OK/ERROR: .
     *
     * @version v1
    */
    function salvarAvaliadores( $avaid, $result, $tipo_avaliador ){
        global $db;

        foreach ($result as $key => $value) {
            $sql = " INSERT INTO rcs.avaliador( avaid, srpid, avrtipoavaliador ) VALUES ( {$avaid}, $value, '{$tipo_avaliador}' ) RETURNING avrid; ";
            $avrid = $db->pegaUm($sql);
        }

        if( $avrid > 0 ){
            return 'OK';
        }else{
            return 'ERROR';
        }
    }

    /**
     * functionName selecionaAvaliadoresExternos
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer $srplotacao: número de avaliador externo desejado para avaliação do professor;
     * @param integer $avaliador_aex:
     * @param string $existe_eixos_tec: mostra a existencia do eixos tecnologicos. 'S - existe eixo tecnologico' N - não existe eixo tecnologico;
     * @param string $existe_eixos_cog: mostra a existencia do eixos cognitivos. 'S - existe eixo cognitivos' N - não existe eixo cognitivos;
     * @param string $extid_tec: trás extid, os id's dos eixos tecnologicos do avalido como string 'tratados' do avalido;
     * @param string $extid_cog: trás extid, os id's dos eixos cognitivos como string 'tratados' do avalido;
     * @param string $srpid: trás srpid, o id identificador do professor;
     *
     * @return array $result: array com os avaliadores selecionados. É usado o srpid para identificar os avaliadores.
     *
     * @version v1
    */
    //function selecionaAvaliadoresExternos( $srplotacao, $orgid, $avaliador_aex, $grdid_grd, $extid_tec, $extid_cog , $srpid = null){
    function selecionaAvaliadoresExternos( $srplotacao, $avaliador_aex, $grdid_grd, $extid_tec, $extid_cog , $srpid = null){
        global $db;

        if( $avaliador_aex > 0 ){
            #1º - BUSCA AVALIADORES EXTERNOS - COM A MESMA GRADUAÇÃO.
            //$arr_avaliadores_grad = buscarAvaliadoresExternos($srplotacao, $orgid, $grdid_grd, '', $avaliador_aex , '' , $srpid);
            $arr_avaliadores_grad = buscarAvaliadoresExternos($srplotacao, $grdid_grd, '', $avaliador_aex , '' , $srpid);

            #NÚMERO DE AVALIADORES.
            $numero_avaliadores = count($arr_avaliadores_grad);

            #CASO O NÚMERO DE AVALIADORES SEJA ATENDIDO, DEFINE-SE OS AVALIADORES.
            if( $numero_avaliadores == $avaliador_aex ){
                    #RESULTADO FINAL
                    $result = $arr_avaliadores_grad;

            #SE O NÚMERO DE AVALIADORES NÃO FOR ATENDIDO É REFEITA A BUSCA USANDO OUTRO EIXO OU NENHUM EIXO E EXCLUIDO DESSA NOVA PESQUISA OS AVALIADORES JÁ "BUSCADOS, PARA QUE NÃO REPITA-OS NOVAMENTE".
            }else{
                #NÚMERO DE AVALIADORES RESTANTES PARA COMPLETAR O DESEJADO.
                $num_diferenca = $avaliador_aex - $numero_avaliadores;

                #AVALIADORES JÁ "BUSCADOS".
                $avaliadores_excludentes = implode(',', $arr_avaliadores_grad);

                #2º - BUSCA AVALIADORES EXTERNOS - EIXO TECNOLOGICO.
                //$arr_avaliadores_tec = buscarAvaliadoresExternos($srplotacao, $orgid, '', $extid_tec, $num_diferenca , $avaliadores_excludentes, $srpid);
                $arr_avaliadores_tec = buscarAvaliadoresExternos($srplotacao, '', $extid_tec, $num_diferenca , $avaliadores_excludentes, $srpid);

                #UNI OS DOIS RESULTADOS, OS AVALIADORES DEFINIDOS NA PRIMEIRA BUSCA E OS RESTANTES BUSCADOS NESSA ULTIMA BUSCA - EIXO COGNITIVOS.
                $arr_avaliadores_grad_unido_tec = array_merge($arr_avaliadores_grad, $arr_avaliadores_tec);

                #NÚMERO DE AVALIADORES.
                $numero_avaliadores = count($arr_avaliadores_grad_unido_tec);

                #CASO O NÚMERO DE AVALIADORES SEJA ATENDIDO, DEFINE-SE OS AVALIADORES.
                if( $numero_avaliadores == $avaliador_aex ){
                    #RESULTADO FINAL
                    $result = $arr_avaliadores_grad_unido_tec;

                }else{
                    #NÚMERO DE AVALIADORES RESTANTES PARA COMPLETAR O DESEJADO.
                    $num_diferenca = $avaliador_aex - $numero_avaliadores;

                    #AVALIADORES JÁ "BUSCADOS".
                    $avaliadores_excludentes = implode(',', $arr_avaliadores_grad_unido_tec);

                    #3º - BUSCA AVALIADORES EXTERNOS - EIXO COGNITIVO.
                    //$arr_avaliadores_cog = buscarAvaliadoresExternos($srplotacao, $orgid, '', $extid_cog, $num_diferenca, $avaliadores_excludentes, $srpid);
                    $arr_avaliadores_cog = buscarAvaliadoresExternos($srplotacao, '', $extid_cog, $num_diferenca, $avaliadores_excludentes, $srpid);

                    #UNI OS DOIS RESULTADOS, OS AVALIADORES DEFINIDOS NA PRIMEIRA BUSCA E OS RESTANTES BUSCADOS NESSA ULTIMA BUSCA - EIXO COGNITIVOS.
                    $arr_avaliadores_grad_unido_tec_unido_cog = array_merge($arr_avaliadores_grad_unido_tec, $arr_avaliadores_cog);

                    #NÚMERO DE AVALIADORES UNIDOS.
                    $numero_aval_unido = count($arr_avaliadores_grad_unido_tec_unido_cog);

                    if( $numero_aval_unido == $avaliador_aex ){
                        #RESULTADO FINAL
                        $result = $arr_avaliadores_grad_unido_tec_unido_cog;
                    }else{
                        #AVALIADORES JÁ "BUSCADOS".
                        $avaliadores_excludentes = implode(',', $arr_avaliadores_grad_unido_tec_unido_cog);

                        #EXECUTA A BUSCA DOS AVALIADORES RESTANTES SEM A DEFINIÇÃO DE EIXOS.
                        //$arr_avaliadores_rest = buscarAvaliadoresExternos($srplotacao, $orgid, '', '', $num_diferenca, $avaliadores_excludentes , $srpid);
                        $arr_avaliadores_rest = buscarAvaliadoresExternos($srplotacao, '', '', $num_diferenca, $avaliadores_excludentes , $srpid);

                        #UNI OS DOIS RESULTADOS, OS AVALIADORES DEFINIDOS NA PRIMEIRA BUSCA E OS RESTANTES BUSCADOS NESSA ULTIMA BUSCA - EIXO COGNITIVOS.
                        $arr_avaliadores_unidos_final = array_merge($arr_avaliadores_grad_unido_tec_unido_cog, $arr_avaliadores_rest);

                        #RESULTADO FINAL
                        $result = $arr_avaliadores_unidos_final;
                    }
                }
            }
        }
        return $result;
    }

    /**
     * functionName selecionaAvaliadoresInternos
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer $avaliador_aex: número de avaliador externo desejado para avaliação do professor;
     * @param string $existe_eixos_tec: mostra a existencia do eixos tecnologicos. 'S - existe eixo tecnologico' N - não existe eixo tecnologico;
     * @param string $existe_eixos_cog: mostra a existencia do eixos cognitivos. 'S - existe eixo cognitivos' N - não existe eixo cognitivos;
     * @param array $extid_arr_tec: trás extid, os id's dos eixos tecnicos do avalido;
     * @param array $extid_arr_cog: trás extid, os id's dos eixos cognitivos do avalido;
     * @param string $extid_tec: trás extid, os id's dos eixos tecnologicos como string 'tratados' do avalido;
     * @param string $extid_cog: trás extid, os id's dos eixos cognitivos como string 'tratados' do avalido;
     *
     * @return array $result: array com os avaliadores selecionados. É usado o srpid para identificar os avaliadores.
     *
     * @version v1
    */
    function selecionaAvaliadoresInternos( $srplotacao, $avaliador_ain, $grdid_grd, $extid_tec, $extid_cog , $srpid = null){
        global $db;

        if( $avaliador_ain > 0 ){
            #1º - BUSCA AVALIADORES EXTERNOS - COM A MESMA GRADUAÇÃO.
            $arr_avaliadores_grad = buscarAvaliadoresInternos($srplotacao, $grdid_grd, '', $avaliador_ain , '' , $srpid);

            #NÚMERO DE AVALIADORES.
            $numero_avaliadores = count($arr_avaliadores_grad);

            #CASO O NÚMERO DE AVALIADORES SEJA ATENDIDO, DEFINE-SE OS AVALIADORES.
            if( $numero_avaliadores == $avaliador_ain ){
                    #RESULTADO FINAL
                    $result = $arr_avaliadores_grad;

            #SE O NÚMERO DE AVALIADORES NÃO FOR ATENDIDO É REFEITA A BUSCA USANDO OUTRO EIXO OU NENHUM EIXO E EXCLUIDO DESSA NOVA PESQUISA OS AVALIADORES JÁ "BUSCADOS, PARA QUE NÃO REPITA-OS NOVAMENTE".
            }else{
                #NÚMERO DE AVALIADORES RESTANTES PARA COMPLETAR O DESEJADO.
                $num_diferenca = $avaliador_ain - $numero_avaliadores;

                #AVALIADORES JÁ "BUSCADOS".
                $avaliadores_excludentes = implode(',', $arr_avaliadores_grad);

                #2º - BUSCA AVALIADORES EXTERNOS - EIXO TECNOLOGICO.
                $arr_avaliadores_tec = buscarAvaliadoresInternos($srplotacao, '', $extid_tec, $num_diferenca , $avaliadores_excludentes, $srpid);

                #UNI OS DOIS RESULTADOS, OS AVALIADORES DEFINIDOS NA PRIMEIRA BUSCA E OS RESTANTES BUSCADOS NESSA ULTIMA BUSCA - EIXO COGNITIVOS.
                $arr_avaliadores_grad_unido_tec = array_merge($arr_avaliadores_grad, $arr_avaliadores_tec);

                #NÚMERO DE AVALIADORES.
                $numero_avaliadores = count($arr_avaliadores_grad_unido_tec);

                #CASO O NÚMERO DE AVALIADORES SEJA ATENDIDO, DEFINE-SE OS AVALIADORES.
                if( $numero_avaliadores == $avaliador_ain ){
                    #RESULTADO FINAL
                    $result = $arr_avaliadores_grad_unido_tec;

                }else{
                    #NÚMERO DE AVALIADORES RESTANTES PARA COMPLETAR O DESEJADO.
                    $num_diferenca = $avaliador_ain - $numero_avaliadores;

                    #AVALIADORES JÁ "BUSCADOS".
                    $avaliadores_excludentes = implode(',', $arr_avaliadores_grad_unido_tec);

                    #3º - BUSCA AVALIADORES EXTERNOS - EIXO COGNITIVO.
                    $arr_avaliadores_cog = buscarAvaliadoresInternos($srplotacao, '', $extid_cog, $num_diferenca, $avaliadores_excludentes, $srpid);

                    #UNI OS DOIS RESULTADOS, OS AVALIADORES DEFINIDOS NA PRIMEIRA BUSCA E OS RESTANTES BUSCADOS NESSA ULTIMA BUSCA - EIXO COGNITIVOS.
                    $arr_avaliadores_grad_unido_tec_unido_cog = array_merge($arr_avaliadores_grad_unido_tec, $arr_avaliadores_cog);

                    #NÚMERO DE AVALIADORES UNIDOS.
                    $numero_aval_unido = count($arr_avaliadores_grad_unido_tec_unido_cog);

                    if( $numero_aval_unido == $avaliador_ain ){
                        #RESULTADO FINAL
                        $result = $arr_avaliadores_grad_unido_tec_unido_cog;
                    }else{
                        #AVALIADORES JÁ "BUSCADOS".
                        $avaliadores_excludentes = implode(',', $arr_avaliadores_grad_unido_tec_unido_cog);

                        #EXECUTA A BUSCA DOS AVALIADORES RESTANTES SEM A DEFINIÇÃO DE EIXOS.
                        $arr_avaliadores_rest = buscarAvaliadoresInternos($srplotacao, '', '', $num_diferenca, $avaliadores_excludentes , $srpid);

                        #UNI OS DOIS RESULTADOS, OS AVALIADORES DEFINIDOS NA PRIMEIRA BUSCA E OS RESTANTES BUSCADOS NESSA ULTIMA BUSCA - EIXO COGNITIVOS.
                        $arr_avaliadores_unidos_final = array_merge($arr_avaliadores_grad_unido_tec_unido_cog, $arr_avaliadores_rest);

                        #RESULTADO FINAL
                        $result = $arr_avaliadores_unidos_final;
                    }
                }
            }
        }
        return $result;
    }

