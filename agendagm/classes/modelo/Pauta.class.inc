<?php
/**
 * Classe de mapeamento da entidade agendagm.pauta.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade agendagm.pauta.
 *
 * @see Modelo
 */
class Agendagm_Model_Pauta extends Modelo
{
    /**
     * Nome da taAgendagm_Pautabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'agendagm.pauta';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'ptaid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ptaid' => null,
        'arqid' => null,
        'usucpf' => null,
        'ptadtlancamento' => null,
        'ptatema' => null,
        'ptasubtema' => null,
        'ptaparticipantes' => null,
        'ptadscata' => null,
        'ptajustificativa' => null,
        'ptaantecedentes' => null,
        'ptaorientaministro' => null,
        'ptaobservacao' => null,
        'ptadtinclusao' => null,
        'ptastatus' => null,
    );

    public function habilitaFormTela( $dados ){

        $ptaid = $dados['ptaid'];

        if( $ptaid > 0 ){
            $sql = "
                SELECT  p.ptadtlancamento,
                        p.ptatema,
                        p.ptasubtema,
                        p.ptaparticipantes,
                        u.ptaid
                FROM agendagm.pauta AS p
                JOIN(
                    SELECT DISTINCT ptaid FROM agendagm.usuarioassessor
                ) AS u ON u.ptaid = p.ptaid

                WHERE p.ptaid = {$ptaid}
            ";
            $result = $this->pegaLinha($sql);
        }

        if( $result['ptadtlancamento'] != '' && $result['ptatema'] != '' && $result['ptasubtema'] != '' ){
            echo 'true';
        }else{
            echo 'false';
        }
        die();
    }
    
    public function anexarArquivoPauta( $boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array() ){

        $ptaid          = $this->ptaid;
        $array_files    = $this->arqid;
        
        if( is_array($array_files) ){
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
            
            $file = new FilesSimec("pauta", array(), "agendagm");

            if( is_file($array_files['arquivos']['tmp_name']) ){
                $file->setUpload('Agenda GM - Anexo da Ata', 'arquivos', null, 'arqid');
            }
            $arqid = $file->getIdArquivo();
            
            if( $arqid > 0 ){
                if( $ptaid > 0 ){                    
                    $sql = "
                        UPDATE agendagm.pauta
                           SET arqid = {$arqid}
                        WHERE ptaid = {$ptaid} RETURNING ptaid;
                    ";
                    $result = $this->pegaUm($sql);
                }
            }
        }

        if( $result > 0 ){
            $this->commit();
            return true;
        }else{
            $this->rollback();
            return false;
        }
    }
    
    public function excluirAnexoPauta( $ptaid ){
        
        if( $ptaid > 0 ){
            $sql = "
                UPDATE agendagm.pauta SET arqid = NULL WHERE ptaid = {$ptaid} RETURNING ptaid
            ";
            $result = $this->pegaUm($sql);
        }

        if( $result > 0 ){
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            if( is_array( $arr_arqid ) ){
                $file = new FilesSimec("arquivotarefa", array(), "agendagm");

                foreach( $arr_arqid as $arqid ){
                    $resp = $file->excluiArquivoFisico($arqid);
                }
            }
            return true;
        }
    }


    
    public function listaAnexoPauta( $ptaid ){
        $ptaid = $ptaid == '' ? 'NULL' : $ptaid;
        
        $sql = "
            SELECT  p.arqid,
                    a.arqnome||'.'||arqextensao AS arquivo,
                    u.usunome,
                    to_char(a.arqdata, 'DD/MM/YYYY') AS arqdate

            FROM agendagm.pauta AS p

            JOIN public.arquivo AS a ON a.arqid = p.arqid
            JOIN seguranca.usuario AS u ON u.usucpf = a.usucpf

            WHERE p.ptaid = {$ptaid}
        ";
        $cabecalho = array("Arquivo", "Responsável", "Inclusão");

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addAcao('delete', 'excluirAnexoPauta');
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
    
    public function listaGridBuscaPauta(){
        
    }
    
    public function verificaAxenoHabilita( $ptaid ){
        
        if( $ptaid > 0 ){
            $sql = "SELECT arqid FROM agendagm.pauta WHERE ptaid = {$ptaid}";
            $resp = $this->pegaUm($sql);

            if( $resp > 0 ){
                return 'disabled';
            }else{
                return '';
            }
        }      
    }
    
    public function listagemGridPautas( $dados ){
        #FILTROS DE PESQUISA
        if( $dados['ptatema'] != '' ){
            $_WHERE[] = "ptatema ilike ('%{$dados['ptatema']}%')";
        }

        if( $dados['ptasubtema'] != '' ){
            $_WHERE[] = "ptasubtema ilike ('%{$dados['ptasubtema']}%')";
        }

        if( $dados['ptadtlancamento_ini'] != '' || $dados['ptadtlancamento_fim'] != ''){
            if( $dados['ptadtlancamento_ini'] != '' ){
                $ptadtlancamento_ini = formata_data( $dados['ptadtlancamento_ini'] );
            }
            if( $dados['ptadtlancamento_fim'] != '' ){
                $ptadtlancamento_fim = formata_data( $dados['ptadtlancamento_fim'] );
            }

            $_WHERE[] = "( ptadtlancamento BETWEEN '{$ptadtlancamento_ini}' AND '{$ptadtlancamento_fim}' )";
        }

        if( $_REQUEST['usucpf'] != '' ){
            $_WHERE[] = "p.usucpf = '{$_REQUEST['usucpf']}'";
        }

        if( $dados['trfprazo'] != '' ){
            switch( $dados['trfprazo'] ){
                case 1:
                    $JOIN = "JOIN agendagm.tarefa AS t ON t.ptaid = p.ptaid";
                    $_WHERE[] = "( (t.trfprazo::date - NOW()::date ) = '3' )";
                    break;
                case 2:
                    $JOIN = "JOIN agendagm.tarefa AS t ON t.ptaid = p.ptaid";
                    $_WHERE[] = "( (t.trfprazo::date - NOW()::date ) < '3' )";
                    break;
                case 3:
                    $JOIN = "JOIN agendagm.tarefa AS t ON t.ptaid = p.ptaid";
                    $_WHERE[] = "( (t.trfprazo::date - NOW()::date ) <= '0' )";
                    break;
            }
        }

        if( $_WHERE[0] != '' ){
            $_WHERE = 'WHERE '.implode($_WHERE, ' AND ');
        }

        $icone = "<img src=\"/imagens/clipe.gif\" title=\"Possui Anexo\">";

        #GERA LISTAGEM DAS TAREFAS. EMCAMINHAMENTO.
        $sql_l = "
            SELECT  p.ptaid,
                    ptatema,
                    ptasubtema,
                    to_char(ptadtinclusao, 'DD/MM/YYYY') AS ptadtinclusao,
                    to_char(ptadtlancamento, 'DD/MM/YYYY') AS ptadtlancamento,
                    u.usunome,

                    CASE WHEN arqid IS NOT NULL
                        THEN '{$icone}'
                        ELSE ''
                    END arqid
            FROM agendagm.pauta AS p
            {$JOIN}
            JOIN seguranca.usuario AS u ON u.usucpf = p.usucpf

            {$_WHERE}

            ORDER BY 1
        ";
        $cabecalho = array("Tema", "Subtema", "Data", "Data de Inclusão", "Acessores", "Anexos");

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql_l);
        $listagem->addCallbackDeCampo('ptadscata', 'alinhaParaEsquerda');
        $listagem->addCallbackDeCampo('ptatema', 'alinhaParaEsquerda');
        $listagem->addCallbackDeCampo('ptasubtema', 'alinhaParaEsquerda');
        $listagem->addAcao('edit', 'editarAgendaPauta');
        $listagem->addAcao('delete', 'deletar');
        $listagem->addAcao('info', 'infoTarefas');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);        
    }
}