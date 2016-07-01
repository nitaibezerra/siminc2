<?php

class SupervisaoMi extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisaomi";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("smiid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                    'smiid' => null,
                                    'sobid' => null,
                                    'entidvistoriador' => null,
                                    'endid' => null,
                                    'docid' => null,
                                    'empid' => null,
                                    'usucpf' => null,
                                    'emiid' => null,
                                    'smidtsupervisao' => null,
                                    'smidtcadastro' => null,
                                    'smifuncionamento' => null,
                                    'smicargorepresentante' => null,
                                    'smistatus' => null,
                                    'smiendcorreto' => null,
                                    'smiacordo' => null,
                                    'smidtatualizacao' => null,
                                  );

    public function listaSql(array $param = array()) {
        $where = array();
        $where[] = "smistatus = 'A'";


        if ($param['empid']) {
            $param['empid'] = (array) $param['empid'];
            $where[] = "sm.empid IN(" . implode(", ", $param['empid']) . ")";
        }

        if ($param['btn_copia_supervisao']) {
            $imgBtnCopy = "<img 
                                    align=\"absmiddle\"
                                    src=\"/imagens/gif_inclui.gif\"
                                    style=\"cursor: pointer; margin-left: 3px;\"
                                    onclick=\"javascript: criaCopiaSupEmpresaMI(\'' || sm.smiid || '\');\"
                                    title=\"Copia os dados para nova Supervisão\">";
        } else {
            $imgBtnCopy = ' ';
        }

        $ultSmiid = $this->pegaUltSmiidByEmpid(current($param['empid']));
        $ultSmiid = ($ultSmiid ? $ultSmiid : 0);
        $cpf = $_SESSION['usucpf'];
        $acessoPerfil = (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR)) ? 'true' : 'false');

        $imgExcAtiva = "<img 
                            align=\"absmiddle\"
                            src=\"/imagens/excluir.gif\"
                            style=\"cursor: pointer; margin-left: 3px;\"
                            onclick=\"javascript: excluirSupMI(\'' || sm.smiid || '\');\"
                            title=\"Excluir Supervisão\">";

        $imgExcInativa = "<img 
                                align=\"absmiddle\"
                                src=\"/imagens/excluir_01.gif\"
                                style=\"cursor: pointer; margin-left: 3px;\"
                                onclick=\"javascript: alert(\'Esta supervisão não pode ser excluída\');\"
                                title=\"Esta supervisão não pode ser excluída\">";

        $imgTelaImpressao = "<img 
                                align=\"absmiddle\"
                                style=\"cursor: pointer; margin-left: 3px;\"
                                title=\"Imprimir supervisão MI\" 
                                onclick=\"abreTelaImpressao(\'' || sm.smiid || '\')\" 
                                src=\"../imagens/print.png\"
                             />";

        $acao = "'<center><div style=\"width:80px\">
                                    <img
                                        align=\"absmiddle\"
                                        src=\"/imagens/alterar.gif\"
                                        style=\"cursor: pointer\"
                                        onclick=\"javascript: alterarSupMI(\'' || sm.smiid || '\');\"
                                        title=\"Alterar Supervisão\">' ||
                                    CASE 
                                        WHEN (sm.smiid = {$ultSmiid}) AND (sm.usucpf = '{$cpf}' OR $acessoPerfil) THEN
                                            '{$imgExcAtiva}'
                                        ELSE
                                            '{$imgExcInativa}'
                                    END ||  '{$imgBtnCopy}' ||
                        '{$imgTelaImpressao}</div></center>'";

        $sql = "SELECT
                            {$acao} AS acao,
                            --ed.esddsc,
                            TO_CHAR(sm.smidtsupervisao, 'dd-mm-YYYY') AS smidtsupervisao,
                            TO_CHAR(sm.smidtcadastro, 'dd-mm-YYYY') AS smidtcadastro,
                            e.entnome,
                            sm.smicargorepresentante,
                            u.usunome
                        FROM
                            obras2.supervisaomi sm
                            LEFT JOIN workflow.documento        d ON d.docid = sm.docid
                            LEFT JOIN workflow.estadodocumento  ed ON ed.esdid = d.esdid
                            LEFT JOIN entidade.entidade         e ON e.entid = sm.entidvistoriador
                            JOIN seguranca.usuario              u ON u.usucpf = sm.usucpf
                            WHERE
                                    " . (count($where) ? implode(' AND ', $where) : "");

        //dbg(simec_htmlentities($sql)); exit;

        return $sql;
    }

    public function pegaUltSmiidByEmpid($empid) {
        $sql = "SELECT
					MAX(smiid) AS smiid
				FROM
					obras2.supervisaomi
				WHERE
					smistatus = 'A' AND
					empid = {$empid}";

        return $this->pegaUm($sql);
    }
    
    public function getDadosSupervisaoMi($smiid, $arr_campos = array()) {
        
        if(empty($arr_campos)){
            $campos = '*';
        }else{
            $campos = implode(', ', $arr_campos);
        }
        
        $sql = 'SELECT '.$campos.'  
                FROM obras2.supervisao sup
                INNER JOIN obras2.supervisaomi smi ON sup.smiid = smi.smiid 
                INNER JOIN obras2.obras          o ON sup.obrid = o.obrid
                WHERE sup.smiid = '.$smiid;

        $arr_dados = $this->pegaLinha($sql);
        $arr_dados = ($arr_dados) ? $arr_dados : array();
        
        return $arr_dados;
    }

    public function posAcaoTramiteSupervisaoMI($smiid) {        
        $this->carregarPorId( $smiid );
        $arr_dados_supMi         = $this->getDados();        
        $docid                   = $arr_dados_supMi['docid'];        
        $arr_dados_tramite_supMi = $this->getDadosTramitacaoSupervisaoMi($docid);        
        //Envia o e-mail
        $this->enviaEmailTamiteSupervisaoMi($smiid, $arr_dados_tramite_supMi);
        
        //Atualiza a porcentagem da Obra caso a vistoria seja Validada, ou seja, vai para o estado Concluído
        $dadosSupMi       = $this->getDadosSupervisaoMi($smiid, array('smi.docid','o.obrid', 'sup.supid'));
        $dadoTramiteSupMi = $this->getDadosTramitacaoSupervisaoMi($dadosSupMi['docid'], array('esd.esdid'));
        $obrid            = $dadosSupMi['obrid'];
        $supid            = $dadosSupMi['supid'];
        if($dadoTramiteSupMi['esdid'] == ESDID_MI_CONCLUIDO){
            $obra        = new Obras($obrid);
            $percent_sup = $this->pegaPercentSupervisaoMi($supid,$obrid);
            $resp_atu    = $obra->atualizaPercentualVistoriaObra($obrid, $percent_sup);
        }
        //Caso necessário, mais ações podem ser colocadas aqui;
    }
    
    public function pegaPercentSupervisaoMi($supid, $obrid, $relativoEdificacao = 'D') {
        /*/*Bugado para o caso das supervisões com itens duplicados;
        //Supervisao->pegaPercentSupervisao($supid)
//        $sql = "SELECT
//                        COALESCE(SUM(si.spivlritemsobreobraexecanterior), 0) AS percentual
//                FROM
//                        obras2.supervisaoitem si
//                JOIN
//                        obras2.itenscomposicaoobra ico ON ico.icoid = si.icoid
//                WHERE
//                        si.icoid IS NOT NULL AND
//                        ico.relativoedificacao = 'D' AND
//                        si.supid = {$supid}";*/
        
        //SQL no mesmo formato da SupervisaoItem->getItensByEtapa($obrid, $supid, $relativoEdificacao = 'D')
        //Pois é por ela que a tela é preenchida com os itens "corretos"
        $sql = "SELECT
                    COALESCE(SUM(sic.spivlritemsobreobraexecanterior), 0) AS percentual                    
                FROM
                    obras2.itenscomposicao ic
                JOIN obras2.itenscomposicaoobra ico ON ico.itcid = ic.itcid
                                                       AND ico.icostatus = 'A'
                JOIN obras2.cronograma cro ON cro.croid = ico.croid AND cro.crostatus = 'A'
                LEFT JOIN
                    (SELECT 
                    	ocrvalorexecucao, obrid 
                     FROM 
                     	obras2.obrascontrato 
                     WHERE 
                     	obrid = {$obrid} AND 
                     	ocrstatus = 'A'
                        ORDER BY ocrid desc
                        LIMIT 1) ocr ON ocr.obrid = ico.obrid 
                        
                LEFT JOIN ( select * 
                            from obras2.supervisaoitem s1 
                            where s1.spiid in( select max(s2.spiid) 
                                               from obras2.supervisaoitem s2 
                                               where s2.supid = {$supid} 
                                                 and s2.icoid = s1.icoid) 
                            AND s1.supid = {$supid}
                ) sic ON sic.icoid = ico.icoid
                     AND sic.supid = {$supid}
                     AND sic.icoid IS NOT NULL
                     AND sic.ditid IS NULL

                LEFT JOIN obras2.unidademedida um ON um.umdid = ico.umdid

                WHERE
                    ic.itcstatus = 'A' AND
                    cro.obrid = {$obrid} AND
                    ico.obrid = cro.obrid AND
                    ico.relativoedificacao = '{$relativoEdificacao}'
                ";
                    
        $percentual = $this->pegaUm($sql);

        return $percentual;
    }
    
    public function enviaEmailTamiteSupervisaoMi($smiid, $arr_dados_tramite_supMi) {
        // Monta o filtro de Perfis Responsáveis por cada ação do Workflow
        switch ($arr_dados_tramite_supMi['aedid']) {
            //Em cadastramento: Enviar para validação
            case 2448:
                $filtro_resp = ' '.PFLCOD_SUPERVISOR_UNIDADE.', '.PFLCOD_GESTOR_UNIDADE;
                break;
            //Validação: Validar
            case 2449:
                $filtro_resp = ' '.PFLCOD_EMPRESA_MI_GESTOR.', '.PFLCOD_EMPRESA_MI_FISCAL;
                break;
            //Validação: Enviar para Correção
            case 2450:
                $filtro_resp = ' '.PFLCOD_EMPRESA_MI_GESTOR.', '.PFLCOD_EMPRESA_MI_FISCAL;
                break;
            //Correção: Enviar para Validação
            case 2451:
                $filtro_resp = ' '.PFLCOD_SUPERVISOR_UNIDADE.', '.PFLCOD_GESTOR_UNIDADE;
                break;
            default:
                $filtro_resp = ' '.PFLCOD_SUPERVISOR_UNIDADE.', '.PFLCOD_GESTOR_UNIDADE;
                break;
        }
        
        $dados_responsaveis = $this->getDadosResponsaveisTramitacaoSupervisaoMi($smiid, $filtro_resp);
        
        // Faz o envio do email do tramite da Supervisão Empresa MI
        if(!empty($dados_responsaveis)){
            foreach ($dados_responsaveis as $key => $usuario) {
                $destinatario = array($usuario['email']);
                $dados_email  = $this->montaCorpoEmailTamiteSupervisaoMi($smiid, $arr_dados_tramite_supMi, $usuario);                
                $email        = new Email();
                $email->popularDadosObjeto($dados_email);                    
                $email->salvar($destinatario);
                $email->enviar();
            }            
        }else{
            $destinatario = array('');
            $dados_email  = $this->montaCorpoEmailTamiteSupervisaoMi($smiid, $arr_dados_tramite_supMi, array());
            $email        = new Email();
            $email->popularDadosObjeto($dados);                    
            $email->salvar($destinatario);
            // Não enviar, apenas salvar até segunda ordem.            
        }
        return true;
    }
    /**
     * 
     * @param int $smiid
     * @param array $array_dados
     * @param array $responsavel_destino
     * @return array no formato de envio do Email.class.inc
     */
    public function montaCorpoEmailTamiteSupervisaoMi($smiid, $array_dados = array(), $responsavel_destino = array()) {
        $arr_sup   = $this->getDadosSupervisaoMi($smiid, array('o.obrid','o.obrnome'));
        $nome_obra = '('.$arr_sup['obrid'].')'.$arr_sup['obrnome'];
        $obrid     = $arr_sup['obrid'];
        
        $data_tramitacao    = $array_dados['data_tramitacao'];
        $nome_tramitador    = $array_dados['usu_nome_exec_tramitacao'];
        $email_tramitador   = $array_dados['usu_email_exec_tramitacao'];
        $estado_atual_os    = $array_dados['estado_de_destino'];
        $estado_anterior_os = $array_dados['estado_de_origem'];
        $acao_realizada     = $array_dados['aeddscrealizada'];
        $email = new Email();
        $dt   = new Data();
        $data = $dt->formataData($dt->dataAtual(), 'Brasília, DD de mesTextual de YYYY.');
        
        if(empty($responsavel_destino)){
            $nome_responsavel   = 'Usuário não encontrado';
            $cpf_responsavel    = '000000';
            $email_responsavel  = '';
        }else{
            $nome_responsavel   = $responsavel_destino['nome'];
            $cpf_responsavel    = $responsavel_destino['cpf'];
            $email_responsavel  = $responsavel_destino['email'];            
        }
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
                                            <p><img  src="data:image/png;base64,'. $email->getBrasao() . '" width="70"/><br/>
                                            <b>MINISTÉRIO DA EDUCAÇÃO</b><br/>
                                            FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO - FNDE<br/>
                                            DIRETORIA DE GESTÃO, ARTICULAÇÃO E PROJETOS EDUCACIONAIS - DIGAP<br/>
                                            COORDENAÇÃO GERAL DE IMPLEMENTAÇÃO E MONITORAMENTO DE PROJETOS EDUCACIONAIS - CGIMP<br/>
                                            SBS Q.2 Bloco F Edifício FNDE - 70.070-929 - Brasília, DF - Telefone: (61) 2022.4696/4694 - E-mail: monitoramento.obras@fnde.gov.br<br/>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="text-align: right; padding: 40px 0 0 0;" colspan="2">
                                            '.$data.'
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
                                            <b> TRAMITAÇÃO DE SUPERVISÃO EMPRESA MI </b>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td style="padding:20px 0 20px 0;" colspan="2">
                                          Assunto: SIMEC - SUPERVISÃO EMPRESA MI - ID '.$smiid.' - Tramitada
                                        </td>
                                    </tr>

                                    <tr>
                                        <td>Nome do Responsável:</td>
                                        <td>'.$nome_responsavel.'</td>
                                    </tr>
                                    <tr>
                                        <td>CPF do Responsável:</td>
                                        <td>'.$cpf_responsavel.'</td>
                                    </tr>
                                    <tr>
                                        <td>Obra:</td>
                                        <td>'.$nome_obra.'</td>
                                    </tr>                            
                                    <tr>
                                        <td>Estado Atual:</td>
                                        <td><b> '.$estado_atual_os.' </b></td>
                                    </tr>
                                    <tr>
                                        <td>&nbsp;</td>
                                        <td>&nbsp;</td>
                                    </tr>
                                    <tr>
                                        <td>Estado Anterior:</td>
                                        <td><b> '.$estado_anterior_os.' </b></td>
                                    </tr>
                                    <tr>
                                        <td>Data da Tramitação:</td>
                                        <td>'.$data_tramitacao.'</td>
                                    </tr>
                                    <tr>
                                        <td>Quem tramitou:</td>
                                        <td> '.$nome_tramitador.' - ('.$email_tramitador.')</td>
                                    </tr>
                                    <tr>
                                        <td>Ação realizada:</td>
                                        <td> <b>'.$acao_realizada.'</b></td>
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
        
            $assunto            = "SIMEC - TRAMITAÇÃO DE SUPERVISÃO EMPRESA MI - ID ".$smiid." - Tramitada";
            $conteudo           = $html;
            
            $dados = array(
                            'usucpf'               => $_SESSION['usucpf'], 
                            'emlconteudo'          => $conteudo,
                            'emlassunto'           => $assunto, 
                            'temid'                => 5,
                            'emlregistroatividade' => true,
                            'obrid'                => $obrid
                          );

            return $dados;
    }

    /**
     * Recebe o docid da Supervisao MI e retorna a linha com os dados da última tramitação da Supervisão MI
     * @param int    $docid      - ID do Doc WF
     * @param array  $arr_campos - Array simples com os campos necessários para consulta. Se não for passado nada, serão considerados os campos padrão da consulta.
     * @return array $dados_tramitacao
     */
    public function getDadosTramitacaoSupervisaoMi($docid = null, $arr_campos = array()) {

        if ($docid == null) {
            $docid = $this->docid;
            if ($docid == null) {
                return array();
            }
        }
        
        if(empty($arr_campos)){
            $campos = " 
                       sup.obrid, smi.smiid, 
                       smi.usucpf as usu_cpf_cad_os, usu_os.usunome as usu_nome_cad_os,
                       smi.docid, tpd.tpddsc, doc.docdsc,
                       esd.esdid, esd.esddsc, hst.hstid, 
                       hst.usucpf as usu_cpf_exec_tramitacao, usu_acao.usunome as usu_nome_exec_tramitacao, 
                       usu_acao.usuemail as usu_email_exec_tramitacao,

                       hst.htddata, aed.aedid,
                       aed.aeddscrealizada, 
                       aed.esdidorigem, aed.esdiddestino,
                       esdorigem.esddsc as estado_de_origem, esddestino.esddsc as estado_de_destino,
                       COALESCE(to_char(hst.htddata, 'DD/MM/YYYY')) AS data_tramitacao
                      ";
        }else{
            $campos = implode(', ', $arr_campos);     
        }

        $sql = " SELECT ".$campos."

                 FROM       obras2.supervisaomi         smi
                 INNER JOIN obras2.supervisao           sup ON sup.smiid  = smi.smiid
                 INNER JOIN workflow.documento          doc ON doc.docid  = smi.docid
                 INNER JOIN workflow.tipodocumento      tpd ON tpd.tpdid  = doc.tpdid
                 INNER JOIN workflow.estadodocumento    esd ON esd.esdid  = doc.esdid
                 INNER JOIN workflow.historicodocumento hst ON hst.hstid  = doc.hstid
                 INNER JOIN workflow.acaoestadodoc      aed ON aed.aedid  = hst.aedid
                 INNER JOIN seguranca.usuario        usu_os ON smi.usucpf = usu_os.usucpf 
                 INNER JOIN seguranca.usuario      usu_acao ON hst.usucpf = usu_acao.usucpf 
                 INNER JOIN workflow.estadodocumento esdorigem  ON aed.esdidorigem  = esdorigem.esdid
                 INNER JOIN workflow.estadodocumento esddestino ON aed.esdiddestino = esddestino.esdid

                 WHERE doc.docid     = " . $docid . "
                   AND smi.smistatus = 'A'
                ";

        $dados_tramitacao = $this->pegaLinha($sql);
        $dados_tramitacao = ($dados_tramitacao) ? $dados_tramitacao : array();

        return $dados_tramitacao;
    }
    
    public function getDadosResponsaveisTramitacaoSupervisaoMi($smiid, $filtro_resp){
        
        $arr = $this->getDadosSupervisaoMi($smiid);
        
        if(!empty($arr)){
            $empid = $arr['empid'];            
        }else{
            return array();
        }
        
        if(trim($filtro_resp) == ''){
            return array();            
        }
        
        $filtro_sql_resp = " AND ur.pflcod IN (".$filtro_resp.")";

        $sql_responsaveis = "SELECT DISTINCT
                                         su.usucpf   as cpf,
                                         su.usunome  as nome,
                                         su.usuemail as email
                             FROM seguranca.usuario su
                             INNER JOIN obras2.usuarioresponsabilidade ur ON ur.usucpf = su.usucpf AND ur.rpustatus = 'A'
                             WHERE
                                     ur.empid = {$empid}
                                     {$filtro_sql_resp}
                             ORDER BY
                                     su.usunome";

        $dados_responsaveis = $this->carregar($sql_responsaveis);
        $dados_responsaveis = (is_array($dados_responsaveis) && count($dados_responsaveis) > 0) ? $dados_responsaveis : array();
        
        return $dados_responsaveis;
    }

    
    
    
    
}
