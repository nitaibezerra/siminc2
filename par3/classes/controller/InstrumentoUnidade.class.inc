<?php

/**
 * Par_Controle_Instrumentounidade
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-09-2015
 * @link     no link
 */
class Par3_Controller_InstrumentoUnidade extends Controle
{

    public function __construct()
    {
        parent::__construct();
    }


    /**
     * Funзгo lista
     * - monta a lista de instrumentos unidade.
     *
     * @return escreve a lista.
     *
     */
    public function lista()
    {
        $sql = Par3_Model_InstrumentoUnidade::pegarSQLLista();

        $cabecalho = self::cabecalhoListaPrincipal();

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addAcao('edit', 'dadosUnidade');

        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    }//end lista()


    /**
     * Funзгo cabecalhoUnidade
     * - monta o cabeзalho da unidade.
     *
     * @return escreve a lista.
     *
     */
    public function pegarItrid($inuid)
    {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        return $modelInstrumentoUnidade->itrid;

    }//end pegarItrid();


    /**
     * Funзгo pegarNomeEntidade
     * - Monta o nome da entidade.
     *
     * @return escreve o nome da entidade.
     *
     */
    public function pegarNomeEntidade($inuid)
    {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        $entidade = $modelInstrumentoUnidade->inudescricao;
        $estuf    = $modelInstrumentoUnidade->estuf;
        $itrid    = $modelInstrumentoUnidade->itrid;

        return ($itrid == 1) ? $entidade : $entidade . ' - ' . $estuf;
    }


    /**
     * Funзгo cabecalhoListaPrincipal
     * - monta o cabeзalho da lista principal.
     *
     * @return cabeзalho.
     *
     */
    public function cabecalhoListaPrincipal()
    {
       return ($_POST['itrid'] == 2) ? array('Municнpio') : array('Estado');
    }//end cabecalhoListaPrincipal();


    /**
     * Funзгo pegarDescricaoEsfera
     * - monta o cabeзalho da unidade.
     *
     * @return escreve a lista.
     *
     */
    public function pegarDescricaoEsfera($inuid)
    {
        $itrid = self::pegarItrid($inuid);
        return ($itrid === '2') ? 'Municipal' : 'Estadual';
    }//end pegarDescricaoEsfera();


    /**
     * Funзгo cabecalhoUnidade
     * - monta o cabeзalho da unidade.
     *
     * @return escreve a lista.
     *
     */
    public function cabecalhoUnidade()
    {
        include APPRAIZ.'par3/modulos/principal/planoTrabalho/cabecalhoUnidade.inc';

    }//end cabecalhoUnidade();


    /**
     * Funзгo verificaPreencimentoUnidade
     * - verifica o preenchimento da aba dados da unidade e retorna um percentual
     * de preenchimento.
     *
     * @return escreve a lista.
     *
     */
    public function verificaPreencimentoUnidade($inuid)
    {
        $entidade = new Par3_Controller_InstrumentoUnidadeEntidade();

        $preenchimentoPrefeitura = $entidade->verificaPreencimentoPrefeitura($inuid);
        $preenchimentoPrefeito   = $entidade->verificaPreencimentoPrefeito($inuid);
        $preenchimentoSecretaria = $entidade->verificaPreencimentoSecretaria($inuid);
        $preenchimentoDirigente  = $entidade->verificaPreencimentoDirigente($inuid);
        $preenchimentoEquipe     = $entidade->verificaPreencimentoEquipe($inuid);

        $perc = $preenchimentoPrefeitura+$preenchimentoPrefeito+$preenchimentoSecretaria+
                $preenchimentoDirigente+$preenchimentoEquipe;

        $perc = $perc/7;

        return (int) $perc;

    }//end cabecalhoUnidade();


    /**
     * Funзгo desenharTela
     * - desenha a tela da unidade de acordo com o menu a ser preenchido
     *
     * @return escreve a tela.
     *
     */
    public function desenharTela($menu)
    {
        $pastaDadosUnidade = 'par3/modulos/principal/planoTrabalho/dadosUnidade/';

        switch($menu){
        	case'prefeito':
        	    include APPRAIZ.$pastaDadosUnidade.'prefeito.php';
        	    break;
        	case'secretaria':
        	    include APPRAIZ.$pastaDadosUnidade.'secretaria.php';
        	    break;
        	case 'prefeitura':
        		include APPRAIZ.$pastaDadosUnidade.'prefeitura.php';
        		break;
        	case 'dirigente':
        		include APPRAIZ.$pastaDadosUnidade.'dirigente.php';
        		break;
        	case 'equipe':
        		include APPRAIZ.$pastaDadosUnidade.'equipe.php';
        		break;
            case 'nutricionista':
                include APPRAIZ.$pastaDadosUnidade.'nutricionista.php';
                break;
            case 'comite':
              	include APPRAIZ.$pastaDadosUnidade.'cacs.php';
              	break;
            case 'conselho':
            	include APPRAIZ.$pastaDadosUnidade.'conselho.php';
              	break;
        	default:
        	    include APPRAIZ.$pastaDadosUnidade.'prefeitura.php';
        	    break;
        }

    }//end desenharTela();

}//end class

?>