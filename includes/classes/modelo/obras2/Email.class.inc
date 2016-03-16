<?php

class Email extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.email";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("emlid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'emlid' => null,
        'emlremetente' => null,
        'emltipo' => null,
        'emldata' => null,
        'emldataatualizacao' => null,
        'usucpf' => null,
        'emlconteudo' => null,
        'emlassunto' => null,
        'emlstatus' => null,
        'temid' => null,
        'obrid' => null,
        'emlregistroatividade' => null,
        'rgaid' => null
    );
    protected $destinatarios = array();

    public function __construct($id = null) {
        parent::__construct($id);
    }

    public function obraInacabada($obrid = null)
    {
        $obrid = ($obrid) ? $obrid : $this->obrid;
        if(!$obrid) return false;

        $sql = "SELECT
                    COUNT(*)
                FROM obras2.obras o
                LEFT JOIN workflow.documento d                       ON d.docid = o.docid
                WHERE o.obrid = $obrid AND d.esdid = 1084";
        return ($this->pegaUm($sql) > 0) ? true : false;
    }

    /**
     * Salva o email em banco
     * 
     * @param array $destinatarios - Array com os usucpf de cada destinatário
     * @param type $anexos - Array com os Arqid dos arquivos de anexo
     * @param type $boAntesSalvar
     * @param type $boDepoisSalvar
     * @param type $arCamposNulo
     */
    public function salvar(array $destinatarios, $anexos = array(), $boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array()) {

        if($this->obraInacabada()){
            return false;
        }

        $emlid = parent::salvar($boAntesSalvar, $boDepoisSalvar, $arCamposNulo);
        $this->commit();


        foreach ($destinatarios as $email) {
            $destinatarioEmail = new DestinatarioEmail();
            $dados = array(
                'emlid' => $emlid,
                'email' => "'" . $email . "'"
            );
            $destinatarioEmail->popularDadosObjeto($dados);
            $destinatarioEmail->salvar();
            $destinatarioEmail->commit();
        }

        foreach ($anexos as $arqid) {
            $anexoEmail = new AnexoEmail();
            $dados = array(
                'emlid' => $emlid,
                'arqid' => $arqid
            );
            $anexoEmail->popularDadosObjeto($dados);
            $anexoEmail->salvar();
            $anexoEmail->commit();
        }
    }

    /**
     * Salva o email em banco
     *
     * @param array $destinatarios - Array com os usucpf de cada destinatário
     * @param type $anexos - Array com os Arqid dos arquivos de anexo
     * @param type $boAntesSalvar
     * @param type $boDepoisSalvar
     * @param type $arCamposNulo
     */
    public function salvarInacabada(array $destinatarios, $anexos = array(), $boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array()) {
        $emlid = parent::salvar($boAntesSalvar, $boDepoisSalvar, $arCamposNulo);
        $this->commit();

        foreach ($destinatarios as $email) {
            $destinatarioEmail = new DestinatarioEmail();
            $dados = array(
                'emlid' => $emlid,
                'email' => "'" . $email . "'"
            );
            $destinatarioEmail->popularDadosObjeto($dados);
            $destinatarioEmail->salvar();
            $destinatarioEmail->commit();
        }

        foreach ($anexos as $arqid) {
            $anexoEmail = new AnexoEmail();
            $dados = array(
                'emlid' => $emlid,
                'arqid' => $arqid
            );
            $anexoEmail->popularDadosObjeto($dados);
            $anexoEmail->salvar();
            $anexoEmail->commit();
        }
    }

    /**
     * Monta um pdf com conteúdo do email e salva nos arquivos
     * 
     * @return int $arqid
     */
    public function montaPdfConteudo() {
        include_once APPRAIZ . "includes/dompdf/dompdf_config.inc.php";
        $this->emlconteudo;

        $dompdf = new DOMPDF();
        $dompdf->load_html($this->emlconteudo);
        $dompdf->render();

        $pdfoutput = $dompdf->output();

        $file = new FilesSimec(null, null, "obras2");
        $file->setPasta('obras2');
        $arqid = $file->setStream('conteudo_email', $pdfoutput, 'application/pdf', '.pdf', false, 'conteudo_email.pdf');

        include_once APPRAIZ . "includes/classes/modelo/obras2/AnexoEmail.class.inc";
        $anexoEmail = new AnexoEmail();
        $dados = array(
            'emlid' => $this->emlid,
            'arqid' => $arqid
        );
        $anexoEmail->popularDadosObjeto($dados);
        $anexoEmail->salvar();
        $anexoEmail->commit();

        return $arqid;
    }

    /**
     * Envia o e-mail para todos os destinatários, com todos os anexos,
     * Um dos anexos é o conteúdo do e-mail em pdf
     * O status do email é alterado para "E" caso tenha sido enviado com sucesso
     */
    public function enviar() {
        if($this->obraInacabada()){
            return false;
        }

        $destinatarios = $this->getDestinatarios();
        $anexos = $this->getAnexos();

        if (count($anexos)) {
            foreach ($anexos as $k => $value) {
                $anexos[$k]['nome'] = $value['arqnome'] . '.' . $value['arqextensao'];
            }
        }

        if ($this->emlregistroatividade) {
            $this->rgaid = $this->pegaUm("select nextval('obras2.registroatividade_rgaid_seq')");
            $this->emlconteudo = str_replace('__RGAID__', $this->rgaid, $this->emlconteudo);
        }

        $arqidConteudo = $this->montaPdfConteudo();
        $file = new FilesSimec(null, null, 'obras2');

        $anexos[] = array(
            'arquivo' => $file->getCaminhoFisicoArquivo($arqidConteudo),
            'nome' => 'Anexo.pdf'
        );

        enviar_email($this->emlremetente, $destinatarios, $this->emlassunto, $this->emlconteudo, '', 'ranieryribeiro@mec.gov.br', $anexos);
        $this->emldataatualizacao = 'now()';
        $this->emlstatus = 'S';

        // Verifica se é para registrar o envio do e-mail no registro de atividades
        if ($this->emlregistroatividade == 't' && $this->obrid) {
            $rga = $this->registraAtividade($arqidConteudo);
        } else {
            $this->rgaid = null;
        }

        parent::salvar();
        $this->commit();
    }

    /**
     * Envia o e-mail para todos os destinatários, com todos os anexos,
     * Um dos anexos é o conteúdo do e-mail em pdf
     * O status do email é alterado para "E" caso tenha sido enviado com sucesso
     */
    public function enviarInacabada() {
        $destinatarios = $this->getDestinatarios();
        $anexos = $this->getAnexos();

        if (count($anexos)) {
            foreach ($anexos as $k => $value) {
                $anexos[$k]['nome'] = $value['arqnome'] . '.' . $value['arqextensao'];
            }
        }

        if ($this->emlregistroatividade) {
            $this->rgaid = $this->pegaUm("select nextval('obras2.registroatividade_rgaid_seq')");
            $this->emlconteudo = str_replace('__RGAID__', $this->rgaid, $this->emlconteudo);
        }

        $arqidConteudo = $this->montaPdfConteudo();
        $file = new FilesSimec(null, null, 'obras2');

        $anexos[] = array(
            'arquivo' => $file->getCaminhoFisicoArquivo($arqidConteudo),
            'nome' => 'Anexo.pdf'
        );
        
        enviar_email($this->emlremetente, $destinatarios, $this->emlassunto, $this->emlconteudo, '', 'ranieryribeiro@mec.gov.br', $anexos);
        $this->emldataatualizacao = 'now()';
        $this->emlstatus = 'S';

        // Verifica se é para registrar o envio do e-mail no registro de atividades
        if ($this->emlregistroatividade == 't' && $this->obrid) {
            $rga = $this->registraAtividade($arqidConteudo);
        } else {
            $this->rgaid = null;
        }

        parent::salvar();
        $this->commit();
    }

    /**
     * Retorna todos os usuários destinatários
     * 
     * return array
     */
    public function getDestinatarios() {
        $sqlDestinatario = "select dte.email as usuemail, dte.email as usunome 
                            from obras2.email eml
                            join obras2.destinatariosemail dte on dte.emlid = eml.emlid
                            where eml.emlid = $this->emlid";

        $this->destinatarios = $this->carregar($sqlDestinatario);

        // Se não for produção, enviar e-mail para usuário logado
        if(!IS_PRODUCAO){
            if(isset($_SESSION['usuemail'])) {
                $this->destinatarios = array(
                    array(
                        'usuemail' => $_SESSION['usuemail'],
                        'usunome' => $_SESSION['usuemail']
                    )
                );
            } else {
                $this->destinatarios = array(
                    array(
                        'usuemail' => 'ranieryribeiro@mec.gov.br',
                        'usunome' => 'ranieryribeiro@mec.gov.br'
                    )
                );
            }
        }

        return $this->destinatarios;
    }

    /**
     * Retorna todos os anexos do e-mail
     * 
     * @return array
     */
    public function getAnexos() {
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $sqlAnexos = "select arq.* from obras2.email eml 
                            join obras2.anexoemail aml on aml.emlid = eml.emlid
                            join arquivo arq on arq.arqid = aml.arqid
                            where eml.emlid = $this->emlid";

        $anexos = $this->carregar($sqlAnexos);
        $file = new FilesSimec(null, null, "obras2");

        if ($anexos) {
            foreach ($anexos as $key => $anexo) {
                $anexos[$key]['arquivo'] = $file->getCaminhoFisicoArquivo($anexo['arqid']);
            }
        } else {
            $anexos = array();
        }

        return $anexos;
    }

    /**
     * Registra na tabela de atividades do envio do e-mail
     */
    protected function registraAtividade($arqidConteudo) {
        // Monta o arquivo com corpo
        $sql = "select * from obras2.tipoemail where temid = $this->temid";
        $tipo = $this->pegaLinha($sql);

        $arDado = array();

        $arDado['rgaid'] = $this->rgaid;
        $arDado['arqid'] = $arqidConteudo;
        $arDado['obrid'] = $this->obrid;
        $arDado['rgaautomatica'] = 'true';
        $arDado['rgadscsimplificada'] = 'E-mail enviado (' . $tipo['temnome'] . ')';
        $arDado['rgadsccompleta'] = 'E-mail enviado (' . $tipo['temnome'] . ') para: ' . $this->destinatariosToString($this->getDestinatarios());


        if (empty($arDado['arqid']))
            $arDado['arqid'] = 'NULL';

        $sql = "INSERT INTO obras2.registroatividade (rgaid, arqid, obrid, rgaautomatica, rgadscsimplificada, rgadsccompleta) VALUES (
                  {$arDado['rgaid']},
                  {$arDado['arqid']},
                  {$arDado['obrid']},
                  {$arDado['rgaautomatica']},
                  '{$arDado['rgadscsimplificada']}',
                  '{$arDado['rgadsccompleta']}'
                  )";

        $this->executar($sql);

        return $this->rgaid;
    }

    /**
     * Metodo para gerar as assinaturas dos emails.
     * Ex.: Nome do Usuário (nome.usuario@mec.gov.br)
     * 
     * @param array $destinatarios
     * @return string
     */
    protected function destinatariosToString($destinatarios) {
        if (empty($destinatarios)) {
            return '';
        }

        if (is_string($destinatarios)) {
            return $destinatarios;
        }

        $assUsu = array();
        foreach ($destinatarios as $usuario) {
            $assUsu[] = $usuario['usuemail'];
        }
        return implode(', ', $assUsu);
    }

    /**
     * Envia todos os emails
     */
    public function enviarTudo($emlstatus = 'A') {
        $sql = "
            select * from obras2.email eml 
            where eml.emlstatus = '$emlstatus' LIMIT 500";
        $emails = $this->carregar($sql);
        if ($emails) {
            foreach ($emails as $email) {
                $this->popularDadosObjeto($email);
                $this->enviar();
            }
        }
    }

    /**
     * Retorna a entidade de acordo com o funid
     * 
     * @param int $obrid
     * @param int $funid
     * @return \Entidades
     */
    public function pegaEntidadePar($obrid, $funid) {
        global $db;
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        $entidade = new Entidades();

        if ($funid == 1 || $funid == 2) {
            $funid1 = 2;
            $funid2 = 1;
        } elseif ($funid == 7) {
            $funid1 = 15;
            $funid2 = 7;
        } elseif ($funid == 6 || $funid == 25) {
            $funid1 = 25;
            $funid2 = 6;
        }

        $sql = "select m.muncod, m.mundescricao, m.estuf, iu.inuid 
            from obras2.obras o
                inner join entidade.endereco e on e.endid = o.endid
                inner join territorios.municipio m on m.muncod = e.muncod
                inner join par.instrumentounidade iu on iu.muncod = m.muncod
            where o.obrid = $obrid";
        $dadosObras = $db->pegaLinha($sql);
        $entidadePar = $dadosObras['muncod'];
        $estuf = $dadosObras['estuf'];
        $stWhere .= $entidadePar ? "and eed2.muncod = '{$entidadePar}'" : "";
        $stWhere .= $estuf ? "and eed2.estuf = '{$estuf}'" : "";

        if ($funid == 1 || $funid == 6 || $funid == 7) {
            $sql = "SELECT
                        max(ent.entid) as entid1				
                    FROM entidade.entidade ent
                        INNER JOIN entidade.endereco 		eed2 ON eed2.entid = ent.entid
                        INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = {$funid2} AND fue.fuestatus = 'A'
                        INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid				
                    WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
                    {$stWhere}";

            $entid = $db->pegaUm($sql);
        } else {
            $sql = "SELECT
                        --max(ent.entid) as entid1
                        max(fue.fuedata) as data,
                        ent.entid as entid1								
                    FROM entidade.entidade ent
                        INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = {$funid1} AND fue.fuestatus = 'A'
                        INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
                        INNER JOIN seguranca.usuario usu ON usu.usucpf = ent.entnumcpfcnpj 				
                        LEFT JOIN par.historicodadospar hdp on hdp.usucpf = usu.usucpf
                        LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = fue.fueid
                        LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid 
                        LEFT JOIN entidade.endereco         eed2 ON eed2.entid = ent2.entid 
                        LEFT JOIN entidade.funcaoentidade 	fue2 ON fue2.entid = ent2.entid AND fue2.funid = {$funid2} AND fue2.fuestatus = 'A'
                        LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = fue2.funid
                    WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL)
                    AND fun2.funstatus IS NOT NULL
                    {$stWhere}

                    group by ent.entid, hdp.hpddatainsercao
                    order by hdp.hpddatainsercao desc
                    limit 1";
            $rsEntid = $db->pegaLinha($sql);
            $entid = $rsEntid['entid1'];
        }

        if ($entid) {
            $entidade->carregarPorEntid($entid);
        }

        return $entidade;
    }

    /**
     * Verifica se algum email desse tipo já foi enviado, no intervalo de dias passado
     * Função utilizada para controlar o envio de email, para que um email não seja enviado novamente
     * 
     * @param int $tipo
     * @param int $obrid
     * @return array|boolean
     */
    public function verificaEmailEnviado($tipo, $obrid, $dias = null) {
        global $db;

        // Verifica se ja foi enviado algum email para determinada obra e tipo
        $sql = "select 
                    *
                from 
                    obras2.email eml
                join 
                    obras2.tipoemail tem on tem.temid = eml.temid
                where 
                    eml.obrid = $obrid and tem.temid = $tipo and (eml.emlstatus = 'S' or eml.emlstatus = 'A')
                order by eml.emlid desc
                limit 1";

        $email = $db->pegaLinha($sql);

        // se nenhum email foi enviado retorna false
        if (!$email)
            return false;

        // Quando não é passado os dias, verifica somente se o email ja foi enviado ou não
        if (!$dias)
            return true;

        // Caso algum email foi enviado, verificar se ja passou a quantidade de dias informado
        $sql = "select 
                    count(*)
                from 
                    obras2.email eml
                join 
                    obras2.tipoemail tem on tem.temid = eml.temid
                where 
                    eml.emlid = {$email['emlid']} and ((emldata::date + interval '" . $dias . " day') < now())";
        $result = $db->pegaUm($sql);

        // Se o email enviado estiver vencido retorna false
        if ($result > 0)
            return false;

        // Se o email ainda esta válido retorna true
        return true;
    }

    /**
     * Retorna os email dos contatos PAR
     * Quando for esfera estadual, retorna o email do secretario
     * Quando for esfera municipal retorna o email da prefeitura e do prefeito
     * @param int $empid
     */
    public function pegaContatosPar($empid, $obrid) {
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;
        $dadosRemetentes = array();
        $esfera = $db->pegaUm("select empesfera from obras2.empreendimento where empid = " . $empid);

        if ($esfera == 'E') {
            $entSecretario = $this->pegaEntidadePar($obrid, 6);
            $entSecretaria = $this->pegaEntidadePar($obrid, 25);
            if ($entSecretario->getEntEmail())
                $dadosRemetentes[] = $entSecretario->getEntEmail();
            if ($entSecretaria->getEntEmail())
                $dadosRemetentes[] = $entSecretaria->getEntEmail();
        } else if ($esfera == 'M') {
            $entPrefeito = $this->pegaEntidadePar($obrid, 2);
            $entPrefeitura = $this->pegaEntidadePar($obrid, 1);
            $entSecretaria = $this->pegaEntidadePar($obrid, 7);

            if ($entPrefeitura->getEntEmail())
                $dadosRemetentes[] = $entPrefeitura->getEntEmail();
            if ($entPrefeito->getEntEmail())
                $dadosRemetentes[] = $entPrefeito->getEntEmail();
            if ($entSecretaria->getEntEmail())
                $dadosRemetentes[] = $entSecretaria->getEntEmail();
        } else {
            return false;
        }
        return $dadosRemetentes;
    }

    /**
     * 
     * @global type $db
     * @param integer $obridRegistra email com restrição ou inconformidade
     * de supervisão da obra para Supervisor e Gestor Unidade
     * @param integer $obrid
     * @return void(0)
     */
    public function enviaEmailRestricaoSupervisao($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);
        $contato = new Contato();
        $empid = $obra->empid;
