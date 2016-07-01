<?php

/**
 * Description of Periodo
 *
 * @author LindalbertoFilho
 */
class Acomporc_Service_Periodo extends Spo_Service_Abstract
{
    /**
     * @see Acomporc_Model_PeriodoReferencia
     */
    protected $modelPeriodoReferencia;

    /**
     * Values: A, S, T;
     * @var char
     */
    protected $dados = array(
        'tipoAcesso' => null,
        'periodoAtual' => null,
        'periodo' => null,
        'perfil' => null,
        'statusAtual' => null
    );

    public function __construct()
    {

    }

    /**
    * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
    * @param type $arDados
    * @return string
    */
   function salvarPeriodoReferencia($arDados)
   {
       $this->modelPeriodoReferencia = new Acomporc_Model_PeriodoReferencia();
       $this->modelPeriodoReferencia->popularDadosObjeto($arDados);
       $resultado['msg'] = 'Falha ao cadastrar Período de referência.';
       $resultado['type'] = Simec_Helper_FlashMessage::ERRO;
       if ($this->modelPeriodoReferencia->inserir()){
           $resultado['msg'] = 'Período de referência cadastrado com sucesso!';
           $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
       }
       $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
   }

   /**
    * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
    * @param type $id
    * @return array()
    */
   function buscarPeriodoReferencia($id)
   {
       $this->modelPeriodoReferencia = new Acomporc_Model_PeriodoReferencia();
       $this->modelPeriodoReferencia->__set('prfid',$id);
       return $this->modelPeriodoReferencia->buscaPorId();
   }

   /**
    * @author Lindalberto Filho <lindalbertorvcf@gmail.com>
    * @param type $arDados
    * @return array()
    */
   function atualizarPeriodoReferencia($arDados)
   {
       $this->modelPeriodoReferencia = new Acomporc_Model_PeriodoReferencia();
       $this->modelPeriodoReferencia->popularDadosObjeto($arDados);
       $resultado['msg'] = 'Falha ao atualizar Período de referência.';
       $resultado['type'] = Simec_Helper_FlashMessage::ERRO;
       if ($this->modelPeriodoReferencia->alterar()){
           $resultado['msg'] = 'Período de referência atualizado com sucesso!';
           $resultado['type'] = Simec_Helper_FlashMessage::SUCESSO;
       }
       $this->getFlashMessage()->addMensagem($resultado['msg'], $resultado['type']);
   }

    public function podeSalvar()
    {
        $this->modelPeriodoReferencia = new Acomporc_Model_PeriodoReferencia();
        $this->modelPeriodoReferencia->__set('prftipo',$this->dados['tipoAcesso']);
        $this->dados['periodoAtual'] = $this->modelPeriodoReferencia->carregarAtual();
        return $this->verificaPerfilPorTipo();
    }

    protected function verificaPerfilPorTipo()
    {
        switch($this->dados['tipoAcesso'])
        {
            case 'A':
                return $this->verificaPerfilAcao();
            case 'S':
                return $this->verificaPerfilSubacao();
            case 'T':
                return $this->verificaPerfilRelatorioTCU();
        }
    }

    protected function verificaPerfilSubacao()
    {
        /* Bloqueia o preenchimento fora do período para os usuário UO */
        if (in_array(PFL_COORDENADORSUBACAO, $this->dados['perfil']) && ($this->dados['periodo'] == $this->dados['periodoAtual'])) {
            return $this->verificaPeriodo();
        }else if(in_array(PFL_CGP_GESTAO, $this->dados['perfil']) || in_array(PFL_SUPERUSUARIO, $this->dados['perfil'])){
            return false;
        }else{
            return true;
        }
    }

    protected function verificaPerfilAcao()
    {
        if ((in_array(PFL_COORDENADORACAO, $this->dados['perfil'])) && ($this->dados['periodo'] == $this->dados['periodoAtual'])
            && (!$this->dados['statusAtual'] || $this->dados['statusAtual'] == ESD_EMPREENCHIMENTO || $this->dados['statusAtual'] == ESD_EMELABORACAO || ESD_EMVALIDACAO == $this->dados['statusAtual'])) {
            return $this->verificaPeriodo();
        } elseif ((in_array(PFL_VALIDADORACAO, $this->dados['perfil']) || in_array(PFL_VALIDADOR_SUBSTITUTO, $this->dados['perfil']))
            && ($this->dados['periodo'] == $this->dados['periodoAtual']) && ((!$this->dados['statusAtual']) ||
                ($this->dados['statusAtual'] == ESD_EMPREENCHIMENTO || $this->dados['statusAtual'] == ESD_EMANALISE || $this->dados['statusAtual'] == ESD_EMELABORACAO || ESD_EMVALIDACAO == $this->dados['statusAtual']))) {
            return $this->verificaPeriodo();
        } elseif (in_array(PFL_CGP_GESTAO, $this->dados['perfil']) || in_array(PFL_SUPERUSUARIO, $this->dados['perfil'])) {
            return false;
        } else {
            return true;
        }
    }

    protected function verificaPerfilRelatorioTCU()
    {
        return null;
    }


    protected function verificaPeriodo()
    {
        $this->modelPeriodoReferencia = new Acomporc_Model_PeriodoReferencia();
        $this->modelPeriodoReferencia->__set('prftipo',$this->dados['tipoAcesso']);
        $this->modelPeriodoReferencia->__set('prfid', $this->dados['periodo']);
        return $this->modelPeriodoReferencia->periodoValido();
    }

    public function detalhePeriodo($prfid)
    {
        $this->modelPeriodoReferencia = new Acomporc_Model_PeriodoReferencia();
        $this->modelPeriodoReferencia->__set('prfid', $prfid);
        return (string)$this->modelPeriodoReferencia;
    }

    /**
     * Converte o prfid de um determinado tipo no prfid do mesmo período de outro tipo.
     * @param int $prfid
     * @param char $para (A,S,T)
     * @return int
     */
    public function convertePeriodo($prfid, $para)
    {
        $this->modelPeriodoReferencia = new Acomporc_Model_PeriodoReferencia();
        $this->modelPeriodoReferencia->__set('prfid',$prfid);
        $periodoDe = $this->modelPeriodoReferencia->buscaPorId();

        if($periodoDe['prftipo'] == $para){
            return $periodoDe['prfid'];
        }
        $this->modelPeriodoReferencia->__set('prsano',$periodoDe['prsano']);
        $this->modelPeriodoReferencia->__set('prftipo',$para);
        $this->modelPeriodoReferencia->carregarUltimo();
        return $this->modelPeriodoReferencia->arAtributos['prfid'];
    }
}
