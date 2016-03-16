<?php

require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/classes/dateTime.inc";
require_once "EstruturaCarreiraRemuneracao.class.inc";
require_once "MunicipiosAlteracaoPeriodoTecRedeae.class.inc";

/**
 * Classe de Indicação de Avaliador Educacional
 *
 * @author Victor Martins Machado <VictorMachado@mec.gov.br>
 *
 * Objeto de Modelo de Avaliador Educacional
 */
$data = new Data();

class RelatorioAvaliadorRedeae extends Modelo{

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sase.relatorioavaliadorredeae";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("ravid");

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    public $arAtributos = array(
        'ravid' => null,
        'aveid' => null,
        'ratperiodo' => null,
        'ratdata1periodo' => null,
        'ratdata2periodo' => null,
        'usucpf' => null,
        'ravinfcomplementar' => null,
        'ravinfcomplqdo1' => null,
        'ravinfcomplqdo2' => null,
        'ravinfcomplqdo3' => null,
        'ravinfcomplqdo4' => null,
        'ravinfcomplqdo5' => null,
        'ravinfcomplqdo6' => null,
        'docid' => null,
        'pflcod' => null
    );

    /**
     * Atributos String da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributosStr = array(
        'usucpf'
    );

    /**
     * @name $arCampos
     * @var array
     * @access protected
     * Campos da Tabela
     */
    protected $arCampos = array(
        'ravid',
        'aveid',
        'ratperiodo',
        'ratdata1periodo',
        'ratdata2periodo',
        'usucpf',
        'ravinfcomplementar',
        'ravinfcomplqdo1',
        'ravinfcomplqdo2',
        'ravinfcomplqdo3',
        'ravinfcomplqdo4',
        'ravinfcomplqdo5',
        'ravinfcomplqdo6',
        'docid',
        'pflcod'
    );

    /**
     * Campos Obrigatórios da Tabela
     * @name $arCampos
     * @var array
     * @access protected
     */
    protected $arAtributosObrigatorios = array(
        'ravid',
        'aveid',
        'ratperiodo',
        'ratdata1periodo',
        'ratdata2periodo',
        'usucpf',
        'ravinfcomplementar',
        'pflcod'
    );

    /**
     * Menságens gerais
     * @name $msg
     * @var string
     * @access public
     */
    public $msg;

    protected $prepEstado = array(
        'AC' => 'do',
        'AL' => 'de',
        'AP' => 'do',
        'AM' => 'do',
        'BA' => 'da',
        'CE' => 'do',
        'DF' => 'do',
        'ES' => 'do',
        'GO' => 'de',
        'MA' => 'do',
        'MT' => 'de',
        'MS' => 'de',
        'MG' => 'de',
        'PA' => 'do',
        'PB' => 'da',
        'PR' => 'do',
        'PE' => 'de',
        'PI' => 'do',
        'RJ' => 'do',
        'RN' => 'do',
        'RS' => 'do',
        'RO' => 'de',
        'RR' => 'de',
        'SC' => 'de',
        'SP' => 'de',
        'SE' => 'de',
        'TO' => 'do'
    );


    protected $marIn = array();

    public $avaliador;

    /**
     * Função que verifica a permissão do usuário
     * @return int:
     *      1 - Permissão de Técnico
     *      2 - Permissão de Supervisor
     *      99 - Permissão de Super Usuário
     */
    public function retornaPermissao(){
        $pfls = arrayPerfil();
        $ret = 1;

        if (in_array(PFLCOD_SASE_TECNICO, $pfls)){
            $ret = 1; // Permissão de técnico
        }
        if (in_array(PFLCOD_SASE_SUPERVISOR, $pfls)){
            $ret = 2; // Permissão de supervisor
        }
        if (in_array(PFLCOD_SASE_SUPER_USUARIO, $pfls) || in_array(PFLCOD_SASE_ADMINISTRADOR, $pfls)){
            $ret = 99; // Permissão de super usuario
        }

        return $ret;
    }

    public function validaRelatorio(){
        global $db;
        $usucpf = $_REQUEST['usucpf'];
        $pflcod = $_REQUEST['pflcod'];
        $dataIni = $_POST['ratdata1periodo'];
        $dataFim = $_POST['ratdata2periodo'];
        if ($usucpf != '') {
            if ($dataIni != '' && $dataFim != '' && $dataIni != '//' && $dataFim != '//') {
                //ver($dataIni, $dataFim, d);
                $sql = "select count(*) from sase.relatorioavaliadorredeae where usucpf = '{$usucpf}' and ratdata1periodo = '{$dataIni}' and ratdata2periodo = '{$dataFim}' and pflcod = {$pflcod}";
                $con = $db->pegaUm($sql);
                if ($con > 0) {
                    $this->msg = 'Ja existe um relatorio para este Avaliador Educacional neste período.';
                    return false;
                }
            } else {
                $this->msg = 'Informe o período em data.';
                return false;
            }
        } else {
            $this->msg = 'Usuário invalido.';
            return false;
        }
        return true;
    }

    public function apagaRelatorio(){
        $ravid = $this->arAtributos['ravid'];
        $sql = "delete from sase.ativdesenvavaliadoresredeae where ravid = {$ravid};";
        $this->executar($sql);

        if (!$this->commit()){
            $this->msg = 'Erro ao apagar as Atividades Desenvolvidas.';
            return false;
        }

        $sql = "delete from sase.municipiosseminftecredeae where marid in (select marid from sase.municipiosassistidosredeae where ravid = {$ravid})";
        $this->executar($sql);

        if (!$this->commit()){
            $this->msg = 'Erro ao apagar os Municípios sem Informação ou Sem Comissão Instituída';
            return false;
        }

        $sql = "delete from sase.municipiossemalteracaoperiodotecredeae where marid in (select marid from sase.municipiosassistidosredeae where ravid = {$ravid})";
        $this->executar($sql);

        if (!$this->commit()){
            $this->msg = 'Erro ao apagar os Municípios sem Alteração no Período.';
            return false;
        }

        $sql = "delete from sase.sitatualmunicipiotecredeae where marid in (select marid from sase.municipiosassistidosredeae where ravid = {$ravid})";
        $this->executar($sql);

        if (!$this->commit()){
            $this->msg = 'Erro ao apagar os Resultados Consolidados.';
            return false;
        }

        $sql = "delete from sase.municipiossemacoesdesenvstecredeaee where marid in (select marid from sase.municipiosassistidosredeae where ravid = {$ravid})";
        $this->executar($sql);

        if (!$this->commit()){
            $this->msg = 'Erro ao apagar os Municípios sem Ações Desenvolvidas.';
            return false;
        }

        $sql = "delete from sase.municipiosalteracaoperiodotecredeae where marid in (select marid from sase.municipiosassistidosredeae where ravid = {$ravid})";
        $this->executar($sql);

        if (!$this->commit()){
            $this->msg = 'Erro ao apagar os Municípios com alteração na etapa.';
            return false;
        }

        $sql = "delete from sase.municipiosassistidosredeae where ravid = {$ravid}";
        $this->executar($sql);

        if (!$this->commit()){
            $this->msg = 'Erro ao apagar os Munisípios Assistidos.';
            return false;
        }

        $sql = "delete from sase.tecnicosupervisionadosredeae where ravid = {$ravid}";
        $this->executar($sql);

        if (!$this->commit()){
            $this->msg = 'Erro ao apagar os Técnicos Relacionados.';
            return false;
        }

        $sql = "delete from sase.relatorioavaliadorredeae where ravid = {$ravid}";
        $this->executar($sql);

        if (!$this->commit()){
            $this->msg = 'Erro ao apagar o Relatório';
            return false;
        }

        return true;
    }

    public function salvarRelatorio(){
        global $db;

        $usucpf = $_REQUEST['usucpf'];
        $aveid = $db->pegaUm("select aveid from sase.avaliadoreducacional where avenumcpf = '{$usucpf}'", 'aveid');
        $pflcod = $_REQUEST['pflcod'];

        if ($pflcod == ''){
            $this->msg = 'Informe o perfil do usuário.';
            return false;
        }

        if ($aveid != '') {
            $ratperiodo = $_POST['ratperiodo'];
            $ratdata1periodo = $_POST['ratdata1periodo'];
            $ratdata2periodo = $_POST['ratdata2periodo'];
            $ravinfcomplementar = $_POST['ravinfcomplementar'];

            // Salva as informações do relatório

            $sql = "insert into sase.relatorioavaliadorredeae (aveid, ratperiodo, ratdata1periodo, ratdata2periodo, usucpf, ravinfcomplementar, pflcod) values ({$aveid}, {$ratperiodo}, '{$ratdata1periodo}', '{$ratdata2periodo}', '{$usucpf}', '{$ravinfcomplementar}', {$pflcod}) returning ravid";

            //ver($sql, d);
            $ravid = $this->pegaUm($sql);

            if (!$this->commit()) {
                $this->msg = "Falha ao cadastrar o relatorio.";
                return false;
            }

            $this->carregarPorId($ravid);

            if ($ravid){
                switch ($pflcod){
                    case PFLCOD_SASE_TECNICO_DIVAPE:
                    case PFLCOD_SASE_TECNICO:
                        $this->salvaMunicipiosAssistidos($pflcod);
                        break;

                    case PFLCOD_SASE_SUPERVISOR:
                        $this->salvaMunicipiosAssistidos($pflcod);
                        $this->salvaMunicipiosAssistidosPorTecnicos();
                        break;

                    case PFLCOD_SASE_SUPERVISOR_GERAL:
                        break;

                    default:
                        $this->salvaMunicipiosAssistidos($pflcod);
                        break;
                }
            }

            return $ravid;
        } else {
            $this->msg = "Usuário não está cadastrado como Avaliador Educacional.";
            return false;
        }

        // Fim das informações do relatório
    }

    public function salvaMunicipiosAssistidos($pflcod = null) {
        global $db;
        // Salva os municípios vinculados ao técnico

        if($pflcod == null){
            $pflcod = PFLCOD_SASE_TECNICO;
        }

        $usucpf = $_REQUEST['usucpf'];
        $ravid = $this->arAtributos['ravid'];
        $pflcod = $this->arAtributos['pflcod'];

        $sql = "select count(*) cont from sase.municipiosassistidosredeae where ravid = {$ravid}";
        $res = $db->pegaUm($sql);

        switch($pflcod){
            case PFLCOD_SASE_TECNICO:
                if ($res > 0){
                    $sql = "select
                        mar.marid,
                        mun.mundescricao,
                        ur.rpuid
                    from sase.municipiosassistidosredeae mar
                    inner join sase.assessoramento ass on mar.assid = ass.assid
                    inner join territorios.municipio mun on mun.muncod = ass.muncod
                    left join sase.usuarioresponsabilidade ur on ur.muncod = mun.muncod and ur.usucpf = '{$usucpf}' and ur.rpustatus = 'A'
                    where ravid = {$ravid}
                    and ur.rpuid is null
                    order by mun.mundescricao";
                    $res = $db->carregar($sql);
                    $m = array();
                    if (is_array($res)) {
                        foreach ($res as $r) {
                            $m[] = $r['marid'];
                        }
                        $this->apagaMunicipioAssistido($m);
                    }

                    $sql = "insert into sase.municipiosassistidosredeae (ravid, assid)
                    select
                        {$ravid} as ravid,
                        ass.assid
                    from sase.usuarioresponsabilidade rpu
                    inner join territorios.municipio mun on mun.muncod = rpu.muncod
                    inner join sase.assessoramento ass on rpu.muncod = ass.muncod
                    left join sase.municipiosassistidosredeae mar on mar.assid = ass.assid and mar.ravid = {$ravid}
                    where rpu.rpustatus = 'A' and rpu.usucpf = '{$usucpf}' and rpu.rpustatus = 'A' and mar.marid is null";

                } else {
                    $sql = "insert into sase.municipiosassistidosredeae (ravid, assid)
                    select
                        {$ravid} as ravid,
                        ass.assid
                    from sase.usuarioresponsabilidade rpu
                    inner join sase.assessoramento ass on ass.muncod = rpu.muncod
                    inner join territorios.municipio mun on rpu.muncod = mun.muncod
                    where pflcod = {$pflcod} and rpustatus = 'A' and rpu.usucpf = '{$usucpf}'";
                }
                break;

            case PFLCOD_SASE_TECNICO_DIVAPE:
                if ($res > 0){
                    $sql = "select
                        mar.marid,
                        mun.mundescricao,
                        ur.rpuid
                    from sase.municipiosassistidosredeae mar
                    inner join sase.planocarreiraprofessor pcp on mar.pcpid = pcp.pcpid
                    inner join territorios.municipio mun on mun.muncod = pcp.muncod
                    left join sase.usuarioresponsabilidade ur on ur.muncod = mun.muncod and ur.usucpf = '{$usucpf}' and ur.rpustatus = 'A'
                    where ravid = {$ravid}
                    and ur.rpuid is null
                    order by mun.mundescricao";
                    $res = $db->carregar($sql);
                    $m = array();
                    if (is_array($res)) {
                        foreach ($res as $r) {
                            $m[] = $r['marid'];
                        }
                        $this->apagaMunicipioAssistido($m);
                    }

                    $sql = "insert into sase.municipiosassistidosredeae (ravid, pcpid)
                            select
                                {$ravid} as ravid,
                                pcp.pcpid
                            from sase.usuarioresponsabilidade rpu
                            inner join territorios.municipio mun on mun.muncod = rpu.muncod
                            inner join sase.planocarreiraprofessor pcp on pcp.muncod = rpu.muncod
                            left join sase.municipiosassistidosredeae mar on mar.pcpid = pcp.pcpid and mar.ravid = {$ravid}
                            where rpu.rpustatus = 'A'
                            and rpu.usucpf = '{$usucpf}'
                            and rpu.rpustatus = 'A'
                            and mar.marid is null";

                } else {
                    $sql = "insert into sase.municipiosassistidosredeae (ravid, pcpid)
                            select
                                {$ravid} as ravid,
                                pcp.pcpid
                            from sase.usuarioresponsabilidade rpu
                            inner join sase.planocarreiraprofessor pcp on pcp.muncod = rpu.muncod
                            inner join territorios.municipio mun on rpu.muncod = mun.muncod
                            where pflcod = {$pflcod} and rpustatus = 'A' and rpu.usucpf = '{$usucpf}'";
                }
                break;

            case PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE:
                if ($res > 0){
                    $sql = "select
                        mar.marid,
                        mun.mundescricao,
                        ur.rpuid
                    from sase.municipiosassistidosredeae mar
                    inner join sase.planocarreiraprofessor pcp on mar.pcpid = pcp.pcpid
                    inner join territorios.municipio mun on mun.muncod = pcp.muncod
                    left join sase.usuarioresponsabilidade ur on ur.muncod = mun.muncod and ur.usucpf = '{$usucpf}' and ur.rpustatus = 'A'
                    where ravid = {$ravid}
                    and ur.rpuid is null
                    order by mun.mundescricao";
                    $res = $db->carregar($sql);
                    $m = array();
                    if (is_array($res)) {
                        foreach ($res as $r) {
                            $m[] = $r['marid'];
                        }
                        $this->apagaMunicipioAssistido($m);
                    }

                    $sql = "insert into sase.municipiosassistidosredeae (ravid, pcpid)
                            select
                                {$ravid} as ravid,
                                pcp.pcpid,
                                mun.mundescricao
                            from sase.usuarioresponsabilidade rpu
                            inner join territorios.municipio mun on mun.estuf = rpu.estuf
                            inner join sase.planocarreiraprofessor pcp on pcp.muncod = mun.muncod
                            left join sase.municipiosassistidosredeae mar on mar.pcpid = pcp.pcpid and mar.ravid = {$ravid}
                            where rpu.rpustatus = 'A'
                            and rpu.usucpf = '{$usucpf}'
                            and rpu.rpustatus = 'A'
                            and mar.marid is null";

                } else {
                    $sql = "insert into sase.municipiosassistidosredeae (ravid, pcpid)
                            select
                                {$ravid} as ravid,
                                pcp.pcpid
                            from sase.usuarioresponsabilidade rpu
                            inner join sase.planocarreiraprofessor pcp on pcp.estuf = rpu.estuf
                            inner join territorios.municipio mun on pcp.muncod = mun.muncod
                            where pflcod = {$pflcod} and rpustatus = 'A' and rpu.usucpf = '{$usucpf}'";
                }
                BREAK;
        }

        //ver($sql, d);
        $this->executar($sql);

        if (!$this->commit()){
            return false;
        }

        return true;

        // Fim dos municípios vinculados ao técnico
    }

    public function apagaMunicipioAssistido($marid = array()){
        global $db;

        $m = implode(",", $marid);

        $sql = "delete from sase.sitatualmunicipiotecredeae where marid in ({$m})";
        $this->executar($sql);

        if (!$this->commit()){
            return false;
        }
        $sql = "delete from sase.municipiossemalteracaoperiodotecredeae where marid in ({$m})";
        $this->executar($sql);

        if (!$this->commit()){
            return false;
        }
        $sql = "delete from sase.municipiossemacoesdesenvstecredeaee where marid in ({$m})";
        $this->executar($sql);

        if (!$this->commit()){
            return false;
        }
        $sql = "delete from sase.municipiosseminftecredeae where marid in ({$m})";
        $this->executar($sql);

        if (!$this->commit()){
            return false;
        }
        $sql = "delete from sase.municipiosassistidosredeae where marid in ({$m})";
        $this->executar($sql);

        if (!$this->commit()){
            return false;
        }
        return true;
    }

    public function salvaMunicipiosAssistidosPorTecnicos(){
        $usucpf = $_REQUEST['usucpf'];
        $ravid = $this->arAtributos['ravid'];
        $pflcod = $this->arAtributos['pflcod'];

        $sql = "insert into sase.tecnicosupervisionadosredeae (ravid, cpftecnico, muncod)
                    select distinct
                        {$ravid} as ravid,
                        cpftecnico,
                        muncod
                    from sase.usuarioresponsabilidade
                    where rpustatus = 'A'
                    and pflcod = {$pflcod}
                    and usucpf = '{$usucpf}'";

        $this->executar($sql);

        if (!$this->commit()){
            return false;
        }

        return true;
    }

    public function apagaRegQuadro6($msaid){
        if (!empty($msaid)) {
            $sql = "delete from sase.municipiossemacoesdesenvstecredeaee where msaid = {$msaid}";
            $this->executar($sql);

            if (!$this->commit()){
                return false;
            }
        }
        return true;
    }

    public function carregaMunicipiosAssistidos($pflcod = null){
        global $db;

        $ravid = $this->arAtributos['ravid'];

        $sql = "";

        switch ($pflcod){
            case PFLCOD_SASE_TECNICO_DIVAPE:
            case PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE:
                $sql = "select
					mar.marid,
					pcp.muncod
				from sase.municipiosassistidosredeae mar
				inner join sase.planocarreiraprofessor pcp on pcp.pcpid = mar.pcpid
				where mar.ravid = {$ravid}";
                break;

            default:
                $sql = "select
					mar.marid,
					ass.muncod
				from sase.municipiosassistidosredeae mar
				inner join sase.assessoramento ass on mar.assid = ass.assid
				where mar.ravid = {$ravid}";
                break;
        }

        $res = $db->carregar($sql);

        $marIn = array();
        if (is_array($res)){
            foreach ($res as $r){
                $marIn[$r['muncod']] = $r['marid'];
            }
        } else {
            if ($this->salvaMunicipiosAssistidos()){
                $marIn = $this->carregaMunicipiosAssistidos();
            }
        }

        return $marIn;
    }

    public function salvaComplemento($campo, $valor){
        if (!empty($campo)){
            $this->arAtributos[$campo] = $valor;
            if ($this->alterar()){
                return $this->commit();
            }
        } else {
            return false;
        }
    }

    public function salvaAtividades(){
        global $data;
        // Salva as informações referentes ao QUADRO 1

        $ravid = $this->arAtributos['ravid'];
        //ver($_POST['avaid'], d);

        if ($_POST['ravinfcomplqdo1']){

            if (strlen($_POST['ravinfcomplqdo1']) > 1000 ) {
                $this->msg = "O campo comentários gerais deve conter até 1000 caracteres.";
                return false;
            }

            if (!$this->salvaComplemento('ravinfcomplqdo1', $_POST['ravinfcomplqdo1'])){
                $this->msg = 'Erro ao salvar os comentarios gerais.';
                return false;
            }
        }

        if (is_array($_POST['reg'])) {
            $i = 0;
            foreach ($_POST['reg'] as $reg) {
                $avaatores          = $_POST['avaatores'][$i];
                $adaid              = $_POST['adaid'][$i];
                $avadescoutrostipos = $_POST['avadescoutrostipos'][$i];
                $avadetalhes        = $_POST['avadetalhes'][$i];
                $avadata1periodo    = $_POST['avadata1periodo'][$i];
                $avadata2periodo    = $_POST['avadata2periodo'][$i];
                $avaresponsavel     = $_POST['avaresponsavel'][$i];
                $avaobservacoes     = '';
                $avaid = $_POST['avaid'][$i];

                if ($avadata1periodo != '' && $avadata2periodo != '') {
                    if (!validaData($avadata1periodo) || !validaData($avadata2periodo)) {
                        $this->msg = "Período de realização, da atividade executada, invalido.";
                        return false;
                    }
                }

                if (strlen($avaatores) > 200) {
                    $this->msg = "O campo \'Atores envolvidos\' deve ter, no máximo, 200 caracteres.";
                    return false;
                }

                if (strlen($avaresponsavel) > 50){
                    $this->msg = "O campo \'Responsável\' deve ter, no máximo, 50 caracteres.";
                    return false;
                }
                if (strlen($avadetalhes) > 1000){
                    $this->msg = "O campo \'Especificar\' deve ter, no máximo, 1000 caracteres.";
                    //ver(strlen($avadetalhes), $this->msg, d);
                    return false;
                }

                if ($adaid != '') {
                    if ((strlen($avadata1periodo) < 10 || strlen($avadata2periodo) < 10) && ($avadata1periodo != '' || $avadata2periodo != '')) {
                        $this->msg = 'Período inválido.';
                        return false;
                    } else {
                        if ($avadata1periodo != '' && $avadata2periodo != '') {
                            if (!empty($avaid)) {
                                $sql = "update sase.ativdesenvavaliadoresredeae set
							avaatores = '{$avaatores}',
							avadata1periodo = '{$avadata1periodo}',
							avadata2periodo = '{$avadata2periodo}',
							avaresponsavel = '{$avaresponsavel}',
							avaobservacoes = '{$avaobservacoes}',
							avadetalhes = '{$avadetalhes}'
						where avaid = {$avaid}";
                            } else {
                                $sql = "insert into sase.ativdesenvavaliadoresredeae (ravid, adaid, avaatores, avadata1periodo, avadata2periodo, avaresponsavel, avadetalhes) values ({$ravid}, {$adaid}, '{$avaatores}', '{$avadata1periodo}', '{$avadata2periodo}', '{$avaresponsavel}', '{$avadetalhes}')";
                            }
                            //ver($sql, d);
                            $this->executar($sql);

                            if (!$this->commit()) {
                                return false;
                            }
                        }
                    }
                } else {
                    if ((strlen($avadata1periodo) < 10 || strlen($avadata2periodo) < 10) && ($avadata1periodo != '' || $avadata2periodo != '')) {
                        $this->msg = 'Período inválido.';
                        return false;
                    } else {
                        if ($avadata1periodo != '' && $avadata2periodo != '') {
                            if (!empty($avaid)) {
                                $sql = "update sase.ativdesenvavaliadoresredeae set
							avaatores = '{$avaatores}',
							avadata1periodo = '{$avadata1periodo}',
							avadescoutrostipos = '{$avadescoutrostipos}',
							avadata2periodo = '{$avadata2periodo}',
							avaresponsavel = '{$avaresponsavel}',
							avaobservacoes = '{$avaobservacoes}',
							avadetalhes = '{$avadetalhes}'
						where avaid = {$avaid}";
                            } else {
                                $sql = "insert into sase.ativdesenvavaliadoresredeae (ravid, avaatores, avadata1periodo, avadata2periodo, avaresponsavel, avadetalhes, avadescoutrostipos) values ({$ravid}, '{$avaatores}', '{$avadata1periodo}', '{$avadata2periodo}', '{$avaresponsavel}', '{$avadetalhes}', '{$avadescoutrostipos}')";
                            }
                            //ver($sql, d);
                            $this->executar($sql);

                            if (!$this->commit()) {
                                return false;
                            }
                        }
                    }
                }
                $i++;
            }
        }

        return true;

        // Fim das informações referentes ao QUADRO 1

    }

