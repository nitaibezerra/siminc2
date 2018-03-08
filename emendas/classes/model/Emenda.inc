<?php
/**
 * Classe de mapeamento da entidade emendas.emenda
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "emenda/classes/WSIntegracaoSiconv.class.inc";


/**
 * Emendas_Model_Emenda
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_Emenda extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.emenda';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'emeid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'prsano' => array('tabela' => 'emendas.programacaoexercicio', 'pk' => 'prsano'),
        'autid' => array('tabela' => 'emendas.autor', 'pk' => 'autid'),
        'ptrid' => array('tabela' => 'monitora.ptres', 'pk' => 'ptrid'),
        'unoid' => array('tabela' => 'unidadeorcamentaria', 'pk' => 'unoid')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'emeid' => null,
        'emenumero' => null,
        'unoid' => null,
        'ptrid' => null,
        'emejustificativa' => null,
        'emeimpositiva' => null,
        'autid' => null,
        'prsano' => null,
        'emestatus' => null,
        'emeatualizacaosiconv' => null,
    );

    public function recuperarListagem($exercicio, $listaSubUnidadeUsuario = array())
    {
        $where = '';
        
        # Sub-Unidade Orçamentária ou Sub-Unidade Orçamentária delegada.
        $where .= $listaSubUnidadeUsuario? "\n AND (suo.suocod::INTEGER IN(". join(',', $listaSubUnidadeUsuario). ") OR suoed.suocod::INTEGER IN(". join(',', $listaSubUnidadeUsuario). ") ) ": NULL;
        
        $sql = "
            SELECT
                eme.emeid,
                eme.emenumero,
                eme.unoid,
                eme.ptrid,
                eme.emeimpositiva,
                eme.autid,
                eme.prsano,
                uno.unocod || ' - ' || uno.unosigla unidade,
                aut.autnome,
                aut.autcod,
                pro.pronome,
                sit.sitdsc,
                ben.benid
            FROM
                emendas.emenda eme
                LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid
                LEFT JOIN emendas.proponente pro ON pro.proid = ben.proid
                LEFT JOIN emendas.siconv sic ON sic.sicid = ben.sicid
                LEFT JOIN emendas.siconvsituacao sit ON sit.sitid = sic.sitid
                JOIN public.unidadeorcamentaria uno ON uno.unoid = eme.unoid
                JOIN emendas.autor aut ON aut.autid = eme.autid
                JOIN monitora.ptres ptr ON ptr.ptrid = eme.ptrid
                LEFT JOIN public.vw_subunidadeorcamentaria suo ON ben.suoid = suo.suoid
		LEFT JOIN emendas.delegacao ed ON ben.benid = ed.benid
		LEFT JOIN public.vw_subunidadeorcamentaria suoed ON ed.suoid = suoed.suoid
            WHERE
                eme.emestatus = 'A'
                AND eme.prsano = '". (int)$exercicio. "'
                $where
            ORDER BY
                aut.autnome,
                eme.emenumero,
                pro.pronome
        ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
    
   
    /**
     * Monta consulta pra buscar dados do relat?rio geral
     * 
     * @param integer $exercicio
     * @param array $listaSubUnidadeUsuario
     * @return string
     */
    public function montarSqlRelatorioGeral($exercicio, $listaSubUnidadeUsuario = array()){
        $where = '';
        
        # Sub-Unidade Orçamentária ou Sub-Unidade Orçamentária delegada.
        $where .= $listaSubUnidadeUsuario? "\n AND (suo.suocod::INTEGER IN(". join(',', $listaSubUnidadeUsuario). ") OR suoed.suocod::INTEGER IN(". join(',', $listaSubUnidadeUsuario). ") ) ": NULL;
        
        $sql = "
            SELECT DISTINCT
		-- DADOS DA EMENDA
                eme.emeid,
                eme.emenumero,
                aut.autcod,
                aut.autnome,
		uno.unocod,
		uno.unosigla,
		ptr.funcional || ' - ' || COALESCE(ptr.acatitulo, '') AS funcional,
                CASE WHEN eme.emeimpositiva = 't' THEN 'Sim' ELSE 'N?o' END AS impositiva,
                eme.emejustificativa,
                emed.gndcod,
                emed.mapcod AS mod_aplic,
                emed.foncod AS fonte,
                emed.emdvalor,
--                eme.*,
-- Beneficiario
                sit.sitdsc
/*
                eme.autid,
                eme.prsano,
                pro.pronome,
                sit.sitdsc,
                ben.benid
*/
            FROM emendas.emenda eme
                LEFT JOIN public.unidadeorcamentaria uno ON uno.unoid = eme.unoid
                LEFT JOIN emendas.autor aut ON aut.autid = eme.autid
                LEFT JOIN monitora.vw_ptres ptr ON ptr.ptrid = eme.ptrid
                LEFT JOIN emendas.emendadetalhe emed ON eme.emeid = emed.emeid -- SELECT * FROM emendas.emendadetalhe

                LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid
                LEFT JOIN emendas.proponente pro ON pro.proid = ben.proid
                LEFT JOIN emendas.siconv sic ON sic.sicid = ben.sicid
                LEFT JOIN emendas.siconvsituacao sit ON sit.sitid = sic.sitid
                LEFT JOIN public.vw_subunidadeorcamentaria suo ON ben.suoid = suo.suoid
                LEFT JOIN emendas.delegacao ed ON ben.benid = ed.benid
                LEFT JOIN public.vw_subunidadeorcamentaria suoed ON ed.suoid = suoed.suoid
            WHERE
                eme.emestatus = 'A'
                AND eme.prsano = '". (int)$exercicio. "'
                $where
            ORDER BY
                1,
                2
        ";
//ver($sql, d);
        return $sql;
    }

    public function importar($files)
    {
        $extensao = substr($files['name'], strrpos($files['name'], '.'));

        $origem = $files['tmp_name'];
        $destino = APPRAIZ . 'emendas/modulos/apoio/importacao/planilha_' . date('YmdHis') . $extensao;
        if(move_uploaded_file($origem , $destino)){

            $file = fopen($destino, 'r');

            $count = 0;
            while (($line = fgetcsv($file, 0, ';')) !== false)
            {

                $line = array_map('trim', $line);
//                    $line = array_map('utf8_decode', $line);

                if(!$count++){ continue; }
                
                $emenumero = trim($line[2]);
                if(!$emenumero) { continue; }

                $sql = "select emeid from emendas.emenda where emenumero = " . $emenumero;
                $emeid = $this->pegaUm($sql);

                if($emeid){
                    $this->carregarPorId($emeid);
                } else {
                    // Unidade Orçamentária
                    $unocod = substr(trim($line[5]), 0, 5);
                    $unoid = $this->_recuperarUnidadeImportacao($unocod, $_SESSION['exercicio']);

                    // Autor
                    $autid = $this->_recuperarAutorImportacao($line, $_SESSION['exercicio']);

                    // Ação
                    $funcional = $this->_recuperarFuncionalImportacao($line[7], $line[6], $line[8], $unocod, $_SESSION['exercicio']);

                    if(!$funcional || !$autid){ continue; }

                    $this->emenumero = $emenumero;
                    $this->unoid = $unoid;
                    $this->ptrid = $funcional[0]['ptrid'];
                    $this->autid = $autid;
                    $this->emeimpositiva = 't';
                    $this->prsano = $_SESSION['exercicio'];
                    $this->salvar();
                }

                if(is_numeric($line[14])){
                    $this->_inserirBeneficiarioImportacao($line);
                } else {
                    $this->_inserirDetalhesImportacao($line);
                }
                $this->commit();
                $this->emeid = null;
            }
            fclose($file);
            ver('FIM', d);
        }
    }

    private function _recuperarUnidadeImportacao($unocod, $exercicio)
    {
        $sql = "select unoid from public.unidadeorcamentaria where unocod = '{$unocod}' and prsano = '$exercicio'";
        return $this->pegaUm($sql);
    }

    private function _recuperarAutorImportacao($line, $exercicio)
    {
        $dado = $line[0];

        if('(-1)' == substr($dado, 0, 4)){
            return false;
        }

        $autor = explode('-', $dado);
        $autcod = trim($autor[0]);
        $sql = "select autid from emendas.autor where autcod = '{$autcod}' order by autstatus";
        $autid = $this->pegaUm($sql);

        $parsigla = $line[1];
        $sql = "select parid from emendas.partido where parsigla = '{$parsigla}' order by parstatus";
        $parid = $this->pegaUm($sql);

        if(!$parid){
            $mPartido = new Emendas_Model_Partido();
            $mPartido->pardsc = $parsigla;
            $mPartido->parsigla = $parsigla;
            $mPartido->salvar();

            $parid = $mPartido->parid;
        }

        // Atualizando nome e partido do autor
        $mAutor = new Emendas_Model_Autor($autid);
        $mAutor->autcod = $autcod;
        $mAutor->autnome = trim($autor[1]);
        $mAutor->parid = $parid;
        $mAutor->salvar();
        $autid = $mAutor->autid;

        return $autid;
    }

    private function _recuperarFuncionalImportacao($acao, $programa, $localizador, $unocod, $exercicio)
    {
        $acacod = substr(trim($acao), 0, 4);
        $prgcod = substr(trim($programa), 0, 4);
        $loccod = substr(trim($localizador), 0, 4);
        $sql = "select * from monitora.ptres where acacod = '$acacod' and prgcod = '$prgcod' and loccod = '$loccod' and unicod = '$unocod' and ptrano = '{$exercicio}' and plocod = 'EIND' and ptrstatus = 'A'";

        $funcional = $this->carregar($sql);
        if(count($funcional)> 1){
            ver('Mais de uma ação', d);
        }

        return $funcional;
    }

    private function _inserirDetalhesImportacao($line)
    {
        $mEmendaDetalhe = new Emendas_Model_EmendaDetalhe();
        $mEmendaDetalhe->emeid = $this->emeid;
        $mEmendaDetalhe->foncod = $line[9];
        $mEmendaDetalhe->gndcod = $line[12];
        $mEmendaDetalhe->mapcod = $line[13];
        $mEmendaDetalhe->emdvalor = str_replace(['.', ','], ['', '.'], $line[18]) + str_replace(['.', ','], ['', '.'], $line[20]);

        return $mEmendaDetalhe->salvar();
    }

    private function _inserirBeneficiarioImportacao($line)
    {
        $proponente = explode('-', $line[15]);

        if(count($proponente) >= 2){
            $procnpj = trim(array_shift($proponente));
            $sql = "select proid from emendas.proponente where procnpj = '{$procnpj}' order by prostatus";

        } else {
            $sql = "select proid from emendas.proponente where procnpj = '00000000000000' order by prostatus";
        }

        $proid = $this->pegaUm($sql);
        
        if(!$proid){
            $mProponente = new Emendas_Model_Proponente();
            $mProponente->procnpj = $procnpj;
            $mProponente->pronome = trim(implode(' - ', $proponente));
            $mProponente->salvar();
            $proid = $mProponente->proid;
        }

        $sql = "select benid from emendas.beneficiario where emeid = {$this->emeid} and proid = {$proid}";
        $benid = $this->pegaUm($sql);

        $mBeneficiario = new Emendas_Model_Beneficiario($benid);

        if(!$benid){
            $mBeneficiario->emeid = $this->emeid;
            $mBeneficiario->proid = $proid;

            $mBeneficiario->salvar();
        }

        $this->_inserirBeneficiarioDetalhe($mBeneficiario->benid, $mBeneficiario->emeid, $line);
    }

    private function _inserirBeneficiarioDetalhe($benid, $emeid, $line)
    {
        $gndcod = $line[12];

        $sql = "select emdid from emendas.emendadetalhe where gndcod = {$gndcod} and emeid = {$emeid}";
        $emdid = $this->pegaUm($sql);

        $mBeneficiarioDetalhe = new Emendas_Model_BeneficiarioDetalhe();
        $mBeneficiarioDetalhe->benid = $benid;
        $mBeneficiarioDetalhe->emdid = $emdid;
        $mBeneficiarioDetalhe->bedvalor = str_replace(['.', ','], ['', '.'], $line[19]);
        $mBeneficiarioDetalhe->bedvalorpriorizado = str_replace(['.', ','], ['', '.'], $line[20]);

        return $mBeneficiarioDetalhe->salvar();
    }

    public function gerarGraficoUf($exercicio)
    {
        $sql = "select  case when coalesce(aut.estuf, '') = '' then 'A Identificar' else  aut.estuf end categoria, 
                        'Valor LOA' descricao, sum(bed.bedvalor) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where eme.prsano = '$exercicio'        
                group by aut.estuf, categoria
                union all
                select  case when coalesce(aut.estuf, '') = '' then 'A Identificar' else  aut.estuf end categoria, 
                        'Valor Após Contingenciamento' descricao, sum(bed.bedvalorpriorizado) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where eme.prsano = '$exercicio'                
                group by aut.estuf, categoria
                order by categoria, descricao desc";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['categoria']][$dado['descricao']] = $dado['valor'];
        }

        $sqlPizza = "  select  case when coalesce(reg.regdescricao, '') = '' then 'A Identificar' else  reg.regdescricao end descricao, reg.regcod, sum(bed.bedvalor) valor
                        from emendas.emenda eme
                                inner join emendas.autor aut on aut.autid = eme.autid
                                left join territorios.estado est on est.estuf = aut.estuf
                                left join territorios.regiao reg on reg.regcod = est.regcod
                                inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                                inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                        where eme.prsano = '$exercicio'        
                        group by descricao, reg.regcod
                        order by reg.regcod";

        $sql = "select  case when coalesce(reg.regdescricao, '') = '' then 'A Identificar' else  reg.regdescricao end categoria, reg.regcod,
                        'Valor LOA' descricao, sum(bed.bedvalor) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        left join territorios.estado est on est.estuf = aut.estuf
                        left join territorios.regiao reg on reg.regcod = est.regcod
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where eme.prsano = '$exercicio'        
                group by descricao, reg.regcod, categoria
                union all
                select  case when coalesce(reg.regdescricao, '') = '' then 'A Identificar' else  reg.regdescricao end categoria, reg.regcod,
                        'Valor Após Contingenciamento' descricao, sum(bed.bedvalorpriorizado) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        left join territorios.estado est on est.estuf = aut.estuf
                        left join territorios.regiao reg on reg.regcod = est.regcod
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where eme.prsano = '$exercicio'        
                group by descricao, reg.regcod, categoria
                order by regcod, categoria desc";

        $dadosRegiao = $this->carregar($sql);
        $dadosRegiao = $dadosRegiao ? $dadosRegiao : [];

        $dadosAgrupadosRegiao = [];
        foreach ($dadosRegiao as $dado) {
            $dadosAgrupadosRegiao[$dado['categoria']][$dado['descricao']] = $dado['valor'];
        }

        include_once APPRAIZ . 'emendas/modulos/principal/grafico_uf.inc';
    }

    public function gerarGraficoPartido($exercicio)
    {
        $sql = "select  case when coalesce(par.parsigla, '') = '' then 'A Identificar' else par.parsigla end categoria, par.pardsc,
                        'Valor LOA' descricao, sum(bed.bedvalor) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.partido par on par.parid = aut.parid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where eme.prsano = '$exercicio'        
                group by categoria, par.pardsc, descricao
                union all
                select  case when coalesce(par.parsigla, '') = '' then 'A Identificar' else par.parsigla end categoria, par.pardsc,
                        'Valor Após Contingenciamento' descricao, sum(bed.bedvalorpriorizado) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.partido par on par.parid = aut.parid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where eme.prsano = '$exercicio'        
                group by categoria, par.pardsc, descricao
                order by categoria, descricao desc";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['categoria']][$dado['descricao']] = $dado['valor'];
        }

        $sql = "select  case when coalesce(par.parlado, '') = '' then 'A Identificar' else par.parlado end categoria,
                        'Valor LOA' descricao, sum(bed.bedvalor) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.partido par on par.parid = aut.parid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where eme.prsano = '$exercicio'        
                group by categoria, descricao
                union all 
                select  case when coalesce(par.parlado, '') = '' then 'A Identificar' else par.parlado end categoria,
                        'Valor Após Contingenciamento' descricao, sum(bed.bedvalorpriorizado) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.partido par on par.parid = aut.parid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where eme.prsano = '$exercicio'        
                group by categoria, descricao
                order by categoria, descricao desc";

        $dadosLado = $this->carregar($sql);
        $dadosLado = $dadosLado ? $dadosLado : [];

        $dadosAgrupadosLado = [];
        foreach ($dadosLado as $dado) {
            $dadosAgrupadosLado[$dado['categoria']][$dado['descricao']] = $dado['valor'];
        }

        include_once APPRAIZ . 'emendas/modulos/principal/grafico_partido.inc';
    }

    public function gerarGraficoSexo($exercicio, $tipo = null)
    {
        $sql = "select  case when aut.autsexo = 'M' then 'Homem' when aut.autsexo = 'F' then 'Mulher' else 'A Identificar' end categoria,
                        'Valor LOA' descricao, sum(bed.bedvalor) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where eme.prsano = '$exercicio'        
                group by categoria, descricao
                union all
                select  case when aut.autsexo = 'M' then 'Homem' when aut.autsexo = 'F' then 'Mulher' else 'A Identificar' end categoria,
                        'Valor Após Contingenciamento' descricao, sum(bed.bedvalorpriorizado) valor
                from emendas.emenda eme
                        inner join emendas.autor aut on aut.autid = eme.autid
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                where eme.prsano = '$exercicio'   
                group by categoria, descricao
                order by categoria, descricao desc";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['categoria']][$dado['descricao']] = $dado['valor'];
        }


        $item = 'Gênero';
        if($tipo){
            $colors =  "'#ff0066', '#00BFFF', '#FFD700'";

            $sqlPizza = "select case when aut.autsexo = 'M' then 'Homem' when aut.autsexo = 'F' then 'Mulher' else 'A Identificar' end descricao, sum(bed.bedvalor) valor
                    from emendas.emenda eme        
                            inner join emendas.autor aut on aut.autid = eme.autid
                            inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                            inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                    where eme.prsano = '$exercicio'     
                    group by descricao";
            include APPRAIZ . 'emendas/modulos/principal/grafico_pizza.inc';

        } else {
            include APPRAIZ . 'emendas/modulos/principal/grafico_coluna.inc';
        }
    }

    public function gerarGraficoGnd($exercicio, $tipo = null)
    {
        $sql = "select  gnd.gnddsc categoria, gnd.gndcod, 'Valor LOA' descricao, sum(bed.bedvalor) valor
                from emendas.emenda eme        
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                        inner join emendas.emendadetalhe emd on bed.emdid = emd.emdid        
                        inner join public.gnd gnd on gnd.gndcod = emd.gndcod
                where eme.prsano = '$exercicio'     
                group by categoria, gnd.gndcod, descricao
                union all
                select  gnd.gnddsc categoria, gnd.gndcod, 'Valor Após Contingenciamento' descricao, sum(bed.bedvalorpriorizado) valor
                from emendas.emenda eme        
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                        inner join emendas.emendadetalhe emd on bed.emdid = emd.emdid        
                        inner join public.gnd gnd on gnd.gndcod = emd.gndcod
                where eme.prsano = '$exercicio'     
                group by categoria, gnd.gndcod, descricao
                order by gndcod, categoria, descricao desc";

        $dados = $this->carregar($sql);
        $dados = $dados ? $dados : [];

        $dadosAgrupados = [];
        foreach ($dados as $dado) {
            $dadosAgrupados[$dado['categoria']][$dado['descricao']] = $dado['valor'];
        }

        $item = 'GND';

        if($tipo){
            $sqlPizza = "select  gnd.gnddsc descricao, sum(bed.bedvalor) valor
                    from emendas.emenda eme        
                            inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                            inner join emendas.beneficiariodetalhe bed on bed.benid = ben.benid
                            inner join emendas.emendadetalhe emd on bed.emdid = emd.emdid        
                            inner join public.gnd gnd on gnd.gndcod = emd.gndcod
                    where eme.prsano = '$exercicio'     
                    group by descricao";
            include APPRAIZ . 'emendas/modulos/principal/grafico_pizza.inc';

        } else {
            include APPRAIZ . 'emendas/modulos/principal/grafico_coluna.inc';
        }
    }

    public function atualizarSiconv()
    {
        global $configSiconv;

        $sql = "select eme.*, uno.unocodorcamento
                from emendas.emenda eme
                    inner join public.unidadeorcamentaria uno on uno.unoid = eme.unoid 
                where eme.emeid = {$this->emeid}
        ";
        $emenda = $this->pegaLinha($sql);

        $usuario = $configSiconv->user;
        $senha = $configSiconv->pwd;

        $arrParam = array(
            'usuario' => $usuario,
            'senha' 	=> $senha,
            'url' 	=> WEB_SERVICE_SICONV_URL,
        );
        $obWS = new WSIntegracaoSiconv($arrParam);

        $sql = "delete from siconv.propostaws";
        $this->executar($sql);

        if($emenda){
            $this->recuperarPropostaWs($obWS, $emenda);

            $mSiconv = new Emendas_Model_Siconv();

            $filtro = [
                "coalesce(situacaopropostaws, '') != ''",
                "coalesce(parecerproposta, '') != ''",
            ];

            $mSiconv->atualizar($filtro);
        }

        $this->emeatualizacaosiconv = date('Y-m-d H:i:s');
        $this->salvar();
        $this->commit();
    }

    public function atualizarPrograma()
    {
        global $configSiconv;

        $mPrograma = new Emendas_Model_Programa();
        $programas = $mPrograma->recuperarTodos();
        
        $usuario = $configSiconv->user;
        $senha = $configSiconv->pwd;

        $arrParam = array(
            'usuario' => $usuario,
            'senha' 	=> $senha,
            'url' 	=> WEB_SERVICE_SICONV_URL,
        );
        $obWS = new WSIntegracaoSiconv($arrParam);

        foreach ($programas as $programa){

            $filtro = [
                'prgcodigo'=>$programa['prgcodigo']
            ];

            $mPrograma->atualizar($obWS, $filtro);
            $mPrograma->commit();
            if(count($programas)>1){
                sleep(5);
            }
        }

        ver('FIM', d);
    }

    public function recuperarPropostaWs($obWS, $emenda)
    {
        $filtro = [
            'unocod'=>$emenda['unocodorcamento'],
            'emenumero'=>substr($emenda['emenumero'], 4)
        ];
        $retorno = $obWS->consultarPropostaPorEmenda($filtro);
        
        if(isset($retorno->propostas)){
            $mPropostaWS = new Siconv_Model_PropostaWs();

            $aPropostas = is_array($retorno->propostas) ? $retorno->propostas : [$retorno->propostas];
            foreach ($aPropostas as $PropostaWS){

                foreach ($PropostaWS as $indice => $dados){
                    $indice = strtolower($indice);

                    if(strpos($indice, 'data')!== false){

                    } elseif(
                        'ws' == substr($indice, -2) ||
                        in_array($indice, ['parecerplanotrabalho', 'parecerproposta'])
                    ){
                        $dados = json_encode($dados);
                    }

                    if(key_exists($indice, $mPropostaWS->arAtributos)){
                        $mPropostaWS->{$indice} = $dados;
                    }
                }

                $mPropostaWS->emeid = $emenda['emeid'];
                $mPropostaWS->salvar();
                $mPropostaWS->commit();
                $mPropostaWS->id = null;
            }
        }
    }

}//end Class
?>