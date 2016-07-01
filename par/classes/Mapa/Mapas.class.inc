<?php

/**
 * classe Mapas
 *
 * @author Sávio Resende <savio@savioresende.com.br>
 * 
 * Objeto que retorna informações (dados em formato geoJSON) direcionadas a popular os mapas
 */
class Mapas extends Modelo {

    public $Solicitacao;
    protected $Joins = array();
    // atributos ligados a requisicao
    public $origemRequisicao;
    public $params;
    public $chamadoMapas;
    // Partes dos Mapas
    protected $Poligonos;
    protected $MetaDados;

    /**
     * 	Construct
     */
    public function __construct() {
        $this->Poligonos = new Poligonos();
        $this->MetaDados = new MetaDados();
    }

    /**
     * Retorna o resultado da requisicao
     * 
     * @todo testar
     */
    public function requisicaoPost() {

        if ($this->params['estado'])
            $this->params['estado'] = $this->garanteQueSejaArray($this->params['estado']);

        if ($this->params['muncod'])
            $this->params['muncod'] = $this->garanteQueSejaArray($this->params['muncod']);

        $this->construtorPoligonos(); // montaEstado, montaEstadoLegenda ...

        $this->construtorMetaDados(); // aplica cores aos dados em json.
        // escreve resultado
        ob_clean();
        echo $this->Poligonos->poligono;
        die();
    }

    /**
     * Construtor de Poligonos
     *
     * @internal aqui encapsulam as regras da construção de polígonos 
     * 			 - praticamente baseadas em $this->origemRequisicao
     * @internal Factory Method
     * @return void
     * @internal preenche os respectivos objetos que compoem a solicitação
     */
    protected function construtorPoligonos() {

        switch ($this->origemRequisicao) {
            case 'mapa-base-nacional-comum':
            case 'assessoramento':
            case 'assessoramento-externo':
                $this->Poligonos->estuf  = $this->params['estado'];
                $this->Poligonos->tpmid  = $this->params['tpmid'];
                $this->Poligonos->muncod = $this->params['muncod'];
                $this->Poligonos->getMunicipios();
                break;

            case 'base-nacional-por-tipo':
                $this->Poligonos->tpmid = $this->params['tpmid'];
                $this->Poligonos->getMunicipiosPorTipo();
                break;

            case 'assessoramento-estado':
            case 'sase-mapas-estado-externo':
                $this->Poligonos->estuf = $this->params['estado'];
                $this->Poligonos->getEstado();
                break;


            case 'questoespontuais':
            case 'questoespontuais-externo':
                $this->Poligonos->estuf = $this->params['estado'];
                $this->Poligonos->getMunicipios();
                break;

            case 'organizacoesterritoriais':
            case 'organizacoesterritoriais-externo':
                $this->Poligonos->estuf = $this->params['estado'];
                $this->Poligonos->orgid = $this->params['orgid'];
                $this->Poligonos->getMunicipios();
                break;

            case 'pais-externo':
                $this->Poligonos->getPais();
                break;
        }
    }

    /**
     * Construtor de MetaDados
     *
     * @internal aqui encapsulam as regras da construção de metadados
     * @internal Factory Method
     * @internal método $this->MetaDados->aplicaCoresAosMunicipios adicionado no switch pq para 
     * 			 cada origem pode demandar uma requisicao diferente.
     * @return void
     */
    protected function construtorMetaDados() {
//		ver($this->origemRequisicao,d);
        switch ($this->origemRequisicao) {
            case 'assessoramento-estado':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado('DadoSituacaoAssessoramentoEstado');
                $this->MetaDados->aplicaCoresAosEstados($this->Poligonos);
                break;

            case 'mapa-base-nacional-comum':
            case 'assessoramento':
                $this->MetaDados->estuf  = $this->params['estado'];
                $this->MetaDados->tpmid  = $this->params['tpmid'];
                $this->MetaDados->muncod = $this->params['muncod'];
                $this->MetaDados->montaDado('DadoSituacaoAssessoramento');


                $this->MetaDados->aplicaCoresAosMunicipios($this->Poligonos);
                break;

            case 'base-nacional-por-tipo':
                $this->MetaDados->tpmid = $this->params['tpmid'];
                $this->MetaDados->montaDado('DadoSituacaoMunPorTipo');

                $this->MetaDados->aplicaCoresAosMunicipios($this->Poligonos);
                break;

            case 'assessoramento-externo':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado('DadoMacroCategoria');
                $this->MetaDados->aplicaCoresAosMunicipios($this->Poligonos);
                break;

            case 'questoespontuais':
            case 'questoespontuais-externo':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->montaDado('DadoSituacaoQuestoesPontuais');
                $this->MetaDados->aplicaCoresAosMunicipios($this->Poligonos);
                break;

            case 'organizacoesterritoriais':
            case 'organizacoesterritoriais-externo':
                $this->MetaDados->estuf = $this->params['estado'];
                $this->MetaDados->orgid = $this->params['orgid'];
                $this->MetaDados->montaDado('DadoMesoregiao');
                $this->MetaDados->aplicaCoresAosMunicipios($this->Poligonos);
                break;

            case 'pais-externo':
                $this->MetaDados->montaDado('DadoEstadosPais');
                $this->MetaDados->aplicaCoresAosEstados($this->Poligonos);
                break;
        }
    }

