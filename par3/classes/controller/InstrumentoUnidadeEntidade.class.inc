<?php
/**
 * Classe de controle do InstrumentoUnidade
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-09-2015
 * @link     no link
 */

/**
 * Par3_Controller_InstrumentoUnidadeEntidade
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-09-2015
 * @link     no link
 */
class Par3_Controller_InstrumentoUnidadeEntidade extends Controle
{

	public function __construct()
	{
		parent::__construct();
	}


    /**
     * Função salvarInformacoesPrefeitura
     * - salvar informações da prefeitura da unidade.
     *  - se o cnpj for diferente do já salvo ele inativa a atual e insere uma
     * prefeitura nova.
     *
     * @return void.
     *
     */
    public function salvarInformacoesPrefeitura($arrDados)
    {
        $url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid='.$arrDados['inuid'];

        $endereco   = new Par3_Controller_Endereco();
        $prefeitura = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);

        try {
            $arrDados['entcnpj']              = str_replace(array('.','-','/'), '', $arrDados['entcnpj']);
            $arrDados['enttelefonecomercial'] = str_replace(array('(','-',')'), '', $arrDados['enttelefonecomercial']);

            if ($prefeitura->entcnpj != $arrDados['entcnpj']) {
                $prefeitura->inativaEntidadesInuidPorTipo($arrDados['inuid'], $arrDados['tenid']);
                unset($_POST['entid']);
                $prefeitura = new Par3_Model_InstrumentoUnidadeEntidade();
            }

            $arrDados['endid'] = $endereco->salvarEndereco($arrDados);

            $prefeitura->popularDadosObjeto($arrDados);
            $prefeitura->salvar();
            $prefeitura->commit();

            simec_redirecionar($url, 'success');

        } catch (Exception $e){
            $prefeitura->rollback();
            simec_redirecionar($url, 'error');

        }
    }//end salvarInformacoesPrefeitura()


    /**
     * Função verificaPreencimentoPrefeitura
     * - verifica o preenchimento dos dados da prefeitura e retorna um percentual
     * de preenchimento.
     *
     * @return percentual de preenchimento.
     *
     */
    public function verificaPreencimentoPrefeitura($inuid)
    {
        $prefeitura = new Par3_Model_InstrumentoUnidadeEntidade();

        $tenid = Par3_Model_InstrumentoUnidadeEntidade::PREFEITURA;

        $objPrefeitura = $prefeitura->carregarDadosEntidPorTipo($inuid, $tenid);
        $objEndereco   = new Par3_Model_Endereco($objPrefeitura->endid);

        $perc = 0;
        if ($objPrefeitura->entcnpj != '') $perc++;
        if ($objPrefeitura->entnome != '') $perc++;
        if ($objPrefeitura->entrazaosocial != '') $perc++;
        if ($objPrefeitura->entinscricaoestadual != '') $perc++;
        if ($objPrefeitura->entemail != '') $perc++;
        if ($objPrefeitura->entsigla != '') $perc++;
        if ($objPrefeitura->enttelefonecomercial != '') $perc++;

        if ($objEndereco->endcep != '') $perc++;
        if ($objEndereco->endlogradouro != '') $perc++;
        if ($objEndereco->endnumero != '') $perc++;
        if ($objEndereco->endbairro != '') $perc++;

        $perc = $perc/11*100;

        return (int) $perc;

    }//end verificaPreencimentoPrefeitura();


    /**
     * Função salvarInformacoesPrefeito
     * - salvar informações do prefeito da unidade.
     *  - se o cpf for diferente do já salvo ele inativa o atual e insere um prefeito novo.
     *
     * @return void.
     *
     */
    public function salvarInformacoesPrefeito($arrDados)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A"
               ."&inuid={$arrDados['inuid']}&menu=prefeito";

        $endereco = new Par3_Controller_Endereco($arrDados['endid']);
        $prefeito = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);

        try {
            $arrDados['entcpf']               = str_replace(array('.','-','/'), '', $arrDados['entcpf']);
            $arrDados['enttelefonecomercial'] = str_replace(array('(','-',')',' ','_'), '', $arrDados['enttelefonecomercial']);
            $arrDados['enttelefonecelular']   = str_replace(array('(','-',')',' ','_'), '', $arrDados['enttelefonecelular']);
            $arrDados['enttelefonefax']      = str_replace(array('(','-',')',' ','_'), '', $arrDados['enttelefonefax']);

            if ($prefeito->entcpf != $arrDados['entcpf']) {
                $prefeito->inativaEntidadesInuidPorTipo($arrDados['inuid'], $arrDados['tenid']);
                unset($_POST['entid']);
                $prefeito = new Par3_Model_InstrumentoUnidadeEntidade();
            }

            $arrDados['endid'] = $endereco->salvarEndereco($arrDados);

            $prefeito->popularDadosObjeto($arrDados);
            $prefeito->salvar();
            $prefeito->commit();

            simec_redirecionar($url, 'success');

        } catch (Exception $e){
            $prefeito->rollback();
            simec_redirecionar($url, 'error');

        }
    }//end salvarInformacoesPrefeito()


    /**
     * Função verificaPreencimentoPrefeito
     * - verifica o preenchimento dos dados do prefeito e retorna um percentual
     * de preenchimento.
     *
     * @return percentual de preenchimento.
     *
     */
    public function verificaPreencimentoPrefeito($inuid)
    {
        $prefeito = new Par3_Model_InstrumentoUnidadeEntidade();

        $tenid = Par3_Model_InstrumentoUnidadeEntidade::PREFEITO;

        $objPrefeito = $prefeito->carregarDadosEntidPorTipo($inuid, $tenid);
        $objEndereco = new Par3_Model_Endereco($objPrefeito->endid);

        $perc = 0;
        if ($objPrefeito->entcpf != '') $perc++;
        if ($objPrefeito->entnome != '') $perc++;
        if ($objPrefeito->entemail != '') $perc++;
        if ($objPrefeito->enttelefonecomercial != '') $perc++;
        if ($objPrefeito->enttelefonecelular != '') $perc++;

        if ($objEndereco->endcep != '') $perc++;
        if ($objEndereco->endlogradouro != '') $perc++;
        if ($objEndereco->endnumero != '') $perc++;
        if ($objEndereco->endbairro != '') $perc++;

        $perc = $perc/9*100;

        return (int) $perc;

    }//end verificaPreencimentoPrefeito();


    /**
     * Função salvarInformacoesSecretaria
     * - salvar informações da Secretaria da unidade.
     *  - se o cnpj for diferente do já salvo ele inativa a atual e insere uma
     * Secretaria nova.
     *
     * @return void.
     *
     */
    public function salvarInformacoesSecretaria($arrDados)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A"
                ."&inuid={$arrDados['inuid']}&menu=secretaria";

        $endereco   = new Par3_Model_Endereco($arrDados['endid']);
        $Secretaria = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);

        try {
            $arrDados['entcnpj']              = str_replace(array('.','-','/'), '', $arrDados['entcnpj']);
            $arrDados['enttelefonecomercial'] = str_replace(array('(','-',')'), '', $arrDados['enttelefonecomercial']);

            if ($Secretaria->entcnpj != $arrDados['entcnpj']) {
                $Secretaria->inativaEntidadesInuidPorTipo($arrDados['inuid'], $arrDados['tenid']);
                unset($arrDados['entid']);
                $Secretaria = new Par3_Model_InstrumentoUnidadeEntidade();
            }

            if ($endereco->endcep != $arrDados['endcep']) {
                $endereco = new Par3_Controller_Endereco();
            }

            $arrDados['endid'] = $endereco->salvarEndereco($arrDados);

            $Secretaria->popularDadosObjeto($arrDados);
            $Secretaria->salvar();
            $Secretaria->commit();

            simec_redirecionar($url, 'success');

        } catch (Exception $e){
            $Secretaria->rollback();
            simec_redirecionar($url, 'error');

        }
    }//end salvarInformacoesSecretaria()


    /**
     * Função verificaPreencimentoSecretaria
     * - verifica o preenchimento dos dados da Secretaria e retorna um percentual
     * de preenchimento.
     *
     * @return percentual de preenchimento.
     *
     */
    public function verificaPreencimentoSecretaria($inuid)
    {
        $Secretaria = new Par3_Model_InstrumentoUnidadeEntidade();

        $tenid = Par3_Model_InstrumentoUnidadeEntidade::SECRETARIA_EDUCACAO;

        $objSecretaria = $Secretaria->carregarDadosEntidPorTipo($inuid, $tenid);
        $objEndereco   = new Par3_Model_Endereco($objSecretaria->endid);

        $perc = 0;
        if ($objSecretaria->entcnpj != '') $perc++;
        if ($objSecretaria->entnome != '') $perc++;
        if ($objSecretaria->entrazaosocial != '') $perc++;
        if ($objSecretaria->entinscricaoestadual != '') $perc++;
        if ($objSecretaria->entemail != '') $perc++;
        if ($objSecretaria->entsigla != '') $perc++;
        if ($objSecretaria->enttelefonecomercial != '') $perc++;

        if ($objEndereco->endcep != '') $perc++;
        if ($objEndereco->endlogradouro != '') $perc++;
        if ($objEndereco->endnumero != '') $perc++;
        if ($objEndereco->endbairro != '') $perc++;

        $perc = $perc/11*100;

        return (int) $perc;

    }//end verificaPreencimentoSecretaria();


	/**
	 * Função salvarInformacoesDirigente
	 * - salvar informações do dirigente da unidade.
	 *
	 * @return void.
	 *
	 */
	public function salvarInformacoesDirigente($arrDados)
	{
		$url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
				"&inuid={$arrDados['inuid']}&menu=dirigente";

		$endereco  			= new Par3_Controller_Endereco();
		$dirigente 			= new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);
		$unidade   			= new Par3_Model_InstrumentoUnidade($arrDados['inuid']);
		$responsabilidade 	= new Par3_Model_UsuarioResponsabilidade();
		$usuario   			= new Seguranca_Model_Usuario();
		
		$entidade = $unidade->inudescricao;
		$estuf    = $unidade->estuf;
		$itrid    = $unidade->itrid;
		
		try {
			$arrDados['entcpf']			   	  = str_replace(array('.','-','/'), '', $arrDados['entcpf']);
			$arrDados['enttelefonecomercial'] = str_replace(array('(','-',')'), '', $arrDados['enttelefonecomercial']);
			$arrDados['enttelefonecelular']   = str_replace(array('(','-',')'), '', $arrDados['enttelefonecelular']);
			$arrDados['enttelefonefax']  	  = str_replace(array('(','-',')'), '', $arrDados['enttelefonefax']);
			$arrDados['entcursomec']          = ($arrDados['entcursomec'] == 'S') ? 'TRUE' : 'FALSE';
			$arrDados['entstatus'] 			  = 'A';
			$arrDados['entempoatuacao'] 	  = (int) $arrDados['entempoatuacao'];
			$arrDados['entdtnascimento'] 	  = $arrDados['entdtnascimento'] != '' ? $arrDados['entdtnascimento'] : null;

			$pflcod = ($itrid == 1) ? Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL : Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL;


			if (!$responsabilidade->validarUsuarioResponsabilidade($arrDados['entcpf'], $pflcod, $unidade->muncod)) {
				$tipo = ($itrid == 1) ? 'Estado' : 'Município';
				$regiao = ($itrid == 1) ? $entidade : $entidade . ' - ' . $estuf;
				throw new \Exception("Este CPF não possui perfil de dirigente para o {$tipo} {$regiao}");
			}
			
			if ($dirigente->entcpf != $arrDados['entcpf'] || $dirigente->entnome != $arrDados['entnome']) {
				$dirigente->inativaEntidadesInuidPorTipo($arrDados['inuid'], $arrDados['tenid']);
				unset($arrDados['entid']);
				$dirigente = new Par3_Model_InstrumentoUnidadeEntidade();
			}

			$dirigente->popularDadosObjeto($arrDados);
			$dirigente->salvar();
			$dirigente->commit();

			simec_redirecionar($url, 'success');
		} catch (Exception $e) {
			$dirigente->rollback();
			simec_redirecionar($url, 'error', $e->getMessage());
		}
	}//end salvarInformacoesDirigente()


    /**
     * Função salvarInformacoesDirigente
     * - salvar informações do dirigente da unidade.
     *
     * @return void.
     *
     */
    public function salvarInformacoesNutricionistaQuadroTecnico($arrDados)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
            "&inuid={$arrDados['inuid']}&menu=nutricionista";

         try {
            $arrDados['entcpf']		= str_replace(array('.','-','/'), '', $arrDados['usucpf2']);
            $arrDados['entnome'] 	= str_replace(array('(','-',')'), '', $arrDados['entnome2']);
            $arrDados['entemail']   = str_replace(array('(','-',')'), '', $arrDados['entemail2']);

            $nutricionistaQuadroTecnico = new Par3_Model_InstrumentoUnidadeEntidade();

            $nutricionistaQuadroTecnico->popularDadosObjeto($arrDados);

            $nutricionistaQuadroTecnico->salvar();
            $nutricionistaQuadroTecnico->commit();

            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $nutricionistaQuadroTecnico->rollback();
            simec_redirecionar($url, 'error');

        }
    }

    
    /**
     * Função salvarInformacoesDirigente
     * - salvar informações do dirigente da unidade.
     *
     * @return void.
     *
     */
    public function salvarInformacoesNutricionistaReponsavel($arrDados)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
            "&inuid={$arrDados['inuid']}&menu=nutricionista";

        $nutricionista = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid1']);
        $usuario = new Seguranca_Model_Usuario();

        try {
            $arrDados['entcpf']		= str_replace(array('.','-','/'), '', $arrDados['usucpf1']);
            $arrDados['entnome'] 	= str_replace(array('(','-',')'), '', $arrDados['entnome1']);
            $arrDados['entemail']   = str_replace(array('(','-',')'), '', $arrDados['entemail1']);
            $arrDados['tenid']		= 7;

            if (!$usuario->recuperarPorCPF($arrDados['entcpf'])) {
                throw new \Exception("Este CPF não possui cadastro no SIMEC");
            }

            if ($nutricionista != $arrDados['entcnpj']) {
                $nutricionista->inativaEntidadesInuidPorTipo($arrDados['inuid'], $arrDados['tenid']);
                unset($arrDados['entid']);
                $nutricionista = new Par3_Model_InstrumentoUnidadeEntidade();
            }

            $nutricionista->popularDadosObjeto($arrDados);

            $nutricionista->salvar();
            $nutricionista->commit();

            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $nutricionista->rollback();
            simec_redirecionar($url, 'error');

        }
    }//end

    
	/**
	* Função verificaPreencimentoDirigente
	* - verifica o preenchimento dos dados do dirigente e retorna um percentual
	* de preenchimento.
	*
	* @return percentual de preenchimento.
	*
	*/
	public function verificaPreencimentoDirigente($inuid)
	{
		$dirigente = new Par3_Model_InstrumentoUnidadeEntidade();

	    $itrid = Par3_Controller_InstrumentoUnidade::pegarItrid($inuid);
        if ($itrid === '2') {
            $tenid = Par3_Model_InstrumentoUnidadeEntidade::DIRIGENTE;
        } else {
            $tenid = Par3_Model_InstrumentoUnidadeEntidade::SECRETARIO_ESTADUAL_EDUCACAO;
        }

		$objDirigente = $dirigente->carregarDadosEntidPorTipo($inuid, $tenid);

		$perc = 0;

		if ($objDirigente->entcpf != '') $perc++;
		if ($objDirigente->entnome != '') $perc++;
		if ($objDirigente->entemail != '') $perc++;
		if ($objDirigente->entnivelensino != '') $perc++;
		if ($objDirigente->entempoatuacao != '') $perc++;
		if ($objDirigente->enttelefonecelular != '') $perc++;

		$perc = $perc/6*100;

		return (int) $perc;
	}//end verificaPreencimentoDirigente();


	/**
	* Função verificaPreencimentoEquipe
	* - verifica o preenchimento dos dados da equipe local e retorna um percentual
	* de preenchimento.
	*
	* @return integer $perc.
	*
	*/
	public function verificaPreencimentoEquipe($inuid)
	{
		$equipe = new Par3_Model_EquipeLocal();

		$arrPost['inuid']     = $inuid;
		$arrPost['elostatus'] = 'A';
		$arrEquipe = $equipe->carregaArrayEquipe($arrPost);

		$perc = 0;
		$qtd  = 0;
		if (is_array($arrEquipe)) {
            foreach ($arrEquipe as $equipe) {
                $qtd ++;
                foreach ($equipe as $campo) {
                    if ($campo != '') $perc++;
                }
            }
		}

		if ($perc > 0) $perc = $perc/(6*$qtd)*100;

		return (int) $perc;
	}//end verificaPreencimentoEquipe();

    public function recuperarNutricionistasQuadroTecnico()
    {
        $objEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $sql = $objEntidade->recuperarNutricionistasQuadroTecnico();

        $cabecalho = array('CPF', 'Nome', 'E-mail');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addCallbackDeCampo('entcpf','formatar_cpf'); // função php para formatar cpf
        $listagem->addAcao('delete', 'inativaIntegranteEquipe');

        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    }//end listaEquipe()

    
	/**
	 * Função salvarInformacoesConselho
	 * - salvar informações do conselho.
	 *
	 * @return void.
	 *
	 */
    public function salvarInformacoesConselho($arrDados) 
    {
    	$url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
    			"&inuid={$arrDados['inuid']}&menu=conselho";
    	
    	$conselheiro = new Par3_Model_InstrumentoUnidadeEntidade();
    	$endereco  	 = new Par3_Controller_Endereco();
    	$unidade   	 = new Par3_Model_InstrumentoUnidade();
    	
    	try {
    		$arrDados['entcpf']	= str_replace(array('.','-','/'), '', $arrDados['entcpf']);
    	
    		if ($conselheiro->existeConselheiro($arrDados['entcpf'], $arrDados['inuid'])) {
    			throw new \Exception("Este CPF já faz parte do conselho.");
    		}
    			
    		$conselheiro->popularDadosObjeto($arrDados);
    		$conselheiro->salvar();
    		$conselheiro->commit();
    	
    		simec_redirecionar($url, 'success');
    	} catch (Exception $e) {
    		$conselheiro->rollback();
    		simec_redirecionar($url, 'error', $e->getMessage());
    	}
    }//end salvarInformacoesConselho

    
    /**
     * Função inativar conselheiro
     * - inativa membro do conselho.
     *
     * @return void.
     *
     */
    public function inativarConselheiro($arrDados)
    {
    	$url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
    			"&inuid={$arrDados['inuid']}&menu=conselho";

    	$conselheiro = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);
    	
    	try {
    		$conselheiro->entstatus = 'I';
    		$conselheiro->entdtinativacao = date('Y-m-d H:i:s');
    		$conselheiro->salvar();
    		$conselheiro->commit();
    
    		simec_redirecionar($url, 'success');
    		die();
    	} catch(Exception $e) {
    		$conselheiro->rollback();
    		simec_redirecionar($url, 'error');
    		die();
    	}
    }//end inativarConselheiro()
    
}//end Class
?>