//        $sql = $contato->ReponsavelObraEGestorUnidade($obra->empid);
        $sql = $contato->ReponsavelObraEGestorUnidadeGestorEmpresaMI($obra->empid);


        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                            <html>
                                <head>
                                    <title></title>
                                </head>
                                <body>
                                    <table style="width: 100%;">
                                        <thead>
                                            <tr>
                                                <td style="text-align: center;">
                                                    <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                    <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                    FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                    DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                    COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                    SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="text-align: right; padding: 40px 0 0 0;">
                                                    ' . $data . '
                                                </td>
                                            </tr>
                                        </thead>
                                        <tbody>
                                            <tr>
                                                <td style="padding:20px 0 20px 0;">
                                                  Assunto: <b>Restrições e/ou Inconformidades da obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="line-height: 15px; text-align:justify">
                                                    <p>Senhor Fiscal e Gestor da Unidade,</p>

                                                    <p>1. Em decorrência do monitoramento realizado por meio do Sistema Integrado de Monitoramento,

                                                    Execução e Controle do Ministério da Educação (Simec) e de supervisão realizada por empresa contratada

                                                    pelo FNDE, verificamos que a obra em questão, apresenta irregularidades que comprometem sua boa

                                                    execução e a plena realização do objeto pactuado. As irregularidades apontadas pela empresa supervisora

                                                    estão listadas na aba \'Restrições e Inconformidades\' da obra no Simec.</p>

                                                    <p>2. Desta forma, solicitamos a Vossa Excelência que sejam tomadas as devidas providencias a fim de

                                                    que as irregularidades apontadas sejam sanadas. Sugerimos que o técnico, fiscal da obra, providencie a

                                                    regularização das pendências solicitando, quando for o caso, que a empresa executora apresente resolução

                                                    dos problemas apontados. </p>

                                                    <p>3. Havendo superação das restrições e/ou inconformidades, o fato deve ser informado ao FNDE através

                                                    da inserção vistoria com fotos que comprovem a regularização da pendência no módulo Obras 2.0 do Simec

                                                    e/ou através de encaminhamento dos documentos requisitados que permitam a este FNDE verificar e ratificar

                                                    a regularização.</p>

                                                    <p>4. Solicitamos, no prazo máximo de 30 dias, que o cumprimento das providências requeridas, para

                                                    cada uma das irregularidades apontadas, sejam informadas a esta Autarquia, por meio do Sistema Integrado

                                                    de Monitoramento, Execução e Controle do Ministério da Educação (Simec). O não atendimento das

                                                    providências solicitadas para cada uma das restrições e inconformidades apontadas causará a suspensão do

                                                    repasse de recursos até a sua resolução.</p>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="padding: 10px 0 0 0;">
                                                        Atenciosamente,
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="text-align: center; padding: 10px 0 0 0;">
                                                        <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                                        <br />
                                                        <b>Fabrício Batista de Araújo<b>
                                                        <br />
                                                        Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                        <br />
                                                        CGIMP/DIRPE/FNDE/MEC
                                                </td>
                                            </tr>
                                        </tbody>
                                        <tfoot>

                                        </tfoot>
                                    </table>
                                </body>
                            </html>
                                    ',
            'emlassunto' => 'Restrições e/ou Inconformidades da obra ' . $obra->obrnome,
            'temid' => 1,
            'emlregistroatividade' => true,
            'obrid' => $obrid
        );

        $rsContatos = $db->carregar($sql);
        if (count($rsContatos)) {
            $dadosRemetentes = array();
            foreach ($rsContatos as $c) {
                array_push($dadosRemetentes, $c['usuemail']);
            }
            //$dadosRemetentes = array('lucass.web@gmail.com');

            $email = new Email();
            $email->popularDadosObjeto($dados);
            $email->salvar($dadosRemetentes);
            $email->enviar();
        }
    }

    public function enviaEmailSolicitacoes($slcid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        global $db;

        $sql = <<<DML
            SELECT usu.usuemail, obr.obrid, obr.obrnome, esd.esddsc
            FROM obras2.solicitacao sol
            INNER JOIN obras2.obras obr ON (sol.obrid = obr.obrid)
            INNER JOIN seguranca.usuario usu ON (sol.usucpf = usu.usucpf)
            INNER JOIN workflow.documento doc ON (sol.docid = doc.docid)
            INNER JOIN workflow.historicodocumento hdt ON (doc.hstid = hdt.hstid)
            INNER JOIN workflow.acaoestadodoc aed ON (hdt.aedid = aed.aedid)
            INNER JOIN workflow.estadodocumento esd ON (aed.esdiddestino = esd.esdid)
            WHERE slcid = $slcid
DML;
        $info = $db->pegaLinha($sql);
        $data = new Data();
        $dataTx = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        
        $html = '
             <html>
                <head>
                    <title></title>
                </head>
                <body>
                    <table style="width: 100%;">
                        <thead>
                            <tr>
                                <td style="text-align: center;">
                                    <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                    <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                    FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                    DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                    COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                    SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                </td>
                            </tr>
                            <tr>
                                <td style="text-align: right; padding: 40px 0 0 0;">
                                    ' . $dataTx . '
                                </td>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td style="padding:20px 0 20px 0;">
                                  Assunto: <b>Tramitação de Solicitação Nº '.$slcid.' da obra (' . $info['obrid'] . ') ' . $info['obrnome'] . '</b>
                                </td>
                            </tr>
                            <tr>
                                <td style="line-height: 15px; text-align:justify">
                                    <p>Prezado Gestor(a),</p>
                                    <p>A Solicitação N° '.$slcid.' foi tramitado para '. $info['esddsc'] .'.</p>
                                </td>
                            </tr>
                            <tr>
                                <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                    <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                </td>
                            </tr>
                        </tbody>
                        <tfoot>
                            <tr>
                                <td>&nbsp;</td>
                                <td>&nbsp;</td>
                            </tr>
                        </tfoot>
                    </table>
                </body>
            </html>
        ';

        $assunto = "Alerta de Solicitações da obra " . $info['obrnome'];
        $conteudo = $html;
        $destinatario = array($info['usuemail']);

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => $conteudo,
            'emlassunto' => $assunto,
            'temid' => 42,
            'emlregistroatividade' => true,
            'obrid' => $info['obrid']
        );

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($destinatario);
        $email->enviar();
        return true;
    }

    public function enviaEmailDemanda($docid = null, $dmdid = null) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        global $db;
        $where = array();
        $assunto = 'Cadastrado';
        $assunto2 = 'CADASTRO';
        if($docid) {
            $where[] = 'd.docid = '.$docid;
            $assunto = 'Tramitado';
            $assunto2 = 'TRAMITAÇÃO';
        }
        if($dmdid) {
            $where[] = 'd.dmdid = '.$dmdid;
        }
        $clausula = implode(' AND ', $where);
        $sql = <<<DML
            SELECT
                d.dmdid,
                d.docid,
                d.dmdnome,
                TO_CHAR(d.dmddatacadastro,'DD/MM/YYYY') as dmddatacadastro,
                usu.usunome AS usunomesolicitante,
                usu.usucpf AS usucpfsolicitante,
                usu.usuemail AS usuemailsolicitante,
                usu1.usunome AS usunomeresponsavel,
                usu1.usucpf AS usucpfresponsavel,
                usu1.usuemail AS usuemailresponsavel,
                usu2.usunome AS usunomecadastro,
                usu2.usucpf AS usucpfcadastro,
                usu2.usuemail AS usuemailcadastro,
                usu3.usunome as usunometramitador,
                usu3.usucpf as usucpftramitador,
                usu3.usuemail as usuemailtramitador,
                to_char(hdt.htddata,'DD/MM/YYYY') AS htddata,
                aed.aeddscrealizada,
                esd.esddsc as estadoorigem,
                esd2.esddsc as estadodestino,
                cd.cmddsc
            FROM obras2.demandas d
            LEFT JOIN seguranca.usuario usu ON (d.dmdcpfsolicitante = usu.usucpf)
            LEFT JOIN seguranca.usuario usu1 ON (d.dmdcpfresponsavel = usu1.usucpf)
            LEFT JOIN seguranca.usuario usu2 ON (d.dmdcpfcadastro = usu2.usucpf)
            LEFT JOIN workflow.documento doc ON (d.docid = doc.docid)
            LEFT JOIN workflow.historicodocumento hdt on (doc.hstid = hdt.hstid)
            LEFT JOIN workflow.acaoestadodoc aed ON (hdt.aedid = aed.aedid)
            LEFT JOIN workflow.estadodocumento esd ON (aed.esdidorigem = esd.esdid)
            LEFT JOIN workflow.estadodocumento esd2 ON (aed.esdiddestino = esd2.esdid)
            LEFT JOIN seguranca.usuario usu3 ON (hdt.usucpf = usu3.usucpf)
            LEFT JOIN workflow.comentariodocumento cd ON (hdt.hstid = cd.hstid)
            WHERE $clausula
