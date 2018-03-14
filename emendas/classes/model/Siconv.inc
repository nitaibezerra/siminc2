<?php
/**
 * Classe de mapeamento da entidade emendas.siconv
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_Siconv
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_Siconv extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.siconv';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'sicid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sicid' => null,
        'sicobjeto' => null,
        'sicproposta' => null,
        'sicdata' => null,
        'sitid' => null,
        'prsano' => null,
        'sicsequencial' => null,
        'idhash' => null,
        'numeroconvenio' => null,
        'sicjustificativa' => null,
    );

    public function recuperarListagem($benid)
    {
        $sql = "select sic.*, sit.sitdsc
                from emendas.siconvbeneficiario sib
                        inner join emendas.siconv sic on sic.sicid = sib.sicid
                        left join emendas.siconvsituacao sit on sit.sitid = sic.sitid
                where sib.benid = ". (int)$benid. "
                order by sicsequencial desc
        ";
        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

    public function atualizar($filtro)
    {
        $mPropostaWS = new Siconv_Model_PropostaWs();
        $campos = 'justificativa, situacaopropostaws, parecerproposta, ano, idhash, objetoconvenio, sequencial, numeroconvenio, propostaprogramaws, parecerplanotrabalho';

        $aPropostas = $mPropostaWS->recuperarTodos($campos, $filtro);

        foreach($aPropostas as $proposta){

            // SITUAÇÃO
            $sitid = $this->_getSituacao($proposta['situacaopropostaws']);

            // ATUALIZA SICONV
            $sql = "select sicid from emendas.siconv where sicsequencial = " . (int) $proposta['sequencial'];
            $sicid = $this->pegaUm($sql);
            $mSiconv = new Emendas_Model_Siconv($sicid);

            $mSiconv->sitid = $sitid;
            $mSiconv->prsano = $proposta['ano'];
            $mSiconv->idhash = $proposta['idhash'];
            $mSiconv->sicobjeto = $proposta['objetoconvenio'];
            $mSiconv->sicsequencial = $proposta['sequencial'];
            $mSiconv->numeroconvenio = $proposta['numeroconvenio'];
            $mSiconv->sicjustificativa = $proposta['justificativa'];

            $mSiconv->salvar();

            // PARECER
            $this->_cadastrarParecer($mSiconv->sicid, $proposta);

            // VINCULAR BENEFICIÁRIOS
            $this->_vincularBeneficiario($mSiconv, $proposta);

            // VINCULAR PROGRAMAS
            $this->_vincularPrograma($mSiconv, $proposta);

            $mSiconv->commit();
            $mSiconv->sicid = null;
        }
    }

    private function _getSituacao($situacaopropostaws)
    {
        $situacao = json_decode($situacaopropostaws);

        $sql = "select sitid from emendas.siconvsituacao where sitcodigo = '{$situacao->value}'";
        $sitid = $this->pegaUm($sql);

        if(!$sitid){
            $mSiconvSituacao = new Emendas_Model_SiconvSituacao();

            $mSiconvSituacao->sitdsc = $situacao->value;
            $mSiconvSituacao->sitcodigo = $situacao->value;

            $mSiconvSituacao->salvar();
            $sitid = $mSiconvSituacao->sitid;
        }

        return $sitid;
    }

    private function _cadastrarParecer($sicid, $proposta)
    {
        $aParecer[Emendas_Model_SiconvParecer::K_TIPO_PROPOSTA] = json_decode($proposta['parecerproposta']);
        $aParecer[Emendas_Model_SiconvParecer::K_TIPO_PLANO_TRABALHO] = json_decode($proposta['parecerplanotrabalho']);

        $aCampos['data'] = [Emendas_Model_SiconvParecer::K_TIPO_PROPOSTA => 'dataWS', Emendas_Model_SiconvParecer::K_TIPO_PLANO_TRABALHO => 'data'];

        foreach($aParecer as $spatipo => $pareceres){
            if(is_array($pareceres)){
                foreach($pareceres as $parecer){

                    $sql = "select * from emendas.siconvparecer where idhash = '{$parecer->idHash}' and sicid = '". (int)$sicid. "'";
                    $spaid = $this->pegaUm($sql);

                    $mSiconvParecer = new Emendas_Model_SiconvParecer($spaid);
                    $mSiconvParecer->sicid = $sicid;
                    $mSiconvParecer->spadsc = $parecer->parecer;
                    $mSiconvParecer->spadata = $parecer->{$aCampos['data'][$spatipo]};
                    $mSiconvParecer->idhash = $parecer->idHash;
                    $mSiconvParecer->spatipo = $spatipo;

                    $mSiconvParecer->salvar();
                    unset($mSiconvParecer);
                }
            }
        }
    }

    private function _vincularBeneficiario($mSiconv, $proposta)
    {
        $aPrograma = json_decode($proposta['propostaprogramaws']);

        $aPrograma = is_array($aPrograma) ? $aPrograma : [$aPrograma];
        foreach($aPrograma as $programa){

            $origemRecursoPropProgramaWS = $programa->origemRecursoPropProgramaWS;
            $origemRecursoPropProgramaWS = is_array($origemRecursoPropProgramaWS) ? $origemRecursoPropProgramaWS : [$origemRecursoPropProgramaWS];

            foreach($origemRecursoPropProgramaWS as $propProgramaWS){

                $cnpjProgramaEmendaWS = isset($propProgramaWS->cnpjProgramaEmendaWS) ? $propProgramaWS->cnpjProgramaEmendaWS : $propProgramaWS;

                // BENEFICIÁRIO
                $procnpj = $cnpjProgramaEmendaWS->cnpj;
                $emenumero = $proposta['ano'] . $cnpjProgramaEmendaWS->numeroEmendaParlamentar;

                $sql = "select ben.benid
                    from emendas.emenda eme
                        inner join emendas.beneficiario ben on ben.emeid = eme.emeid
                        inner join emendas.proponente pro on pro.proid = ben.proid
                    where eme.emenumero = '{$emenumero}'
                    and pro.procnpj = '{$procnpj}'";
                $benid = $this->pegaUm($sql);

                $sql = "select 1 from emendas.siconvbeneficiario where emenumero = '{$emenumero}' and procnpj = '{$procnpj}' and sicid = {$mSiconv->sicid}";
                $existeSiconv = $this->pegaUm($sql);
                if(!$existeSiconv){
                    $mSiconvBeneficiario = new Emendas_Model_SiconvBeneficiario();
                    $mSiconvBeneficiario->sicid = $mSiconv->sicid;
                    $mSiconvBeneficiario->emenumero = $emenumero;
                    $mSiconvBeneficiario->procnpj = $procnpj;
                    $mSiconvBeneficiario->benid = (int)$benid;
                    $mSiconvBeneficiario->salvar();
                }

                if(!$benid){
                    $aBen[$benid][] = $sql;
                    continue;
                }

                $mBeneficiario = new Emendas_Model_Beneficiario($benid);

                $mBeneficiario->sicid = $mSiconv->sicid;
                $mBeneficiario->salvar();
            }
        }
    }

    private function _vincularPrograma($mSiconv, $proposta)
    {
        $aPrograma = json_decode($proposta['propostaprogramaws']);

        $aPrograma = is_array($aPrograma) ? $aPrograma : [$aPrograma];
        foreach($aPrograma as $programa){

            $prgid = (new Emendas_Model_Programa())->getPrograma($programa->programaWS);

            $sql = "select 1 from emendas.siconvprograma where prgid = '{$prgid}' and sicid = {$mSiconv->sicid}";
            $existeSiconv = $this->pegaUm($sql);
            
            if(!$existeSiconv){
                $mSiconvPrograma= new Emendas_Model_SiconvPrograma();
                $mSiconvPrograma->sicid = $mSiconv->sicid;
                $mSiconvPrograma->prgid = $prgid;
                $mSiconvPrograma->salvar();
            }
        }
    }

}//end Class
?>