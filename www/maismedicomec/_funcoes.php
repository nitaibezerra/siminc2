<?PHP
    #------------------------------------------------------------- FUNÇÕES MODULO AVALIAÇÃO DE SERVIDOR --------------------------------------------------#
    #AS FUNÇÕES SÃO: (EM ORDER ALFABETICA)
    # - atualizaComboMunicipio;
    # - buscarDadosUnidadeSaude;
    # - buscarRespostaUnidadeSaude;
    # - buscaRespostaPergunta;
    # - cadastraEntidadeQuestionarioMunicipio;
    # - iniciaVariaveisSessao;
    # - salvarAvaliacaoDescricaoGerais;
    # - salvarQuestUnidade;
    # - salvarUnidadeServico;
    # - montaListaGridUnidadeSaude;


   /**
     * functionName anexarDocumentos
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $files arquivo a ser anexado.
     * @param string $rptid id da resposta.
     * @param string $prgid id da perguta.
     * @return string retorna se foi ou não anexo o arquivo.
     *
     * @version v1
    */
    function anexarDocumentos( $files, $rptid, $prgid ){
        global $db;

        include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

        $rptid          = $rptid;
        $usucpf         = $_SESSION['usucpf'];
        $aqrdtinclusao  = "'".gmdate('Y-m-d')."'";

        $campos = array(
            'rptid'         => $rptid,
            'usucpf'        => "'".$usucpf."'",
            'arqstatus'     => "'A'",
            'aqrdtinclusao' => $aqrdtinclusao
        );

        $file = new FilesSimec('arquivoresposta', $campos, 'maismedicomec');

        if ( $files ) {
            $arquivoSalvo = $file->setUpload('Mais Médico - MEC - Anexo Referente à pergunta '.$prgid);
            if ($arquivoSalvo) {
                return 'S';
            }else{
                return 'N';
            }
        }
        exit;
    }

   /**
     * functionName anexarDocumentosAvaliacao
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $files arquivo a ser anexado.
     * @param string $rptid id da resposta.
     * @param string $prgid id da perguta.
     * @return string retorna se foi ou não anexo o arquivo.
     *
     * @version v1
    */
    function anexarDocumentosAvaliacao( $dados, $files ){
        global $db;

        include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

        $tpaid  = $dados['tpaid'];
        $avadsc = $dados['avadsc'];

        $qstid = $_SESSION['maismedicomec']['qstid'];
        $etqid = $_SESSION['maismedicomec']['etqid'];

        $usucpf         = $_SESSION['usucpf'];
        $avadtinclusao  = "'".gmdate('Y-m-d')."'";

        $campos = array(
            'etqid'         => $etqid,
            'qstid'         => $qstid,
            'usucpf'        => "'".$usucpf."'",
            'tpaid'         => $tpaid,
            'avadsc'        => "'".$avadsc."'",
            'avastatus'     => "'A'",
            'avadtinclusao' => $avadtinclusao
        );

        $file = new FilesSimec('arquivoavaliacao', $campos, 'maismedicomec');

        if ( $files ) {
            $arquivoSalvo = $file->setUpload('Mais Médico - MEC - Anexo Referente à Avaliação');
            if ($arquivoSalvo) {
                $db->sucesso("principal/instrumentoavaliacao/cad_documentos_aval", "", "Arquivos anexados com sucesso!");
            }else{
                $db->insucesso( "INSCRIÇÃO não realizada, tente novamente mais tarde!", "", "principal/instrumentoavaliacao/cad_documentos_aval" );
            }
        }
        exit;
    }

    /**
     * functionName atualizaComboMunicipio
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $estuf sigla do estado.
     * @return string  retorna a o combo com os municípios referente ao estado (UF).
     *
     * @version v1
    */
    function atualizaComboMunicipio( $estuf ){
        global $db;

        $estuf = $estuf['estuf'];

        $sql = "
            SELECT  m.muncod AS codigo,
                    m.mundescricao AS descricao
            FROM maismedicomec.municipioliberado AS l
            LEFT JOIN territorios.municipio AS m ON substr(m.muncod, 1, 6) = l.muncod
            LEFT JOIN territorios.estado AS u ON u.estuf = m.estuf
            WHERE u.estuf = '{$estuf}'
            ORDER BY m.mundescricao
        ";
        $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', 450, 'N', 'muncod', false, $muncod, null);
        die();
    }

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
    function atualizaComboMunicipioMantenedora( $estuf ){
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

    function buscarDadosUnidadeSaude( $dados ){
        global $db;

        $ussid = trim($dados['ussid']);

        $tipo = trim($dados['tipo']); # O TIPO DEFINI QUAL O PROCESSO "TELA" ESTA REQUISITANDO A INFORMAÇÃO.

        $sql = "
            SELECT  u.ussid,
                    u.ussdsc,
                    u.usscnes

            FROM maismedicomec.unidservicosaude AS u

            WHERE u.ussid = {$ussid}

            ORDER BY 1
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            $dados["ussdsc"] = iconv("ISO-8859-1", "UTF-8", $dados["ussdsc"]);
            echo simec_json_encode($dados);
            die;
        }
    }

    function buscaLiberacao_Data_Extraordinaria(){
        #define('DATA_FECHAMENTO_RECURSO', '2013-12-10GMT17:59:59');
        $data_hoje = strtotime( gmdate('Y-m-dTh:i:s') );
        $data_fech = strtotime( '2015-08-05GMT23:59:59' );

        #CASO EXTRAORDINARIO - POR FORÇA DE DECISÃO JUDICIAL. ASSOCIAÇÃO EDUCATIVA DO BRASIL
        define('ASSOCIACA_EDUCATIVA_BRASIL_SOEBRAS', 1509);
        $data_EXTRA_aber = strtotime( '2015-09-21GMT00:00:01' );
        $data_EXTRA_hoje = strtotime( gmdate('Y-m-dTh:i:s') );
        $data_EXTRA_fech = strtotime( '2015-09-30GMT23:59:59' );

        #CASO EXTRAORDINARIO - POR FORÇA DE DECISÃO JUDICIAL. SOCEC- SOCIEDADE CAPIBARIBE DE EDUCAÇÃO E CULTURA
        define('SOCIEDADE_CAPIBARIBE_EDUCACAO', 1198);
        $data_EX_aber = strtotime( '2015-09-24GMT00:00:01' );
        $data_EX_hoje = strtotime( gmdate('Y-m-dTh:i:s') );
        $data_EX_fech = strtotime( '2015-09-25GMT00:00:01' );
        
        #CASO EXTRAORDINARIO - POR FORÇA DE DECISÃO JUDICIAL. UNNESA - UNIAO DE ENSINO SUPERIOR DA AMAZONIA OCIDENTAL S/C LTDA ? EPP
        define('UNIAO_ENSINO_SUPERIOR_AMAZONIA_OCIDENTAL', 1352);
        $data_EX_aber_amaz = strtotime( '2015-10-20GMT00:00:01' );
        $data_EX_hoje_amaz = strtotime( gmdate('Y-m-dTh:i:s') );
        $data_EX_fech_amaz = strtotime( '2015-10-22GMT00:00:01' );

        #POR FORÇA DE DECISÃO JUDICIAL. O USUÁRIO DA INSTITUIÇÃO "SOEBRAS" DO CPF: , FICA HABILITADO A USAR O SISTEMA NO PERÍODO ACIMA DEFINIDO.
        if( ( $_SESSION['maismedicomec']['mntid'] == SOCIEDADE_CAPIBARIBE_EDUCACAO ) && ( $_SESSION['usucpf'] == '' ) && ( $data_EX_hoje >= $data_EX_aber ) && ($data_EX_hoje <= $data_EX_fech) ){
            $habilita = 'S';

        #POR FORÇA DE DECISÃO JUDICIAL. O USUÁRIO DA INSTITUIÇÃO "SOEBRAS" DO CPF: , FICA HABILITADO A USAR O SISTEMA NO PERÍODO ACIMA DEFINIDO.
        }elseif( ( $_SESSION['maismedicomec']['mntid'] == ASSOCIACA_EDUCATIVA_BRASIL_SOEBRAS ) && ( $_SESSION['usucpf'] == '' ) && ( $data_EXTRA_hoje >= $data_EXTRA_aber ) && ($data_EXTRA_hoje <= $data_EXTRA_fech) ){
            $habilita = 'S';
            
        #POR FORÇA DE DECISÃO JUDICIAL. O USUÁRIO DA INSTITUIÇÃO "UNNESA" DO CPF: , FICA HABILITADO A USAR O SISTEMA NO PERÍODO ACIMA DEFINIDO.    
        }elseif( ( $_SESSION['maismedicomec']['mntid'] == UNIAO_ENSINO_SUPERIOR_AMAZONIA_OCIDENTAL ) && ( $_SESSION['usucpf'] == '' ) && ( $data_EX_hoje_amaz >= $data_EX_aber_amaz ) && ($data_EX_hoje_amaz <= $data_EX_fech_amaz) ){
            $habilita = 'S';

        }elseif( ($data_hoje <= $data_fech) ){
            $habilita = 'S';

        }else{
            $habilita = 'N';
        }
        return $habilita;
    }

    function buscarRespostaUnidadeSaude( $dados ){
        global $db;

        $ussid = trim($dados['ussid']);

        $tipo = trim($dados['tipo']); # O TIPO DEFINI QUAL O PROCESSO "TELA" ESTA REQUISITANDO A INFORMAÇÃO.

        $sql = "
            SELECT  r.rusid AS rusid,
                    r.prgid AS prgid,
                    p.prgtipo AS prgtipo,
                    r.rusresposta AS rusresposta

            FROM maismedicomec.unidservicosaude AS u
            JOIN maismedicomec.respostaunidadeservicosaude AS r ON r.ussid = u.ussid
            JOIN maismedicomec.pergunta AS p ON p.prgid = r.prgid

            WHERE u.ussid = {$ussid}

            ORDER BY 1
        ";
        $dados = $db->carregar($sql);

        if($dados != ''){
            foreach ($dados as $i => $dado) {
                //$dados[$key]['rusresposta'] = utf8_encode($dado['rusresposta']);
                $dados[$i]['rusresposta'] = iconv( "ISO-8859-1", "UTF-8", stripslashes( $dado["rusresposta"] ) );
            }
        }

        if($dados != ''){
            echo simec_json_encode( $dados );
        }
        die();
    }

    function buscarRespostaUnidadeSaudeParaImpressao( $ussid, $prgid ){
        global $db;

        $sql = "
            SELECT  r.rusresposta AS rusresposta

            FROM maismedicomec.unidservicosaude AS u
            JOIN maismedicomec.respostaunidadeservicosaude AS r ON r.ussid = u.ussid
            JOIN maismedicomec.pergunta AS p ON p.prgid = r.prgid

            WHERE u.ussid = {$ussid} AND r.prgid = {$prgid}

            ORDER BY 1
        ";
        $rusresposta = $db->pegaUm($sql);
        return $rusresposta;
    }

    /**
     * functionName buscaRespostaPergunta
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $prgid id da pergunta no qual se busca a resposta.
     * @return integer $rptresposta a resposta de pergunta.
     *
     * @version v1
    */
    function buscaRespostaPergunta( $prgid ){
        global $db;

        $qstid = $_SESSION['maismedicomec']['qstid'];
        $etqid = $_SESSION['maismedicomec']['etqid'];

        $sql = "
            SELECT  rptresposta
            FROM maismedicomec.resposta

            WHERE rptstatus = 'A' AND qstid = {$qstid} AND etqid = {$etqid} AND prgid = {$prgid}
        ";
        $rptresposta = $db->pegaUm($sql);
        return $rptresposta;
    }

    /**
     * functionName buscaUnidadeCadastrada
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $muncod É o código do municípo selecioando
     * @return integer $ETQID OU $etqid retorna o id da entidadequestionario, "novo ou já existente".
     *
     * @version v1
    */
    function buscaUnidadeCadastrada( $tusid ){
        global $db;

        $qstid = $_SESSION['maismedicomec']['qstid'];
        $etqid = $_SESSION['maismedicomec']['etqid'];

        $sql = "
            SELECT  ussid
            FROM maismedicomec.unidservicosaude

            WHERE ussstatus = 'A' AND qstid = {$qstid} AND etqid = {$etqid} AND tusid = {$tusid}
        ";
        $ussid = $db->pegaUm($sql);
        return $ussid;
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
     * functionName cadastraEntidadeQuestionarioMunicipio
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $muncod É o código do municípo selecioando
     * @return integer $ETQID OU $etqid retorna o id da entidadequestionario, "novo ou já existente".
     *
     * @version v1
    */
    function cadastraEntidadeQuestionarioMunicipio( $muncod ){
        global $db;

        $qstid = $_SESSION['maismedicomec']['qstid'];

        $SQL = " SELECT etqid FROM maismedicomec.entidadequestionario WHERE qstid = {$qstid} AND muncod = '{$muncod}' ";
        $ETQID = $db->pegaUm($SQL);

        if( $ETQID == '' ){

            $sql = "
                INSERT INTO maismedicomec.entidadequestionario( qstid, muncod ) VALUES ( {$qstid}, '{$muncod}' ) RETURNING etqid;
            ";
            $etqid = $db->pegaUm($sql);

            if($etqid > 0){
                $db->commit();
                $ent_quest_id = $etqid;
            }

        }else{
            $ent_quest_id = $ETQID;
        }
        return $ent_quest_id;
    }

    /**
     * functionName dowloadDocAnexo
     *
     * @author Luciano F. Ribeiro
     *
     * @param array $dados é usado o id da pergunta.
     * @return o download do arquivo".
     *
     * @version v1
    */
    function dowloadDocAnexo( $dados ){

        $arqid = $dados['arqid'];

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        if ( $arqid ){
            $file = new FilesSimec('arquivoresposta', $campos, 'maismedicomec');
            $file->getDownloadArquivo( $arqid );
        }
    }

    /**
     * functionName dowloadDocAnexo
     *
     * @author Luciano F. Ribeiro
     *
     * @param array $dados é usado o id da pergunta.
     * @return o download do arquivo".
     *
     * @version v1
    */
    function dowloadDocAnexoMaisMedicoPAR( $dados ){

        $arqid = $dados['arqid'];

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        if ( $arqid ){
            $file = new FilesSimec('arquivosmunicipio', $campos, 'par');
            $arquivo = $file->getDownloadArquivo($arqid);
        }
    }

   /**
     * functionName excluirDocAnexo
     *
     * @author Luciano F. Ribeiro
     *
     * @param array $dados é usado o id da pergunta.
     * @return exclusão logica e fisica do arquivo.
     *
     * @version v1
    */
    function excluirDocAnexo( $dados ) {
        global $db;

        $arqid = $dados['arqid'];
        $prgid = $dados['prgid'];

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        if ($arqid != '') {
            $sql = " UPDATE maismedicomec.arquivoresposta SET arqstatus = 'I' WHERE arqid = {$arqid} ";
        }

        if( $db->executar($sql) ){
            $file = new FilesSimec('arquivoresposta', $campos, 'maismedicomec');
            $file->excluiArquivoFisico( $arqid );

            $db->commit();
            $db->sucesso('principal/instrumentoavaliacao/lista_grid_arquivos_anexo', '&acao=A&prgid='.$prgid);
        }
    }

   /**
     * functionName excluirDocAnexo
     *
     * @author Luciano F. Ribeiro
     *
     * @param array $dados é usado o id da pergunta.
     * @return exclusão logica e fisica do arquivo.
     *
     * @version v1
    */
    function excluirDocAnexoAvaliacao( $dados ) {
        global $db;

        $arqid = $dados['arqid'];

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        if ($arqid != '') {
            $sql = " UPDATE maismedicomec.arquivoavaliacao SET avastatus = 'I' WHERE arqid = {$arqid} ";
        }

        if( $db->executar($sql) ){
            $file = new FilesSimec('arquivoavaliacao', $campos, 'maismedicomec');
            $file->excluiArquivoFisico( $arqid );

            $db->commit();
            $db->sucesso('principal/instrumentoavaliacao/cad_documentos_aval', '&acao=A');
        }
    }

    /**
     * functionName excluirUnidadeSaude
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $ussid é id da unidade de saude.
     * @return string não há retorno. Só atribuição dos valores a sessão.
     *
     * @version v1
    */
    function excluirUnidadeSaude($dados){
        global $db;

        $ussid = $dados['ussid'];
        $tusid = $dados['tusid'];

        $sql = "
            UPDATE maismedicomec.unidservicosaude
                SET ussstatus = 'I'
            WHERE ussid = {$ussid} RETURNING ussid;
        ";
        $ussid = $db->pegaUm($sql);

        if( $ussid > 0 ){
            $db->commit();
            echo montaListaGridUnidadeSaude( $tusid );
        }
        die;
    }


    /**
     * functionName formataDataBanco
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $valor data a ser formatada no padrão USA
     * @return date data formatada no padrão americano.
     *
     * @version v1
    */
    function formataDataBanco($valor) {
        $data = explode("/", $valor);

        $dia = $data[0];
        $mes = $data[1];
        $ano = $data[2];

        return $ano . "-" . $mes . "-" . $dia;
    }

    /**
     * functionName habilitaPerfilEstadoAcao
     *
     * @author Luciano F. Ribeiro
     *
     * @return string de retorna 'S' sim ou 'N' não de acordo com a permissão verificando regras do perfil e estado do workflow.
     *
     * @version v1
    */
    function habilitaPerfilEstadoAcao(){

        $perfil = pegaPerfilGeral();

        if(in_array(PERFIL_CONSULTA, $perfil)){
            return 'N';
        }

        $etqid  = $_SESSION['maismedicomec']['etqid'];

        $docid  = buscarDocidAvaliacaoMM( $etqid );
        $estado = pegaEstadoAtualWorkflow( $docid );

        if( !( in_array(PERFIL_MM_MEC_SUPER_USUARIO, $perfil) || in_array(PERFIL_MM_MEC_ADMINISTRADOR, $perfil) ) ){

            if( !( in_array(PERFIL_INST_AVAL_CONSULTA, $perfil) ) ){

                if( $estado ==  '' || $estado == WF_EM_PREENCHIMENTO_AVALIADOR || $estado == WF_EM_AJUSTE_AVALIADOR ){
                    if( in_array(PERFIL_INST_AVAL_ANALISTA_MEC, $perfil) ){
                        $habilitado = 'N';
                    }else{
                        $habilitado = 'S';
                    }
                }

                if( $estado == WF_EM_ANALISE_MEC || $estado == WF_EM_REANALISE_MEC ){
                    if( in_array(PERFIL_INST_AVAL_AVALIADOR_MEC, $perfil) ){
                        $habilitado = 'N';
                    }else{
                        $habilitado = 'S';
                    }
                }

                if( $estado == WF_PROCESSO_FINALIZADO ){
                    if( in_array(PERFIL_INST_AVAL_ANALISTA_MEC, $perfil) || in_array(PERFIL_INST_AVAL_AVALIADOR_MEC, $perfil) ){
                        $habilitado = 'N';
                    }else{
                        $habilitado = 'S';
                    }
                }
            }else{
                $habilitado = 'N';
            }

        }else{
            $habilitado = 'S';
        }

        return $habilitado;
    }

    /**
     * functionName iniciaVariaveisSessao
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $variavel é o request da variavel que vai ser atribuida a sessão.
     * @param string $tipo é o tipo da variavel que vai ser atribuida a sessão. Podendo ser questionario ou município.
     * @return string não há retorno. Só atribuição dos valores a sessão.
     *
     * @version v1
    */
    function iniciaVariaveisSessao( $variavel, $tipo){
        global $db;

        if( $tipo == 'M' ){
            $sql = "SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$variavel}'";
            $mundescricao = $db->pegaUm($sql);
        }

        switch ($tipo){
            case 'Q':
                $_SESSION['maismedicomec']['qstid'] = $variavel;
                break;
            case 'M':
                $_SESSION['maismedicomec']['muncod'] = $variavel;
                $_SESSION['maismedicomec']['mundescricao'] = $mundescricao;
                break;
            case 'E':
                $_SESSION['maismedicomec']['etqid'] = $variavel;
                break;
            default :
                $_SESSION['maismedicomec']['qstid'] = '';
                $_SESSION['maismedicomec']['muncod'] = '';
                $_SESSION['maismedicomec']['etqid'] = '';
        }
    }


    /**
     * functionName salvarAvaliacaoDescricaoGerais
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é o request dos dados "$_POST" do formulário.
     * @return string não há retorno. Só atribuição dos valores a sessão.
     *
     * @version v1
    */
    function salvarAvaliacaoDescricaoGerais($dados, $files=null){
        global $db;

        $etqid          = $_SESSION['maismedicomec']['etqid'];
        $qstid          = $_SESSION['maismedicomec']['qstid'];
        $usucpfresposta = $_SESSION['usucpf'];

        $arryModulo = explode("/", $dados['modulo']);

        $url = $arryModulo[2];

        foreach ($dados['pergunta'] as $i => $resposta){
            $prgid         = $i;
            $rptresposta   = trim( addslashes($resposta) );

            if( $dados['status'][$i] == 'U'){
                $sql = "
                    UPDATE maismedicomec.resposta
                        SET rptresposta = '{$rptresposta}'
                        WHERE qstid = {$qstid} AND etqid = {$etqid} and prgid = {$prgid} RETURNING rptid;
                ";
                $rptid = $db->pegaUm($sql);
            }else{
                $sql = "
                    INSERT INTO maismedicomec.resposta(
                            etqid, qstid, usucpfresposta, prgid, rptresposta
                    ) VALUES (
                            {$etqid}, {$qstid}, '{$usucpfresposta}', {$prgid}, '{$rptresposta}'
                    ) RETURNING rptid;
                ";
                $rptid = $db->pegaUm($sql);
            }

            if( $files['pergunta_'.$i]['name'] ){
                $existe = 'S';
                $result = anexarDocumentos( $files, $rptid, $i );
            }
        }

        if( $rptid > 0 ){
            $docid = criaDocidAvaliacaoMM( $etqid );

            if( $docid > 0 ){
                $db->commit();

                if($existe == 'S'){
                    if( $result == 'S' ){
                        $db->sucesso("principal/instrumentoavaliacao/{$url}", "", "Arquivos e Dados salvo com sucesso!");
                    }else{
                        $db->sucesso("principal/instrumentoavaliacao/{$url}", "", "Dados salvo com sucesso! Ocorreu problemas com os Arquivos.");
                    }
                }else{
                    $db->sucesso("principal/instrumentoavaliacao/{$url}", "", "INSCRIÇÃO executada com sucesso!");
                }

            }else{
                $db->sucesso("principal/instrumentoavaliacao/{$url}", "", "INSCRIÇÃO não realizada, Tente novamnete mais tarde!");
            }
        }else{
            $db->sucesso("principal/instrumentoavaliacao/{$url}", "", "INSCRIÇÃO não realizada, Tente novamnete mais tarde!");
        }
    }

    /**
     * functionName salvarQuestUnidade
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é o request dos dados "$_POST" do formulário.
     * @return string não há retorno. Só atribuição dos valores a sessão.
     *
     * @version v1
    */
    function salvarQuestUnidadeSaude($dados){
        global $db;

        $pagina     = $dados['pagina'];
        $continuar  = $dados['continuar'];

        $ussid = $dados['ussid'];

        $etqid          = $_SESSION['maismedicomec']['etqid'];
        $qstid          = $_SESSION['maismedicomec']['qstid'];
        $usucpfresposta = $_SESSION['usucpf'];

        $arryModulo = explode("/", $dados['modulo']);

        $url = $arryModulo[2];

        foreach ($dados['pergunta'] as $i => $resposta){
            $prgid          = $i;
            $rusresposta   = trim( addslashes($resposta) );

            if( $dados['status'][$i] == 'U'){
                $sql = "
                    UPDATE maismedicomec.respostaunidadeservicosaude
                        SET rusresposta = '{$rusresposta}'
                    WHERE ussid = {$ussid} AND prgid = {$prgid} RETURNING rusid;
                ";
                $rusid = $db->pegaUm($sql);
            }else{
                $sql = "
                    INSERT INTO maismedicomec.respostaunidadeservicosaude(
                            ussid, prgid, usucpfresposta, rusresposta
                        ) VALUES (
                            {$ussid}, {$prgid}, '{$usucpfresposta}', '{$rusresposta}'
                    ) RETURNING rusid;
                ";
                $rusid = $db->pegaUm($sql);
            }
        }

        if( $rusid > 0 ){
            $docid = criaDocidAvaliacaoMM( $etqid );

            if( $docid > 0 ){
                $db->commit();
                $db->sucesso("principal/instrumentoavaliacao/{$url}", "", "INSCRIÇÃO executada com sucesso!");
            }else{
                $db->sucesso("principal/instrumentoavaliacao/{$url}", "", "INSCRIÇÃO não realizada, Tente novamnete mais tarde!");
            }
        }else{
            $db->sucesso("principal/instrumentoavaliacao/{$url}", "", "INSCRIÇÃO não realizada, Tente novamnete mais tarde!");
        }
    }


    /**
     * functionName salvarUnidadeServico
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é o request dos dados "$_POST" do formulário.
     * @return string não há retorno. Só atribuição dos valores a sessão.
     *
     * @version v1
    */
    function salvarUnidadeServico($dados){
        global $db;

        $tusid  = $dados['tusid'];
        $ussdsc = $dados['ussdsc'];
        $usscnes= $dados['usscnes'];

        $qstid  = $_SESSION['maismedicomec']['qstid'];
        $etqid  = $_SESSION['maismedicomec']['etqid'];
        $usucpf = $_SESSION['usucpf'];

        switch ( $tusid ){
            case 1:
                $url = 'perg_ss_hospital';
                break;
            case 2:
                $url = 'perg_ss_unidade_saude';
                break;
            case 3:
                $url = 'perg_ss_upa';
                break;
            case 4:
                $url = 'perg_ss_caps';
                break;
            case 5:
                $url = 'perg_ss_ambulatorio';
                break;
            default :
                $url = 'perg_ss_hospital';
        }

        $sql = "
            INSERT INTO maismedicomec.unidservicosaude(
                    etqid, qstid, tusid, usucpf, ussdsc, usscnes
                ) VALUES (
                    {$etqid}, {$qstid}, {$tusid}, '{$usucpf}', '{$ussdsc}', '{$usscnes}'
            ) RETURNING ussid;
        ";
        $ussid = $db->pegaUm($sql);

        if( $ussid > 0 ){
            $db->commit();
            $db->sucesso("principal/instrumentoavaliacao/{$url}", "&acao=A", "INSCRIÇÃO executada com sucesso!", "S", "S");
        }else{
            $db->sucesso("principal/instrumentoavaliacao/{$url}", "&acao=A", "INSCRIÇÃO não realizada, Tente novamente mais tarde!", "S", "S");
        }
    }


    /**
     * functionName montaListaGridUnidadeSaude
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $tusid é o tipo de unidade de saúde.
     * @return string retorna o Grid com a listagem das unidades de saúde referentes ao questionario.
     *
     * @version v1
    */
    function montaListaGridUnidadeSaude( $tusid ){
        global $db;

        $etqid = $_SESSION['maismedicomec']['etqid'];
        $qstid = $_SESSION['maismedicomec']['qstid'];

        $acao = "
            <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"buscarDadosUnidadeSaude('||ussid||');\" title=\"Selecionar Unidade de Saúde\" >
            <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirUnidadeSaude('||ussid||');\" title=\"Excluir Unidade de Saúde\" >
        ";

        $sql = "
            SELECT  '{$acao}',
                    ussid,
                    tusdsc,
                    ussdsc AS descricao,
                    usscnes
            FROM maismedicomec.unidservicosaude AS u
            LEFT JOIN maismedicomec.tipounidadeservico AS t ON t.tusid = u.tusid

            WHERE u.ussstatus = 'A' AND u.tusid = {$tusid} AND u.etqid = {$etqid} AND u.qstid = {$qstid}

            ORDER BY ussdsc
        ";
        $cabecalho = array("Ação", "Código", "Tipo de Unidade Saúde", "Nome", "CNES nº");
        $alinhamento = Array('center', '', '', '');
        $tamanho = Array('5%', '10%', '', '');
        $db->monta_lista($sql, $cabecalho, 100, 10, 'N', 'left', 'N', 'N', $tamanho, $alinhamento);

    }


    /**
     * functionName verificaExisteArquivo
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $prgid é o id da pergunta.
     * @return string retorna se a pergunta em questão tem arquivos anexados a ela.
     *
     * @version v1
    */
    function validaDataInicialMaior( $dados ){

        $dataIni = strtotime( formataDataBanco( $dados['dataIni'] ) );
        $dataFim = strtotime( formataDataBanco( $dados['dataFim'] ) );

        if( $dataIni > $dataFim ){
            $retorno = 'erro';
        }else{
            $retorno = 'ok';
        }
        echo $retorno;

        die();
    }

    /**
     * functionName verificaExisteArquivo
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $prgid é o id da pergunta.
     * @return string retorna se a pergunta em questão tem arquivos anexados a ela.
     *
     * @version v1
    */
    function verificaExisteArquivo( $prgid ){
        global $db;

        $qstid = $_SESSION['maismedicomec']['qstid'];
        $etqid = $_SESSION['maismedicomec']['etqid'];

        $sql = "
            SELECT  --r.rptid
                    count(aqrrespid) AS total_arquivo
            FROM maismedicomec.arquivoresposta anx

            LEFT JOIN maismedicomec.tipoarquivo t on t.tpaid = anx.tpaid

            JOIN maismedicomec.resposta r ON r.rptid = anx.rptid
            JOIN public.arquivo arq on arq.arqid = anx.arqid

            WHERE anx.arqstatus = 'A' AND etqid = {$etqid} AND qstid = {$qstid} AND r.prgid = {$prgid}
        ";
        return $result = $db->pegaUm($sql);
    }

    /**
     * functionName verificaExisteSessaoMM
     *
     * @author Luciano F. Ribeiro
     *
     * @return string faz redirecionamento caso não exista sessão.
     *
     * @version v1
    */
    function verificaExisteSessaoMM(){
        global $db;

        if( $_SESSION['maismedicomec']['qstid'] == '' || $_SESSION['maismedicomec']['muncod'] == '' || $_SESSION['maismedicomec']['etqid'] == '' ){
            $db->sucesso('principal/inicio_direcionamento', '&acao=A', 'Não foi possivél acessar o sistema, Ocorreu um problema interno ou houve perca de sessão. Tente novamente mais tarde!');
        }
    }



    #---------------------------------------------------- FUNÇÕES (CRIAÇÃO, VERIFICAÇÃO E TRAMITAÇÃO  DAS AÇOES DO WORKFLOW) --------------------------------------------#

    /**
     * functionName buscarAvaliacaoMM
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer $etqid id da entidade avaliada.
     * @return integer retorna o docid da entidade caso exista.
     *
     * @version v1
    */
    function buscarDocidAvaliacaoMM( $etqid ){
        global $db;

        if( $etqid != ''){
            $sql = "
                SELECT  etqid,
                        docid
                FROM maismedicomec.entidadequestionario
                WHERE etqid = {$etqid}
            ";
            $dados = $db->pegaLinha($sql);

            return $dados['docid'];

        }else{
            return false;
        }
    }

    /**
     * functionName criaDocidAvaliacaoMM
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer $etqid id da entidade avaliada.
     * @return boolean retorna true caso o docid seja criado.
     *
     * @version v1
    */
    function criaDocidAvaliacaoMM( $etqid ){
        global $db;

        require_once APPRAIZ ."includes/workflow.php";

        $usucpf = $_SESSION['usucpf'];

        $existeDocid = buscarDocidAvaliacaoMM( $etqid );

        if($existeDocid == ''){
            $tpdid = WF_FLUXO_AVALIACAO_IN_LOCO_MM;

            $docid = wf_cadastrarDocumento($tpdid, 'Mais Médico - Mec - Avaliação Inloco');

            if($etqid != '' && $docid != ''){
                $sql = "
                    UPDATE maismedicomec.entidadequestionario SET docid = {$docid} WHERE etqid = {$etqid} RETURNING etqid;
                ";
                $etqid = $db->pegaUm($sql);

                if( $etqid > 0 ){
                    $db->commit();
                    return $docid;
                }else{
                    return false;
                }
            }

        }else{
            return $existeDocid;
        }
    }

	function buscarDocidMantenedora( $mntid ){
        global $db;

        if( $mntid != ''){
            $sql = "
                SELECT  mntid,
                        docid
                FROM maismedicomec.mantenedora
                WHERE mntid = {$mntid}
            ";
            $dados = $db->pegaLinha($sql);

            return $dados['docid'];

        }else{
            return false;
        }
    }

	function criaDocidMantenedora( $mntid ){
        global $db;

        require_once APPRAIZ ."includes/workflow.php";

        $usucpf = $_SESSION['usucpf'];

        $existeDocid = buscarDocidMantenedora( $mntid );

        if($existeDocid == ''){
            $tpdid = WF_FLUXO_MANTENEDORA;

            $docid = wf_cadastrarDocumento($tpdid, 'Mais Médico - Mec - Mantenedora');

            if($mntid != '' && $docid != ''){
                $sql = "
                    UPDATE maismedicomec.mantenedora SET docid = {$docid} WHERE mntid = {$mntid} RETURNING mntid;
                ";
                $mntid = $db->pegaUm($sql);

                if( $mntid > 0 ){
                    $db->commit();
                    return $docid;
                }else{
                    return false;
                }
            }

        }else{
            return $existeDocid;
        }
    }



    /**
     * functionName pegaEstadoAtualWorkflow
     *
     * @author Luciano F. Ribeiro
     *
     * @param integer $docid id do documento gerado no workflow.
     * @return integer retorna id do estado que se encontra o documento.
     *
     * @version v1
    */
    function pegaEstadoAtualWorkflow( $docid ){
        global $db;

        if($docid) {
            $docid = (integer) $docid;
            $sql = "
                SELECT  ed.esdid
                FROM workflow.documento d
                JOIN workflow.estadodocumento AS ed ON ed.esdid = d.esdid
                WHERE d.docid = $docid
            ";
            $estado = $db->pegaUm($sql);
            return $estado;
        } else {
            return false;
        }
    }


    #------------------------------------------------------------- FUNÇÕES (MOTOR PARA A ESCREVER OS COMPONENTES HTML) --------------------------------------------------#

    /**
     * functionName montaPerguntasQuestionario
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $numeracao É o numeração ou marcador da pergunta.
     * @param string $tituloAgrupador É o titulo do grupo de perguntas, uma pergunta usada para descrever as demais perguntas ou as opções.
     * @param string $subTitulo É o titulo usado para descrever o label do campo.
     * @param array $arrPerguntas É o id das perguntas no banco.
     *
     * @return string  retorna o html dos componentes.
     *
     * @version v1
    */
    function montaPerguntasQuestionario( $numeracao = '', $tituloAgrupador = '', $subTitulo = '', $arrPerguntas, $b_funcao_onclick = NULL, $tr_grupo_perg = NULL ){
        global $db;

        $habilita = habilitaPerfilEstadoAcao();

        if( $habilita == 'S' ){
            $disabled = '';
        }else{
            $disabled = 'disabled="disabled"';
        }

        if( is_array($arrPerguntas) ){
            $where = implode(',', $arrPerguntas);
        }

        $sql = "
            SELECT  prgid,
                    qstid,
                    prgdsc,
                    prgtipo,
                    prgtamanho,
                    prgobrigatoriedade,
                    prgmascara,
                    prgiditem,
                    prgadicionaarq

            FROM maismedicomec.pergunta

            WHERE prgstatus = 'A' AND prgid IN ({$where})
            ORDER BY 1;
        ";
        $dados = $db->carregar($sql);

        foreach ($dados as $i => $pergunta){
            switch ( trim($pergunta['prgtipo']) ){
                case "B":#TIPO BOLENO
                    $resposta = buscaRespostaPergunta( $pergunta['prgid'] );

                    if($resposta != ''){
                        $value = 'U';

                        if(trim($resposta) == 'S'){
                            $checked_S = 'checked="checked"';
                            $checked_N = '';
                        }else{
                            $checked_S = '';
                            $checked_N = 'checked="checked"';
                        }
                    }else{
                        $value = 'I';
                    }

                    if($b_funcao_onclick != ''){
                        $funcao = $b_funcao_onclick."(this);";
                    }else{
                        $funcao = "";
                    }

                    if( $pergunta['prgobrigatoriedade'] == 't' ){
                        $obrigatorio = "<img border=\"0\" style=\"margin:0px 0px 0px 5px;\" title=\"Indica campo obrigatório.\" src=\"../imagens/obrig.gif\">";
                        $class = "class=\"obrigatorio\"";
                    }else{
                        $obrigatorio = "";
                        $class = "";
                    }

                    if($tituloAgrupador != '' && $i == 0){
                        $html .= "<tr>";
                        $html .= "<td colspan=\"2\" style=\"font-weight: bold;\">";

                        if($tr_grupo_perg != ''){
                            $html .= "<img border=\"0\" id=\"sinal_mais_{$tr_grupo_perg}\" name=\"{$tr_grupo_perg}\" style=\"cursor: pointer;\" src=\"../imagens/mais.gif\" onclick=\"abrirGrupoPergunta(this, 'mais');\">";
                            $html .= "<img border=\"0\" id=\"sinal_menos_{$tr_grupo_perg}\" name=\"{$tr_grupo_perg}\" style=\"cursor: pointer; display:none;\" src=\"../imagens/menos.gif\" onclick=\"abrirGrupoPergunta(this, 'menos');\">";
                        }

                        $html .= "&nbsp; &nbsp;".$numeracao;
                        $html .= " ".$tituloAgrupador;
                        $html .= "</td>";
                        $html .= "</tr>";
                    }

                    if($pergunta['prgdsc'] != ''){

                        if($tr_grupo_perg != ''){
                            $html .= "<tr class=\"{$tr_grupo_perg}\" style=\"display:none;\">";
                        }else{
                            $html .= "<tr>";
                        }
                        $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$pergunta['prgdsc']}: </td>";

                        $html .= "<td>";

                        $html .= "<input type=\"radio\" {$class} name=\"pergunta[{$pergunta['prgid']}]\" id=\"pergunta_S_{$pergunta['prgid']}\" value=\"S\" onclick=\"{$funcao}\" title=\"{$pergunta['prgdsc']}\" {$checked_S} {$disabled} style=\"margin:0px 0px 0px 5px;\"> Sim";
                        $html .= "<input type=\"radio\" {$class} name=\"pergunta[{$pergunta['prgid']}]\" id=\"pergunta_N_{$pergunta['prgid']}\" value=\"N\" onclick=\"{$funcao}\" title=\"{$pergunta['prgdsc']}\" {$checked_N} {$disabled} style=\"margin:0px 0px 0px 10px;\"> Não";
                        $html .= "<input type=\"hidden\" name=\"status[{$pergunta['prgid']}]\" id=\"status_{$pergunta['prgid']}\" value=\"{$value}\">";

                        $html .= $obrigatorio;

                        $html .= "</td>";
                        $html .= "</tr>";
                    }

                    if($pergunta['prgadicionaarq'] == 't'){

                        if($tr_grupo_perg != ''){
                            $html .= "<tr class=\"{$tr_grupo_perg}\" style=\"display:none;\">";
                        }else{
                            $html .= "<tr>";
                        }

                        $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> Anexar </td>";

                        $html .= "<td>";

                        $html .= "<input type=\"file\" name=\"pergunta_{$pergunta['prgid']}\" id=\"arquivo_{$pergunta['prgid']}\" title=\"Anexar arquivo referente a pergunta acima\" style=\"margin:0px 0px 0px 10px;\">";
                        $html .= "<input type=\"hidden\" name=\"status[{$pergunta['prgid']}]\" id=\"status_{$pergunta['prgid']}\" value=\"{$value}\">";

                        $html .= "</td>";
                        $html .= "</tr>";

                        $existe = verificaExisteArquivo( $pergunta['prgid'] );
                        if( $existe > 0 ){
                            if($tr_grupo_perg != ''){
                                $html .= "<tr class=\"{$tr_grupo_perg}\" style=\"display:none;\">";
                            }else{
                                $html .= "<tr>";
                            }
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> Lista de Arquivos: </td>";
                            $html .= "<td> <input type=\"button\" id=\"abrirPagina\" name=\"abrirPagina\" value=\"Ver Lista de Arquivos\" onclick=\"abrirPaginaVerArquivos('{$pergunta['prgid']}');\" style=\"margin:0px 0px 0px 10px;\"> <b>Existe(m) {$existe} arquivo(s) Anexado(s).</b> </td>";
                            $html .= "</tr>";
                        }
                    }
                    break;

                case 'D':#TIPO DATA
                    $resposta = buscaRespostaPergunta( $pergunta['prgid'] );

                    if($resposta != ''){
                        $value = 'U';
                    }else{
                        $value = 'I';
                    }

                    if( $pergunta['prgobrigatoriedade'] == 't' ){
                        $obrigatorio = 'S';
                    }else{
                        $obrigatorio = 'N';
                    }

                    if($tituloAgrupador != '' && $i == 0){
                        $html .= "<tr>";
                        $html .= "<td colspan=\"2\" style=\"font-weight: bold;\">";

                        if($tr_grupo_perg != ''){
                            $html .= "<img border=\"0\" id=\"sinal_mais_{$tr_grupo_perg}\" name=\"{$tr_grupo_perg}\" style=\"cursor: pointer;\" src=\"../imagens/mais.gif\" onclick=\"abrirGrupoPergunta(this, 'mais');\">";
                            $html .= "<img border=\"0\" id=\"sinal_menos_{$tr_grupo_perg}\" name=\"{$tr_grupo_perg}\" style=\"cursor: pointer; display:none;\" src=\"../imagens/menos.gif\" onclick=\"abrirGrupoPergunta(this, 'menos');\">";
                        }

                        $html .= "&nbsp; &nbsp;".$numeracao;
                        $html .= " ".$tituloAgrupador;
                        $html .= "</td>";
                        $html .= "</tr>";
                    }

                    if($pergunta['prgdsc'] != ''){

                        if($tr_grupo_perg != ''){
                            $html .= "<tr class=\"{$tr_grupo_perg}\" style=\"display:none;\">";
                        }else{
                            $html .= "<tr>";
                        }

                        if( str_to_upper(trim($tituloAgrupador)) == str_to_upper(trim($pergunta['prgdsc'])) ){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> Digite a data da visita: </td>";
                        }else{
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$pergunta['prgdsc']}: </td>";
                        }
                        $html .= "<td>";
                        $html .= campo_data2("pergunta[{$pergunta['prgid']}]", $obrigatorio, $habilita, $pergunta['prgdsc'], $pergunta['prgmascara'],'','', $resposta, '', '', 'pergunta_'.$pergunta['prgid'] );
                        $html .= "<input type=\"hidden\" name=\"status[{$pergunta['prgid']}]\" id=\"status_{$pergunta['prgid']}\" value=\"{$value}\">";
                        $html .= "</td>";
                        $html .= "</tr>";
                    }
                    break;

                case 'C':#TIPO TEXTO CURTO
                    $resposta = stripslashes( buscaRespostaPergunta( $pergunta['prgid'] ) );

                    if($resposta != ''){
                        $value = 'U';
                    }else{
                        $value = 'I';
                    }

                    if( $pergunta['prgobrigatoriedade'] == 't' ){
                        $obrigatorio = 'S';
                    }else{
                        $obrigatorio = 'N';
                    }

                    if($tituloAgrupador != '' && $i == 0){
                        $html .= "<tr>";
                        $html .= "<td colspan=\"2\" style=\"font-weight: bold;\">";

                        if($tr_grupo_perg != ''){
                            $html .= "<img border=\"0\" id=\"sinal_mais_{$tr_grupo_perg}\" name=\"{$tr_grupo_perg}\" style=\"cursor: pointer;\" src=\"../imagens/mais.gif\" onclick=\"abrirGrupoPergunta(this, 'mais');\">";
                            $html .= "<img border=\"0\" id=\"sinal_menos_{$tr_grupo_perg}\" name=\"{$tr_grupo_perg}\" style=\"cursor: pointer; display:none;\" src=\"../imagens/menos.gif\" onclick=\"abrirGrupoPergunta(this, 'menos');\">";
                        }

                        $html .= "&nbsp; &nbsp;".$numeracao;
                        $html .= " ".$tituloAgrupador;
                        $html .= "</td>";
                        $html .= "</tr>";
                    }
                    $prgtamanho = trim($pergunta['prgtamanho']);

                    if($pergunta['prgdsc'] != ''){

                        if($tr_grupo_perg != ''){
                            $html .= "<tr class=\"{$tr_grupo_perg}\" style=\"display:none;\">";
                        }else{
                            $html .= "<tr>";
                        }

                        if( str_to_upper(trim($tituloAgrupador)) == str_to_upper(trim($pergunta['prgdsc'])) && $subTitulo == '' ){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> Descreva: </td>";
                        }elseif($subTitulo != ''){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$subTitulo}: </td>";
                        }else{
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$pergunta['prgdsc']}: </td>";
                        }
                        $html .= "<td>";
                        $html .= campo_textarea("pergunta[{$pergunta['prgid']}]", $obrigatorio, $habilita, '', 100, 6, $prgtamanho, '', 0, '', false, $pergunta['prgdsc'], $resposta,'50%', "pergunta_{$pergunta['prgid']}");
                        $html .= "<input type=\"hidden\" name=\"status[{$pergunta['prgid']}]\" id=\"status_{$pergunta['prgid']}\" value=\"{$value}\">";
                        $html .= "</td>";
                        $html .= "</tr>";
                    }
                    break;

                case 'T':#TIPO TEXTO LONGO.
                    $resposta = stripslashes( buscaRespostaPergunta( $pergunta['prgid'] ) );

                    if($resposta != ''){
                        $value = 'U';
                    }else{
                        $value = 'I';
                    }

                    if( $pergunta['prgobrigatoriedade'] == 't' ){
                        $obrigatorio = 'S';
                    }else{
                        $obrigatorio = 'N';
                    }

                    if($tituloAgrupador != '' && $i == 0){
                        $html .= "<tr>";
                        $html .= "<td colspan=\"2\" style=\"font-weight: bold;\">";

                        if($tr_grupo_perg != ''){
                            $html .= "<img border=\"0\" id=\"sinal_mais_{$tr_grupo_perg}\" name=\"{$tr_grupo_perg}\" style=\"cursor: pointer;\" src=\"../imagens/mais.gif\" onclick=\"abrirGrupoPergunta(this, 'mais');\">";
                            $html .= "<img border=\"0\" id=\"sinal_menos_{$tr_grupo_perg}\" name=\"{$tr_grupo_perg}\" style=\"cursor: pointer; display:none;\" src=\"../imagens/menos.gif\" onclick=\"abrirGrupoPergunta(this, 'menos');\">";
                        }

                        $html .= "&nbsp; &nbsp;".$numeracao;
                        $html .= " ".$tituloAgrupador;
                        $html .= "</td>";
                        $html .= "</tr>";
                    }

                    if($pergunta['prgdsc'] != ''){

                        if($tr_grupo_perg != ''){
                            $html .= "<tr class=\"{$tr_grupo_perg}\" style=\"display:none;\">";
                        }else{
                            $html .= "<tr>";
                        }

                        if( str_to_upper(trim($tituloAgrupador)) == str_to_upper(trim($pergunta['prgdsc'])) && $subTitulo == '' ){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> Descreva: </td>";
                        }elseif($subTitulo != ''){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$subTitulo}: </td>";
                        }
                        else{
                            $html .= " <td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$pergunta['prgdsc']}: </td>";
                        }
                        $prgtamanho = trim($pergunta['prgtamanho']);

                        $html .= "<td>";
                        $html .= campo_textarea("pergunta[{$pergunta['prgid']}]", $obrigatorio, $habilita, '', 100, 8, $prgtamanho, '', '', '', false, $pergunta['prgdsc'], $resposta, '90%', "pergunta_{$pergunta['prgid']}" );
                        $html .= "<input type=\"hidden\" name=\"status[{$pergunta['prgid']}]\" id=\"status_{$pergunta['prgid']}\" value=\"{$value}\">";
                        $html .= "</td>";
                        $html .= "</tr>";
                    }
                    break;
                case 'N':#TIPO NUMÉRICO
                    $resposta = buscaRespostaPergunta( $pergunta['prgid'] );

                    if($resposta != ''){
                        $value = 'U';
                    }else{
                        $value = 'I';
                    }

                    if( $pergunta['prgobrigatoriedade'] == 't' ){
                        $obrigatorio = 'S';
                    }else{
                        $obrigatorio = 'N';
                    }

                    if($tituloAgrupador != '' && $i == 0){
                        $html .= "<tr>";
                        $html .= "<td colspan=\"2\" style=\"font-weight: bold;\">";

                        if($tr_grupo_perg != ''){
                            $html .= "<img border=\"0\" id=\"sinal_mais_{$tr_grupo_perg}\" name=\"{$tr_grupo_perg}\" style=\"cursor: pointer;\" src=\"../imagens/mais.gif\" onclick=\"abrirGrupoPergunta(this, 'mais');\">";
                            $html .= "<img border=\"0\" id=\"sinal_menos_{$tr_grupo_perg}\" name=\"{$tr_grupo_perg}\" style=\"cursor: pointer; display:none;\" src=\"../imagens/menos.gif\" onclick=\"abrirGrupoPergunta(this, 'menos');\">";
                        }

                        $html .= "&nbsp; &nbsp;".$numeracao;
                        $html .= " ".$tituloAgrupador;
                        $html .= "</td>";
                        $html .= "</tr>";
                    }

                    if($pergunta['prgdsc'] != ''){

                        if($tr_grupo_perg != ''){
                            $html .= "<tr class=\"{$tr_grupo_perg}\" style=\"display:none;\">";
                        }else{
                            $html .= "<tr>";
                        }

                        if( str_to_upper(trim($tituloAgrupador)) == str_to_upper(trim($pergunta['prgdsc'])) && $subTitulo == '' ){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> Descreva: </td>";
                        }elseif($subTitulo != ''){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$subTitulo}: </td>";
                        }
                        else{
                            $html .= "<td width=\"25%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$pergunta['prgdsc']}: </td>";
                        }
                        $prgmascara = trim($pergunta['prgmascara']);
                        $prgtamanho = trim($pergunta['prgtamanho']);

                        $html .= "<td>";
                        $html .= campo_texto("pergunta[{$pergunta['prgid']}]", $obrigatorio, $habilita, $pergunta['prgdsc'], 10, $prgtamanho, $prgmascara, '', '', '', 0, "id=\"pergunta[{$pergunta['prgid']}]\"", '', $resposta, '', null);
                        $html .= "<input type=\"hidden\" name=\"status[{$pergunta['prgid']}]\" id=\"status_{$pergunta['prgid']}\" value=\"{$value}\">";
                        $html .= "</td>";
                        $html .= "</tr>";
                    }
                    break;
                case "M":#TIPO MÉDIA (RADIO BOX  "R"-RUIM - "M"-MÉDIO "B"-BOM E "0"-OTIMO)
                    $resposta = buscaRespostaPergunta( $pergunta['prgid'] );

                    if($resposta != ''){
                        $value = 'U';

                        if(trim($resposta) == 'R'){
                            $checked_R = 'checked="checked"';
                            $checked_M = '';
                            $checked_B = '';
                            $checked_O = '';
                        }elseif(trim($resposta) == 'M'){
                            $checked_R = '';
                            $checked_M = 'checked="checked"';
                            $checked_B = '';
                            $checked_O = '';
                        }elseif(trim($resposta) == 'B'){
                            $checked_R = '';
                            $checked_M = '';
                            $checked_B = 'checked="checked"';
                            $checked_O = '';
                        }else{
                            $checked_R = '';
                            $checked_M = '';
                            $checked_B = '';
                            $checked_O = 'checked="checked"';
                        }
                    }else{
                        $value = 'I';
                    }

                    if( $pergunta['prgobrigatoriedade'] == 't' ){
                        $obrigatorio = "<img border=\"0\" style=\"margin:0px 0px 0px 5px;\" title=\"Indica campo obrigatório.\" src=\"../imagens/obrig.gif\">";
                        $class = "class=\"obrigatorio\"";
                    }else{
                        $obrigatorio = "";
                        $class = "";
                    }

                    if($tituloAgrupador != '' && $i == 0){
                        $html .= "<tr>";
                        $html .= "<td colspan=\"2\" style=\"font-weight: bold;\">";

                        if($tr_grupo_perg != ''){
                            $html .= "<img border=\"0\" id=\"sinal_mais_{$tr_grupo_perg}\" name=\"{$tr_grupo_perg}\" style=\"cursor: pointer;\" src=\"../imagens/mais.gif\" onclick=\"abrirGrupoPergunta(this, 'mais');\">";
                            $html .= "<img border=\"0\" id=\"sinal_menos_{$tr_grupo_perg}\" name=\"{$tr_grupo_perg}\" style=\"cursor: pointer; display:none;\" src=\"../imagens/menos.gif\" onclick=\"abrirGrupoPergunta(this, 'menos');\">";
                        }

                        $html .= "&nbsp; &nbsp;".$numeracao;
                        $html .= " ".$tituloAgrupador;
                        $html .= "</td>";
                        $html .= "</tr>";
                    }

                    if($pergunta['prgdsc'] != ''){

                        if($tr_grupo_perg != ''){
                            $html .= "<tr class=\"{$tr_grupo_perg}\" style=\"display:none;\">";
                        }else{
                            $html .= "<tr>";
                        }

                        if( str_to_upper(trim($tituloAgrupador)) == str_to_upper(trim($pergunta['prgdsc'])) ){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> Selecione uma alternativa: </td>";
                        }else{
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$pergunta['prgdsc']}: </td>";
                        }
                        $html .= "<td>";

                        $html .= "<input type=\"radio\" {$class} name=\"pergunta[{$pergunta['prgid']}]\" id=\"pergunta_R_{$pergunta['prgid']}\" value=\"R\" title=\"{$pergunta['prgdsc']}\" {$checked_R} {$disabled} style=\"margin:0px 0px 0px 5px;\"> Ruim ";
                        $html .= "<input type=\"radio\" {$class} name=\"pergunta[{$pergunta['prgid']}]\" id=\"pergunta_M_{$pergunta['prgid']}\" value=\"M\" title=\"{$pergunta['prgdsc']}\" {$checked_M} {$disabled} style=\"margin:0px 0px 0px 10px;\"> Médio";
                        $html .= "<input type=\"radio\" {$class} name=\"pergunta[{$pergunta['prgid']}]\" id=\"pergunta_B_{$pergunta['prgid']}\" value=\"B\" title=\"{$pergunta['prgdsc']}\" {$checked_B} {$disabled} style=\"margin:0px 0px 0px 10px;\"> Bom";
                        $html .= "<input type=\"radio\" {$class} name=\"pergunta[{$pergunta['prgid']}]\" id=\"pergunta_O_{$pergunta['prgid']}\" value=\"O\" title=\"{$pergunta['prgdsc']}\" {$checked_O} {$disabled} style=\"margin:0px 0px 0px 10px;\"> Ótimo";
                        $html .= "<input type=\"hidden\" name=\"status[{$pergunta['prgid']}]\" id=\"status_{$pergunta['prgid']}\" value=\"{$value}\">";

                        $html .= $obrigatorio;

                        $html .= "</td>";
                    }
                    break;
            }
        }
        return $html;
    }

    /**
     * functionName montaPerguntasQuestionarioImpressao
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $numeracao É o numeração ou marcador da pergunta.
     * @param string $tituloAgrupador É o titulo do grupo de perguntas, uma pergunta usada para descrever as demais perguntas ou as opções.
     * @param string $subTitulo É o titulo usado para descrever o label do campo.
     * @param array $arrPerguntas É o id das perguntas no banco.
     *
     * @return string  retorna o html para impressão do relatório.
     *
     * @version v1
    */
    function montaPerguntasQuestionarioImpressao( $numeracao = '', $tituloAgrupador = '', $subTitulo = '', $arrPerguntas, $ussid = NULL ){
        global $db;

        $habilita = habilitaPerfilEstadoAcao();

        if( $habilita == 'S' ){
            $disabled = '';
        }else{
            $disabled = 'disabled="disabled"';
        }

        if( is_array($arrPerguntas) ){
            $where = implode(',', $arrPerguntas);
        }

        $sql = "
            SELECT  prgid,
                    qstid,
                    prgdsc,
                    prgtipo,
                    prgtamanho,
                    prgobrigatoriedade,
                    prgmascara,
                    prgiditem,
                    prgadicionaarq

            FROM maismedicomec.pergunta

            WHERE prgstatus = 'A' AND prgid IN ({$where})
            ORDER BY 1;
        ";
        $dados = $db->carregar($sql);

        foreach ($dados as $i => $pergunta){
            switch ( trim($pergunta['prgtipo']) ){
                case "B":#TIPO BOLENO
                    if( $ussid == '' ){
                        $resposta = buscaRespostaPergunta( $pergunta['prgid'] );
                    }else{
                        $resposta = buscarRespostaUnidadeSaudeParaImpressao( $ussid, $pergunta['prgid'] );
                    }
                    $resposta = $resposta == '' ? ' - ' : $resposta;

                    if(trim($resposta) == 'S'){
                        $resposta_check = "Sim";
                    }elseif(trim($resposta) == 'N'){
                        $resposta_check = 'Não';
                    }else{
                        $resposta_check = ' - ';
                    }

                    if($tituloAgrupador != '' && $i == 0){
                        $html .= "<tr>";
                        $html .= "<td colspan=\"2\" style=\"font-weight: bold;\"> {$numeracao} {$tituloAgrupador} </td>";
                        $html .= "</tr>";
                    }

                    if($pergunta['prgdsc'] != ''){
                        $html .= "<tr>";
                        $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$pergunta['prgdsc']}: </td>";
                        $html .= "<td>{$resposta_check}</td>";
                        $html .= "</tr>";
                    }
                    break;

                case 'D':#TIPO DATA
                   if( $ussid == '' ){
                        $resposta = buscaRespostaPergunta( $pergunta['prgid'] );
                    }else{
                        $resposta = buscarRespostaUnidadeSaudeParaImpressao( $ussid, $pergunta['prgid'] );
                    }
                    $resposta = $resposta == '' ? ' - ' : $resposta;

                    if($tituloAgrupador != '' && $i == 0){
                        $html .= "<tr>";
                        $html .= "<td colspan=\"2\" style=\"font-weight: bold;\"> {$numeracao} {$tituloAgrupador} </td>";
                        $html .= "</tr>";
                    }

                    if($pergunta['prgdsc'] != ''){
                        $html .= "<tr>";

                        if( str_to_upper(trim($tituloAgrupador)) == str_to_upper(trim($pergunta['prgdsc'])) ){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> Digite a data: </td>";
                        }else{
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$pergunta['prgdsc']}: </td>";
                        }
                        $html .= "<td>{$resposta}</td>";

                        $html .= "</tr>";
                    }
                    break;

                case 'C':#TIPO TEXTO CURTO
                    if( $ussid == '' ){
                        $resposta = buscaRespostaPergunta( $pergunta['prgid'] );
                    }else{
                        $resposta = buscarRespostaUnidadeSaudeParaImpressao( $ussid, $pergunta['prgid'] );
                    }
                    $resposta = $resposta == '' ? ' - ' : $resposta;

                    if($tituloAgrupador != '' && $i == 0){
                        $html .= "<tr>";
                        $html .= "<td colspan=\"2\" style=\"font-weight: bold;\"> {$numeracao} {$tituloAgrupador} </td>";
                        $html .= "</tr>";
                    }

                    if($pergunta['prgdsc'] != ''){
                        $html .= "<tr>";

                        if( str_to_upper(trim($tituloAgrupador)) == str_to_upper(trim($pergunta['prgdsc'])) && $subTitulo == '' ){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> Descreva: </td>";
                        }elseif($subTitulo != ''){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$subTitulo}: </td>";
                        }else{
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$pergunta['prgdsc']}: </td>";
                        }
                        $html .= "<td> {$resposta} </td>";

                        $html .= "</tr>";
                    }
                    break;

                case 'T':#TIPO TEXTO LONGO.
                    if( $ussid == '' ){
                        $resposta = buscaRespostaPergunta( $pergunta['prgid'] );
                    }else{
                        $resposta = buscarRespostaUnidadeSaudeParaImpressao( $ussid, $pergunta['prgid'] );
                    }
                    $resposta = $resposta == '' ? ' - ' : $resposta;

                    if($tituloAgrupador != '' && $i == 0){
                        $html .= "<tr>";
                        $html .= "<td colspan=\"2\" style=\"font-weight: bold;\"> {$numeracao} {$tituloAgrupador} </td>";
                        $html .= "</tr>";
                    }

                    if($pergunta['prgdsc'] != ''){
                        $html .= "<tr>";

                        if( str_to_upper(trim($tituloAgrupador)) == str_to_upper(trim($pergunta['prgdsc'])) && $subTitulo == '' ){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> Descreva: </td>";
                        }elseif($subTitulo != ''){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$subTitulo}: </td>";
                        }else{
                            $html .= " <td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$pergunta['prgdsc']}: </td>";
                        }
                        $html .= "<td> {$resposta} </td>";

                        $html .= "</tr>";
                    }
                    break;
                case 'N':#TIPO NUMÉRICO
                    if( $ussid == '' ){
                        $resposta = buscaRespostaPergunta( $pergunta['prgid'] );
                    }else{
                        $resposta = buscarRespostaUnidadeSaudeParaImpressao( $ussid, $pergunta['prgid'] );
                    }
                    $resposta = $resposta == '' ? ' - ' : $resposta;

                    if($tituloAgrupador != '' && $i == 0){
                        $html .= "<tr>";
                        $html .= "<td colspan=\"2\" style=\"font-weight: bold;\"> {$numeracao} {$tituloAgrupador} </td>";
                        $html .= "</tr>";
                    }

                    if($pergunta['prgdsc'] != ''){
                        $html .= "<tr>";

                        if( str_to_upper(trim($tituloAgrupador)) == str_to_upper(trim($pergunta['prgdsc'])) && $subTitulo == '' ){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> Descreva: </td>";
                        }elseif($subTitulo != ''){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$subTitulo}: </td>";
                        }else{
                            $html .= "<td width=\"25%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$pergunta['prgdsc']}: </td>";
                        }
                        $html .= "<td> {$resposta} </td>";

                        $html .= "</tr>";
                    }
                    break;
                case "M":#TIPO MÉDIA (RADIO BOX  "R"-RUIM - "M"-MÉDIO "B"-BOM E "0"-OTIMO)
                    if( $ussid == '' ){
                        $resposta = buscaRespostaPergunta( $pergunta['prgid'] );
                    }else{
                        $resposta = buscarRespostaUnidadeSaudeParaImpressao( $ussid, $pergunta['prgid'] );
                    }
                    $resposta = $resposta == '' ? ' - ' : $resposta;

                    if(trim($resposta) == 'R'){
                        $resposta_media = 'Regular';
                    }elseif(trim($resposta) == 'M'){
                        $resposta_media = 'Médio';
                    }elseif(trim($resposta) == 'B'){
                        $resposta_media = 'Bom';
                    }else{
                        $resposta_media = 'Otimo';
                    }

                    if($tituloAgrupador != '' && $i == 0){
                        $html .= "<tr>";
                        $html .= "<td colspan=\"2\" style=\"font-weight: bold;\"> {$numeracao} {$tituloAgrupador} </td>";
                        $html .= "</tr>";
                    }

                    if($pergunta['prgdsc'] != ''){
                        $html .= "<tr>";

                        if( str_to_upper(trim($tituloAgrupador)) == str_to_upper(trim($pergunta['prgdsc'])) ){
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> Selecione uma alternativa: </td>";
                        }else{
                            $html .= "<td width=\"35%\" class =\"SubTituloDireita\" style=\"font-weight: normal;\"> {$pergunta['prgdsc']}: </td>";
                        }
                        $html .= "<td> {$resposta_media} </tr>";

                        $html .= "</tr>";
                    }
                    break;
            }
        }
        return $html;
    }


    #------------------------------------------------------------- FUNÇÕES DO MODULO DE CADASTRO DA MANTENEDORA --------------------------------------------------#

    /**
     * functionName buscarDadosCorpoDirigente
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $cpdid é o id da tabela de corpo dirigente.
     * @return string dados referente a mantenedora, preenche o formulario.
     *
     * @version v1
    */
    function buscarDadosCorpoDirigente( $cpdid ){
        global $db;

        $sql = "
            SELECT  c.cpdid,
                    trim( replace( to_char( cast(c.cpdcpf as bigint), '000:000:000-00' ), ':', '.' ) ) AS cpdcpf,
                    c.cpdnome,
                    c.cpdtelefonecomercial,
                    c.cpdtelcelular,
                    c.cpdemail,
                    c.cpdcargo,
                    dmt.dmtid,
                    dmt.mntid,
                    dmd.dmdid,
                    dmd.mtdid

            FROM maismedicomec.corpodirigente AS c

            LEFT JOIN maismedicomec.dirigentemantenedora AS dmt ON dmt.cpdid = c.cpdid
            LEFT JOIN maismedicomec.dirigentemantida AS dmd ON dmd.cpdid = c.cpdid

            WHERE c.cpdid = {$cpdid}
        ";
        return $dados = $db->pegaLinha($sql);
    }

    /**
     * functionName buscarDadosPessoaCorpoDirigente
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $cpdcpf é o CPF do dirigente "já cadastrado" corpo dirigente.
     * @return string dados referente ao dirigente.
     *
     * @version v1
    */
    function buscarDadosDirigente( $dados ){
        global $db;

        $cpdcpf = "'".str_replace( '.', '', str_replace( '-', '', $dados['cpdcpf'] ) )."'";

        $sql = "
            SELECT * FROM maismedicomec.corpodirigente WHERE cpdcpf = {$cpdcpf}
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            foreach($dados as $key => $value){
                $dados[$key] = iconv("ISO-8859-1", "UTF-8", trim($value) );
            }
        }else{
            $dados = "V";
        }
        echo simec_json_encode( $dados );
        die;
    }

    function buscarDadosEditarCorpoDirigente( $dados ){
        global $db;

        $cpdid = $dados['cpdid'];

        $sql = "
            SELECT  c.cpdid,
                    trim( replace( to_char( cast(c.cpdcpf as bigint), '000:000:000-00' ), ':', '.' ) ) AS cpdcpf,
                    c.cpdnome,
                    c.cpdtelefonecomercial,
                    c.cpdtelcelular,
                    c.cpdemail,
                    c.cpdcargo,
                    dmt.dmtid,
                    dmt.mntid,
                    dmd.dmdid,
                    dmd.mtdid
            FROM maismedicomec.corpodirigente AS c

            LEFT JOIN maismedicomec.dirigentemantenedora AS dmt ON dmt.cpdid = c.cpdid
            LEFT JOIN maismedicomec.dirigentemantida AS dmd ON dmd.cpdid = c.cpdid

            WHERE c.cpdid = {$cpdid}
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            foreach($dados as $key => $value){
                $dados[$key] = iconv("ISO-8859-1", "UTF-8", trim($value) );
            }
        }else{
            $dados = "";
        }
        echo simec_json_encode( $dados );
        die;
    }

    /**
     * functionName buscarDadosRepresentanteLegal
     *
     * @author Luciano F. Ribeiro
     *
     * @param string rplcpf é o CPF do dirigente "já cadastrado" corpo dirigente.
     * @return string dados referente ao dirigente da Mantenedora.
     *
     * @version v1
    */
    function buscarDadosRepresentanteLegal( $dados ){
        global $db;

        $rplcpf = "'".str_replace( '.', '', str_replace( '-', '', $dados['rplcpf'] ) )."'";

        $sql = "
            SELECT  *,
                    estuf AS rplestuf
            FROM maismedicomec.representantelegal WHERE rplcpf = {$rplcpf}
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            foreach($dados as $key => $value){
                $dados[$key] = iconv("ISO-8859-1", "UTF-8", trim($value) );
            }
        }else{
            $dados = '';
        }
        echo simec_json_encode( $dados );
        die;
    }

    /**
     * functionName buscarDadosMantenedora
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $mntid é o id da tabela de mantenedores.
     * @return string dados referente a mantenedora, preenche o formulario.
     *
     * @version v1
    */
    function buscarDadosMantenedora( $mntid ){
        global $db;

        $sql = "
            SELECT  *,
                    mm.mntid AS mm_mntid,
                    m.mntid AS mntid,
                    trim( replace( to_char( cast(mm.mntcep as bigint), '00000-000' ), ':', '.' ) ) AS mntcep,
                    mm.estuf as mntestuf,
                    mm.muncod as mntmuncod,
                    r.estuf as rplestuf,
                    r.muncod as rplmuncod,
                    trim( replace( to_char( cast(m.mntcnpj as bigint), '00:000:000/0000-00' ), ':', '.' ) ) AS mntcnpj,
                    trim( replace( to_char( cast(r.rplcpf as bigint), '000:000:000-00' ), ':', '.' ) ) AS rplcpf,
                    case when mm.mntrazaosocial is not null then
							mm.mntrazaosocial
				     	else
							m.mntdsc
					end as mntrazaosocial
            FROM gestaodocumentos.mantenedoras m
            left JOIN maismedicomec.mantenedora AS mm on mm.mntid = m.mntid
            left JOIN maismedicomec.representantelegal AS r ON r.rplid = mm.rplidreplegal
            WHERE m.mntid = {$mntid}
        ";
        //dbg($sql,1);
        return $dados = $db->pegaLinha($sql);
    }


    /**
     * functionName buscarDadosMantenedora
     *
     * @author Marcus Rocha
     *
     *
     * @version v1
    */
    function buscarDadosHospital( $hptid ){
        global $db;

        $sql = "
            SELECT  *,

                    hspid AS hspid,
                    trim( replace( to_char( cast(m.hspcep as bigint), '00000-000' ), ':', '.' ) ) AS hspcep,
                    m.hspcnes as hspcnes,
                    m.estuf as hspestuf,
                    m.muncod as hspmuncod,
                    r.estuf as rplestuf,
                    r.muncod as rplmuncod,
                    trim( replace( to_char( cast(m.hspcnpj as bigint), '00:000:000/0000-00' ), ':', '.' ) ) AS hspcnpj,
                    trim( replace( to_char( cast(r.rplcpf as bigint), '000:000:000-00' ), ':', '.' ) ) AS rplcpf
            FROM maismedicomec.hospital AS m
            JOIN maismedicomec.representantelegal AS r ON r.rplid = m.rplid
            WHERE hspid = {$hptid}
        ";
        return $dados = $db->pegaLinha($sql);
    }

    /**
     * functionName buscarDadosMantenedora
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $mntid é o id da tabela de mantenedores.
     * @return string dados referente a mantenedora, preenche o formulario.
     *
     * @version v1
    */
    function buscarDadosMantida( $mnmid ){
        global $db;

        $sql = "
            SELECT  *,
                    m.mtdid AS mm_mtdid,
                    trim( replace( to_char( cast(m.mtdcep as bigint), '00000-000' ), ':', '.' ) ) AS mtdcep,
                    mt.muncod AS mantida_muncod,
                    m.muncod as muncod
            FROM maismedicomec.mantida AS m
            JOIN maismedicomec.mantidamunicipio AS mt ON mt.mtdid = m.mtdid
            WHERE mt.mnmid = {$mnmid}
        ";
        return $dados = $db->pegaLinha($sql);

    }


     /**
     * functionName buscarDadosMantenedora
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $mntid é o id da tabela de mantenedores.
     * @return string dados referente a mantenedora, preenche o formulario.
     *
     * @version v1
    */
    function buscarDadosMantidaCandidata( $mnmid ){
        global $db;

        $sql = "
            SELECT  *,
                    m.mtdid AS mm_mtdid,
                    trim( replace( to_char( cast(m.mtdcep as bigint), '00000-000' ), ':', '.' ) ) AS mtdcep,
                    mt.muncod AS mantida_muncod,
                    m.muncod as muncod
            FROM maismedicomec.mantidacandidata AS m
            JOIN maismedicomec.mantidamunicipiocandidata AS mt ON mt.mtdid = m.mtdid
            WHERE mt.mnmid = {$mnmid}
        ";
        return $dados = $db->pegaLinha($sql);
    }

    /**
     * functionName buscarDadoseditarMantida
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $mtdid é o id da tabela de mantenedores.
     * @return string dados referente a mantenedora, preenche o formulario.
     *
     * @version v1
    */
    function buscarDadosEditarMantida( $dados ){
        global $db;

        $mnmid = $dados['mnmid'];

        $sql = "
            SELECT  *,
                    m.mtdid AS mm_mtdid,
                    trim( replace( to_char( cast(m.mtdcep as bigint), '00000-000' ), ':', '.' ) ) AS mtdcep,
                    mt.muncod AS mantida_muncod,
                    m.muncod as muncod
            FROM maismedicomec.mantida AS m
            JOIN maismedicomec.mantidamunicipio AS mt ON mt.mtdid = m.mtdid
            WHERE mt.mnmid = {$mnmid}
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            foreach($dados as $key => $value){
                $dados[$key] = iconv("ISO-8859-1", "UTF-8", trim($value) );
            }
            echo simec_json_encode( $dados );
            die;
        }
    }


    /**
     * functionName buscarDadoseditarMantidaCandidata
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $mtdid é o id da tabela de mantenedores.
     * @return string dados referente a mantenedora, preenche o formulario.
     *
     * @version v1
    */
    function buscarDadosEditarMantidaCandidata( $dados ){
        global $db;

        $mnmid = $dados['mnmid'];

        $sql = "
            SELECT  *,
                    m.mtdid AS mm_mtdid,
                    trim( replace( to_char( cast(m.mtdcep as bigint), '00000-000' ), ':', '.' ) ) AS mtdcep,
                    mt.muncod AS mantida_muncod,
                    m.muncod as muncod
            FROM maismedicomec.mantidacandidata AS m
            JOIN maismedicomec.mantidamunicipiocandidata AS mt ON mt.mtdid = m.mtdid
            WHERE mt.mnmid = {$mnmid}
        ";
        $dados = $db->pegaLinha($sql);

        if($dados != ''){
            foreach($dados as $key => $value){
                $dados[$key] = iconv("ISO-8859-1", "UTF-8", trim($value) );
            }
            echo simec_json_encode( $dados );
            die;
        }
    }
    /**
     * functionName excluirDadosMantida
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $mtdid é o id da tabela de mantenedores.
     * @return string OK para confirmação que o registro foi "excluido" de maneira logica.
     *
     * @version v1
    */
    function excluirDadosMantida( $dados ){
        global $db;

        $mnmid = $dados['mnmid'];

        $sql = "delete from maismedicomec.mantidamunicipio WHERE mnmid = {$mnmid}";
        $db->executar($sql);
        $db->commit();
        echo "OK";
        die();
    }

    function excluirDadosMantidaExpReg( $dados ){
        global $db;

        $mneid = $dados['mneid'];

        $sql = "
            UPDATE maismedicomec.mantidaexpregulatoria
                SET mnestatus = 'I'
             WHERE mneid = {$mneid} RETURNING mneid;
        ";
        $mneid = $db->pegaUm($sql);

        if( $mneid > 0 ){
            $db->commit();
            echo '<script>
            		alert("Operação efetuda com sucesso!");
            		window.location.href = "maismedicomec.php?modulo=principal/mantenedora/cad_exp_regulatoria&acao=A&aba=mantida";
            	  </script>';
            die();
        }
    }

     /**
     * functionName excluirDadosMantidaCAndidata
     *
     * @author Marcus Rocha
     *
     * @param string $mtdid é o id da tabela de mantenedores.
     * @return string OK para confirmação que o registro foi "excluido" de maneira logica.
     *
     * @version v1
    */

    function excluirDadosMantidaCandidata( $dados ){
        global $db;
        /*
        $mtdid = $dados['mtdid'];

        $sql = "
            UPDATE maismedicomec.mantidacandidata
                SET mtdstatus = 'I'
             WHERE mtdid = {$mtdid} RETURNING mtdid;
        ";
        $mtdid = $db->pegaUm($sql);

        if( $mtdid > 0 ){
            $db->commit();
            echo "OK";
            die();
        }
		*/

        $mnmid = $dados['mnmid'];

        $sql = "delete from maismedicomec.mantidamunicipiocandidata WHERE mnmid = {$mnmid}";
        $db->executar($sql);
        $db->commit();
        echo "OK";
        die();
    }

    /**
     * functionName excluirDadosMantida
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $mtdid é o id da tabela de mantenedores.
     * @return string OK para confirmação que o registro foi "excluido" de maneira logica.
     *
     * @version v1
    */
    function excluirDadosMantidaCorpoDirigente( $dados ){
        global $db;

        $cpdid = $dados['cpdid'];
        //$dmdid = $dados['dmdid'];
        //$dmtid = $dados['dmtid'];

        /*
        if( $cpdid != '' ){
            if( $dmdid != '' ){
                $sql = "
                    DELETE FROM maismedicomec.dirigentemantida WHERE cpdid = {$cpdid} AND dmdid = {$dmdid} RETURNING cpdid;
                ";
            }
            if( $dmtid != '' ){
                $sql .= "
                    DELETE FROM maismedicomec.dirigentemantenedora WHERE cpdid = {$cpdid} AND dmtid = {$dmtid} RETURNING cpdid;
                ";
            }
            $cpdid = $db->pegaUm($sql);
        }
		*/

        $sql = "DELETE FROM maismedicomec.dirigentemantida WHERE cpdid = {$cpdid}";
        $db->executar($sql);

        $sql = "UPDATE maismedicomec.corpodirigente SET cpdstatus='I' WHERE cpdid = {$cpdid} RETURNING cpdid;";
        $cpdid = $db->pegaUm($sql);


        if( $cpdid > 0 ){
            $db->commit();
            echo "OK";
            die();
        }
    }

    /**
     * functionName salvarDadosCorpoDirigente
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é o request dos dados "$_POST" do formulário.
     * @return string não há retorno. Só atribuição dos valores a sessão.
     *
     * @version v1
    */
    function salvarDadosCorpoDirigente( $dados ){
        global $db;

        $mntid_mtdid    = trim($dados['mntid_mtdid']);
        $dmdid          = trim($dados['dmdid']);
        $dmtid          = trim($dados['dmtid']);

        $mtdid              = $dados['mtdid'];
        $mntid              = $_SESSION['maismedicomec']['mntid'];

        $cpdid                = $dados['cpdid'];
        $cpdcpf               = "'".str_replace( '.', '', str_replace( '-', '', $dados['cpdcpf'] ) )."'";
        $cpdnome              = "'".trim( $dados['cpdnome'] )."'";
        $cpdcargo             = "'".trim( $dados['cpdcargo'] )."'";
        $cpdtelefonecomercial = "'".trim( $dados['cpdtelefonecomercial'] )."'";
        $cpdtelcelular        = "'".trim( $dados['cpdtelcelular'] )."'";
        $cpdemail             = "'".trim( $dados['cpdemail'] )."'";

        #DADOS DO REPRESENTANTE LEGAL.
        if( $cpdid != '' ){
            $sql = "
                UPDATE maismedicomec.corpodirigente
                    SET cpdcpf                  = {$cpdcpf},
                        cpdnome                 = {$cpdnome},
                        cpdtelefonecomercial    = {$cpdtelefonecomercial},
                        cpdtelcelular           = {$cpdtelcelular},
                        cpdemail                = {$cpdemail},
                        cpdcargo                = {$cpdcargo}
                WHERE cpdid = {$cpdid} RETURNING cpdid;
            ";
            $cpdid = $db->pegaUm($sql);
        }else{
            $sql = "
                INSERT INTO maismedicomec.corpodirigente(
                        cpdcpf,
                        cpdnome,
                        cpdtelefonecomercial,
                        cpdtelcelular,
                        cpdemail,
                        cpdcargo
                    ) VALUES (
                        {$cpdcpf},
                        {$cpdnome},
                        {$cpdtelefonecomercial},
                        {$cpdtelcelular},
                        {$cpdemail},
                        {$cpdcargo}
                ) RETURNING cpdid;
            ";
            $cpdid = $db->pegaUm($sql);
        }

        if( $cpdid > 0 && ($mtdid > 0 || $mntid > 0) ){


            if( $mntid_mtdid == 'M' ){
                if( $dmtid > 0 ){
                    $sql = "UPDATE maismedicomec.dirigentemantenedora SET mntid = {$mntid}, cpdid = {$cpdid} WHERE dmtid = {$dmtid} RETURNING dmtid;";
                }else{
                    $sql = "INSERT INTO maismedicomec.dirigentemantenedora(mntid, cpdid)VALUES({$mntid}, {$cpdid}) RETURNING dmtid;";
                }
                $param_retorno = "&dmtid={$dmtid}&cpdid={$cpdid}";
                $dmtid = $db->pegaUm($sql);
            }

            if( $mntid_mtdid == 'D' ){
                if( $dmdid > 0 ){
                    $sql = "UPDATE maismedicomec.dirigentemantida SET mtdid = {$mtdid}, cpdid = {$cpdid} WHERE dmdid = {$dmdid} RETURNING dmdid;";
                }else{

                    $sql = "INSERT INTO maismedicomec.dirigentemantida(cpdid, mtdid)VALUES({$cpdid}, {$mtdid}) RETURNING dmdid;";
                }
                $param_retorno = "&dmdid={$dmdid}&cpdid={$cpdid}";
                $dmdid = $db->pegaUm($sql);
            }
        }

        if( $cpdid > 0 && ($dmtid > 0 || $dmdid > 0) ){
            $db->commit();
            $db->sucesso("principal/mantenedora/cad_corpo_dirigente", $param_retorno, "Dirigente cadastrado com sucesso!");
        }else{
            $db->sucesso("principal/mantenedora/cad_corpo_dirigente", $param_retorno, "Cadastrado não realizado, Tente novamnete mais tarde!");
        }
    }

    /**
     * functionName salvarDadosMantenedora
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é o request dos dados "$_POST" do formulário.
     * @return string não há retorno. Só atribuição dos valores a sessão.
     *
     * @version v1
    */
    function salvarDadosMantenedora($dados){
        global $db;

        #DADOS DO REPRESENTANTE LEGAL.
        $rplid          = $dados['rplid'];
        $rplcpf         = "'".trim( str_replace( '.', '', str_replace( '-', '', $dados['rplcpf'] ) ) )."'";
        $rpldsc         = "'".trim( $dados['rpldsc'] )."'";
        $rplsexo        = "'".trim( $dados['rplsexo'] )."'";
        $rplrg          = "'".trim( $dados['rplrg'] )."'";
        $rplorgaoexprg  = "'".trim( $dados['rplorgaoexprg'] )."'";
        $rplestuf       = "'".trim( $dados['rplestuf'] )."'";
        $rpltelcomercial= "'".trim( $dados['rpltelcomercial'] )."'";
        $rpltelcelular  = "'".trim( $dados['rpltelcelular'] )."'";
        $rplemail       = "'".trim( $dados['rplemail'] )."'";

        #DADOS DA MANTENEDORA.
        $mm_mntid       = $dados['mm_mntid'];
        $mntid          = $dados['mntid'];
        $mntcnpj        = "'".trim( str_replace( '.', '', str_replace( '/', '', str_replace( '-', '', $dados['mntcnpj'] ) ) ) )."'";
        $mntrazaosocial = "'".trim( addslashes( $dados['mntrazaosocial'] ) )."'";

        $mntsigla       = $dados['mntsigla']  == '' ? 'NULL' : "'".trim( addslashes( $dados['mntsigla'] ) )."'";
        $mntcep         = $dados['mntcep'] == '' ? 'NULL' : "'".str_replace( '-', '', $dados['mntcep'] )."'";
        $mntlogradouro  = $dados['mntlogradouro'] == '' ? 'NULL' : "'".trim( $dados['mntlogradouro'] )."'";
        $mntcomplemento = $dados['mntcomplementoend'] == '' ? 'NULL' : "'".trim($dados['mntcomplementoend'])."'";
        $mntbairro      = $dados['mntbairro'] == '' ? 'NULL' : "'".trim($dados['mntbairro'])."'";
        $estuf          = $dados['estuf'] == '' ? 'NULL' : "'".trim($dados['estuf'])."'";
        $muncod         = $dados['muncod'] == '' ? 'NULL' : "'".trim($dados['muncod'])."'";
        $mnttelefone01  = $dados['mnttelefone01'] == '' ? 'NULL' : "'".trim($dados['mnttelefone01'])."'";
        $mnttelefone02  = $dados['mnttelefone02'] == '' ? 'NULL' : "'".trim($dados['mnttelefone02'])."'";
        $mnttelefone03  = $dados['mnttelefone03'] == '' ? 'NULL' : "'".trim($dados['mnttelefone03'])."'";
        $mntemail       = $dados['mntemail'] == '' ? 'NULL' : "'".trim($dados['mntemail'])."'";

        $usucpf = "'".$_SESSION['usucpf']."'";

        #DADOS DO REPRESENTANTE LEGAL.
        if( $rplid != '' ){
            $sql = "
                UPDATE  maismedicomec.representantelegal
                    SET rpldsc          = {$rpldsc},
                        rplcpf          = {$rplcpf},
                        rplsexo         = {$rplsexo},
                        rplrg           = {$rplrg},
                        rplorgaoexprg   = {$rplorgaoexprg},
                        estuf           = {$rplestuf},
                        rpltelcomercial = {$rpltelcomercial},
                        rpltelcelular   = {$rpltelcelular},
                        rplemail        = {$rplemail},
                        usucpf          = {$usucpf},
                        rpldtinclusao   = now()
                WHERE rplid = {$rplid} RETURNING rplid;
            ";
            $rplid = $db->pegaUm($sql);
        }else{
            $sql = "
                INSERT INTO maismedicomec.representantelegal(
                        rpldsc,
                        rplcpf,
                        rplsexo,
                        rplrg,
                        rplorgaoexprg,
                        estuf,
                        rpltelcomercial,
                        rpltelcelular,
                        rplemail,
                        usucpf,
                        rpldtinclusao
                    ) VALUES (
                        {$rpldsc},
                        {$rplcpf},
                        {$rplsexo},
                        {$rplrg},
                        {$rplorgaoexprg},
                        {$rplestuf},
                        {$rpltelcomercial},
                        {$rpltelcelular},
                        {$rplemail},
                        {$usucpf},
                        now()
                ) RETURNING rplid;
            ";
            $rplid = $db->pegaUm($sql);
        }
        if( $rplid != '' ){
            if( $mm_mntid != '' ){
                $sql = "
                    UPDATE maismedicomec.mantenedora
                        SET rplidreplegal       = {$rplid},
                            mntcnpj             = {$mntcnpj},
                            mntrazaosocial      = {$mntrazaosocial},
                            mntsigla            = {$mntsigla},
                            mntcep              = {$mntcep},
                            mntlogradouro       = {$mntlogradouro},
                            mntcomplementoend   = {$mntcomplemento},
                            mntbairro           = {$mntbairro},
                            estuf               = {$estuf},
                            muncod              = {$muncod},
                            mnttelefone01       = {$mnttelefone01},
                            mnttelefone02       = {$mnttelefone02},
                            mnttelefone03       = {$mnttelefone03},
                            mntemail            = {$mntemail},
                            usucpf              = {$usucpf},
                            mntdtinclusao		= now()
                    WHERE mntid = {$mntid} RETURNING mntid;
                ";
                $mntid = $db->pegaUm($sql);
            }else{
                $sql = "
                    INSERT INTO maismedicomec.mantenedora(
                            rplidreplegal,
                            mntid,
                            mntcnpj,
                            mntrazaosocial,
                            mntsigla,
                            mntcep,
                            mntlogradouro,
                            mntcomplementoend,
                            mntbairro,
                            estuf,
                            muncod,
                            mnttelefone01,
                            mnttelefone02,
                            mnttelefone03,
                            mntemail,
                            usucpf,
                            mntdtinclusao
                        )VALUES(
                            {$rplid},
                            {$mntid},
                            {$mntcnpj},
                            {$mntrazaosocial},
                            {$mntsigla},
                            {$mntcep},
                            {$mntlogradouro},
                            {$mntcomplemento},
                            {$mntbairro},
                            {$estuf},
                            {$muncod},
                            {$mnttelefone01},
                            {$mnttelefone02},
                            {$mnttelefone03},
                            {$mntemail},
                            {$usucpf},
                            now()
                    ) RETURNING mntid;
                ";
                $mntid = $db->pegaUm($sql);
            }
        }

        if( $mntid > 0 ){
            $db->commit();
            $db->sucesso("principal/mantenedora/cad_mantenedora", "&acao=A&mntid={$mntid}", "Operação realizada com sucesso!");
        }else{
            $db->sucesso("principal/mantenedora/cad_mantenedora", "&acao=A&mntid={$mntid}", "Operação não realizada, Tente novamnete mais tarde!");
        }
    }

    /**
     * functionName salvarDadosMatida
     *
     * @author Luciano F. Ribeiro
     *
     * @param string $dados é o request dos dados "$_POST" do formulário.
     * @return string não há retorno. Só atribuição dos valores a sessão.
     *
     * @version v1
    */
    function salvarDadosMatida( $dados ){
        global $db;

        $mtdid              = $dados['mtdid'];
        $mnmid              = $dados['mnmid'];
        $mm_mtdid           = $dados['mm_mtdid'];

        $mtddsc             = "'".trim( $dados['mtddsc'] )."'";
        $mntsigla           = "'".trim( strtoupper($dados['mntsigla']) )."'";
        $mtdimovelsit       = "'".trim( $dados['mtdimovelsit'] )."'";

        $mtdcep             = $dados['mtdcep'] == '' ? 'NULL' : "'".str_replace( '-', '', $dados['mtdcep'] )."'";
        $mtdlogradouro      = $dados['mtdlogradouro'] == '' ? 'NULL' : "'".trim( $dados['mtdlogradouro'] )."'";
        $mtdcomplementoend  = $dados['mtdcomplementoend'] == '' ? 'NULL' : "'".trim( $dados['mtdcomplementoend'] )."'";
        $mtdbairro          = $dados['mtdbairro'] == '' ? 'NULL' : "'".trim( $dados['mtdbairro'] )."'";
        $estuf              = $dados['estuf'] == '' ? 'NULL' : "'".trim( $dados['estuf'] )."'";
        $muncod             = $dados['muncod'] == '' ? 'NULL' : "'".trim( $dados['muncod'] )."'";

        $mtdtelefone01      = $dados['mtdtelefone01'] == '' ? 'NULL' : "'".trim( $dados['mtdtelefone01'] )."'";
        $mtdtelefone02      = $dados['mtdtelefone02'] == '' ? 'NULL' : "'".trim( $dados['mtdtelefone02'] )."'";
        $mtdtelefone03      = $dados['mtdtelefone03'] == '' ? 'NULL' : "'".trim( $dados['mtdtelefone03'] )."'";
        $mtdsite            = $dados['mtdsite'] == '' ? 'NULL' : "'".trim( $dados['mtdsite'] )."'";

        $mtdfaculdade    = $dados['mtdfaculdade'] != '' ? "'".$dados['mtdfaculdade']."'" : 'NULL' ;
        $mtdcentrouniversitario = $dados['mtdcentrouniversitario'] != '' ? "'".$dados['mtdcentrouniversitario']."'" : 'NULL' ;
        $mtduniversidade = $dados['mtduniversidade'] != '' ? "'".$dados['mtduniversidade']."'" : 'NULL' ;

        $mtdfilantropica    = $dados['mtdfilantropica'] != '' ? "'".$dados['mtdfilantropica']."'" : 'NULL' ;
        $mtdcomunitaria     = $dados['mtdcomunitaria'] != '' ? "'".$dados['mtdcomunitaria']."'" : 'NULL' ;
        $mtdconfessional    = $dados['mtdconfessional'] != '' ? "'".$dados['mtdconfessional']."'" : 'NULL' ;


        $mtdautfuncionamento= $dados['mtdautfuncionamento'] == 't' ? "'".$dados['mtdautfuncionamento']."'" : "'".'f'."'" ;
        $mtdcredenciamento  = $dados['mtdcredenciamento'] == 't' ? "'".$dados['mtdcredenciamento']."'" : "'".'f'."'" ;

        $mtdprivadacomlucro  = $dados['mtdprivadacomlucro'] == 't' ? "'".$dados['mtdprivadacomlucro']."'" : "'".'f'."'" ;
        $mtdprivadasemlucro  = $dados['mtdprivadasemlucro'] == 't' ? "'".$dados['mtdprivadasemlucro']."'" : "'".'f'."'" ;

        $mantida_muncod     = $dados['mantida_muncod'] == '' ? 'NULL' : "'".trim( $dados['mantida_muncod'] )."'";
        $mnmprioridade     = $dados['mnmprioridade'] == '' ? 'NULL' : "'".trim( $dados['mnmprioridade'] )."'";

        $usucpf = "'".$_SESSION['usucpf']."'";


        //verifica se tem municipio na candidata
		    if($mantida_muncod) {
	        	$sql = "select mnmid from maismedicomec.mantidamunicipiocandidata where mntid = {$_SESSION['maismedicomec']['mntid']} and muncod = {$mantida_muncod} ";
	        	$existe = $db->pegaUm($sql);
	        	if($existe){
	        		echo '<script>
	        				alert("Na aba \'Credenciar nova mantida\', Já existe uma mantida com este Município. Favor selecione outro Município.");
	        				history.back();
	        			  </script>';
	        		die;
	        	}
		    }
    	//verifica se exite o mesmo municipio para a mesma mantida
        	if($mtdid){
        		if($mnmid) $andmnmid = " and mnmid not in ({$mnmid}) ";
	        	$sql = "select mnmid from maismedicomec.mantidamunicipio where mntid = {$_SESSION['maismedicomec']['mntid']} and mtdid = {$mtdid} and muncod = {$mantida_muncod} {$andmnmid}";
	        	$existe = $db->pegaUm($sql);
	        	if($existe){
	        		echo '<script>
	        				alert("Já existe um Município para esta Mantida. Favor selecione outro Município.");
	        				history.back();
	        			  </script>';
	        		die;
	        	}

	        	//no maximo 3 da mesma mantida
	        	/*
	        	$sql = "select count(mnmid) as total from maismedicomec.mantidamunicipio where mntid = {$_SESSION['maismedicomec']['mntid']} and mtdid = {$mtdid} {$andmnmid}";
	        	$qtd = $db->pegaUm($sql);
	        	if((int)$qtd >= 3){
	        		echo '<script>
	        				alert("No máximo 3 Municípios para mesma mantida.");
	        				history.back();
	        			  </script>';
	        		die;
	        	}
				*/
        	}

        //verifica total de mantidas por estado
    		if($mantida_muncod){

                		$sql = "SELECT estuf FROM territorios.municipio where muncod = ".$mantida_muncod;
                		$estuf2 = $db->pegaUm($sql);

                		$sql = "select lpuflimiteproposta from maismedicomec.limitepropostaufies where estuf = '{$estuf2}'";
                		$limiteEstado = $db->pegaUm($sql);

                		if(!$limiteEstado){
                			echo '<script>
			        				alert("Limite de mantida por o estado não existe, favor avisar o gestor do sistema.");
			        				history.back();
			        			  </script>';
		        			die;
                		}

                		if($mnmid) $andmnmid2 = " and mm.mnmid not in ({$mnmid}) ";
	                	$sqlp2 = "select count(mm.mnmid) from maismedicomec.mantidamunicipio mm
	                			 inner join territorios.municipio mu on mu.muncod = mm.muncod
	                			 where mu.estuf = '{$estuf2}' and
	                			 	   mm.mntid = {$_SESSION['maismedicomec']['mntid']} {$andmnmid2}
	                	         ";
	                	$totalEstado = $db->pegaUm($sqlp2);

	                	if($totalEstado >= $limiteEstado){
	                		echo '<script>
			        				alert("Limite de '.$limiteEstado.' mantida(s) para o estado de '.$estuf2.'.");
			        				history.back();
			        			  </script>';
		        			die;
	                	}

    		}

        //verifica se existe mantida
    		if($mtdid){
		        $sql = "select mtdid from maismedicomec.mantida where mtdid = ".$mtdid;
		        $mtdidExiste = $db->pegaUm($sql);
    		}


        if( $mtdidExiste != '' ){


            $sql = "
                UPDATE maismedicomec.mantida
                    SET mtddsc              = {$mtddsc},
                        mntsigla            = {$mntsigla},
                        mtdimovelsit        = {$mtdimovelsit},
                        mtdcep              = {$mtdcep},
                        mtdlogradouro       = {$mtdlogradouro},
                        mtdcomplementoend   = {$mtdcomplementoend},
                        mtdbairro           = {$mtdbairro},
                        estuf               = {$estuf},
                        muncod              = {$muncod},
                        mtdtelefone01       = {$mtdtelefone01},
                        mtdtelefone02       = {$mtdtelefone02},
                        mtdtelefone03       = {$mtdtelefone03},
                        mtdsite             = {$mtdsite },
                        mtdfaculdade        = {$mtdfaculdade},
                        mtdcentrouniversitario = {$mtdcentrouniversitario},
                        mtduniversidade     = {$mtduniversidade},
                        mtdfilantropica     = {$mtdfilantropica},
                        mtdcomunitaria      = {$mtdcomunitaria},
                        mtdconfessional     = {$mtdconfessional},
                        mtdautfuncionamento = {$mtdautfuncionamento},
                        mtdcredenciamento   = {$mtdcredenciamento},
                        usucpf              = {$usucpf},
                        mtdprivadacomlucro  = {$mtdprivadacomlucro},
                        mtdprivadasemlucro  = {$mtdprivadasemlucro},
                        mtddtinclusao		= now()
                 WHERE mtdid = {$mtdid} RETURNING mtdid;
            ";

        }else{

        	/*
        	if(!$mtdid){
        		$sql = "select (max(mtdid)+1) as maximo from maismedicomec.mantida";
        		$mtdid = $db->pegaUm($sql);
        	}
        	*/

        	if(!$mtdid){
        		echo '<script>
        				alert("É necessário selecionar a mantida!");
        				history.back();
        			  </script>';
        		die;
        	}

            $sql = "
                INSERT INTO maismedicomec.mantida(
                        mtdid,
                        mtddsc,
                        mntsigla,
                        mtdimovelsit,
                        mtdcep,
                        mtdlogradouro,
                        mtdcomplementoend,
                        mtdbairro,
                        estuf,
                        muncod,
                        mtdtelefone01,
                        mtdtelefone02,
                        mtdtelefone03,
                        mtdsite,
                        mtdfaculdade,
                        mtdcentrouniversitario,
                        mtduniversidade,
                        mtdfilantropica,
                        mtdcomunitaria,
                        mtdconfessional,
                        mtdautfuncionamento,
                        mtdcredenciamento,
                        mtdprivadacomlucro,
                        mtdprivadasemlucro,
                        usucpf,
                        mtddtinclusao
                    ) VALUES (
                        {$mtdid},
                        {$mtddsc},
                        {$mntsigla},
                        {$mtdimovelsit},
                        {$mtdcep},
                        {$mtdlogradouro},
                        {$mtdcomplementoend},
                        {$mtdbairro},
                        {$estuf},
                        {$muncod},
                        {$mtdtelefone01},
                        {$mtdtelefone02},
                        {$mtdtelefone03},
                        {$mtdsite },
                        {$mtdfaculdade},
                        {$mtdcentrouniversitario},
                        {$mtduniversidade},
                        {$mtdfilantropica},
                        {$mtdcomunitaria},
                        {$mtdconfessional},
                        {$mtdautfuncionamento},
                        {$mtdcredenciamento},
                        {$mtdprivadacomlucro},
                        {$mtdprivadasemlucro},
                        {$usucpf},
                        now()
                ) RETURNING mtdid;
            ";

        }

        $mtdid = $db->pegaUm($sql);

        if( !$mm_mtdid ){
       		$var_controle = "I";
        }else{
       		$var_controle = "U";
        }




        if( $var_controle == 'I' && $mtdid > 0 ){
        	$sql = "select (COALESCE(max(mnmid)+1,1)) as maximo from maismedicomec.mantidamunicipio
					union
					select (COALESCE(max(mnmid)+1,1)) as maximo from maismedicomec.mantidamunicipiocandidata
					order by 1 desc";
        	$mnmid = $db->pegaUm($sql);
            $sql = "INSERT INTO maismedicomec.mantidamunicipio(mnmid, mntid, mtdid, muncod, mnmprioridade) VALUES ({$mnmid}, {$_SESSION['maismedicomec']['mntid']}, {$mtdid}, {$mantida_muncod}, {$mnmprioridade}) RETURNING mntid;";
            $mntid = $db->executar($sql);
        }else{
            $sql = "UPDATE maismedicomec.mantidamunicipio SET muncod = {$mantida_muncod}, mnmprioridade = {$mnmprioridade} WHERE mnmid = {$mnmid} RETURNING mntid;";
            $mntid = $db->executar($sql);
        }

        if( $mtdid > 0 ){
            $db->commit();
            $db->sucesso("principal/mantenedora/cad_mantida", "", "INSCRIÇÃO executada com sucesso!");
        }else{
            $db->sucesso("principal/mantenedora/cad_mantida", "", "INSCRIÇÃO não realizada, Tente novamnete mais tarde!");
        }

    }


        /**
     * functionName salvarDadosMatidaCandidata
     *
     * @author Marcus Rocha
     *
     * @param string $dados é o request dos dados "$_POST" do formulário.
     * @return string não há retorno. Só atribuição dos valores a sessão.
     *
     * @version v1
    */
    function salvarDadosMatidaCandidata( $dados ){
        global $db;

        $mtdid              = $dados['mtdid'];
        $mnmid              = $dados['mnmid'];
        $mm_mtdid           = $dados['mm_mtdid'];

        $mtddsc             = "'".trim( $dados['mtddsc'] )."'";
        $mntsigla           = "'".trim( strtoupper($dados['mntsigla']) )."'";
        $mtdimovelsit       = "'".trim( $dados['mtdimovelsit'] )."'";

        $mtdcep             = $dados['mtdcep'] == '' ? 'NULL' : "'".str_replace( '-', '', $dados['mtdcep'] )."'";
        $mtdlogradouro      = $dados['mtdlogradouro'] == '' ? 'NULL' : "'".trim( $dados['mtdlogradouro'] )."'";
        $mtdcomplementoend  = $dados['mtdcomplementoend'] == '' ? 'NULL' : "'".trim( $dados['mtdcomplementoend'] )."'";
        $mtdbairro          = $dados['mtdbairro'] == '' ? 'NULL' : "'".trim( $dados['mtdbairro'] )."'";
        $estuf              = $dados['estuf'] == '' ? 'NULL' : "'".trim( $dados['estuf'] )."'";
        $muncod             = $dados['muncod'] == '' ? 'NULL' : "'".trim( $dados['muncod'] )."'";

        $mtdtelefone01      = $dados['mtdtelefone01'] == '' ? 'NULL' : "'".trim( $dados['mtdtelefone01'] )."'";
        $mtdtelefone02      = $dados['mtdtelefone02'] == '' ? 'NULL' : "'".trim( $dados['mtdtelefone02'] )."'";
        $mtdtelefone03      = $dados['mtdtelefone03'] == '' ? 'NULL' : "'".trim( $dados['mtdtelefone03'] )."'";
        $mtdsite            = $dados['mtdsite'] == '' ? 'NULL' : "'".trim( $dados['mtdsite'] )."'";


        $mtdcentrouniversitario = $dados['mtdcentrouniversitario'] != '' ? "'".$dados['mtdcentrouniversitario']."'" : 'NULL' ;
        $mtduniversidade = $dados['mtduniversidade'] != '' ? "'".$dados['mtduniversidade']."'" : 'NULL' ;

        $mtdfilantropica    = $dados['mtdfilantropica'] != '' ? "'".$dados['mtdfilantropica']."'" : 'NULL' ;
        $mtdcomunitaria     = $dados['mtdcomunitaria'] != '' ? "'".$dados['mtdcomunitaria']."'" : 'NULL' ;
        $mtdconfessional    = $dados['mtdconfessional'] != '' ? "'".$dados['mtdconfessional']."'" : 'NULL' ;
        $mtdprivada  		= $dados['mtdprivada'] == 't' ? "'".$dados['mtdprivada']."'" : "'".'f'."'" ;

        $mtdfaculdade    =  $dados['mtdfaculdade'] == 't' ? "'".$dados['mtdfaculdade']."'" : "'".'f'."'" ;

        $mtdautfuncionamento= $dados['mtdautfuncionamento'] == 't' ? "'".$dados['mtdautfuncionamento']."'" : "'".'f'."'" ;
        $mtdcredenciamento  = $dados['mtdcredenciamento'] == 't' ? "'".$dados['mtdcredenciamento']."'" : "'".'f'."'" ;

        $mantida_muncod     = $dados['mantida_muncod'] == '' ? 'NULL' : "'".trim( $dados['mantida_muncod'] )."'";
        $mnmprioridade     = $dados['mnmprioridade'] == '' ? 'NULL' : "'".trim( $dados['mnmprioridade'] )."'";

        $usucpf = "'".$_SESSION['usucpf']."'";

    	//verifica se tem municipio na mantida indicada
		    if($mantida_muncod) {
	        	$sql = "select mnmid from maismedicomec.mantidamunicipio where mntid = {$_SESSION['maismedicomec']['mntid']} and muncod = {$mantida_muncod} ";
	        	$existe = $db->pegaUm($sql);
	        	if($existe){
	        		echo '<script>
	        				alert("Na aba \'Mantida Indicada\', Já existe uma mantida com este Município. Favor selecione outro Município.");
	        				history.back();
	        			  </script>';
	        		die;
	        	}
		    }

    	//verifica se exite o mesmo municipio para a mesma mantida
        	if($mtdid){
        		if($mnmid) $andmnmid = " and mnmid not in ({$mnmid}) ";
	        	$sql = "select mnmid from maismedicomec.mantidamunicipiocandidata where mntid = {$_SESSION['maismedicomec']['mntid']} and mtdid = {$mtdid} and muncod = {$mantida_muncod} {$andmnmid}";
	        	$existe = $db->pegaUm($sql);
	        	if($existe){
	        		echo '<script>
	        				alert("Já existe um Município para esta Mantida. Favor selecione outro Município.");
	        				history.back();
	        			  </script>';
	        		die;
	        	}

	        	//no maximo 3 da mesma mantida
	        	$sql = "select count(mnmid) as total from maismedicomec.mantidamunicipiocandidata where mntid = {$_SESSION['maismedicomec']['mntid']} and mtdid = {$mtdid} {$andmnmid}";
	        	$qtd = $db->pegaUm($sql);
	        	if((int)$qtd >= 3){
	        		echo '<script>
	        				alert("No máximo 3 Municípios para mesma mantida.");
	        				history.back();
	        			  </script>';
	        		die;
	        	}
        	}

    	//verifica total de mantidas por estado
    		if($mantida_muncod){

                		$sql = "SELECT estuf FROM territorios.municipio where muncod = ".$mantida_muncod;
                		$estuf2 = $db->pegaUm($sql);

                		$sql = "select lpuflimiteproposta from maismedicomec.limitepropostaufies where estuf = '{$estuf2}'";
                		$limiteEstado = $db->pegaUm($sql);

                		if(!$limiteEstado){
                			echo '<script>
			        				alert("Limite de nova mantida por o estado não existe, favor avisar o gestor do sistema.");
			        				history.back();
			        			  </script>';
		        			die;
                		}

                		if($mnmid) $andmnmid2 = " and mm.mnmid not in ({$mnmid}) ";
	                	$sqlp2 = "select count(mm.mnmid) from maismedicomec.mantidamunicipiocandidata mm
	                			 inner join territorios.municipio mu on mu.muncod = mm.muncod
	                			 where mu.estuf = '{$estuf2}' and
	                			 	   mm.mntid = {$_SESSION['maismedicomec']['mntid']} {$andmnmid2}
	                	         ";
	                	$totalEstado = $db->pegaUm($sqlp2);

	                	if($totalEstado >= $limiteEstado){
	                		echo '<script>
			        				alert("Limite de '.$limiteEstado.' nova mantida(s) para o estado de '.$estuf2.'.");
			        				history.back();
			        			  </script>';
		        			die;
	                	}

    		}

        if( $mtdid != '' ){
            $sql = "
                UPDATE maismedicomec.mantidacandidata
                    SET mtddsc              = {$mtddsc},
                        mntsigla            = {$mntsigla},
                        mtdimovelsit        = {$mtdimovelsit},
                        mtdcep              = {$mtdcep},
                        mtdlogradouro       = {$mtdlogradouro},
                        mtdcomplementoend   = {$mtdcomplementoend},
                        mtdbairro           = {$mtdbairro},
                        estuf               = {$estuf},
                        muncod              = {$muncod},
                        mtdtelefone01       = {$mtdtelefone01},
                        mtdtelefone02       = {$mtdtelefone02},
                        mtdtelefone03       = {$mtdtelefone03},
                        mtdsite             = {$mtdsite },
                        mtdfaculdade        = {$mtdfaculdade},
                        --mtdcentrouniversitario = {$mtdcentrouniversitario},
                        --mtduniversidade     = {$mtduniversidade},
                        mtdfilantropica     = {$mtdfilantropica},
                        mtdcomunitaria      = {$mtdcomunitaria},
                        mtdconfessional     = {$mtdconfessional},
                        mtdautfuncionamento = {$mtdautfuncionamento},
                        mtdcredenciamento   = {$mtdcredenciamento},
                        usucpf              = {$usucpf},
                        mtdprivada          = {$mtdprivada}
                 WHERE mtdid = {$mtdid} RETURNING mtdid;
            ";

        }else{

        	if(!$mtdid){
        		$sql = "select (COALESCE(max(mtdid)+1,1)) as maximo from maismedicomec.mantidacandidata";
        		$mtdid = $db->pegaUm($sql);

        		//verifica se existe na matida
        		$sql = "select count(mtdid) as total from maismedicomec.mantida where mtdid = {$mtdid}";
        		$totalM = $db->pegaUm($sql);

        		if($totalM>0){
        			$sql = "select (COALESCE(max(mtdid)+1,1)) as maximo from maismedicomec.mantida";
        			$mtdid = $db->pegaUm($sql);
        		}

        	}

            $sql = "
                INSERT INTO maismedicomec.mantidacandidata(
                        mtdid,
                        mtddsc,
                        mntsigla,
                        mtdimovelsit,
                        mtdcep,
                        mtdlogradouro,
                        mtdcomplementoend,
                        mtdbairro,
                        estuf,
                        muncod,
                        mtdtelefone01,
                        mtdtelefone02,
                        mtdtelefone03,
                        mtdsite,
                        mtdfaculdade,
                        mtdfilantropica,
                        mtdcomunitaria,
                        mtdconfessional,
                        mtdautfuncionamento,
                        mtdcredenciamento,
                        usucpf,
                        mtdprivada
                    ) VALUES (
                        {$mtdid},
                        {$mtddsc},
                        {$mntsigla},
                        {$mtdimovelsit},
                        {$mtdcep},
                        {$mtdlogradouro},
                        {$mtdcomplementoend},
                        {$mtdbairro},
                        {$estuf},
                        {$muncod},
                        {$mtdtelefone01},
                        {$mtdtelefone02},
                        {$mtdtelefone03},
                        {$mtdsite },
                        't',
                        {$mtdfilantropica},
                        {$mtdcomunitaria},
                        {$mtdconfessional},
                        {$mtdautfuncionamento},
                        {$mtdcredenciamento},
                        {$usucpf},
                        {$mtdprivada}
                ) RETURNING mtdid;
            ";

        }
        $mtdid = $db->pegaUm($sql);

    	if( !$mm_mtdid ){
       		$var_controle = "I";
        }else{
       		$var_controle = "U";
        }

        if( $var_controle == 'I' && $mtdid > 0 ){
        	$sql = "select (COALESCE(max(mnmid)+1,1)) as maximo from maismedicomec.mantidamunicipio
					union
					select (COALESCE(max(mnmid)+1,1)) as maximo from maismedicomec.mantidamunicipiocandidata
					order by 1 desc";
        	$mnmid = $db->pegaUm($sql);
            $sql = "INSERT INTO maismedicomec.mantidamunicipiocandidata(mnmid, mntid, mtdid, muncod, mnmprioridade) VALUES ({$mnmid}, {$_SESSION['maismedicomec']['mntid']}, {$mtdid}, {$mantida_muncod}, {$mnmprioridade}) RETURNING mntid;";
            $mntid = $db->executar($sql);
        }else{
            $sql = "UPDATE maismedicomec.mantidamunicipiocandidata SET muncod = {$mantida_muncod}, mnmprioridade = {$mnmprioridade} WHERE mnmid = {$mnmid} RETURNING mntid;";
            $mntid = $db->executar($sql);
        }

        if( $mtdid > 0 ){
            $db->commit();
            $db->sucesso("principal/mantenedora/cad_mantida_candidata", "", "INSCRIÇÃO executada com sucesso!");
        }else{
            $db->sucesso("principal/mantenedora/cad_mantida_candidata", "", "INSCRIÇÃO não realizada, Tente novamnete mais tarde!");
        }
    }



    /**
     * functionName salvarDadosHospital
     *
     * @author Marcus Rocha
     *
     * @param string $dados é o request dos dados "$_POST" do formulário.
     * @return string não há retorno. Só atribuição dos valores a sessão.
     *
     * @version v1
    */
    function salvarDadosHospital($dados){
        global $db;

        #DADOS DO REPRESENTANTE LEGAL.

        $rplid          = $dados['rplid'];
        $rplcpf         = "'".trim( str_replace( '.', '', str_replace( '-', '', $dados['rplcpf'] ) ) )."'";
        $rpldsc         = "'".trim( $dados['rpldsc'] )."'";
        $rplsexo        = "'".trim( $dados['rplsexo'] )."'";
        $rplrg          = "'".trim( $dados['rplrg'] )."'";
        $rplorgaoexprg  = "'".trim( $dados['rplorgaoexprg'] )."'";
        $rplestuf       = "'".trim( $dados['rplestuf'] )."'";
        $rpltelcomercial= "'".trim( $dados['rpltelcomercial'] )."'";
        $rpltelcelular  = "'".trim( $dados['rpltelcelular'] )."'";
        $rplemail       = "'".trim( $dados['rplemail'] )."'";

        #DADOS DA MANTENEDORA.
        $mtdid       = $dados['mtdid'];
        $hspid          = $dados['hspid'];
        $hspcnes          = $dados['hspcnes'];
        $hspcnpj        = "'".trim( str_replace( '.', '', str_replace( '/', '', str_replace( '-', '', $dados['hspcnpj'] ) ) ) )."'";
        $hsprazaosocial = "'".trim( addslashes( $dados['hsprazaosocial'] ) )."'";

        $hspsigla       = $dados['hspsigla']  == '' ? 'NULL' : "'".trim( addslashes( $dados['hspsigla'] ) )."'";
        $hspcep         = $dados['hspcep'] == '' ? 'NULL' : "'".str_replace( '-', '', $dados['hspcep'] )."'";
        $hsplogradouro  = $dados['hsplogradouro'] == '' ? 'NULL' : "'".trim( $dados['hsplogradouro'] )."'";
        $hspcomplemento = $dados['hspcomplementoend'] == '' ? 'NULL' : "'".trim($dados['hspcomplementoend'])."'";
        $hspbairro      = $dados['hspbairro'] == '' ? 'NULL' : "'".trim($dados['hspbairro'])."'";
        $estuf          = $dados['estuf'] == '' ? 'NULL' : "'".trim($dados['estuf'])."'";
        $muncod         = $dados['muncod'] == '' ? 'NULL' : "'".trim($dados['muncod'])."'";
        $hsptelefone01  = $dados['hsptelefone01'] == '' ? 'NULL' : "'".trim($dados['hsptelefone01'])."'";
        $hsptelefone02  = $dados['hsptelefone02'] == '' ? 'NULL' : "'".trim($dados['hsptelefone02'])."'";
        $hsptelefone03  = $dados['hsptelefone03'] == '' ? 'NULL' : "'".trim($dados['hsptelefone03'])."'";
        $hspemail       = $dados['hspemail'] == '' ? 'NULL' : "'".trim($dados['hspemail'])."'";

        $usucpf = "'".$_SESSION['usucpf']."'";

        #DADOS DO REPRESENTANTE LEGAL.
        if( $rplid != '' ){
            $sql = "
                UPDATE  maismedicomec.representantelegal
                    SET rpldsc          = {$rpldsc},
                        rplcpf          = {$rplcpf},
                        rplsexo         = {$rplsexo},
                        rplrg           = {$rplrg},
                        rplorgaoexprg   = {$rplorgaoexprg},
                        estuf           = {$rplestuf},
                        rpltelcomercial = {$rpltelcomercial},
                        rpltelcelular   = {$rpltelcelular},
                        rplemail        = {$rplemail},
                        usucpf          = {$usucpf}
                WHERE rplid = {$rplid} RETURNING rplid;
            ";
            $rplid = $db->pegaUm($sql);
        }else{
            $sql = "
                INSERT INTO maismedicomec.representantelegal(
                        rpldsc,
                        rplcpf,
                        rplsexo,
                        rplrg,
                        rplorgaoexprg,
                        estuf,
                        rpltelcomercial,
                        rpltelcelular,
                        rplemail,
                        usucpf
                    ) VALUES (
                        {$rpldsc},
                        {$rplcpf},
                        {$rplsexo},
                        {$rplrg},
                        {$rplorgaoexprg},
                        {$rplestuf},
                        {$rpltelcomercial},
                        {$rpltelcelular},
                        {$rplemail},
                        {$usucpf}
                ) RETURNING rplid;
            ";
            $rplid = $db->pegaUm($sql);

        }
        if( $hspid != '' ){

                $sql = "
                    UPDATE maismedicomec.hospital
                        SET rplid       = {$rplid},
                            mtdid               = {$mtdid},
                            hspcnpj             = {$hspcnpj},
                            hsprazaosocial      = {$hsprazaosocial},
                            hspsigla            = {$hspsigla},
                            hspcep              = {$hspcep},
                            hsplogradouro       = {$hsplogradouro},
                            hspcomplementoend   = {$hspcomplemento},
                            hspbairro           = {$hspbairro},
                            estuf               = {$estuf},
                            muncod              = {$muncod},
                            hsptelefone01       = {$hsptelefone01},
                            hsptelefone02       = {$hsptelefone02},
                            hsptelefone03       = {$hsptelefone03},
                            hspemail            = {$hspemail},
                            usucpf              = {$usucpf}
                    WHERE hspid = {$hspid} RETURNING hspid;
                ";
                $hspid = $db->pegaUm($sql);

            }else{
                $sql = "
                    INSERT INTO maismedicomec.hospital(
                            rplid,
                            hspcnes,
                            mtdid,
                            hspcnpj,
                            hsprazaosocial,
                            hspsigla,
                            hspcep,
                            hsplogradouro,
                            hspcomplementoend,
                            hspbairro,
                            estuf,
                            muncod,
                            hsptelefone01,
                            hsptelefone02,
                            hsptelefone03,
                            hspemail,
                            usucpf
                        )VALUES(

                            {$rplid},
                            {$hspcnes},
                            {$mtdid},
                            {$hspcnpj},
                            {$hsprazaosocial},
                            {$hspsigla},
                            {$hspcep},
                            {$hsplogradouro},
                            {$hspcomplemento},
                            {$hspbairro},
                            {$estuf},
                            {$muncod},
                            {$hsptelefone01},
                            {$hsptelefone02},
                            {$hsptelefone03},
                            {$hspemail},
                            {$usucpf}
                    ) RETURNING hspid;
                ";
                $hspid = $db->pegaUm($sql);
            }


        if( $hspid > 0 ){
            $db->commit();
            $db->sucesso("principal/mantenedora/cad_hospital", "&acao=A&hspid={$hspid}", "INSCRIÇÃO executada com sucesso!");
        }else{
            $db->sucesso("principal/mantenedora/cad_hospital", "&acao=A&hspid={$hspid}", "INSCRIÇÃO não realizada, Tente novamnete mais tarde!");
        }
    }


    function salvarDadosExpRegulatoria($dados){
        global $db;

        #DADOS DA MANTENEDORA.
        $merid      = $dados['merid'];
        $mntid 		= $_SESSION['maismedicomec']['mntid'];
        $usucpf 	= "'".$_SESSION['usucpf']."'";


        $mermantidaindicada1       = $dados['mermantidaindicada1'] == '' ? 'NULL' : "'".trim($dados['mermantidaindicada1'])."'";
        $mermantidaindicada2       = $dados['mermantidaindicada2'] == '' ? 'NULL' : "'".trim($dados['mermantidaindicada2'])."'";
        $mermantidaindicada3       = $dados['mermantidaindicada3'] == '' ? 'NULL' : "'".trim($dados['mermantidaindicada3'])."'";
        $merinexistsupervinstitucional = $dados['merinexistsupervinstitucional'] == '' ? 'NULL' : "'".trim($dados['merinexistsupervinstitucional'])."'";
        /*
        $mercursomedicina1        = $dados['mercursomedicina1'] == '' ? 'NULL' : "'".trim($dados['mercursomedicina1'])."'";
        $mercursomedicina2        = $dados['mercursomedicina2'] == '' ? 'NULL' : "'".trim($dados['mercursomedicina2'])."'";
        $mercursomedicina3     	  = $dados['mercursomedicina3'] == '' ? 'NULL' : "'".trim($dados['mercursomedicina3'])."'";
        $mercursoareasaude1       = $dados['mercursoareasaude1'] == '' ? 'NULL' : "'".trim($dados['mercursoareasaude1'])."'";
        $mercursoareasaude2       = $dados['mercursoareasaude2'] == '' ? 'NULL' : "'".trim($dados['mercursoareasaude2'])."'";
        $mercursoareasaude3       = $dados['mercursoareasaude3'] == '' ? 'NULL' : "'".trim($dados['mercursoareasaude3'])."'";
        $merprogresidencia1       = $dados['merprogresidencia1'] == '' ? 'NULL' : "'".trim($dados['merprogresidencia1'])."'";
        $merprogresidencia2       = $dados['merprogresidencia2'] == '' ? 'NULL' : "'".trim($dados['merprogresidencia2'])."'";
        $merprogresidencia3       = $dados['merprogresidencia3'] == '' ? 'NULL' : "'".trim($dados['merprogresidencia3'])."'";
        $merprogresidencia4       = $dados['merprogresidencia4'] == '' ? 'NULL' : "'".trim($dados['merprogresidencia4'])."'";
        $merprogresidencia5       = $dados['merprogresidencia5'] == '' ? 'NULL' : "'".trim($dados['merprogresidencia5'])."'";
        $merprogmestdoutorado1       = $dados['merprogmestdoutorado1'] == '' ? 'NULL' : "'".trim($dados['merprogmestdoutorado1'])."'";
        $merprogmestdoutorado2       = $dados['merprogmestdoutorado2'] == '' ? 'NULL' : "'".trim($dados['merprogmestdoutorado2'])."'";
        $merprogmestdoutorado3       = $dados['merprogmestdoutorado3'] == '' ? 'NULL' : "'".trim($dados['merprogmestdoutorado3'])."'";
        $merprogmestdoutorado4       = $dados['merprogmestdoutorado4'] == '' ? 'NULL' : "'".trim($dados['merprogmestdoutorado4'])."'";
        $merprogmestdoutorado5       = $dados['merprogmestdoutorado5'] == '' ? 'NULL' : "'".trim($dados['merprogmestdoutorado5'])."'";
        */

        #DADOS.
        if( !$merid ){
            $sql = "
                INSERT INTO maismedicomec.mantenedoraexpregulatoria(
				            mntid, mermantidaindicada1, mermantidaindicada2, mermantidaindicada3,
				            merinexistsupervinstitucional,
				            usucpf, merdtinclusao, merstatus)
			    VALUES ($mntid,
			    		$mermantidaindicada1,
			    		$mermantidaindicada2,
			    		$mermantidaindicada3,
			            $merinexistsupervinstitucional,
			            $usucpf,
			            now(),
			            'A') RETURNING merid";
            $merid = $db->pegaUm($sql);


        }else{
            $sql = "
                UPDATE  maismedicomec.mantenedoraexpregulatoria
                    SET mermantidaindicada1          	= {$mermantidaindicada1},
                        mermantidaindicada2          	= {$mermantidaindicada2},
                        mermantidaindicada3         	= {$mermantidaindicada3},
                        merinexistsupervinstitucional   = {$merinexistsupervinstitucional},
                        usucpf       					= {$usucpf},
                        merdtinclusao        			= now()
                WHERE merid = {$merid} RETURNING merid;
            ";
            $merid = $db->pegaUm($sql);

        }


        //area medicina
	        $sql = "delete from maismedicomec.mantenedoraareamedica where mntid = $mntid";
			$db->executar($sql);

			if($dados['mercursomedicina1'][0] && $dados['mermantidaindicada1']){
				foreach($dados['mercursomedicina1'] as $v){
					if($v){
						$sql = "INSERT INTO maismedicomec.mantenedoraareamedica(mntid, mtdid, crmid)
							    VALUES ($mntid, ".$dados['mermantidaindicada1'].", $v)";
		            	$db->executar($sql);
		            }
				}
			}
	    	if($dados['mercursomedicina2'][0] && $dados['mermantidaindicada2']){
				foreach($dados['mercursomedicina2'] as $v){
					if($v){
						$sql = "INSERT INTO maismedicomec.mantenedoraareamedica(mntid, mtdid, crmid)
							    VALUES ($mntid, ".$dados['mermantidaindicada2'].", $v)";
		            	$db->executar($sql);
		            }
				}
			}
	    	if($dados['mercursomedicina3'][0] && $dados['mermantidaindicada3']){
				foreach($dados['mercursomedicina3'] as $v){
					if($v){
						$sql = "INSERT INTO maismedicomec.mantenedoraareamedica(mntid, mtdid, crmid)
							    VALUES ($mntid, ".$dados['mermantidaindicada3'].", $v)";
		            	$db->executar($sql);
		            }
				}
			}

        //area saude
	        $sql = "delete from maismedicomec.mantenedoraareasaude where mntid = $mntid";
			$db->executar($sql);

    		if($dados['mercursosaude1'][0] && $dados['mermantidaindicada1']){
				foreach($dados['mercursosaude1'] as $v){
					if($v){
						$sql = "INSERT INTO maismedicomec.mantenedoraareasaude(mntid, mtdid, casid)
							    VALUES ($mntid, ".$dados['mermantidaindicada1'].", $v)";
		            	$db->executar($sql);
		            }
				}
			}
	    	if($dados['mercursosaude2'][0] && $dados['mermantidaindicada2']){
				foreach($dados['mercursosaude2'] as $v){
					if($v){
						$sql = "INSERT INTO maismedicomec.mantenedoraareasaude(mntid, mtdid, casid)
							    VALUES ($mntid, ".$dados['mermantidaindicada2'].", $v)";
		            	$db->executar($sql);
		            }
				}
			}
	    	if($dados['mercursosaude3'][0] && $dados['mermantidaindicada3']){
				foreach($dados['mercursosaude3'] as $v){
					if($v){
						$sql = "INSERT INTO maismedicomec.mantenedoraareasaude(mntid, mtdid, casid)
							    VALUES ($mntid, ".$dados['mermantidaindicada3'].", $v)";
		            	$db->executar($sql);
		            }
				}
			}

        //area programa residencia
	        $sql = "delete from maismedicomec.mantenedoraprogresidencia where mntid = $mntid";
			$db->executar($sql);

    		if($dados['merresidencia1'][0] && $dados['mermantidaindicada1']){
				foreach($dados['merresidencia1'] as $v){
					if($v){
						$sql = "INSERT INTO maismedicomec.mantenedoraprogresidencia(mntid, mtdid, mprdsc)
							    VALUES ($mntid, ".$dados['mermantidaindicada1'].", '$v')";
		            	$db->executar($sql);
		            }
				}
			}
	    	if($dados['merresidencia2'][0] && $dados['mermantidaindicada2']){
				foreach($dados['merresidencia2'] as $v){
					if($v){
						$sql = "INSERT INTO maismedicomec.mantenedoraprogresidencia(mntid, mtdid, mprdsc)
							    VALUES ($mntid, ".$dados['mermantidaindicada2'].", '$v')";
		            	$db->executar($sql);
		            }
				}
			}
	    	if($dados['merresidencia3'][0] && $dados['mermantidaindicada3']){
				foreach($dados['merresidencia3'] as $v){
					if($v){
						$sql = "INSERT INTO maismedicomec.mantenedoraprogresidencia(mntid, mtdid, mprdsc)
							    VALUES ($mntid, ".$dados['mermantidaindicada3'].", '$v')";
		            	$db->executar($sql);
		            }
				}
			}

        //area mestrado/doutorado
	        $sql = "delete from maismedicomec.mantenedoraprogmestdoutorado where mntid = $mntid";
			$db->executar($sql);

    		if($dados['mermestrado1'][0] && $dados['mermantidaindicada1']){
				foreach($dados['mermestrado1'] as $v){
					if($v){
						$sql = "INSERT INTO maismedicomec.mantenedoraprogmestdoutorado(mntid, mtdid, mpmdsc)
							    VALUES ($mntid, ".$dados['mermantidaindicada1'].", '$v')";
		            	$db->executar($sql);
		            }
				}
			}
	    	if($dados['mermestrado2'][0] && $dados['mermantidaindicada2']){
				foreach($dados['mermestrado2'] as $v){
					if($v){
						$sql = "INSERT INTO maismedicomec.mantenedoraprogmestdoutorado(mntid, mtdid, mpmdsc)
							    VALUES ($mntid, ".$dados['mermantidaindicada2'].", '$v')";
		            	$db->executar($sql);
		            }
				}
			}
	    	if($dados['mermestrado3'][0] && $dados['mermantidaindicada3']){
				foreach($dados['mermestrado3'] as $v){
					if($v){
						$sql = "INSERT INTO maismedicomec.mantenedoraprogmestdoutorado(mntid, mtdid, mpmdsc)
							    VALUES ($mntid, ".$dados['mermantidaindicada3'].", '$v')";
		            	$db->executar($sql);
		            }
				}
			}



        if( $merid > 0 ){
            $db->commit();
            $db->sucesso("principal/mantenedora/cad_exp_regulatoria", "&acao=A", "Operação efetuada com sucesso!");
        }else{
            $db->sucesso("principal/mantenedora/cad_exp_regulatoria", "&acao=A", "Erro ao cadastrar, Tente novamnete mais tarde!");
        }
    }


	function salvarDadosExpRegulatoria2($dados){
        global $db;

        #DADOS DA MANTENEDORA.
        $mneid      = $dados['mneid'];
        $mtdid 		= $dados['mtdid'];
        $mntid 		= $_SESSION['maismedicomec']['mntid'];
        $usucpf 	= "'".$_SESSION['usucpf']."'";



        $mnecursomedicina        = $dados['mnecursomedicina'] == '' ? 'NULL' : "'".trim($dados['mnecursomedicina'])."'";

        $mneprogresidencia1       = $dados['mneprogresidencia1'] == '' ? 'NULL' : "'".trim($dados['mneprogresidencia1'])."'";
        $mneprogresidencia2       = $dados['mneprogresidencia2'] == '' ? 'NULL' : "'".trim($dados['mneprogresidencia2'])."'";
        $mneprogresidencia3       = $dados['mneprogresidencia3'] == '' ? 'NULL' : "'".trim($dados['mneprogresidencia3'])."'";
        $mneprogresidencia4       = $dados['mneprogresidencia4'] == '' ? 'NULL' : "'".trim($dados['mneprogresidencia4'])."'";
        $mneprogresidencia5       = $dados['mneprogresidencia5'] == '' ? 'NULL' : "'".trim($dados['mneprogresidencia5'])."'";

        $mneprogmestdoutorado1       = $dados['mneprogmestdoutorado1'] == '' ? 'NULL' : "'".trim($dados['mneprogmestdoutorado1'])."'";
        $mneprogmestdoutorado2       = $dados['mneprogmestdoutorado2'] == '' ? 'NULL' : "'".trim($dados['mneprogmestdoutorado2'])."'";
        $mneprogmestdoutorado3       = $dados['mneprogmestdoutorado3'] == '' ? 'NULL' : "'".trim($dados['mneprogmestdoutorado3'])."'";
        $mneprogmestdoutorado4       = $dados['mneprogmestdoutorado4'] == '' ? 'NULL' : "'".trim($dados['mneprogmestdoutorado4'])."'";
        $mneprogmestdoutorado5       = $dados['mneprogmestdoutorado5'] == '' ? 'NULL' : "'".trim($dados['mneprogmestdoutorado5'])."'";

        $mnecursoareasaude1       = $dados['mnecursoareasaude1'] == '' ? 'NULL' : "'".trim($dados['mnecursoareasaude1'])."'";
        $mnecursoareasaude2       = $dados['mnecursoareasaude2'] == '' ? 'NULL' : "'".trim($dados['mnecursoareasaude2'])."'";
        $mnecursoareasaude3       = $dados['mnecursoareasaude3'] == '' ? 'NULL' : "'".trim($dados['mnecursoareasaude3'])."'";

        $mneaderentefies = $dados['mneaderentefies'] == '' ? 'NULL' : "'".trim($dados['mneaderentefies'])."'";
        $mneaderenteprouni = $dados['mneaderenteprouni'] == '' ? 'NULL' : "'".trim($dados['mneaderenteprouni'])."'";


        #DADOS.
        if( !$mneid ){
            $sql = "
                INSERT INTO maismedicomec.mantidaexpregulatoria(
				            mntid, mtdid, mnecursomedicina, mneprogresidencia1, mneprogresidencia2, mneprogresidencia3, mneprogresidencia4,
				            mneprogresidencia5, mneprogmestdoutorado1, mneprogmestdoutorado2,
				            mneprogmestdoutorado3, mneprogmestdoutorado4, mneprogmestdoutorado5,
				            mnecursoareasaude1, mnecursoareasaude2, mnecursoareasaude3,
				            mneaderentefies, mneaderenteprouni,
				            usucpf, mnedtinclusao, mnestatus)
			    VALUES ($mntid,
            			$mtdid,
			            $mnecursomedicina,
			            $mneprogresidencia1,
			            $mneprogresidencia2,
			            $mneprogresidencia3,
			            $mneprogresidencia4,
			            $mneprogresidencia5,
			            $mneprogmestdoutorado1,
			            $mneprogmestdoutorado2,
			            $mneprogmestdoutorado3,
			            $mneprogmestdoutorado4,
			            $mneprogmestdoutorado5,
			            $mnecursoareasaude1,
			            $mnecursoareasaude2,
			            $mnecursoareasaude3,
			            $mneaderentefies,
			            $mneaderenteprouni,
			            $usucpf,
			            now(),
			            'A') RETURNING mneid";
            $mneid = $db->pegaUm($sql);


        }else{
            $sql = "
                UPDATE  maismedicomec.mantidaexpregulatoria
                    SET mtdid          					= {$mtdid},
                        mnecursomedicina   				= {$mnecursomedicina},
                        mneprogresidencia1        		= {$mneprogresidencia1},
                        mneprogresidencia2        		= {$mneprogresidencia2},
                        mneprogresidencia3        		= {$mneprogresidencia3},
                        mneprogresidencia4        		= {$mneprogresidencia4},
                        mneprogresidencia5        		= {$mneprogresidencia5},
                        mneprogmestdoutorado1        	= {$mneprogmestdoutorado1},
                        mneprogmestdoutorado2        	= {$mneprogmestdoutorado2},
                        mneprogmestdoutorado3        	= {$mneprogmestdoutorado3},
                        mneprogmestdoutorado4        	= {$mneprogmestdoutorado4},
                        mneprogmestdoutorado5        	= {$mneprogmestdoutorado5},
                        mnecursoareasaude1   			= {$mnecursoareasaude1},
                        mnecursoareasaude2        		= {$mnecursoareasaude2},
                        mnecursoareasaude3        		= {$mnecursoareasaude3},
                        mneaderentefies   				= {$mneaderentefies},
                        mneaderenteprouni   			= {$mneaderenteprouni},
                        usucpf       					= {$usucpf},
                        mnedtinclusao        			= now()
                WHERE mneid = {$mneid} RETURNING mneid;
            ";
            $mneid = $db->pegaUm($sql);

        }

        if( $mneid > 0 ){
            $db->commit();
            $db->sucesso("principal/mantenedora/cad_exp_regulatoria", "&acao=A&aba=mantida", "Operação efetuada com sucesso!");
        }else{
            $db->sucesso("principal/mantenedora/cad_exp_regulatoria", "&acao=A&aba=mantida", "Erro ao cadastrar, Tente novamnete mais tarde!");
        }
    }

    function wfVerificaAnaliseMec(){
    	global $db;

    	$mntid = $_SESSION['maismedicomec']['mntid'];

    	if(!$mntid) return 'Sessão expirou';

    	//verifica matida indicada
    	/*
		$sql = "select count(mntid) from maismedicomec.mantidamunicipio where mntid = " . $mntid;
		$total = $db->pegaUm($sql);
		if($total == 0) return 'Aba Mantida indicada: Favor cadastrar uma instituição.';
		*/

		//verifica Corpo dirigente da Mantida
		/*
		$sql = "select count(c.cpdid) from maismedicomec.corpodirigente AS c
       			inner join maismedicomec.dirigentemantida AS d ON d.cpdid = c.cpdid
				inner join maismedicomec.mantidamunicipio m on m.mtdid = d.mtdid
				where c.cpdstatus='A' and m.mntid =  " . $mntid;
		*/
    	$sql = "select count(c.cpdid) from maismedicomec.corpodirigente AS c
       			inner join maismedicomec.dirigentemantida AS d ON d.cpdid = c.cpdid
				left join maismedicomec.mantidamunicipio m on m.mtdid = d.mtdid
				left join maismedicomec.mantidamunicipiocandidata m2 on m2.mtdid = d.mtdid
				where c.cpdstatus='A' and (m.mntid =  " . $mntid." or m2.mntid =  " . $mntid.")";
		$total = $db->pegaUm($sql);
		if($total == 0) return 'Aba Corpo dirigente da Mantida: Favor cadastrar um dirigente.';

		//verifica Corpo dirigente da Mantida
		/*
		$sql = "select count(mntid) from maismedicomec.mantidamunicipiocandidata where mntid = " . $mntid;
		$total = $db->pegaUm($sql);
		if($total == 0) return 'Aba Credenciar Nova mantida: Favor cadastrar uma instituição.';
		*/

		//verifica Experiência Regulatória
		/*
		$sql = "select count(mntid) from maismedicomec.mantenedoraexpregulatoria where mntid = " . $mntid;
		$total = $db->pegaUm($sql);
		if($total == 0) return 'Aba Experiência Regulatória: Favor cadastrar as instituições, cursos e programas.';
		*/

		//verifica matida indicada = portifolio matida indicada
		//$sql = "select count(mntid) from maismedicomec.mantidamunicipio where mntid = " . $mntid;
		$sql = "select count(mtdid) from maismedicomec.mantida where mtdid in (select mtdid from maismedicomec.mantidamunicipio where mntid = {$mntid} group by mtdid)";
		$total = $db->pegaUm($sql);

		$sql = "select count(mntid) from maismedicomec.mantidaexpregulatoria where mnestatus='A' and mntid = " . $mntid;
		$total2 = $db->pegaUm($sql);
		if($total != $total2) return 'Aba Portfólio de Mantidas: para cada Mantida Indicada inscrita, é obrigatório o preenchimento da mesma nas Abas Portfólio de Mantidas/Mantida Indicada.';


		return true;
    }

    function verificaMantenedora(){
    	global $db;

    	$sql = "select count(mntid) as total from maismedicomec.mantenedora where mntid = {$_SESSION['maismedicomec']['mntid']}";
    	$total = $db->pegaUm($sql);
		if($total == 0){
			 echo '<script>
			 		alert("É necessário atualizar os dados da aba mantenedora para depois acessar esta aba.");
			 		location.href="maismedicomec.php?modulo=principal/mantenedora/cad_mantenedora&acao=A";
			 		</script>';
			 die();
		}
    }
?>