DML;
        $info = $db->pegaLinha($sql);
        $nome_responsavel = $info['usunomeresponsavel'];
        $cpf_responsavel = $info['usucpfresponsavel'];
        $nome_solicitante = $info['usunomesolicitante'];
        $cpf_solicitante = $info['usucpfsolicitante'];
        $nome_demanda = $info['dmdnome'];
        $estado_atual_demanda = 'Em Cadastramento';
        $estado_anterior_demanda = 'N/A';
        $data_tramitacao = $info['dmddatacadastro'];
        $nome_tramitador = $info['usunomecadastro'];
        $email_tramitador = $info['usuemailcadastro'];
        $acao_realizada = 'Cadastro';
        $comentario_tramitacao = 'N/A';
        if($info['docid']) {
            $estado_atual_demanda = $info['estadodestino'];
            $estado_anterior_demanda = $info['estadoorigem'];
            $data_tramitacao = $info['htddata'];
            $nome_tramitador = $info['usunometramitador'];
            $email_tramitador = $info['usuemailtramitador'];
            $acao_realizada = $info['aeddscrealizada'];
            $comentario_tramitacao = $info['cmddsc'];
        }
        $data = new Data();
        $dataTx = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $html = '
            <html>
                <head>
                    <title></title>
                </head>
                <body>
                    <table style="width: 100%;">
                        <thead>
                            <tr>
                                <td style="text-align: center; bgcolor: #ccc;" colspan="2" >
                                    <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                    <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                    FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                    DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                    COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                    SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                </td>
                            </tr>
                            <tr>
                                <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                    ' . $dataTx . '
                                </td>
                            </tr>
                            <tr>
                                <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                    &nbsp;
                                </td>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                    <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                </td>
                            </tr>
                            <tr>
                                <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                    &nbsp;
                                </td>
                            </tr>
                            <tr>
                                <td style="line-height: 15px;" colspan="2">
                                    <b> '.$assunto2.' DE DEMANDA </b>
                                </td>
                            </tr>
                            <tr>
                                <td style="padding:20px 0 20px 0;" colspan="2">
                                  Assunto: SIMEC - DEMANDA - Nº ' . $info['dmdid'] . ' - '.strtoupper($assunto).'
                                </td>
                            </tr>

                            <tr>
                                <td>Nome do Solicitante:</td>
                                <td>' . $nome_solicitante . '</td>
                            </tr>
                            <tr>
                                <td>Nome do Responsável:</td>
                                <td>' . $nome_responsavel . '</td>
                            </tr>
                            <tr>
                                <td>Demanda:</td>
                                <td>' . $nome_demanda . '</td>
                            </tr>
                            <tr>
                                <td>Estado Atual:</td>
                                <td><b> ' . $estado_atual_demanda . ' </b></td>
                            </tr>
                            <tr>
                                <td>&nbsp;</td>
                                <td>&nbsp;</td>
                            </tr>
                            <tr>
                                <td>Estado Anterior:</td>
                                <td><b> ' . $estado_anterior_demanda . ' </b></td>
                            </tr>
                            <tr>
                                <td>Data da Tramitação:</td>
                                <td>' . $data_tramitacao . '</td>
                            </tr>
                            <tr>
                                <td>Quem tramitou:</td>
                                <td> ' . $nome_tramitador . ' - (' . $email_tramitador . ')</td>
                            </tr>
                            <tr>
                                <td>Ação realizada:</td>
                                <td> <b>' . $acao_realizada . '</b></td>
                            </tr>
                            <tr>
                                <td>Comentário da Tramitação:</td>
                                <td> ' . $comentario_tramitacao . '</td>
                            </tr>
                            <tr>
                                <td>&nbsp;</td>
                                <td>&nbsp;</td>
                            </tr>
                            <tr>
                                <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                    <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                </td>
                            </tr>
                        </tbody>
                        <tfoot>
                            <tr>
                                <td>&nbsp;</td>
                                <td>&nbsp;</td>
                            </tr>
                        </tfoot>
                    </table>
                </body>
            </html>
        ';

        $assunto = "SIMEC - DEMANDA - Nº {$info['dmdid']} - ".strtoupper($assunto);
        $conteudo = $html;
        $destinatario = array($info['usuemailsolicitante'],$info['usuemailcadastro'],$info['usuemailresponsavel']);
        if(!in_array($_SESSION['usuemail'], $destinatario)) {
            $destinatario[] = $_SESSION['usuemail'];
        }

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => $conteudo,
            'emlassunto' => $assunto,
            'temid' => 45,
            'emlregistroatividade' => true,
            'obrid' => $info['obrid']
        );

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar(array_unique($destinatario));
        $email->enviar();
        return true;
    }

    /**
     * Adiciona um email na fila com alerta para restricos e/ou inconformidades da obra
     * 
     * @global type $db
     * @param type $obrid
     * @return type
     */
    public function enviaEmalRestricaoObra($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);
        $esfera = $db->pegaUm("select empesfera from obras2.empreendimento where empid = " . $obra->empid);

        // Por enquanto e-mail enviado somente para prefeitura
        if ($esfera != 'M')
            return;

        if ($esfera == 'E') {
            $entPrefeito = $this->pegaEntidadePar($obrid, 6);
            $entPrefeitura = $this->pegaEntidadePar($obrid, 25);
        } else {
            $entPrefeito = $this->pegaEntidadePar($obrid, 2);
            $entPrefeitura = $this->pegaEntidadePar($obrid, 7);
        }

        if (is_array($entPrefeitura->enderecos)) {
            $enderecoPrefeitura = current($entPrefeitura->enderecos);
        } else {
            $enderecoPrefeitura = $entPrefeitura->enderecos;
        }

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . $data . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="line-height: 15px;">
                                                A Sua Excelência a Senhor(a)
                                                <br />
                                                ' . $entPrefeito->getEntnome() . '
                                                <br />
                                                Prefeito(a) do Município de ' . $enderecoPrefeitura['mundescricao'] . ' - ' . $enderecoPrefeitura['estuf'] . '
                                                <br />
                                                ' . $enderecoPrefeitura['endlog'] . ', nº ' . $enderecoPrefeitura['endnum'] . ' - ' . $enderecoPrefeitura['endbai'] . '
                                                <br />
                                                ' . $enderecoPrefeitura['mundescricao'] . ' - ' . $enderecoPrefeitura['estuf'] . ' - ' . $enderecoPrefeitura['endcep'] . '
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Restrições e/ou Inconformidades da obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor Prefeito(a),</p>

                                                <p>1. Em decorrência do monitoramento realizado por meio do Sistema Integrado de Monitoramento, 

                                                Execução e Controle do Ministério da Educação (Simec) e de supervisão realizada por empresa contratada 

                                                pelo FNDE, verificamos que a obra em questão, apresenta irregularidades que comprometem sua boa 

                                                execução e a plena realização do objeto pactuado. As irregularidades apontadas pela empresa supervisora 

                                                estão listadas na aba \'Restrições e Inconformidades\' da obra no Simec.</p>

                                                <p>2. Desta forma, solicitamos a Vossa Excelência que sejam tomadas as devidas providencias a fim de 

                                                que as irregularidades apontadas sejam sanadas. Sugerimos que o técnico, fiscal da obra, providencie a 

                                                regularização das pendências solicitando, quando for o caso, que a empresa executora apresente resolução 

                                                dos problemas apontados. </p>

                                                <p>3. Havendo superação das restrições e/ou inconformidades, o fato deve ser informado ao FNDE através 

                                                da inserção vistoria com fotos que comprovem a regularização da pendência no módulo Obras 2.0 do Simec 

                                                e/ou através de encaminhamento dos documentos requisitados que permitam a este FNDE verificar e ratificar 

                                                a regularização.</p>

                                                <p>4. Solicitamos, no prazo máximo de 30 dias, que o cumprimento das providências requeridas, para 

                                                cada uma das irregularidades apontadas, sejam informadas a esta Autarquia, por meio do Sistema Integrado 

                                                de Monitoramento, Execução e Controle do Ministério da Educação (Simec). O não atendimento das 

                                                providências solicitadas para cada uma das restrições e inconformidades apontadas causará a suspensão do 

                                                repasse de recursos até a sua resolução.</p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="padding: 10px 0 0 0;">
                                                    Atenciosamente,
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: center; padding: 10px 0 0 0;">
                                                    <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                                    <br />
                                                    <b>Fabrício Batista de Araújo<b>
                                                    <br />
                                                    Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                    <br />
                                                    CGIMP/DIRPE/FNDE/MEC
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>

                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Restrições e/ou Inconformidades da obra ' . $obra->obrnome,
            'temid' => 1,
            'emlregistroatividade' => true,
            'obrid' => $obrid
        );

        // $dadosAnexos = array(5,8,9);
        $dadosRemetentes = array($entPrefeito->getEntEmail(), $entPrefeitura->getEntEmail());
        //$dadosRemetentes = array('lucass.web@gmail.com');

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }

    /**
     * Adiciona na fila de e-mails um e-mail para cada obra com mais de 60 dias sem atualização ('obras vermelhas')
     */
    public function enviaEmail60Dias() {
        global $db;
        require_once APPRAIZ . "includes/classes/dateTime.inc";

        // Pega obras com 60 dias
        $obras = new Obras();
        $cabecalho = $obras->cabecalhoListaSql();
        $sql = "
            SELECT DISTINCT o.*
            FROM obras2.obras o
            LEFT JOIN workflow.documento 		 d ON d.docid = o.docid AND tpdid = 105
            LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = d.esdid
            LEFT JOIN obras2.empreendimento 	 e ON e.empid = o.empid AND e.empstatus = 'A'
            WHERE
                o.obrstatus = 'A' AND o.obridpai IS NULL AND d.esdid IN(691,690) AND e.orgid IN(3) AND 
                CASE
                    WHEN
                    (obrdtultvistoria IS NOT NULL AND 
                    ed.esdid IN (690, 691) AND
                    DATE_PART('days', NOW() - obrdtultvistoria) > 60 AND
                    obrpercentultvistoria < 100.00) THEN								
                        true
                    ELSE
                        false				
                END";

        $listaObras = $db->carregar($sql);

//        ver(count($listaObras),d);

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        foreach ($listaObras as $obra) {

            if (!$this->verificaEmailEnviado(4, $obra['obrid'], 7)) {

                $dadosRemetentes = $this->pegaContatosPar($obra['empid'], $obra['obrid']);
                if (empty($dadosRemetentes))
                    continue;

                $responsaveis = $this->pegaResponsaveisObra($obra['empid']);

                if (!empty($responsaveis) && !empty($dadosRemetentes))
                    $dadosRemetentes = array_merge($responsaveis, $dadosRemetentes);

                $dados = array(
                    'usucpf' => $_SESSION['usucpf'],
                    'emlconteudo' => '
                                <html>
                                    <head>
                                        <title></title>
                                    </head>
                                    <body>
                                        <table style="width: 100%;">
                                            <thead>
                                                <tr>
                                                    <td style="text-align: center;">
                                                        <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                        <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                        DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                        COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                        SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td style="text-align: right; padding: 40px 0 0 0;">
                                                        ' . $data . '
                                                    </td>
                                                </tr>
                                            </thead>
                                            <tbody>
                                                <tr>
                                                    <td style="line-height: 15px;">

                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td style="padding:20px 0 20px 0;">
                                                      Assunto: <b>Aviso de 60 dias sem atualização da obra (' . $obra['obrid'] . ') ' . $obra['obrnome'] . '</b>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td style="line-height: 15px; text-align:justify">
                                                        <p>Sr(a). Gestor, a obra (' . $obra['obrid'] . ') ' . $obra['obrnome'] . ' está há mais de 60 dias sem atualização/inserção de vistoria, solicitamos que seja realizada com urgência a atualização do Simec, módulo Obras 2.0. Informamos que esta obra agora passa a ser uma pendência e bloqueia a inserção de informações no módulo PAR.</p>
                                                        
                                                        <p>Para obter informações de como preencher o sistema, acesse o manual no portal do FNDE, por meio de seguinte link: http://www.fnde.gov.br/programas/par/par-monitoramento. Após leitura do manual, caso persista alguma dúvida, encaminhar e-mail para monitoramento.obras@fnde.gov.br.</b>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td style="padding: 10px 0 0 0;">
                                                            Atenciosamente,
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td style="text-align: center; padding: 10px 0 0 0;">
                                                            <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                                            <br />
                                                            <b>Fabrício Batista de Araújo<b>
                                                            <br />
                                                            Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                            <br />
                                                            CGIMP/DIRPE/FNDE/MEC
                                                    </td>
                                                </tr>
                                            </tbody>
                                            <tfoot>

                                            </tfoot>
                                        </table>
                                    </body>
                                </html>
                                            ',
                    'emlassunto' => 'Aviso de 60 dias sem atualização da obra ' . $obra['obrnome'],
                    'temid' => 4,
                    'emlregistroatividade' => true,
                    'obrid' => $obra['obrid']
                );

                $email = new Email();
                $email->popularDadosObjeto($dados);
                $email->salvar($dadosRemetentes);
            }
        }
    }

    /**
     * Adiciona na fila de e-mails um e-mail para cada obra com mais de 45 ate 60 dias sem atualização ('obras amarelas')
     */
    public function enviaEmail45Dias() {
        global $db;
        require_once APPRAIZ . "includes/classes/dateTime.inc";

        // Pega obras com 60 dias
        $obras = new Obras();
        $cabecalho = $obras->cabecalhoListaSql();
        $sql = "
            SELECT DISTINCT o.*
            FROM obras2.obras o
            LEFT JOIN workflow.documento 		 d ON d.docid = o.docid AND tpdid = 105
            LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = d.esdid
            LEFT JOIN obras2.empreendimento 	 e ON e.empid = o.empid AND e.empstatus = 'A'
            WHERE
                o.obrstatus = 'A' AND o.obridpai IS NULL AND d.esdid IN(691,690) AND e.orgid IN(3) AND 
                CASE
                    WHEN
                    (obrdtultvistoria IS NOT NULL AND 
                    ed.esdid IN (690, 691
                    ) AND
                    DATE_PART('days', NOW() - obrdtultvistoria) BETWEEN 46 AND 60 AND
                    obrpercentultvistoria < 100.00) THEN								
                        true
                    ELSE
                        false				
                END";

        $listaObras = $db->carregar($sql);

//        ver(count($listaObras),d);

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        foreach ($listaObras as $obra) {

            if (!$this->verificaEmailEnviado(3, $obra['obrid'], 7)) {

//                if(!$this->verificaEmailEnviado(3, $obra['obrid'])){

                $dadosRemetentes = $this->pegaContatosPar($obra['empid'], $obra['obrid']);
                if (empty($dadosRemetentes))
                    continue;

                $responsaveis = $this->pegaResponsaveisObra($obra['empid']);

                if (!empty($responsaveis) && !empty($dadosRemetentes)) {
                    $dadosRemetentes = array_merge($responsaveis, $dadosRemetentes);
                }

                $dados = array(
                    'usucpf' => $_SESSION['usucpf'],
                    'emlconteudo' => '
                                    <html>
                                        <head>
                                            <title></title>
                                        </head>
                                        <body>
                                            <table style="width: 100%;">
                                                <thead>
                                                    <tr>
                                                        <td style="text-align: center;">
                                                            <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                            <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                            FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                            DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                            COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                            SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="text-align: right; padding: 40px 0 0 0;">
                                                            ' . $data . '
                                                        </td>
                                                    </tr>
                                                </thead>
                                                <tbody>
                                                    <tr>
                                                        <td style="line-height: 15px;">

                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="padding:20px 0 20px 0;">
                                                          Assunto: <b>Aviso de 45 dias sem atualização da obra (' . $obra['obrid'] . ') ' . $obra['obrnome'] . '</b>
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="line-height: 15px; text-align:justify">
                                                            <p>Sr(a). Gestor, a obra (' . $obra['obrid'] . ') ' . $obra['obrnome'] . ' está há mais de 45 dias sem atualização/inserção de vistoria, solicitamos que seja realizada com urgência a atualização do Simec, módulo Obras 2.0.</p>

                                                            <p>A ausência de informações atualizadas no Simec poderá ocasionar o bloqueio dos recursos ou de futuros repasses financeiros, atraso na aquisição dos equipamentos e mobiliários ou ainda o cancelamento do Termo de Compromisso/Convênio com o envio para Tomada de Contas Especial.</p>

                                                            <p>Para obter informações de como preencher o sistema, acesse o manual no portal do FNDE, por meio de seguinte link: http://www.fnde.gov.br/programas/par/par-monitoramento. Após leitura do manual, caso persista alguma dúvida, encaminhar e-mail para monitoramento.obras@fnde.gov.br.</p>
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="padding: 10px 0 0 0;">
                                                                Atenciosamente,
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="text-align: center; padding: 10px 0 0 0;">
                                                                <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                                                <br />
                                                                <b>Fabrício Batista de Araújo<b>
                                                                <br />
                                                                Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                                <br />
                                                                CGIMP/DIRPE/FNDE/MEC
                                                        </td>
                                                    </tr>
                                                </tbody>
                                                <tfoot>

                                                </tfoot>
                                            </table>
                                        </body>
                                    </html>
                                                ',
                    'emlassunto' => 'Aviso de 45 dias sem atualização da obra ' . $obra['obrnome'],
                    'temid' => 3,
                    'emlregistroatividade' => true,
                    'obrid' => $obra['obrid']
                );

                $email = new Email();
                $email->popularDadosObjeto($dados);
                $email->salvar($dadosRemetentes);
//                }
            }
        }
    }

    /**
     * Adiciona na fila de e-mails um e-mail para cada obra com mais de 45 ate 60 dias sem atualização ('obras amarelas')
     */
    public function enviaEmail30Dias() {
        global $db;
        require_once APPRAIZ . "includes/classes/dateTime.inc";

        // Pega obras com 60 dias
        $obras = new Obras();
        $cabecalho = $obras->cabecalhoListaSql();
        $sql = "
            SELECT DISTINCT o.*
            FROM obras2.obras o
            LEFT JOIN workflow.documento        d ON d.docid = o.docid AND tpdid = 105
            LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
            LEFT JOIN obras2.empreendimento     e ON e.empid = o.empid AND e.empstatus = 'A'
            WHERE
                o.obrstatus = 'A' AND o.obridpai IS NULL AND d.esdid IN(691,690) AND e.orgid IN(3) AND 
                CASE
                    WHEN
                    (obrdtultvistoria IS NOT NULL AND 
                    ed.esdid IN (690, 691) AND
                    DATE_PART('days', NOW() - obrdtultvistoria) BETWEEN 30 AND 45 AND
                    obrpercentultvistoria < 100.00) THEN								
                        true
                    ELSE
                        false				
                END";

        $listaObras = $db->carregar($sql);

//        ver(count($listaObras),d);

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        foreach ($listaObras as $obra) {
            if (!$this->verificaEmailEnviado(2, $obra['obrid'], 7)) {
//                if(!$this->verificaEmailEnviado(2, $obra['obrid'])){

                $dadosRemetentes = $this->pegaContatosPar($obra['empid'], $obra['obrid']);

                if (empty($dadosRemetentes))
                    continue;

                $responsaveis = $this->pegaResponsaveisObra($obra['empid']);


                if (!empty($responsaveis) && !empty($dadosRemetentes))
                    $dadosRemetentes = array_merge($responsaveis, $dadosRemetentes);

                $dados = array(
                    'usucpf' => $_SESSION['usucpf'],
                    'emlconteudo' => '
                                    <html>
                                        <head>
                                            <title></title>
                                        </head>
                                        <body>
                                            <table style="width: 100%;">
                                                <thead>
                                                    <tr>
                                                        <td style="text-align: center;">
                                                            <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                            <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                            FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                            DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                            COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                            SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="text-align: right; padding: 40px 0 0 0;">
                                                            ' . $data . '
                                                        </td>
                                                    </tr>
                                                </thead>
                                                <tbody>
                                                    <tr>
                                                        <td style="line-height: 15px;">

                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="padding:20px 0 20px 0;">
                                                          Assunto: <b>Aviso de 30 dias sem atualização da obra (' . $obra['obrid'] . ') ' . $obra['obrnome'] . '</b>
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="line-height: 15px; text-align:justify">
                                                             <p>Sr(a). Gestor, a obra (' . $obra['obrid'] . ') ' . $obra['obrnome'] . ' está há mais de 30 dias sem atualização/inserção de vistoria, solicitamos que seja realizada com urgência a atualização do Simec, módulo Obras 2.0.</p>

                                                            <p>Para obter informações de como preencher o sistema, acesse o manual no portal do FNDE, por meio de seguinte link: http://www.fnde.gov.br/programas/par/par-monitoramento. Após leitura do manual, caso persista alguma dúvida, encaminhar e-mail para monitoramento.obras@fnde.gov.br.</p>
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="padding: 10px 0 0 0;">
                                                                Atenciosamente,
                                                        </td>
                                                    </tr>
                                                    <tr>
                                                        <td style="text-align: center; padding: 10px 0 0 0;">
                                                                <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                                                <br />
                                                                <b>Fabrício Batista de Araújo<b>
                                                                <br />
                                                                Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                                <br />
                                                                CGIMP/DIRPE/FNDE/MEC
                                                        </td>
                                                    </tr>
                                                </tbody>
                                                <tfoot>

                                                </tfoot>
                                            </table>
                                        </body>
                                    </html>
                                                ',
                    'emlassunto' => 'Aviso de 30 dias sem atualização da obra ' . $obra['obrnome'],
                    'temid' => 2,
                    'emlregistroatividade' => true,
                    'obrid' => $obra['obrid']
                );

                $email = new Email();
                $email->popularDadosObjeto($dados);
                $email->salvar($dadosRemetentes);
//                }
            }
        }
    }

    public function pegaResponsaveisObra($empid) {
        global $db;
        $sql = "
                select 
                    u.usuemail 	
                from obras2.usuarioresponsabilidade ur 
                inner join seguranca.usuario u on u.usucpf = ur.usucpf
                inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and us.sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
                where ur.empid = {$empid} and ur.rpustatus = 'A' AND u.suscod = 'A'";

        $resp = $db->carregar($sql);
        $result = array();
        if (empty($resp))
            return $retult;

        foreach ($resp as $email) {
            $result[] = $email['usuemail'];
        }
        return $result;
    }

    public function enviaEmailTramitacaoOsMi($osmid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";

        global $db;

        $osMI = new OrdemServicoMI();
        $osDados = $osMI->carregarPorOsmid($osmid);
        $obrid = $osDados['obrid'];
        $docid = $osDados['docid'];

        $obra = new Obras($obrid);
        $empid = $obra->empid;
        $nome_obra = '(' . $obrid . ')' . $obra->obrnome;

        $dados_tramitacao = $osMI->getDadosTramitacaoOsMi($docid);

        // Monta o filtro de Perfis Responsáveis por cada ação do Workflow
        switch ($dados_tramitacao['aedid']) {
            // CADASTRAMENTO - Enviar para aceite da empresa
            case 2079:
                $filtro_resp = PFLCOD_EMPRESA_MI_GESTOR;
                break;
            // CADASTRAMENTO - Cancelar
            case 2080:
                $filtro_resp = '' . PFLCOD_GESTOR_MEC . ',' . PFLCOD_SUPERVISOR_MEC . ',' . PFLCOD_GESTOR_UNIDADE . ',' . PFLCOD_SUPERVISOR_UNIDADE . '';
                break;
            // Aguardando aceite da empresa - Reprovar O.S.
            case 2081:
                $filtro_resp = '' . PFLCOD_GESTOR_MEC . ',' . PFLCOD_SUPERVISOR_MEC . ',' . PFLCOD_GESTOR_UNIDADE . ',' . PFLCOD_SUPERVISOR_UNIDADE . '';
                break;
            // Aguardando aceite da empresa - Aceitar O.S. para Execução
            case 2082:
                $filtro_resp = PFLCOD_EMPRESA_MI_GESTOR . ',' . PFLCOD_EMPRESA_MI_FISCAL;
                break;
            // Execução - Enviar para validação
            case 2083:
                $filtro_resp = '' . PFLCOD_GESTOR_MEC . ',' . PFLCOD_GESTOR_UNIDADE . ',' . PFLCOD_SUPERVISOR_UNIDADE . '';
                break;
            // Validação - Aprovar execução
            case 2084:
                $filtro_resp = '' . PFLCOD_GESTOR_MEC . ',' . PFLCOD_GESTOR_UNIDADE . ',' . PFLCOD_SUPERVISOR_UNIDADE . '';
                break;
            // Validação - Enviar para correção
            case 2085:
                $filtro_resp = PFLCOD_EMPRESA_MI_GESTOR . ',' . PFLCOD_EMPRESA_MI_FISCAL;
                break;
            // Correção da execução - Enviar para validação
            case 2086:
                $filtro_resp = '' . PFLCOD_GESTOR_MEC . ',' . PFLCOD_GESTOR_UNIDADE . ',' . PFLCOD_SUPERVISOR_UNIDADE . '';
                break;
            // Concluída - Retornar para execução
            case 2087:
                $filtro_resp = PFLCOD_EMPRESA_MI_GESTOR . ',' . PFLCOD_EMPRESA_MI_FISCAL;
                break;
            // Cancelada - Retornar para cadastradamento
            case 2088:
                $filtro_resp = '' . PFLCOD_GESTOR_MEC . ',' . PFLCOD_SUPERVISOR_MEC . ',' . PFLCOD_GESTOR_UNIDADE . ',' . PFLCOD_SUPERVISOR_UNIDADE . '';
                break;
            default:
                break;
        }

        $filtro_sql_resp = " AND ur.pflcod IN (" . $filtro_resp . ")";

        $sql_responsaveis = "SELECT DISTINCT
                                         su.usucpf   as cpf,
                                         su.usunome  as nome,
                                         su.usuemail as email
                             FROM seguranca.usuario su
                             INNER JOIN obras2.usuarioresponsabilidade ur ON ur.usucpf = su.usucpf AND ur.rpustatus = 'A'
                             inner join seguranca.usuario_sistema us on us.usucpf = su.usucpf and us.sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
                             WHERE
                                     su.suscod = 'A' AND
                                     ur.empid = {$empid}
                                     {$filtro_sql_resp}
                             ORDER BY
                                     su.usunome";

        $dados_responsaveis = $db->carregar($sql_responsaveis);
        $dados_responsaveis = (is_array($dados_responsaveis) && count($dados_responsaveis) > 0) ? $dados_responsaveis : array();

        $html = ' Não foi encontrato nenhum Responsável pela Obra para a ação: ' . $dados_tramitacao['aedid'];
        $possui_responsavel = false;

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        foreach ($dados_responsaveis as $key => $usuario) {
            $nome_responsavel = $usuario['nome'];
            $cpf_responsavel = $usuario['cpf'];
            $email_responsavel = $usuario['email'];

            $data_tramitacao = $dados_tramitacao['data_tramitacao'];
            $nome_tramitador = $dados_tramitacao['usu_nome_exec_tramitacao'];
            $email_tramitador = $dados_tramitacao['usu_email_exec_tramitacao'];
            $estado_atual_os = $dados_tramitacao['estado_de_destino'];
            $estado_anterior_os = $dados_tramitacao['estado_de_origem'];
            $acao_realizada = $dados_tramitacao['aeddscrealizada'];

            $html = ' 
                    <html>
                        <head>
                            <title></title>
                        </head>
                        <body>
                            <table style="width: 100%;">
                                <thead>
                                    <tr>
                                        <td style="text-align: center; bgcolor: #ccc;" colspan="2" >
                                            <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                            <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                            FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                            DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                            COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                            SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                            ' . $data . '
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                            &nbsp;
                                        </td>
                                    </tr>
                                </thead>
                                <tbody>
                                    <tr>
                                        <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                            <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                            &nbsp;
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="line-height: 15px;" colspan="2">
                                            <b> TRAMITAÇÃO DE O.S. </b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="padding:20px 0 20px 0;" colspan="2">
                                          Assunto: SIMEC - ORDEM DE SERVICO - Nº ' . $osmid . ' - Tramitada
                                        </td>
                                    </tr>

                                    <tr>
                                        <td>Nome do Responsável:</td>
                                        <td>' . $nome_responsavel . '</td>
                                    </tr>
                                    <tr>
                                        <td>CPF do Responsável:</td>
                                        <td>' . $cpf_responsavel . '</td>
                                    </tr>
                                    <tr>
                                        <td>Obra:</td>
                                        <td>' . $nome_obra . '</td>
                                    </tr>                            
                                    <tr>
                                        <td>Estado Atual:</td>
                                        <td><b> ' . $estado_atual_os . ' </b></td>
                                    </tr>
                                    <tr>
                                        <td>&nbsp;</td>
                                        <td>&nbsp;</td>
                                    </tr>
                                    <tr>
                                        <td>Estado Anterior:</td>
                                        <td><b> ' . $estado_anterior_os . ' </b></td>
                                    </tr>
                                    <tr>
                                        <td>Data da Tramitação:</td>
                                        <td>' . $data_tramitacao . '</td>
                                    </tr>
                                    <tr>
                                        <td>Quem tramitou:</td>
                                        <td> ' . $nome_tramitador . ' - (' . $email_tramitador . ')</td>
                                    </tr>
                                    <tr>
                                        <td>Ação realizada:</td>
                                        <td> <b>' . $acao_realizada . '</b></td>
                                    </tr>                        
                                    <tr>
                                        <td>&nbsp;</td>
                                        <td>&nbsp;</td>
                                    </tr>
                                    <tr>
                                        <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                            <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                        </td>
                                    </tr>
                                </tbody>
                                <tfoot>
                                    <tr>
                                        <td>&nbsp;</td>
                                        <td>&nbsp;</td>
                                    </tr>
                                </tfoot>
                            </table>
                        </body>
                    </html>
                    ';

            $assunto = "SIMEC - ORDEM DE SERVICO - Nº {$osmid} - Tramitada";
            $conteudo = $html;
            $destinatario = array($email_responsavel);
            $possui_responsavel = true;

            $dados = array(
                'usucpf' => $_SESSION['usucpf'],
                'emlconteudo' => $conteudo,
                'emlassunto' => $assunto,
                'temid' => 5,
                'emlregistroatividade' => true,
                'obrid' => $obrid
            );

            $email = new Email();
            $email->popularDadosObjeto($dados);
            $email->salvar($destinatario);
            $email->enviar();
        }

        if ($possui_responsavel == false) {

            $data_tramitacao = $dados_tramitacao['data_tramitacao'];
            $nome_responsavel = 'Usuário não encontrado';
            $cpf_responsavel = '000000';
            $email_responsavel = '';
            $nome_tramitador = $dados_tramitacao['usu_nome_exec_tramitacao'];
            $email_tramitador = $dados_tramitacao['usu_email_exec_tramitacao'];
            $estado_atual_os = $dados_tramitacao['estado_de_destino'];
            $estado_anterior_os = $dados_tramitacao['estado_de_origem'];
            $acao_realizada = $dados_tramitacao['aeddscrealizada'];

            $html = ' 
                    <html>
                        <head>
                            <title></title>
                        </head>
                        <body>
                            <table style="width: 100%;">
                                <thead>
                                    <tr>
                                        <td style="text-align: center; bgcolor: #ccc;" colspan="2" >
                                            <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                            <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                            FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                            DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                            COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                            SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                            ' . $data . '
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                            &nbsp;
                                        </td>
                                    </tr>
                                </thead>
                                <tbody>
                                    <tr>
                                        <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                            <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                            &nbsp;
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="line-height: 15px;" colspan="2">
                                            <b> TRAMITAÇÃO DE O.S. </b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="padding:20px 0 20px 0;" colspan="2">
                                          Assunto: SIMEC - ORDEM DE SERVICO - Nº ' . $osmid . ' - Tramitada
                                        </td>
                                    </tr>

                                    <tr>
                                        <td>Nome do Responsável:</td>
                                        <td>' . $nome_responsavel . '</td>
                                    </tr>
                                    <tr>
                                        <td>CPF do Responsável:</td>
                                        <td>' . $cpf_responsavel . '</td>
                                    </tr>
                                    <tr>
                                        <td>Obra:</td>
                                        <td>' . $nome_obra . '</td>
                                    </tr>                            
                                    <tr>
                                        <td>Estado Atual:</td>
                                        <td><b> ' . $estado_atual_os . ' </b></td>
                                    </tr>
                                    <tr>
                                        <td>&nbsp;</td>
                                        <td>&nbsp;</td>
                                    </tr>
                                    <tr>
                                        <td>Estado Anterior:</td>
                                        <td><b> ' . $estado_anterior_os . ' </b></td>
                                    </tr>
                                    <tr>
                                        <td>Data da Tramitação:</td>
                                        <td>' . $data_tramitacao . '</td>
                                    </tr>
                                    <tr>
                                        <td>Quem tramitou:</td>
                                        <td> ' . $nome_tramitador . ' - (' . $email_tramitador . ')</td>
                                    </tr>
                                    <tr>
                                        <td>Ação realizada:</td>
                                        <td> <b>' . $acao_realizada . '</b></td>
                                    </tr>                        
                                    <tr>
                                        <td>&nbsp;</td>
                                        <td>&nbsp;</td>
                                    </tr>
                                    <tr>
                                        <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                            <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                        </td>
                                    </tr>
                                </tbody>
                                <tfoot>

                                </tfoot>
                            </table>
                        </body>
                    </html>
                    ';

            $assunto = "SIMEC - ORDEM DE SERVICO - Nº {$osmid} - Tramitada - Obra: " . $obrid;
            $conteudo = $html;
            $destinatario = array($email_responsavel);

            $dados = array(
                'usucpf' => $_SESSION['usucpf'],
                'emlconteudo' => $conteudo,
                'emlassunto' => $assunto,
                'temid' => 5,
                'emlregistroatividade' => true,
                'obrid' => $obrid
            );

            $email = new Email();
            $email->popularDadosObjeto($dados);
            $email->salvar($destinatario);
            $email->enviar();
        }
        return true;
    }

    /**
     * Registra e-mail para envio caso houver informação de mudança de endereço
     * @param integer $obrid
     * @param integer $sueid
     * @return void(0)
     */
    public function enviaEmailAlteracaoEnderecoFNDE($obrid, $sueid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);
        $supEmp = new SupervisaoEmpresa($sueid);
        $oS = new Supervisao_Os($supEmp->sosid);
        $OsGrupoEmpresa = new Supervisao_Grupo_Empresa();
        $endSup = new Endereco($supEmp->endid);
        $endObra = new Endereco($obra->endid);
        $vistoriador = new Entidade($supEmp->entidvistoriador);
        $situacaoObra = new SituacaoObra($supEmp->sobid);
        $homolog = $supEmp->carregaDadosHomologacao($sueid);
        $municipioObra = new Municipio($endObra->muncod);
        $municipioSup = new Municipio($endSup->muncod);
        $contato = new Contato();
        $sql = $contato->listaContatoFNDE(array('estuf' => $endObra->estuf));

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                    <html>
                        <head>
                            <title></title>
                        </head>
                        <body>
                            <table style="width: 100%;">
                                <thead>
                                    <tr>
                                        <td style="text-align: center;">
                                            <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                            <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                            FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                            DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                            COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                            SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: right; padding: 40px 0 0 0;">
                                            ' . $data . '
                                        </td>
                                    </tr>
                                </thead>
                                <tbody>
                                    <tr>
                                        <td style="padding:20px 0 20px 0;">
                                          Assunto: <b>MUDANÇA DE ENDEREÇO POR SUPERVISÃO (' . $obrid . ') ' . $obra->obrnome . '</b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="line-height: 15px; text-align:justify">
                                            <table border="0" style="width:95%" cellpadding="2" cellspacing="2" align="center">
                                                <tr>
                                                    <td>
                                                        <table border="0" width="95%" cellpadding="2" cellspacing="2" align="center">
                                                            <tr>
                                                                <td style="background-color:#DCDCDC;color:#000;font-size:8pt;" colspan="2" align="center"><strong>Ordem de serviços</strong></td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Nº</strong></td>
                                                                <td width="70%">' . $oS->sosnum . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Empresa</strong></td>
                                                                <td width="70%">' . $OsGrupoEmpresa->pegaNomeEmpresaPorSosid($supEmp->sosid) . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Período de realização</strong></td>
                                                                <td width="70%">' . $oS->sosdtinicio . ' até ' . $oS->sosdttermino . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Emergencial</strong></td>
                                                                <td width="70%">' . ($oS->sosemergencial == 't' ? 'Sim' : 'Não') . '</td>
                                                            </tr>
                                                        </table>
                                                        
                                                        <table border="0" style="width:95%" cellpadding="2" cellspacing="2" align="center">
                                                            <tr>
                                                                <td style="background-color:#DCDCDC;color:#000;font-size:8pt;" colspan="2" align="center"><strong>Obra</strong></td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>ID</strong></td>
                                                                <td width="70%">' . $obrid . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Nome</strong></td>
                                                                <td width="70%">' . $obra->obrnome . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>UF/Município</strong></td>
                                                                <td width="70%">' . $endObra->estuf . '/' . $endObra->endbai . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Endereço</strong></td>
                                                                <td width="70%">' . $endObra->endlog . '</td>
                                                            </tr>
                                                        </table>
                                                        
                                                        <table border="0" style="width:95%" cellpadding="2" cellspacing="2" align="center">
                                                            <tr>
                                                                <td style="background-color:#DCDCDC;color:#000;font-size:8pt;" colspan="2" align="center"><strong>Supervisão</strong></td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Data de realização da supervisão</strong></td>
                                                                <td width="70%">' . $supEmp->suedtsupervisao . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Nome do responsável</strong></td>
                                                                <td width="70%">' . $vistoriador->entnome . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Cargo</strong></td>
                                                                <td width="70%">' . $supEmp->suecargovistoriador . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Situação da obra</strong></td>
                                                                <td width="70%">' . $situacaoObra->sobdsc . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Unidade em funcionamento</strong></td>
                                                                <td width="70%">' . $supEmp->suefuncionamento . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Homologado por</strong></td>
                                                                <td width="70%">' . $homolog['usunome'] . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Data de homologação</strong></td>
                                                                <td width="70%">' . $homolog['htddata'] . '</td>
                                                            </tr>
                                                        </table>
                                                        
                                                        <table border="0" style="width:95%" cellpadding="2" cellspacing="2" align="center">
                                                            <tr>
                                                                <td style="background-color:#DCDCDC;color:#000;font-size:8pt;" colspan="2" align="center"><strong>Local da obra</strong></td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>No cadastro da obra o endereço está correto?</strong></td>
                                                                <td width="70%">' . ($supEmp->sueendcorreto == 'n' ? 'Não' : 'Sim') . '</td>
                                                            </tr>
                                                        </table>
                                                        
                                                        <table border="0" style="width:95%" cellpadding="2" cellspacing="2" align="center">
                                                            <tr>
                                                                <td style="background-color:#DCDCDC;color:#000;font-size:8pt;" colspan="2" align="center"><strong>Local da obra - Cadastro da Obra</strong></td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>CEP</strong></td>
                                                                <td width="70%">' . $endObra->endcep . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Logradouro</strong></td>
                                                                <td width="70%">' . $endObra->endlog . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Número</strong></td>
                                                                <td width="70%">' . $endObra->endnum . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Complemento</strong></td>
                                                                <td width="70%">' . $endObra->endcom . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Bairro</strong></td>
                                                                <td width="70%">' . $endObra->endbai . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Município/UF</strong></td>
                                                                <td width="70%">' . $municipioObra->mundescricao . '/' . $municipioObra->estuf . '</td>
                                                            </tr>
                                                        </table>
                                                        
                                                        <table border="0" style="width:95%" cellpadding="2" cellspacing="2" align="center">
                                                            <tr>
                                                                <td style="background-color:#DCDCDC;color:#000;font-size:8pt;" colspan="2" align="center"><strong>Local da obra - Supervisão</strong></td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>CEP</strong></td>
                                                                <td width="70%">' . $endSup->endcep . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Logradouro</strong></td>
                                                                <td width="70%">' . $endSup->endlog . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Número</strong></td>
                                                                <td width="70%">' . $endSup->endnum . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Complemento</strong></td>
                                                                <td width="70%">' . $endSup->endcom . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Bairro</strong></td>
                                                                <td width="70%">' . $endSup->endbai . '</td>
                                                            </tr>
                                                            <tr>
                                                                <td width="30%" align="right"><strong>Município/UF</strong></td>
                                                                <td width="70%">' . $municipioSup->mundescricao . '/' . $municipioSup->estuf . '</td>
                                                            </tr>
                                                        </table>
                                                    </td>
                                                </tr>
                                            </table>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="padding: 10px 0 0 0;">
                                                Atenciosamente,
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: center; padding: 10px 0 0 0;">
                                                <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                                <br />
                                                <b>Fabrício Batista de Araújo<b>
                                                <br />
                                                Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                <br />
                                                CGIMP/DIRPE/FNDE/MEC
                                        </td>
                                    </tr>
                                </tbody>
                                <tfoot>

                                </tfoot>
                            </table>
                        </body>
                    </html>',
            'emlassunto' => 'SIMEC - OBRA ID ' . $obra->obrid . ' (' . $obra->obrnome . ') - MUDANÇA DE ENDEREÇO POR SUPERVISÃO',
            'temid' => 6,
            'emlregistroatividade' => true,
            'obrid' => $obrid
        );

        //$dadosRemetentes = array('lucass.web@gmail.com');
        $dadosRemetentes = array();
        $rsContatos = $db->carregar($sql);

        $rsContatos = (empty($rsContatos)) ? array() : $rsContatos;

        if (count($rsContatos)) {
            foreach ($rsContatos as $c) {
                if (!empty($c['usuemail']))
                    array_push($dadosRemetentes, $c['usuemail']);
            }

            if (count($dadosRemetentes)) {
                $email = new Email();
                $email->popularDadosObjeto($dados);
                $email->salvar($dadosRemetentes);
                $email->enviar();
            }
        }
    }

    public function enviaEmailPainelFNDE_RelatorioValidacoes(array $anexos) {
        require_once APPRAIZ . 'includes/classes/dateTime.inc';
        global $db;

        foreach ($anexos as $arqid) {
            $row = $db->pegaLinha("SELECT arqid, arqnome FROM public.arquivo WHERE arqid = {$arqid}");
            $htmlUls .= '<li><a href="http://' . $_SERVER['HTTP_HOST'] . '/obras2/obras2.php?modulo=principal/download&acao=A&arqid=' . $row['arqid'] . '&secao=relatorio">' . $row['arqnome'] . '</a></li>';
        }
        $htmlUls .= '</ul>';

        $date = new Data();
        $data = $date->formataData($date->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                    <html>
                        <head>
                            <title></title>
                        </head>
                        <body>
                            <table style="width: 100%;">
                                <thead>
                                    <tr>
                                        <td style="text-align: center;">
                                            <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                            <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                            FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                            DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                            COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                            SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: right; padding: 40px 0 0 0;">
                                            ' . $data . '
                                        </td>
                                    </tr>
                                </thead>
                                <tbody>
                                    <tr>
                                        <td style="padding:20px 0 20px 0;">
                                          Assunto: <b>SIMEC - Relatório Mensal - Painel FNDE / Validações</b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="line-height: 15px; text-align:justify">
                                            Relatório para download em: ' . date('d/m/Y : H:i:s') . '
                                            ' . $htmlUls . '
                                        </td>
                                    </tr>
                                </tbody>
                                <tfoot>
                                </tfoot>
                            </table>
                        </body>
                    </html>',
            'emlassunto' => 'SIMEC - Relatório Mensal - Painel FNDE / Relatório de Validações',
            'temid' => 7,
            'emlregistroatividade' => 'false',
            'obrid' => null
        );

        $dadosDestinatario = array('fabio.cardoso@fnde.gov.br', 'fabricio.araujo@fnde.gov.br');

        $this->popularDadosObjeto($dados);
        $this->salvar($dadosDestinatario);
        $this->enviar();
    }

    /**
     * Envia um e-mail para os responsáveis da obra informando que a mesma foi enviada para paralisação
     *
     * @param $obrid
     */
    public function enviaEmalParalisacaoObra($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);
        $empreendimento = new Empreendimento($obrid->empid);
        $endereco = new Endereco(($obra->endid ? $obra->endid : $empreendimento->endid));
        $municipio = new Municipio($endereco->muncod);
        $contato = new Contato();
        $sql = $contato->listaContatoFNDE(array('estuf' => $endereco->estuf));

        $contatos = $db->carregar($sql);
        $contatos = (empty($contatos) ? array() : $contatos);


        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $data2 = new Data();
        $data2 = $data2->formataData($data2->dataAtual(), 'DD de mesTextual de YYYY');

        // Envia um e-mail para o FNDE
        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . $data . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Obra paralisada (' . $obrid . ') ' . $obra->obrnome . '</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a),</p>
                                                <p>A obra (' . $obrid . ') ' . $obra->obrnome . ' ' . $municipio->mundescricao . '/' . $municipio->estuf . ' entrou na situação paralisada na data ' . $data2 . '.</p>
                                                <p>Entrar em contato urgente com o município.</p>
                                                <p></p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="padding: 10px 0 0 0;">
                                                    Atenciosamente,
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: center; padding: 10px 0 0 0;">
                                                    <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                                    <br />
                                                    <b>Fabrício Batista de Araújo<b>
                                                    <br />
                                                    Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                    <br />
                                                    CGIMP/DIRPE/FNDE/MEC
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>

                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Aviso de obra paralisada (' . $obrid . ') ' . $obra->obrnome,
            'temid' => 8,
            'emlregistroatividade' => 'false',
            'obrid' => $obrid
        );

        $dadosRemetentes = array();
        foreach ($contatos as $c) {
            if ($c['usuemail']) {
                $dadosRemetentes[] = $c['usuemail'];
            }
        }

        $dadosRemetentes[] = 'fabricio.araujo@fnde.gov.br';
        $dadosRemetentes[] = 'pedro.wanderley@fnde.gov.br';
        $dadosRemetentes[] = 'atendimento.monitora@fnde.gov.br';

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();

        // Envia um e-mail para o prefeito
        $dados['emlregistroatividade'] = true;
        $dados['emlconteudo'] = '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . $data . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Obra paralisada (' . $obrid . ') ' . $obra->obrnome . '</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a) Prefeito(a),</p>
                                                <p>Constatamos que a obra (' . $obrid . ') ' . $obra->obrnome . ' ' . $municipio->mundescricao . '/' . $municipio->estuf . ' , se encontra paralisada, através da vistoria registrada nessa data, razão pela qual solicitamos a confirmação dessa informação e esclarecimentos adicionais sobre o motivo da paralisação, encaminhando e-mail para: atendimento.monitora@fnde.gov.br.</p>
                                                <p>Lembramos que esta situação prejudica duplamente o município: primeiro, porque atrasa a entrega de uma obra importante para a comunidade; e segundo, porque impossibilita a transferência voluntária de recursos do MEC/FNDE (obras, mobiliário e outras ações submetidas).</p>
                                                <p>Desta forma, colocamos nossa equipe técnica à disposição, para que a retomada da execução da obra ocorra o mais rápido possível, já informando que entraremos em contato com V.Sa., ou o fiscal da obra, em até cinco dias úteis do envio desta mensagem.</p>
                                                <p></p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="padding: 10px 0 0 0;">
                                                    Atenciosamente,
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: center; padding: 10px 0 0 0;">
                                                    <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                                    <br />
                                                    <b>Fabrício Batista de Araújo<b>
                                                    <br />
                                                    Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                    <br />
                                                    CGIMP/DIRPE/FNDE/MEC
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>

                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ';

        $dadosRemetentes = array();
        $entPrefeito = $this->pegaEntidadePar($obrid, 2);
        $entPrefeitura = $this->pegaEntidadePar($obrid, 1);

        if ($entPrefeito->getEntEmail())
            $dadosRemetentes[] = $entPrefeito->getEntEmail();
        if ($entPrefeitura->getEntEmail())
            $dadosRemetentes[] = $entPrefeitura->getEntEmail();

        if (empty($dadosRemetentes))
            return;

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }

    /**
     * Envia um e-mail quando o contrato da obra e editado
     *
     * @param $obrid
     */
    public function enviaEmalEdicaoContratoObra($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);
        $empreendimento = new Empreendimento($obrid->empid);
        $endereco = new Endereco(($obra->endid ? $obra->endid : $empreendimento->endid));
        $municipio = new Municipio($endereco->muncod);
        $contato = new Contato();
        $sql = $contato->listaContatoFNDE(array('estuf' => $endereco->estuf));

        $contatos = $db->carregar($sql);
        $contatos = (empty($contatos) ? array() : $contatos);

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $data2 = new Data();
        $data2 = $data2->formataData($data2->dataAtual(), 'DD de mesTextual de YYYY');

        // Envia um e-mail para o FNDE
        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . $data . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Edição de contrato da obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a),</p>
                                                <p>O contrato da obra (' . $obrid . ') ' . $obra->obrnome . ' - ' . $municipio->mundescricao . '/' . $municipio->estuf . ' foi editado na data ' . $data2 . ' pelo (' . $_SESSION['usucpf'] . ') ' . $_SESSION['usunome'] . '.</p>
                                                <p></p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Aviso de edição do contrato da obra (' . $obrid . ') ' . $obra->obrnome,
            'temid' => 14,
            'emlregistroatividade' => 'false',
            'obrid' => $obrid
        );

        $dadosRemetentes = array();
        foreach ($contatos as $c) {
            if ($c['usuemail']) {
                $dadosRemetentes[] = $c['usuemail'];
            }
        }

        $dadosRemetentes[] = 'fabio.cardoso@fnde.gov.br';
        $dadosRemetentes[] = 'fabricio.araujo@fnde.gov.br';
        $dadosRemetentes[] = 'pedro.wanderley@fnde.gov.br';

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }

    /**
     * Envia um e-mail quando o contrato da obra e editado
     *
     * @param $obrid
     */
    public function enviaEmailEdicaoCronogramaObra($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);
        $empreendimento = new Empreendimento($obrid->empid);
        $endereco = new Endereco(($obra->endid ? $obra->endid : $empreendimento->endid));
        $municipio = new Municipio($endereco->muncod);
        $contato = new Contato();
        $sql = $contato->listaContatoFNDE(array('estuf' => $endereco->estuf));

        $contatos = $db->carregar($sql);
        $contatos = (empty($contatos) ? array() : $contatos);

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $data2 = new Data();
        $data2 = $data2->formataData($data2->dataAtual(), 'DD de mesTextual de YYYY');

        // Envia um e-mail para o FNDE
        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . $data . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Edição de cronograma da obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a),</p>
                                                <p>O cronograma da obra (' . $obrid . ') ' . $obra->obrnome . ' - ' . $municipio->mundescricao . '/' . $municipio->estuf . ' foi editado na data ' . $data2 . ' pelo (' . $_SESSION['usucpf'] . ') ' . $_SESSION['usunome'] . '.</p>
                                                <p></p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Aviso de edição do cronograma da obra (' . $obrid . ') ' . $obra->obrnome,
            'temid' => 14,
            'emlregistroatividade' => 'false',
            'obrid' => $obrid
        );

        $dadosRemetentes = array();
        foreach ($contatos as $c) {
            if ($c['usuemail']) {
                $dadosRemetentes[] = $c['usuemail'];
            }
        }

