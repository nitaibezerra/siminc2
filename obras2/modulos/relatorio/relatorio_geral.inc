<?php

function retornaSQL($nomeSQL, $parametros = array()){
    $sql = '';
    
    switch ($nomeSQL) {
        case 'ConsultaPublica':
            $sql = sprintf("UPDATE public.parametros_tela 
                            SET prtpublico = case when prtpublico = true 
                                                  then false 
                                                  else true 
                                                  end 
                            WHERE prtid = %d", $_REQUEST['prtid']);
            break;
        case 'DeletaConsultaPublica':
            $sql = sprintf( " DELETE FROM public.parametros_tela 
                              WHERE prtid = %d", $_REQUEST['prtid'] );
            break;
        // Verificar montagem do WF e da clausula WHERE
        case 'EstadoWorkflow':
            $where = $parametros['where'];
            $sql = "  SELECT
                        staid AS codigo,
                        stadesc AS descricao
                      FROM 
                        --obras.situacaoobra
                        obras2.situacaoatividade
                      WHERE
                        stastatus = 'A'
                        {$where}
                      ORDER BY
                        stadesc";
            break;
        case 'Programa':
            $sql = "  SELECT
                            prfid AS codigo,
                            prfdesc AS descricao
                      FROM 
                            obras2.programafonte
                            " . ( !empty($_POST['orgid']) ? 'WHERE orgid IN (' . $_POST['orgid'] . ')' : '' ) . "
                      ORDER BY
                            prfdesc ";
            break;
        case 'ExibeConsulta':
            $sql = sprintf(" SELECT prtobj 
                              FROM public.parametros_tela 
                              WHERE prtid = %d" , $_REQUEST['prtid'] );
            break;
        case 'CarregaConsultaBanco':
            $sql = sprintf(" SELECT prtobj 
                             FROM public.parametros_tela 
                             WHERE prtid = %d" , $_REQUEST['prtid']);
            break;
        case 'TipoEnsino':
            $sql = "SELECT orgid, orgdesc FROM obras2.orgao ORDER BY orgid";
            break;
        case 'ListaRelatorios':
            $bt_publicar = $parametros['btn'];
            $sql = sprintf(
                            "SELECT Case when prtpublico = true and usucpf = '%s' 
                                         then '{$bt_publicar}<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/salvar.png\" title=\" Download consulta \" onclick=\"carregar_relatorio(' || prtid || ', true)\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                         else '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/salvar.png\" title=\" Download consulta \" onclick=\"carregar_relatorio(' || prtid || ', true)\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                         end as acao, 
                                         '' || prtdsc || '' as descricao 
                             FROM public.parametros_tela 
                             WHERE mnuid = %d 
                             AND prtpublico = TRUE", 
                             $_SESSION['usucpf'], $_SESSION['mnuid'], $_SESSION['usucpf']
                           );
            break;
        case 'MinhasConsultas':
            $sql = sprintf(
                            "SELECT 
                                    CASE WHEN prtpublico = false THEN '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
                                                                       <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
                                                                       <img border=\"0\" src=\"../imagens/salvar.png\" title=\" Download consulta \" onclick=\"carregar_relatorio(' || prtid || ', true)\">&nbsp;&nbsp;
                                                                       <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                                                 ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
                                                                      <img border=\"0\" src=\"../imagens/salvar.png\" title=\" Download consulta \" onclick=\"carregar_relatorio(' || prtid || ', true)\">&nbsp;&nbsp;
                                                                       <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                    END as acao, 
                                    '' || prtdsc || '' as descricao 
                             FROM 
                                    public.parametros_tela 
                             WHERE 
                                    mnuid = %d AND usucpf = '%s'", 
                            $_SESSION['mnuid'], 
                            $_SESSION['usucpf']
                           );
            break;
        /**
         * Querys para as combos de filtro de consulta
         */
        // ok
        case 'Regiao':
            $sql = ' SELECT
                        regcod AS codigo,
                        regdescricao AS descricao
                     FROM 
                        territorios.regiao
                     ORDER BY
                        regdescricao ';
            break;
        // ok
        case 'MesoRegiao':
            $sql = '  SELECT
                        mescod AS codigo,
                        mesdsc AS descricao
                      FROM 
                        territorios.mesoregiao
                      ORDER BY
                        mesdsc ';
            break;
        // ok
        case 'MicroRegiao':
            $sql = ' SELECT
                        miccod AS codigo,
                        micdsc AS descricao
                     FROM 
                        territorios.microregiao
                     ORDER BY
                        micdsc';
            break;
        // ok
        case 'UF':
            $sql = 'SELECT
                        estuf AS codigo,
                        estdescricao AS descricao
                    FROM 
                        territorios.estado
                    ORDER BY
                        estdescricao';
            break;
        // ok
        case 'GrupoMunicipio':
            $sql = 'SELECT
                        gtmid as codigo,
                        gtmdsc as descricao
                    FROM
                        territorios.grupotipomunicipio
                    ORDER BY
                        gtmdsc';
            break;
        // ok
        case 'TipoMunicipio':
            $sql = 'SELECT
                        tpmid AS codigo,
                        tpmdsc AS descricao
                    FROM 
                        territorios.tipomunicipio
                    ORDER BY
                        tpmdsc';
            break;
        // ok
        case 'Municipio':
            $sql = "SELECT
                        tm.muncod AS codigo,
                        tm.estuf || ' - ' || tm.mundescricao AS descricao
                    FROM 
                        territorios.municipio tm
                    INNER JOIN
                        entidade.endereco ee 
                        ON ee.muncod = tm.muncod
                    --INNER JOIN obras.obrainfraestrutura oi  ON oi.endid = ee.endid
                    INNER JOIN obras2.obras oi  ON oi.endid = ee.endid
                    WHERE
                        oi.obrstatus = 'A'
                    ORDER BY
                        mundescricao";
            break;
        // Verificar diferença, pois em obras retorna 25243 registros e em obras2 retorna 0 com 21690
        // + ou - OK
        case 'Unidades':
            $sql = "SELECT
                        ee.entid AS codigo,
                        ee.entnome AS descricao
                    FROM 
                        entidade.entidade ee
                    INNER JOIN
                        obras2.obras oi 
                    ON ee.entid = oi.entid
                    WHERE
                        oi.obrstatus = 'A'
                    ORDER BY
                        entnome";
            break;
        // Verificar diferença, pois em obras retorna 8096 registros e em obras2 retorna 0 com ef.funid in(17,18) 
        case 'Campus':
            $sql = "SELECT 
                        ee.entid AS codigo, 
                        ee.entnome AS descricao 
                    FROM 
                        entidade.entidade ee
                    INNER JOIN
                        obras.obrainfraestrutura oi 
                     ON ee.entid = oi.entidcampus
                    INNER JOIN
                        entidade.funcaoentidade ef 
                     ON ef.entid = ee.entid 
                    WHERE ef.funid in(17,18) 
                    AND   oi.obsstatus = 'A'";
            break;
        // ok
        case 'Programa':
            $sql = 'SELECT
                        prfid AS codigo,
                        prfdesc AS descricao
                    FROM 
                        obras2.programafonte
                    ORDER BY
                        prfdesc';
            break;
        // ok
        case 'Fonte':
            $sql = "SELECT
                        tooid AS codigo,
                        toodescricao AS descricao
                    FROM 
                        obras2.tipoorigemobra
                    WHERE
                        toostatus = 'A'
                    ORDER BY
                        toodescricao";
            break;
        // ok
        case 'TipologiaObra':
            $sql = 'SELECT
                        tpoid AS codigo,
                        tpodsc AS descricao
                    FROM 
                        obras2.tipologiaobra
                    ORDER BY
                        tpodsc';
            break;
        // ok
        case 'ClassificacaoObra':
            $sql = 'SELECT
                        cloid AS codigo,
                        clodsc AS descricao
                    FROM 
                        obras2.classificacaoobra
                    ORDER BY
                        clodsc';
            break;
        // OK
        case 'SituacaoObra':

            $sql = "SELECT esdid  as codigo, 
                           esddsc as descricao 
                    FROM workflow.estadodocumento 
                    WHERE tpdid='" . TPDID_OBJETO . "' 
                    AND esdstatus='A' ORDER BY esdordem";
            
/*/*            $stWhere = " AND stoid not in (11) ";
//            if ($_SESSION['obras']['orgid'] == ORGAO_FNDE) {
//                $stWhere = " AND stoid not in (6,7,11) ";
//            }
//            $sql = " SELECT
//                            stoid AS codigo,
//                            stodesc AS descricao
//                       FROM 
//                            obras.situacaoobra
//                       WHERE
//                            stostatus = 'A'
//                       {$stWhere}
//                       ORDER BY
//                            stodesc ";*/
            break;
        // ok
        case 'ResponsavelUltimaVistoria':
            $sql = 'SELECT rsuid as codigo, 
                           rsudsc as descricao 
                    FROM  obras2.realizacaosupervisao';
            break;
        
        default:
            break;
    }
    return $sql;
}

function retornaCombo($nomeCombo){
    
    $combo           = '';
    $stSqlCarregados = '';
    
    switch ($nomeCombo) {
        case 'Regiao':
            $sql = retornaSQL('Regiao');
            mostrarComboPopup('Regiões', 'regiao', $sql, $stSqlCarregados, 'Selecione a(s) Regiões(s)');
            break;
        case 'MesoRegiao':
            $sql = retornaSQL('MesoRegiao');
            mostrarComboPopup('Mesorregião', 'mesoregiao', $sql, $stSqlCarregados, 'Selecione a(s) Mesorregião(ões)');
            break;
        case 'MicroRegiao':
            $sql = retornaSQL('MicroRegiao');
            mostrarComboPopup('Microrregião', 'microregiao', $sql, $stSqlCarregados, 'Selecione a(s) Microrregião(ões)');
            break;
        case 'UF':
            $sql = retornaSQL('UF');
            mostrarComboPopup('UF', 'uf', $sql, $stSqlCarregados, 'Selecione a(s) UF(s)');
            break;
        case 'GrupoMunicipio':
            $sql = retornaSQL('GrupoMunicipio');
            mostrarComboPopup('Grupo de Municipio', 'grupomun', $sql, $stSqlCarregados, 'Selecione o(s) Grupo(s) de Município(s)');
            break;
        case 'TipoMunicipio':
            $sql = retornaSQL('TipoMunicipio');
            mostrarComboPopup('Tipo de Município', 'tipomun', $sql, $stSqlCarregados, 'Selecione o(s) tipo(s) de Município(s)');
            break;
        case 'Municipio':
            $sql = retornaSQL('Municipio');
            mostrarComboPopup('Município', 'municipio', $sql, $stSqlCarregados, 'Selecione o(s) Município(s)');
            break;
        case 'Unidades':
            $sql = retornaSQL('Unidades');
            mostrarComboPopup('Unidades', 'unidade', $sql, $stSqlCarregados, 'Selecione o(s) Unidades(s)');
            break;
        case 'Campus':
            $sql = retornaSQL('Campus');
            mostrarComboPopup('Campus', 'campus', $sql, $stSqlCarregados, 'Selecione a(s) Unidade(s)');
            break;
        case 'Programa':
            $sql = retornaSQL('Programa');
            mostrarComboPopup('Programa', 'prfid', $sql, $stSqlCarregados, 'Selecione o(s) Programa(s)');
            break;
        case 'Fonte':
            $sql = retornaSQL('Fonte');
            mostrarComboPopup('Fonte', 'tooid', $sql, $stSqlCarregados, 'Selecione a(s) Fonte(s)');
            break;
        case 'TipologiaObra':
            $sql = retornaSQL('TipologiaObra');
            mostrarComboPopup('Tipologia da Obra', 'tpoid', $sql, $stSqlCarregados, 'Selecione a(s) Tipologia(a) da(s) Obra(s)');
            break;
        case 'ClassificacaoObra':
            $sql = retornaSQL('ClassificacaoObra');
            mostrarComboPopup('Classificação da Obra', 'cloid', $sql, $stSqlCarregados, 'Selecione a(s) Classificação(ões) da(s) Obra(s)');
            break;
        case 'SituacaoObra':
            $sql = retornaSQL('SituacaoObra');
            mostrarComboPopup('Situação da Obra', 'stoid', $sql, $stSqlCarregados, 'Selecione a(s) Situação(ões) da(s) Obra(s)');
            break;
        default:
            break;
    }
    
    return $combo;
    
}

function exibeAgrupador($nomeAgrupador, $nomeForm){
    // Início dos agrupadores
    $agrupador = new Agrupador($nomeForm, '');
    // Dados padrão de destino (nulo)
    $destino = isset($agrupador2) ? $agrupador2 : array();
    
    switch ($nomeAgrupador) {
        case 'colunas':
            $origem = array(
                    'avanco'                  => array( 'codigo' => 'avanco',                  'descricao' => 'Avanço da Obra' ),
                    'pronumeroprocesso'      => array( 'codigo' => 'pronumeroprocesso',      'descricao' => 'Processo' ),
                    'termo_convenio'             => array( 'codigo' => 'termo_convenio',             'descricao' => 'Número do Convênio/Termo' ),
                    'tipoaquisicao'           => array( 'codigo' => 'tipoaquisicao',           'descricao' => 'Tipo de aquisição' ),
                    'metragem'                => array( 'codigo' => 'metragem',                'descricao' => 'Área/Quantidade a ser Construída' ),
                    'situacaoobra'            => array( 'codigo' => 'situacaoobra',            'descricao' => 'Situação da obra' ),
                    'dataparalisado'          => array( 'codigo' => 'dataparalisado',          'descricao' => 'Data da ùltima Paralisação' ),
                    'unidade'                 => array( 'codigo' => 'unidade',                 'descricao' => 'Unidade' ),
                    'campus'                  => array( 'codigo' => 'campus',                  'descricao' => 'Campus'  ),
                    'municipio'               => array( 'codigo' => 'municipio',               'descricao' => 'Município' ),
                    'mesoregiao'              => array( 'codigo' => 'mesoregiao',              'descricao' => 'Mesorregião' ),
                    'microregiao'             => array( 'codigo' => 'microregiao',             'descricao' => 'Microrregião' ),
                    'estado'                  => array( 'codigo' => 'estado',                  'descricao' => 'UF' ),
                    'ultatualizacao'          => array( 'codigo' => 'ultatualizacao',          'descricao' => 'Última atualização' ),
                    'qtdvistorias'            => array( 'codigo' => 'qtdvistorias',            'descricao' => 'Qtd de vistorias' ),
                    'porcexecucao'            => array( 'codigo' => 'porcexecucao',            'descricao' => '% de execução' ),
                    'qtdrestricoes'           => array( 'codigo' => 'qtdrestricoes',           'descricao' => 'Restrições' ),
                    'cnpjempresacontratada'   => array( 'codigo' => 'cnpjempresacontratada',   'descricao' => 'CNPJ Empresa contratada' ),
                    'nomeempresacontratada'   => array( 'codigo' => 'nomeempresacontratada',   'descricao' => 'Nome Empresa contratada' ),
                    'tipoobra'                => array( 'codigo' => 'tipoobra',                'descricao' => 'Tipo da obra' ),
                    'obrdtfim'                => array( 'codigo' => 'obrdtfim',                'descricao' => 'Data de Conclusão da Obra (contrato)' ),
                    'obrdtfimvistoria'        => array( 'codigo' => 'obrdtfimvistoria',        'descricao' => 'Data de Conclusão da Obra' ),
                    'subacao'                 => array( 'codigo' => 'subacao',                 'descricao' => 'Programa' ),
                    'fonte'                   => array( 'codigo' => 'fonte',                   'descricao' => 'Fonte' ),
                    'obrvalorprevisto'        => array( 'codigo' => 'obrvalorprevisto',        'descricao' => 'Valor Previsto (R$):' ),
                    'obrcustocontrato'        => array( 'codigo' => 'obrcustocontrato',        'descricao' => 'Valor Contratado da Obra (R$):', 'type' => "string" ),
                    'moldsc'                  => array( 'codigo' => 'moldsc',                  'descricao' => 'Modalidade de Licitação' ),
                    'licitacaouasg'           => array( 'codigo' => 'licitacaouasg',           'descricao' => 'Número da UASG' ),
                    'numlicitacao'            => array( 'codigo' => 'numlicitacao',            'descricao' => 'Número da Licitação' ),
                    'obrdtassinaturacontrato' => array( 'codigo' => 'obrdtassinaturacontrato', 'descricao' => 'Data de Assinatura do Contrato' ),
                    'dtterminocontrato'       => array( 'codigo' => 'dtterminocontrato',       'descricao' => 'Data de Término do Contrato' ),
                    'obrdtordemservico'       => array( 'codigo' => 'obrdtordemservico',       'descricao' => 'Data de Ordem de Serviço' ),
                    'obrdtinicio'             => array( 'codigo' => 'obrdtinicio',             'descricao' => 'Início de Execução da Obra' ),
                    'obrdttermino'            => array( 'codigo' => 'obrdttermino',            'descricao' => 'Término de Execução da Obra' ),
//                    'frpdesc'                 => array( 'codigo' => 'frpdesc',                 'descricao' => 'Tipo (Origem dos Recursos)' ),
                    'obridid'                 => array( 'codigo' => 'obridid',                 'descricao' => 'ID' ),
                    'preid'                   => array( 'codigo' => 'preid',                   'descricao' => 'ID Pré-Obra' ),
                    'dtpublicacao'            => array( 'codigo' => 'dtpublicacao',            'descricao' => 'Publicação do Edital' ),
                    'dthomologacao'           => array( 'codigo' => 'dthomologacao',           'descricao' => 'Homologação do Edital' ),
                    'orgao'                   => array( 'codigo' => 'orgao',                   'descricao' => 'Órgão' ),
                    'ano_termo_convenio'                  => array( 'codigo' => 'ano_termo_convenio',                  'descricao' => 'Ano do Convênio/Termo' ),
                    'dt_inicio_conv'          => array( 'codigo' => 'dt_inicio_conv',          'descricao' => 'Data de inicio do convênio' ),
                    'dt_final_conv'           => array( 'codigo' => 'dt_final_conv',           'descricao' => 'Data de fim do convênio' ),
                    'vlrliquidado'            => array( 'codigo' => 'vlrliquidado',            'descricao' => 'Exec. Orc. - Valor Liquidado' ),
                    'coordenadas_geograficas' => array( 'codigo' => 'coordenadas_geograficas', 'descricao' => 'Coordenadas Geográficas' ),
                    'muncod1'                 => array( 'codigo' => 'muncod1',                 'descricao' => 'Código do IBGE' ),
                    'cep'                     => array( 'codigo' => 'cep',                     'descricao' => 'CEP' ),
                    'endlog'                  => array( 'codigo' => 'endlog',                  'descricao' => 'Endereço - Logradouro' ),
                    'endcom'                  => array( 'codigo' => 'endcom',                  'descricao' => 'Endereço - Complemento' ),
                    'endbai'                  => array( 'codigo' => 'endbai',                  'descricao' => 'Endereço - Bairro' ),
                    'endnum'                  => array( 'codigo' => 'endnum',                  'descricao' => 'Endereço - Número' ),
                    'endcomunidade'           => array( 'codigo' => 'endcomunidade',           'descricao' => 'Endereço - Comunidade' ),
                    'obrsupervisoes'          => array( 'codigo' => 'obrsupervisoes',          'descricao' => 'Nº de Sup.Empresa' ),
//                    'termocompromisso'        => array( 'codigo' => 'termocompromisso',        'descricao' => 'Nº do Termo de Compromisso' ),
                    'unidade_endcep'          => array( 'codigo' => 'unidade_endcep',          'descricao' => 'CEP (Unidade)' ),
                    'unidade_endlog'          => array( 'codigo' => 'unidade_endlog',          'descricao' => 'Endereço - Logradouro (Unidade)' ),
                    'unidade_endcom'          => array( 'codigo' => 'unidade_endcom',          'descricao' => 'Endereço - Complemento (Unidade)' ),
                    'unidade_endbai'          => array( 'codigo' => 'unidade_endbai',          'descricao' => 'Endereço - Bairro (Unidade)' ),
                    'unidade_endnum'          => array( 'codigo' => 'unidade_endnum',          'descricao' => 'Endereço - Número (Unidade)' ),
                    'unidade_endcomunidade'   => array( 'codigo' => 'unidade_endcomunidade',   'descricao' => 'Endereço - Comunidade (Unidade)' ),
                    'unidade_email'           => array( 'codigo' => 'unidade_email',           'descricao' => 'Email (Unidade)' ),
                    'unidade_numcomercial'    => array( 'codigo' => 'unidade_numcomercial',    'descricao' => 'N° de Telefone Comercial (Unidade)' ),
                    'tipologia'               => array( 'codigo' => 'tipologia',               'descricao' => 'Tipologia' ),
                    'dqtdatingido'            => array( 'codigo' => 'dqtdatingido',            'descricao' => 'Qtd. dias % atingido' ),
                    'medidasexcecao'          => array( 'codigo' => 'medidasexcecao',          'descricao' => 'Medidas de Exceção' ),
                    'stiid'                   => array( 'codigo' => 'stiid',                   'descricao' => 'Situação do Instrumento' ),
                    'frpid'                   => array( 'codigo' => 'frpid',                   'descricao' => 'Tipo do Instrumento' ),
                    'qtdsupervisaoempresa'    => array( 'codigo' => 'qtdsupervisaoempresa',    'descricao' => 'Qtd. Supervisão Empresa' ),
                
                
                
                );
            $agrupador->setOrigem('naoColunas', null, $origem);
            $agrupador->setDestino('colunas', null, $destino);
            $agrupador->exibir();
            break;
        
        default:
            break;
    }
}

function montaEstadoWorkflow($dados) {
    // Situação da Obra				
    $stWhere = "";
    $orgids = explode(',', $dados['orgids']);
    foreach ($orgids as $orgid) {
        // Verificar como monta o WHERE do WorkFlow
        switch ($orgid) {
            case ORGAO_SESU:
                //$stWhere .= " AND stoedsuperior IS TRUE ";
                $stWhere .= "  ";
                break;
            case ORGAO_SETEC:
                //$stWhere .= " AND stoedprofissional IS TRUE ";
                $stWhere .= "  ";
                break;
            case ORGAO_FNDE:
                //$stWhere .= " AND stoedbasica IS TRUE ";
                $stWhere .= "  ";
                break;
            case ORGAO_ADM:
                //$stWhere .= " AND stoedadm IS TRUE ";
                $stWhere .= "  ";
                break;
            case ORGAO_REHUF:
                //$stWhere .= " AND stoedhospitais IS TRUE ";
                $stWhere .= "  ";
                break;
        }
    }
    //$stSqlCarregados_sto = '';
    $stSql               = retornaSQL('SituacaoObra', array('where'=> $stWhere));
    mostrarComboPopup('Situação da Obra', 'stoid', $stSql, $stSqlCarregados_sto, 'Selecione a(s) Situação(ões) da(s) Obra(s)');
}

if ($_REQUEST['reqAjax']) {
    $_REQUEST['reqAjax']($_REQUEST);
    die();
}

if (isset($_POST['tipo_ensino'])) {
    // Programa 
    $stSql = retornaSQL('Programa');
    mostrarComboPopup('Programa', 'prfid', $stSql, $stSqlCarregados, 'Selecione o(s) Programa(s)');
    die;
}

function obras_monta_coluna_relatorio2() {

    $coluna = array();

    foreach ($_REQUEST['colunas'] as $valor) {

        switch ($valor) {
            case 'obrdtexclusao':
                array_push($coluna, array("campo" => "obrdtexclusao",
                    "label" => "Data da Exclusão",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'usucpfexclusao':
                array_push($coluna, array("campo" => "usucpfexclusao",
                    "label" => "Resposansável pela Exclusão",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'obrobsexclusao':
                array_push($coluna, array("campo" => "obrobsexclusao",
                    "label" => "Observação da Exclusão",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'tipoaquisicao':
                array_push($coluna, array("campo" => "tipoaquisicao",
                    "label" => "Tipo de aquisição",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'metragem':
                array_push($coluna, array("campo" => "metragem",
                    "label" => "Metragem",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'situacaoobra':
                array_push($coluna, array("campo" => "situacaoobra",
                    "label" => "Situação da obra",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'dataparalisado':
                array_push($coluna, array("campo" => "dataparalisado",
                    "label" => "Data da ùltima Paralisação",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'unidade':
                array_push($coluna, array("campo" => "unidade",
                    "label" => "Unidade",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'campus':
                array_push($coluna, array("campo" => "campus",
                    "label" => "Campus",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'municipio':
                array_push($coluna, array("campo" => "municipio",
                    "label" => "Municipio",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'estado':
                array_push($coluna, array("campo" => "estado",
                    "label" => "UF",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'ultatualizacao':
                array_push($coluna, array("campo" => "ultatualizacao",
                    "label" => "Última atualização",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'qtdvistorias':
                array_push($coluna, array("campo" => "qtdvistorias",
                    "label" => "Qtd de vistorias",
                    "blockAgp" => "",
                    "type" => "numeric"));
                break;
            case 'porcexecucao':
                array_push($coluna, array("campo" => "porcexecucao",
                    "label" => "% de execução",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'qtdrestricoes':
                array_push($coluna, array("campo" => "qtdrestricoes",
                    "label" => "Restrições",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'cnpjempresacontratada':
                array_push($coluna, array("campo" => "cnpjempresacontratada",
                    "label" => "CNPJ Empresa contratada",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'nomeempresacontratada':
                array_push($coluna, array("campo" => "nomeempresacontratada",
                    "label" => "Nome Empresa contratada",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'tipoobra':
                array_push($coluna, array("campo" => "tipoobra",
                    "label" => "Tipo da obra",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'subacao':
                array_push($coluna, array("campo" => "subacao",
                    "label" => "Programa",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'fonte':
                array_push($coluna, array("campo" => "fonte",
                    "label" => "Fonte",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'orgao':
                array_push($coluna, array("campo" => "orgao",
                    "label" => "Órgão",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'obrdtfim':
                array_push($coluna, array("campo" => "obrdtfim",
                    "label" => "Data de Conclusão da Obra (contrato)",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'obrdtfimvistoria':
                array_push($coluna, array("campo" => "obrdtfimvistoria",
                    "label" => "Data de Conclusão da Obra",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'obrvalorprevisto':
                if ($_REQUEST['pesquisa'] == '1') {
                    array_push($coluna, array("campo" => "obrvalorprevisto",
                        "label" => "Valor Previsto (R$):",
                        "blockAgp" => "",
                        "html" => "<div style='color:#0066CC; width:100%; text-align:right;'>{obrvalorprevisto}</div>"));
                } else {
                    array_push($coluna, array("campo" => "obrvalorprevisto",
                        "label" => "Valor Previsto (R$):",
                        "blockAgp" => "",
                        "type" => ""));
                }
                break;
            case 'obrcustocontrato':
                if ($_REQUEST['pesquisa'] == '1') {
                    array_push($coluna, array("campo" => "obrcustocontrato",
                        "label" => "Valor Contratado da Obra (R$):",
                        "html" => "<div style='color:#0066CC; width:100%; text-align:right;'>{obrcustocontrato}</div>"));
                } else {
                    array_push($coluna, array("campo" => "obrcustocontrato",
                        "label" => "Valor Contratado da Obra (R$):",
                        "type" => ""));
                }
                break;
            case 'moldsc':
                array_push($coluna, array("campo" => "moldsc",
                    "label" => "Modalidade de Licitação",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'licitacaouasg':
                array_push($coluna, array("campo" => "licitacaouasg",
                    "label" => "Número da UASG",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'numlicitacao':
                array_push($coluna, array("campo" => "numlicitacao",
                    "label" => "Número da Licitação",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'obrdtassinaturacontrato':
                array_push($coluna, array("campo" => "obrdtassinaturacontrato",
                    "label" => "Data de Assinatura do Contrato",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'dtterminocontrato':
                array_push($coluna, array("campo" => "dtterminocontrato",
                    "label" => "Data de Término do Contrato",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'obrdtordemservico':
                array_push($coluna, array("campo" => "obrdtordemservico",
                    "label" => "Data de Ordem de Serviço",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'obrdtinicio':
                array_push($coluna, array("campo" => "obrdtinicio",
                    "label" => "Início de Execução da Obra",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'obrdttermino':
                array_push($coluna, array("campo" => "obrdttermino",
                    "label" => "Término de Execução da Obra",
                    "blockAgp" => "",
                    "type" => ""));
                break;
//            case 'frpdesc':
//                array_push($coluna, array("campo" => "frpdesc",
//                    "label" => "Tipo (Origem dos Recursos)",
//                    "blockAgp" => "",
//                    "type" => ""));
//                break;
            case 'obridid':
                array_push($coluna, array("campo" => "obridid",
                    "label" => "ID",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'preid':
                array_push($coluna, array("campo" => "preid",
                    "label" => "ID Pré-Obra",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'dtpublicacao':
                array_push($coluna, array("campo" => "dtpublicacao",
                    "label" => "Publicação do Edital",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'dthomologacao':
                array_push($coluna, array("campo" => "dthomologacao",
                    "label" => "Homologação da Licitação",
                    "blockAgp" => "",
                    "type" => ""));
                break;
            case 'termo_convenio':
                array_push($coluna, array("campo" => "termo_convenio",
                    "label" => "Número do Convênio/Termo",
                    "blockAgp" => "",
                    "type" => "string"));

                break;
            case 'pronumeroprocesso':
                array_push($coluna, array("campo" => "pronumeroprocesso",
                    "label" => "Processo",
                    "blockAgp" => "",
                    "type" => "string"));

                break;
            case 'avanco':
                array_push($coluna, array("campo" => "avanco",
                    "label" => "Avanço da Obra",
                    "blockAgp" => "",
                    "type" => "string"));

                break;
            case 'ano_termo_convenio':
                array_push($coluna, array("campo" => "ano_termo_convenio",
                    "label" => "Ano do Convênio/Termo",
                    "blockAgp" => "",
                    "type" => "string"));

                break;
            case 'dt_inicio_conv':
                array_push($coluna, array("campo" => "dt_inicio_conv",
                    "label" => "Data de inicio do convênio",
                    "blockAgp" => "",
                    "type" => ""));

                break;
            case 'dt_final_conv':
                array_push($coluna, array("campo" => "dt_final_conv",
                    "label" => "Data de fim do convênio",
                    "blockAgp" => "",
                    "type" => ""));

                break;
            case 'vlrliquidado':
                if ($_REQUEST['pesquisa'] == '1') {
                    array_push($coluna, array("campo" => "vlrliquidado",
                        "label" => "Execução Orçamentária - Valor Liquidado",
                        "blockAgp" => "",
                        "html" => "<div style='color:#0066CC; width:100%; text-align:right;'>{vlrliquidado}</div>"));
                } else {
                    array_push($coluna, array("campo" => "vlrliquidado",
                        "label" => "Execução Orçamentária - Valor Liquidado",
                        "blockAgp" => "",
                        "type" => ""));
                }

                break;
            case 'termocompromisso':
                    array_push($coluna, array("campo" => "termocompromisso",
                        "label" => "Nº do Termo de Compromisso",
                        "blockAgp" => "",
                        "type" => "string"));

                break;
            case 'coordenadas_geograficas':
                array_push($coluna, array("campo" => "coordenadas_geograficas",
                    "label" => "Coordenadas Geográficas",
                    "blockAgp" => "",
                    "type" => ""));

                break;
            case 'muncod1':
                array_push($coluna, array("campo" => "muncod1",
                    "label" => "Código do IBGE",
                    "blockAgp" => "",
                    "type" => "string"));

                break;
            case 'cep':
                array_push($coluna, array("campo" => "cep",
                    "label" => "CEP",
                    "blockAgp" => "",
                    "type" => "cep"));

                break;
            case 'endlog':
                array_push($coluna, array("campo" => "endlog",
                    "label" => "Endereço - Logradouro",
                    "blockAgp" => "",
                    "type" => ""));

                break;
            case 'endcom':
                array_push($coluna, array("campo" => "endcom",
                    "label" => "Endereço - Complemento",
                    "blockAgp" => "",
                    "type" => ""));

                break;
            case 'endnum':
                array_push($coluna, array("campo" => "endnum",
                    "label" => "Endereço - Número",
                    "blockAgp" => "",
                    "type" => "string"));

                break;
            case 'dqtdatingido':
                array_push($coluna, array("campo" => "dqtdatingido",
                    "label" => "Qtd. dias % atingido",
                    "blockAgp" => "",
                    "type" => "string"));

                break;
            case 'endbai':
                array_push($coluna, array("campo" => "endbai",
                    "label" => "Endereço - Bairro",
                    "blockAgp" => "",
                    "type" => "string"));

                break;
            case 'unidade_endbai':
                array_push($coluna, array("campo" => "unidade_endbai",
                    "label" => "Endereço - Bairro (Unidade)",
                    "blockAgp" => "",
                    "type" => "string"));

                break;
            case 'endcomunidade':
                array_push($coluna, array("campo" => "endcomunidade",
                    "label" => "Endereço - Comunidade",
                    "blockAgp" => "",
                    "type" => ""));

                break;
            case 'obrsupervisoes':
                array_push($coluna, array("campo" => "obrsupervisoes",
                    "label" => "Nº de Sup.Empresa",
                    "blockAgp" => "",
                    "type" => "string"));

                break;
            case 'unidade_email':
                array_push($coluna, array("campo" => "unidade_email",
                    "label" => "Email (Unidade)",
                    "blockAgp" => "",
                    "type" => "string"));

                break;
            case 'unidade_numcomercial':
                array_push($coluna, array("campo" => "unidade_numcomercial",
                    "label" => "N° de Telefone Comercial (Unidade)",
                    "blockAgp" => "",
                    "type" => "string"));

                break;
            case 'unidade_endlog':
                array_push($coluna, array("campo" => "unidade_endlog",
                    "label" => "Endereço - Logradouro (Unidade)",
                    "blockAgp" => "",
                    "type" => ""));

                break;
            case 'unidade_endcom':
                array_push($coluna, array("campo" => "unidade_endcom",
                    "label" => "Endereço - Complemento (Unidade)",
                    "blockAgp" => "",
                    "type" => ""));

                break;
            case 'unidade_endnum':
                array_push($coluna, array("campo" => "unidade_endnum",
                    "label" => "Endereço - Número (Unidade)",
                    "blockAgp" => "",
                    "type" => "string"));

                break;
            case 'unidade_endcomunidade':
                array_push($coluna, array("campo" => "unidade_endcomunidade",
                    "label" => "Endereço - Comunidade (Unidade)",
                    "blockAgp" => "",
                    "type" => ""));

                break;
            case 'unidade_endcep':
                array_push($coluna, array("campo" => "unidade_endcep",
                    "label" => "CEP (Unidade)",
                    "blockAgp" => "",
                    "type" => "cep"));

                break;
            case 'mesoregiao':
                array_push($coluna, array("campo" => "mesoregiao",
                    "label" => "Mesorregião",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'microregiao':
                array_push($coluna, array("campo" => "microregiao",
                    "label" => "Microrregião",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'tipologia':
                array_push($coluna, array("campo" => "tipologia",
                    "label" => "Tipologia",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'stiid':
                array_push($coluna, array("campo" => "stiid",
                    "label" => "Situação do Instrumento",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'medidasexcecao':
                array_push($coluna, array("campo" => "medidasexcecao",
                    "label" => "Medidas de Exceção?",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            case 'frpid':
                array_push($coluna, array("campo" => "frpid",
                    "label" => "Tipo do Instrumento",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
            
            case 'qtdsupervisaoempresa':
                array_push($coluna, array("campo" => "qtdsupervisaoempresa",
                    "label" => "Qtd. Supervisão Empresa",
                    "blockAgp" => "",
                    "type" => "string"));
                break;
//            case 'soma_cronograma':
//                array_push($coluna, array("campo" => "soma_cronograma",
//                    "label" => "Valor do Cronograma",
//                    "blockAgp" => "",
//                    "type" => "string"));
//                break;
//            case 'diverg_crono_contrato':
//                //'diverg_crono_contrato'   => array( 'codigo' => 'diverg_crono_contrato',   'descricao' => 'Divergência entre cronograma/contrato' )
//                array_push($coluna, array("campo" => "diverg_crono_contrato",
//                    "label" => "Divergência entre cronograma/contrato",
//                    "blockAgp" => "",
//                    "type" => "string"));
//                break;
        }
    }
    return $coluna;
}


// transforma consulta em pública
if ($_REQUEST['prtid'] && $_REQUEST['publico']) {
    $sql = retornaSQL('ConsultaPublica');
    $db->executar($sql);
    $db->commit();
    ?>
    <script type="text/javascript">
        location.href = '/obras2/obras2.php?modulo=relatorio/relatorio_geral&acao=A';
    </script>
    <?php
    die;
}
// FIM transforma consulta em pública
// remove consulta
if ($_REQUEST['prtid'] && $_REQUEST['excluir'] == 1) {
    $sql = retornaSQL('DeletaConsultaPublica');
    $db->executar($sql);
    $db->commit();
    ?>
    <script type="text/javascript">
        location.href = '/obras2/obras2.php?modulo=relatorio/relatorio_geral&acao=A';
    </script>
    <?php
    die;
}
// FIM remove consulta
// remove flag de submissão de formulário
if ($_REQUEST['prtid'] && $_REQUEST['carregar']) {
    unset($_REQUEST['form']);
}
// FIM remove flag de submissão de formulário

// exibe consulta
if (isset($_REQUEST['form']) == true) {

    if ($_REQUEST['prtid']) {

        $sql      = retornaSQL('ExibeConsulta');
        $itens    = $db->pegaUm($sql);
        $dados    = unserialize(stripslashes(stripslashes($itens)));

        if ($_REQUEST['pesquisa'] == '2'){
            $dados['pesquisa'] = 2;
        }

        $_REQUEST = $dados; //array_merge( $_REQUEST, $dados );

        unset($_REQUEST['salvar']);
    }
    include APPRAIZ . 'obras2/modulos/relatorio/geral_resultado.inc';
    exit;
}

// carrega consulta do banco
if ($_REQUEST['prtid'] && $_REQUEST['carregar'] == 1) {
    $sql   = retornaSQL('CarregaConsultaBanco');
    $itens = $db->pegaUm($sql);
    $dados = unserialize(stripslashes(stripslashes($itens)));
    
    extract($dados);
    $_REQUEST = $dados;
    unset($_REQUEST['form']);
    unset($_REQUEST['pesquisa']);
    $titulo = $_REQUEST['titulo'];

    $agrupador2 = array();

    if ($_REQUEST['agrupador']) {
        foreach ($_REQUEST['agrupador'] as $valorAgrupador) {
            array_push($agrupador2, array('codigo' => $valorAgrupador, 'descricao' => $valorAgrupador));
        }
    }
}

if (isset($_REQUEST['pesquisa']) || isset($_REQUEST['tipoRelatorio'])) {
    switch ($_REQUEST['pesquisa']) {
        case '1':
            include APPRAIZ . 'obras2/modulos/relatorio/geral_resultado.inc';
            exit;
        case '2':
            include APPRAIZ . 'obras2/modulos/relatorio/geralxls_resultado.inc';
            exit;
    }
}

/**
 * Início da montagem da tela
 */
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela, $url, '');
$titulo_modulo = "Relatório Geral";
monta_titulo($titulo_modulo, 'Selecione os filtros e agrupadores desejados');
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

    jQuery.noConflict();

    divCarregando();

    jQuery(document).ready(function() {

        jQuery('input[name=orgid[]]').click(function() {
            filtros = [];
            jQuery.each(jQuery('input[name=orgid[]]:checked'), function(i, obj) {
                filtros.push(obj.value);
            });
            divCarregando();
            jQuery.ajax({
                url: 'obras2.php?modulo=relatorio/relatorio_geral&acao=A',
                type: 'post',
                data: 'tipo_ensino=true&orgid=' + filtros,
                success: function(e) {
                    jQuery('#tr_prfid').remove();
                    jQuery('#tr_campus').after(e);
                    divCarregado();
                }
            });
            divCarregando();
            jQuery.ajax({
                url: window.location,
                type: 'post',
                data: 'reqAjax=montaEstadoWorkflow&orgids=' + filtros,
                success: function(e) {
                    jQuery('#tr_stoid').remove();
//					jQuery('#dbg').remove();
                    jQuery('#tr_cloid').after(e);
                    divCarregado();
                }
            });
        });

        divCarregado();
    });
    
    function obras_exibeRelatorioGeralXLS() {

        var formulario = document.filtro;
        var colunas    = document.getElementById('colunas');
        // Tipo de relatorio
        formulario.pesquisa.value = '2';

        prepara_formulario();
        selectAllOptions(formulario.colunas);


        if (!document.getElementsByName('orgid[]').item(0).checked &&
                !document.getElementsByName('orgid[]').item(1).checked &&
                !document.getElementsByName('orgid[]').item(2).checked &&
                !document.getElementsByName('orgid[]').item(3).checked &&
                !document.getElementsByName('orgid[]').item(4).checked) {
            alert('Favor selecionar ao menos um tipo de ensino!');
            return false;
        }

        if (!colunas.options.length) {
            alert('Favor selecionar ao menos uma coluna!');
            return false;
        }

        selectAllOptions(colunas);
        selectAllOptions(document.getElementById('regiao'));
        selectAllOptions(document.getElementById('mesoregiao'));
        selectAllOptions(document.getElementById('microregiao'));
        selectAllOptions(document.getElementById('uf'));
        selectAllOptions(document.getElementById('municipio'));
        selectAllOptions(document.getElementById('unidade'));
        selectAllOptions(document.getElementById('campus'));
        selectAllOptions(document.getElementById('prfid'));
        selectAllOptions(document.getElementById('tooid'));
        selectAllOptions(document.getElementById('tpoid'));
        selectAllOptions(document.getElementById('cloid'));
        selectAllOptions(document.getElementById('stoid'));
        selectAllOptions(document.getElementById('grupomun'));
        selectAllOptions(document.getElementById('tipomun'));

        formulario.submit();
    }

    function obras_exibeRelatorioGeral(tipo) {

        var formulario = document.filtro;
        var agrupador  = document.getElementById('colunas');

        // Tipo de relatorio
        formulario.pesquisa.value = '1';

        prepara_formulario();
        selectAllOptions(formulario.colunas);

        if (tipo == 'relatorio') {
            formulario.action = 'obras2.php?modulo=relatorio/relatorio_geral&acao=A';
            window.open('', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
            formulario.target = 'relatorio';
        } else {

            if (tipo == 'planilha') {

                if (document.getElementsByName('orgid[]')) {
                    if (!document.getElementsByName('orgid[]').item(0).checked &&
                            !document.getElementsByName('orgid[]').item(1).checked &&
                            !document.getElementsByName('orgid[]').item(2).checked &&
                            !document.getElementsByName('orgid[]').item(3).checked &&
                            !document.getElementsByName('orgid[]').item(4).checked) {
                        alert('Favor selecionar ao menos um tipo de ensino!');
                        return false;
                    }
                } else {
                    alert('Nenhum tipo de ensino associado a seu perfil!');
                    return false;
                }

                if (!colunas.options.length) {
                    alert('Favor selecionar ao menos uma coluna!');
                    return false;
                }

                formulario.action = 'obras2.php?modulo=relatorio/relatorio_geral&acao=A&tipoRelatorio=xls';

            } else if (tipo == 'salvar') {

                if (formulario.titulo.value == '') {
                    alert('É necessário informar a descrição do relatório!');
                    formulario.titulo.focus();
                    return;
                }
                var nomesExistentes = new Array();
                
                <?php
                $sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela";
                $nomesExistentes = $db->carregar($sqlNomesConsulta);
                if ($nomesExistentes) {
                    foreach ($nomesExistentes as $linhaNome) {
                        print "nomesExistentes[nomesExistentes.length] = '" . str_replace("'", "\'", $linhaNome['prtdsc']) . "';";
                    }
                }
                ?>
                
                var confirma = true;
                var i, j = nomesExistentes.length;
                for (i = 0; i < j; i++) {
                    if (nomesExistentes[i] == formulario.titulo.value) {
                        confirma = confirm('Deseja alterar a consulta já existente?');
                        break;
                    }
                }
                if (!confirma) {
                    return;
                }
                formulario.action = 'obras2.php?modulo=relatorio/relatorio_geral&acao=A&salvar=1';
                formulario.target = '_self';

            } else if (tipo == 'exibir') {

                if (!document.getElementsByName('orgid[]').item(0).checked &&
                    !document.getElementsByName('orgid[]').item(1).checked &&
                    !document.getElementsByName('orgid[]').item(2).checked &&
                    !document.getElementsByName('orgid[]').item(3).checked &&
                    !document.getElementsByName('orgid[]').item(4).checked
                   ) {
                    alert('Favor selecionar ao menos um tipo de ensino!');
                    return false;
                }

                if (!agrupador.options.length) {
                    alert('Favor selecionar ao menos uma coluna!');
                    return false;
                }

                selectAllOptions(agrupador);
                selectAllOptions(document.getElementById('regiao'));
                selectAllOptions(document.getElementById('mesoregiao'));
                selectAllOptions(document.getElementById('microregiao'));
                selectAllOptions(document.getElementById('uf'));
                selectAllOptions(document.getElementById('municipio'));
                selectAllOptions(document.getElementById('unidade'));
                selectAllOptions(document.getElementById('campus'));
                selectAllOptions(document.getElementById('prfid'));
                selectAllOptions(document.getElementById('tooid'));
                selectAllOptions(document.getElementById('tpoid'));
                selectAllOptions(document.getElementById('cloid'));
                selectAllOptions(document.getElementById('stoid'));
                selectAllOptions(document.getElementById('grupomun'));
                selectAllOptions(document.getElementById('tipomun'));

                formulario.target = 'resultadoObrasGeral';
                var janela = window.open('', 'resultadoObrasGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
                janela.focus();
            }
        }
        formulario.submit();
    }

    function tornar_publico(prtid) {
        document.filtro.publico.value = '1';
        
        document.filtro.prtid.value   = prtid;
        document.filtro.target        = '_self';
        document.filtro.submit();
    }

    function excluir_relatorio(prtid) {
        document.filtro.excluir.value = '1';
        
        document.filtro.prtid.value   = prtid;
        document.filtro.target        = '_self';
        document.filtro.submit();
    }

    function carregar_consulta(prtid) {
        document.filtro.carregar.value = '1';
        
        document.filtro.prtid.value    = prtid;
        document.filtro.target         = '_self';
        document.filtro.submit();
    }

    function carregar_relatorio(prtid, xls) {
        document.filtro.prtid.value = prtid;
        var formulario = document.filtro;
        if(xls){
            formulario.pesquisa.value = '2';
            formulario.action = 'obras2.php?modulo=relatorio/relatorio_geral&acao=A&tipoRelatorio=xls';
            formulario.submit();
        } else{
            obras_exibeRelatorioGeral('relatorio');
        }

    }

    /* Função para substituir todos */
    function replaceAll(str, de, para) {
        var pos = str.indexOf(de);
        while (pos > -1) {
            str = str.replace(de, para);
            pos = str.indexOf(de);
        }
        return (str);
    }
    /* Função para adicionar linha nas tabelas */

    /* CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
    function criarrequisicao() {
        return window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject('Msxml2.XMLHTTP');
    }
    /* FIM - CRIANDO REQUISIÇÃO (IE OU FIREFOX) */

    /* FUNÇÃO QUE TRATA O RETORNO */
    var pegarretorno = function() {
        try {
            if (evXmlHttp.readyState == 4) {
                if (evXmlHttp.status == 200 && evXmlHttp.responseText != '') {
                    // criando options
                    var x = evXmlHttp.responseText.split("&&");
                    for (i = 1; i < (x.length - 1); i++) {
                        var dados = x[i].split("##");
                        document.getElementById('usrs').options[i] = new Option(dados[1], dados[0]);
                    }
                    var dados = x[0].split("##");
                    document.getElementById('usrs').options[0] = new Option(dados[1], dados[0]);
                    document.getElementById('usrs').value = cpfselecionado;
                }
                if (evXmlHttp.dispose) {
                    evXmlHttp.dispose();
                }
                evXmlHttp = null;
            }
        }
        catch (e) {
        }
    };
    /* FIM - FUNÇÃO QUE TRATA O RETORNO */


    /**
     * Alterar visibilidade de um bloco.
     * 
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco(bloco)
    {
        var div_on = document.getElementById(bloco + '_div_filtros_on');
        var div_off = document.getElementById(bloco + '_div_filtros_off');
        var img = document.getElementById(bloco + '_img');
        var input = document.getElementById(bloco + '_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '0';
            img.src = '/imagens/menos.gif';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '1';
            img.src = '/imagens/mais.gif';
        }
    }

    /**
     * Alterar visibilidade de um campo.
     * 
     * @param string indica o campo a ser mostrado/escondido
     * @return void
     */
    function onOffCampo(campo)
    {
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }

//-->
</script>
<form action="" method="post" name="filtro" id="filtro"> 
    <input type="hidden" name="form"     value="1" />
    <input type="hidden" name="pesquisa" value="1" />
    <input type="hidden" name="publico"  value=""  /> <!-- indica se foi clicado para tornar o relatório público ou privado -->
    <input type="hidden" name="prtid"    value=""  /> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
    <input type="hidden" name="carregar" value=""  /> <!-- indica se foi clicado para carregar o relatório -->
    <input type="hidden" name="excluir"  value=""  /> <!-- indica se foi clicado para excluir o relatório já gravado -->

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita">Título</td>
            <td>
                <?php echo campo_texto('titulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo"'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipo de Ensino</td>
            <td>
                <?php
                // Monta as opções de tipo de ensino de acordo com a responsabilidade do usuario
                if ( ($db->testa_superuser()) || ( possuiPerfil(PERFIL_CONSULTAGERAL) || 
                                                   possuiPerfil(PERFIL_GESTORMEC) || 
                                                   possuiPerfil(PERFIL_ADMINISTRADOR) ) 
                   ) {

                    $orgaos = $db->carregar(retornaSQL('TipoEnsino'));
                    $count  = count($orgaos);
                    for ($i = 0; $i < $count; $i++) {
                        echo '<input type="checkbox" id="orgid" name="orgid[]" value="' . $orgaos[$i]['orgid'] . '"/> ' . $orgaos[$i]["orgdesc"] . '</label>&nbsp;';
                    }
                } else {
                    $orgaos = obras_pegarOrgaoPermitido();
                    $count = count($orgaos);
                    for ($i = 0; $i < $count; $i++) {
                        echo '<input type="checkbox" id="orgid" name="orgid[]" value="' . $orgaos[$i]['id'] . '"/> ' . $orgaos[$i]["descricao"] . '</label>&nbsp;';
                    }
                }
               ?>
           </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Colunas</td>
            <td>
                <?php exibeAgrupador('colunas', 'filtro'); ?>
            </td>
        </tr>
    </table>
    <!-- OUTROS FILTROS -->
    <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
        <tr>
            <td onclick="javascript:onOffBloco('outros');">
                <img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
                Relatórios Gerenciais
                <input type="hidden" id="outros_flag" name="outros_flag" value="0" />
            </td>
        </tr>
    </table>
    <div id="outros_div_filtros_off"></div>

    <div id="outros_div_filtros_on" style="display:none;">
        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
            <tr>
                <td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
                    <?php
                        if ($db->testa_superuser() || possuiPerfil(PERFIL_SUPERVISORMEC) || possuiPerfil(PERFIL_ADMINISTRADOR)) {
                            $bt_publicar = "<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;";
                        }
                        $sql = retornaSQL('ListaRelatorios', array('btn'=>$bt_publicar));
                        $cabecalho = array('Ação', 'Nome');
                    ?>
                <td>
                    <?php $db->monta_lista_simples($sql, $cabecalho, 50, 50, null, null, null); ?>
                </td>
            </tr>
        </table>
    </div>
    <!-- FIM OUTROS FILTROS -->
    <!-- MINHAS CONSULTAS -->
    <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
        <tr>
            <td onclick="javascript:onOffBloco('minhasconsultas');">
                <img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
                Minhas Consultas
                <input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />
            </td>
        </tr>
    </table>
    <div id="minhasconsultas_div_filtros_off">
    </div>
    <div id="minhasconsultas_div_filtros_on" style="display:none;">
        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
            <tr>
                <td width="195" class="SubTituloDireita" valign="top">Consultas</td>
                    <?php
                        $sql = retornaSQL('MinhasConsultas');
                        $cabecalho = array('Ação', 'Nome');
                    ?>
                <td>
                    <?php $db->monta_lista_simples($sql, $cabecalho, 50, 50, 'N', '80%', null); ?>
                </td>
            </tr>
        </table>
    </div>
    <!-- FIM MINHAS CONSULTAS -->

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                <?php
                    retornaCombo('Regiao');
                    retornaCombo('UF');
                    retornaCombo('MesoRegiao');
                    retornaCombo('MicroRegiao');
                    retornaCombo('GrupoMunicipio');
                    retornaCombo('TipoMunicipio');
                    retornaCombo('Municipio');
                    retornaCombo('Unidades');
                    retornaCombo('Campus');
                    retornaCombo('Programa');
                    retornaCombo('Fonte');
                    retornaCombo('TipologiaObra');
                    retornaCombo('ClassificacaoObra');
                    retornaCombo('SituacaoObra');
                ?>
        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Esfera:</td>
            <td>
            <?php
                $arrEsfera = array( array('codigo' => 'E', 'descricao' => 'Estadual'),  
                                    array('codigo' => 'M', 'descricao' => 'Municipal')
                                  );
                echo $db->monta_combo("obrtipoesfera", $arrEsfera, "S", "Todas", "", "", '', "", "N", "obrtipoesfera", true);
            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Percentual da Obra</td>
            <td>
                <table>
                    <tr>
                        <th>Mínimo</th>
                        <th>Máximo</th>
                    </tr>
                    <tr>
                        <?php
                        $arPercentual[] = array('codigo' => 0,   'descricao' => '0 %');
                        $arPercentual[] = array('codigo' => 5,   'descricao' => '5 %');
                        $arPercentual[] = array('codigo' => 10,  'descricao' => '10 %');
                        $arPercentual[] = array('codigo' => 15,  'descricao' => '15 %');
                        $arPercentual[] = array('codigo' => 20,  'descricao' => '20 %');
                        $arPercentual[] = array('codigo' => 25,  'descricao' => '25 %');
                        $arPercentual[] = array('codigo' => 30,  'descricao' => '30 %');
                        $arPercentual[] = array('codigo' => 35,  'descricao' => '35 %');
                        $arPercentual[] = array('codigo' => 40,  'descricao' => '40 %');
                        $arPercentual[] = array('codigo' => 45,  'descricao' => '45 %');
                        $arPercentual[] = array('codigo' => 50,  'descricao' => '50 %');
                        $arPercentual[] = array('codigo' => 55,  'descricao' => '55 %');
                        $arPercentual[] = array('codigo' => 60,  'descricao' => '60 %');
                        $arPercentual[] = array('codigo' => 65,  'descricao' => '65 %');
                        $arPercentual[] = array('codigo' => 70,  'descricao' => '70 %');
                        $arPercentual[] = array('codigo' => 75,  'descricao' => '75 %');
                        $arPercentual[] = array('codigo' => 80,  'descricao' => '80 %');
                        $arPercentual[] = array('codigo' => 85,  'descricao' => '85 %');
                        $arPercentual[] = array('codigo' => 90,  'descricao' => '90 %');
                        $arPercentual[] = array('codigo' => 95,  'descricao' => '95 %');
                        $arPercentual[] = array('codigo' => 100, 'descricao' => '100 %');

                        $percentualinicial = 0;
                        $percentualfinal = 100;
                        echo '<td>';
                        $db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
                        echo '</td><td>';
                        $db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal');
                        echo '</td>';
                        ?>
                    </tr>
                </table>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">Evolução da Obra</td>
            <td>
                <table>
                    <tr>
                        <th>Mínimo</th>
                        <th>Máximo</th>
                    </tr>
                    <tr>
                        <?php

                        $percentualevolucaoinicial = 0;
                        $percentualevolucaofinal = 100;
                        echo '<td>';
                        $db->monta_combo("percentualevolucaoinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualevolucaoinicial');
                        echo '</td><td>';
                        $db->monta_combo("percentualevolucaofinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualevolucaofinal');
                        echo '</td>';
                        ?>
                    </tr>
                </table>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">Qtd. dias % atingido</td>
            <td>
                <?php
                $db->monta_combo("dqtdatingido", $arPercentual, 'S', '', 'dqtdatingido', '', '', '', 'N', 'dqtdatingido');
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita">Possui foto?</td>
            <td>
                <input type='radio' id='foto' name='foto' value='sim' /> Sim
                <input type='radio' id='foto' name='foto' value='nao'/> Não
                <input type='radio' id='foto' name='foto' value='todos' checked="checked"/> Todas
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Possui vistoria?</td>
            <td>
                <input type='radio' id='vistoria' name='vistoria' value='sim'   onclick="mostraResponsavel(this.value);" /> Sim
                <input type='radio' id='vistoria' name='vistoria' value='nao'   onclick="mostraResponsavel(this.value);" /> Não
                <input type='radio' id='vistoria' name='vistoria' value='todos' onclick="mostraResponsavel(this.value);"  checked="checked" /> Todas
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Status da Obra:</td>
            <td>
                <input type='radio' id='status_a'     name='status' value='' checked="checked"    /> Ativo
                <input type='radio' id='status_i'     name='status' value='inativo'   /> Inativo
                <input type='radio' id='status_todas' name='status' value='todos'  /> Todas
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Possui obra vinculada</td>
            <td>
                <input type="radio" name="obrvinculada" id="" value="S" <?= ( $_POST["obrvinculada"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                <input type="radio" name="obrvinculada" id="" value="N" <?= ( $_POST["obrvinculada"] == "N"  ? "checked='checked'" : "" ) ?>/> Não
                <input type="radio" name="obrvinculada" id="" value="T"  <?= ( $_POST["obrvinculada"] == "T" || $_POST["obrvinculada"] == "" ? "checked='checked'" : "" ) ?> /> Todas
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Possui Supervisão:</td>
            <td>
                <input type="radio" name="supervisao" onclick="exibeSubFiltros('S')" id="" value="S" 
                    <?php if ($_SESSION["obras"]["filtros"]["supervisao"] == "S") {
                            print "checked='checked'";
                          } 
                     ?>/> Sim
                <input type="radio" name="supervisao" onclick="exibeSubFiltros('N')" id="" value="N" 
                    <?php if ($_SESSION["obras"]["filtros"]["supervisao"] == "N") {
                            print "checked='checked'";
                          } 
                     ?>/> Não
                <input type="radio" name="supervisao" onclick="exibeSubFiltros('N')" id="" value="" 
                    <?php if ($_SESSION["obras"]["filtros"]["supervisao"] == "") {
                            print "checked='checked'";
                        } 
                     ?>/> Todas
                <script type="text/javascript">
                    function verificaSubFiltro(tipo)
                    {
                        if (tipo == "entre") {
                            document.getElementById('fim_subfiltro').style.display = "";
                        } else {
                            document.getElementById('fim_subfiltro').style.display = "none";
                        }
                    }

                    function exibeSubFiltros(tipo)
                    {
                        if (tipo == "S") {
                            document.getElementById('inicio_subfiltro').style.display = "";
                        } else {
                            document.getElementById('inicio_subfiltro').style.display = "none";
                        }
                    }
                </script>
                <span style="padding-left:10px;display:none" id="inicio_subfiltro"  >
                <?php
                    $arrTipoSupervisao = array(
                                                0 => array("codigo" => ">",     "descricao" => "Maior que"),
                                                1 => array("codigo" => ">=",    "descricao" => "Maior Igual a"),
                                                2 => array("codigo" => "=",     "descricao" => "Igual a"),
                                                3 => array("codigo" => "<",     "descricao" => "Menor que"),
                                                4 => array("codigo" => "<=",    "descricao" => "Menor Igual a"),
                                                5 => array("codigo" => "entre", "descricao" => "Entre")
                                               );
                    $db->monta_combo("tiposupervisao", $arrTipoSupervisao, "S", "", "verificaSubFiltro", "");
                    echo campo_texto("subfiltro_inicio", "N", "S", "", 10, 20, "[#]", "", "", "", "", "id='subfiltro_inicio'");
                ?>  
                    <span id="fim_subfiltro" style="display:none"  > de 
                        <?php echo campo_texto("subfiltro_fim", "N", "S", "", 10, 20, "[#]", "", "", "", "", "id='subfiltro_fim'") ?>
                    </span>
                </span>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Obra em funcionamento?:</td>
            <td>
                <input type='radio' name='funundfuncionamento' value='sim' /> Sim
                <input type='radio' name='funundfuncionamento' value='nao' /> Não
                <input type='radio' name='funundfuncionamento' value='todos' checked="checked" /> Todas
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Medidas de Exceção?:</td>
            <td>
                <input type='radio' name='medidasexcecao' value='sim' /> Sim
                <input type='radio' name='medidasexcecao' value='nao' /> Não
                <input type='radio' name='medidasexcecao' value='todos' checked="checked" /> Todas
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Tipo do Instrumento:</td>
            <td>
                <?php
                $tipoFormaRepasseRecursos = new TipoFormaRepasseRecursos();
                $sql                      = $tipoFormaRepasseRecursos->listaCombo($frpid);
                $db->monta_combo('frpid', $sql, 'S', "Selecione...", 'abreCamposRecursos', '', '', '150', 'S', 'frpid');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Situação do Instrumento:</td>
            <td>
                <?php
                $tipoSituacaoInstrumento = new SituacaoInstrumento();
                $sql                      = $tipoSituacaoInstrumento->listaCombo();
                $db->monta_combo('stiid', $sql, 'S', "Selecione...", '', '', '', '150', 'S', 'stiid');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Valor da Obra:</td>
            <td>
                De:&nbsp;
                        <?php echo campo_texto('vlrmenor', 'N', 'S', '', 20, 40, '[.###],##', '', 'left', '', 0, ''); ?>
                Até:&nbsp;
                        <?php echo campo_texto('vlrmaior', 'N', 'S', '', 20, 40, '[.###],##', '', 'left', '', 0, ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Valor Contratado da Obra (R$):</td>
            <td>
                De:&nbsp;
                    <?php echo campo_texto('obrcustocontrato_inicio', 'N', 'S', '', 20, 40, '', '', 'left', '', 0, 'onkeyup="this.value=mascaraglobal(\'###.###.###.###,##\',this.value);" onfocus="this.select();"'); ?>
                Até:&nbsp;
                    <?php echo campo_texto('obrcustocontrato_fim', 'N', 'S', '', 20, 40, '', '', 'left', '', 0, 'onkeyup="this.value=mascaraglobal(\'###.###.###.###,##\',this.value);" onfocus="this.select();"'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Metragem da obra:</td>
            <td>
                <script type="text/javascript">
                    function verificaOperador(valor)
                    {
                        if (valor == 'entre') {
                            jQuery('#fim_metragem').show();
                        } else {
                            jQuery('#fim_metragem').hide();
                        }
                    }
                </script>
                <?php
                    $arrOperadores = array(
                                            0 => array("codigo" => ">",     "descricao" => "Maior que"),
                                            1 => array("codigo" => ">=",    "descricao" => "Maior Igual a"),
                                            2 => array("codigo" => "=",     "descricao" => "Igual a"),
                                            3 => array("codigo" => "<",     "descricao" => "Menor que"),
                                            4 => array("codigo" => "<=",    "descricao" => "Menor Igual a"),
                                            5 => array("codigo" => "entre", "descricao" => "Entre")
                                          );
                    $db->monta_combo("metragem_operador", $arrOperadores, "S", "", "verificaOperador", "");
                    echo ' ';
                    echo campo_texto("metragem_inicio", "N", "S", "", 10, 20, "[#]", "", "", "", "", "id='metragem_inicio'"); 
                ?>
                <span id="fim_metragem" style="display:none"  > de 
                    <?php echo campo_texto("metragem_fim", "N", "S", "", 10, 20, "[#]", "", "", "", "", "id='metragem_fim'"); ?>
                </span>
            </td>
        </tr>
        <tr id="trResponsavel">
            <td class="SubTituloDireita">Responsável pela última vistoria</td>
            <td>
                <?php
                $rsuid = $_SESSION["obras"]["filtros"]["rsuid"];
                
                $arrSupervisao = $db->carregar(retornaSQL('ResponsavelUltimaVistoria'));
                $arrSupervisao = $arrSupervisao ? $arrSupervisao : array();

                $html = '<select name="responsavel" class="CampoEstilo" style="width: auto" id="responsavel">
                            <option value="">Todas</option>
                            <option value="0" ' . ($rsuid == '0' ? 'selected="selected"' : '' ) . '>Sem Supervisão</option>';
                foreach ($arrSupervisao as $v) {
                    $select = '';
                    if ($rsuid == $v['codigo'])
                        $select = 'selected="selected"';
                    $html .= '<option value="' . $v['codigo'] . '" ' . $select . '>' . $v['descricao'] . '</option>';
                }
                $html.= '</select>';

                echo $html;
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Possui restrição</td>
            <td>
                <input type='radio' id='restricao' name='restricao' value='sim' /> Sim
                <input type='radio' id='restricao' name='restricao' value='nao'/> Não
                <input type='radio' id='restricao' name='restricao' value='todos' checked="checked"/> Todas
            </td>
        </tr>
        <tr>
            <td bgcolor="#CCCCCC"></td>
            <td bgcolor="#CCCCCC">
                <input type="button" value="Visualizar"      onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
                <input type="button" value="Visualizar XLS"  onclick="obras_exibeRelatorioGeralXLS();"      style="cursor: pointer;"/>
                <input type="button" value="Salvar Consulta" onclick="obras_exibeRelatorioGeral('salvar');" style="cursor: pointer;"/>
            </td>
        </tr>
    </table>
</form>
<script type="text/javascript">
    function mostraResponsavel(valor) {
        var tr = document.getElementById('trResponsavel');
        var resp = document.getElementById('responsavel');

        if (valor == 'nao') {
            resp.value = '';
            tr.style.display = 'none';
        } else {
            tr.style.display = 'table-row';
        }
    }
</script>
