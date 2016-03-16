<?php
/**
 * Implementa uma versão simplificada do zend flash message.
 * $Id: FlashMessage.php 97955 2015-05-28 18:07:18Z maykelbraz $
 */

/**
 * Utilize para armazenar mensagens de aviso, sucesso, informações
 * ou erros e exibí-las posteriamente. Assim que uma mensagem é
 * exibida, ela é removida do armazenamento.
 * Depois de instanciar a classe definindo o identificador de onde
 * sua mensagem será armazenada, basta chamar addMensagem para
 * adicionar uma nova mensagem. Quando for exibir as mensagens,
 * basta chamar getMensagens, que elas serão retornadas já formatadas.
 *
 * Exemplo de adição de mensagem:
 * $fm = new Simec_Helper_FlashMessage('elabrev/tc/formnc');
 * $fm->addMensagem(
 *      'Não foi possível processar sua requisição.',
 *      Simec_Helper_FlashMessage::ERRO
 * );
 *
 * Exemplo de exibição de mensagem:
 * $fm = new Simec_Helper_FlashMessage('elabrev/tc/formnc');
 * echo $fm->getMensagens();
 *
 */
class Simec_Helper_FlashMessage
{
    /**
     * Use para criar uma mensagem de sucesso. Balão verde.
     * Este é o tipo de mensagem padrão.
     */
    const SUCESSO = 'success';
    /**
     * Use para criar uma mensagem de informação. Balão azul.
     */
    const INFO = 'info';
    /**
     * Use para criar uma mensagem de aviso. Balão amarelo.
     */
    const AVISO = 'warning';
    /**
     * Use para criar uma mensagem de erro. Balão vermelho.
     */
    const ERRO = 'danger';

    /**
     * Identificador de armazenamento das mensagens na sessão.
     * Será cria na variável de sessão uma entrada do tipo:
     * $_SESSION[$identificador]['msg'], onde as mensagens
     * serão armazenadas.
     * @var string
     */
    protected $identificador;

    /**
     * Cria uma instância de gerenciamento de mensagens com
     * um identificador próprio. É recomendado a utilização
     * de identificadores compostos para evitar conflitos
     * com outras áreas da sessão.
     * Exemplo: "elabrev/termocooperacao".
     *
     * @param string $identificador
     *          Identificador de armazenamento das mensagens.
     */
    public function __construct($identificador)
    {
        if (empty($identificador)) {
            trigger_error('O identificador não pode ser vazio.');
        }
        $this->identificador = $identificador;

        if (!isset($_SESSION[$this->identificador]['msg'])) {
            $_SESSION[$this->identificador]['msg'];
        }
    }

    /**
     * Adiciona uma nova mensagem à lista de mensagens do identificador.
     * Mais de uma mensagem pode ser adicionada.
     * @param string $mensagem O texto a mensagem a ser incluída na lista.
     * @param string $tipo O Tipo da mensagem a ser incluída.
     * @see Simec_Helper_FlashMessage::SUCESSO
     * @see Simec_Helper_FlashMessage::INFO
     * @see Simec_Helper_FlashMessage::AVISO
     * @see Simec_Helper_FlashMessage::ERRO
     */
    public function addMensagem($mensagem, $tipo = self::SUCESSO)
    {
        if (empty($mensagem)) {
            trigger_error('A mensagem não pode ser vazia.');
        }
        $_SESSION[$this->identificador]['msg'][] = array(
            'tipo' => $tipo,
            'texto' => $mensagem
        );
    }

    /**
     * Retorna um HTML com todas as mensagens armazenadas e
     * as remove da lista do identificador.
     * @return string
     */
    public function getMensagens()
    {

        $msgs = $_SESSION[$this->identificador];

        if (!isset($msgs['msg'])
            || empty($msgs['msg'])
            || !is_array($msgs['msg'])) {
            return;
        }

        // -- imprimindo mensagens
        $html = '';
        foreach ($msgs['msg'] as $msg) {
            $html .= <<<DML
<div class="alert alert-{$msg['tipo']} text-center col-md-8 col-md-offset-2">
    <button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
{$msg['texto']}
</div>
<br style="clear:both" />
DML;
        }

        // -- Limpando o identificador
        unset($_SESSION[$this->identificador]);
        return $html;
    }

    public function __toString()
    {
        $retorno = $this->getMensagens();
        return $retorno?$retorno:'';
    }
}