//        $dadosRemetentes[] = 'fabio.cardoso@fnde.gov.br';
//        $dadosRemetentes[] = 'fabricio.araujo@fnde.gov.br';
//        $dadosRemetentes[] = 'pedro.wanderley@fnde.gov.br';

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }

    public function enviaEmalOsAceita($sosid)
    {
        $osObra = new Supervisao_Os_Obra();
        $os = new Supervisao_Os($sosid);
        $sosGrupo = new Supervisao_Grupo_Empresa();
        $arrEmpid = $osObra->listaEmpidPorOs($sosid);
        $contatosEmp = $sosGrupo->getContatosPorSosid($sosid);

        $telefone = '';
        $empresa = '';
        $contatos = array();
        if (!empty($contatosEmp)) {
            foreach ($contatosEmp as $contato) {
                $contatos[] = $contato['usunome'];
                $telefone = $contato['telefone'];
                $empresa = $contato['entnome'];
            }
        }
        $contatos = implode(', ', $contatos);

        foreach ($arrEmpid as $empid) {
            $emp = new Empreendimento($empid);
            $obra = new Obras();
            $dadosObra = $obra->pegaObraPorEmpid($empid);
            $obra->popularDadosObjeto($dadosObra);

            $dadosRemetentes = array();
            $nomeDestinatario = '';

            if ($emp->empesfera == 'M') {
                $entPrefeito = $this->pegaEntidadePar($obra->obrid, 2);
                $entPrefeitura = $this->pegaEntidadePar($obra->obrid, 1);

                $nomeDestinatario = 'Secretário(a) ' . $entPrefeito->getEntnome();

                if ($entPrefeito->getEntEmail())
                    $dadosRemetentes[] = $entPrefeito->getEntEmail();
                if ($entPrefeitura->getEntEmail())
                    $dadosRemetentes[] = $entPrefeitura->getEntEmail();

                if (empty($dadosRemetentes))
                    continue;
            } else {
                $entSecretario = $this->pegaEntidadePar($obra->obrid, 25);
                $entSecretaria = $this->pegaEntidadePar($obra->obrid, 6);

                $nomeDestinatario = 'Prefeiro(a) ' . $entSecretario->getEntnome();

                if ($entSecretario->getEntEmail())
                    $dadosRemetentes[] = $entSecretario->getEntEmail();
                if ($entSecretaria->getEntEmail())
                    $dadosRemetentes[] = $entSecretaria->getEntEmail();

                if (empty($dadosRemetentes))
                    continue;
            }

            $data = new Data();
            $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

            $dados = array(
                'usucpf' => $_SESSION['usucpf'],
                'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td>
                                                <p style="float:left; text-align: left; padding: 40px 0 0 0;">Comunicado Nº __RGAID__ - CGIMP/DIGAP/FNDE</p>
                                                <p style="float-right; text-align: right; padding: 40px 0 0 0;">' . $data . '</p>
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>

                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Comunicado de supervisão a ser realizada pela empresa contratada na obra '.$obra->obrid.' - '.$obra->obrnome.'.</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Prezado '.$nomeDestinatario.',</p>
                                                <p>Informamos que está prevista para até o dia '.formata_data($os->sosdttermino).' a realização de Supervisão Técnica pela empresa representante do FNDE em seu município, para a obra '.$obra->obrid.' - '.$obra->obrnome.'.</p>
                                                <p>Por gentileza, mantenha o fiscal da obra e a equipe técnica da empresa construtora à disposição para acompanhar o supervisor à obra, bem como disponibilize todos os documentos e projetos no canteiro de obras.</p>
                                                <p>Mantenha atualizada as informações e os documentos (inclusive localização - latitude e longitude) no sistema Simec-Obras 2.0 representando de forma completa a atual evolução da obra em execução.</p>
                                                <p>Ressaltamos que todos as dúvidas com relação a supervisão realizada podem ser dirimidas com o supervisor da empresa representante do FNDE. </p>
                                                <p>Caso queira entrar em contato com a empresa de supervisão - '.$empresa.' - ligar para:  '.$telefone.' - Resposável(is): '.$contatos.'. </p>
                                                <p></p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="padding: 10px 0 0 0;">
                                                    Atenciosamente,
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: center; padding: 10px 0 0 0;">
                                                    <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                                    <br />
                                                    <b>Fabrício Batista de Araújo<b>
                                                    <br />
                                                    Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                    <br />
                                                    CGIMP/DIRPE/FNDE/MEC
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>

                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
                'emlassunto' => 'Monitoramento de Obras',
                'temid' => 9,
                'emlregistroatividade' => 'true',
                'obrid' => $obra->obrid
            );
            $email = new Email();
            $email->popularDadosObjeto($dados);
            $email->salvar($dadosRemetentes);
            $email->enviar();
        }
    }

    /**
     * Envia um e-mail quando o percentual da supervisão tem 10% de diferença entre o percentual da obra
     */
    public function enviaEmailDefasagemSupervisão($obrid) {
        global $db;
        require_once APPRAIZ . "includes/classes/dateTime.inc";

        $obra = new Obras($obrid);
        $emp = new Empreendimento($obra->empid);

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dadosRemetentes = $this->pegaContatosPar($obra->empid, $obrid);

        $head = '';
        $nome = '';

        if ($emp->empesfera == 'M') {
            $entPrefeito = $this->pegaEntidadePar($obrid, 2);

            if (!$entPrefeito->getEntId()) {
                return false;
            }

            if (is_array($entPrefeito->enderecos)) {
                $enderecoPrefeitura = current($entPrefeito->enderecos);
            } else {
                $enderecoPrefeitura = $entPrefeito->enderecos;
            }

            $nome = 'Prefeito(a)';
            $head = '
                A Sua Excelência o(a) Senhor(a)<br />
                ' . $entPrefeito->getEntnome() . '<br />
                Prefeito(a) do Município de ' . $enderecoPrefeitura['mundescricao'] . ' - ' . $enderecoPrefeitura['estuf'] . '<br />
                ' . $enderecoPrefeitura['endlog'] . ', nº ' . $enderecoPrefeitura['endnum'] . ' - ' . $enderecoPrefeitura['endbai'] . '<br />
                ' . $enderecoPrefeitura['mundescricao'] . ' - ' . $enderecoPrefeitura['estuf'] . ' - ' . $enderecoPrefeitura['endcep'] . '<br />
            ';
        } elseif ($emp->empesfera == 'E') {
            $entSecretario = $this->pegaEntidadePar($obrid, 6);

            if (!$entSecretario->getEntId())
                return false;

            $enderecoSecretaria = current($entSecretario->enderecos);
            $nome = 'Secretário(a)';
            $head = '
                A Sua Excelência o(a) Senhor(a)<br />
                ' . $entSecretario->getEntnome() . '<br />
                Secretário de Educação do Estado ' . $enderecoSecretaria['estuf'] . '<br />
                ' . $enderecoSecretaria['endlog'] . ', nº ' . $enderecoSecretaria['endnum'] . ' - ' . $enderecoSecretaria['endbai'] . '<br />
                ' . $enderecoSecretaria['mundescricao'] . ' - ' . $enderecoSecretaria['estuf'] . ' - ' . $enderecoSecretaria['endcep'] . '<br />
            ';
        }

        $valorp = percentualSupEmpresa($emp->empid);

        $dados = array(
            'usucpf' => '',
            'emlconteudo' => '
            <html>
                <head>
                    <title></title>
                </head>
                <body>
                    <table style="width: 100%;">
                        <thead>
                            <tr>
                                <td style="text-align: center;">
                                    <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                    <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                    FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                    DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                    COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                    SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                </td>
                            </tr>
                            <tr>
                                <td style="text-align: right; padding: 40px 0 0 0;">
                                    ' . $data . '
                                </td>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td style="line-height: 15px;">
                                    ' . $head . '
                                </td>
                            </tr>
                            <tr>
                                <td style="padding:20px 0 20px 0;">
                                  Assunto: <b>Percentual de execução da obra (' . $obra->obrid . ') ' . $obra->obrnome . '</br>
                                </td>
                            </tr>
                            <tr>
                                <td style="line-height: 15px; text-align:justify">
                                     <p>Senhor(a) ' . $nome . ',</p>
                                            <p>1. Em decorrência do monitoramento realizado por meio do Sistema Integrado de Monitoramento,
                                            Execução e Controle do Ministério da Educação (Simec) e de supervisão realizada por empresa contratada
                                            pelo FNDE, verificamos que existe uma grande divergência entre o percentual de execução da obra
                                            informado em vistoria pelo fiscal do estado responsável pela obra (' . number_format($valorp['percental']['unidade'], 2, ',', '.') . '%) e o da empresa de supervisão
                                            (' . number_format( $valorp['percental']['empresa'], 2, ',', '.') . '%).</p>

                                            <p>2. Desta forma, solicitamos o fiscal reveja os percentuais informados dos serviços executados,
                                            corrigindo-os se for o caso, ou que insira nova vistoria, com fotos atualizadas da obra, que comprovem o
                                            percentual de execução informado.</p>

                                            <p>3. Solicitamos, no prazo máximo de 15 dias, que o cumprimento das providências requeridas seja
                                            informado a esta Autarquia, por meio do Sistema Integrado de Monitoramento, Execução e Controle do
                                            Ministério da Educação (Simec). O não atendimento das providências solicitadas causará a suspensão do
                                            repasse de recursos dessa obra até a sua resolução.</p>
                                </td>
                            </tr>
                            <tr>
                                <td style="padding: 10px 0 0 0;">
                                        Atenciosamente,
                                </td>
                            </tr>
                            <tr>
                                <td style="text-align: center; padding: 10px 0 0 0;">
                                        <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                        <br />
                                        <b>Fabrício Batista de Araújo<b>
                                        <br />
                                        Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                        <br />
                                        CGIMP/DIRPE/FNDE/MEC
                                </td>
                            </tr>
                        </tbody>
                        <tfoot>

                        </tfoot>
                    </table>
                </body>
            </html>
            ',
            'emlassunto' => 'Percentual de execução da obra ' . $obra->obrnome,
            'temid' => 11,
            'emlregistroatividade' => true,
            'obrid' => $obra->obrid
        );

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }

    /**
     * Envia um e-mail quando a obra sai de contratação para execução
     *
     * Destinatários:   FISCAL UNIDADE
     *                  GESTOR UNIDADE
     *
     * @param $obrid
     * @return bool
     */
    public function enviaEmailContratacaoParaExecucao($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);
        $empreendimento = new Empreendimento($obrid->empid);
        $endereco = new Endereco(($obra->endid ? $obra->endid : $empreendimento->endid));
        $municipio = new Municipio($endereco->muncod);

        $contato = new ContatosObra();
        $contatos = $contato->getContatos($obrid);
        $contatos = (empty($contatos) ? array() : $contatos);

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . $data . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Tramitação da obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a),</p>
                                                <p>Em virtude de a obra (' . $obrid . ') ' . $obra->obrnome . ' - ' . $municipio->mundescricao . '/' . $municipio->estuf . ' ter entrado em execução, solicitamos que insiram os documentos: ART/RRT de execução e ART/RRT de fiscalização. Lembramos que, sem esses documentos, as próximas parcelas não serão desembolsadas.</p>
                                                <p></p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Aviso de tramitação da obra (' . $obrid . ') ' . $obra->obrnome,
            'temid' => 16,
            'emlregistroatividade' => true,
            'obrid' => $obrid
        );

        $dadosRemetentes = array();
        foreach ($contatos as $c) {
            if ($c['usuemail']) {
                $dadosRemetentes[] = $c['usuemail'];
            }
        }

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }

    /**
     * Envia um e-mail quando a obra atinge 25% ou 30%
     *
     * Destinatários:   FISCAL UNIDADE
     *                  GESTOR UNIDADE
     *
     * @param $obrid
     * @return bool
     */
    public function enviaEmailPercentualExecucao1($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        if ($this->verificaEmailEnviado(17, $obrid, null))
            return;

        $obra = new Obras($obrid);
        $empreendimento = new Empreendimento($obrid->empid);
        $endereco = new Endereco(($obra->endid ? $obra->endid : $empreendimento->endid));
        $municipio = new Municipio($endereco->muncod);

        $contato = new ContatosObra();
        $contatos = $contato->getContatos($obrid);
        $contatos = (empty($contatos) ? array() : $contatos);

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . $data . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Execução da obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a),</p>
                                                <p>Para que não haja atraso na solicitação de desembolso da obra  (' . $obrid . ') ' . $obra->obrnome . ' - ' . $municipio->mundescricao . '/' . $municipio->estuf . ', solicitamos verificar se já estão inseridos no SIMEC, os documentos abaixo listados. Caso não estejam inseridos, pedimos providenciar o mais rápido possível.</p>
                                                    <ul>
                                                        <li>Aba licitação:  publicação do edital; e homologação da licitação;</li>
                                                        <li>Aba Contratação: Prazo ainda vigente do contrato com a empresa vencedora da licitação. Caso o contrato esteja expirado, insira termo aditivo;</li>
                                                        <li>Aba Cronograma:  Verificar se:
                                                            <ul>
                                                                <li>Aba Vistoria: ART/RRT de execução e ART/RRT de fiscalização; relatório técnico preenchido, fotografias em número e qualidade suficiente para que a análise possa constatar o percentual de execução informado;
                                                                <li>Aba Execução Orçamentária: Notas Fiscais; Boletins de medição; e Comprovantes de pagamentos;.
                                                            </ul>
                                                        </li>
                                                    </ul>
                                                <p>Obs: Todos os documentos devem estar devidamente datados e assinados.</p>
                                                <p></p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Aviso de execução da obra (' . $obrid . ') ' . $obra->obrnome,
            'temid' => 17,
            'emlregistroatividade' => true,
            'obrid' => $obrid
        );

        $dadosRemetentes = array();
        foreach ($contatos as $c) {
            if ($c['usuemail']) {
                $dadosRemetentes[] = $c['usuemail'];
            }
        }

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }

    /**
     * Envia um e-mail quando a obra atinge 25% ou 30%
     *
     * Destinatários:   FISCAL UNIDADE
     *                  GESTOR UNIDADE
     *
     * @param $obrid
     * @return bool
     */
    public function enviaEmailPercentualExecucao2($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);

        if ($this->verificaEmailEnviado(18, $obrid, null))
            return;

        $empreendimento = new Empreendimento($obrid->empid);
        $endereco = new Endereco(($obra->endid ? $obra->endid : $empreendimento->endid));
        $municipio = new Municipio($endereco->muncod);

        $contato = new ContatosObra();
        $contatos = $contato->getContatos($obrid);
        $contatos = (empty($contatos) ? array() : $contatos);

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . $data . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Execução da obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a),</p>
                                                <p>Para que não haja atraso na solicitação de desembolso da  obra  (' . $obrid . ') ' . $obra->obrnome . ' - ' . $municipio->mundescricao . '/' . $municipio->estuf . ',  solicitamos verificar se já estão inseridos no SIMEC, os documentos abaixo listados. Caso não estejam inseridos, pedimos providenciar o mais rápido possível.</p>
                                                    <ul>
                                                        <li>Aba Contratação: Prazo ainda vigente do contrato com a empresa vencedora da licitação. Caso o contrato esteja expirado, insira termo aditivo;</li>
                                                        <li>Aba Vistoria: ART/RRT de execução e ART/RRT de fiscalização; relatório técnico preenchido, fotografias em número e qualidade suficiente para que a análise possa constatar o percentual de execução informado;</li>
                                                        <li>Aba Execução Orçamentária: Notas Fiscais; Boletins de medição; e Comprovantes de pagamentos;.</li>
                                                    </ul>
                                                <p>Obs: Todos os documentos devem estar devidamente datados e assinados.</p>
                                                <p></p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Aviso de tramitação da obra (' . $obrid . ') ' . $obra->obrnome,
            'temid' => 18,
            'emlregistroatividade' => true,
            'obrid' => $obrid
        );

        $dadosRemetentes = array();
        foreach ($contatos as $c) {
            if ($c['usuemail']) {
                $dadosRemetentes[] = $c['usuemail'];
            }
        }

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }

    /**
     * @param $coid
     * @return bool
     */
    public function enviaEmailCumprimentoObjeto($coid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";

        $cumprimentoObjeto = new CumprimentoObjeto($coid);
        $dadosTramite = $cumprimentoObjeto->dadosTramite();
        $obra = new Obras($cumprimentoObjeto->obrid);
        
        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                <html>
                <head>
                    <title></title>
                </head>
                <body>
                    <table style="width: 100%;">
                        <thead>
                            <tr>
                                <td style="text-align: center; bgcolor: #ccc;" colspan="2" >
                                    <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                    <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                    FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                    DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                    COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                    SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                </td>
                            </tr>
                            <tr>
                                <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                    ' . $data . '
                                </td>
                            </tr>
                            <tr>
                                <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                    &nbsp;
                                </td>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                    <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA. FAVOR NÃO RESPONDER. </b>
                                </td>
                            </tr>
                            <tr>
                                <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                    &nbsp;
                                </td>
                            </tr>
                            <tr>
                                <td style="padding:20px 0 20px 0;" colspan="2">
                                  Assunto: SIMEC - Cumprimento do Objeto - Obra ' . $obra->obrid . ' - '.$obra->obrnome.'
                                </td>
                            </tr>
                            <tr>
                                <td style="line-height: 15px;" colspan="2">
                                    <b> TRAMITAÇÃO DE CUMPRIMENTO DO OBJETO </b>
                                </td>
                            </tr>
                            <tr>
                                <td>Estado Anterior:</td>
                                <td><b> ' . $dadosTramite['estado_anterior'] . ' </b></td>
                            </tr>
                            <tr>
                                <td>Estado Atual:</td>
                                <td><b> ' . $dadosTramite['estado_atual'] . ' </b></td>
                            </tr>
                            <tr>
                                <td>Data da Tramitação:</td>
                                <td>' . $dadosTramite['htddata'] . '</td>
                            </tr>
                            <tr>
                                <td>Quem tramitou:</td>
                                <td> ' . $dadosTramite['usunome'] . ' - (' . $dadosTramite['usuemail'] . ')</td>
                            </tr>
                            <tr>
                                <td>Ação realizada:</td>
                                <td> <b>' . $dadosTramite['acao'] . '</b></td>
                            </tr>
                            <tr>
                                <td>Comentário da Tramitação:</td>
                                <td> ' . $dadosTramite['cmddsc'] . '</td>
                            </tr>
                            <tr>
                                <td>&nbsp;</td>
                                <td>&nbsp;</td>
                            </tr>
                            <tr>
                                <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                    <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                </td>
                            </tr>
                        </tbody>
                        <tfoot>
                            <tr>
                                <td>&nbsp;</td>
                                <td>&nbsp;</td>
                            </tr>
                        </tfoot>
                    </table>
                </body>
            </html>
            ',
            'emlassunto' => 'Aviso de Tramitação Cumprimento do Objeto. Obra ' . $obra->obrid . ' - '.strtoupper($obra->obrnome),
            'temid' => 47,
            'emlregistroatividade' => true,
            'obrid' => $obra->obrid
        );

        $dadosRemetentes = array();

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }
    
    
     /**
     * Envia um e-mail quando a obra atinge 25% ou 30%
     *
     * Destinatários:   FISCAL UNIDADE
     *                  GESTOR UNIDADE
     *
     * @param $obrid
     * @return bool
     */
    public function paralisacaoPorContratoRescindido ($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);

       

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        $conteudo_interno = "
            
            <p>
            Prezados,<br><br>
            A obra {$obrid} - {$obra->obrnome} entrou em paralisação por contrato rescindido na data " . date("d/m/Y") . "
            </p>";
            
        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . $data . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Paralisação da obra (' . $obrid . ') ' . $obra->obrnome.' por contrato rescindido</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a),</p>
                                                <p>
                                                    ' . $conteudo_interno . '

                                                </p>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Paralisação da obra (' . $obrid . ') ' . $obra->obrnome.' por contrato rescindido',
            'temid' => 24,
            'emlregistroatividade' => 'false',
            'obrid' => $obrid
        );
        
        
       $dadosDestinatario = array('fabricio.araujo@fnde.gov.br','atendimento.monitora@fnde.gov.br');
      //$dadosDestinatario = array('marcus.rocha90@gmail.com');

        $email = new Email();
        //ver($dados,d);
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosDestinatario);
        $email->enviar();
    }

    /**
     * Envia um e-mail ao FNDE quando uma supervisão é marcada com falha grave
     *
     * Destinatários:   FNDE
     *
     * @param $obrid
     * @return bool
     */
    public function enviaEmailSupervisaoObraComProblema ($obrid, $sosid, $sueid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);
        $endObra = new Endereco($obra->endid);
        $os = new Supervisao_Os($sosid);
        $supervisao = new SupervisaoEmpresa($sueid);
        $contato = new Contato();

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                            <head>
                                <title></title>
                            </head>
                            <body>
                                <table style="width: 100%;">
                                    <thead>
                                        <tr>
                                            <td style="text-align: center;">
                                                <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="text-align: right; padding: 40px 0 0 0;">
                                                ' . $data . '
                                            </td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td style="padding:20px 0 20px 0;">
                                              Assunto: <b>Alerta de problema grave na obra (' . $obrid . ') ' . $obra->obrnome.'.</b>
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:justify">
                                                <p>Senhor(a),</p>
                                                <p>
                                                    Através da supervisão realizada pela OS de nº '.$os->sosnum.' a obra (' . $obrid . ') ' . $obra->obrnome.' foi sinalizada por conter um problema grave.
                                                </p>
                                                <br />
                                                <p>
                                                    <b>Observaçao técnica:</b><br />'.$supervisao->sueobsproblema.'
                                                </p>
                                                <br /><br />
                                            </td>
                                        </tr>
                                        <tr>
                                            <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                            </td>
                                        </tr>
                                    </tbody>
                                    <tfoot>
                                        <tr>
                                            <td>&nbsp;</td>
                                            <td>&nbsp;</td>
                                        </tr>
                                    </tfoot>
                                </table>
                            </body>
                        </html>
                                    ',
            'emlassunto' => 'Alerta de problema grave na obra (' . $obrid . ') ' . $obra->obrnome,
            'temid' => 34,
            'emlregistroatividade' => 'false',
            'obrid' => $obrid
        );

        $dadosRemetentes = array();
        $sql = $contato->listaContatoFNDE(array('estuf' => $endObra->estuf));

        $contatos = $db->carregar($sql);
        $contatos = (empty($contatos) ? array() : $contatos);
        foreach ($contatos as $c) {
            if ($c['usuemail']) {
                $dadosRemetentes[] = $c['usuemail'];
            }
        }

        $dadosRemetentes[] = 'fabio.cardoso@fnde.gov.br';
        $dadosRemetentes[] = 'fabricio.araujo@fnde.gov.br';
        $dadosRemetentes[] = 'fernanda.angelis@fnde.gov.br';
        $dadosRemetentes[] = 'pedro.wanderley@fnde.gov.br';

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }

    public function getBrasao(){
        $tempocache = 86400; // 1 dia

        $key = md5('obras2_brasao_email');

        if(!IS_PRODUCAO){
            return base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif'));
        }

        try {
            //global $memcache_obj;
            /* conectando com o memcached server */
            //if (!$memcache_obj) $memcache_obj = memcache_connect($GLOBALS["memcachehost"], $GLOBALS["memcacheport"]);
            /* pegando informações do memcached server, key igual ao md5 do SQL */
        	if (function_exists('zend_shm_cache_fetch')) {$cache_result = zend_shm_cache_fetch($key);}
            //$cache_result = memcache_get($memcache_obj, $key);

            if ($cache_result) { /* se existir cache, carregar com o resultado do memcached server */
                $res = $cache_result;
            } else { /* senão executa o SQL e guarda o resultado no memcached server */
                $res = base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif'));
                /* Armazenando os dados memcached server na chave md5(SQL), 0 => sem compressão, tempo para expirar de 30 seconds */
                //memcache_set($memcache_obj, $key, $res, 0, $tempocache);
                if (function_exists('zend_shm_cache_store')) {
                	if(zend_shm_cache_store($key, $res, $tempocache) === false) echo '[ZEND CACHE FALHOU]';
                }
                
            }
        } catch (Exception $e){
            if($_SESSION['usucpf'] = ''){
                echo $e->getMessage(); exit;
            } else {
                return base64_encode(file_get_contents(APPRAIZ . '/www/' . 'imagens/brasao.gif'));
            }

        }
        return $res;
    }


    public function getAssinatura(){
        $tempocache = 86400; // 1 dia

        $key = md5('obras2_brasao_email');

        if(!IS_PRODUCAO){
            return base64_encode(file_get_contents(APPRAIZ . 'www/imagens/obras/assinatura-fabricio.png'));
        }

        try {
        	
            //global $memcache_obj;
            /* conectando com o memcached server */
            //if (!$memcache_obj) $memcache_obj = memcache_connect($GLOBALS["memcachehost"], $GLOBALS["memcacheport"]);
            /* pegando informações do memcached server, key igual ao md5 do SQL */
            //$cache_result = memcache_get($memcache_obj, $key);
        	if (function_exists('zend_shm_cache_fetch')) {$cache_result = zend_shm_cache_fetch($key);}

            if ($cache_result) { /* se existir cache, carregar com o resultado do memcached server */
                $res = $cache_result;
            } else { /* senão executa o SQL e guarda o resultado no memcached server */
                $res = base64_encode(file_get_contents(APPRAIZ . 'www/imagens/obras/assinatura-fabricio.png'));
                /* Armazenando os dados memcached server na chave md5(SQL), 0 => sem compressão, tempo para expirar de 30 seconds */
                //memcache_set($memcache_obj, $key, $res, 0, $tempocache);
                if (function_exists('zend_shm_cache_store')) {
                	if(zend_shm_cache_store($key, $res, $tempocache) === false) echo '[ZEND CACHE FALHOU]';
                }
                
            }
        } catch (Exception $e){
            if($_SESSION['usucpf'] = ''){
                echo $e->getMessage(); exit;
            } else {
                return base64_encode(file_get_contents(APPRAIZ . 'www/imagens/obras/assinatura-fabricio.png'));
            }

        }
        return $res;
    }

    /**
     * Email enviado sempre que uma vistoria de paralisação é inserida
     *
     * @param $obrid
     */
    public function enviaEmailVistoriaParalisacao($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);
        $empreendimento = new Empreendimento($obrid->empid);
        $endereco = new Endereco(($obra->endid ? $obra->endid : $empreendimento->endid));
        $municipio = new Municipio($endereco->muncod);

        $contato = new ContatosObra();
        $contatos = $contato->getContatos($obrid);
        $contatos = (empty($contatos) ? array() : $contatos);

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                                <head>
                                    <title></title>
                                </head>
                                <body>
                                    <table style="width: 100%;">
                                        <thead>
                                            <tr>
                                                <td style="text-align: center;">
                                                    <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                    <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                    FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                    DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                    COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                    SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="text-align: right; padding: 40px 0 0 0;">
                                                    <p style="float:left; text-align: left; padding: 40px 0 0 0;">Comunicado Nº __RGAID__ - CGIMP/DIGAP/FNDE</p>
                                                    <p style="float-right; text-align: right; padding: 40px 0 0 0;">' . $data . '</p>
                                                </td>
                                            </tr>
                                        </thead>
                                        <tbody>
                                            <tr>
                                                <td style="padding:20px 0 20px 0;">
                                                  Assunto: <b>Aviso de obra paralisada (' . $obrid . ') ' . $obra->obrnome . '</b>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="line-height: 15px; text-align:justify">
                                                    <p>Senhor(a) Gestor(a),</p>
                                                    <p></p>
                                                    <p>Considerando a situação em que se encontra a obra (' . $obra->obrid . ') ' . $obra->obrnome . ', orientamos que caso a empresa contratada tenha dado causa à paralisação da obra, ou em caso de abandono da obra por parte da empresa sem o cumprimento do objeto contratado, orientamos Vossa Senhoria a entrar em contato com o departamento jurídico do estado/município para que sejam aplicadas as sanções previstas em contrato, e na Lei nº 8.666/93, em relação a mora no cumprimento do objeto contratado, ou seu inadimplemento.</p>
                                                    <p>Orientamos, ainda, que o estado/município busque o quanto antes a responsabilização da empresa contratada, cobrando judicialmente caso necessário, o ressarcimento de prejuízos causados em razão do abandono, especialmente se houver perda dos serviços já executados.</p>
                                                    <p>Além disso, informamos que, até a retomada da execução da obra, constitui obrigação da entidade beneficiada a adoção das providências necessárias à preservação dos recursos federais já empregados, providenciando a vigilância e proteção das obras com o intuito de evitar seu abandono, depredação, e desgastes por intempéries.</p>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="padding: 10px 0 0 0;">
                                                        Atenciosamente,
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="text-align: center; padding: 10px 0 0 0;">
                                                        <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                                        <br />
                                                        <b>Fabrício Batista de Araújo<b>
                                                        <br />
                                                        Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                        <br />
                                                        CGIMP/DIRPE/FNDE/MEC
                                                </td>
                                            </tr>
                                        </tbody>
                                        <tfoot>

                                        </tfoot>
                                    </table>
                                </body>
                            </html>
                                    ',
            'emlassunto' => 'Aviso de obra paralisada (' . $obrid . ') ' . $obra->obrnome,
            'temid' => 8,
            'emlregistroatividade' => true,
            'obrid' => $obrid
        );

        $dadosRemetentes = array();
        foreach ($contatos as $c) {
            if ($c['usuemail']) {
                $dadosRemetentes[] = $c['usuemail'];
            }
        }

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }



    /**
     * Email enviado sempre que uma vistoria com percentual acima de 85% é inserida
     *
     * @param $obrid
     */
    public function enviaEmailFaseDeConclusao($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        require_once APPRAIZ . "includes/classes/modelo/obras2/Empreendimento.class.inc";
        global $db;

        $obra = new Obras($obrid);
        $empreendimento = new Empreendimento($obrid->empid);
        $endereco = new Endereco(($obra->endid ? $obra->endid : $empreendimento->endid));
        $municipio = new Municipio($endereco->muncod);

        $contato = new ContatosObra();
        $contatos = $contato->getContatos($obrid);
        $contatos = (empty($contatos) ? array() : $contatos);

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                                <head>
                                    <title></title>
                                </head>
                                <body>
                                    <table style="width: 100%;">
                                        <thead>
                                            <tr>
                                                <td style="text-align: center;">
                                                    <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                    <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                    FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                    DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                    COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                    SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="text-align: right; padding: 40px 0 0 0;">
                                                    <p style="float:left; text-align: left; padding: 40px 0 0 0;">Comunicado Nº __RGAID__ - CGIMP/DIGAP/FNDE</p>
                                                    <p style="float-right; text-align: right; padding: 40px 0 0 0;">' . $data . '</p>
                                                </td>
                                            </tr>
                                        </thead>
                                        <tbody>
                                            <tr>
                                                <td style="padding:20px 0 20px 0;">
                                                  Assunto: <b>Obra em fase de conclusão (' . $obrid . ') ' . $obra->obrnome . '</b>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="line-height: 15px; text-align:justify">
                                                    <p>Senhor(a) Gestor(a),</p>
                                                    <p></p>

                                                    <p>Considerando que a (' . $obra->obrid . ') ' . $obra->obrnome . ' se encontra de fase de conclusão, orientamos Vossa Senhoria que quando de seu recebimento provisório, cabe ao responsável por seu acompanhamento e fiscalização, a verificação do cumprimento do objeto contratado em conformidade com o projeto e de acordo com as normas da Associação Brasileira de Normas Técnicas ? ABNT.</p>
                                                    <p>Caso seja verificada a existência de vícios, defeitos ou incorreções em razão da execução ou da qualidade dos materiais utilizados, cabe à empresa contratada adotar as providências necessárias à superação dessas irregularidades, caso contrário, a Administração poderá rejeitar a obra no todo ou em parte, se executada em desacordo com o contrato, conforme previsão legal constante nos artigos 69, 70 e 76 da Lei nº 8.666, de 1993.</p>
                                                    <p>Nesse sentido, orientamos que esta entidade se abstenha de:</p>
                                                    <p> a) receber provisoriamente qualquer parcela de obra sob sua responsabilidade com pendências a serem solucionadas pela construtora; e</p>
                                                    <p> b) receber definitivamente qualquer objeto sem que exista: o "as built" da obra; o laudo de vistoria do corpo de bombeiros aprovando a obra; a comprovação das ligações definitivas de energia, água, telefone e gás; a carta "habite-se" emitida pela Prefeitura local e a certidão negativa de débitos previdenciários específica para o registro da obra junto ao Cartório de Registro de Imóveis;</p>
                                                    <p>Mesmo após o recebimento provisório ou definitivo da obra, a empresa contratada continua sendo responsável civilmente pela solidez e segurança do empreendimento pelo prazo de cinco anos, devendo apresentar a correção dos vícios que surgirem nesse período, nos termos do §2º do art. 73 da Lei nº 8.666/93 c/c art. 618 da Lei nº 10.406, de 2012.</p>
                                                    <p>Quando da conclusão da obra, a unidade gestora deverá garantir condições de funcionamento e habitabilidade da edificação com as ligações definitivas de energia elétrica e água potável. Caso haja atraso das concessionárias de serviços públicos em fornecer esses serviços, orientamos o gestor público a acionar as Agências Reguladoras responsáveis, alertando, ainda, o FNDE acerca das providências em andamento por meio do Simec ? Obras 2.0.</p>

                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="padding: 10px 0 0 0;">
                                                        Atenciosamente,
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="text-align: center; padding: 10px 0 0 0;">
                                                        <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                                        <br />
                                                        <b>Fabrício Batista de Araújo<b>
                                                        <br />
                                                        Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                        <br />
                                                        CGIMP/DIRPE/FNDE/MEC
                                                </td>
                                            </tr>
                                        </tbody>
                                        <tfoot>

                                        </tfoot>
                                    </table>
                                </body>
                            </html>
                                    ',
            'emlassunto' => 'Aviso de obra em fase de conclusão (' . $obrid . ') ' . $obra->obrnome,
            'temid' => 39,
            'emlregistroatividade' => true,
            'obrid' => $obrid
        );

        $dadosRemetentes = array();
        foreach ($contatos as $c) {
            if ($c['usuemail']) {
                $dadosRemetentes[] = $c['usuemail'];
            }
        }

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }

    /**
     * Email enviado sempre que uma supervisão gera restrições
     *
     * @param $obrid
     */
    public function enviaEmailRestricoesSupervisao($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);
        $empreendimento = new Empreendimento($obrid->empid);
        $endereco = new Endereco(($obra->endid ? $obra->endid : $empreendimento->endid));
        $municipio = new Municipio($endereco->muncod);

        $contato = new ContatosObra();
        $contatos = $contato->getContatos($obrid);
        $contatos = (empty($contatos) ? array() : $contatos);

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                                <head>
                                    <title></title>
                                </head>
                                <body>
                                    <table style="width: 100%;">
                                        <thead>
                                            <tr>
                                                <td style="text-align: center;">
                                                    <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                    <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                    FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                    DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                    COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                    SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="text-align: right; padding: 40px 0 0 0;">
                                                    <p style="float:left; text-align: left; padding: 40px 0 0 0;">Comunicado Nº __RGAID__ - CGIMP/DIGAP/FNDE</p>
                                                    <p style="float-right; text-align: right; padding: 40px 0 0 0;">' . $data . '</p>
                                                </td>
                                            </tr>
                                        </thead>
                                        <tbody>
                                            <tr>
                                                <td style="padding:20px 0 20px 0;">
                                                  Assunto: <b>Restrições na Obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="line-height: 15px; text-align:justify">
                                                    <p>Senhor(a) Gestor(a),</p>
                                                    <p></p>

                                                    <p>Orientamos o acionamento dos órgãos competentes de registro e fiscalização profissional dos sistemas CONFEA/CREA e CAUs sempre que forem detectados problemas que envolvam a segurança estrutural da obra, com vistas à responsabilização legal ético-profissional, sem prejuízo de outras medidas junto à defesa civil.</p>

                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="padding: 10px 0 0 0;">
                                                        Atenciosamente,
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="text-align: center; padding: 10px 0 0 0;">
                                                        <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                                        <br />
                                                        <b>Fabrício Batista de Araújo<b>
                                                        <br />
                                                        Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                                        <br />
                                                        CGIMP/DIRPE/FNDE/MEC
                                                </td>
                                            </tr>
                                        </tbody>
                                        <tfoot>

                                        </tfoot>
                                    </table>
                                </body>
                            </html>
                                    ',
            'emlassunto' => 'Aviso de Restrições na Obra (' . $obrid . ') ' . $obra->obrnome,
            'temid' => 32,
            'emlregistroatividade' => true,
            'obrid' => $obrid
        );

        $dadosRemetentes = array();
        foreach ($contatos as $c) {
            if ($c['usuemail']) {
                $dadosRemetentes[] = $c['usuemail'];
            }
        }

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }

    /**
     * Email enviado para avisar que a obra ainda nao possui mobiliario empenhado
     *
     * Só PROINFÂNCIA (inclui todas as fontes) e com mais de 60% de execução, na situação EXECUÇÃO ou CONCLUÍDA.
     * Verificar se já não foi solicitado o empenho no filtro do Obras 2.0:
     *    Mobiliário Empenhado:
     *    Sim Não Todas
     *
     * @param $obrid
     */
    public function enviaEmailEmpenhoMobiliario($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);
        $sql = "
                SELECT
                  o.*,
                  e.prfid,
                  d.esdid,
                  mun.mundescricao,
                  mun.estuf,
                  tpo.tpodsc,
                  ed.esddsc
                FROM obras2.obras o
                JOIN obras2.empreendimento e ON e.empid = o.empid
                JOIN workflow.documento d ON d.docid = o.docid
                JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                JOIN entidade.endereco edr ON edr.endid = o.endid
                JOIN territorios.municipio mun ON mun.muncod = edr.muncod
                JOIN territorios.estado est ON est.estuf = mun.estuf
                JOIN obras2.tipologiaobra tpo ON tpo.tpoid = o.tpoid
                WHERE
                  e.prfid IN (41)
                  AND o.obrpercentultvistoria > 60
                  AND d.esdid IN (690, 693)
                  AND o.obrstatus = 'A'
                  AND o.obridpai IS NULL
                  AND o.obrid IN ($obrid)
                  AND o.obrid NOT IN (SELECT obrid FROM (
                                                        SELECT DISTINCT sov.obrid, sum(es.saldo) AS saldo
													    FROM par.subacaoobravinculacao sov
													    JOIN par.v_saldo_empenho_por_subacao es ON es.sbaid = sov.sbaid AND es.eobano = sov.sovano
													    WHERE
													        sov.obrid IS NOT NULL
													    GROUP BY sov.obrid
													) AS foo
                                    where saldo > 0
                                UNION ALL
                                SELECT obrid FROM obras2.mobiliarioempenhado
                                WHERE moeid IN (
                                                  SELECT MAX(moeid)
                                                  FROM obras2.mobiliarioempenhado
                                                  GROUP BY obrid)
                                AND moeempenhadoflag = 'S')";
        $dadosObra = $db->pegaLinha($sql);

        if (empty($dadosObra))
            return true;

        if($this->verificaEmailEnviado(40, $obrid))
            return true;

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                                <head>
                                    <title></title>
                                </head>
                                <body>
                                    <table style="width: 100%;">
                                        <thead>
                                            <tr>
                                                <td style="text-align: center;">
                                                    <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                    <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                    FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                    DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                    COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                    SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="text-align: right; padding: 40px 0 0 0;">
                                                    <p style="float-right; text-align: right; padding: 40px 0 0 0;">' . $data . '</p>
                                                </td>
                                            </tr>
                                        </thead>
                                        <tbody>
                                            <tr>
                                                <td style="padding:20px 0 20px 0;">
                                                  Assunto: <b>Alerta para empenho de mobiliário da obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="line-height: 15px; text-align:justify">
                                                    <p>Senhor(a)</p>
                                                    <p></p>
                                                    <p>Informo que a obra (' . $obrid . ') ' . $obra->obrnome . ', '.$dadosObra['mundescricao'].'/'.$dadosObra['estuf'].', '.$dadosObra['tpodsc'].', com '.$dadosObra['obrpercentultvistoria'].'% de execução, e na situação '.$dadosObra['esddsc'].',  ainda não possui mobiliário e equipamento empenhado, devendo ser providenciado.</p>
                                                </td>
                                            </tr>
                                           <tr>
                                                <td>&nbsp;</td>
                                                <td>&nbsp;</td>
                                            </tr>
                                            <tr>
                                                <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                    <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                                </td>
                                            </tr>
                                        </tbody>
                                        <tfoot>

                                        </tfoot>
                                    </table>
                                </body>
                            </html>
                                    ',
            'emlassunto' => 'Alerta para empenho de mobiliário da obra (' . $obrid . ') ' . $obra->obrnome,
            'temid' => 40,
            'emlregistroatividade' => 'false',
            'obrid' => $obrid
        );

        $dadosRemetentes = array('laisa.sousa@fnde.gov.br', 'dario.freitas@fnde.gov.br', 'bruno.souza@fnde.gov.br');

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }

    /**
     * Email enviado para avisar que o mobiliario deve ser pago
     *
     * @param $obrid
     */
    public function enviaEmailPagamentoMobiliario($obrid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;

        $obra = new Obras($obrid);
        $sql = "
                SELECT
                  o.*,
                  e.prfid,
                  d.esdid,
                  mun.mundescricao,
                  mun.estuf,
                  tpo.tpodsc,
                  ed.esddsc
                FROM obras2.obras o
                JOIN obras2.empreendimento e ON e.empid = o.empid
                JOIN workflow.documento d ON d.docid = o.docid
                JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                JOIN entidade.endereco edr ON edr.endid = o.endid
                JOIN territorios.municipio mun ON mun.muncod = edr.muncod
                JOIN territorios.estado est ON est.estuf = mun.estuf
                JOIN obras2.tipologiaobra tpo ON tpo.tpoid = o.tpoid
                WHERE
                  e.prfid IN (41)
                  AND o.obrpercentultvistoria > 85
                  AND d.esdid IN (690, 693)
                  AND o.obrstatus = 'A'
                  AND o.obridpai IS NULL
                  AND o.obrid IN ($obrid)
                  AND o.obrid IN (SELECT obrid FROM (
                                                        SELECT DISTINCT sov.obrid, sum(es.saldo) AS saldo
													    FROM par.subacaoobravinculacao sov
													    JOIN par.v_saldo_empenho_por_subacao es ON es.sbaid = sov.sbaid AND es.eobano = sov.sovano
													    WHERE
													        sov.obrid IS NOT NULL
													    GROUP BY sov.obrid
													) AS foo
                                    where saldo > 0
                                UNION ALL
                                SELECT obrid FROM obras2.mobiliarioempenhado
                                WHERE moeid IN (
                                                  SELECT MAX(moeid)
                                                  FROM obras2.mobiliarioempenhado
                                                  GROUP BY obrid)
                                AND moeempenhadoflag = 'S')

                AND (SELECT count(s.sbaid)
                    FROM par.subacaoobravinculacao  sv
                    INNER JOIN par.subacao s on s.sbaid = sv.sbaid
                    INNER JOIN obras.preobra p on p.preid = sv.preid
                    INNER JOIN obras2.obras o on o.obrid = sv.obrid
                    INNER JOIN par.v_dadosempenhosubacao vs on vs.sbaid = s.sbaid AND vs.sbdano = sv.sovano
                    INNER JOIN par.pagamento pa on pa.empid = vs.empid and pa.pagstatus = 'A'
                    INNER JOIN par.pagamentosubacao ps on ps.pagid = pa.pagid and s.sbaid = ps.sbaid and ps.pobano = sv.sovano
                    WHERE sv.obrid = $obrid) = 0";
        $dadosObra = $db->pegaLinha($sql);

        if (empty($dadosObra))
            return true;

        if($this->verificaEmailEnviado(41, $obrid))
            return true;

        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                        <html>
                                <head>
                                    <title></title>
                                </head>
                                <body>
                                    <table style="width: 100%;">
                                        <thead>
                                            <tr>
                                                <td style="text-align: center;">
                                                    <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                                    <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                                    FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                                    DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                                    COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                                    SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="text-align: right; padding: 40px 0 0 0;">
                                                    <p style="float-right; text-align: right; padding: 40px 0 0 0;">' . $data . '</p>
                                                </td>
                                            </tr>
                                        </thead>
                                        <tbody>
                                            <tr>
                                                <td style="padding:20px 0 20px 0;">
                                                  Assunto: <b>Alerta para pagamento de mobiliário da obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td style="line-height: 15px; text-align:justify">
                                                    <p>Senhor(a)</p>
                                                    <p></p>
                                                    <p>Informo que a obra (' . $obrid . ') ' . $obra->obrnome . ', '.$dadosObra['mundescricao'].'/'.$dadosObra['estuf'].', '.$dadosObra['tpodsc'].', com '.$dadosObra['obrpercentultvistoria'].'% de execução, e na situação '.$dadosObra['esddsc'].',  ainda não possui mobiliário e equipamento com pagamento solicitado, devendo ser providenciado.</p>
                                                </td>
                                            </tr>
                                           <tr>
                                                <td>&nbsp;</td>
                                                <td>&nbsp;</td>
                                            </tr>
                                            <tr>
                                                <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                                    <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                                </td>
                                            </tr>
                                        </tbody>
                                        <tfoot>

                                        </tfoot>
                                    </table>
                                </body>
                            </html>
                                    ',
            'emlassunto' => 'Alerta para pagamento de mobiliário da obra (' . $obrid . ') ' . $obra->obrnome,
            'temid' => 41,
            'emlregistroatividade' => 'false',
            'obrid' => $obrid
        );

        $dadosRemetentes = array(
                            'elzanir.gomes@fnde.gov.br',
                            'rosemary.montalvao@fnde.gov.br',
                            'cristiane.tavares@fnde.gov.br',
                            'laisa.sousa@fnde.gov.br',
                            'dario.freitas@fnde.gov.br',
                            'bruno.souza@fnde.gov.br');

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($dadosRemetentes);
        $email->enviar();
    }


    public function enviaEmailSolicitacoesDesembolso($sldid) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        global $db;

        $sql = <<<DML
            SELECT usu.usuemail, obr.obrid, obr.obrnome, esd.esddsc
            FROM obras2.solicitacao_desembolso s
            INNER JOIN obras2.obras obr ON s.obrid = obr.obrid
            INNER JOIN seguranca.usuario usu ON s.usucpf = usu.usucpf
            INNER JOIN workflow.documento doc ON s.docid = doc.docid
            INNER JOIN workflow.historicodocumento hdt ON doc.hstid = hdt.hstid
            INNER JOIN workflow.acaoestadodoc aed ON hdt.aedid = aed.aedid
            INNER JOIN workflow.estadodocumento esd ON aed.esdiddestino = esd.esdid
            WHERE sldid = $sldid
