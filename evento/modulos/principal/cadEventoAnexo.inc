<?PHP

    $docid = evtCriarDoc();

    if($_REQUEST['download'] == 'S'){
            /*
        $param = array();
        $param["arqid"] = $_REQUEST['arqid'];
        DownloadArquivo($param);
        exit;
        */
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $arqid = $_REQUEST['arqid'];
        $file = new FilesSimec();

        $arquivo = $file->getDownloadArquivo($arqid);
        echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
        exit;
    }

    #BUSCA DADOS CO EVENTO NECESSARIO PARA O FUNCIONAMENTO DA TELA.
    if( $_SESSION['evento']['eveid'] != ''){

        $eveid = $_SESSION['evento']['eveid'];

        $rsDadosEvento = buscaDadosCadEvento( $eveid );
    }
    
    //insere dados do orçamento resultado
    if($_REQUEST['salvar'] == '1'){
        if($_FILES['arquivo']['error'] == 0) {
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            $campos = array(
                "eveid"             => $_SESSION['evento']['eveid'],
                "tpaid"             => $_REQUEST["tpaid"],
                "usucpf"            => "'{$_SESSION['usucpf']}'",
                "axedatainclusao"   => "NOW()",
                "axestatus"         => "'A'"
            );
            $file = new FilesSimec("anexoevento", $campos ,"evento");
            $file->setUpload( (($_REQUEST['arqdescricao']) ? $_REQUEST['arqdescricao'] : NULL ), $key  = "arquivo" );

            echo"<script> alert('Operação realizada com sucesso!');</script>";
            echo"<script>window.location.href = 'evento.php?modulo=principal/cadEventoAnexo&acao=A';</script>";
            exit;
        }
    }

    if($_REQUEST['arqidDel'] != ''){
        //deleta arquivo
        $arquivo = $db->pegaLinha("select arqid, axpid from evento.anexoevento where arqid = ". $_REQUEST['arqidDel']);
        if($arquivo['arqid']){
                include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
                $campos	= array("axpid" => $arquivo['axpid']);
                $file   = new FilesSimec("anexoevento", $campos, "evento");
                // Remove o registro do anexo antigo (pelo arqid) e o arquivo físico
                $file->setRemoveUpload( $arquivo['arqid'] );
        }
        echo"<script>window.location.href = 'evento.php?modulo=principal/cadEventoAnexo&acao=A';</script>";
    }

    //evento.php?modulo=principal/cadEventos&acao=A
    $titulo_modulo = "Eventos";
    //Chamada de programa

    include  APPRAIZ."includes/cabecalho.inc";
    monta_titulo( $titulo_modulo, 'Solicitar Pré-agendamento de eventos.' );
    
    $perfis = arrayPerfil();

    if(in_array(PERFIL_EMPRESA, $perfis) && count($perfis) == 1){
       $res = array();
    }else{
        $res = array(
                0 => array ( "descricao" => "Lista", "id" => "4", "link" => "/evento/evento.php?modulo=inicio&acao=C&submod=evento" ),
                1 => array ( "descricao" => "Informações Básicas", "id" => "4", "link" => "/evento/evento.php?modulo=principal/cadEvento&acao=A" )
        );
    }
    if( ( $rsDadosEvento['sevid'] != 1) AND ( $rsDadosEvento['sevid'] != '') ){
        array_push(
            $res,
            array ( "descricao" => "Documentos Anexos", "id" => "3", "link" => "/evento/evento.php?modulo=principal/cadEventoAnexo&acao=A" ),
            array ("descricao" => "Infraestrutura", "id" => "2", "link" => "/evento/evento.php?modulo=principal/cadEventoInfra&acao=A" )
        );
        $pflcod = pegaPerfil( $_SESSION['usucpf'] );
    }

    if(mostraAbaDocOS($_SESSION['evento']['eveid'])){
        array_push(
            $res,
            array ("descricao" => "Ordem de Serviço", "id" => "7", "link" => "/evento/evento.php?modulo=principal/cadOrdemServico&acao=A" )
        );
    }

    if(mostraAbaDocPagamento($_SESSION['evento']['eveid'])){
        array_push(
            $res,
            array( "descricao" => "Documento de Pagamento", "id" => "5", "link" => "/evento/evento.php?modulo=principal/cadDocPagamento&acao=A" )
        );
    }

    if( ( $rsDadosEvento['sevid'] != 1) AND ( $rsDadosEvento['sevid'] != '') ){
            array_push(
                $res,
                array ("descricao" => "Avaliação", "id" => "0", "link" => "/evento/evento.php?modulo=principal/avaliacaoEvento&acao=A" )
            );
    }

    echo montarAbasArray($res, $_REQUEST['org'] ? false : "/evento/evento.php?modulo=principal/cadEventoAnexo&acao=A");

    headEvento($rsDadosEvento['evetitulo'],$rsDadosEvento['everespnome'], $rsDadosEvento['ungdsc'], $rsDadosEvento['ungcod'], $rsDadosEvento['eveurgente'], $rsDadosEvento['evedatainclusao'], false);

    echo "<br>";

