<?php
	
class OrdemServicoMI extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.ordemservicomi";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "osmid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'osmid' => null, 
                                        'obrid' => null, 
                                        'usucpf' => null, 
                                        'tomid' => null, 
                                        'osmdtcadastro' => null, 
                                        'osmdtinicio' => null, 
                                        'osmdtinicioempresa' => null,
                                        'osmdttermino' => null,
                                        'osmprazo' => null, 
                                        'osmstatus' => null, 
                                        'arqid' => null, 
                                        'docid' => null, 
                                        'osmdtterminomun' => null,
                                        'osmdtiniciomun' => null,
                                      );

    public function carregarPorObrid( $obrid ){
            $sql = "SELECT
                                    *
                            FROM
                                    obras2.ordemservicomi
                            WHERE
                                    osmstatus = 'A' AND
                                    obrid = {$obrid}";

            $dados = $this->pegaLinha( $sql );
            $dados = ($dados ? $dados : array());
            $this->popularDadosObjeto( $dados );

            return $dados;
    }

    public function carregarPorObridETomid( $obrid, $tomid ){
            $sql = "SELECT
                                    *
                            FROM
                                    obras2.ordemservicomi
                            WHERE
                                    osmstatus = 'A' AND
                                    obrid = {$obrid} AND
                                    tomid = {$tomid}
                            order by osmid desc limit 1         
                                    ";
//                                        ver($sql,d);
            $dados = $this->pegaLinha( $sql );
            $dados = ($dados ? $dados : array());
            $this->popularDadosObjeto( $dados );

            return $dados;
    }

    public function carregarPorOsmid( $osmid ){
            $sql = "SELECT
                                    *
                            FROM
                                    obras2.ordemservicomi
                            WHERE
                                    osmstatus = 'A' AND
                                    osmid = {$osmid}";

            $dados = $this->pegaLinha( $sql );
            $dados = ($dados ? $dados : array());
            $this->popularDadosObjeto( $dados );

            return $dados;
    }

    public function carregarPorObrideTipo( $obrid, $tomid ){
            $sql = "SELECT
                            *
                    FROM
                            obras2.ordemservicomi
                    WHERE
                            osmstatus = 'A' AND
                            obrid = {$obrid} AND
                            tomid = {$tomid}
                            ";

            $dados = $this->pegaLinha( $sql );
            $dados = ($dados ? $dados : array());
            $this->popularDadosObjeto( $dados );

            return $dados;
    }

    public function pegaCadastrantePorOsmid( $osmid ){
            $sql = "SELECT
                            u.usunome
                    FROM
                            obras2.ordemservicomi om
                    JOIN seguranca.usuario u ON u.usucpf = om.usucpf
                    WHERE
                            osmstatus = 'A' AND
                            osmid = {$osmid}";
            $usunome = $this->pegaUm( $sql );

            return $usunome;
    }

    public function pegaTodasOSMI($onlySQL=false, $where=false){
        
        if (possui_perfil(array(PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_ADMINISTRATIVO)) && !$this->testa_superuser()) {

            $joinEmpresaMI = " 	INNER JOIN obras2.empresami_uf euf ON euf.estuf = edo.estuf AND euf.eufstatus = 'A'
                                INNER JOIN obras2.empresami    emi ON emi.emiid = euf.emiid  AND emi.emistatus = 'A'";
                              
            $where[] = "
                        emi.emiid IN (SELECT urs.emiid FROM obras2.usuarioresponsabilidade urs
                                            WHERE  urs.rpustatus = 'A' AND urs.usucpf = '{$_SESSION['usucpf']}'  
                                            GROUP BY urs.emiid)
                       ";

            
            $btn_acoes = "
                             '<center>
                        <div style=\"width:100px\">
                            
                            <img
                                align=\"absmiddle\"
                                src=\"/imagens/alterar.gif\"
                                style=\"cursor: pointer\"
                                onclick=\"javascript: alterarObr(\'' || o.obrid || '\');\"
                                title=\"Alterar Obra\"
                            />' || '
                        </div>
                        </center>'
                            
                            ";
            
            
            
        }  else {
            $joinEmpresaMI = "LEFT JOIN obras2.empresami_uf euf ON euf.estuf = edo.estuf AND euf.eufstatus = 'A'
                              LEFT JOIN obras2.empresami    emi ON emi.emiid = euf.emiid AND emi.emistatus = 'A'";
            
            $btn_acoes = " '<center>
                        <div style=\"width:100px\">
                            <img
                                align=\"absmiddle\"
                                src=\"/imagens/icone_lupa.png\"
                                style=\"cursor: pointer\"
                                onclick=\"cadastrarOS(\''||o.obrid||'\', '||tos.tomid||', '||os.osmid||');\"
                                title=\"Ver OS\"
                            />

                            <img
                                align=\"absmiddle\"
                                src=\"/imagens/edit_on.gif\"
                                style=\"cursor: pointer\"
                                onclick=\"javascript: abreListaSupervisaoFnde(\'' || o.obrid || '\',\'' || o.empid || '\');\"
                                title=\"Supervisão FNDE\"
                            />
                            <img
                                align=\"absmiddle\"
                                src=\"/imagens/alterar.gif\"
                                style=\"cursor: pointer\"
                                onclick=\"javascript: alterarObr(\'' || o.obrid || '\');\"
                                title=\"Alterar Obra\"
                            />'
                            ||
                            CASE
                            WHEN oc.obrid IS NOT NULL THEN
                            ' <img
                                align=\"absmiddle\"
                                src=\"/imagens/seriehistorica_ativa.gif\"
                                style=\"cursor: pointer\"
                                onclick=\"javascript: abreEvolucaoFinan(\'' || o.obrid || '\');\"
                                title=\"Evolução Financeira\"
                            />'
                            ELSE
                             '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
                            END || '
                        </div>
                        </center>' ";
            
        }
    
            $where = !empty($where) ? ' AND ' . implode(' AND ', $where) : '';
            $sql = "SELECT 
                        {$btn_acoes} AS acao,
                        o.obrid, 
                        tos.tomdsc AS tomdsc,
                        CASE 
                            WHEN d.esdid = ".ESDID_OS_MI_CADASTRAMENTO." THEN
                                '<img src=\"../imagens/0_inexistente.png\" border=\"0\" title=\"Cadastramento\" />'
                            WHEN d.esdid = ".ESDID_OS_MI_EXECUCAO." THEN
                                '<img src=\"../imagens/0_ativo.png\" border=\"0\" title=\"Execução\" />'
                            WHEN d.esdid = ".ESDID_OS_MI_AGUARDANDO_ACEITE." THEN
                                '<img src=\"../imagens/0_alerta.png\" border=\"0\" title=\"Aguardando aceite da empresa\" />'
                            WHEN d.esdid = ".ESDID_OS_MI_VALIDACAO." THEN
                                '<img src=\"../imagens/0_ativo.png\" border=\"0\" title=\"Validação\" />'
                            WHEN d.esdid = ".ESDID_OS_MI_CORRECAO." THEN
                                '<img src=\"../imagens/0_ativo.png\" border=\"0\" title=\"Correção da execução\" />'
                            WHEN d.esdid = ".ESDID_OS_MI_CONCLUIDA." THEN
                                '<img src=\"../imagens/0_concluido.png\" border=\"0\" title=\"Concluída\" />'
                            WHEN d.esdid = ".ESDID_OS_MI_CANCELADA." THEN
                                '<img src=\"../imagens/0_inativo.png\" border=\"0\" title=\"Cancelada\" />'
                            WHEN d.esdid = ".ESDID_OS_MI_REPROVADA." THEN
                                '<img src=\"../imagens/0_inativo.png\" border=\"0\" title=\"O.S. Reprovada\" />'
                            ELSE
                                ''
                        END
                        AS os,
                        ed.esddsc as esdos,
                        edo.estuf,
                        mun.mundescricao,
                        emi.emidscresumo AS descricao, --foo.descricao AS empresa,
                        o.obrnome,
                        esd.esddsc as esdidobr,
                        TO_CHAR(os.osmdtcadastro, 'dd/mm/YYYY') AS osmdtcadastro, 
                        TO_CHAR(os.osmdtinicio, 'dd/mm/YYYY') AS osmdtinicio,
                        (CASE 
                            WHEN d.esdid <> ".ESDID_OS_MI_CONCLUIDA." AND os.osmdttermino < NOW() THEN
                                '<font color=\"#FF0000\">' || TO_CHAR(os.osmdttermino, 'dd/mm/YYYY') || '</font>'
                            ELSE
                                TO_CHAR(os.osmdttermino, 'dd/mm/YYYY')
                        END) AS osmdttermino,
                        os.osmprazo,
                        CASE WHEN d.esdid = 905 THEN
                        DATE_PART('days', NOW() - os.osmdtinicio)
                                ELSE
                        null
                                END as tempoexecucao
                FROM obras2.ordemservicomi os 
                        JOIN obras2.obras               o ON os.obrid  = o.obrid
                        JOIN entidade.entidade          e ON e.entid   = o.entid
                        JOIN obras2.tipoosmi          tos ON os.tomid  = tos.tomid
                        JOIN seguranca.usuario          u ON os.usucpf = u.usucpf
                        --Estado OS
                        JOIN workflow.documento         d ON os.docid  = d.docid
                        JOIN workflow.estadodocumento  ed ON ed.esdid  = d.esdid
                        --Estado Obra
                        JOIN workflow.documento       doc ON o.docid   = doc.docid
                        JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                        
                        LEFT JOIN entidade.endereco     edo ON edo.endid            = o.endid
                        LEFT JOIN territorios.estado     uf ON uf.estuf             = edo.estuf
                        LEFT JOIN territorios.municipio mun ON mun.muncod           = edo.muncod
                        LEFT JOIN obras2.supervisaomi   smi ON smi.entidvistoriador = e.entid
                        LEFT JOIN obras2.obrascontrato   oc ON oc.obrid             = o.obrid AND oc.ocrstatus = 'A'
                        LEFT JOIN obras2.supervisao_grupo_empresa sge ON e.entid    = sge.entid
                        
                        LEFT JOIN(SELECT
                                euf.emiid, 
                                euf.estuf, 
                                em.emidscresumo AS descricao
                                --em.emidsc AS descricao
                            FROM obras2.empresami em
                            JOIN obras2.empresami_uf euf ON euf.emiid = em.emiid AND euf.eufstatus = 'A'
                            WHERE emistatus = 'A') foo ON (edo.estuf = foo.estuf)
                            
                            {$joinEmpresaMI}
                WHERE ed.esdstatus = 'A' AND os.osmstatus = 'A'                 
                     {$where}
                ORDER BY os.osmid DESC";
            if ($onlySQL) return $sql;
            
            $dados = $this->carregar($sql);
            
            return $dados;
        }
    
    /**
     * Recebe o docid da OS MI e retorna a linha com os dados da última tramitação da OS
     * @param int $docid 
     * @return array $dados_tramitacao
     */
    public function getDadosTramitacaoOsMi($docid = null){
        
        if($docid == null){
           $docid = $this->docid; 
           if($docid == null){
               return array();
           }
        }
        
        $sql = " SELECT osm.obrid, osm.osmid, 
                        osm.usucpf as usu_cpf_cad_os, usu_os.usunome as usu_nome_cad_os,
                        tom.tomdsc, osm.docid, tpd.tpddsc, doc.docdsc,
                        esd.esdid, esd.esddsc, hst.hstid, 
                        hst.usucpf as usu_cpf_exec_tramitacao, usu_acao.usunome as usu_nome_exec_tramitacao, 
                        usu_acao.usuemail as usu_email_exec_tramitacao,

                        hst.htddata, aed.aedid,
                        aed.aeddscrealizada, 
                        aed.esdidorigem, aed.esdiddestino,
                        esdorigem.esddsc as estado_de_origem, esddestino.esddsc as estado_de_destino,
                        COALESCE(to_char(hst.htddata, 'DD/MM/YYYY')) AS data_tramitacao

                 FROM obras2.ordemservicomi             osm
                 INNER JOIN obras2.tipoosmi             tom ON tom.tomid  = osm.tomid
                 INNER JOIN workflow.documento          doc ON doc.docid  = osm.docid
                 INNER JOIN workflow.tipodocumento      tpd ON tpd.tpdid  = doc.tpdid
                 INNER JOIN workflow.estadodocumento    esd ON esd.esdid  = doc.esdid
                 INNER JOIN workflow.historicodocumento hst ON hst.hstid  = doc.hstid
                 INNER JOIN workflow.acaoestadodoc      aed ON aed.aedid  = hst.aedid
                 INNER JOIN seguranca.usuario        usu_os ON osm.usucpf = usu_os.usucpf 
                 INNER JOIN seguranca.usuario      usu_acao ON hst.usucpf = usu_acao.usucpf 
                 INNER JOIN workflow.estadodocumento esdorigem  ON aed.esdidorigem  = esdorigem.esdid
                 INNER JOIN workflow.estadodocumento esddestino ON aed.esdiddestino = esddestino.esdid

                 WHERE doc.docid = ".$docid."
                   AND osm.osmstatus = 'A'
                "; 
        
        $dados_tramitacao = $this->pegaLinha($sql);        
        $dados_tramitacao = ($dados_tramitacao) ? $dados_tramitacao  : array();
        
        return $dados_tramitacao;
    }
    
    /**
     * Retorna um array com os botões de forma individual e juntos
     */
    public function getBtnSinalizacaoOsMi($obrid) {
        
        $fil['tipoAcao']      = 'aguardandoAceite';
        $fil['obrid']         = $obrid;
        $fil['not(obridpai)'] = true;
        $fil['IS(mi)']        = true;

        $obr  = new Obras($obrid);
        $sql1 = $obr->listaMiSql($fil);
        $dd   = $this->carregar($sql1);
        $ddos = $dd[0]['os'];
        $text = preg_replace('/\s+/', ' ', $ddos);
        $text_arr = explode('>', $text);
        
        $mi_visu_os = ' <script type="text/javascript">                            
                            function visualizarOsTomidObra(obrid,tomid){
                                var url = "obras2.php?modulo=principal/popupAceiteOS&acao=A&tomid="+tomid+"&obrid="+obrid;
                                window.open( url, "Aceite/Emição de OS", "width=750,height=600,scrollbars=yes,scrolling=no,resizebled=no");            
                            };
                            
                            function cadastrarOsTomidObra(obrid,tomid){
                                var url = "obras2.php?modulo=principal/cadOsMi&acao=A&tomid="+tomid+"&obrid="+obrid;
                                window.open( url, "Aceite/Emição de OS", "width=750,height=600,scrollbars=yes,scrolling=no,resizebled=no");            
                            };                            
                        </script>
                        ';
        
        $text_arr[0] = (trim($text_arr[0]) == '') ? '<img align="absmiddle" src="/imagens/0_inexistente.png" style="cursor: pointer; width:15px;" title="Não existe OS de Sondagem cadastrada para essa obra!" '               : $text_arr[0];
        $text_arr[1] = (trim($text_arr[1]) == '') ? '<img align="absmiddle" src="/imagens/0_inexistente.png" style="cursor: pointer; width:15px;" title="Não existe OS do Projeto de Implantação cadastrada para essa obra!" ' : $text_arr[1];
        $text_arr[2] = (trim($text_arr[2]) == '') ? '<img align="absmiddle" src="/imagens/0_inexistente.png" style="cursor: pointer; width:15px;" title="Não existe OS de Execução cadastrada para essa obra!" '               : $text_arr[2];
        
        $func_sondagem    = (strpos($text_arr[0],'inexistente') == false ) ? 'visualizarOsTomidObra': 'cadastrarOsTomidObra'; 
        $func_implantacao = (strpos($text_arr[1],'inexistente') == false ) ? 'visualizarOsTomidObra': 'cadastrarOsTomidObra'; 
        $func_execucao    = (strpos($text_arr[2],'inexistente') == false ) ? 'visualizarOsTomidObra': 'cadastrarOsTomidObra'; 
        
        $mi_visu_os .= ' '.$text_arr[0].' onclick="javascript: '.$func_sondagem.   '('.$obr->obrid.',2);">'. 
                       ' '.$text_arr[1].' onclick="javascript: '.$func_implantacao.'('.$obr->obrid.',3);">'. 
                       ' '.$text_arr[2].' onclick="javascript: '.$func_execucao.   '('.$obr->obrid.',1);">'; 
        
        $arr_retorno['sondagem']    = $text_arr[0].' onclick="javascript: '.$func_sondagem.   '('.$obr->obrid.',2);">';
        $arr_retorno['implantacao'] = $text_arr[1].' onclick="javascript: '.$func_implantacao.'('.$obr->obrid.',3);">';
        $arr_retorno['execucao']    = $text_arr[2].' onclick="javascript: '.$func_execucao.   '('.$obr->obrid.',1);">'; 
        $arr_retorno['imgs_juntas'] = $mi_visu_os;
        
        return $arr_retorno;
        
    }
    
    public function getDadosEspelhoOsMi($obrid = NULL) {
        
        if($obrid == NULL || trim($obrid) == ''){
            $obrid = $_SESSION['obras2']['obrid'];
            if($obrid == NULL || trim($obrid) == ''){
                $obrid = $this->obrid;
            }
            if($obrid == NULL || trim($obrid) == ''){
                return array();
            }
        }

        $os_exec = $this->carregarPorObridETomid($obrid, 1 ); // Execução               - tomid = 1
        $os_sond = $this->carregarPorObridETomid($obrid, 2 ); // Sondagem               - tomid = 2
        $os_impl = $this->carregarPorObridETomid($obrid, 3 ); // Projeto de Implantação - tomid = 3

        // Carrega o estado (WF) das OS
        if($os_exec['docid']){
            $estado_os = $this->getDadosTramitacaoOsMi($os_exec['docid']);
            $os_exec['estado_os'] = $estado_os['esddsc'];
            $os_exec['estado_id'] = $estado_os['esdid'];
        }
        if($os_sond['docid']){
            $estado_os = $this->getDadosTramitacaoOsMi($os_sond['docid']);
            $os_sond['estado_os'] = $estado_os['esddsc'];
            $os_sond['estado_id'] = $estado_os['esdid'];
        }
        if($os_impl['docid']){
            $estado_os = $this->getDadosTramitacaoOsMi($os_impl['docid']);
            $os_impl['estado_os'] = $estado_os['esddsc'];
            $os_impl['estado_id'] = $estado_os['esdid'];
        }

        // Carrega arquivos anexo das OSs
        // OS de Execução               - tomid = 1
        if(count($os_exec) > 0){
            // OS Assinada
            $anexoOs = new AnexoOsMi();
            $anexoOs = $anexoOs->getOsAssinada($os_exec['osmid']);
            if($anexoOs){
                $arquivo = new Arquivo($anexoOs['arqid']);
                if ($arquivo->arqid) {
                    $arr_arq['link']         = "<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>" . $arquivo->arqnome . "." . $arquivo->arqextensao . "</a>";
                    $arr_arq['arqid']        = $arquivo->arqid;
                    $arr_arq['arqnome']      = $arquivo->arqnome.".".$arquivo->arqextensao;
                    $os_exec['arq_os_ass'][] = $arr_arq;
                }
            }
            //ArqOS
            $arquivo = new Arquivo($os_exec['arqid']);
            if ($arquivo->arqid) {
                $arr_arq['link']         = "<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
                $arr_arq['arqid']        = $arquivo->arqid;
                $arr_arq['arqnome']      = $arquivo->arqnome.".".$arquivo->arqextensao;
                $os_exec['arquivo_os'][] = $arr_arq;
            }
        }
        // OS de Sondagem               - tomid = 2
        // Laudo de Sondagem
        if(count($os_sond) > 0){
            $anexoOs = new AnexoOsMi();
            $anexoOs = $anexoOs->getAnexoExecucaoSondagem($os_sond['osmid']);
            $arquivo = new Arquivo($anexoOs['arqid']);
            if ($arquivo->arqid) {
                $arr_arq['link']         = "<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
                $arr_arq['arqid']        = $arquivo->arqid;
                $arr_arq['arqnome']      = $arquivo->arqnome.".".$arquivo->arqextensao;
                $os_sond['laudo_sondagem'][] = $arr_arq;
            }
            // OS Assinada
            $anexoOs = new AnexoOsMi();
            $anexoOs = $anexoOs->getOsAssinada($os_sond['osmid']);
            if($anexoOs){
                $arquivo = new Arquivo($anexoOs['arqid']);
                if ($arquivo->arqid) {
                    $arr_arq['link']         = "<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>" . $arquivo->arqnome . "." . $arquivo->arqextensao . "</a>";
                    $arr_arq['arqid']        = $arquivo->arqid;
                    $arr_arq['arqnome']      = $arquivo->arqnome.".".$arquivo->arqextensao;
                    $os_sond['arq_os_ass'][] = $arr_arq;
                }
            }
            //ArqOS
            $arquivo = new Arquivo($os_sond['arqid']);
            if ($arquivo->arqid) {
                $arr_arq['link']         = "<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
                $arr_arq['arqid']        = $arquivo->arqid;
                $arr_arq['arqnome']      = $arquivo->arqnome.".".$arquivo->arqextensao;
                $os_sond['arquivo_os'][] = $arr_arq;
            }
        }
        // OS de Projeto de Implantação - tomid = 3
        if(count($os_impl) > 0){
            $os_impl['anexos'] = array();
            $anexoOsMi = new AnexoOsMi();
            $anexoOsMi = $anexoOsMi->getAnexoExecucaoImplantacao($os_impl['osmid']);
            if (!empty($anexoOsMi) > 0) {
                foreach ($anexoOsMi as $anexo) {
                    $arquivo = new Arquivo($anexo['arqid']); 
                    $arr_arq['link']     = "<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>" . $arquivo->arqnome . "." . $arquivo->arqextensao . " (" . $anexo['taonome'] . ")</a>";
                    $arr_arq['arqid']    = $arquivo->arqid;
                    $arr_arq['arqnome']  = $arquivo->arqnome.".". $arquivo->arqextensao;
                    $os_impl['anexos'][] = $arr_arq;
                } 
            }else{
                $os_impl['anexos'] = array();
            }
            // OS Assinada
            $anexoOs = new AnexoOsMi();
            $anexoOs = $anexoOs->getOsAssinada($os_impl['osmid']);
            if($anexoOs){
                $arquivo = new Arquivo($anexoOs['arqid']);
                if ($arquivo->arqid) {
                    $arr_arq['link']         = "<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>" . $arquivo->arqnome . "." . $arquivo->arqextensao . "</a>";
                    $arr_arq['arqid']        = $arquivo->arqid;
                    $arr_arq['arqnome']      = $arquivo->arqnome.".".$arquivo->arqextensao;
                    $os_impl['arq_os_ass'][] = $arr_arq;
                }
            }
            //ArqOS
            $arquivo = new Arquivo($os_impl['arqid']);
            if ($arquivo->arqid) {
                $arr_arq['link']         = "<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
                $arr_arq['arqid']        = $arquivo->arqid;
                $arr_arq['arqnome']      = $arquivo->arqnome.".".$arquivo->arqextensao;
                $os_impl['arquivo_os'][] = $arr_arq;
            }
            //Serviços externos
            $os_impl['servicos_externos'] = array();
            $estado = $os_impl['estado_id'];
            
            if ($estado != ESDID_OS_MI_AGUARDANDO_ACEITE){
                $os_impl['servicos_externos'] = $this->getArrayServicosExternosImplantacao($obrid);
            }
        }
        
        $dados_os_espelho['os_exec'] = $os_exec;
        $dados_os_espelho['os_sond'] = $os_sond;
        $dados_os_espelho['os_impl'] = $os_impl; 
        
        return $dados_os_espelho;
    }
    
    
    public function getArrayServicosExternosImplantacao($obrid = NULL) {
        
        if($obrid == NULL){
            $obrid = $this->obrid;
            if($obrid == NULL){
                return array();
            }
        }
        
        $servicos_externos = array();
        
        //Formato array
        $arr_servicos_externos['item']           = '';
        $arr_servicos_externos['valor_unitario'] = '';
        $arr_servicos_externos['quantidade']     = '';
        $arr_servicos_externos['unidade_medida'] = '';
        $arr_servicos_externos['valor_total']    = '';
        
        $obra = new Obras($obrid);
        $end  = $obra->getEnderecoObra($obra->obrid);
        
        if(empty($end)){
            return array();
        }
        
        $cronogramaPadrao = new Cronograma_PadraoMi();
        $cronograma       = $cronogramaPadrao->pegaCronogramaPadrao($end['estuf'], $obra->tpoid);
        
        if(empty($cronograma)){
            return array();
        }

        $icm = new Itens_Composicao_PadraoMi();
        $icplistForaEdificacao = $icm->pegaPorCpm($cronograma['cpmid'], 'F');
        
        if(empty($icplistForaEdificacao)){
            return array();
        }

        $sql = "SELECT
                    umdid as codigo,
                    umdeesc as descricao
                FROM obras2.unidademedida
                ORDER BY umdeesc ASC";

        $unidadesDeMedida = $this->carregar($sql);

        foreach ($icplistForaEdificacao as $icm){
            $qtdItens = new QtdItensComposicaoObraMi();
            $qtdItem  = $qtdItens->pegaPorObridEIcmid($obrid, $icm['icmid']);
            //Item:
            $iC = new ItensComposicao();
            $itens = $iC->listaCombo();
            foreach ($itens as $it) {
                if($it['codigo'] == $icm['itcid']){
                    $arr_servicos_externos['item'] = $it['descricao'];
                }
            }
            //Valor Unitario:
            $icmvalorconvertido = ($icm['icmvalor']) ? number_format($icm['icmvalor'], 2, ',', '.') : '';
            $arr_servicos_externos['valor_unitario'] = $icmvalorconvertido;
            //Quantidade:
            $arr_servicos_externos['quantidade'] = number_format($qtdItem['qioquantidade'], 2, ',', '.');
            //Unidade de Medida
            foreach ($unidadesDeMedida as $udm) {
                if($udm['codigo'] == $icm['umdid']){
                    $arr_servicos_externos['unidade_medida'] = $udm['descricao'];
                }
            }
            //Valor total
            $icm['icmvalortotal'] = $qtdItem['qioquantidade'] * $icm['icmvalor'];
            $icmvalortotalconvertido = ($icm['icmvalortotal']) ? number_format($icm['icmvalortotal'], 2, ',', '.') : '';
            $arr_servicos_externos['valor_total'] = $icmvalortotalconvertido;

            //Concatena no array de serviços externos
            $servicos_externos[] = $arr_servicos_externos;        
        }
        return $servicos_externos;
    }

    public function atualizaContratoObraMi($obrid)
    {
        $sql = "SELECT o.obrid, h.osmid, h.osmdtinicio, h.osmdttermino, oc.ocrid, oc.crtid
            FROM obras2.obras o
              JOIN workflow.documento d ON d.docid = o.docid
              LEFT JOIN obras2.obrascontrato           oc ON oc.obrid   = o.obrid AND oc.ocrstatus = 'A'
              LEFT JOIN (
                     SELECT os.* FROM obras2.ordemservicomi os
                       JOIN workflow.documento d ON d.docid = os.docid
                       JOIN workflow.historicodocumento h ON d.docid = h.docid AND h.aedid = 2079
                     WHERE os.osmstatus = 'A' AND os.tomid = 1) h ON h.obrid = o.obrid
            WHERE o.obridpai IS NULL AND o.obrstatus = 'A' AND o.tpoid IN (104,105) AND d.esdid IN (690, 693)  AND oc.ocrdtinicioexecucao IS NULL AND o.obrid = {$obrid} ";

        $obra = $this->pegaLinha($sql);

        if(!$obra) return false;

        $dadosContrato = array();
        $obrasContrato = new ObrasContrato();
        $objObra = new Obras($obra['obrid']);

        // Endereçp
        $end = $objObra->getEnderecoObra($objObra->obrid);

        // OS Execução
        $ordemservico = new OrdemServicoMI();
        $os = current($ordemservico->recuperarTodos("*", array("obrid = ".$objObra->obrid, "tomid = 1" ) ));

        // Cronograma
        $cronogramaPadrao = new Cronograma_PadraoMi();
        $cronograma = $cronogramaPadrao->pegaCronogramaPadrao($end['estuf'], $objObra->tpoid);

        // Empresa
        $sql = "SELECT
                           *
                        FROM obras2.empresami em
                        JOIN obras2.empresami_uf euf ON euf.emiid = em.emiid AND euf.eufstatus = 'A'
                        WHERE emistatus = 'A' AND euf.estuf = '{$end['estuf']}'";
        $empresa = $this->pegaLinha($sql);

        if (!empty($obra['ocrid'])) {
            $obrasContrato = new ObrasContrato($obra['ocrid']);
            $dadosContrato = $obrasContrato->getDados();
        } else {
            // Cria contrato
            $contrato = new Contrato();

            $licitacao = new Licitacao();
            $licitacao->popularDadosObjeto(array('orgid' => 3));
            $licitacao->salvar();

            $obralic = new ObraLicitacao();
            $obralic->popularDadosObjeto(array('obrid' => $obra['obrid'], 'licid' => $licitacao->licid));
            $obralic->salvar();

            $dados = array(
                'orgid' => 3,
                'crtvalorexecucao' => $cronograma['cpmvalor'],
                'crtprazovigencia' => $os['osmprazo'],
                'crtdttermino' => $os['osmdttermino'],
                'licid' => $licitacao->licid,
                'mlid' => 2
            );
            $contrato->popularDadosObjeto($dados);
            $contrato->salvar();
            $contrato->commit();
            $dadosContrato['crtid'] = $contrato->crtid;
        }

        $dadosContrato['ocrid'] = $obra['ocrid'];
        $dadosContrato['obrid'] = $obra['obrid'];
        $dadosContrato['ocrdtinicioexecucao'] = $os['osmdtinicio'];
        $dadosContrato['ocrdtterminoexecucao'] = $os['osmdttermino'];
        $dadosContrato['ocrprazoexecucao'] = $os['osmprazo'];
        $dadosContrato['ocrvalorexecucao'] = $cronograma['cpmvalor'];
        $dadosContrato['ocraditivado'] = 'f';

        $obrasContrato->popularDadosObjeto($dadosContrato);
        $obrasContrato->salvar();
        $obrasContrato->commit();
    }

}