DML;
        $info = $db->pegaLinha($sql);
        $data = new Data();
        $dataTx = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');

        $html = '
             <html>
                <head>
                    <title></title>
                </head>
                <body>
                    <table style="width: 100%;">
                        <thead>
                            <tr>
                                <td style="text-align: center;">
                                    <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                    <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                    FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                    DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                    COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                    SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                </td>
                            </tr>
                            <tr>
                                <td style="text-align: right; padding: 40px 0 0 0;">
                                    ' . $dataTx . '
                                </td>
                            </tr>
                        </thead>
                        <tbody>
                            <tr>
                                <td style="padding:20px 0 20px 0;">
                                  Assunto: <b>Tramitação de Solicitação de Desembolso Nº '.$sldid.' da obra (' . $info['obrid'] . ') ' . $info['obrnome'] . '</b>
                                </td>
                            </tr>
                            <tr>
                                <td style="line-height: 15px; text-align:justify">
                                    <p>Prezado Gestor(a),</p>
                                    <p>A Solicitação de Desembolso N° '.$sldid.' foi tramitada para <b>'. $info['esddsc'] .'</b>.</p>
                                </td>
                            </tr>
                            <tr>
                                <td style="line-height: 15px; text-align:center; bgcolor: #ccc;" colspan="2">
                                    <b> ESTE E-MAIL FOI ENVIADO AUTOMATICAMENTE PELO SISTEMA, FAVOR NÃO RESPONDER. </b>
                                </td>
                            </tr>
                        </tbody>
                        <tfoot>
                            <tr>
                                <td>&nbsp;</td>
                                <td>&nbsp;</td>
                            </tr>
                        </tfoot>
                    </table>
                </body>
            </html>
        ';

        $assunto = "Alerta de Solicitações de Desembolso da Obra " . $info['obrnome'];
        $conteudo = $html;
        $destinatario = array($info['usuemail']);

        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => $conteudo,
            'emlassunto' => $assunto,
            'temid' => 43,
            'emlregistroatividade' => true,
            'obrid' => $info['obrid']
        );

        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvar($destinatario);
        $email->enviar();
        return true;
    }

    /**
     *
     * Destinatários:   FISCAL UNIDADE
     *                  GESTOR UNIDADE
     *
     * @param $obrid
     * @return bool
     */
    public function enviaEmailObraConcluida($obrid, $qtd = 45) {
        require_once APPRAIZ . "includes/classes/dateTime.inc";
        require_once APPRAIZ . "includes/classes/entidades.class.inc";
        global $db;
        $obra = new Obras($obrid);
        $contatos = $this->pegaContatosPar($obra->empid, $obrid);
        $sql = <<<DML
            SELECT empesfera FROM obras2.empreendimento WHERE empid = $obra->empid;
DML;
        $empesfera = $db->pegaUm($sql);
        if($empesfera == 'M') {
            $conteudo_interno = "
                Sr (a) Prefeito (a) <br>
                Informamos que está disponível para a obra ({$obra->obrid} - {$obra->obrnome}) a aba Cumprimento de Objeto.<br>
                Essa aba faz parte da fase de Prestação de Contas do instrumento pactuado, e deve ser preenchida no prazo de $qtd dias, independentemente da abertura ou não do SIGPC-Contas On Line.<br>
                Em caso de dúvidas sobre o preenchimento, contatar através do e-mail cumprimentodoobjeto@fnde.gov.br.
            ";
        } else {
            $conteudo_interno = "
                Sr (a) Secretário (a) <br>
                Informamos que está disponível para a obra ({$obra->obrid} - {$obra->obrnome}) a aba Cumprimento de Objeto.<br>
                Essa aba faz parte da fase de Prestação de Contas do instrumento pactuado, e deve ser preenchida no prazo de $qtd dias, independentemente da abertura ou não do SIGPC-Contas On Line.<br>
                Em caso de dúvidas sobre o preenchimento, contatar através do e-mail cumprimentodoobjeto@fnde.gov.br.
            ";
        }
        $data = new Data();
        $data = $data->formataData($data->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        $dados = array(
            'usucpf' => $_SESSION['usucpf'],
            'emlconteudo' => '
                <html>
                    <head>
                        <title></title>
                    </head>
                    <body>
                        <table style="width: 100%;">
                            <thead>
                                <tr>
                                    <td style="text-align: center;">
                                        <p><img  src="data:image/png;base64,' . $this->getBrasao() . '" width="70"/><br/>
                                        <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                        FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                        DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                        COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                        SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: right; padding: 40px 0 0 0;">
                                        <p style="float:left; text-align: left; padding: 40px 0 0 0;">Comunicado Nº __RGAID__ - CGIMP/DIGAP/FNDE</p>
                                        <p style="float-right; text-align: right; padding: 40px 0 0 0;">' . $data . '</p>
                                    </td>
                                </tr>
                            </thead>
                            <tbody>
                                <tr>
                                    <td style="padding:20px 0 20px 0;">
                                      Assunto: <b>Abertura da Aba Cumprimento do Objeto, obra (' . $obrid . ') ' . $obra->obrnome . '</b>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="line-height: 15px; text-align:justify">
                                        <p>
                                            ' . $conteudo_interno . '
                                        </p>
                                    </td>
                                </tr>
                                <tr>
                                    <td style="padding: 10px 0 0 0;">
                                            Atenciosamente,
                                    </td>
                                </tr>
                                <tr>
                                    <td style="text-align: center; padding: 10px 0 0 0;">
                                            <img align="center" style="height:80px;margin-top:5px;margin-bottom:5px;" src="data:image/png;base64,' . $this->getAssinatura() . '" />
                                            <br />
                                            <b>Fabrício Batista de Araújo<b>
                                            <br />
                                            Coordenador Geral de Implementação e Monitoramento de Projetos Educacionais
                                            <br />
                                            CGIMP/DIRPE/FNDE/MEC
                                    </td>
                                </tr>
                            </tbody>
                            <tfoot>
                                <tr>
                                    <td>&nbsp;</td>
                                    <td>&nbsp;</td>
                                </tr>
                            </tfoot>
                        </table>
                    </body>
                </html>
            ',
            'emlassunto' => 'Abertura da Aba Cumprimento do Objeto, obra (' . $obrid . ') ' . $obra->obrnome,
            'temid' => 46,
            'emlregistroatividade' => true,
            'obrid' => $obrid
        );
        if(!$contatos) {
            $dadosRemetentes = array();
        } else {
            $dadosRemetentes = array($contatos[0]);
        }
        $email = new Email();
        $email->popularDadosObjeto($dados);
        $email->salvarInacabada($dadosRemetentes);
        $email->enviarInacabada();
    }
}