    public function salvaResultadosConsolidades(){

        // Salva as informações referentes ao QUADRO 2

        $i = 0;
        $sqlRes = "";
        //ver($_POST['stmid'], d);
        $pflcod = isset($_POST['pflcod']) ? $_POST['pflcod'] : null;
        $marIn = $this->carregaMunicipiosAssistidos($pflcod);

        if (is_array($_POST['muncodq2'])) {

            foreach ($_POST['muncodq2'] as $m) {

                $stmid = $_POST['stmidq2'][$i];
                if ($stmid) {
                    $marid = $_POST['maridq2'][$i];
                } else {
                    $marid = $marIn[$m];
                }

                $esdid = $_POST['esdidq2'][$i];
                $stmobservacoes = $_POST['stmobservacoes'][$i];

                if (strlen($stmobservacoes) > 200){
                    $this->msg = "Campo \'Observação\', do quadro de Resultados Consolidados Alcançados, deve conter até 200 caracteres.";
                    return false;
                }

                if (!empty($stmid)) {
                    $sqlRes .= " update sase.sitatualmunicipiotecredeae set
							stmobservacoes = '{$stmobservacoes}'
						where stmid = {$stmid}; ";
                } else {
                    if($marid && $esdid) {
                        $sqlRes .= " insert into sase.sitatualmunicipiotecredeae (marid, esdid, stmobservacoes) values ({$marid}, {$esdid}, '{$stmobservacoes}'); ";
                    }
                }

                $i++;
            }
            if($sqlRes){
                $this->executar($sqlRes);

                if (!$this->commit()) {
                    return false;
                }
            }

        }

        return true;
        // Fim das informações referentes ao QUADRO 2

    }

    public function salvaMunicipiosComAlteracao(){

        // Salva as informações referentes ao QUADRO 3

        $i = 0;
        $marIn = $this->carregaMunicipiosAssistidos();

        if ($_POST['ravinfcomplqdo3']){
            if (!$this->salvaComplemento('ravinfcomplqdo3', $_POST['ravinfcomplqdo3'])){
                $this->msg = 'Erro ao salvar os comentários gerais.';
                return false;
            }
        }

        if (is_array($_POST['muncodq3'])) {
            foreach ($_POST['muncodq3'] as $m) {

                $marid = $marIn[$m];
                $origem = $_POST['esdorigemq3'][$i];
                $destino = $_POST['esddestinoq3'][$i];
                $matid = $_POST['matidq3'][$i];

                if (empty($matid)) {
                    $sql = "insert into sase.municipiosalteracaoperiodotecredeae (marid, esdid_ant, esdid_atual) values ({$marid}, {$origem}, {$destino})";

                    $this->executar($sql);

                    if (!$this->commit()) {
                        return false;
                    }
                }

                $i++;
            }
        }

        return true;

        // Fim das informações referentes ao QUADRO 3

    }

    public function salvarMunicipiosSemAlteracao(){
        global $db;
        // Salva as informações referentes ao QUADRO 4

        $i = 0;
        $marIn = $this->carregaMunicipiosAssistidos();

        if ($_POST['ravinfcomplqdo4']){

            if (strlen($_POST['ravinfcomplqdo4']) > 1000 ) {
                $this->msg = "O campo comentários gerais deve conter até 1000 caracteres.";
                return false;
            }

            if (!$this->salvaComplemento('ravinfcomplqdo4', $_POST['ravinfcomplqdo4'])){
                $this->msg = 'Erro ao salvar os comentários gerais.';
                return false;
            }
        }

        if (is_array($_POST['muncodq4'])) {
            foreach ($_POST['muncodq4'] as $m) {

                $marid = $marIn[$m];
                $msaobservacoes = $_POST['msaobservacoes'][$i];

                if (strlen($msaobservacoes) > 200){
                    $this->msg = "O campo 'observações' do quadro de Municípios assistidos sem alteração de etapa.";
                    return false;
                }

                $sciid = $_POST['sciid'][$i] != '' ? $_POST['sciid'][$i] : 'null';
                $esdid = $_POST['esdidq4'][$i] != '' ? $_POST['esdidq4'][$i] : 'null';
                $mstid = $_POST['mstidq4'][$i] != '' ? $_POST['mstidq4'][$i] : 'null';

//                $sqlS = "select count(sciid) c from sase.subetapacomissaoinsttecredeae sci inner join sase.situacaoassessoramento sta on sci.stacod = sta.stacod where sta.esdid = {$esdid}";
//                $resS = $db->pegaUm($sqlS);

                if ($mstid == 'null') {
                    if ($sciid == 'null') {
                        $sql = "insert into sase.municipiossemalteracaoperiodotecredeae (marid, esdid, msaobservacoes) values ({$marid}, {$esdid}, '{$msaobservacoes}')";
                    } else {
                        $sql = "insert into sase.municipiossemalteracaoperiodotecredeae (sciid, marid, esdid, msaobservacoes) values ({$sciid}, {$marid}, {$esdid}, '{$msaobservacoes}')";
                    }
                } else {
                    if ($sciid == 'null') {
                        $sql = "update sase.municipiossemalteracaoperiodotecredeae set
                                sciid = null,
                                msaobservacoes = '{$msaobservacoes}'
                            where mstid = {$mstid}";
                    } else {
                        $sql = "update sase.municipiossemalteracaoperiodotecredeae set
                                sciid = {$sciid},
                                msaobservacoes = '{$msaobservacoes}'
                            where mstid = {$mstid}";
                    }
                }

                $this->executar($sql);

                if (!$this->commit()) {
                    return false;
                }

                $i++;
            }
        }

        return true;

        // Fim das informações referentes ao QUADRO 4

    }

    public function savlarMunicipiosSemInformacao(){

        // Salva as informações referentes ao QUADRO 5

        $i = 0;
        $marIn = $this->carregaMunicipiosAssistidos();

        if ($_POST['ravinfcomplqdo5']){

            if (strlen($_POST['ravinfcomplqdo5']) > 1000 ) {
                $this->msg = "O campo comentários gerais deve conter até 1000 caracteres.";
                return false;
            }

            if (!$this->salvaComplemento('ravinfcomplqdo5', $_POST['ravinfcomplqdo5'])){
                $this->msg = 'Erro ao salvar os comentários gerais.';
                return false;
            }
        }

        if(is_array($_POST['muncodq5'])) {
            foreach ($_POST['muncodq5'] as $m) {
                $marid = $marIn[$m];
                $esdid = $_POST['esdidq5'][$i];
                $mstobservacoes = $_POST['mstobservacoes'][$i];
                $msfid = $_POST['msfidq5'][$i];

                if ($msfid == '') {
                    $sql = "insert into sase.municipiosseminftecredeae (marid, esdid, mstobservacoes) values ({$marid}, {$esdid}, '{$mstobservacoes}')";
                } else {
                    $sql = "update sase.municipiosseminftecredeae set
                            mstobservacoes = '{$mstobservacoes}'
                        where msfid = {$msfid}";
                }

                $this->executar($sql);

                if (!$this->commit()) {
                    return false;
                }

                $i++;
            }
        }

        return true;

        // Fim das informações referentes ao QUADRO 5
    }

    public function salvaQuadro6(){
        global $db;
        $marIn = $this->carregaMunicipiosAssistidos();
        $ravid = $this->arAtributos['ravid'];
        $quadro = $_POST['quadro'];
        $msaquadro = str_replace('q6_','',$quadro);
        $esdid = $_POST['esdid'] != '' ? $_POST['esdid'] : 'null';
        //ver($_POST[$quadro . 'codTab'], d);


        $i = 0;
        if(is_array($_POST[$quadro . 'muncod'])) {
            foreach ($_POST[$quadro . 'muncod'] as $m) {
                $marid = $marIn[$m];
                //ver($marIn, $m, d);

                $msaacaoproposta = $_POST[$quadro . 'msaacaoproposta'][$i];
                $msaid = $_POST[$quadro . 'codTab'][$i];
                $msaacaoproposta = trim($msaacaoproposta);

                if (strlen($msaacaoproposta) > 200){
                    $this->msg = "As ações propostas devem conter, no máximo, 200 caracteres.";
                    return false;
                }

                if ($msaacaoproposta != '') {
                    if ($msaid == '') {
                        $sql = "insert into sase.municipiossemacoesdesenvstecredeaee(msaquadro, marid, esdid, msaacaoproposta) values ({$msaquadro},{$marid}, {$esdid},'{$msaacaoproposta}')";
                    } else {
                        $sql = "update sase.municipiossemacoesdesenvstecredeaee set msaacaoproposta = '{$msaacaoproposta}' where msaid = {$msaid}";
                    }


                    $this->executar($sql);

                    if (!$this->commit()) {
                        return false;
                    }
                }

                $i++;
            }
        }

        return true;

    }

    public function salvaInformacoesComplementaresQ6(){
        $ravid = $this->arAtributos['ravid'];
        $ravinfcomplqdo6 = $_POST['ravinfcomplqdo6'];

        if (strlen($ravinfcomplqdo6) <= 1000 ) {
            $sql = "update sase.relatorioavaliadorredeae set
                    ravinfcomplqdo6 = '{$ravinfcomplqdo6}'
                where ravid = {$ravid}";

            $this->executar($sql);

            if (!$this->commit()) {
                return false;
            }
        } else {
            $this->msg = "O campo 'Comentarios gerais sobre avanços e dificuldades relacionados às ações propostas para o municípios no período' deve conter até 1000 caracteres.";
            return false;
        }

        return true;
    }

    public function salvaInformacoesComplementares()
    {
        $ravid = $this->arAtributos['ravid'];
        $ravinfcomplementar = $_POST['ravinfcomplementar'] ? $_POST['ravinfcomplementar'] : $_POST['ravinfcomplementar2'];

        if (strlen($ravinfcomplementar) <= 500 ) {
            $sql = "update sase.relatorioavaliadorredeae set
                    ravinfcomplementar = '{$ravinfcomplementar}'
                where ravid = {$ravid}";

            $this->executar($sql);

            if (!$this->commit()) {
                return false;
            }
        } else {
            $this->msg = "O campo 'Informações complementares sobre o periodo' deve conter até 500 caracteres.";
            return false;
        }

        return true;
    }

    public function getListaMunComAlteracaoPEESupervisorGeral($estuf, $mes, $layout = true){
        global $db;

        $sql = "select
                    aed.aedid,
                    esdo.esddsc esddsco,
                    esdd.esddsc esddscd,
                    (
                        select
                            count(*)
                        from sase.assessoramento ass
                        inner join workflow.historicodocumento hst on ass.docid = hst.docid
                        where hst.aedid = aed.aedid
                        and ass.estuf = '{$estuf}'
                        and extract(month from hst.htddata) = {$mes}
                    ) cont
                from workflow.acaoestadodoc aed
                inner join workflow.estadodocumento esdo on esdo.esdid = aed.esdidorigem
                inner join workflow.estadodocumento esdd on esdd.esdid = aed.esdiddestino
                where aedstatus = 'A'
                and aed.aedid in (2603, 2611, 2591, 2585, 2593, 2612, 2616, 2617)
                order by esdo.esdordem";
        $res = $db->carregar($sql);

        $html = <<<HTML
		<style>
			.campoData{
				width: 100px !important;
			}
			.spanObs{
				font-size: 10px;
			}
			th{
			    text-align: center;
			}
		</style>
		<div id="listagem" style="background: #ffffff !important;">
HTML;
        if (!$layout){
            $html .= <<< HTML
                <table name="tabAtvDes" border="1" cellspacing="0" cellpadding="5" width="100%" >
HTML;
        } else {
            $html .= <<< HTML
                <table name="tabAtvDes" class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }

        $html .= <<<HTML
        <thead>
            <tr>
                <th width="460" style="padding-top: 14px !important;">Etapa anterior</th>
                <th width="460" style="padding-top: 14px !important;">Etapa atual</th>
                <th>Número de Munucípios que apresentaram alteração</th>
            </tr>
        </thead>
        <tbody>
HTML;

        foreach ($res as $r) {
            $html .= <<<HTML
            <tr>
                <td>
                    {$r['esddsco']}
                </td>
                <td>
                    {$r['esddscd']}
                </td>
                <td style="text-align: center;">
                    {$r['cont']}
                </td>
            </tr>
HTML;
        }

        $html .= <<<HTML
                </tbody>
            </table>
HTML;

        $html .= <<<HTML
        </div>
HTML;

        $sql = "select tab.mundescricao, tab.cont from (
                select
                    mun.mundescricao,
                    count(*) cont
                from sase.assessoramento ass
                inner join territorios.municipio mun on ass.muncod = mun.muncod
                inner join workflow.historicodocumento hst on ass.docid = hst.docid
                where hst.aedid in (2603, 2611, 2591, 2585, 2593, 2612, 2616, 2617)
                and ass.estuf = '{$estuf}'
                and extract(month from hst.htddata) = {$mes}
                group by mun.mundescricao ) as tab
                where tab.cont > 1";
        $res = $db->carregar($sql);

        if (is_array($res)){
            // <span class="spanObs">*Observação: Municípios exibidos na cor vermelha, passaram por mais de uma situação no período deste relatório.</span>
            $html .= "<span class=\"spanObs\">";
            $html .= "*Observação: os municípios que passaram por mais de uma etapa no mês foram: ";
            $obs = '';
            foreach ($res as $r) {
                if ($obs != '') {
                    $obs .= ', '.$r['mundescricao'];
                } else {
                    $obs .= $r['mundescricao'];
                }
            }
            $html .= $obs.'.';
            $html .= "</span>";
        }