    /**
     * Garante que variável seja array
     *
     * @param $var 
     * @return $array
     */
    protected function garanteQueSejaArray($var) {
        if (
                $var && !is_array($var)
        )
            $var = array($var);

        return $var;
    }

    /**
     *
     */
    public function requisicaoPostLegenda() {
        global $db;

        if (count($this->params['estados']) > 0) {
            ob_clean();
            switch ($this->origemRequisicao) {
                case 'assessoramento-legenda':
                    echo '	<div id="legendaMapaContainer">';

                    /**
                     * Tabela com as respostas dos municípios selecionados
                     */
                    $municipio = $this->params['municipios'];
                    // Verifica se o retorno da combo de municípios é um array, se não for ele cria um array com o valor, somente para manter o mesmo código.
                    if(!is_array($municipio)){
                        $municipio = array($municipio);
                    }

                    if (count($municipio) > 0 && $municipio[0] != '' && !empty($municipio[0])){

                        foreach ($municipio as $muncod) {

                            $sql = "select
                                    legenda,
                                    mundescricao,
                                    case
                                        when legenda = 'Sim' then 'green'
                                        when legenda = 'Não' then 'red'
                                        else 'black'
                                    end as cor,
                                    COUNT(legenda) as quantidade
                                from (
                                    select
                                        case
                                            when (bncperg1 =  'a' or bncperg2 =  'a'  or bncperg3 =  'a' or   bncperg4 = 'a' or bncperg5 = 'a') then 'Sim'
                                            when bncid is null then 'Não respondeu'
                                            else  'Não'
                                        end as legenda,
                                        mun.mundescricao as mundescricao
                                    from territorios.municipio mun
                                    inner join par.instrumentounidade iu on mun.muncod = iu.muncod
                                    left join par.basenacionalcomum bn on iu.inuid = bn.inuid
                                    where mun.muncod = '{$muncod}'
                                ) as cor
                                group by legenda, mundescricao order by cor";
                            $corMuncod = $db->carregar($sql);

                            echo "<span style=\"color: " . $corMuncod['0']['cor'] . "; font-size: 18px;\" >Respostas - " . $corMuncod['0']['mundescricao'] . "</span></b><ul>";

                            $sql = "select
                                        bn.bncid,
                                        bna.bnaid,
                                        bna.arqid,
                                        arq.arqnome,
                                        arq.arqdescricao,
                                        mun.muncod,
                                        mun.mundescricao,
                                        bncperg1,
                                        bncperg2,
                                        bncperg3,
                                        bncperg4,
                                        bncperg5
                                        from par.basenacionalcomum bn
                                            inner join par.instrumentounidade iu on iu.inuid = bn.inuid
                                            --left join territorios.municipio mun on mun.muncod = iu.muncod
                                            left  join par.basenacionalcomumarquivo bna on bna.bncid = bn.bncid
                                            left  join public.arquivo arq on arq.arqid = bna.arqid
                                            right join territorios.municipio mun on mun.muncod = iu.muncod
                                        where mun.muncod = '{$muncod}'";
                            $dados = $db->carregar($sql);

                            $dadosA = array();
                            $i = 0;
                            if(is_array($dados)){
                                foreach ($dados as $dado) {
                                    if ($dado['arqid'] != ''){
                                        $dadosA[$i]['arqid'] = $dado['arqid'];
                                        $dadosA[$i]['arqnome'] = $dado['arqnome'];
                                        $dadosA[$i]['muncod'] = $dado['muncod'];
                                        $i++;
                                    }
                                }
                            }

                            $trCons = <<<HTML
                                        <tr>
                                            <td>[descPer]</td>
                                            <td>[respPer]</td>
                                        </tr>
HTML;

                            $resp = "";

                            if($dados[0]['bncperg1'] == 'a'){
                                $resp .= str_replace('[respPer]', 'Sim', str_replace('[descPer]', 'Educaçao Infantil', $trCons));
                            }
                            if($dados[0]['bncperg2'] == 'a' || $dados[0]['bncperg4'] == 'a'){
                                $resp .= str_replace('[respPer]', 'Sim', str_replace('[descPer]', 'Ensino Fundamental', $trCons));
                            }

                            if($dados[0]['bncperg3'] == 'a'){
                                $resp .= str_replace('[respPer]', 'Sim', str_replace('[descPer]', 'Ensino Médio', $trCons));
                            }

                            if($dados[0]['bncperg5'] == 'a'){
                                $resp .= str_replace('[respPer]', 'Sim', str_replace('[descPer]', 'Educação de Jovens e Adultos', $trCons));
                            }

                            if($resp == ''){
                                $resp = <<<HTML
                                    <tr>
                                        <td>
                                            <span style="font-size: 18px;" >Não informado</span>
                                        </td>
                                    </tr>
HTML;
                            }

                            echo <<<HTML
                                <table class='table table-striped table-bordered table-hover tabela-listagem'>
                                    {$resp}
                                </table>
HTML;

                            require_once APPRAIZ."/includes/library/simec/Listagem.php";
                            $list = new Simec_Listagem();
                            $list->setTamanhoPagina(50);
                            $list->setCabecalho(array(
                                'Arquivos'
                            ));
                            $list->esconderColunas(array(
                                'arqid',
                                'muncod'
                            ));
                            $list->addAcao('download', array(
                                'func' => 'downloadArquivo',
                                'titulo' => 'Download do arquivo',
                                'extra-params' => array(
                                    'arqid',
                                    'muncod'
                                )
                            ));
                            $list->setDados($dadosA);
                            $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
                            $list->render();

                            echo <<<HTML
                                </ul>
                                <hr style='border: 1px #aaa  solid; display:  block;'><b>
HTML;

                        }
                    }

		    echo '
		    <div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div><ul>';
                    
                    $sql = "select *,
	case 
		when legenda = 'Sim' then 'green'
		when legenda = 'Não' then 'red'
		else 'white'
	end as cor, COUNT(legenda) as quantidade
 from (
 select  case when (bncperg1 =  'a' or bncperg2 =  'a'  or bncperg3 =  'a' or   bncperg4 = 'a' or bncperg5 = 'a') then 'Sim' 
when bncid is null then 'Não respondeu'
else  'Não' end as legenda
           from territorios.municipio mun 
                  inner join par.instrumentounidade iu on mun.muncod = iu.muncod
                   left join par.basenacionalcomum bn on iu.inuid = bn.inuid
                  " . (($this->params['estados'] != '' && count($this->params['estados']) > 0) ? " WHERE MUN.ESTUF IN ( '" . (implode("','", $this->params['estados'])) . "' ) " : "") . "

) as cor
group by legenda order by cor";
                    $lista = $db->carregar($sql);

                    foreach ($lista as $key => $value) {
                        echo "<li ><table><tr><td><span style='background:" . $value['cor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . (($value['quantidade'] != '') ? $value['quantidade'] : '0') . "</b>&nbsp;&nbsp;</td><td>" . $value['legenda'] . "</td></tr></table></li>";
                    }
                    echo '</ul>';
                    
                    $estado = $this->params['estados'];

                    if ($estado) {
                        foreach ($estado as $listaEstado) {
                            $sqlCorEstado = "select e.estuf || ' - ' || estdescricao as estado,
	case when (bncperg1 =  'a' or bncperg2 =  'a'  or bncperg3 =  'a' or   bncperg4 = 'a' or bncperg5 = 'a') then 'green' 
        when (bncperg1 is null and bncperg2 is null and bncperg3 is null and bncperg4 is null and bncperg5 is null) then 'black' 
	else  'red' end as cor
from territorios.estado e
inner join par.instrumentounidade iu on iu.estuf = e.estuf and (itrid = 1 or iu.estuf = 'DF')
left join par.basenacionalcomum bn on bn.inuid = iu.inuid
where iu.estuf = '$listaEstado'
order by cor";
                            $corEstado = $db->carregar($sqlCorEstado);

                            $sql = "select *,
	case 
		when legenda = 'Sim' then 'green'
		when legenda = 'Não' then 'red'
		else 'white'
	end as cor, COUNT(legenda) as quantidade
 from (
 select  case when (bncperg1 =  'a' or bncperg2 =  'a'  or bncperg3 =  'a' or   bncperg4 = 'a' or bncperg5 = 'a') then 'Sim' 
when bncid is null then 'Não respondeu'
else  'Não' end as legenda
           from territorios.municipio mun 
                  inner join par.instrumentounidade iu on mun.muncod = iu.muncod
                   left join par.basenacionalcomum bn on iu.inuid = bn.inuid
                  WHERE MUN.ESTUF = '{$listaEstado}'

) as cor
group by legenda order by cor";
                            $listaEstados = $db->carregar($sql);
                            
                            echo "<hr style='border: 1px #aaa  solid; display:  block;'><b><font color='" . $corEstado['0']['cor'] . "'>" . $corEstado['0']['estado'] . "</font></b><ul>";
                            
                            foreach ($listaEstados as $key => $value) {
                                echo "<li ><table><tr><td><span style='background:" . $value['cor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . (($value['quantidade'] != '') ? $value['quantidade'] : '0') . "</b>&nbsp;&nbsp;</td><td>" . $value['legenda'] . "</td></tr></table></li>";
                                                           
                            }
                             $sqlQuadro = "select  sum(case when (bncperg1 =  'a') then 1 else 0 end) as infantil,
	sum(case when (bncperg2 =  'a' or bncperg4 =  'a') then 1 else 0 end) as fundamental,
	sum(case when (bncperg3 =  'a') then 1 else 0 end) as medio
from territorios.municipio mun 
	inner join par.instrumentounidade iu on mun.muncod = iu.muncod
	inner join par.basenacionalcomum bn on iu.inuid = bn.inuid
WHERE MUN.ESTUF IN ('{$listaEstado}')
and (
	bncperg1 =  'a' or
	bncperg2 =  'a' or
	bncperg3 =  'a' or
	bncperg4 =  'a' or
	bncperg5 =  'a'
)";
$quadro = $db->carregar($sqlQuadro);
$totalSim = $quadro[0]['infantil'] + $quadro[0]['fundamental'] + $quadro[0]['medio'];
echo "<table class='table table-striped table-bordered table-hover tabela-listagem'><tr><td colspan='2'>Total: ".$totalSim."</td></tr><tr><td>Ensino Infantil</td><td>".$quadro[0]['infantil']."</td></tr><tr><td>Ensino Fundamental</td><td>".$quadro[0]['fundamental']."</td></tr><tr><td>Ensino Médio</td><td>".$quadro[0]['medio']."</td></table>";
                            echo "</ul>";
                        }
                    }

                    echo '</div>
</div>';
                    break;
            }
            die();
        }
    }

    /**
     * Componentes em _componentes.php
     *
     * @param $componentes - nome da função
     * @param $params - parametros a serem passados para funcao
     */
    public function componente($componente, $params) {
        ver($componente, $params, d);
        $funcao = $componente . '("' . implode('","', $params) . '");';
        eval($funcao);
    }

}

// ############################################################################ CAPTURA DE REQUISICAO POST
$chamadoMapas = new Mapas();

if ($_POST['chamadoMapas']) {
    // if( $_POST['origemRequisicao']=='sase-mapas-estado-externo' ){ print_r($_POST['params']);exit; }

    $chamadoMapas->origemRequisicao = $_POST['origemRequisicao'];
    $chamadoMapas->params = $_POST['params'];
    $chamadoMapas->chamadoMapas = $_POST['chamadoMapas'];

    if ($_POST['chamadoMapas'] == 'legenda') {
        $chamadoMapas->requisicaoPostLegenda();
    } else if ($_POST['chamadoMapas'] == 'componente-buscamunicipios') {
        $chamadoMapas->componente('BuscaMunicipio', array('estuf' => $chamadoMapas->params));
    } else
        $chamadoMapas->requisicaoPost();
}
// ########################################################################################################

