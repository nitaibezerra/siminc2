<?PHP

    function alinhaParaEsquerda($valor){
        $valor = "<p style=\"text-align: justify !important;\">$valor</p>";
        return $valor;
    }

    /**
    * Importante: Qdo utilizar arquivos inclusos, as variáveis globais devem ser declaradas para uso, ou acessadas
    * através da variável $_GLOBALS.
    *
    * @param type $titulo
    * @param type $id
    * @param type $content
    * @param array $botoes
    * @param array $opcoes
    */
    function bootstrapPopup($titulo, $id, $content, array $botoes = array(), array $opcoes = array()){
       $tamanhoModal = '';
       
        if (isset($opcoes['tamanho'])) {
            $tamanhoModal = "modal-{$opcoes['tamanho']}";
            echo <<<CSS
             <style type="text/css">
                 .modal-lg{
                     width:70%!important
                 }
             </style>
CSS;
        }

        echo <<<HTML
            <div class="modal fade" id="{$id}" style="z-index:999999999;">
                <div class="modal-dialog {$tamanhoModal}">
                    <div class="modal-content">
                        <div class="modal-header bs-example-modal-lg">
                            <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                            <h4 class="modal-title">{$titulo}</h4>
                        </div>
                        <div class="modal-body">
HTML;
        if (is_file($content)) {
            require_once $content;
        } else {
            echo $content;
        }

        echo <<<HTML
            </div>
            <div class="modal-footer">
HTML;
        foreach ($botoes as $botao) {
            switch ($botao) {
                case 'cancelar':
                    echo <<<HTML
                        <button type="button" class="btn btn-default" data-dismiss="modal">Cancelar</button>
HTML;
                    break;
                case 'confirmar':
                    break;
                case 'salvar':
                    $label = ucfirst($botao);
                    echo <<<HTML
                        <button type="button" class="btn btn-primary btn-{$botao}">{$label}</button>
HTML;
                    break;
                case 'fechar':
                    echo <<<HTML
                        <button type="button" class="btn btn-danger btn-fechar" data-dismiss="modal">Fechar</button>
HTML;
                    break;
                default:
                    break;
           }
       }
       echo <<<HTML
                    </div>
                </div>
            </div>
        </div>
HTML;
       if (in_array('confirmar', $botoes)){
           echo <<<JAVASCRIPT
                <script type="text/javascript">
                    $('#{$id} .btn-confirmar').click(function(){
                        bootbox.confirm('Tem certeza que deseja confirmar as alterações?', function(confirm){
                            confirm && $('#{$id} form').submit();
                        });
                    });
                </script>
JAVASCRIPT;
       }
   }

    function infoTarefas( $dados ){
        require_once APPRAIZ . 'includes/library/simec/Listagem.php';

        $ptaid = $dados['ptaid'];

        #GERA LISTAGEM DAS TAREFAS. EMCAMINHAMENTO.
        $sql = "
            SELECT  t.trfid,
                    t.trfdsc,
                    usunome,
                    CASE
                        WHEN (t.trfprazo::date - NOW()::date ) = '3' THEN '<p style=\"color:#1E90FF !important;\">'|| to_char(t.trfprazo, 'DD/MM/YYYY') || '</p>'
                        WHEN (t.trfprazo::date - NOW()::date ) < '3' THEN '<p style=\"color:#FF0000 !important;\">'|| to_char(t.trfprazo, 'DD/MM/YYYY') || '</p>'
                        ELSE '<p style=\"color:#000000 !important;\">'||to_char(t.trfprazo, 'DD/MM/YYYY')||'</p>'
                    END AS trfprazo,
                    (t.trfprazo::date - NOW()::date ) AS DIAS

            FROM agendagm.tarefa AS t
            JOIN seguranca.usuario AS u ON u.usucpf = t.usucpf
            LEFT JOIN(
                SELECT DISTINCT trfid FROM agendagm.arquivotarefa
            ) AS a ON a.trfid = t.trfid

            WHERE t.ptaid = {$ptaid}
                
            ORDER BY 1
        ";
        $cabecalho = array("Código","Tarefa", "Responsável", "Prazo", "DIAS");

        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->turnOffForm();
        $html = $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

        bootstrapPopup('Listagem de Tarefas por Pauta', 'lista_tarefas_pauta', $html, array('fechar'), array('tamanho'=>'lg') );
        die();
    }
?>