        return $html;
    }

    public function getListaInformacoesPEESupervisorGeral($campos = false, $layout = true){
        global $db;

        $ravid = $this->arAtributos['ravid'];

        $sql = "select
                    ipe.ipeid,
                    sta.stacod,
                    sta.stadsc,
                    ipe.ipeinformreunidas
                from sase.situacaoassessoramento sta
                left join sase.informacoespeesupgeralredeae ipe on ipe.stacod = sta.stacod and ipe.ravid = {$ravid}
                inner join workflow.estadodocumento esd on esd.esdid = sta.esdid
                where sta.esdid not in (".ESDID_SASE_SEM_INFORMACAO.", ".ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA.")
                order by esd.esdordem";

        $res = $db->carregar($sql);

        $html = <<<HTML
		<style>
			.campoData{
				width: 100px !important;
			}
			.spanObs{
				font-size: 10px;
			}
			th{
			    text-align: center;
			}
		</style>
		<div id="listagem" style="background: #ffffff !important;">
HTML;
        if (!$campos && !$layout){
            $html .= <<< HTML
                <table name="tabAtvDes" border="1" cellspacing="0" cellpadding="5" width="100%" >
HTML;
        } else {
            $html .= <<< HTML
                <table name="tabAtvDes" class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }

        $html .= <<<HTML
        <thead>
            <tr>
                <th width="360" style="padding-top: 14px !important;">Etapa</th>
                <th>Informações reunidas e descrição dos comprovantes anexados a este relatório</th>
            </tr>
        </thead>
        <tbody>
HTML;
        $i = 1;
        foreach ($res as $r) {
            $html .= <<<HTML
            <tr>
                <td>
                    <input type="hidden" name="ipeid[]" id="ipeid{$i}" value="{$r['ipeid']}"/>
                    <input type="hidden" name="stacod[]" id="stacod{$i}" value="{$r['stacod']}"/>
                    {$r['stadsc']}
                </td>
HTML;
            if (!$campos) {
                $html .= <<<HTML
                <td>{$r['ipeinformreunidas']}</td>
HTML;
            } else {
                $html .= <<<HTML
                <td>
                    <textarea class="form-control" maxlength="500" id="ipeinformreunidas{$i}" name="ipeinformreunidas[]" rows="3">{$r['ipeinformreunidas']}</textarea>
                </td>
HTML;
            }

            $html .= <<<HTML
            </tr>
HTML;
        }

        $html .= <<<HTML
                </tbody>
            </table>
        </div>
HTML;
        return $html;
    }

    public function salvaInformacoesPEESupervisorGeral(){
        global $db;
        $ravid = $this->arAtributos['ravid'];

        $i = 0;
        foreach ($_REQUEST['ipeinformreunidas'] as $ipeinformreunidas) {
            $ipeid  = $_REQUEST['ipeid'][$i];
            $stacod = $_REQUEST['stacod'][$i];

            if ($ipeinformreunidas != '') {
                if (strlen($ipeinformreunidas) > 500){
                    $this->msg = "O campo \'Informações reunidas e descrição dos comprovantes anexados a este relatório\', do quadro de Informações a respeito do Plano Estadual de Educação, deve conter até 500 caracteres.";
                    return false;
                }
                if ($ipeid == '') {
                    /**
                     * Insere um novo registro
                     */
                    $sql = "insert into sase.informacoespeesupgeralredeae (ravid, stacod, ipeinformreunidas) values ({$ravid},'{$stacod}','{$ipeinformreunidas}')";
                } else {
                    /**
                     * Altera um registro existente
                     */
                    $sql = "update sase.informacoespeesupgeralredeae set
                            ipeinformreunidas = '{$ipeinformreunidas}'
                        where ipeid = {$ipeid}";
                }
                //ver($sql, d);
                $this->executar($sql);

                if (!$this->commit()) {
                    $this->msg = "Erro ao salvar o quadro de Informações a respeito do Plano Estadual de Educação";
                    return false;
                }
            }
            $i++;
        }
        return true;
    }

    public function getContadorSituacaoAtualEtapasPNE($estuf, $mes, $layout = true){
        global $db;
//        $sql = "select
//                    sta.stadsc,
//                    (select
//                        count(mar.marid)
//                    from sase.municipiosassistidosredeae mar
//                    inner join sase.sitatualmunicipiotecredeae stm on stm.marid = mar.marid
//                    inner join sase.assessoramento ass on ass.assid = mar.assid
//                    inner join sase.relatorioavaliadorredeae rav on mar.ravid = rav.ravid
//                    where esdid = sta.esdid
//                    and ass.estuf = '{$estuf}'
//                    and extract('Month' from rav.ratdata2periodo) = {$mes}) c
//                from sase.situacaoassessoramento sta
//                inner join workflow.estadodocumento esd on sta.esdid = esd.esdid
//                where esd.esdid not in (".ESDID_SASE_SEM_INFORMACAO.", ".ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA.")
//                order by esd.esdordem";

        $sql = "select
                    esd.esddsc as stadsc,
                    count(tb.muncod) as c
                from workflow.estadodocumento esd
                left join (
                    select distinct
                        (
                            select
                                aed.esdiddestino
                            from workflow.historicodocumento hst
                            inner join workflow.acaoestadodoc aed on hst.aedid = aed.aedid
                            where hst.docid = ass.docid
                            and extract('Month' from hst.htddata) <= '{$mes}'
                            order by hst.htddata desc limit 1
                        ) as esdid,
                        ass.muncod
                    from sase.situacaoassessoramento sta
                    left join sase.assessoramento ass on ass.stacod = sta.stacod and ass.estuf = '{$estuf}'
                    left join sase.municipiosassistidosredeae mar on mar.assid = ass.assid
                ) as tb on tb.esdid = esd.esdid
                where esd.tpdid = ".TPDID_SASE_ASSESSORAMENTO."
                and esd.esdid not in (".ESDID_SASE_SEM_INFORMACAO.", ".ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA.")
                group by esd.esddsc, tb.esdid, esd.esdordem
                order by esd.esdordem";

        //ver($sql);
        $res = $db->carregar($sql);
        $html = <<<HTML
		<style>
			.campoData{
				width: 100px !important;
			}
			.spanObs{
				font-size: 10px;
			}
			th{
			    text-align: center;
			}
		</style>
		<div id="listagem" style="background: #ffffff !important;">
HTML;
        if (!$layout){
            $html .= <<< HTML
                <table name="tabAtvDes" border="1" cellspacing="0" cellpadding="5" width="100%" >
HTML;
        } else {
            $html .= <<< HTML
                <table name="tabAtvDes" class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }

        $html .= <<<HTML
        <thead>
            <tr>
                <th width="80%" style="padding-top: 14px !important;">Etapa</th>
                <th>Número de Municípios</th>
            </tr>
        </thead>
        <tbody>
HTML;

        foreach ($res as $r) {
            $html .= <<<HTML
            <tr>
                <td>
                    {$r['stadsc']}
                </td>
                <td style="text-align: center;">{$r['c']}</td>
            </tr>
HTML;
        }

        $html .= <<<HTML
                </tbody>
            </table>
        </div>
HTML;
        return $html;
    }

    public function getListaMunSemAlteracaoEtapa($estuf, $mes, $layout = true){
        global $db;
        $sql = "select
                    sta.stadsc,
                    (
                        select
                            count(*)
                        from sase.municipiosassistidosredeae mar
                        inner join sase.sitatualmunicipiotecredeae stm on stm.marid = mar.marid
                        inner join sase.assessoramento ass on ass.assid = mar.assid
                        inner join territorios.municipio mun on mun.muncod = ass.muncod
                        inner join sase.relatorioavaliadorredeae rav on mar.ravid = rav.ravid
                        where esdid = esd.esdid
                        and ass.estuf = '{$estuf}'
                        and extract('Month' from rav.ratdata2periodo) = {$mes}
                        and not exists (
                            select
                                1
                            from workflow.historicodocumento hst
                            where hst.docid = ass.docid
                            and date(hst.htddata) > date(rav.ratdata1periodo)
                        )
                    ) as c
                from sase.situacaoassessoramento sta
                inner join workflow.estadodocumento esd on sta.esdid = esd.esdid
                where esd.esdid not in (".ESDID_SASE_SEM_INFORMACAO.", ".ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA.")
                order by esd.esdordem";
        $res = $db->carregar($sql);
        $html = <<<HTML
		<style>
			.campoData{
				width: 100px !important;
			}
			.spanObs{
				font-size: 10px;
			}
			th{
			    text-align: center;
			}
		</style>
		<div id="listagem" style="background: #ffffff !important;">
HTML;
        if (!$layout){
            $html .= <<< HTML
                <table name="tabAtvDes" border="1" cellspacing="0" cellpadding="5" width="100%" >
HTML;
        } else {
            $html .= <<< HTML
                <table name="tabAtvDes" class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }

        $html .= <<<HTML
        <thead>
            <tr>
                <th width="80%" style="padding-top: 14px !important;">Etapa</th>
                <th>Número de Municípios</th>
            </tr>
        </thead>
        <tbody>
HTML;

        foreach ($res as $r) {
            $html .= <<<HTML
            <tr>
                <td>
                    {$r['stadsc']}
                </td>
                <td style="text-align: center;">{$r['c']}</td>
            </tr>
HTML;
        }

        $html .= <<<HTML
                </tbody>
            </table>
        </div>
HTML;
        return $html;
    }

    public function getListaMunSemInfo($estuf, $mes, $layout = true){
        global $db;
        $sql = "select
                    sta.stadsc,
                    (select
                        count(mar.marid)
                    from sase.municipiosassistidosredeae mar
                    inner join sase.sitatualmunicipiotecredeae stm on stm.marid = mar.marid
                    inner join sase.assessoramento ass on ass.assid = mar.assid
                    inner join sase.relatorioavaliadorredeae rav on mar.ravid = rav.ravid
                    where esdid = sta.esdid
                    and ass.estuf = '{$estuf}'
                    and extract('Month' from rav.ratdata2periodo) = {$mes}) c
                from sase.situacaoassessoramento sta
                inner join workflow.estadodocumento esd on sta.esdid = esd.esdid
                where esd.esdid in (".ESDID_SASE_SEM_INFORMACAO.", ".ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA.")
                order by esd.esdordem";
        $res = $db->carregar($sql);
        $html = <<<HTML
		<style>
			.campoData{
				width: 100px !important;
			}
			.spanObs{
				font-size: 10px;
			}
			th{
			    text-align: center;
			}
		</style>
		<div id="listagem" style="background: #ffffff !important;">
HTML;
        if (!$layout){
            $html .= <<< HTML
                <table name="tabAtvDes" border="1" cellspacing="0" cellpadding="5" width="100%" >
HTML;
        } else {
            $html .= <<< HTML
                <table name="tabAtvDes" class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }

        $html .= <<<HTML
        <thead>
            <tr>
                <th width="80%" style="padding-top: 14px !important;">Etapa</th>
                <th>Número de Municípios</th>
            </tr>
        </thead>
        <tbody>
HTML;

        foreach ($res as $r) {
            $html .= <<<HTML
            <tr>
                <td>
                    {$r['stadsc']}
                </td>
                <td style="text-align: center;">{$r['c']}</td>
            </tr>
HTML;
        }

        $html .= <<<HTML
                </tbody>
            </table>
        </div>
HTML;
        return $html;
    }

    public function getAcoesSuperGeral($estuf, $mes, $campos = false, $layout = true){
        global $db;
        $ravid = $this->arAtributos['ravid'];
        $arQuadro = array(
            1 => "Municípios sem informação",
            2 => "Municípios sem Comissão instituída",
            3 => "Municípios sem alteração da etapa de trabalho, mas com demora justificável",
            4 => "Municípios sem alteração da etapa de trabalho com demora maior do que a esperada"
        );
        $sql = "with temp_quadro as (select distinct msaquadro from sase.municipiossemacoesdesenvstecredeaee order by msaquadro)
                select
                    q.msaquadro,
                    (
                        select
                            count(*)
                        from sase.assessoramento ass
                        where exists (
                            select
                                1
                            from sase.municipiossemacoesdesenvstecredeaee  msa
                            inner join sase.municipiosassistidosredeae mar on msa.marid = mar.marid
                            inner join sase.relatorioavaliadorredeae rav on mar.ravid = rav.ravid
                            where mar.assid = ass.assid
                            and extract('Month' from rav.ratdata2periodo) = {$mes}
                            and msa.msaquadro = q.msaquadro
                        )
                        and ass.estuf = '{$estuf}'
                    ) c,
                    (
                        select
                            apmacoesporpostas
                        from sase.acoespropmunseminfsupgeralredeae apm
                        where apm.ravid = {$ravid}
                        and apm.apmquadro = q.msaquadro
                    ) apmacoesporpostas,
                    (
                        select
                            apmid
                        from sase.acoespropmunseminfsupgeralredeae apm
                        where apm.ravid = {$ravid}
                        and apm.apmquadro = q.msaquadro
                    ) apmid
                from temp_quadro q;";
        $res = $db->carregar($sql);
        $html = <<<HTML
		<style>
			.campoData{
				width: 100px !important;
			}
			.spanObs{
				font-size: 10px;
			}
			th{
			    text-align: center;
			}
		</style>
		<div id="listagem" style="background: #ffffff !important;">
HTML;
        if (!$campos && !$layout){
            $html .= <<< HTML
                <table name="tabAtvDes" border="1" cellspacing="0" cellpadding="5" width="100%" >
HTML;
        } else {
            $html .= <<< HTML
                <table name="tabAtvDes" class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }

        $html .= <<<HTML
        <thead>
            <tr>
                <th width="200" style="padding-top: 14px !important;"></th>
                <th>Número de Municípios</th>
                <th>Ações propostas</th>
            </tr>
        </thead>
        <tbody>
HTML;
        $i = 1;
        foreach ($res as $r) {
            $html .= <<<HTML
            <tr>
                <td style="text-align: center;">
                    <input type="hidden" name="apmquadro[]" id="apmquadro{$i}" value="{$r['msaquadro']}"/>
                    <input type="hidden" name="apmid[]" id="apmid{$i}" value="{$r['apmid']}"/>
                    {$arQuadro[$r['msaquadro']]}
                </td>
                <td style="text-align: center;">{$r['c']}</td>
                <td>
HTML;
            if ($campos){
                $html .= <<<HTML
                <textarea class="form-control" id="apmacoesporpostas{$i}" maxlength="500" name="apmacoesporpostas[]" rows="2">{$r['apmacoesporpostas']}</textarea>
HTML;
            } else {
                $html .= $r['apmacoesporpostas'];
            }
            $html .= <<<HTML
                </td>
            </tr>
HTML;
            $i++;
        }

        $html .= <<<HTML
                </tbody>
            </table>
        </div>
HTML;
        return $html;
    }

    public function salvaAcoesSuperGeral(){
        global $db;

        $ravid = $this->arAtributos['ravid'];

        if (is_array($_REQUEST['apmacoesporpostas'])) {
            $i = 0;
            foreach ($_REQUEST['apmacoesporpostas'] as $apmacoesporpostas) {
                $apmid = $_REQUEST['apmid'][$i];
                $apmquadro = $_REQUEST['apmquadro'][$i];
                if ($apmacoesporpostas != '') {
                    if (strlen($apmacoesporpostas) > 500) {
                        $this->msg = "O campo \'Ações propostas\', do quadro de Ações propostas peos AE Supervisores, deve conter ate 500 caracteres.";
                        return false;
                    }
                    if ($apmid == '') {
                        /**
                         *  Insere um novo registro
                         */
                        $sql = "insert into sase.acoespropmunseminfsupgeralredeae (ravid, apmquadro, apmacoesporpostas) values ({$ravid},{$apmquadro},'{$apmacoesporpostas}')";
                    } else {
                        /**
                         *  Altera um registro existente
                         */
                        $sql = "update sase.acoespropmunseminfsupgeralredeae set apmacoesporpostas = '{$apmacoesporpostas}' where apmid = {$apmid}";
                    }
                    $this->executar($sql);

                    if (!$this->commit()) {
                        $this->msg = "Erro ao salvar o quadro de Ações propostas pelos AE Supervisores para os municipios sem informação, sem comissao instituída e sem alteração de etapa de trabalho";
                        return false;
                    }
                }
                $i++;
            }
        }else{
            $this->msg = "Nenhuma açao informada.";
            return false;
        }
        return true;
    }

    public function getListaAtividadesDesenvolvidas($campos = false, $layout = true, $pflcod = null, $camposIn = false, $estuf = null, $adadiretoria = 1)
    {
        global $db, $data;

        $ravid = $this->arAtributos['ravid'];
        //$pflcod = $this->arAtributos['pflcod'];
        $usucpf = $this->arAtributos['usucpf'];
        $dataIni = $data->formataData($this->arAtributos['ratdata1periodo']);
        $dataFim = $data->formataData($this->arAtributos['ratdata2periodo']);

        switch ($pflcod){
            case PFLCOD_SASE_COORDENADOR_ESTADUAL_DIVAPE:
            case PFLCOD_SASE_EXECUTIVO:
                if($adadiretoria = 1) {
                    $sql = "select
                            ada.adaid,
                            ada.adadsc,
                            ava.avadescoutrostipos,
                            ava.avaid,
                            ava.avaatores,
                            to_char(ava.avadata1periodo, 'dd/mm/yyyy') as avtdata1periodo,
                            to_char(ava.avadata2periodo, 'dd/mm/yyyy') as avtdata2periodo,
                            ava.avaresponsavel,
                            ava.avadetalhes
                        from sase.ativdesenvavaliadoresredeae ava
                        left join sase.atividadesavaliadoresredeae ada on ava.adaid = ada.adaid and adtstatus = 'A' and ada.pflcod = {$pflcod} and ada.adadiretoria = {$adadiretoria}
                        where ava.ravid = {$ravid}";
                }
                if($adadiretoria = 2){
                    $sql = "select
                            ada.adaid,
                            ada.adadsc,
                            ava.avadescoutrostipos,
                            ava.avaid,
                            ava.avaatores,
                            to_char(ava.avadata1periodo, 'dd/mm/yyyy') as avtdata1periodo,
                            to_char(ava.avadata2periodo, 'dd/mm/yyyy') as avtdata2periodo,
                            ava.avaresponsavel,
                            ava.avadetalhes
                        from sase.atividadesavaliadoresredeae ada
                        left join sase.ativdesenvavaliadoresredeae ava on ava.adaid = ada.adaid and ava.ravid = {$ravid}
                        where adtstatus = 'A'
                        and ada.pflcod = {$pflcod}
                        and ada.adadiretoria = {$adadiretoria}";
                }
                break;

            default:
                if (!empty($ravid)) {
                    $join = "left join sase.ativdesenvavaliadoresredeae ava on ada.adaid = ava.adaid and ava.ravid = " . $ravid . " and ava.avaid = (select max(avaid) from sase.ativdesenvavaliadoresredeae where adaid = ada.adaid and ravid = " . $ravid . ")";
                } else {
                    $join = "left join sase.ativdesenvavaliadoresredeae ava on ada.adaid = ava.adaid";
                }

                $where = '';
                if (!empty($pflcod)) {
                    $where .= " and ada.pflcod = {$pflcod}";
                }
                $where .= " and ada.adadiretoria = {$adadiretoria}";

                $sql = "select
					ada.adaid,
					ada.adadsc,
					ava.avadescoutrostipos,
					ava.avaid,
					ava.avaatores,
					to_char(ava.avadata1periodo, 'dd/mm/yyyy') as avtdata1periodo,
					to_char(ava.avadata2periodo, 'dd/mm/yyyy') as avtdata2periodo,
					ava.avaresponsavel,
					ava.avadetalhes
				from sase.atividadesavaliadoresredeae ada
				{$join}
				where adtstatus = 'A'
				{$where}";
                break;
        }

        //ver($sql);
        $res = $db->carregar($sql);
        $contN = 0;
        if (is_array($res)){
            $contN = count($res);
        }
        //$contN = $contN == 0 ? 0 : $contN;

        if ($pflcod == PFLCOD_SASE_EXECUTIVO && $adadiretoria == 1){
            $sqlS = "select
                        ada.adadsc,
                        ava.avadetalhes,
                        ava.avaatores,
                        ava.avaresponsavel,
                        to_char(ava.avadata1periodo, 'dd/mm/yyyy') as avadata1periodo,
                        to_char(ava.avadata2periodo, 'dd/mm/yyyy') as avadata2periodo,
                        usu.usunome,
                        usu.usucpf,
                        rav.ravinfcomplqdo1
                    from sase.ativdesenvavaliadoresredeae ava
                    inner join sase.atividadesavaliadoresredeae ada on ava.adaid = ada.adaid
                    inner join sase.relatorioavaliadorredeae rav on rav.ravid = ava.ravid
                    inner join seguranca.usuario usu on usu.usucpf = rav.usucpf
                    where ada.pflcod = ".PFLCOD_SASE_SUPERVISOR."
                        and ada.adadiretoria = {$adadiretoria}
                        and rav.ratdata1periodo = '{$dataIni}'
                        and rav.ratdata2periodo = '{$dataFim}'
                        and usu.regcod = '{$estuf}'
                    order by usu.usunome";

            $resS = $db->carregar($sqlS);

            $sqlT = "select
                        ada.adadsc,
                        ava.avadetalhes,
                        ava.avaatores,
                        ava.avaresponsavel,
                        to_char(ava.avadata1periodo, 'dd/mm/yyyy') as avadata1periodo,
                        to_char(ava.avadata2periodo, 'dd/mm/yyyy') as avadata2periodo,
                        usu.usunome,
                        usu.usucpf,
                        rav.ravinfcomplqdo1
                    from sase.ativdesenvavaliadoresredeae ava
                    inner join sase.atividadesavaliadoresredeae ada on ava.adaid = ada.adaid
                    inner join sase.relatorioavaliadorredeae rav on rav.ravid = ava.ravid
                    inner join seguranca.usuario usu on usu.usucpf = rav.usucpf
                    where ada.pflcod = ".PFLCOD_SASE_TECNICO."
                        and ada.adadiretoria = {$adadiretoria}
                        and rav.ratdata1periodo = '{$dataIni}'
                        and rav.ratdata2periodo = '{$dataFim}'
                        and usu.regcod = '{$estuf}'
                    order by usu.usunome";

            $resT = $db->carregar($sqlT);
        }

        if ($pflcod == PFLCOD_SASE_SUPERVISOR && $adadiretoria == 1){
            $sqlT = "select
                        ada.adadsc,
                        ava.avadetalhes,
                        ava.avaatores,
                        ava.avaresponsavel,
                        to_char(ava.avadata1periodo, 'dd/mm/yyyy') as avadata1periodo,
                        to_char(ava.avadata2periodo, 'dd/mm/yyyy') as avadata2periodo,
                        usu.usunome,
                        usu.usucpf,
                        rav.ravinfcomplqdo1
                    from sase.ativdesenvavaliadoresredeae ava
                    inner join sase.atividadesavaliadoresredeae ada on ava.adaid = ada.adaid
                    inner join sase.relatorioavaliadorredeae rav on rav.ravid = ava.ravid
                    inner join seguranca.usuario usu on usu.usucpf = rav.usucpf
                    where ada.pflcod = ".PFLCOD_SASE_TECNICO."
                    and ada.adadiretoria = {$adadiretoria}
                    and rav.ratdata1periodo = '{$dataIni}'
                    and rav.ratdata2periodo = '{$dataFim}'
                    and rav.usucpf in (
                        select distinct
                            cpftecnico
                        from sase.usuarioresponsabilidade
                        where rpustatus = 'A'
                        and cpftecnico is not null
                        and usucpf = '{$usucpf}'
                    )
                    order by usu.usunome";

            $resT = $db->carregar($sqlT);
        }

        $htmlN = <<<HTML
            <tr><td></td><td><input type="hidden" name="reg[]" id="reg{i}" value="{i}" /><textarea class="form-control" id="avadescoutrostipos{i}" maxlength="1000" name="avadescoutrostipos[]" rows="2"></textarea></td><td><textarea class="form-control" id="avadetalhes{i}" maxlength="1000" name="avadetalhes[]" rows="2"></textarea></td><td><textarea class="form-control" id="avaatores{i}" maxlength="200" name="avaatores[]" rows="2"></textarea></td><td><center><table><tr><td><input id="avadata1periodo{i}" name="avadata1periodo[]" type="text" class="campoData form-control" value="" placeholder=""></td><td>à</td><td><input id="avadata2periodo{i}" name="avadata2periodo[]" type="text" class="campoData form-control" value="" placeholder=""></td></tr></table></center></td><td><textarea class="form-control" id="avaresponsavel{i}" maxlength="50" name="avaresponsavel[]" rows="2"></textarea></td></tr>
HTML;

        if ($layout) {
            $html = <<<HTML
        <script>
            function adicionaAtividade(){
                c = jQuery("#contN").val();
                t = '{$htmlN}';
                c++;
                t = t.replace(new RegExp('{i}', 'g'), c);
                jQuery("#camposN").append(t);
                jQuery("#contN").val(c);
                jQuery('.campoData').mask('99/99/9999');
                jQuery('.campoData').datepicker();
            }
        </script>
		<style>
			.campoData{
				width: 100px !important;
			}
			.spanObs{
				font-size: 10px;
			}
			th{
			    text-align: center;
			}
		</style>
		<div id="listagem" style="background: #ffffff !important;">
		    <input type="hidden" id="contN" name="contN" value="{$contN}"/>
HTML;
        }

        if (!$campos && !$layout){
            $html .= <<< HTML
                <table name="tabAtvDes" border="1" cellspacing="0" cellpadding="5" width="100%" class="tableFontReduct" >
HTML;
        } else {
            $html .= <<< HTML
                <table name="tabAtvDes" class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }

        $htmlNovoBtn = "";
;        if ($camposIn && $campos){
            $htmlNovoBtn .= <<<HTML
                <th>
                    <button title="Novo" class="btn btn-info" type="button" id="btnAdd" onclick="adicionaAtividade()">+
                    </button>
                </th>
HTML;
        }

        $wdthAtv = 140;
        if (!$campos && !$layout) {
            $widthAtv = 140;
        }

            $html .= <<<HTML
				<thead>
					<tr>
					    {$htmlNovoBtn}
                        <th width="{$wdthAtv}">Atividades</th>
						<th width="140">Especificar</th>
						<th width="140">Atores envolvidos</th>
						<th>Período de realização</th>
						<th>Responsável</th>
					</tr>
				</thead>
				<tbody>
HTML;

        $i = 1;
        if (is_array($res)) {
            foreach ($res as $r) {
                $atvdssc = empty($r['adadsc']) ? $campos ? "<textarea class=\"form-control\" id=\"avadescoutrostipos{$i}\" maxlength=\"1000\" name=\"avadescoutrostipos[]\" rows=\"2\">{$r['avadescoutrostipos']}</textarea>" : $r['avadescoutrostipos'] : $r['adadsc'];
                $html .= <<<HTML
            <tr>
HTML;

                if ($camposIn && $campos){
                    $html .= <<<HTML
                    <td>
                    </td>
HTML;
                }

                if ($campos) {
                    $html .= <<<HTML
                    <td>
                        <input type="hidden" name="adaid[]" id="adaiid{$i}" value="{$r['adaid']}" />
                        <input type="hidden" name="avaid[]" id="avaiid{$i}" value="{$r['avaid']}" />
                        <input type="hidden" name="reg[]" id="reg{$i}" value="{$i}" />
                        {$atvdssc}
                    </td>
                    <td>
                        <textarea class="form-control" id="avadetalhes{$i}" maxlength="1000" name="avadetalhes[]" rows="2">{$r['avadetalhes']}</textarea>
                    </td>
                    <td>
                        <textarea class="form-control" id="avaatores{$i}" maxlength="200" name="avaatores[]" rows="2">{$r['avaatores']}</textarea>
                    </td>
                    <td>
                        <center>
                            <table>
                                <tr>
                                    <td><input id="avadata1periodo{$i}" name="avadata1periodo[]" type="text" class="campoData form-control" value="{$r['avtdata1periodo']}" placeholder=""></td>
                                    <td>à</td>
                                    <td><input id="avadata2periodo{$i}" name="avadata2periodo[]" type="text" class="campoData form-control" value="{$r['avtdata2periodo']}" placeholder=""></td>
                                </tr>
                            </table>
                        </center>
                    </td>
                    <td>
                        <textarea class="form-control" id="avaresponsavel{$i}" maxlength="50" name="avaresponsavel[]" rows="2">{$r['avaresponsavel']}</textarea>
                    </td>
HTML;

                } else {

                    $datas = $r['avtdata1periodo'] != '' && $r['avtdata2periodo'] != '' ? $r['avtdata1periodo'] . ' à ' . $r['avtdata2periodo'] : '';

                    $html .= <<<HTML
                    <td>
                        <input type="hidden" name="adaid[]" id="adaiid{$i}" value="{$r['adaid']}" />
                        <input type="hidden" name="avaid[]" id="avaiid{$i}" value="{$r['avaid']}" />
                        <input type="hidden" name="reg[]" id="reg{$i}" value="{$i}" />
                        {$atvdssc}
                    </td>
                    <td>{$r['avadetalhes']}</td>
                    <td>{$r['avaatores']}</td>
                    <td>{$datas}</td>
                    <td>{$r['avaresponsavel']}</td>
HTML;

                }
                $html .= "</tr>";
                $i++;
            }
        }
        $html .= <<<HTML
                <tbody id="camposN">

                </tbody>

HTML;

        switch ($pflcod) {
            case PFLCOD_SASE_EXECUTIVO:
                if($adadiretoria == 1) {
                    $html .= <<<HTML
                    <tr>
                        <th colspan="6">
                            <center>
                                ATIVIDADES DESENVOLVIDAS PELOS SUPERVISORES
                            </center>
                        </th>
                    </tr>
HTML;

                    $cpf = '';
                    if (is_array($resS)) {
                        foreach ($resS as $rT) {
                            $datas = $rT['avadata1periodo'] != '' && $rT['avadata2periodo'] != '' ? $rT['avadata1periodo'] . ' à ' . $rT['avadata2periodo'] : '';
                            if ($cpf != $rT['usucpf']) {
                                $html .= <<<HTML
                <tr>
                    <th colspan="6" style="text-align: left !important;;">{$rT['usunome']}</th>
                </tr>
HTML;
                                $cpf = $rT['usucpf'];
                            }

                            $html .= <<<HTML
                <tr>
HTML;

                            if ($camposIn && $campos) {
                                $html .= <<<HTML
                    <td>
                    </td>
HTML;
                            }
                            $html .= <<<HTML
                    <td>{$rT['adadsc']}</td>
                    <td>{$rT['avadetalhes']}</td>
                    <td>{$rT['avaatores']}</td>
                    <td>{$datas}</td>
                    <td>{$rT['avaresponsavel']}</td>
                </tr>
HTML;
                        }
                    }

                    $html .= <<<HTML

                    <tr>
                        <th colspan="6">
                            <center>
                                ATIVIDADES DESENVOLVIDAS PELOS TÉCNICOS
                            </center>
                        </th>
                    </tr>
HTML;

                    $cpf = '';
                    if (is_array($resT)) {
                        foreach ($resT as $rT) {
                            $datas = $rT['avadata1periodo'] != '' && $rT['avadata2periodo'] != '' ? $rT['avadata1periodo'] . ' à ' . $rT['avadata2periodo'] : '';
                            if ($cpf != $rT['usucpf']) {
                                $html .= <<<HTML
                <tr>
                    <th colspan="6" style="text-align: left !important;;">{$rT['usunome']}</th>
                </tr>
HTML;
                                $cpf = $rT['usucpf'];
                            }

                            $html .= <<<HTML
                <tr>
HTML;

                            if ($camposIn && $campos) {
                                $html .= <<<HTML
                    <td>
                    </td>
HTML;
                            }

                            $atores = htmlentities($rT['avaatores']);

                            $html .= <<<HTML
                    <td>{$rT['adadsc']}</td>
                    <td>{$rT['avadetalhes']}</td>
                    <td>{$atores}</td>
                    <td>{$datas}</td>
                    <td>{$rT['avaresponsavel']}</td>
                </tr>
HTML;
                        }
                    }
                }
                break;

            case PFLCOD_SASE_SUPERVISOR:
                if($adadiretoria == 1) {
                    $html .= <<<HTML

                    <tr>
                        <th colspan="6">
                            <center>
                                ATIVIDADES DESENVOLVIDAS PELOS TÉCNICOS
                            </center>
                        </th>
                    </tr>
HTML;

                    $cpf = '';
                    if (is_array($resT)) {
                        foreach ($resT as $rT) {
                            $datas = $rT['avadata1periodo'] != '' && $rT['avadata2periodo'] != '' ? $rT['avadata1periodo'] . ' à ' . $rT['avadata2periodo'] : '';
                            if ($cpf != $rT['usucpf']) {
                                $html .= <<<HTML
                <tr>
                    <th colspan="5" style="text-align: left !important;;">{$rT['usunome']}</th>
                </tr>
HTML;
                                $cpf = $rT['usucpf'];
                            }

                            $html .= <<<HTML
                <tr>
HTML;

                            if ($camposIn && $campos) {
                                $html .= <<<HTML
                    <td>
                    </td>
HTML;
                            }
                            $html .= <<<HTML
                    <td>{$rT['adadsc']}</td>
                    <td>{$rT['avadetalhes']}</td>
                    <td>{$rT['avaatores']}</td>
                    <td>{$datas}</td>
                    <td>{$rT['avaresponsavel']}</td>
                </tr>
HTML;
                        }
                    }
                }
                break;
        }

        if ($layout) {
            $html .= <<<HTML
				</tbody>
			</table>
		</div>
HTML;
        } else {
            $html .= <<<HTML
				</tbody>
			</table>
HTML;
        }

        if ($pflcod == PFLCOD_SASE_SUPERVISOR && !$layout) {
            $ravinfcomplqdo = $this->arAtributos['ravinfcomplqdo1'] != '' ? $this->arAtributos['ravinfcomplqdo1'] : '&nbsp;';
            $html .= <<<HTML
                <span class="spanObs">*para este tipo de analise, os AE Técnicos deverão usar e anexar planilha padrão disponibilizada pela SASE/MEC ao seu relatório quinzenal e o AE Supervisor deverá considerá-la.</span>
                <h3>Comentários gerais sobre acanços e dificuldades relacionados às atividades realizadas no período.</h3>
                <div style="text-align: justify; border: 1px solid #000; padding: 5px; min-height: 60px; height: 150px;">
                    {$ravinfcomplqdo}
                </div>

HTML;

        }

        return $html;
    }

    public function getListaResultadosConsolidados($campos = false, $layout = true){
        global $db, $data;

        //$usucpf = ''; //$_REQUEST['usucpf'];
        //$htddata = '11/02/2015'; //$_REQUEST['ratdata1periodo'];

        $usucpf = $this->arAtributos['usucpf'];
        $htddata = $data->formataData($this->arAtributos['ratdata2periodo']);
        $ravid = $this->arAtributos['ravid'];
        $cont = array(); // Contador dos estados

        //verifica perfil relatorio
        $sql    = "SELECT
				pu.pflcod
			FROM
				seguranca.perfilusuario pu
			INNER JOIN
				seguranca.perfil p ON p.pflcod = pu.pflcod
								  AND p.sisid = " . SASE_SISID . "
			WHERE
				pu.usucpf = '" . $usucpf . "'
			ORDER BY
				p.pflnivel";
        $pfls = $db->carregarColuna( $sql );

        if (is_array($pfls)){
            if (in_array(PFLCOD_SASE_TECNICO, $pfls)){
                $pflcod = PFLCOD_SASE_TECNICO;
            } else if (in_array(PFLCOD_SASE_SUPERVISOR, $pfls)){
                $pflcod = PFLCOD_SASE_SUPERVISOR;
            }
        }

        $sqlEsd = "select
 						esdid,
						esddsc
 					from workflow.estadodocumento where tpdid = ".TPDID_SASE_ASSESSORAMENTO."
 					and esdid not in (".ESDID_SASE_SEM_INFORMACAO.", ".ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA.")
 					and esdstatus = 'A'
 					order by esdordem";
        $resEsd = $db->carregar($sqlEsd);

        if($pflcod == PFLCOD_SASE_SUPERVISOR) {

            //pega os ravid dos tecnicos supervisionados
            $sql    = "select
                            tsp.cpftecnico
                        from sase.tecnicosupervisionadosredeae tsp
                        where ravid = {$ravid}
                        and tsp.tspid = (select max(tspid) from sase.tecnicosupervisionadosredeae where cpftecnico = tsp.cpftecnico and muncod = tsp.muncod and ravid = {$ravid} )
                        group by tsp.cpftecnico";

//            $sql    = "select
//                            usu.usucpf
//                        from seguranca.usuario usu
//                        inner join sase.tecnicosupervisionadosredeae tsp on tsp.cpftecnico = usu.usucpf and tsp.tspid = (select max(tspid) from sase.tecnicosupervisionadosredeae where cpftecnico = tsp.cpftecnico and muncod = tsp.muncod and ravid = {$ravid} )
//                        where ravid = {$ravid}
//                        group by usu.usucpf";
            $cpfTecnicos = $db->carregarColuna( $sql );

            if($cpfTecnicos){
                $sql = "SELECT ravid FROM sase.relatorioavaliadorredeae
                        where usucpf in ('".implode("','",$cpfTecnicos)."') and ratdata1periodo = '{$this->arAtributos['ratdata1periodo']}' and ratdata2periodo = '{$this->arAtributos['ratdata2periodo']}' and ratperiodo = {$this->arAtributos['ratperiodo']}";
                $arrayRavid = $db->carregarColuna( $sql );
            }

            if (is_array($arrayRavid) && count($arrayRavid) > 0) {
                $sql = "select
					tab.muncod,
					mun.mundescricao,
					tab.esdid,
					tab.marid,
					tab.stmid,
					tab.stmobservacoes,
					esd2.esddsc,
					tab.htddata
				from (
					select
						ass.muncod as muncod,
						max(esd.esdid) as esdid,
						mar.marid,
						stm.stmid,
						stm.stmobservacoes,
						hst.htddata
					from sase.assessoramento ass
					inner join sase.usuarioresponsabilidade rpu on ass.muncod = rpu.muncod
					inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid and mar.ravid in (" . implode(",", $arrayRavid) . ")
					left join sase.sitatualmunicipiotecredeae stm on stm.marid = mar.marid and stm.stmid = (select max(stmid)  from sase.sitatualmunicipiotecredeae where marid = mar.marid)
					left join workflow.historicodocumento hst on hst.docid = ass.docid
						and hst.htddata = (select max(htddata) from workflow.historicodocumento where docid = ass.docid and date(htddata) <= date('{$htddata}'))
					left join workflow.acaoestadodoc aed on aed. aedid = hst.aedid
					inner join workflow.estadodocumento esd on esd.esdid = aed.esdiddestino and esd.esdid not in (" . ESDID_SASE_SEM_INFORMACAO . ", " . ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA . ")
					where rpu.usucpf = '{$usucpf}'
					and rpu.rpustatus = 'A'
					group by ass.muncod, mar.marid, stm.stmid, stm.stmobservacoes, hst.htddata
				) as tab
				left join territorios.municipio mun on tab.muncod = mun.muncod
				left join workflow.estadodocumento esd2 on tab.esdid = esd2.esdid
				order by mun.mundescricao";
            } else {
                $sql = "select
					tab.muncod,
					mun.mundescricao,
					tab.esdid,
					tab.marid,
					tab.stmid,
					tab.stmobservacoes,
					esd2.esddsc,
					tab.htddata
				from (
					select
						ass.muncod as muncod,
						max(esd.esdid) as esdid,
						mar.marid,
						stm.stmid,
						stm.stmobservacoes,
						hst.htddata
					from sase.assessoramento ass
					inner join sase.usuarioresponsabilidade rpu on ass.muncod = rpu.muncod
					inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid and mar.ravid = {$ravid}
					left join sase.sitatualmunicipiotecredeae stm on stm.marid = mar.marid and stm.stmid = (select max(stmid)  from sase.sitatualmunicipiotecredeae where marid = mar.marid)
					left join workflow.historicodocumento hst on hst.docid = ass.docid
						and hst.htddata = (select max(htddata) from workflow.historicodocumento where docid = ass.docid and date(htddata) <= date('{$htddata}'))
					left join workflow.acaoestadodoc aed on aed. aedid = hst.aedid
					inner join workflow.estadodocumento esd on esd.esdid = aed.esdiddestino and esd.esdid not in (" . ESDID_SASE_SEM_INFORMACAO . ", " . ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA . ")
					where rpu.usucpf = '{$usucpf}'
					and rpu.rpustatus = 'A'
					group by ass.muncod, mar.marid, stm.stmid, stm.stmobservacoes, hst.htddata
				) as tab
				left join territorios.municipio mun on tab.muncod = mun.muncod
				left join workflow.estadodocumento esd2 on tab.esdid = esd2.esdid
				order by mun.mundescricao";
            }
        }else{
            $sql = "select
					tab.muncod,
					mun.mundescricao,
					tab.esdid,
					tab.marid,
					tab.stmid,
					tab.stmobservacoes,
					esd2.esddsc,
					tab.htddata
				from (
					select
						ass.muncod as muncod,
						max(esd.esdid) as esdid,
						mar.marid,
						stm.stmid,
						stm.stmobservacoes,
						hst.htddata
					from sase.assessoramento ass
					inner join sase.usuarioresponsabilidade rpu on ass.muncod = rpu.muncod
					inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid and mar.ravid = {$ravid}
					left join sase.sitatualmunicipiotecredeae stm on stm.marid = mar.marid and stm.stmid = (select max(stmid)  from sase.sitatualmunicipiotecredeae where marid = mar.marid)
					left join workflow.historicodocumento hst on hst.docid = ass.docid
						and hst.htddata = (select max(htddata) from workflow.historicodocumento where docid = ass.docid and date(htddata) <= date('{$htddata}'))
					left join workflow.acaoestadodoc aed on aed. aedid = hst.aedid
					inner join workflow.estadodocumento esd on esd.esdid = aed.esdiddestino and esd.esdid not in (" . ESDID_SASE_SEM_INFORMACAO . ", " . ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA . ")
					where rpu.usucpf = '{$usucpf}'
					and rpu.rpustatus = 'A'
					group by ass.muncod, mar.marid, stm.stmid, stm.stmobservacoes, hst.htddata
				) as tab
				left join territorios.municipio mun on tab.muncod = mun.muncod
				left join workflow.estadodocumento esd2 on tab.esdid = esd2.esdid
				order by mun.mundescricao";
        }
        //ver($sql);
        $res = $db->carregar($sql);

        $html = <<< HTML
        <style>
            .rotate {
                /*-webkit-transform: rotate(270deg);
                transform: rotate(270deg);*/
                height: 150px;
                font-size: 10px;
                margin: 0 !important;
                padding: 2px !important;
            }

</style>
		<div id="listagem" style="background: #ffffff !important;">
HTML;

        if (!$campos && !$layout){
            $html .= <<< HTML
                <table border="1" cellspacing="0" cellpadding="5" width="695" class="col-md-12 table-condensed table-hover table-responsive" >
HTML;
        } else {
            $html .= <<< HTML
                <table class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }

        $html .= <<<HTML
				<thead>
					<tr>
						<th width="350">Municípios</th>
HTML;

        foreach ($resEsd as $r){
            $cont[$r['esdid']] = 0;
            $html .= <<<HTML
                                <th class="rotate"><div><span>{$r['esddsc']}</span></div></th>
HTML;
        }

        $html .= <<<HTML
						<th width="350">Observação</th>
					</tr>
				</thead>
				<tbody>
HTML;

        //ver($res, d);
        if (is_array($res)) {
            foreach ($res as $s) {
                $html .= <<< HTML
                            <tr>
                                <td>
                                    <input type="hidden" name="muncodq2[]" value="{$s['muncod']}"/>
                                    <input type="hidden" name="esdidq2[]" value="{$s['esdid']}"/>
                                    <input type="hidden" name="maridq2[]" value="{$s['marid']}"/>
                                    <input type="hidden" name="stmidq2[]" value="{$s['stmid']}"/>
                                    {$s['mundescricao']}
                                </td>
HTML;

                foreach ($resEsd as $r) {
                    if ($s['esdid'] == $r['esdid']){
                        $cont[$r['esdid']]++;
                        $esdid = 'X';
                    } else {
                        $esdid = '';
                    }
                    //$esdid = $s['esdid'] == $r['esdid'] ? 'X' : '';
                    $html .= <<<HTML
                                    <td>
                                        <div
                                            style="text-align: center;">{$esdid}</div>
                                    </td>
HTML;
                }
                $html .= <<<HTML
                                <td>
HTML;

                if ($campos) {
                    $html .= <<<HTML
                                    <textarea class="form-control maxRC" id="stmobservacoes" name="stmobservacoes[]"
                                              rows="2"
                                              cols="150">{$s['stmobservacoes']}</textarea>
HTML;
                } else {
                    $html .= $s['stmobservacoes'];
                }

                $html .= <<<HTML
                                </td>
                            </tr>
HTML;
            }

            $html .= <<<HTML
                            <tr>
                                <td style="text-align: right;">Totais:</td>
HTML;
            $contM = 0;
            foreach ($cont as $r) {
                $contM = $contM + $r;
                $html .= <<<HTML
                                <td style="text-align: center;">{$r}</td>
HTML;
            }

            $html .= <<<HTML
                                <td style="text-align: center;">{$contM}</td>
                            </tr>
HTML;


        }

        $html .= <<< HTML
            				</tbody>
			</table>
		</div>
HTML;

        return $html;
    }

    public function getMunicipiosComAlteracaoNaData($campos = false, $layout = true) {
        global $db, $data;

        //$usucpf = ''; //$_REQUEST['usucpf'];
        //$ratdata1periodo = '02/02/2015'; //$_REQUEST['ratdata1periodo'];
        //$ratdata2periodo = '11/02/2015'; //$_REQUEST['ratdata2periodo'];

        $usucpf = $this->arAtributos['usucpf'];
        $ratdata1periodo = $data->formataData($this->arAtributos['ratdata1periodo']);
        $ratdata2periodo = $data->formataData($this->arAtributos['ratdata2periodo']);
        $ravid = $this->arAtributos['ravid'];
        $pflcod = $this->arAtributos['pflcod'];

        $sqlC = "select
					tab.muncod,
					count(tab.muncod) as cont
				from (
				select
					aed.aedid,
					ass.muncod,
					esdO.esddsc as origem,
					esdO.esdid as esdorigem,
					esdD.esddsc as destino,
					esdD.esdid as esddestino,
					mat.matid
				from sase.assessoramento ass
				left join sase.usuarioresponsabilidade rpu on ass.muncod = rpu.muncod
				left join sase.municipiosassistidosredeae mar on mar.assid = ass.assid and mar.ravid = {$ravid}
				left join workflow.historicodocumento hst on hst.docid = ass.docid
				left join workflow.acaoestadodoc aed on aed. aedid = hst.aedid
				left join workflow.estadodocumento esdO on esdO.esdid = aed.esdidorigem
				left join workflow.estadodocumento esdD on esdD.esdid = aed.esdiddestino
				left join sase.municipiosalteracaoperiodotecredeae mat on mat.marid = mar.marid and mat.esdid_ant = esdO.esdid and mat.esdid_atual = esdD.esdid
				where rpu.usucpf = '{$usucpf}'
				and (date(htddata) >= date('{$ratdata1periodo}') and date(htddata) <= date('{$ratdata2periodo}'))
				and rpu.rpustatus = 'A'
				group by aed.aedid, ass.muncod, esdO.esddsc, esdO.esdid, esdD.esddsc, esdD.esdid, mat.matid
				) as tab
				inner join territorios.municipio mun on tab.muncod = mun.muncod
				group by tab.muncod";
        $resC = $db->carregar($sqlC);

        $resC2 = array();

        if (is_array($resC)){
            foreach ($resC as $r){
                $resC2[$r['muncod']] = $r['cont'];
            }
        }
        //ver($resC2);

        //verifica perfil relatorio
        $sql    = "SELECT
				pu.pflcod
			FROM
				seguranca.perfilusuario pu
			INNER JOIN
				seguranca.perfil p ON p.pflcod = pu.pflcod
								  AND p.sisid = " . SASE_SISID . "
			WHERE
				pu.usucpf = '" . $usucpf . "'
			ORDER BY
				p.pflnivel";
        $pfls = $db->carregarColuna( $sql );

        if (is_array($pfls)){
            if (in_array(PFLCOD_SASE_TECNICO, $pfls)){
                $pflcod = PFLCOD_SASE_TECNICO;
            } else if (in_array(PFLCOD_SASE_SUPERVISOR, $pfls)){
                $pflcod = PFLCOD_SASE_SUPERVISOR;
            }
        }

        if($pflcod == PFLCOD_SASE_SUPERVISOR) {

            //pega os ravid dos tecnicos supervisionados
            $cpfTecnicos = $this->retornaTecnicosSupervisionados();

            if ($cpfTecnicos) {
                $sql = "SELECT ravid FROM sase.relatorioavaliadorredeae
                        where usucpf in ('" . implode("','", $cpfTecnicos) . "') and ratdata1periodo = '{$this->arAtributos['ratdata1periodo']}' and ratdata2periodo = '{$this->arAtributos['ratdata2periodo']}' and ratperiodo = {$this->arAtributos['ratperiodo']}";
                $arrayRavid = $db->carregarColuna($sql);
            }

            if (is_array($arrayRavid) && count($arrayRavid) > 0) {
                $sql = "select
                        tab.aedid,
                        mun.mundescricao,
                        mun.muncod,
                        tab.origem,
                        tab.esdorigem,
                        tab.origemordem,
                        tab.destino,
                        tab.esddestino,
                        tab.matid
                    from (
                    select
                        aed.aedid,
                        ass.muncod,
                        esdO.esddsc as origem,
                        esdO.esdid as esdorigem,
                        esdO.esdordem as origemordem,
                        esdD.esddsc as destino,
                        esdD.esdid as esddestino,
                        mat.matid
                    from sase.assessoramento ass
                    left join sase.usuarioresponsabilidade rpu on ass.muncod = rpu.muncod
                    inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid and mar.ravid in (" . implode(",", $arrayRavid) . ")
                    left join workflow.historicodocumento hst on hst.docid = ass.docid and hst.htddata = (select max(htddata) from workflow.historicodocumento where docid = ass.docid and date(htddata) <= date('{$ratdata2periodo}'))
                    left join workflow.acaoestadodoc aed on aed. aedid = hst.aedid
                    left join workflow.estadodocumento esdO on esdO.esdid = aed.esdidorigem
                    left join workflow.estadodocumento esdD on esdD.esdid = aed.esdiddestino
                    left join sase.municipiosalteracaoperiodotecredeae mat on mat.marid = mar.marid and mat.esdid_ant = esdO.esdid and mat.esdid_atual = esdD.esdid
                    where rpu.usucpf = '{$usucpf}'
                    and (date(htddata) >= date('{$ratdata1periodo}') and date(htddata) <= date('{$ratdata2periodo}'))
                    --and to_char(htddata, 'dd/mm/yyyy') between '{$ratdata1periodo}' and '{$ratdata2periodo}'
                    and rpu.rpustatus = 'A'
                    group by aed.aedid, ass.muncod, esdO.esddsc, esdO.esdid, esdO.esdordem, esdD.esddsc, esdD.esdid, mat.matid
                                    ) as tab
                    inner join territorios.municipio mun on tab.muncod = mun.muncod
                    order by tab.origemordem";
            } else {
                $sql = "select
                        tab.aedid,
                        mun.mundescricao,
                        mun.muncod,
                        tab.origem,
                        tab.esdorigem,
                        tab.origemordem,
                        tab.destino,
                        tab.esddestino,
                        tab.matid
                    from (
                    select
                        aed.aedid,
                        ass.muncod,
                        esdO.esddsc as origem,
                        esdO.esdid as esdorigem,
                        esdO.esdordem as origemordem,
                        esdD.esddsc as destino,
                        esdD.esdid as esddestino,
                        mat.matid
                    from sase.assessoramento ass
                    left join sase.usuarioresponsabilidade rpu on ass.muncod = rpu.muncod
                    inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid and mar.ravid = {$ravid}
                    left join workflow.historicodocumento hst on hst.docid = ass.docid and hst.htddata = (select max(htddata) from workflow.historicodocumento where docid = ass.docid and date(htddata) <= date('{$ratdata2periodo}'))
                    left join workflow.acaoestadodoc aed on aed. aedid = hst.aedid
                    left join workflow.estadodocumento esdO on esdO.esdid = aed.esdidorigem
                    left join workflow.estadodocumento esdD on esdD.esdid = aed.esdiddestino
                    left join sase.municipiosalteracaoperiodotecredeae mat on mat.marid = mar.marid and mat.esdid_ant = esdO.esdid and mat.esdid_atual = esdD.esdid
                    where rpu.usucpf = '{$usucpf}'
                    and (date(htddata) >= date('{$ratdata1periodo}') and date(htddata) <= date('{$ratdata2periodo}'))
                    --and to_char(htddata, 'dd/mm/yyyy') between '{$ratdata1periodo}' and '{$ratdata2periodo}'
                    and rpu.rpustatus = 'A'
                    group by aed.aedid, ass.muncod, esdO.esddsc, esdO.esdid, esdO.esdordem, esdD.esddsc, esdD.esdid, mat.matid
                                    ) as tab
                    inner join territorios.municipio mun on tab.muncod = mun.muncod
                    order by tab.origemordem";
            }
        }else{

            $sql = "select
                        tab.aedid,
                        mun.mundescricao,
                        mun.muncod,
                        tab.origem,
                        tab.esdorigem,
                        tab.origemordem,
                        tab.destino,
                        tab.esddestino,
                        tab.matid
                    from (
                    select
                        aed.aedid,
                        ass.muncod,
                        esdO.esddsc as origem,
                        esdO.esdid as esdorigem,
                        esdO.esdordem as origemordem,
                        esdD.esddsc as destino,
                        esdD.esdid as esddestino,
                        mat.matid
                    from sase.assessoramento ass
                    left join sase.usuarioresponsabilidade rpu on ass.muncod = rpu.muncod
                    inner join sase.municipiosassistidosredeae mar on mar.assid = ass.assid and mar.ravid = {$ravid}
                    left join workflow.historicodocumento hst on hst.docid = ass.docid and hst.htddata = (select max(htddata) from workflow.historicodocumento where docid = ass.docid and date(htddata) <= date('{$ratdata2periodo}'))
                    left join workflow.acaoestadodoc aed on aed. aedid = hst.aedid
                    left join workflow.estadodocumento esdO on esdO.esdid = aed.esdidorigem
                    left join workflow.estadodocumento esdD on esdD.esdid = aed.esdiddestino
                    left join sase.municipiosalteracaoperiodotecredeae mat on mat.marid = mar.marid and mat.esdid_ant = esdO.esdid and mat.esdid_atual = esdD.esdid
                    where rpu.usucpf = '{$usucpf}'
                    and (date(htddata) >= date('{$ratdata1periodo}') and date(htddata) <= date('{$ratdata2periodo}'))
                    --and to_char(htddata, 'dd/mm/yyyy') between '{$ratdata1periodo}' and '{$ratdata2periodo}'
                    and rpu.rpustatus = 'A'
                    group by aed.aedid, ass.muncod, esdO.esddsc, esdO.esdid, esdO.esdordem, esdD.esddsc, esdD.esdid, mat.matid
                                    ) as tab
                    inner join territorios.municipio mun on tab.muncod = mun.muncod
                    order by tab.origemordem";

        }
        //ver($sql);

        $res = $db->carregar($sql);

        $html = <<<HTML
		<style>
			.spanObs{
				font-size: 10px;
			}

			.table{
				margin-bottom: 0px !important;
			}
		</style>

		<div id="listagem" style="background: #ffffff !important;">
HTML;

        if (!$campos && !$layout){
            $html .= <<< HTML
                <table border="1" cellspacing="0" cellpadding="5" width="695" class="col-md-12 table-condensed table-hover table-responsive" >
HTML;
        } else {
            $html .= <<< HTML
                <table class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }

        $html .= <<<HTML

				<thead>
					<tr>
						<th>Nome do Município</th>
						<th>Etapa anterior</th>
						<th>Etapa atual</th>
					</tr>
				</thead>
				<tbody>
HTML;
        $esdorigem = '';
        $i = 0;
        $t = 0;
        //ver(array_count_values($res));

        if (is_array($res)){
            foreach ($res as $r) {
                if ($esdorigem == '') {
                    $esdorigem = $r['esdorigem'];
                } else {
                    if ($r['esdorigem'] != $esdorigem){
                        $html .= <<<HTML
									<tr>
										<th colspan="2" style="text-align: right;">Total:</th>
										<th>{$i}</th>
									</tr>
HTML;
                        $i = 0;
                        $esdorigem = $r['esdorigem'];
                    }
                }
                if ($resC2[$r['muncod']] > 1){
                    $stilo = <<<HTML
                                    style="color: red;"
HTML;

                    $valor = <<<HTML
                                    *{$r['mundescricao']}
HTML;
                } else {
                    $stilo = "";

                    $valor = <<<HTML
                                    {$r['mundescricao']}
HTML;

                }
                $i2 = $i + 1;
                $html .= <<<HTML
							<tr>
								<td>
                                    <input type="hidden" name="contq3[]" value="{$i2}" />
									<input type="hidden" name="muncodq3[]" value="{$r['muncod']}" />
									<input type="hidden" name="esdorigemq3[]" value="{$r['esdorigem']}" />
									<input type="hidden" name="esddestinoq3[]" value="{$r['esddestino']}" />
									<input type="hidden" name="matidq3[]" value="{$r['matid']}" />
									<div {$stilo}>
										{$valor}
									</div>
								</td>
								<td>{$r['origem']}</td>
								<td>{$r['destino']}</td>
							</tr>
HTML;
                $i++;
                $t++;
            }
            $html .= <<<HTML
						<tr>
							<th colspan="2" style="text-align: right;">Total:</th>
							<th>{$i}</th>
						</tr>
						<tr>
							<th colspan="2" style="text-align: right;">Total Geral:</th>
							<th>{$t}</th>
						</tr>
HTML;
        } else {
            $html .= <<<HTML
						<tr>
							<td colspan="3" style="text-align: center;">Sem registro</td>
						</tr>
HTML;
        }
        $html .= <<<HTML
				</tbody>
			</table>
			<span class="spanObs">*Observação: Municípios exibidos na cor vermelha, passaram por mais de uma situação no período deste relatório.</span>
		</div>
HTML;


        if ($pflcod == PFLCOD_SASE_SUPERVISOR && !$layout) {
            $ravinfcomplqdo = $this->arAtributos['ravinfcomplqdo3'] != '' ? $this->arAtributos['ravinfcomplqdo3'] : '&nbsp;';
            $html .= <<<HTML
                <h3>Comentários gerais sobre avanços e dificultades relacionados às alterações das etapas de trabalho pelos Municípios no período.</h3>
                <div style="text-align: justify; border: 1px solid #000; padding: 5px; min-height: 60px; height: 150px;">
                    {$ravinfcomplqdo}
                </div>

HTML;
        }

        return $html;

    }

    public function getMunicipioSemAlteracao($campos = false, $layout = false){
        global $db, $data;

        //$usucpf = ''; //$_REQUEST['usucpf'];
        //$ratdata1periodo = '02/02/2015'; //$_REQUEST['ratdata1periodo'];
        //$ratdata2periodo = '11/02/2015'; //$_REQUEST['ratdata2periodo'];

        $usucpf = $this->arAtributos['usucpf'];
        $ratdata1periodo = $data->formataData($this->arAtributos['ratdata1periodo']);
        $ratdata2periodo = $data->formataData($this->arAtributos['ratdata2periodo']);
        $ravid = $this->arAtributos['ravid'];
        $pflcod = $this->arAtributos['pflcod'];


        //verifica perfil relatorio
        $sql    = "SELECT
				pu.pflcod
			FROM
				seguranca.perfilusuario pu
			INNER JOIN
				seguranca.perfil p ON p.pflcod = pu.pflcod
								  AND p.sisid = " . SASE_SISID . "
			WHERE
				pu.usucpf = '" . $usucpf . "'
			ORDER BY
				p.pflnivel";
        $pfls = $db->carregarColuna( $sql );

        if (is_array($pfls)){
            if (in_array(PFLCOD_SASE_TECNICO, $pfls)){
                $pflcod = PFLCOD_SASE_TECNICO;
            } else if (in_array(PFLCOD_SASE_SUPERVISOR, $pfls)){
                $pflcod = PFLCOD_SASE_SUPERVISOR;
            }
        }

        if($pflcod == PFLCOD_SASE_SUPERVISOR) {

            //pega os ravid dos tecnicos supervisionados
//            $sql = "select
//                            usu.usucpf
//                        from seguranca.usuario usu
//                        inner join sase.tecnicosupervisionadosredeae tsp on tsp.cpftecnico = usu.usucpf and tsp.tspid = (select max(tspid) from sase.tecnicosupervisionadosredeae where cpftecnico = tsp.cpftecnico and muncod = tsp.muncod and ravid = {$ravid} )
//                        where ravid = {$ravid}
//                        group by usu.usucpf";
//            $cpfTecnicos = $db->carregarColuna($sql);

            $cpfTecnicos = $this->retornaTecnicosSupervisionados();

            if ($cpfTecnicos) {
                $sql = "SELECT ravid FROM sase.relatorioavaliadorredeae
                        where usucpf in ('" . implode("','", $cpfTecnicos) . "') and ratdata1periodo = '{$this->arAtributos['ratdata1periodo']}' and ratdata2periodo = '{$this->arAtributos['ratdata2periodo']}' and ratperiodo = {$this->arAtributos['ratperiodo']}";
                $arrayRavid = $db->carregarColuna($sql);
            }

            if (is_array($arrayRavid) && count($arrayRavid)) {
                $sql = "select distinct
                        case
                            when esd.esddsc is null then 'Sem histórico'
                            when esd.esddsc is not null then esd.esddsc
                        end as esddsc,
                        esd.esdordem,
                        esd.esdid,
                        mun.muncod,
                        mun.mundescricao,
                        mst.sciid,
                        mst.msaobservacoes,
                        mst.mstid,
                        sci.scidsc
                    from sase.assessoramento ass
                    inner join sase.municipiosassistidosredeae mar on ass.assid = mar.assid and mar.ravid in (" . implode(",", $arrayRavid) . ")
                    left join sase.municipiossemalteracaoperiodotecredeae mst on mst.marid = mar.marid and mst.mstid = (select max(mstid) from sase.municipiossemalteracaoperiodotecredeae where marid = mar.marid)
                    left join sase.subetapacomissaoinsttecredeae sci on mst.sciid = sci.sciid
                    inner join workflow.historicodocumento hst on hst.docid = ass.docid
                        and hst.htddata = (select max(htddata) from workflow.historicodocumento where docid = ass.docid and date(htddata) <= date('{$ratdata1periodo}'))
                    inner join workflow.acaoestadodoc aed on aed.aedid = hst.aedid
                    inner join workflow.estadodocumento esd on esd.esdid = aed.esdiddestino
                    inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
                    inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
                    inner join territorios.municipio mun on mun.muncod = ass.muncod
                    where ur.usucpf = '{$usucpf}'
                    and ur.rpustatus = 'A'
                    and not exists (
                        select
                            1
                        from workflow.historicodocumento doc
                        where doc.docid = ass.docid
                        and date(doc.htddata) between date('$ratdata1periodo') and date('$ratdata2periodo')
                    )
                    order by esd.esdordem";
            } else {
                $sql = "select distinct
                    case
                        when esd.esddsc is null then 'Sem histórico'
                        when esd.esddsc is not null then esd.esddsc
                    end as esddsc,
                    esd.esdordem,
                    esd.esdid,
					mun.muncod,
					mun.mundescricao,
                    mst.sciid,
                    mst.msaobservacoes,
                    mst.mstid,
                    sci.scidsc
				from sase.assessoramento ass
                inner join sase.municipiosassistidosredeae mar on ass.assid = mar.assid and mar.ravid = {$ravid}
                left join sase.municipiossemalteracaoperiodotecredeae mst on mst.marid = mar.marid and mst.mstid = (select max(mstid) from sase.municipiossemalteracaoperiodotecredeae where marid = mar.marid)
                left join sase.subetapacomissaoinsttecredeae sci on mst.sciid = sci.sciid
                left join workflow.historicodocumento hst on hst.docid = ass.docid
                    and hst.htddata = (select max(htddata) from workflow.historicodocumento where docid = ass.docid and date(htddata) <= date('{$ratdata1periodo}'))
                left join workflow.acaoestadodoc aed on aed.aedid = hst.aedid
                left join workflow.estadodocumento esd on esd.esdid = aed.esdiddestino
				inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
				inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
				inner join territorios.municipio mun on mun.muncod = ass.muncod
				where ur.usucpf = '{$usucpf}'
				and ur.rpustatus = 'A'
				and not exists (
					select
						1
					from workflow.historicodocumento doc
					where doc.docid = ass.docid
					and date(doc.htddata) between date('$ratdata1periodo') and date('$ratdata2periodo')
				)
				order by esd.esdordem";
            }
        }else{

            $sql = "select distinct
                    case
                        when esd.esddsc is null then 'Sem histórico'
                        when esd.esddsc is not null then esd.esddsc
                    end as esddsc,
                    esd.esdordem,
                    esd.esdid,
					mun.muncod,
					mun.mundescricao,
                    mst.sciid,
                    mst.msaobservacoes,
                    mst.mstid,
                    sci.scidsc
				from sase.assessoramento ass
                inner join sase.municipiosassistidosredeae mar on ass.assid = mar.assid and mar.ravid = {$ravid}
                left join sase.municipiossemalteracaoperiodotecredeae mst on mst.marid = mar.marid and mst.mstid = (select max(mstid) from sase.municipiossemalteracaoperiodotecredeae where marid = mar.marid)
                left join sase.subetapacomissaoinsttecredeae sci on mst.sciid = sci.sciid
                left join workflow.historicodocumento hst on hst.docid = ass.docid
                    and hst.htddata = (select max(htddata) from workflow.historicodocumento where docid = ass.docid and date(htddata) <= date('{$ratdata1periodo}'))
                left join workflow.acaoestadodoc aed on aed.aedid = hst.aedid
                left join workflow.estadodocumento esd on esd.esdid = aed.esdiddestino
				inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
				inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
				inner join territorios.municipio mun on mun.muncod = ass.muncod
				where ur.usucpf = '{$usucpf}'
				and ur.rpustatus = 'A'
				and not exists (
					select
						1
					from workflow.historicodocumento doc
					where doc.docid = ass.docid
					and date(doc.htddata) between date('$ratdata1periodo') and date('$ratdata2periodo')
				)
				order by esd.esdordem";

        }
        //ver($sql);
        $res = $db->carregar($sql);

        $html = <<<HTML
		<div id="listagem" style="background: #ffffff !important;">
HTML;
//$html .= $sql;
        if (!$layout){
            $html .= <<< HTML
                <table border="1" cellspacing="0" cellpadding="5" width="695" class="col-md-12 table-condensed table-hover table-responsive" >
HTML;
        } else {
            $html .= <<< HTML
                <table class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }

        $html .= <<<HTML
				<tr>
					<th>Município</th>
					<th>Etapa</th>
					<th>Sub-Etapa</th>
					<th>Motivo</th>
				</tr>
HTML;
        if (is_array($res)){

            $stacod = '';
            $i = 0;
            $t = 0;
            foreach ($res as $r){
                if ($stacod == '') {
                    $stacod = $r['esdid'];
                } else {
                    if ($r['esdid'] != $stacod){
                        $html .= <<<HTML
								<tr>
									<th colspan="3" style="text-align: right;">Total:</th>
									<th>{$i}</th>
								</tr>
HTML;
                        $i = 0;
                        $stacod = $r['esdid'];
                    }
                }

                $html .= <<<HTML
						<tr>
							<td>
                                <input name="mstidq4[]" type="hidden" value="{$r['mstid']}" />
                                {$r['mundescricao']}
                            </td>
							<td>{$r['esddsc']}</td>
							<td style="max-width: 250px; min-width: 100px;">
								<input type="hidden" name="muncodq4[]" value="{$r['muncod']}" />
								<input type="hidden" name="esdidq4[]" value="{$r['esdid']}" />
HTML;

                if ($campos && $stacod != ''){

                    $sql = "select sciid as codigo, scidsc as descricao From sase.subetapacomissaoinsttecredeae where stacod = (select stacod from sase.situacaoassessoramento where esdid = {$stacod})";
                    $dados = $db->carregar($sql);

                    if (is_array($dados)) {
                        $sciid = getOptions($dados, array('prompt' => ' Selecione...'), 'sciid[]', null, $r['sciid']);

                        $html .= $sciid;
                    } else {
                        $html .= "&nbsp;";
                    }

                    ?>

                <?php
                } else {
                    $html .= $r['scidsc'];
                }
                $html .= <<<HTML
							</td>
							<td style="max-width: 400px; min-width: 200px;">
HTML;

                if ($campos){
                    $html .= <<<HTML
								<textarea class="form-control" maxlength="200" id="msaobservacoes" name="msaobservacoes[]"  rows="2" cols="5" >{$r['msaobservacoes']}</textarea>
HTML;
                } else {
                    $html .= $r['msaobservacoes'];
                }
                $html .= <<<HTML
							</td>
						</tr>
HTML;
                $i++;
                $t++;
            }
            $html .= <<<HTML
					<tr>
						<th colspan="3" style="text-align: right;">Total:</th>
						<th>{$i}</th>
					</tr>
					<tr>
						<th colspan="3" style="text-align: right;">Total Geral:</th>
						<th>{$t}</th>
					</tr>
HTML;

        } else {
            $html .= <<<HTML
					<tr>
						<td colspan="4" style="text-align: center;">Sem registro</td>
					</tr>
HTML;
        }
        $html .= <<<HTML
			</table>
		</div>
HTML;

        if ($pflcod == PFLCOD_SASE_SUPERVISOR && !$layout) {
            $ravinfcomplqdo = $this->arAtributos['ravinfcomplqdo4'] != '' ? $this->arAtributos['ravinfcomplqdo4'] : '&nbsp;';
            $html .= <<<HTML
                <h3>Comentários gerais sobre avanços e dificuldades relacionados à permanência dos Municípios na mesma etapa de trabalho no período:</h3>
                <div style="text-align: justify; border: 1px solid #000; padding: 5px; min-height: 60px; height: 150px;">
                    {$ravinfcomplqdo}
                </div>

HTML;
        }

        return $html;

    }

    public function getMunicipiosSemInfo($campos = false, $layout = true) {
        global $db, $data;

        //$usucpf = ''; //$_REQUEST['usucpf'];
        //$ratdata1periodo = '02/02/2015'; //$_REQUEST['ratdata1periodo'];
        //$ratdata2periodo = '11/02/2015'; //$_REQUEST['ratdata2periodo'];

        $usucpf = $this->arAtributos['usucpf'];
        //$ratdata1periodo = $data->formataData($this->arAtributos['ratdata1periodo']);
        //$ratdata2periodo = $data->formataData($this->arAtributos['ratdata2periodo']);
        $ratdata1periodo = str_replace(" 00:00:00","",$this->arAtributos['ratdata1periodo']);
        $ratdata2periodo = str_replace(" 00:00:00","",$this->arAtributos['ratdata2periodo']);
        $ravid = $this->arAtributos['ravid'];
        $pflcod = $this->arAtributos['pflcod'];
        /*
        $sql = "select
					mun.muncod,
					--sta.stadsc,
					--sta.esdid,
                    aed.aeddscrealizada as stadsc,
					aed.esdiddestino as esdid,
					mun.mundescricao,
                    msf.msfid,
                    msf.mstobservacoes
				from sase.assessoramento ass
                inner join sase.municipiosassistidosredeae mar on ass.assid = mar.assid and mar.ravid = {$ravid}
                left join sase.municipiosseminftecredeae msf on mar.marid = msf.marid
				inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
				--inner join sase.situacaoassessoramento sta on ass.stacod = sta.stacod
				inner join territorios.municipio mun on mun.muncod = ass.muncod
				inner join workflow.historicodocumento hst on hst.docid = ass.docid and htddata <= '{$ratdata2periodo} 23:59:59'
				inner join workflow.acaoestadodoc aed on aed.aedid = hst.aedid and aed.esdiddestino = 1118
				where ur.usucpf = '{$usucpf}'
				--and to_char(htddata, 'dd/mm/yyyy') between '{$ratdata1periodo}' and '{$ratdata2periodo}'
				--and sta.esdid in (".ESDID_SASE_SEM_INFORMACAO.", ".ESDID_SASE_SEM_COMISSAO_COORDENADORA_CONSTITUIDA.")
				and ur.rpustatus = 'A'
				order by 4";
        */

        $sql = "-- busca Sem Comissão Coordenadora Instituída
                select
                            mun.muncod,
                            aed.aeddscrealizada as stadsc,
                            aed.esdiddestino as esdid,
                            mun.mundescricao,
                            msf.msfid,
                            msf.mstobservacoes
                from sase.assessoramento ass
                inner join sase.municipiosassistidosredeae mar on ass.assid = mar.assid and mar.ravid = {$ravid}
                left join sase.municipiosseminftecredeae msf on mar.marid = msf.marid
                inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
                inner join territorios.municipio mun on mun.muncod = ass.muncod
                inner join workflow.historicodocumento hst on hst.docid = ass.docid and htddata <= '{$ratdata2periodo} 23:59:59' and hst.htddata = (select max(htddata)  from workflow.historicodocumento where docid = hst.docid and htddata <= '{$ratdata2periodo} 23:59:59')
                inner join workflow.acaoestadodoc aed on aed.aedid = hst.aedid and aed.esdiddestino = 1118
                where ur.usucpf = '{$usucpf}'
                and ur.rpustatus = 'A'

                union

                --sem informação
                select
                            mun.muncod,
                            esd.esddsc as stadsc,
                            esd.esdid as esdid,
                            mun.mundescricao,
                            msf.msfid,
                            msf.mstobservacoes
                from sase.assessoramento ass
                inner join sase.municipiosassistidosredeae mar on ass.assid = mar.assid and mar.ravid = {$ravid}
                left join sase.municipiosseminftecredeae msf on mar.marid = msf.marid
                inner join sase.usuarioresponsabilidade ur on ass.muncod = ur.muncod
                inner join territorios.municipio mun on mun.muncod = ass.muncod
                inner join workflow.documento doc on doc.docid = ass.docid and docdatainclusao <= '{$ratdata2periodo} 23:59:59'
                inner join workflow.estadodocumento esd on esd.esdid = doc.esdid and esd.esdid = 1117
                where ur.usucpf = '{$usucpf}'
                and ur.rpustatus = 'A'

                order by 4";

        //ver($sql,d);
        $res = $db->carregar($sql);

        $html = <<<HTML
		<div id="listagem" style="background: #ffffff !important;">
HTML;

        if (!$campos && !$layout){
            $html .= <<< HTML
                <table border="1" cellspacing="0" cellpadding="5" width="695" class="col-md-12 table-condensed table-hover table-responsive" >
HTML;
        } else {
            $html .= <<< HTML
                <table class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }
        $html .= <<<HTML
					<tr>
						<th>Município</th>
						<th>Etapa</th>
						<th>Motivo da situação?</th>
					</tr>
HTML;

        if (is_array($res)){

            $stacod = '';
            $i = 0;
            $t = 0;
            foreach ($res as $r){
                if ($stacod == '') {
                    $stacod = $r['stacod'];
                } else {
                    if ($r['stacod'] != $stacod){
                        $html .= <<<HTML
								<tr>
									<th colspan="2" style="text-align: right;">Total:</th>
									<th>{$i}</th>
								</tr>
HTML;
                        $i = 0;
                        $stacod = $r['stacod'];
                    }
                }
                $html .= <<<HTML
						<tr>
							<td>
								<input type="hidden" name="muncodq5[]" value="{$r['muncod']}" />
								<input type="hidden" name="esdidq5[]" value="{$r['esdid']}" />
                                <input type="hidden" name="msfidq5[]" value="{$r['msfid']}" />
								{$r['mundescricao']}
							</td>
							<td>{$r['stadsc']}</td>
							<td>
HTML;
                if ($campos){
                    $html .= <<<HTML
									<textarea class="form-control" id="mstobservacoes" maxlength="200" name="mstobservacoes[]" rows="2" cols="5" >{$r['mstobservacoes']}</textarea>
HTML;
                } else {
                    $html .= $r['mstobservacoes'];
                }

                $html .= <<<HTML
							</td>
						</tr>
HTML;
                $i++;
                $t++;
            }
            $html .= <<<HTML
					<tr>
						<th colspan="2" style="text-align: right;">Total:</th>
						<th>{$i}</th>
					</tr>
					<tr>
						<th colspan="2" style="text-align: right;">Total Geral:</th>
						<th>{$t}</th>
					</tr>
HTML;
        } else {
            $html .= <<<HTML
					<tr>
						<td colspan="3" style="text-align: center;">Sem registro</td>
					</tr>
HTML;
        }
        $html .= <<<HTML
			</table>
		</div>
HTML;

        if ($pflcod == PFLCOD_SASE_SUPERVISOR && !$layout) {
            $ravinfcomplqdo = $this->arAtributos['ravinfcomplqdo5'] != '' ? $this->arAtributos['ravinfcomplqdo5'] : '&nbsp;';
            $html .= <<<HTML
                <h3>Comentários gerais sobre avanços e dificuldades relacionados à permanência dos Municípios na mesma etapa de trabalho no período:</h3>
                <div style="text-align: justify; border: 1px solid #000; padding: 5px; min-height: 60px; height: 150px;">
                    {$ravinfcomplqdo}
                </div>

HTML;
        }

        return $html;

    }

    public function getListaMunicipiosAssistidos($layout = false){
        global $db;

        $ravid  = $this->arAtributos['ravid'];
        $aveid  = $this->arAtributos['aveid'];
        $usucpf = $this->arAtributos['usucpf'];

        $sqlAve = "select avenome, mundescricao from sase.avaliadoreducacional ave inner join territorios.municipio mun on ave.muncod = mun.muncod where aveid = {$aveid}";

        $avenome = $db->pegaUm($sqlAve, 'avenome');
        $mundesc = $db->pegaUm($sqlAve, 'mundescricao');
        $data = date('d/m/Y');

        $sql = "select
                    mun.mundescricao,
                    ur.rpuid
                from sase.municipiosassistidosredeae mar
                inner join sase.assessoramento ass on mar.assid = ass.assid
                inner join territorios.municipio mun on mun.muncod = ass.muncod
                left join sase.usuarioresponsabilidade ur on ur.muncod = mun.muncod and ur.usucpf = '{$usucpf}' and ur.rpustatus = 'A'
                where ravid = {$ravid}
                order by mun.mundescricao";

        $res = $db->carregar($sql);

        $html = <<<HTML
        <div id="listagem" style="background: #ffffff !important; align-self: center; border: none;">
HTML;
        if (!$layout){
            $html .= <<<HTML
                <table border="1" cellspacing="0" cellpadding="5" width="695" class="col-md-12 table-condensed table-hover table-responsive" >
HTML;
        } else {
            $html .= <<<HTML
                <table class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }
        $html .= <<<HTML
                <thead>
                    <tr>
                        <th colspan="2">
                            <div style="text-align: center;">
                                MUNICÍPIOS ASSISTIDOS EM ORDEM ALFABÉTICA
                            </div>
                        </th>
                    </tr>
                </thead>
                <tbody>
HTML;

        if (is_array($res)) {
            $i  = 1;
            $rp = 1;
            $p  = 1;
            foreach ($res as $r) {
                if ($rp > 23 and $p == 1 && !$layout){
                    $css = 'class="pagina"';
                    $html .= <<<HTML
                    </tbody>
                </table>
                <table border="1" cellspacing="0" cellpadding="5" width="695" class="col-md-12 table-condensed table-hover table-responsive pagina" >
                    <thead>
                        <tr>
                            <th colspan="2">
                                <div style=\"text-align: center;\">
                                    MUNICÍPIOS ASSISTIDOS EM ORDEM ALFABÉTICA
                                </div>
                            </th>
                        </tr>
                    </thead>
                    <tbody>
HTML;
                    $p++;
                    $rp = 1;
                } else {
                    if ($rp > 30){
                        $css = 'class="pagina"';
                        $html .= <<<HTML
                    </tbody>
                </table>
                <table border="1" cellspacing="0" cellpadding="5" width="695" class="col-md-12 table-condensed table-hover table-responsive pagina" >
                    <thead>
                        <tr>
                            <th colspan="2">
                                <div style=\"text-align: center;\">
                                    MUNICÍPIOS ASSISTIDOS EM ORDEM ALFABÉTICA
                                </div>
                            </th>
                        </tr>
                    </thead>
                    <tbody>
HTML;
                        $p++;
                        $rp = 1;
                    }
                }
                $t = $r['rpuid'] != '' ? $r['mundescricao'] : $r['mundescricao'] . ' <label style="color: #ccc;"> - Não associado </label>';
                $html .="
                <tr>
                    <td style=\"text-align: center;\" width=\"30\">
                        {$i}
                    </td>
                    <td>
                        {$t}
                    </td>
                </tr>";
                $i++;
                $rp++;
            }
        }

        $html .= <<<HTML
                </tbody>
            </table>
        </div>
        <br/>
HTML;
        if (!$layout) {
            $html .= <<<HTML
        <div style="width: 100%; text-align: right;">
            {$avenome}
        </div>
        <div style="width: 100%; text-align: center;">
            {$mundesc}, {$data}
        </div>
HTML;
        }

        return $html;

    }

    public function getListaTecnicosSupervisionados($layout = false){
        global $db;

        $ravid = $this->arAtributos['ravid'];
        $aveid = $this->arAtributos['aveid'];

        $sqlAve = "select avenome, mundescricao from sase.avaliadoreducacional ave inner join territorios.municipio mun on ave.muncod = mun.muncod where aveid = {$aveid}";

        $avenome = $db->pegaUm($sqlAve, 'avenome');
        $mundesc = $db->pegaUm($sqlAve, 'mundescricao');
        $data = date('d/m/Y');

        /*
        $sql = "select
                    usu.usunome,
                    count(*) as qtdmun
                from sase.tecnicosupervisionadosredeae tsp
                inner join seguranca.usuario usu on tsp.cpftecnico = usu.usucpf
                where ravid = {$ravid}
                group by usu.usunome";
        */
//        $sql = "select
//                    usu.usunome,
//                    count(tsp.muncod) as qtdmun
//                from seguranca.usuario usu
//                inner join sase.tecnicosupervisionadosredeae tsp on tsp.cpftecnico = usu.usucpf and tsp.tspid = (select max(tspid) from sase.tecnicosupervisionadosredeae where cpftecnico = tsp.cpftecnico and muncod = tsp.muncod and ravid = {$ravid} )
//                where ravid = {$ravid}
//                group by usu.usunome
//                order by 1";
        $sql = "select
                    UPPER(ave.avenome) as usunome,
                    tsp.cpftecnico,
                    count(tsp.muncod) as qtdmun
                from sase.tecnicosupervisionadosredeae tsp
                inner join sase.avaliadoreducacional ave on tsp.cpftecnico = ave.avenumcpf
                where ravid = {$ravid}
                and tsp.tspid = (select max(tspid) from sase.tecnicosupervisionadosredeae where cpftecnico = tsp.cpftecnico and muncod = tsp.muncod and ravid = {$ravid} )
                group by ave.avenome, tsp.cpftecnico
                order by 1";
        //ver($sql,d);
        $dados = $db->carregar($sql);

        $html = <<<HTML
        <div id="listagem" style="background: #ffffff !important; align-self: center; border: none;">
HTML;
        if (!$layout){
            $html .= <<<HTML
                <table border="1" cellspacing="0" cellpadding="5" width="695" class="col-md-12 table-condensed table-hover table-responsive" >
HTML;
        } else {
            $html .= <<<HTML
                <table class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }
        $html .= <<<HTML
                    <thead>
                        <tr>
                            <th colspan="2">
                                <div style="text-align: center;">
                                    AE TÉCNICOS SUPERVISIONADOS
                                </div>
                            </th>
                            <th>
                                <div style="text-align: center;">
                                    NÚMERO DE MUNICÍPIOS ASSISTIDOS
                                </div>
                            </th>
                        </tr>
                    </thead>
                    <tbody>
HTML;

        $nr = 1;
        $maxMun = 0;
        if (is_array($dados)) {
            foreach ($dados as $d) {
                $maxMun += $d['qtdmun'];
                $html .= <<<HTML
                        <tr>
                            <td width="30">{$nr}</td>
                            <td>{$d['usunome']}</td>
                            <td>{$d['qtdmun']}</td>
                        </tr>
HTML;
                $nr++;
            }
        }

        $html .= <<<HTML
                        <tr>
                            <td colspan="2">Total de municípios</td>
                            <td>{$maxMun}</td>
                        </tr>
HTML;

        $html .= <<<HTML
                    </tbody>
                </table>
            </div>
HTML;
        if (!$layout) {
            $html .= <<<HTML
        <div style="width: 100%; text-align: right;">
            {$avenome}
        </div>
        <div style="width: 100%; text-align: center; margin-top: 10px;">
            {$mundesc}, {$data}
        </div>
HTML;
        }

        return $html;
    }

    public function getHtmlQuadro6(){
        global $db;

        $ravid = $this->arAtributos['ravid'];
        $pflcod = $this->arAtributos['pflcod'];

        $quadros = array(
            1 => 'Municípios sem informação',
            2 => 'Municipios sem Comissão instituída',
            3 => 'Municípios sem alteração de etapa de trabalho, mas com demora justificável',
            4 => 'Municípios sem alteração da etapa de trabalho com demora maior do que a esperada'
        );

        $sql = "select
                    msa.msaid as codigo,
                    msa.msaacaoproposta,
                    mun.mundescricao,
                    mun.muncod as chaveVal,
                    msa.msaquadro
                from sase.municipiossemacoesdesenvstecredeaee msa
                inner join sase.municipiosassistidosredeae mar on mar.marid = msa.marid
                inner join sase.assessoramento ass on ass.assid = mar.assid
                inner join territorios.municipio mun on mun.muncod = ass.muncod
                where mar.ravid = {$ravid}
                order by msaquadro";

        $dados = $db->carregar($sql);

        $html = <<<HTML
        <style>
            .q6Titulo{
                text-align: center;
                font-weight: bold;
            }
        </style>
		<div id="listagem" style="background: #ffffff !important;">
            <table border="1" cellspacing="0" cellpadding="5" width="695" class="col-md-12 table-condensed table-hover table-responsive" >
HTML;

        foreach ($quadros as $qi => $qv) {
            $cl = 0;
            $html .= <<<HTML
                    <tr>
                        <td class="q6Titulo" colspan="2">
                            {$qv}
                        </td>
                    </tr>
                    <tr>
                        <td style="text-align: center">Município</td>
                        <td style="text-align: center">Ações propostas</td>
                    </tr>
HTML;
            if (is_array($dados)){
                foreach ($dados as $d) {
                    if ($d['msaquadro'] == $qi) {
                        $html .= <<<HTML
                            <tr>
                                <td width="300">{$d['mundescricao']}</td>
                                <td width="395" style="max-width: 300px !important">{$d['msaacaoproposta']}</td>
                            </tr>
HTML;
                        $cl++;
                    }
                }
            }
            if ($cl == 0){
                $html .= <<<HTML
                        <tr>
                            <td colspan="2" style="text-align: center;">
                                Sem Registros
                            </td>
                        </tr>
HTML;

            }
        }

        $html .= <<<HTML
		    </table>
        </div>
HTML;

        if ($pflcod == PFLCOD_SASE_SUPERVISOR) {
            $ravinfcomplqdo = $this->arAtributos['ravinfcomplqdo6'] != '' ? $this->arAtributos['ravinfcomplqdo6'] : '&nbsp;';
            $html .= <<<HTML
                <h3>Comentários gerais sobre avanços e dificuldades relacionados às ações propostas para os municípios no período</h3>
                <div style="text-align: justify; border: 1px solid #000; padding: 5px; min-height: 60px; height: 150px;">
                    {$ravinfcomplqdo}
                </div>

HTML;
        }

        return $html;

    }

    public function geraRelatorio() {
        global $db, $data;

        $dataIni = $data->formataData($this->arAtributos['ratdata1periodo']);
        $dataFim = $data->formataData($this->arAtributos['ratdata2periodo']);
        $ravinfcomplementar = $this->arAtributos['ravinfcomplementar'] != '' ? $this->arAtributos['ravinfcomplementar'] : '&nbsp;';
        $aveid = $this->arAtributos['aveid'];
        $pflcod = $this->arAtributos['pflcod'];

        $sqlAve = "select avenome, mundescricao from sase.avaliadoreducacional ave inner join territorios.municipio mun on ave.muncod = mun.muncod where ave.aveid = {$aveid}";
        $avenome = $db->pegaUm($sqlAve, 'avenome');
        $mundesc = $db->pegaUm($sqlAve, 'mundescricao');

        $dataA = date('d/m/Y');
        //$aveid = $db->pegaUm($sqlAve, 'aveid');

        $estado = $this->pegaEstadoAvaliador();

        switch ($pflcod){
            case PFLCOD_SASE_TECNICO:
                $htmlMunAss    = $this->getListaMunicipiosAssistidos();
                $apresentacao  = "Este documento traz o relatorio quinzenal da assistencia tecnica realizada nos municipios assistidos no trabalho de adequaçao ou elaboração dos planos de educaçao.";
                $objetivogeral = "Apresentar relatório quinzenal da assistência técnica realizada nos municípios assistidos para adequação ou elaboração dos planos de educação ao PNE, considerando: (i) as ações de mobilização, formação, oficinas, reuniões técnicas, atividades nos municípios, análise da consonância dos planos ao PNE entre outras e (ii) os avanços alcançados em cada município assistido.";
                $objetivosespec = <<<HTML
                        <ul>
                            <li>Informar os resultados consolidados do processo de assistência técnica para adequação ou elaboração dos Planos Municipais de Educação nos municípios assistidos, no período de {$dataIni} a {$dataFim}.</li>
                            <li>Informar, de maneira consolidada, as ações desenvolvidas nos municípios assistidos no período.</li>
                            <li>Propor ações a serem desenvolvidas com os municípios que não tiveram progresso no período relatado.</li>
                        </ul>
HTML;
                $txtQuadro1 = "As atividades consolidadas no Quadro 1 foram realizadas no período.";
                $txtQuadro2 = "A partir do trabalho de assistência técnica realizado e dos levantamentos feitos via telefone ou e-mail, foi produzida a sistematização de resultados no período, expressa nos Quadros 2 a 6.";
                break;

            case PFLCOD_SASE_SUPERVISOR:
                $htmlMunAss  = $this->getListaTecnicosSupervisionados();
                $apresentacao = "Este documento traz o relatório quinzenal da assistência técnica realizada nos municípios supervisionados pelos AE Técnicos relacionados.";
                $objetivogeral = "Apresentar relatório quinzenal da assistência técnica para elaboração ou adequação dos planos de educação ao PNE realizada nos municípios supervisionados, considerando as ações de supervisão (planejamento da atuação dos AE Técnicos e apoio nas dificuldades enfrentadas, capacitação de novos AE Técnicos, atividades nos municípios supervisionados, atualização do registro da situação dos municípios, consolidação das avaliações dos Técnicos sobre a consonância dos Planos Municipais de Educação ao PNE, realização de trabalho articulado com os demais supervisores de acordo com as orientações do AE Coordenador, entre outras) e a síntase das atividades dos Técnicos.";
                $objetivosespec = <<<HTML
                        <ul>
                            <li>Informar os resultados consolidados do processo de assistência técnica para adequação ou elaboração dos Planos Municipais de Educação nos municípios assistidos pelos AE Técnicos relacionados, no período de {$dataIni} a {$dataFim}.</li>
                            <li>Informar, de maneira consolidada, as ações desenvolvidas pelos AE Técnicos nos municípios em que atuaram no período.</li>
                            <li>Propor ações a serem desenvolvidas com os municípios que não tiveram progresso no período relatado.</li>
                        </ul>
HTML;
                $txtQuadro1 = "As atividades consolidadas no Quadro 1 foram realizadas por mim e pelos AE Técnicos no período.";
                $txtQuadro2 = "A partir dos relatórios e informações apresentadas pelos AE Técnicos e dos levantamentos feitos via telefone ou e-mail, foi produzida a sistematização de resultados no período, expressa nos Quadros 2 a 6.";
                break;

            default:
                $htmlMunAss  = $this->getListaMunicipiosAssistidos();
                $apresentacao = "Este documento traz o relatorio quinzenal da assistencia tecnica realizada nos municipios assistidos no trabalho de adequaçao ou elaboração dos planos de educaçao.";
                $objetivogeral = "Apresentar relatório quinzenal da assistência técnica realizada nos municípios assistidos para adequação ou elaboração dos planos de educação ao PNE, considerando: (i) as ações de mobilização, formação, oficinas, reuniões técnicas, atividades nos municípios, análise da consonância dos planos ao PNE entre outras e (ii) os avanços alcançados em cada município assistido.";
                $objetivosespec = <<<HTML
                        <ul>
                            <li>Informar os resultados consolidados do processo de assistência técnica para adequação ou elaboração dos Planos Municipais de Educação nos municípios assistidos, no período de {$dataIni} a {$dataFim}.</li>
                            <li>Informar, de maneira consolidada, as ações desenvolvidas nos municípios assistidos no período.</li>
                            <li>Propor ações a serem desenvolvidas com os municípios que não tiveram progresso no período relatado.</li>
                        </ul>
HTML;
                $txtQuadro1 = "As atividades consolidadas no Quadro 1 foram realizadas no período.";
                $txtQuadro2 = "A partir do trabalho de assistência técnica realizado e dos levantamentos feitos via telefone ou e-mail, foi produzida a sistematização de resultados no período, expressa nos Quadros 2 a 6.";
                break;
        }

        $htmlQuadro1 = $this->getListaAtividadesDesenvolvidas(false, false, $pflcod);
        $htmlQuadro2 = $this->getListaResultadosConsolidados(false, false);
        $htmlQuadro3 = $this->getMunicipiosComAlteracaoNaData(false, false);
        $htmlQuadro4 = $this->getMunicipioSemAlteracao(false);
        $htmlQuadro5 = $this->getMunicipiosSemInfo(false, false);
        $htmlQuadro6 = $this->getHtmlQuadro6();


        $html = <<<HTML
        <style media="print">
            .pagina{
                page-break-before: always;
            }
        </style>
        <table border="0" cellspacing="0" cellpadding="5" width="710" class="col-md-12 table-condensed table-hover table-responsive" >
            <tr>
                <td>
                    <div style="text-align: center; width: 100%;">
                        MINISTERIO DA EDUCAÇÃO <br/>
                        SECRETARIA DE ARTICULAÇÃO COM OS SISTEMAS DE ENSINO <br/>
                        DIRETORIA DE COOPERAÇÃO E PLANOS DE ADUCAÇÃO <br/>
                        ESTADO {$estado} <br/>
                    </div>
                </td>
            </tr>
            <tr>
                <td></td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: justify; width: 100%;">
                        RELATÓRIO DOS RESULTADOS DE ELABORAÇÃO/ADEQUAÇÃO DOS PLANOS DE
                        EDUCAÇÃO AO PNE NOS MUNICÍPIOS ASSISTIDOS, NO PERÍODO DE {$dataIni} A {$dataFim}.
                    </div>
                </td>
            </tr>
            <tr>
                <td></td>
            </tr>
            <tr>
                <td>
                    <div>
                        {$htmlMunAss}
                    </div>
                </td>
            </tr>
            <tr class="pagina">
                <table>
                    <tr>
                        <td>
                            <div style="text-align: center;">
                                <b>SUMÁRIO</b>
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <ul style="list-style-type: none;">
                                <li>Apresentação</li>
                                <li>Justificativa</li>
                                <li>Objetivo Geral</li>
                                <li>Objetivos Específicos</li>
                                <li>Atividades desenvolvidas</li>
                                <li>Resultados consolidados alcançados</li>
                                <li>Informaçoes complementares sobre o período avaliado</li>
                                <li>Anexos</li>
                            </ul>
                        </td>
                    </tr>
                </table>
            </tr>
            <tr class="pagina">
                <td></td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: left;">
                        <h3>APRESENTAÇÃO</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: justify;">
                        {$apresentacao}
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: left;">
                        <h3>JUSTIFICATIVA</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: justify; letter-spacing: 0px; word-spacing: -2em; white-space: normal;">
                        A elaboração ou adequação dos Planos Estaduais, Distrital e Municipais em consonância ao Plano Nacional de Educação, Lei 13.005 de 2014, é processo determinante para a instituição do Sistema Nacional de Educação. Os governos precisam planejar e utilizar técnicas e métodos que resultem em políticas públicas para a solução dos problemas educacionais. Elaborar ou adequar os planos de educação é, portanto, uma tarefa técnica e política.
                    </div>
                    <br/>
                    <div style="text-align: justify; letter-spacing: 0px; word-spacing: -2em; white-space: normal;">
                        Considerando que o Artigo 8° da Lei determina que todos os entes federados deverão elaborar ou adequar seus Planos de Educação em consonância ao PNE no prazo de um ano a contar da aprovação da lei, o MEC, o Conselho Nacional dos Secretários de Educação - Consed e a União Nacional dos Dirigentes Municipais de Educação Undime,  constituíram uma rede de assistência técnica visando prestar apoio aos estados, Distrito Federal e municípios no cumprimento desta determinação. O presente relatório apresenta as ações e resultados ocorridos nos municípios assistidos, que decorrem dos trabalhos de assistência técnica direta às equipes responsáveis pelos Planos de Educação.
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: left;">
                        <h3>OBJETIVO GERAL</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: justify;">
                        {$objetivogeral}
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: left;">
                        <h3>OBJETIVOS ESPECÍFICOS</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: justify;">
                       {$objetivosespec}
                    </div>
                </td>
            </tr>
            <tr class="pagina">
                <td>
                    <div>
                        <h3>ATIVIDADES DESENVOLVIDAS</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>
                        {$txtQuadro1}
                    </div>
                </td>
            </tr>
            <tr>
                <td></td>
            </tr>
            <tr>
                <td style="font-weight: bold;">
                    <div>Quadro 1. Atividades executadas no periodo</div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>
                        {$htmlQuadro1}
                    </div>
                </td>
            </tr>
            <tr class="pagina">
                <td>
                    <div>
                        <h3>RESULTADOS CONSOLIDADOS ALCANÇADOS</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>
                        {$txtQuadro2}
                    </div>
                </td>
            </tr>
            <tr>
                <td></td>
            </tr>
            <tr>
                <td style="font-weight: bold;">
                    <div>Quadro 2. Situação atual dos Municípios assistidos com relação às etapas de trabalho propostas para a elaboração ou adequação dos Planos de Educação ao PNE.</div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>
                        {$htmlQuadro2}
                    </div>
                </td>
            </tr>
            <tr class="pagina">
                <td style="font-weight: bold;">
                    <div>Quadro 3. Municípios assistidos que apresentaram alteraçao na sua etapa de trabalho no periodo*.</div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>{$htmlQuadro3}</div>
                </td>
            </tr>
            <tr class="pagina">
                <td style="font-weight: bold;">
                    <div>Quadro 4. Municípios assistidos que permaneceram na mesma etapa do período anterior.</div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>{$htmlQuadro4}</div>
                </td>
            </tr>
            <tr class="pagina">
                <td style="font-weight: bold;">
                    <div>Quadro 5. Municípios ainda sem informação ou sem trabalho iniciado.</div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>{$htmlQuadro5}</div>
                </td>
            </tr>
            <tr class="pagina">
                <td style="font-weight: bold;">
                    <div>Quadro 6. Ações a serem desenvolvidas nos municípios sem informação, sem Comissão instituída e sem alteração da etapa de trabalho.</div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>{$htmlQuadro6}</div>
                </td>
            </tr>
            <tr class="pagina">
                <td>
                    <div style="text-align: left;">
                        <h3>INFORMAÇÕES COMPLEMENTARES SOBRE O PERÍODO AVALIADO</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: justify; border: 1px solid #000; padding: 5px; min-height: 60px; height: 150px;">
                        {$ravinfcomplementar}
                    </div>
                    <br/>
                    <div style="width: 100%; text-align: left;">
                        {$mundesc}, {$dataA}
                    </div>
                    <div style="width: 100%; text-align: left;">
                        {$avenome}
                    </div>
                </td>
            </tr>
        </table>
HTML;

        return $html;

    }

    public function gerarRelatorioSupervisor(){
        global $db, $data;

        $dataIni = $data->formataData($this->arAtributos['ratdata1periodo']);
        $dataFim = $data->formataData($this->arAtributos['ratdata2periodo']);
        $ravinfcomplementar = $this->arAtributos['ravinfcomplementar'] != '' ? $this->arAtributos['ravinfcomplementar'] : '&nbsp;';
        $aveid = $this->arAtributos['aveid'];
        $pflcod = $this->arAtributos['pflcod'];

        $sqlAve = "select avenome, mundescricao from sase.avaliadoreducacional ave inner join territorios.municipio mun on ave.muncod = mun.muncod where ave.aveid = {$aveid}";
        $avenome = $db->pegaUm($sqlAve, 'avenome');
        $mundesc = $db->pegaUm($sqlAve, 'mundescricao');

        $dataA = date('d/m/Y');
        //$aveid = $db->pegaUm($sqlAve, 'aveid');

        $sql = "select
                    UPPER(est.estdescricao) as estdescricao
                from sase.avaliadoreducacional ave
                inner join territorios.estado est on est.estuf = ave.estuf
                where ave.aveid = {$aveid}";

        $res = $db->carregar($sql);
        $estado = $res[0]['estdescricao'];

        switch ($pflcod){
            case PFLCOD_SASE_TECNICO:
                $htmlMunAss  = $this->getListaMunicipiosAssistidos();
                break;

            case PFLCOD_SASE_SUPERVISOR:
                $htmlMunAss  = $this->getListaTecnicosSupervisionados();
                break;

            default:
                $htmlMunAss  = $this->getListaMunicipiosAssistidos();
                break;
        }

        $htmlQuadro1 = $this->getListaAtividadesDesenvolvidas(false, false, PFLCOD_SASE_TECNICO);
        $htmlQuadro2 = $this->getListaResultadosConsolidados(false, false);
        $htmlQuadro3 = $this->getMunicipiosComAlteracaoNaData(false, false);
        $htmlQuadro4 = $this->getMunicipioSemAlteracao(false);
        $htmlQuadro5 = $this->getMunicipiosSemInfo(false, false);
        $htmlQuadro6 = $this->getHtmlQuadro6();

        $html = <<<HTML
        <style media="print">
            .pagina{
                page-break-before: always;
            }
        </style>
        <table border="0" cellspacing="0" cellpadding="5" width="710" class="col-md-12 table-condensed table-hover table-responsive" >
            <tr>
                <td>
                    <div style="text-align: center; width: 100%;">
                        MINISTERIO DA EDUCAÇÃO <br/>
                        SECRETARIA DE ARTICULAÇÃO COM OS SISTEMAS DE ENSINO <br/>
                        DIRETORIA DE COOPERAÇÃO E PLANOS DE ADUCAÇÃO <br/>
                        ESTADO {$estado} <br/>
                    </div>
                </td>
            </tr>
            <tr>
                <td></td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: justify; width: 100%;">
                        RELATÓRIO DOS RESULTADOS DE ELABORAÇÃO/ADEQUAÇÃO DOS PLANOS DE
                        EDUCAÇÃO AO PNE NOS MUNICÍPIOS ASSISTIDOS PELOS TÉCNICOS RELACIONADOS, NO PERÍODO DE {$dataIni} A {$dataFim}.
                    </div>
                </td>
            </tr>
            <tr>
                <td></td>
            </tr>
            <tr>
                <td>
                    <div>
                        {$htmlMunAss}
                    </div>
                </td>
            </tr>
            <tr class="pagina">
                <table>
                    <tr>
                        <td>
                            <div style="text-align: center;">
                                <b>SUMÁRIO</b>
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <ul style="list-style-type: none;">
                                <li>Apresentação</li>
                                <li>Justificativa</li>
                                <li>Objetivo Geral</li>
                                <li>Objetivos Específicos</li>
                                <li>Atividades desenvolvidas</li>
                                <li>Resultados consolidados alcançados</li>
                                <li>Informaçoes complementares sobre o período avaliado</li>
                                <li>Anexos</li>
                            </ul>
                        </td>
                    </tr>
                </table>
            </tr>
            <tr class="pagina">
                <td></td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: left;">
                        <h3>APRESENTAÇÃO</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: justify;">
                        Este documento traz o relatorio quinzenal da assistencia tecnica realizada nos municipios supervisionados pelos AE Tecnicos relacionados.
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: left;">
                        <h3>JUSTIFICATIVA</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: justify; letter-spacing: 0px; word-spacing: -2em; white-space: normal;">
                        A elaboração ou adequação dos Planos Estaduais, Distrital e Municipais em consonância ao PNE é processo determinante para a instituição do Sistema Nacional de Educação. Os governos precisam planejar e utilizar técnicas e métodos que resultem em políticas públicas para a solução dos problemas educacionais. Elaborar ou adequar os planos de educação é, portanto, uma tarefa técnica e política.
                    </div>
                    <br/>
                    <div style="text-align: justify; letter-spacing: 0px; word-spacing: -2em; white-space: normal;">
                        Considerando que o Artigo 8º da Lei 13.005/2014 determina que todos os entes federados deverão elaborar ou adequar seus Planos Educação em consonância ao PNE no prazo de um ano a contar da aprovação da lei, o MEC, o CONSED e a UNDIME constituíram uma rede de assistência técnica visando prestar apoio aos estados e municípios no cumprimento desta determinação. O presente relatório apresenta as ações e resultados ocorridos nos Municípios supervisionados, que decorrem dos trabalhos de supervisão e da assistência técnica direta dos AE Técnicos às comissões responsáveis pelos Planos de Educação.
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: left;">
                        <h3>OBJETIVO GERAL</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: justify;">
                        Apresentar relatório quinzenal da assistência técnica para elaboração ou adequação dos planos de educação ao PNE realizada nos municípios supervisionados, considerando as ações de supervisão (planejamento da atuação dos AE Técnicos e apoio nas dificuldades enfrentadas, capacitação de novos AE Técnicos, atividades nos municípios supervisionados, atualização do registro da situação dos municípios, consolidação das avaliações dos Técnicos sobre a consonância dos Planos Municipais de Educação ao PNE, realização de trabalho articulado com os demais supervisores de acordo com as orientações do AE Coordenador, entre outras) e a síntese das atividades dos Técnicos.
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: left;">
                        <h3>OBJETIVOS ESPECÍFICOS</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: justify;">
                        <ul>
                            <li>Informar os resultados consolidados do  processo de assistência técnica para adequação ou elaboração dos Planos Municipais de Educação nos municípios assistidos pelos AE Técnicos relacionados, no período de {$dataIni} a {$dataFim}.</li>
                            <li>Informar, de maneira consolidada, as ações desenvolvidas pelos AE Técnicos nos municípios em que atuaram no período.</li>
                            <li>Propor ações a serem desenvolvidas com os municípios que não tiveram progresso no período relatado.</li>
                        </ul>
                    </div>
                </td>
            </tr>
            <tr class="pagina">
                <td>
                    <div>
                        <h3>ATIVIDADES DESENVOLVIDAS</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>
                        As atividades consolidadas no Quadro 1 foram realizadas por mim e pelos AE Técnicos no período.
                    </div>
                </td>
            </tr>
            <tr>
                <td></td>
            </tr>
            <tr>
                <td style="font-weight: bold;">
                    <div>Quadro 1. Atividades executadas no periodo</div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>
                        {$htmlQuadro1}
                    </div>
                </td>
            </tr>
            <tr class="pagina">
                <td>
                    <div>
                        <h3>RESULTADOS CONSOLIDADOS ALCANÇADOS</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>
                        A partir do trabalho de assistência técnica realizado e dos levantamentos feitos via telefone ou e-mail, foi produzida a sistematização de resultados no período, expressa nos Quadros 2 a 6.
                    </div>
                </td>
            </tr>
            <tr>
                <td></td>
            </tr>
            <tr>
                <td style="font-weight: bold;">
                    <div>Quadro 2. Situação atual dos Municípios assistidos com relação às etapas de trabalho propostas para a elaboração ou adequação dos Planos de Educação ao PNE.</div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>
                        {$htmlQuadro2}
                    </div>
                </td>
            </tr>
            <tr class="pagina">
                <td style="font-weight: bold;">
                    <div>Quadro 3. Municípios assistidos que apresentaram alteraçao na sua etapa de trabalho no periodo*.</div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>{$htmlQuadro3}</div>
                </td>
            </tr>
            <tr class="pagina">
                <td style="font-weight: bold;">
                    <div>Quadro 4. Municípios assistidos que permaneceram na mesma etapa do período anterior.</div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>{$htmlQuadro4}</div>
                </td>
            </tr>
            <tr class="pagina">
                <td style="font-weight: bold;">
                    <div>Quadro 5. Municípios ainda sem informação ou sem trabalho iniciado.</div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>{$htmlQuadro5}</div>
                </td>
            </tr>
            <tr class="pagina">
                <td style="font-weight: bold;">
                    <div>Quadro 6. Ações a serem desenvolvidas nos municípios sem informação, sem Comissão instituída e sem alteração da etapa de trabalho.</div>
                </td>
            </tr>
            <tr>
                <td>
                    <div>{$htmlQuadro6}</div>
                </td>
            </tr>
            <tr class="pagina">
                <td>
                    <div style="text-align: left;">
                        <h3>INFORMAÇÕES COMPLEMENTARES SOBRE O PERÍODO AVALIADO</h3>
                    </div>
                </td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: justify; border: 1px solid #000; padding: 5px; min-height: 60px; height: 150px;">
                        {$ravinfcomplementar}
                    </div>
                    <br/>
                    <div style="width: 100%; text-align: left;">
                        {$mundesc}, {$dataA}
                    </div>
                    <div style="width: 100%; text-align: left;">
                        {$avenome}
                    </div>
                </td>
            </tr>
        </table>
HTML;

        return $html;

    }

    public function retornaDivJustificado($texto = array()){
        $divIni = <<<HTML
            <div style="text-align: justify; letter-spacing: 0px; white-space: normal;">
HTML;
        $divFim = <<<HTML
            </div>
HTML;
        $retorno = $divIni . implode($divFim."<br/>".$divIni, $texto) . $divFim;
        return $retorno;
    }

    public function retornaDivLista($texto = array()){
        $divIni = <<<HTML
            <ul><li>
HTML;
        $divFim = <<<HTML
            </li></ul>
HTML;
        $retorno = $divIni . implode("</li><li>", $texto) . $divFim;
        return $retorno;
    }

    public function geraRelatorioSuperGeral(){
        global $db, $data;
        //$estado = $this->pegaEstadoAvaliador();
        $dataIni = $data->formataData($this->arAtributos['ratdata1periodo']);
        $dataFim = $data->formataData($this->arAtributos['ratdata2periodo']);
        $mes     = $data->formataData($this->arAtributos['ratdata2periodo'], 'MM');
        $aveid = $this->arAtributos['aveid'];
        $pflcod = $this->arAtributos['pflcod'];
        $sqlAve = "select avenome, ave.estuf, mundescricao from sase.avaliadoreducacional ave inner join territorios.municipio mun on ave.muncod = mun.muncod where ave.aveid = {$aveid}";
        $avenome = $db->pegaUm($sqlAve, 'avenome');
        $mundesc = $db->pegaUm($sqlAve, 'mundescricao');
        $estuf   = $db->pegaUm($sqlAve, 'estuf');
        $estado = strtoupper($this->prepEstado[trim($estuf)] . ' ' . $this->pegaEstadoAvaliador());
        $estadol = $this->prepEstado[trim($estuf)] . ' ' . ucfirst(strtolower(htmlentities($this->pegaEstadoAvaliador())));
        $dataA = date('d/m/Y');
        $apresentacao = array();
        $justificativa = array();
        $objetivogeral = array();
        $objetivoespecifico = array();

        switch ($pflcod){
            case PFLCOD_SASE_SUPERVISOR_GERAL:
                $apresentacao[] = "Este documento traz o relatório mensal das ações de supervisão geral do trabalho de assistência técnica realizada no Estado {$estadol} para elaboração ou adequação dos planos de educação ao PNE.";

                $justificativa[] = "A elaboração ou adequação dos Planos Estaduais, Distrital e Municipais em consonância ao PNE é processo determinante para a instituição do Sistema Nacional de Educação. Os governos precisam planejar e utilizar técnicas e métodos que resultem em politicas públicas para a solução dos problemas educacionais. Elaborar ou adequar os planos de educação é, portanto, uma tarefa técnica e política.";
                $justificativa[] = "Considerando que o Artigo 8° da Lei 13.005/2014 determina que todos os entes federados deverão elaborar ou adequar seu Planos Educação em consonância ao PNE no prazo de um ano a contar da aprovação da lei, o MEC, o CONSED e a UNDIME constituíram uma rede de assistência técnica visando prestar apoio aos estados e municípios no cumprimento desta determinação. O presente relatório apresenta as ações e resultados no Estado {$estadol}, que decorrem dos trabalhos de Supervisão Geral, visando garantir a qualidade do trabalho da assistência realizada pelo conjunto de AE Técnicos.";

                $objetivogeral[] = "Apresentando o relatorio mensal de Supervisao Geral do trabalho de assistencia técnica realizada no Estado {$estadol} para elaboração ou adequação dos planos de adequação ao PNE, considerando: (i) monitoramento das ações dos AE Técnicos e Supervisores, (ii) consolidaçao das informações do Estado e (iii) orientação das ações de assistência técnica dos AE Técnicos e Supervisores de acordo com as pactuações estaduais.";

                $objetivoespecifico[] = "Reunir as informações sobre a (adequação ou elaboração) do Plano Estadual de Educação.";
                $objetivoespecifico[] = "Consolidar os resultados dos Municípios no processo de adequação ou elaboração dos Planos Municipais de Educação no Estado {$estadol}.";
                $objetivoespecifico[] = "Monitorar a rede assistência técnica conforme o planejamento estadual e consolidar as ações desenvolvidas a cada quinzena.";
                $objetivoespecifico[] = "Sistematizar as ações a serem desenvolvidas com os municípios que não tiveram alteração de situação no período relatado.";
                break;

            case PFLCOD_SASE_EXECUTIVO:
                $apresentacao[] = "Este documento traz o relatório quinzenal da assistência técnica realizada no Estado {$estadol} para elaboração ou adequação dos planos de educação ao PNE.";

                $justificativa[] = "A elaboração ou adequação dos Planos Estaduais, Distrital e Municipais em consonância ao PNE é processo determinante para a instituição do Sistema Nacional de Educação. Os governos precisam planejar e utilizar técnicas e métodos que resultem em politicas públicas para a solução dos problemas educacionais. Elaborar ou adequar os planos de educação é, portanto, uma tarefa técnica e política.";
                $justificativa[] = "Considerando que o Artigo 8° da Lei 13.005/2014 determina que todos os entes federados deverão elaborar ou adequar seu Planos Educação em consonância ao PNE no prazo de um ano a contar da aprovação da lei, o MEC, o CONSED e a UNDIME constituíram uma rede de assistência técnica visando prestar apoio aos estados e municípios no cumprimento desta determinação. O presente relatório apresenta as ações e resultados no Estado {$estadol}, que decorrem dos trabalhos de coordenação, supervisão e assistência técnica direta às equipes responsáveis <label style=\"font-style: italic;\">(pelo Plano Estadual e)</label> pelos Planos Municipais de Educação";

                $objetivogeral[] = "Apresentar o relatório quinzenal da assistência técnica realizada no Estado {$estadol} para elaboração ou adequação dos planos de educação ao PNE, considerando: (i) as ações de coordenação (mobilização, formação, reuniões técnicas, atividades nos municípios do Estado, entre outras), (ii) os avanços alcançados pelos municípios, (iii) os avanços alcançados no Plano Estadual de Educação e (iv) a síntase das atividades dos Técnicos (e Supervisores).";

                $objetivoespecifico[] = "<label style=\"font-style: italic;\">Informar as ações para que o Estado {$estadol} (adeque ou elabore) seu Plano Estadual de Educação.</label>";
                $objetivoespecifico[] = "Analisar e informar os resultados consolidados dos municípios no processo de adequação ou elaboração dos Planos Municipais de Educação.";
                $objetivoespecifico[] = "Avaliar e informar, de maneira consolidada, as ações desenvolvidas pelos Avaliadores Educacionais no período.";
                $objetivoespecifico[] = "Orientar as ações a serem desenvolvidas com os municípios que não tiveram alteração de situação no período relatado.";
                break;
        }

        $apresentacao          = $this->retornaDivJustificado($apresentacao);
        $justificativa         = $this->retornaDivJustificado($justificativa);
        $objetivogeral         = $this->retornaDivJustificado($objetivogeral);
        $objetivoespecifico    = $this->retornaDivLista($objetivoespecifico);
        $atividades            = $this->getListaAtividadesDesenvolvidas(false, false, $pflcod, true, $estuf);
        $resultados            = $this->getListaInformacoesPEESupervisorGeral(false, false);
        $situacaoatual         = $this->getContadorSituacaoAtualEtapasPNE($estuf, $mes, false);
        $municipiocomalteracao = $this->getListaMunComAlteracaoPEESupervisorGeral($estuf, $mes, false);
        $municipiosemalteracao = $this->getListaMunSemAlteracaoEtapa($estuf, $mes, false);
        $municipioseminfo      = $this->getListaMunSemInfo($estuf, $mes, false);
        $acoespropostas        = $this->getAcoesSuperGeral($estuf, $mes, false, false);
        $ravinfcomplementar = trim($this->arAtributos['ravinfcomplementar']) != '' ? $this->arAtributos['ravinfcomplementar'] : '&nbsp;';


        $html = <<<HTML
        <style media="print">
            .tableFontReduct td{
                font-size: 10px;
            }
            .pagina{
                page-break-before: always;
            }
        </style>
        <table border="0" cellspacing="0" cellpadding="5" width="100%" >
            <tr>
                <td>
                    <div style="text-align: center; width: 100%;">
                        MUNISTÉRIO DA EDUCAÇÃO <br/>
                        SECRETARIA DE ARTICULAÇÃO COM OS SISTEMAS DE ENSINO <br/>
                        DIRETORIA DE COOPERAÇÃO E PLANOS DE EDUCAÇÃO <br/>
                        ESTADO {$estado}
                    </div>
                </td>
            </tr>
            <tr>
                <td><br/></td>
            </tr>
            <tr>
                <td><br/></td>
            </tr>
            <tr>
                <td><br/></td>
            </tr>
            <tr>
                <td><br/></td>
            </tr>
            <tr>
                <td><br/></td>
            </tr>
            <tr>
                <td><br/></td>
            </tr>
            <tr>
                <td><br/></td>
            </tr>
            <tr>
                <td><br/></td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: justify; width: 100%;">
                        RELATÓRIO DOS RESULTADOS DE ELABORAÇÃO OU ADEQUAÇÃO DOS PLANOS DE
                        EDUCAÇÃO AO PNE NO ESTADO {$estado}, NO PERÍODO DE {$dataIni} A {$dataFim}.
                    </div>
                </td>
            </tr>
            <tr>
                <td><br/></td>
            </tr>
            <tr>
                <td><br/></td>
            </tr>
            <tr>
                <td><br/></td>
            </tr>
            <tr>
                <td><br/></td>
            </tr>
            <tr>
                <td>
                    <div style="text-align: right; width: 100%;">
                        {$avenome}
                    </div>
                </td>
            </tr>
            <tr class="pagina">
                <td>
                    <table border="0" cellspacing="0" cellpadding="5" width="100%" >
                        <tr>
                            <td>
                                <div style="text-align: center;">
                                    <b>SUMÁRIO</b>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <ul style="list-style-type: none;">
                                    <li>Apresentação</li>
                                    <li>Justificativa</li>
                                    <li>Objetivo Geral</li>
                                    <li>Objetivos Específicos</li>
                                    <li>Atividades desenvolvidas</li>
                                    <li>Resultados consolidados alcançados</li>
                                    <li>Informações complementares sobre o período avaliado</li>
                                    <li>Anexos</li>
                                </ul>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr class="pagina">
                <td>
                    <table border="0" cellspacing="0" cellpadding="5" width="100%" >
                        <tr>
                            <td>
                                <div style="text-align: left;">
                                    <h3>APRESENTAÇÃO</h3>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div style="text-align: justify;">
                                    {$apresentacao}
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div style="text-align: left;">
                                    <h3>JUSTIFICATIVA</h3>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div style="text-align: justify;">
                                    {$justificativa}
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div style="text-align: left;">
                                    <h3>OBJETIVO GERAL</h3>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div style="text-align: justify;">
                                    {$objetivogeral}
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div style="text-align: left;">
                                    <h3>OBJETIVOS ESPECÍFICOS</h3>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div style="text-align: justify;">
                                    {$objetivoespecifico}
                                </div>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr class="pagina">
                <td>
                    <table border="0" cellspacing="0" cellpadding="5" width="100%" >
                        <tr>
                            <td style="text-align: left;">
                                <h3>ATIVIDADES DESENVOLVIDAS</h3>
                            </td>
                        </tr>
                        <tr>
                            <td style="font-weight: bold;">
                                <div>Quadro 1. Atividades executadas no período.</div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div>
                                    {$atividades}
                                </div>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr class="pagina">
                <td>
                    <table border="0" cellspacing="0" cellpadding="5" width="100%" >
                        <tr>
                            <td>
                                <div style="text-align: left;">
                                    <h3>RESULTADOS CONSOLIDADOS ALCANÇADOS</h3>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div>A partir do trabalho de assistência técnica realizado e dos levantamentos feitos via telefone ou e-mail, foi produzida a sistematização de resultados no período, expressa nos Quadros 2 a 7.</div>
                            </td>
                        </tr>
                        <tr>
                            <td><br/></td>
                        </tr>
                        <tr>
                            <td style="font-weight: bold;">
                                <div>Quadro 2. Informações a respeito do Plano Estadual de Educação.</div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div>
                                    {$resultados}
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td><br/><br/></td>
                        </tr>
                        <tr>
                            <td style="font-weight: bold; text-align: justify; letter-spacing: 0px; white-space: normal;">
                                <div>Quadro 3. Situação atual com relação às etapas de trabalho propostas para a elaboração ou adequação dos Planos Municipais de Educação ao PNE.</div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div>
                                    {$situacaoatual}
                                </div>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr class="pagina">
                <td>
                    <table border="0" cellspacing="0" cellpadding="5" width="100%" >
                        <tr>
                            <td style="font-weight: bold;">
                                <div>Quadro 4. Municípios do Estado que apresentaram alteração na sua etapa de trabalho no período.</div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div>
                                    {$municipiocomalteracao}
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td><br/></td>
                        </tr>
                        <tr>
                            <td style="font-weight: bold;">
                                <div>Quadro 5. Municípios que permaneceram na mesma etapa do período anterior.</div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div>
                                    {$municipiosemalteracao}
                                </div>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
            <tr class="pagina">
                <td>
                    <table border="0" cellspacing="0" cellpadding="5" width="100%" >
                        <tr>
                            <td style="font-weight: bold;">
                                <div>Quadro 6. Municípios ainda sem informação ou sem trabalho iniciado.</div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div>
                                    {$municipioseminfo}
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td><br/></td>
                        </tr>
                        <tr>
                            <td style="font-weight: bold;">
                                <div>Quadro 7. Ações propostas pelos AE Supervisores para os municípios sem informação, sem Comissão instituída e sem alteração da etapa de trabalho.</div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div>
                                    {$acoespropostas}
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td><br/></td>
                        </tr>
                        <tr>
                            <td style="font-weight: bold;">
                                <div>Informações complementares.</div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div style="text-align: justify; border: 1px solid #000; padding: 5px; min-height: 60px; height: 150px;">
                                    {$ravinfcomplementar}
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <br/>
                                <div style="width: 100%; text-align: left;">
                                    {$mundesc}, {$dataA}
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td><br/></td>
                        </tr>
                        <tr>
                            <td>
                                <div style="width: 100%; text-align: left;">
                                    {$avenome}
                                </div>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>
HTML;
        return $html;
    }

    public function retornaPerfil(){
        $pfls = arrayPerfil();
        $pflcod = '';

        if (is_array($pfls)){
            if (in_array(PFLCOD_SASE_TECNICO, $pfls)){
                $pflcod = PFLCOD_SASE_TECNICO;
            } if (in_array(PFLCOD_SASE_SUPERVISOR, $pfls)){
                $pflcod = PFLCOD_SASE_SUPERVISOR;
            } if (in_array(PFLCOD_SASE_ADMINISTRADOR, $pfls)){
                $pflcod = PFLCOD_SASE_ADMINISTRADOR;
            } if (in_array(PFLCOD_SASE_EXECUTIVO, $pfls)){
                $pflcod = PFLCOD_SASE_EXECUTIVO;
            } if (in_array(PFLCOD_SASE_SUPER_USUARIO, $pfls)) {
                $pflcod = PFLCOD_SASE_SUPER_USUARIO;
            }
        }
        return $pflcod;
    }

    public function recuperarListagem($tipo = 'rel', $diretoria = 1)
    {
        global $db, $aveid;

        $pflcod = $this->retornaPerfil();
        $usucpf = $this->arAtributos['usucpf'];
        $per = $this->retornaPermissao();

        if ($_REQUEST['aveid'] != '' && $per > 1){
            $aveid = $_REQUEST['aveid'];
        } else {
            $sql = "select aveid from sase.avaliadoreducacional where avenumcpf = '".$usucpf."' and avediretoria = {$diretoria}";
            $aveid = $db->pegaUm($sql);
        }
        if ($per == 1 and $aveid == '' and $pflcod != PFLCOD_SASE_ADMINISTRADOR){
            return '';
        }

        $ratdata1periodo = $_REQUEST['ratdata1periodo'];
        $ratdata2periodo = $_REQUEST['ratdata2periodo'];

        $where = array();

        $where[] = "1=1";

        if ($_REQUEST['pflcod'] != ''){
//            $where[] = "exists (
//                            select
//                                1
//                            from seguranca.perfilusuario pu
//                            inner join seguranca.perfil p on pu.pflcod = p.pflcod and p.sisid = ".SASE_SISID."
//                            where pu.usucpf = usu.usucpf
//                            and p.pflcod = {$_REQUEST['pflcod']}
//                        )";
            $where[] = "rav.pflcod = {$_REQUEST['pflcod']}";
        }

        if ($_REQUEST['aveid'] != ''){
            $where[] = "ave.aveid = {$_REQUEST['aveid']}";
        }

        if ($_REQUEST['esdid'] != ''){
            $where[] = "esd.esdid = {$_REQUEST['esdid']}";
        }

        if ($_REQUEST['estuf'] != ''){
            $where[] = "ave.estuf = '{$_REQUEST['estuf']}'";
        }

        if ($ratdata1periodo != '' && $ratdata1periodo != '//'){
            $where[] = "to_char(rav.ratdata1periodo, 'dd/mm/yyyy') = '{$ratdata1periodo}'";
        }

        if ($ratdata2periodo != '' && $ratdata2periodo != '//'){
            $where[] = "to_char(rav.ratdata2periodo, 'dd/mm/yyyy') = '{$ratdata2periodo}'";
        }

        // Define o filtro para os usuarios dependendo do usuário logado
        switch ($pflcod){
            case PFLCOD_SASE_SUPERVISOR:
                $sql = "select distinct cpftecnico from sase.usuarioresponsabilidade where rpustatus = 'A' and cpftecnico is not null and usucpf = '{$usucpf}'";
                $dados = $db->carregar($sql);

                $usu = array();
                if (is_array($dados)) {
                    foreach ($dados as $d) {
                        $usu[] = $d['cpftecnico'];
                    }

                    $where[] = "usu.usucpf in ('" . implode('\', \'', $usu) . "', '{$usucpf}')";
                }
                break;

            case PFLCOD_SASE_SUPERVISOR_GERAL:
            case PFLCOD_SASE_TECNICO:
                $where[] = "usu.usucpf = '{$usucpf}'";
                break;

            case PFLCOD_SASE_EXECUTIVO:
                $sql = "select regcod from seguranca.usuario where usucpf = '{$usucpf}'";
                $estuf = $db->pegaUm($sql);
                $where[] = "usu.regcod = '{$estuf}'";
                break;
        }


        /*if ($pflcod != ''){
            $where[] = "rav.pflcod = {$pflcod}";
        }*/

        if (count($where) > 0){
            $a = "and ";
        }

        if ($tipo == 'rel'){
            $col = 'rav.ravid,';
        }

        $where[] = "ave.avediretoria = {$diretoria}";

        $sql = "select
                    {$col}
                    usu.usunome as usunome,
                    usu.usucpf as usucpf,
                    rav.pflcod as pflcod,
                    esddsc,
                    to_char(rav.ratdata1periodo, 'dd/mm/yyyy') as ratdata1periodo,
                    to_char(rav.ratdata2periodo, 'dd/mm/yyyy') as ratdata2periodo,
                    case
                        when ratperiodo = 1 then '1° Quinzena'
                        when ratperiodo = 2 then '2° Quinzena'
                        when ratperiodo = 3 then 'Mensal'
                    end ratperiodo
                from sase.relatorioavaliadorredeae rav
                inner join sase.avaliadoreducacional ave on rav.aveid = ave.aveid
                inner join seguranca.usuario usu on usu.usucpf = ave.avenumcpf
                left join workflow.documento doc on rav.docid = doc.docid
                left join workflow.estadodocumento esd on esd.esdid = doc.esdid
                where 1=1 ".$a.implode(' and ', $where);

        $dados = $db->carregar($sql);

        if (!is_array($dados)) { $dados = array(); }
        if ($tipo == 'rel') {
            $list = new Simec_Listagem();
            $list->setFormFiltros('form-save');
            $list->setTamanhoPagina(50);
            $list->setCabecalho(array(
                'Avaliador',
                'Situação',
                'Data Início',
                'Data Final',
                'Período'
            ));
            $list->esconderColunas(array(
                'ravid',
                'usucpf',
                'pflcod'
            ));
            $list->addAcao('edit', array(
                'func' => 'editRelatorio',
                'titulo' => 'Editar relatório',
                'extra-params' => array(
                    'ravid',
                    'pflcod'
                )
            ));
            if ($pflcod == PFLCOD_SASE_SUPER_USUARIO ||
                $pflcod == PFLCOD_SASE_ADMINISTRADOR
            ) {
                $list->addAcao('delete', array(
                    'func' => 'apagarRelatorio',
                    'titulo' => 'Apagar relatório',
                    'extra-params' => array(
                        'ravid'
                    )
                ));
            }
            $list->addAcao('download', array(
                'func' => 'downloadRelatorio',
                'titulo' => 'Download do relatório',
                'extra-params' => array(
                    'ravid'
                )
            ));
            $list->setDados($dados);
            $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $list->render();
        } else {
            $cabecalho = array(
                'Avaliador',
                'Situação',
                'Data Início',
                'Data Final',
                'Período'
            );
            $alinhamento = array(
                'left',
                'left',
                'left',
                'left',
                'left'
            );
            $larguras = array(
                '80%',
                '5%',
                '5%',
                '5%',
                '5%'
            );

            ob_clean();
            header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
            header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
            header("Pragma: no-cache");
            header("Content-type: application/xls; name=simec_lista_relatorio_" . date("Ymdhis") . ".xls");
            header("Content-Disposition: attachment; filename=simec_lista_relatorio_" . date("Ymdhis") . ".xls");
            header("Content-Description: MID Gera excel");

            $db->monta_lista($sql,$cabecalho,100000,5,'N','','N','formCadastroLista',$larguras,$alinhamento);
        }

    }

    /**
     * Verifica as permissões do usuário e o estado do relatório no workflow.
     * @return bool
     *      true  - Para quando o usuário estiver visualizando seu próprio relatório ou ser super usuário
     *      false - Para todo o resto
     */
    public function retornaDisCampos($usucpfS){
        global $db;
        $ret = false;
        //$usucpfS = $_SESSION['usucpf'];
        $pfls = arrayPerfil();
        $usucpf = $this->arAtributos['usucpf'];

        if ($this->arAtributos['ravid']){
            if ($this->arAtributos['docid'] != '') {
                $sql = "select esdid from workflow.documento where docid = " . $this->arAtributos['docid'];
                $esdid = $db->pegaUm($sql);
            } else {
                //$docid = wf_cadastrarDocumento( TPDID_SASE_AVALIADOREDUCACIONAL, 'Documento Avaliador Educacional' );
                $docid = cadastraDocumentoAE($this->arAtributos['pflcod'], 'Documento Avaliador Educacional');
                $this->docid = $docid;
                // ver($Assessoramento,d);
                $this->alterar();
                $this->commit();

                $sql = "select esdid from workflow.documento where docid = " . $docid;
                $esdid = $db->pegaUm($sql);
            }
        }
//$this->msg = $esdid;
        // Não executa nenhuma validação quando o usuário é um Super Usuário, pois o mesmo não possui nenhuma restrição de acesso
        if (!in_array(PFLCOD_SASE_SUPER_USUARIO, $pfls)){

            switch ($esdid){
                case ESDID_SASE_EM_CADASTRAMENTO_PELO_TECNICO:
                case ESDID_SASE_EM_CORRECOES_PELO_TECNICO:
                    if (in_array(PFLCOD_SASE_TECNICO, $pfls)){
                        $this->msg = $usucpfS;
                        $sql = "select aveid from sase.avaliadoreducacional where avenumcpf = '".$usucpfS."'";
                        $aveid = $db->pegaUm($sql);
                        if ($aveid == $this->aveid){
                            $ret = true;
                        }
                    }
                    break;

                case ESDID_SASE_EM_CADASTRAMENTO_PELO_SUPERVISOR:
                case ESDID_SASE_EM_CORRECOES_PELO_SUPERVISOR:
                    if (in_array(PFLCOD_SASE_SUPERVISOR, $pfls)){
                        $sql = "select aveid from sase.avaliadoreducacional where avenumcpf = '".$usucpfS."'";
                        $aveid = $db->pegaUm($sql);
                        if ($aveid == $this->aveid){
                            $ret = true;
                        }
                    }
                    break;

                case ESDID_SASE_EM_CADASTRAMENTO_PELO_SUPERVISOR_GERAL:
                case ESDID_SASE_EM_CORRECOES_PELO_SUPERVISOR_GERAL:
                    if (in_array(PFLCOD_SASE_SUPERVISOR_GERAL, $pfls)){
                        $sql = "select aveid from sase.avaliadoreducacional where avenumcpf = '".$usucpfS."'";
                        $aveid = $db->pegaUm($sql);
                        if ($aveid == $this->aveid){
                            $ret = true;
                        }
                    }
                    break;

                case ESDID_SASE_EM_ANALISE_SUPERVISOR:
                case ESDID_SASE_EM_RE_ANALISE_SUPERVISOR:
                    if (in_array(PFLCOD_SASE_SUPERVISOR, $pfls)){
                        $sql = "select distinct cpftecnico from sase.usuarioresponsabilidade where usucpf = '{$usucpfS}'";
                        $dados = $db->carregar($sql);
                        $aveid = array();
                        foreach ($dados as $d) {
                            $aveid[] = $d['cpftecnico'];
                        }
                        if (in_array($usucpf, $aveid)){
                            $ret = true;
                        }
                    }
                    break;

                case ESDID_SASE_EM_CADASTRAMENTO_PELO_COORDENADOR_ESTADUAL:
                case ESDID_SASE_EM_CORRECOES_PELO_COORDENADOR_ESTADUAL:
                    if (in_array(PFLCOD_SASE_EXECUTIVO, $pfls)){
                        $sql = "select aveid from sase.avaliadoreducacional where avenumcpf = '".$usucpfS."'";
                        $aveid = $db->pegaUm($sql);
                        if ($aveid == $this->aveid){
                            $ret = true;
                        }
                    }
                    break;
            }
        } else {
            $ret = true;
        }

        return $ret;
    }

    public function pegaAvaliador(){
        global $db;
        $usucpf = $this->arAtributos['usucpf'];
        $sql = "select usunome from seguranca.usuario where usucpf = '{$usucpf}'";
        return $db->pegaUm($sql);
    }

    public function pegaEstadoAvaliador($tipo = 'estdescricao')
    {
        global $db;
        $aveid = $this->arAtributos['aveid'];
        if ($aveid == '') {
            $usucpf = $this->arAtributos['usucpf'];
            $sql = "select UPPER(est.estdescricao) estdescricao, est.estuf from seguranca.usuario usu inner join territorios.estado est on est.estuf = usu.regcod where usucpf = '{$usucpf}'";
            return $db->pegaUm($sql, $tipo);
        } else {
            $sql = "select
                    UPPER(est.estdescricao) as estdescricao,
                    est.estuf
                from sase.avaliadoreducacional ave
                inner join territorios.estado est on est.estuf = ave.estuf
                where ave.aveid = {$aveid}";

            $res = $db->carregar($sql);
            return $res[0][$tipo];
        }
    }

    public function retornaTecnicosSupervisionados(){
        global $db;
        $ravid = $this->arAtributos['ravid'];
        $sql = "select
                    tsp.cpftecnico as usucpf
                from sase.tecnicosupervisionadosredeae tsp
                where ravid = {$ravid}
                and tsp.tspid = (select max(tspid) from sase.tecnicosupervisionadosredeae where cpftecnico = tsp.cpftecnico and muncod = tsp.muncod and ravid = {$ravid} )
                group by tsp.cpftecnico";

        return $db->carregarColuna($sql);
    }

    public function getResultadosConsolidadosPlanoCarreira($campos = false, $layout = false){
        global $db;
        $ravid = $this->arAtributos['ravid'];
        $ratdata1periodo = $this->arAtributos['ratdata1periodo'];
        $ratdata2periodo = $this->arAtributos['ratdata2periodo'];

        if(!$layout) {
            $abreTable = <<<HTML
                <table border="1" cellspacing="0" cellpadding="5" width="695" class="col-md-12 table-condensed table-hover table-responsive" >
HTML;
        } else {
            $abreTable = <<<HTML
                <table class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }

        $sql = <<<DML
                select
                    mun.muncod,
                    mun.mundescricao,
                    spc.spccomplano,
                    spc.spcpagapiso,
                    spc.spccumprejornada,
                    stm.stmobservacoes,
                    mar.marid,
                    spc.esdid,
                    stm.stmid
                from sase.municipiosassistidosredeae mar
                inner join sase.planocarreiraprofessor pcp on mar.pcpid = pcp.pcpid
                inner join territorios.municipio mun on pcp.muncod = mun.muncod
                left join sase.sitatualmunicipiotecredeae stm on stm.marid = mar.marid
                left join workflow.documento doc on doc.docid = pcp.docid
                left join sase.sitplancarprofessor spc on spc.esdid = doc.esdid
                where ravid = {$ravid}
                order by mun.mundescricao
DML;
        $res = $db->carregar($sql);
        $linhas = "";
        if(is_array($res)) {
            foreach ($res as $r) {
                $sCP = $r['spccomplano'] == 't' ? "X" : "";
                $nCP = $r['spccomplano'] == 'f' ? "X" : "";
                $sPP = $r['spcpagapiso'] == 't' ? "X" : "";
                $nPP = $r['spcpagapiso'] == 'f' ? "X" : "";
                $sCJ = $r['spccumprejornada'] == 't' ? "X" : "";
                $nCJ = $r['spccumprejornada'] == 'f' ? "X" : "";
                $obs = "";
                $disable = $r['esdid'] == "" ? "disabled title=\"Situação, do plano de carreira, não informada, no período do relatório\"" : "";
                if ($campos) {
                    $obs = <<<HTML
                        <textarea class="form-control" maxlength="200" {$disable} id="stmobservacoes" name="stmobservacoes[]"  rows="2" cols="5" >{$r['stmobservacoes']}</textarea>
HTML;

                } else {
                    $obs = $r['stmobservacoes'];
                }
                $linhas .= <<<HTML
                    <tr>
                        <td>{$r['mundescricao']}</td>
                        <td>{$sCP}</td>
                        <td>{$nCP}</td>
                        <td>{$sPP}</td>
                        <td>{$nPP}</td>
                        <td>{$sCJ}</td>
                        <td>{$nCJ}</td>
                        <td>
                            <input type="hidden" name="muncodq2[]" value="{$r['muncod']}"/>
                            <input type="hidden" name="esdidq2[]" value="{$r['esdid']}"/>
                            <input type="hidden" name="maridq2[]" value="{$r['marid']}"/>
                            <input type="hidden" name="stmidq2[]" value="{$r['stmid']}"/>
                            {$obs}
                        </td>
                    </tr>
HTML;
            }
        }


        $html = <<<HTML
            <div id="listagem" style="background: #ffffff !important;">
                {$abreTable}
                    <thead>
                        <tr>
                            <th>Municípios</th>
                            <th colspan="2">Com Plano de Carreira</th>
                            <th colspan="2">Paga o Piso (Lei 11.738/2008)</th>
                            <th colspan="2">Cumpre Jornada (Lei 11.738/2008)</th>
                            <th rowspan="2">Observações Quanto à meta 18 (PNE)</th>
                        </tr>
                        <tr>
                            <th>Situaçao</th>
                            <th>Sim</th>
                            <th>Não</th>
                            <th>Sim</th>
                            <th>Não</th>
                            <th>Sim</th>
                            <th>Não</th>
                        </tr>
                    </thead>
                    <tbody>
                        {$linhas}
                    </tbody>
                </table>
            </div>
HTML;
        return $html;
    }

    public function getListaAlteracaoExistPlano($layout = false){
        global $db;
        $ravid = $this->arAtributos['ravid'];
        $ratdata1periodo = $this->arAtributos['ratdata1periodo'];
        $ratdata2periodo = $this->arAtributos['ratdata2periodo'];

        $sql = <<<DML
                select
                    aed.aedid,
                    pcp.muncod,
                    mun.mundescricao,
                    esdO.esddsc as origem,
                    esdO.esdid as esdorigem,
                    esdD.esddsc as destino,
                    esdD.esdid as esddestino,
                    mat.matid
                from sase.planocarreiraprofessor pcp
                inner join territorios.municipio mun on pcp.muncod = mun.muncod
                inner join sase.municipiosassistidosredeae mar on mar.pcpid = pcp.pcpid
                inner join sase.relatorioavaliadorredeae rav on rav.ravid = mar.ravid
                inner join sase.usuarioresponsabilidade rpu on rpu.usucpf = rav.usucpf and rpu.rpustatus = 'A'
                left join workflow.historicodocumento hst on hst.docid = pcp.docid
                left join workflow.acaoestadodoc aed on aed. aedid = hst.aedid
                left join workflow.estadodocumento esdO on esdO.esdid = aed.esdidorigem
                left join workflow.estadodocumento esdD on esdD.esdid = aed.esdiddestino
                left join sase.municipiosalteracaoperiodotecredeae mat on mat.marid = mar.marid and mat.esdid_ant = esdO.esdid and mat.esdid_atual = esdD.esdid
                where rav.ravid = {$ravid}
                and (date(htddata) >= date('{$ratdata1periodo}') and date(htddata) <= date('{$ratdata2periodo}'))
                order by htddata
DML;
        $res = $db->carregar($sql);

        if(!$layout) {
            $abreTable = <<<HTML
                <table border="1" cellspacing="0" cellpadding="5" width="695" class="col-md-12 table-condensed table-hover table-responsive" >
HTML;
        } else {
            $abreTable = <<<HTML
                <table class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }

        $linhas = "";
        if(is_array($res)) {
            foreach ($res as $r) {
                $linhas .= <<<HTML
                <tr>
                    <td>{$r['mundescricao']}</td>
                    <td>{$r['origem']}</td>
                    <td>{$r['destino']}</td>
                </tr>
HTML;
            }
        } else {
            $linhas .= <<<HTML
                <tr>
                    <td colspan="3">Sem Registro</td>
                </tr>
HTML;
        }


        $html = <<<HTML
            <div id="listagem" style="background: #ffffff !important;">
                {$abreTable}
                    <thead>
                        <tr>
                            <th style="width: 33.33%">Municípios</th>
                            <th style="width: 33.33%">Situaçao anterior</th>
                            <th style="width: 33.33%">Situação atual</th>
                        </tr>
                    </thead>
                    <tbody>
                        {$linhas}
                    </tbody>
                </table>
            </div>
HTML;
        return $html;

    }

    public function getComprometimento($campos = false, $layout = false){
        global $db;
        $ravid = $this->arAtributos['ravid'];
        $ratdata1periodo = $this->arAtributos['ratdata1periodo'];
        $ratdata2periodo = $this->arAtributos['ratdata2periodo'];

        $sql = <<<DML
            select
                mun.mundescricao,
                mar.marid,
                doc.esdid,
                crp.crpid,
                crp.crpcompate60,
                crp.crpcompate6180,
                crp.crpcompate8190,
                crp.crpcompate91
            from sase.municipiosassistidosredeae mar
            inner join sase.planocarreiraprofessor pcp on mar.pcpid = pcp.pcpid
            inner join territorios.municipio mun on pcp.muncod = mun.muncod
            left join workflow.documento doc on doc.docid = pcp.docid
            left join sase.comprometimentoreceitapessoal crp on crp.marid = mar.marid
            where mar.ravid = {$ravid}
DML;
        $res = $db->carregar($sql);

        if(!$layout) {
            $abreTable = <<<HTML
                <table border="1" cellspacing="0" cellpadding="5" width="695" class="col-md-12 table-condensed table-hover table-responsive" >
HTML;
        } else {
            $abreTable = <<<HTML
                <table class="table table-bordered table-hover table-condensed" border="1">
HTML;
        }

        $linhas = "";
        if(is_array($res)){
            foreach ($res as $r) {

                $disable = $r['esdid'] == '' ? "disabled title=\"Situação, do plano de carreira, não informada, no período do relatório\"" : "";

                $crp60 = !$campos ? $r['crpcompate60'] : "<textarea class=\"form-control\" {$disable} maxlength=\"200\" id=\"crpcompate60\" name=\"crpcompate60[]\"  rows=\"2\" cols=\"5\" >{$r['crpcompate60']}</textarea>";
                $crp6180 = !$campos ? $r['crpcompate6180'] : "<textarea class=\"form-control\" {$disable} maxlength=\"200\" id=\"crpcompate6180\" name=\"crpcompate6180[]\"  rows=\"2\" cols=\"5\" >{$r['crpcompate6180']}</textarea>";
                $crp8190 = !$campos ? $r['crpcompate8190'] : "<textarea class=\"form-control\" {$disable} maxlength=\"200\" id=\"crpcompate8190\" name=\"crpcompate8190[]\"  rows=\"2\" cols=\"5\" >{$r['crpcompate8190']}</textarea>";
                $crp91 = !$campos ? $r['crpcompate91'] : "<textarea class=\"form-control\" {$disable} maxlength=\"200\" id=\"crpcompate91\" name=\"crpcompate91[]\"  rows=\"2\" cols=\"5\" >{$r['crpcompate91']}</textarea>";

                $linhas .= <<<HTML
                    <tr>
                        <td>
                            <input type="hidden" id="esdidq5" name="esdidq5[]" value="{$r['esdid']}"/>
                            <input type="hidden" id="crpidq5" name="crpidq5[]" value="{$r['crpid']}"/>
                            <input type="hidden" id="maridq5" name="maridq5[]" value="{$r['marid']}"/>
                            {$r['mundescricao']}
                        </td>
                        <td>{$crp60}</td>
                        <td>{$crp6180}</td>
                        <td>{$crp8190}</td>
                        <td>{$crp91}</td>
                    </tr>
HTML;
            }
        } else {
            $linhas .= <<<HTML
                <tr>
                    <td colspan="5">Sem Registro</td>
                </tr>
HTML;
        }

        $html = <<<HTML
            <div id="listagem" style="background: #ffffff !important;">
                {$abreTable}
                    <thead>
                        <tr>
                            <th style="width: 20%">Municípios</th>
                            <th style="width: 20%">Compromete até 60%</th>
                            <th style="width: 20%">Comrpomete de 61% à 80%</th>
                            <th style="width: 20%">Compromete de 81% à 90%</th>
                            <th style="width: 20%">Compromete acima de 91%</th>
                        </tr>
                    </thead>
                    <tbody>
                        {$linhas}
                    </tbody>
                </table>
            </div>
HTML;
        return $html;

    }

    public function salvaComprometimento(){
        $arrMarids          = $_POST['maridq5'];
        $arrEsdids          = $_POST['esdidq5'];
        $arrCrpids          = $_POST['crpidq5'];
        $arrCrpcompate60    = $_POST['crpcompate60'];
        $arrCrpcompate6180  = $_POST['crpcompate6180'];
        $arrCrpcompate8190  = $_POST['crpcompate8190'];
        $arrCrpcompate91    = $_POST['crpcompate91'];
        $sql = "";

        if($arrMarids){
            $i = 0;
            foreach ($arrMarids as $key => $a) {
                $marid          = $a;
                $esdid          = isset($arrEsdids[$key]) ? $arrEsdids[$key] : null;
                $crpid          = isset($arrCrpids[$key]) ? $arrCrpids[$key] : null;
                $crpcompate60   = isset($arrCrpcompate60[$key]) ? $arrCrpcompate60[$key] : null;
                $crpcompate6180 = isset($arrCrpcompate6180[$key]) ? $arrCrpcompate6180[$key] : null;
                $crpcompate8190 = isset($arrCrpcompate8190[$key]) ? $arrCrpcompate8190[$key] : null;
                $crpcompate91   = isset($arrCrpcompate91[$key]) ? $arrCrpcompate91[$key] : null;

                if($esdid != null) {
                    if ($crpid == null) {
                        // Inserção
                        $sql .= <<<DML
                        insert into sase.comprometimentoreceitapessoal (marid, esdid, crpcompate60, crpcompate6180, crpcompate8190, crpcompate91) values (
                            {$marid},
                            {$esdid},
                            '{$crpcompate60}',
                            '{$crpcompate6180}',
                            '{$crpcompate8190}',
                            '{$crpcompate91}'
                        );
DML;
                    } else {
                        // Update
                        $sql .= <<<DML
                        update sase.comprometimentoreceitapessoal set
                            crpcompate60 = '{$crpcompate60}',
                            crpcompate6180 = '{$crpcompate6180}',
                            crpcompate8190 = '{$crpcompate8190}',
                            crpcompate91 = '{$crpcompate91}'
                        where crpid = {$crpid}
DML;
                    }
                }
            }

            if($sql){
                $this->executar($sql);

                if (!$this->commit()) {
                    return false;
                }
            }
            return true;
        }
    }

    public function getEstruturaPlanoCarreiraRemuneracao($campos = false, $layout = false){
        $epr = new EstruturaCarreiraRemuneracao(null, $this->ravid);
        return $epr->getTabelaRelatorio($campos, $layout);
    }

    public function salvaEstruturaPlanoCarreiraRemuneracao(){
        $arrMarid                  = isset($_POST['maridq6']) ? $_POST['maridq6'] : null;
        $arrEsdid                  = isset($_POST['esdidq6']) ? $_POST['esdidq6'] : null;
        $arrEcrid                  = isset($_POST['ecridq6']) ? $_POST['ecridq6'] : null;
        $arrEcrpercentdispersao    = isset($_POST['ecrpercentdispersao']) ? $_POST['ecrpercentdispersao'] : null;
        $arrEcrrelprofaluno        = isset($_POST['ecrrelprofaluno']) ? $_POST['ecrrelprofaluno'] : null;
        $arrEcrneceshorasdocente   = isset($_POST['ecrneceshorasdocente']) ? $_POST['ecrneceshorasdocente'] : null;
        $arrEcrhorasdocentecontrat = isset($_POST['ecrhorasdocentecontrat']) ? $_POST['ecrhorasdocentecontrat'] : null;

        foreach ($arrMarid as $k => $m) {
            $reg = new EstruturaCarreiraRemuneracao();
            $reg->marid = $m;
            $reg->esdid = $arrEsdid[$k];
            $reg->ecrpercentdispersao = $arrEcrpercentdispersao[$k];
            $reg->ecrrelprofaluno = $arrEcrrelprofaluno[$k];
            $reg->ecrneceshorasdocente = $arrEcrneceshorasdocente[$k];
            $reg->ecrhorasdocentecontrat = $arrEcrhorasdocentecontrat[$k];
            if(empty($arrEcrid[$k])){
                $reg->inserir();
            } else {
                $reg->ecrid = $arrEcrid[$k];
                $reg->alterar();
            }
            if(!$reg->commit()){
                $this->msg = "Erro ao salvar os dados.";
                return false;
            }
        }
        return true;
    }

    public function getMunicipiosAlteracaoPerioroTecRedeae(){
        $mat = new MunicipiosAlteracaoPeriodoTecRedeae(null, $this->ravid, $this->arAtributos['ratdata1periodo'], $this->arAtributos['ratdata2periodo']);
        return $mat->getTabelaRelatorio();
    }

    public function salvaMunicipiosAlteracaoPeriodoTecRedeae(){
        $arrMarid            = isset($_POST['maridq4']) ? $_POST['maridq4'] : null;
        $arrMatid            = isset($_POST['matidq4']) ? $_POST['matidq4'] : null;
        $arrOrigem           = isset($_POST['esdorigemq4']) ? $_POST['esdorigemq4'] : null;
        $arrDestino          = isset($_POST['esddestinoq4']) ? $_POST['esddestinoq4'] : null;

        foreach ($arrMarid as $k => $m) {
            $reg = new MunicipiosAlteracaoPeriodoTecRedeae();
            $reg->marid = $m;
            $reg->esdid_ant = $arrOrigem[$k];
            $reg->esdid_atual = $arrDestino[$k];
            if(empty($arrMatid[$k])){
                $reg->inserir();
            } else {
                $reg->matid = $arrMatid[$k];
                $reg->alterar();
            }
            if(!$reg->commit()){
                $this->msg = "Erro ao salvar os dados de municípios assistidos com alteração na existência de plano de carreira.";
                return false;
            }
        }
        return true;
    }
}