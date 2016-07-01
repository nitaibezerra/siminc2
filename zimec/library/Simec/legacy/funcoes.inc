<?php
function getLinkManual($sisid)
{
    global $db;
    
    require_once APPRAIZ . "includes/arquivo.inc";
    
    $strSql = "select arqid
               from public.manual as manual
               WHERE manual.sisid = {$sisid} ";

    if (isset($_SESSION['sistemas']['manual'][$sisid]))
    {
    	$arrDadosManual = $_SESSION['sistemas']['manual'][$sisid];
    }
    else
    {
    	$arrDadosManual = $db->pegaLinha( $strSql );
    	$_SESSION['sistemas']['manual'][$sisid] = $arrDadosManual; 
    }
    
    $arrDadosManual = $db->pegaLinha( $strSql );

    $booArquivoExiste = existeArquivoFisico( $arrDadosManual["arqid"] );
    
    if( $booArquivoExiste && sizeof( $arrDadosManual ) > 0 && $arrDadosManual["arqid"] > 0 )
    {
        $intArqId = $arrDadosManual["arqid"];
        
        return '../mostra_arquivo.php?id='.$intArqId;
    }
    
    return null;
}

function alterarRaizZend($file)
{
    if(stripos($file, 'Zend/') === 0){
        $file = str_replace('Zend/', APPRAIZ_ZEND, $file);
    } elseif(stripos($file, 'Zend' . DIRECTORY_SEPARATOR) === 0){
        $file = str_replace('Zend' . DIRECTORY_SEPARATOR, APPRAIZ_ZEND, $file);
    }
    return $file;
}


function listagem($sql , $heads , $config = array())
{
    global $db;
    $default = array( 'page' => 1 , 'perPage' => 10, 'actions' => array() );

    foreach ($config as $keyConfig => $itemConfig) {
        if (isset($default[$keyConfig]))
            $default[$keyConfig] = $itemConfig;
    }

    $list = $db->carregar($sql);
?>
    <?php if($list):?>
        <table class="table table-striped table-bordered table-hover">
            <thead>
                <tr>
                    <th class="text-center" colspan="<?php echo count($default['actions']) ?>">Ação</th>
                    <!--<th>#</th>-->
                    <?php foreach ($heads as $head): ?>
                        <th class="text-center"><?php echo $head ?></th>
                    <?php endforeach; ?>
                    <!--<th class="text-center">Excluir</th>-->
                </tr>
            </thead>
            <tbody>
                <?php foreach ($list as $itemTrKey => $itemTr): $id = array_shift($itemTr);?>
                    <tr>
                        <?php foreach($config['actions'] as $actionKey => $action): ?>
                            <?php if($action): ?>
                                <td class="text-center">
                                    <?php             
                                        switch ($actionKey) {
                                            case 'edit':
                                                $icon = 'pencil';
                                                break;
                                            case 'delete':
                                                $icon = 'remove';
                                                break;
                                            default:
                                                $icon = '';
                                                break; 
                                        }
                                    ?>
                                    <a href="#" onclick="<?php echo $action . '(' . $id . ')' ?>">
                                        <i class="glyphicon glyphicon-<?php echo $icon ?>"></i>
                                    </a>
                                </td>
                            <?php endif ?>
                        <?php endforeach ?>
                        <?php foreach ($itemTr as $itemTdKey => $itemTd): ?>
                            <td>
                                <?php echo $itemTd ?>
                            </td>
                        <?php endforeach; ?>
                    </tr>
                <?php endforeach; ?>
            </tbody>
        </table>
<?php endif ?>
<?php
}

function inputText()
{
    
}

function exibirGraficoHistoricoWorkflow(){
    include_once APPRAIZ . "includes/library/simec/montarGraficoWorkflow.php";
}

function executarArquivosAssincrono($arUrls){
	
	$url_count = count($arUrls);
	$curl_array = array();
	
	$ch = curl_multi_init();
	
	foreach($arUrls as $count => $url) {
		$curl_array[$count] = curl_init($url);
		curl_setopt($curl_array[$count], CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($curl, CURLOPT_URL, $url);
		curl_setopt($curl, CURLOPT_HEADER, true);
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($curl, CURLOPT_FOLLOWLOCATION, true);
		
		//curl_setopt($curl_array[$count], CURLOPT_FRESH_CONNECT, 1);
		curl_multi_add_handle($ch, $curl_array[$count]);
	}
	
	do {
		curl_multi_exec($ch, $exec);
	} while($exec > 0);
	
	/* do {
	 $res = curl_multi_exec($ch, $active);
	} while ($res == CURLM_CALL_MULTI_PERFORM);*/
	
	$arRetorno = array();
	foreach($arUrls as $count => $url) {
		$returned = curl_multi_getcontent($curl_array[$count]);
		array_push($arRetorno, $url.' - '.$returned);
	}
	
	foreach($arUrls as $count => $url) {
		curl_multi_remove_handle($ch, $curl_array[$count]);
	}
	
	curl_multi_close($ch);
	
	foreach($arUrls as $count => $url) {
		curl_close($curl_array[$count]);
	}
	
	return $arRetorno;
}

?>