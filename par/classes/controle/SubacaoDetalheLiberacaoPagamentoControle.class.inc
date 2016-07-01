<?php 
class SubacaoDetalheLiberacaoPagamentoControle extends Controle
{

	public function __construct()
	{
		parent::__construct();
	} 
        
        public function salvarValorLiberacao($arrPost = array()) {
            try {
                #Verifica se possui dados no post
                if (count($arrPost['sbdids']) > 0) {
                    #Processa o Array de Subacoes
                    foreach ($arrPost['sbdids'] as $sbdid => $valor) {
                        $valor = str_replace('.', '', $valor);
                        $valor = str_replace(',', '.', $valor);
                        $obSubacaoDetalheLiberacaoPagamento = new SubacaoDetalheLiberacaoPagamento();
                        $arrResult = $obSubacaoDetalheLiberacaoPagamento->findBy(
                            array('*'),
                            array("sbdid='{$sbdid}'"),
                            array("order by sdpid desc", "limit 1")
                        );
                        $sdpvalorliberado = $valor;
                        $sdpvalortotal = $valor;
                        if ($arrResult != '') {
                            $sdpvalorliberado = ($valor - $arrResult[0]['sdpvalortotal']);
                            $sdpvalortotal = $valor;
                            $obSubacaoDetalheLiberacaoPagamento->alterarStatusValorLiberacaoSbdid($sbdid, 'I');
                        }

                        $arrParam = array(
                            'sbdid' => $sbdid,
                            'sdpvalortotal' => $sdpvalortotal,
                            'sdpvalorliberado' => $sdpvalorliberado,
                            'sdpdata' => date('Y-m-d G:i:s'),
                            'usucpf' => $_SESSION['usucpf'],
                            'sdpstatus' => 'A'
                        );
                        $obSubacaoDetalheLiberacaoPagamento->salvarValorLiberacao($arrParam);
                    }

                    #Processa o Array de Contratos
                    /*if (count($arrPost['conids']) > 0) {
                        $objContratos = new Contratos();
                        foreach ($arrPost['conids'] as $conid) {
                            $objContratos->alterarValidacaoContrato($conid, TRUE);
                        }
                    }*/
                    #Processa o Array de Contratos
                    if (is_array($arrPost['ntfids'])) {
                        if (count($arrPost['ntfids']) > 0) {
                            $objNotasFiscais = new NotasFiscais();
                            foreach ($arrPost['ntfids'] as $ntfid) {
                                $objNotasFiscais->alterarValidacaoNotasFiscais($ntfid, TRUE);
                            }
                        }
                    }

                    $this->db->commit();
                    return true;
                } else {
                    return false;
                }
            } catch (Exception $exception) {
                $this->db->rollback();
                echo $exception->getTraceAsString();
            }
        }
	
}