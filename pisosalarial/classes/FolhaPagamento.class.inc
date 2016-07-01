<?php
/**
 * Modelo de Folha de Pagamento
 * @author Wesley Romualdo da Silva
 */
class FolhaPagamento extends Modelo{

    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pisosalarial.folhapagamento";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "flpid" );

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'flpid' => null,
									  	'muncod' => null,
									  	'flpanoreferencia' => null,
									  	'flpmesreferencia' => null,
									  	'flpfinalizado' => null
									  );

    public function recuperarFolhaPagamento($post, $pegaum = false)
    {
        extract($post);

        $stWhere = "";
        if(!empty($flpid)){
          $stWhere .= " AND f.flpid = {$flpid} ";
        }
        if(!empty($pmuid)){
          $stWhere .= " AND f.pmuid = '{$pmuid}' ";
        }
        if(!empty($flpanoreferencia)){
          $stWhere .= " AND f.flpanoreferencia = '{$flpanoreferencia}' ";
        }
        if(!empty($flpmesreferencia)){
          $stWhere .= " AND f.flpmesreferencia = '{$flpmesreferencia}' ";
        }
        if(!empty($flpfinalizado)){
          $stWhere .= " AND f.flpfinalizado = '{$flpfinalizado}' ";
        }
        if(!empty($arpid)){
          $stWhere .= " AND f.arpid = {$arpid} ";
        }

        $sql = "SELECT
                    *
                FROM {$this->stNomeTabela} f
                INNER JOIN pisosalarial.pisomunicipio pm ON pm.pmuid = f.pmuid
                WHERE f.pmuid IS NOT NULL
                AND pm.muncod = '{$muncod}'
                {$stWhere}";

        if($pegaum){
            return $this->pegaUm($sql);
        }else{
            return $this->carregar($sql);
        }
    }


    public function recuperarFolhasPagamentosMeses($post)
    {
        if(is_array($post)){
            extract($post);
        }

        $stWhere = "";
        if(!empty($flpanoreferencia)){
            $stWhere .= " AND f.flpanoreferencia = '{$flpanoreferencia}'";
        }
        if(!empty($muncod)){
            $stWhere .= " AND pi.muncod = '{$muncod}'";
        }

        $sql = "SELECT
                    ltrim(m.mescod, '0') as mescod,
                    mesdsc,
                    flpid,
                    muncod,
                    flpanoreferencia,
                    flpmesreferencia,
                    flpfinalizado
                FROM public.meses m
                LEFT JOIN {$this->stNomeTabela} f ON f.flpmesreferencia = m.mescod::integer
                LEFT JOIN pisosalarial.pisomunicipio pi ON pi.pmuid = f.pmuid
                {$stWhere}
                ";

        return $this->carregar($sql);
    }

    public function gerarFolhaPagamento($post)
    {
        extract($post);
        $sql = "SELECT pmuid FROM pisosalarial.pisomunicipio WHERE muncod = '{$post['muncod']}'";
        $pmuid = $this->pegaUm($sql);

        if($pmuid){

            $post['pmuid'] = $pmuid;
            $arFolhaPagamento = self::recuperarFolhaPagamento($post);

            if(!$arFolhaPagamento){
                $sql = "INSERT INTO {$this->stNomeTabela}
                            (flpanoreferencia, flpmesreferencia, flpfinalizado, pmuid)
                        VALUES
                            ('{$flpanoreferencia}', '{$flpmesreferencia}', 'f', {$pmuid})
                        RETURNING flpid";

                $flpid =  $this->pegaUm($sql);
                $this->commit();
                return $flpid;
            }
            return $arFolhaPagamento[0]['flpid'];

        }else{

            $sql = "INSERT INTO pisosalarial.pisomunicipio
                    (muncod)
                    VALUES
                    ('{$muncod}')";

            $this->executar($sql);
            $this->commit();
            self::gerarFolhaPagamento($post);
        }
    }

    public function verificaFolhaFinalizada($muncod, $ano, $mes)
    {
        $sql = "SELECT
                    flpfinalizado
                FROM {$this->stNomeTabela} f
                INNER JOIN pisosalarial.pisomunicipio pi ON pi.pmuid = f.pmuid
                WHERE f.flpanoreferencia = '{$ano}'
                AND f.flpmesreferencia = '{$mes}'
                AND pi.muncod = '{$muncod}'";

        if($this->pegaUm($sql) == 'f'){
            return true;
        }
        return false;
    }

    public function recuperarArquivosImportados()
    {
        $sql = "SELECT
                    a.arpdata
                FROM pisosalarial.folhapagamento p
                INNER JOIN pisosalarial.arquivopagamento a ON p.arpid = a.arpid
                INNER JOIN pisosalarial.pisomunicipio pi ON pi.pmuid = f.pmuid
                WHERE pi.muncod = '{$_SESSION['piso']['muncod']}'";

//        $rs = $this->carregar($sql);
        $cabecalho = array("Visualizar Arquivo", "Data de Importação");
        $this->monta_lista($sql,$cabecalho,100,50,false,"center");
    }

    /**
     * Lista os arquivos que já foram importados
     * @name listaArquivosImportados
     * @author Wesley Romualdo da Silva
     * @access public
     * @return void
     */
    public function listaArquivosImportados(){
        $stWhere = "";
        if(isset($_REQUEST['anoref']) && isset($_REQUEST['mesref'])){
            $stWhere = "AND to_char(ap.arpdata, 'MM') = '".(sprintf('%02s',$_REQUEST['mes'])."' AND to_char(ap.arpdata, 'YYYY') = '".$_REQUEST['ano'])."'";
            $stWhere = "AND fp.flpanoreferencia = '{$_REQUEST['anoref']}' AND fp.flpmesreferencia = '{$_REQUEST['mesref']}'";
        }
        
        //<img id=\"visualizarArquivo-' || m.fmrmescod || '-' || a.farano || '\" src=\"../imagens/excel.gif\" title=\"Importar arquivo excel\" class=\"acao_folha_pagamento\" />
        $sql = "SELECT
                    '<center>
                        <a style=\"cursor:pointer;\" onclick=\"download_file('||ap.arqid||')\">
                            <img src=\"/imagens/excel.gif \" border=0 title=\"Visualizar xls\">
                        </a>
                     </center>' as visualiza,
                    '<center>
                        <a style=\"cursor:pointer;\" onclick=\"view_log('||ap.arqid||')\">
                            <img src=\"/imagens/consultar.gif \" border=0 title=\"Visualizar log\">
                        </a>
                     </center>' as log,
        
                  --(SELECT flpmesreferencia || '/' || flpanoreferencia FROM pisosalarial.folhapagamento WHERE arpid = ap.arpid limit 1) as flpmesano,
                  to_char(ap.arpdata, 'DD/MM/YYYY HH24:MI:SS') as arpdata
                FROM pisosalarial.arquivopagamento ap
                INNER JOIN pisosalarial.folhapagamentoarquivo fpa ON fpa.arpid=ap.arpid
                INNER JOIN pisosalarial.folhapagamento fp ON fp.flpid = fpa.flpid
                INNER JOIN pisosalarial.pisomunicipio pi ON pi.pmuid = fp.pmuid
                WHERE arqid in (SELECT arqid FROM {$this->stNomeTabela} WHERE arqid = ap.arqid)
                AND pi.muncod = '{$_SESSION['piso']['muncod']}'
                {$stWhere}";

#                echo "<pre>";exit($sql);

        $cabecalho = array("Visualizar Arquivos", "Visualizar Log", "Data de Importação");
        $this->monta_lista($sql,$cabecalho,100,50,false,"center");
    }

    public function recuperarListaFolhasPagamento()
    {
        $esdid = pegarEstadoAtual($_SESSION['piso']['muncod']);

        if(possuiPerfil(array(PERFIL_CADASTRO_MUNICIPAL)) && !in_array($esdid, array(WF_APROVADO))){
            $stAcoes = "case when f.flpfinalizado = 't' then
                        '
                        <img id=\"listaProfissionais-' || m.fmrmescod || '-' || a.farano || '\" src=\"../imagens/preview.gif\" title=\"Visualizar profissionais\" class=\"acao_folha_pagamento\" />
                        '
                        ||
                        (case when (select
                                       count(*)
                                   from pisosalarial.folhapagamento ff
                                   inner join pisosalarial.pisomunicipio pii on pii.pmuid = ff.pmuid
                                   inner join pisosalarial.folhapagamentoprofissionais pp on ff.flpid = pp.flpid
                                   where ff.flpmesreferencia = m.fmrmescod
                                   and ff.flpanoreferencia = a.farano
                                   and pii.muncod = '{$_SESSION['piso']['muncod']}') > 0
                             then '<img id=\"duplicarMes-' || m.fmrmescod || '-' || a.farano || '\" src=\"../imagens/grupo.gif\" title=\"Duplicar este mês\" class=\"acao_folha_pagamento\" />'
                        else '' end)
                        ||
                        '
                        <img id=\"abrirFolha-' || m.fmrmescod || '-' || a.farano || '\" src=\"../imagens/cadiado.png\" title=\"Abrir folha de pagamento\" class=\"acao_folha_pagamento\" height=\"15\"/>
                        '
                        else
                        '
                        <img id=\"listaProfissionais-' || m.fmrmescod || '-' || a.farano || '\" src=\"../imagens/preview.gif\" title=\"Visualizar profissionais\" class=\"acao_folha_pagamento\" />
                        '
                        ||
                        (case when (select
                                       count(*)
                                   from pisosalarial.folhapagamento ff
                                   inner join pisosalarial.folhapagamentoprofissionais pp on ff.flpid = pp.flpid
                                   inner join pisosalarial.pisomunicipio pii on pii.pmuid = ff.pmuid
                                   where ff.flpmesreferencia = m.fmrmescod
                                   and ff.flpanoreferencia = a.farano
                                   and pii.muncod = '{$_SESSION['piso']['muncod']}') > 0
                             then '<img id=\"duplicarMes-' || m.fmrmescod || '-' || a.farano || '\" src=\"../imagens/grupo.gif\" title=\"Duplicar este mês\" class=\"acao_folha_pagamento\" />'
                        else '' end)
                        ||
                        '
                        <img id=\"formProfissional-' || m.fmrmescod || '-' || a.farano || '\" src=\"../imagens/gif_inclui.gif\" title=\"Incluir profissional\" class=\"acao_folha_pagamento\" />
                        <img id=\"importarArquivo-' || m.fmrmescod || '-' || a.farano || '\" src=\"../imagens/excel.gif\" title=\"Importar arquivo excel\" class=\"acao_folha_pagamento\" />
                        <img id=\"fecharFolha-' || m.fmrmescod || '-' || a.farano || '\" src=\"../imagens/cadeadoAberto.png\" title=\"Fechar folha de pagamento\" class=\"acao_folha_pagamento\" height=\"15\"/>
                        '
                        end as acoes,";
        }else{
            $stAcoes = "'<img id=\"listaProfissionais-' || m.fmrmescod || '-' || a.farano || '\" src=\"../imagens/preview.gif\" title=\"Visualizar\" class=\"acao_folha_pagamento\" />' as acoes,";
        }

        $sql = "select
                    {$stAcoes}
                    farano,
                    fmrmesdsc,
                    (select count(*) from pisosalarial.folhapagamentoprofissionais p where p.flpid = f.flpid) as profissionais,
                    coalesce((select sum(fppsalbase+fppadigrat) as total from pisosalarial.folhapagamentoprofissionais p where p.flpid = f.flpid),0) as totalfolha,
                    case when f.flpfinalizado = 't' then '<center><img src=\"../imagens/valida1.gif\" align=\"absmiddle\"/>&nbsp;Finalizada &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</center>'
                    else '<center><img src=\"../imagens/valida3.gif\" align=\"absmiddle\"/>&nbsp;Não finalizada</center>' end as situacao
                from pisosalarial.folhaanoreferencia  a
                inner join pisosalarial.folhamesreferencia m on a.farid = m.farid
                left join (SELECT * FROM pisosalarial.folhapagamento ff
                           inner join pisosalarial.pisomunicipio pi on pi.pmuid = ff.pmuid
                           where pi.muncod = '{$_SESSION['piso']['muncod']}'
                           ) f ON f.flpanoreferencia = a.farano and f.flpmesreferencia = m.fmrmescod

                order by farano, fmrmescod";

                    /*
                     * --pisosalarial.folhapagamento f on f.flpanoreferencia = a.farano and f.flpmesreferencia = m.fmrmescod
                --left join pisosalarial.pisomunicipio pi on pi.pmuid = f.pmuid and pi.muncod = '{$_SESSION['piso']['muncod']}'
                     */

        $cabecalho = array("Ações", "Ano de referência", "Mes de referência", "Profissionais", "Valor total", "Situação");
        return $this->monta_lista($sql,$cabecalho,50,10,false,"center");
    }
}
?>