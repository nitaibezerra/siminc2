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
        'acaid' => array('tabela' => 'acao', 'pk' => 'acaid'),
        'unoid' => array('tabela' => 'unidadeorcamentaria', 'pk' => 'unoid'),
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
        'acaid' => null,
        'emejustificativa' => null,
        'emeimpositiva' => null,
        'autid' => null,
        'prsano' => null,
        'emestatus' => null,
    );

    public function recuperarListagem($exercicio)
    {
        $sql = "select  eme.emeid, eme.emenumero, eme.unoid, eme.acaid, eme.emeimpositiva, eme.autid, eme.prsano, uno.unocod || ' - ' || uno.unonome unonome, 
                        aut.autnome, aut.autcod, aca.acacod, aca.prgcod || '.' || aca.acacod || '.' || aca.acaobjetivocod || '.' || aca.loccod || ' - ' || aca.acatitulo funcional
                from emendas.emenda eme
                    inner join public.unidadeorcamentaria uno on uno.unoid = eme.unoid
                    inner join emendas.autor aut on aut.autid  = eme.autid
                    inner join monitora.acao aca on aca.acaid = eme.acaid
                where eme.emestatus = 'A'        
                and eme.prsano = '$exercicio'
                order by aut.autnome";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
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
                    $autid = $this->_recuperarAutorImportacao($line[0], $_SESSION['exercicio']);

                    // Ação
                    $acoes = $this->_recuperarAcaoImportacao($line[7], $line[6], $line[8], $unocod, $_SESSION['exercicio']);

                    if(!$acoes || !$autid){ continue; }

                    $this->emenumero = $emenumero;
                    $this->unoid = $unoid;
                    $this->acaid = $acoes[0]['acaid'];
                    $this->autid = $autid;
                    $this->prsano = $_SESSION['exercicio'];
                    $this->salvar();
                }

                $this->_inserirDetalhesImportacao($line);
                $this->_inserirBeneficiarioImportacao($line);
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

    private function _recuperarAutorImportacao($dado, $exercicio)
    {
        $autor = explode('-', $dado);
        $autcod = trim($autor[0]);
        $sql = "select autid from emendas.autor where autcod = '{$autcod}' order by autstatus";
        return $this->pegaUm($sql);
    }

    private function _recuperarAcaoImportacao($acao, $programa, $localizador, $unocod, $exercicio)
    {
        $acacod = substr(trim($acao), 0, 4);
        $prgcod = substr(trim($programa), 0, 4);
        $loccod = substr(trim($localizador), 0, 4);
        $sql = "select acaid from monitora.acao where acacod = '$acacod' and prgcod = '$prgcod' and loccod = '$loccod' and unicod = '$unocod' and prgano = '{$exercicio}'";

        $acoes = $this->carregar($sql);

        if(count($acoes)> 1){
            ver('Mais de uma ação', d);
        }

        return $acoes;
    }

    private function _inserirDetalhesImportacao($line)
    {
        $mEmendaDetalhe = new Emendas_Model_EmendaDetalhe();
        $mEmendaDetalhe->emeid = $this->emeid;
        $mEmendaDetalhe->foncod = $line[9];
        $mEmendaDetalhe->gndcod = $line[12];
        $mEmendaDetalhe->mapcod = str_replace('1', '0', $line[13]);
        $mEmendaDetalhe->emdvalor = $line[18] + $line[20];

        return $mEmendaDetalhe->salvar();
    }

    private function _inserirBeneficiarioImportacao($line)
    {
        $proponente = explode('-', $line[15]);

        if(count($proponente) == 2){
            $procnpj = trim($proponente[0]);
            $sql = "select proid from emendas.proponente where procnpj = '{$procnpj}' order by prostatus";

        } else {
            $sql = "select proid from emendas.proponente where procnpj = '000' order by prostatus";
        }

        $proid = $this->pegaUm($sql);
        if(!$proid) return;

        $mBeneficiario = new Emendas_Model_Beneficiario();
        $mBeneficiario->emeid = $this->emeid;
        $mBeneficiario->proid = $proid;

        return $mBeneficiario->salvar();
    }

}//end Class
?>