//verifica permissão edição
//$eventoPermissaoEdicao = eventoPermissaoEdicao();
$eventoPermissaoEdicao = '';

?>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <form name=formulario id=formulario method=post enctype="multipart/form-data">
        <input type="hidden" name="salvar" value="0">
        <div style="position:absolute; right:0px;top:80px;"></div>

        <table class="tabela listagem" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
            <tr><td colspan="3">&nbsp;</td></tr>
            <tr>
                <td class="SubTituloDireita"> Arquivo: </td>
                <td width='50%'>
                    <input type="file" name="arquivo" id="arquivo"/>
                </td>
                <td rowspan="3" width="5%" style="text-align: center;">
                    <?PHP wf_desenhaBarraNavegacao($docid, array('' => '')); ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Tipo:</td>
                <td>
                    <?PHP
                        if ($pflcod == EVENTO_PERFIL_PERFIL_EMPRESA) {
                            $sql = "SELECT tpaid AS codigo, tpadescricao AS descricao FROM evento.tipoanexo WHERE tpaid = 5";
                        } else {
                            $sql = "SELECT tpaid AS codigo, tpadescricao AS descricao FROM evento.tipoanexo";
                        }

                        $db->monta_combo('tpaid', $sql, 'S', "Selecione...", '', '', '', '355', 'S', 'tpaid');
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
                <td><?= campo_textarea('arqdescricao', 'S', 'S', '', 60, 4, 250); ?></td>
            </tr>
        </table>

        <table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
            <tr>
                <td class="SubTituloCentro">
                    <input type="button" name="botao" value="Salvar" onclick="enviar()">
                </td>
            </tr>
        </table>
    </form>
              
    <?PHP
//        $acao = "'<center><a href=\"#\" onclick=\"javascript:excluirAnexo(' || arq.arqid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>'";
//        if ($eventoPermissaoEdicao)
//            $acao = "''";

        if ($_SESSION['evento']['eveid']) {
            $sql = "
                SELECT  to_char(arq.arqdata,'DD/MM/YYYY'),
                        tarq.tpadescricao,
                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadEventoAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
                        arq.arqtamanho || ' kbs' as tamanho,
                        arq.arqdescricao,
                        usu.usunome
                FROM public.arquivo arq
                
                INNER JOIN evento.anexoevento aqb ON arq.arqid = aqb.arqid
                INNER JOIN evento.tipoanexo tarq ON tarq.tpaid = aqb.tpaid
                INNER JOIN seguranca.usuario usu ON usu.usucpf = arq.usucpf
                
                WHERE arq.arqstatus = 'A' AND aqb.eveid = {$_SESSION['evento']['eveid']}
            ";
        } else {
            $sql = array();
        }

        $cabecalho = array("Data Inclusão", "Tipo Arquivo", "Nome Arquivo", "Tamanho (Mb)", "Descrição Arquivo", "Responsável");
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '');
    ?>
 
    <script language="javascript" type="text/javascript">
	function cadastrar_anexo(){
		if ( validar_formulario_anexo() ) {
			document.anexo.submit();
		}
	}

	function enviar()
	{
		var arq   = document.getElementById('arquivo');
		var tpaid = document.getElementById('tpaid');
		if( tpaid.value == '' )
		{
			alert('É necessário informar um tipo de documento.');
			return false;
		}
		if( arq.value == '' )
		{
			alert('É necessário anexar um arquivo.');
			return false;
		}

		//location.href= window.location+'&salvar=1';
		document.formulario.salvar.value=1;
		document.formulario.submit();
	}

	function validar_formulario_anexo(){
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		//document.anexo.taaid.value = trim( document.anexo.taaid.value );
		document.anexo.anedescricao.value = trim( document.anexo.anedescricao.value );
		/*
		if ( document.anexo.taaid.value == '' ) {
			mensagem += '\nTipo';
			validacao = false;
		}
		*/
		if ( document.anexo.anedescricao.value == '' ) {
			mensagem += '\nDescrição';
			validacao = false;
		}
		if ( document.anexo.arquivo.value == '' ) {
			mensagem += '\nArquivo';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

	function excluirAnexo( arqid ){
		if ( confirm( 'Deseja excluir o Documento?' ) ) {
			location.href= window.location+'&arqidDel='+arqid;
		}
	}

	function cadastrar_versao( formulario ){
		if ( formulario.arquivo.value == '' ) {
			return;
		}
		formulario.submit();
	}

	function ltrim( value ){
		var re = /\s*((\S+\s*)*)/;
		return value.replace(re, "$1");
	}

	function rtrim( value ){
		var re = /((\s*\S+)*)\s*/;
		return value.replace(re, "$1");
	}

	function trim( value ){
		return ltrim(rtrim(value));
	}
</script>

<?PHP
//verifica permissao edição
echo $eventoPermissaoEdicao;
?>